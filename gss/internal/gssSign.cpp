///////////////////////////////////////////////////////////////
//
// gssSign: Signature Scan Service
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   26.08.2014 23:30 - created (moved out from gssMain)
//   27.08.2014 00:53 - fixed siScan incorrect-last-byte-scan bug
//   25.11.2014 20:45 - updated siInitHdr to support one-big-array signatures
//   29.11.2016 22:33 - moved TailHook to gtaScan
//


#include <ulib/uDef.h>
#include <ulib/tunic.h>
#include <ulib/uLog.h>
#include <ulib/uMemory.h>

#include "gssSign.h"




void signInit(BYTE* pSignData, BYTE* pMaskData, SIOFFSET* pOffsetData, SIHDR* pSignHeaders, DWORD numSignatures)
{
    DWORD iSignature;
    DWORD iSignPos = 0;
    DWORD iOffsetPos = 0;
    for(iSignature = 0; iSignature < numSignatures; iSignature++)
    {
        // init header
        SIHDR* phdr = &pSignHeaders[iSignature];
        phdr->pSignature = &pSignData[iSignPos];
        phdr->pMask = &pMaskData[iSignPos];
        phdr->pOffset = &pOffsetData[iOffsetPos];
        // fill arrays
        DWORD iSignatureByte;
        DWORD iSignatureByteStop;
        DWORD iOffset;
        DWORD iOffsetStop;
        DWORD offsetBytesRemain;
        BYTE* pMask;
        BYTE byMask;
        pMask = phdr->pMask;
        iSignatureByteStop = phdr->codeLen;
        iOffsetStop = phdr->numOffsets;
        offsetBytesRemain = 0;
        for(iSignatureByte = 0; iSignatureByte < iSignatureByteStop; iSignatureByte++)
        {
            for(iOffset = 0; iOffset < iOffsetStop; iOffset++)
            {
                // is it offset first byte?
                if(phdr->pOffset[iOffset].codePos == iSignatureByte)
                {
                    offsetBytesRemain = phdr->pOffset[iOffset].type & SI_SIZEMASK;
                    break;
                }
            }
            byMask = 0xFF;
            if(offsetBytesRemain > 0)
            {
                offsetBytesRemain--;
                byMask = 0x00;
            }
            pMask[iSignatureByte] = byMask;
        }
        // next signature
        iSignPos += phdr->codeLen;
        iOffsetPos += phdr->numOffsets;
    }
}

BOOL signCheck(SIHDR* pSignHeaders, DWORD* pSignIndices, DWORD numSignIndices)
{
    BOOL bSuccess = TRUE;
    SIHDR* phdr;
    for(DWORD iIndex = 0; iIndex < numSignIndices; iIndex++)
    {
        phdr = &pSignHeaders[pSignIndices[iIndex]];
        bSuccess &= (phdr->numMatchLocs == phdr->numRequiredLocs);
    }
    return bSuccess;
}

BYTE* signGetOffsetData(SIHDR* pSignHeaders, SIDATAPTR* pData)
{
    BYTE* ptr;

  //  lss << UL::DEBUG << L("getOffset: iSign ") << pData->signatureIndex << L(", iLoc ") << pData->locationIndex << L(", iOff ") << pData->offsetIndex << UL::ENDL;

    switch(pData->type)
    {
    case SIDP_PLAIN:
        ptr = pSignHeaders[pData->signatureIndex].matchLocs[pData->locationIndex];
  //      lss << UL::DEBUG << L("getOffset: ptr plain: ") << ulhex(ptr) << UL::ENDL;
        break;

    case SIDP_OFFSET:
        BYTE* pbyAddr;
        DWBYTE dbValue;
        DWORD offsetSize;
        SIHDR* pSign;
        SIOFFSET* pOffset;

        dbValue.dword = 0;
        pSign = &pSignHeaders[pData->signatureIndex];
        pOffset = &pSign->pOffset[pData->offsetIndex];
        offsetSize = pOffset->type & SI_SIZEMASK;
  //      lss << UL::DEBUG << L("getOffset: match loc: ") << ulhex(pSign->matchLocs[pData->locationIndex]) << UL::ENDL;
  //      lss << UL::DEBUG << L("getOffset: offset: ") << pOffset->codePos << L(" <") << ulhex(pOffset->type) << L(">") << UL::ENDL;
        pbyAddr = pSign->matchLocs[pData->locationIndex] + pOffset->codePos;
  //      lss << UL::DEBUG << L("getOffset: match+offset: ") << ulhex(pbyAddr) << UL::ENDL;

        // read byte / address / displacement
        for(DWORD iByte = 0; iByte < offsetSize; iByte++)
        {
            dbValue.byte[iByte] = pbyAddr[iByte];
        }
  //      lss << UL::DEBUG << L("getOffset: read: ") << ulhex(dbValue.dword) << UL::ENDL;

        switch(pOffset->type)
        {
        case SI_DISPLACEMENT:
            // use read value as signed displacement from matched address
            dbValue.pby = pbyAddr + SI_DISPLACEMENT_SHIFT + dbValue.sint;
            break;
        case SI_BYTE:
        case SI_DWORD:
        case SI_ADDRESS:
            // do-nothing
            break;
        }
        ptr = dbValue.pby;
  //      lss << UL::DEBUG << L("getOffset: ptr offset: ") << ulhex(ptr) << UL::ENDL;
        break;
    }

    return ptr;
}


void signScan(BYTE* pMemStart, BYTE* pMemStop, SIHDR* pSignHeaders, DWORD numSignatures)
{
    // scan memory
    DWORD iSignature;
    BYTE* pbyAddr;
    BYTE* pbyMatchAddr;
    BYTE bySign;
    BYTE byData;
    BOOL bScanning;
    DWORD siCodePos;
    bScanning = TRUE;
    for(pbyAddr = pMemStart; pbyAddr < pMemStop; pbyAddr += 1)
    {
        // skip unreadable regions
        if(IsReadableMemory(pbyAddr) == FALSE)
        {
            lss << UL::INFO << L("mem.drop: ") << ulhex(pbyAddr) << UL::ENDL;
            while(IsNextMemoryRegionReadable(&pbyAddr) == FALSE)
            {
                lss << UL::INFO << L("mem.next: ") << ulhex(pbyAddr) << UL::ENDL;
                // reset all scanning (if any)
                if(bScanning != FALSE)
                {
                    bScanning = FALSE;
                    for(iSignature = 0; iSignature < numSignatures; iSignature++)
                    {
                        pSignHeaders[iSignature].codePos = 0;
                    }
                }
            }
            lss << UL::INFO << L("mem.read: ") << ulhex(pbyAddr) << UL::ENDL;
            // do not cross end boundary
            if(pbyAddr >= pMemStop)
            {
                break;
            }
        }
        // test all signatures simultaneously
        for(iSignature = 0; iSignature < numSignatures; iSignature++)
        {
            // test current byte
            SIHDR* pSI = &pSignHeaders[iSignature];
            siCodePos = pSI->codePos;
            byData = *pbyAddr & pSI->pMask[siCodePos];
            bySign = pSI->pSignature[siCodePos] & pSI->pMask[siCodePos];
            if(byData == bySign)
            {
                pSI->codePos++;
                // we have a match, is it last byte?
                if(pSI->codePos == pSI->codeLen)
                {
                    // it is!
                    pbyMatchAddr = (pbyAddr + 1) - pSI->codeLen;
                    lss << UL::INFO << ulpad(3, iSignature) << L(":") << ulhex(pbyMatchAddr) << UL::ENDL;
                    pSI->codePos = 0;
                    if(pSI->numMatchLocs < SI_MAX_MATCH_LOCS)
                    {
                        pSI->matchLocs[pSI->numMatchLocs] = pbyMatchAddr;
                        pSI->numMatchLocs++;
                    }
                }
            }
            else
            {
                // no match, reset scan
                pSI->codePos = 0;
            }
        }
        // proceed to next byte (if any)
    }
}

