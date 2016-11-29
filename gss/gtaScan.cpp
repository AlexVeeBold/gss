///////////////////////////////////////////////////////////////
//
// gtaScan: memory scanner / injector
//
// Copyright © 2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   29.11.2016 22:18 - created (moved out from gssInterface)
//

#include "uDef.h"
#include "tunic.h"
#include "tstr.h"
#include "uLog.h"
#include "uMemory.h"

#include "gssDef.h"
#include "gssSign.h"
#include "gssInterface.h"

#include "gtaVersion.h"

#include "gtaScanData.h"




enum GTA30CODEENTRYID {         // order must be the same as in following arrays
    SCEI_G30_VEHICLE_POOL,
    SCEI_G30_VEHPOOL_GET_AT,
    SCEI_G30_CURRENT_TEXTINFO,
    SCEI_G30_TEXTINFO_POOL,
    SCEI_G30_THESCRIPTS_PROCESS,
    SCEI_G30_THESCRIPTS_INIT,  //plain
    SCEI_G30_SCRIPT_DATA_BUFFER,
    SCEI_G30_SCRIPT_PROCESS_ONE_COMMAND,
    SCEI_G30_GAME_TIME,
    SCEI_G30_VEH_NEXT_VARIATION1,
    SCEI_G30_VEH_NEXT_VARIATION2,
    SCEI_G30_MARKER_PLACE_SET,  //plain
    SCEI_G30_MARKER_PLACE_ONE,

    // counter (must be the last one)
    NUM_GTA30_CODE_ENTRIES
};
SIDATAPTR G3CodeEntries[NUM_GTA30_CODE_ENTRIES] = {
    {SIGN_G3_Opcode0137, 0, SIDP_OFFSET, SIDX_G3_0137_VEHICLE_POOL},
    {SIGN_G3_Opcode0137, 0, SIDP_OFFSET, SIDX_G3_0137_VEHPOOL_GET_AT},
    {SIGN_G3_Opcode033F, 0, SIDP_OFFSET, SIDX_G3_033F_CURRENT_TEXTINFO},
    {SIGN_G3_Opcode033F, 0, SIDP_OFFSET, SIDX_G3_033F_TEXTINFO_POOL},
    {SIGN_G3_GameProcess, 0, SIDP_OFFSET, SIDX_G3_GP_THESCRIPTS_PROCESS},
    {SIGN_G3_TheScriptsInit, 0, SIDP_PLAIN, 0},
    {SIGN_G3_TheScriptsInit, 0, SIDP_OFFSET, SIDX_G3_TSI_SCRIPT_DATA_BUFFER},
    {SIGN_G3_RunningScriptProcess, 0, SIDP_OFFSET, SIDX_G3_RSP_PROCESS_ONE_COMMAND},
    {SIGN_G3_RunningScriptProcess, 0, SIDP_OFFSET, SIDX_G3_RSP_GAME_TIME},
    {SIGN_G3VC_SetNextVehicleVariation, 0, SIDP_OFFSET, SIDX_G3VC_SNVV_NEXT_VEH_VARIATION1},
    {SIGN_G3VC_SetNextVehicleVariation, 0, SIDP_OFFSET, SIDX_G3VC_SNVV_NEXT_VEH_VARIATION2},
    {SIGN_G3VC_PlaceMarkerSet, 0, SIDP_PLAIN, 0},
    {SIGN_G3VC_PlaceMarkerSet, 0, SIDP_OFFSET, SIDX_G3VC_PMS_PLACE_MARKER},
};
SIDATAPTR VCCodeEntries[NUM_GTA30_CODE_ENTRIES] = {
    {SIGN_VC_Opcode0137, 0, SIDP_OFFSET, SIDX_VC_0137_VEHICLE_POOL},
    {SIGN_VC_Opcode0137, 0, SIDP_OFFSET, SIDX_VC_0137_VEHPOOL_GET_AT},
    {SIGN_VC_Opcode033F, 0, SIDP_OFFSET, SIDX_VC_033F_CURRENT_TEXTINFO},
    {SIGN_VC_Opcode033F, 0, SIDP_OFFSET, SIDX_VC_033F_TEXTINFO_POOL},
    {SIGN_VC_GameProcess, 0, SIDP_OFFSET, SIDX_VC_GP_THESCRIPTS_PROCESS},
    {SIGN_VC_TheScriptsInit, 0, SIDP_PLAIN, 0},
    {SIGN_VC_TheScriptsInit, 0, SIDP_OFFSET, SIDX_VC_TSI_SCRIPT_DATA_BUFFER},
    {SIGN_VC_RunningScriptProcess, 0, SIDP_OFFSET, SIDX_VC_RSP_PROCESS_ONE_COMMAND},
    {SIGN_VC_RunningScriptProcess, 0, SIDP_OFFSET, SIDX_VC_RSP_GAME_TIME},
    {SIGN_G3VC_SetNextVehicleVariation, 0, SIDP_OFFSET, SIDX_G3VC_SNVV_NEXT_VEH_VARIATION1},
    {SIGN_G3VC_SetNextVehicleVariation, 0, SIDP_OFFSET, SIDX_G3VC_SNVV_NEXT_VEH_VARIATION2},
    {SIGN_G3VC_PlaceMarkerSet, 0, SIDP_PLAIN, 0},
    {SIGN_G3VC_PlaceMarkerSet, 0, SIDP_OFFSET, SIDX_G3VC_PMS_PLACE_MARKER},
};


enum GTA30SIGNATUREID {         // order must be the same as in following arrays
    SIGI_G30_Opcode0137,
    SIGI_G30_Opcode033F,
    SIGI_G30_RunningScriptProcess,
    SIGI_G30_GameProcess,
    SIGI_G30_TheScriptsInit,
    SIGI_G30_SetNextVehicleVariation,
    SIGI_G30_PlaceMarkerSet,

    // counter (must be the last one)
    NUM_G30_SIGNINDICES
};
DWORD G3SignIndices[NUM_G30_SIGNINDICES] = {
    SIGN_G3_Opcode0137,
    SIGN_G3_Opcode033F,
    SIGN_G3_RunningScriptProcess,
    SIGN_G3_GameProcess,
    SIGN_G3_TheScriptsInit,
    SIGN_G3VC_SetNextVehicleVariation,
    SIGN_G3VC_PlaceMarkerSet,
};
DWORD VCSignIndices[NUM_G30_SIGNINDICES] = {
    SIGN_VC_Opcode0137,
    SIGN_VC_Opcode033F,
    SIGN_VC_RunningScriptProcess,
    SIGN_VC_GameProcess,
    SIGN_VC_TheScriptsInit,
    SIGN_G3VC_SetNextVehicleVariation,
    SIGN_G3VC_PlaceMarkerSet,
};




#define INJ_DEBUG

#define SI_DEBUG

#ifdef INJ_DEBUG
#define INJDBG(x)    x;
#else
#define INJDBG(x)    // nothing
#endif

#ifdef SI_DEBUG
#define SIDBG(x)    x;
#else
#define SIDBG(x)    // nothing
#endif


//////// //////// //////// //////// Tail hook //////// //////// //////// //////// 

// run-time memory patcher
#define RMP_DEBUG

void WriteTailCallHook(BYTE* pbyStart, SITH* psith, void* pfnSetTo)
{
    const DWORD chunkSize = 8;
    const DWORD callSize = 5;   // E9 xx xx xx xx (actually it's a "jump")
    DWORD dwMemMode = 0;
    BOOL bSetAllAccess;
    DWORD iByte;
    BYTE byCode[chunkSize];
    DWBYTE dbCallDisp;
    BYTE* pbyDest;
    BYTE* pbySign;
    DWORD callOffset;

    pbyDest = pbyStart + psith->tailOffset;
    pbySign = &psith->tail8Bytes[0];
    callOffset = psith->callInsPos;

    // do we have space for bytecode?
    if(callOffset > (chunkSize - callSize))
    {
#ifdef RMP_DEBUG
        lss << UL::DEBUG << L(" hook size mismatch") << UL::ENDL;
#endif //RMP_DEBUG
        return;
    }

    // test if it's right location
    for(iByte = 0; iByte < chunkSize; iByte++)
    {
        if(pbyDest[iByte] != pbySign[iByte])
        {
#ifdef RMP_DEBUG
            lss << UL::DEBUG << L("tail hook signature mismatch [") << pbyDest << L("]") << UL::ENDL;
            lss << UL::DEBUG << L("D: ") << ulhex(pbyDest[0]) << L(" ") << ulhex(pbyDest[1]) << L(" ") << ulhex(pbyDest[2]) << L(" ") << ulhex(pbyDest[3]);
            lss <<               L("  ") << ulhex(pbyDest[4]) << L(" ") << ulhex(pbyDest[5]) << L(" ") << ulhex(pbyDest[6]) << L(" ") << ulhex(pbyDest[7]) << UL::ENDL;
            lss << UL::DEBUG << L("S: ") << ulhex(pbySign[0]) << L(" ") << ulhex(pbySign[1]) << L(" ") << ulhex(pbySign[2]) << L(" ") << ulhex(pbySign[3]);
            lss <<               L("  ") << ulhex(pbySign[4]) << L(" ") << ulhex(pbySign[5]) << L(" ") << ulhex(pbySign[6]) << L(" ") << ulhex(pbySign[7]) << UL::ENDL;
#endif //RMP_DEBUG
            return;
        }
        // copy original code chunk before call
        if(iByte < callOffset)
        {
            byCode[iByte] = pbyDest[iByte];
        }
    }

    // build code chunk
    dbCallDisp.dword = (BYTE*)pfnSetTo - (pbyDest + callOffset + callSize);
    byCode[callOffset] = 0xE9;
    byCode[callOffset+1] = dbCallDisp.byte[0];
    byCode[callOffset+2] = dbCallDisp.byte[1];
    byCode[callOffset+3] = dbCallDisp.byte[2];
    byCode[callOffset+4] = dbCallDisp.byte[3];
    for(iByte = callOffset+5; iByte < chunkSize; iByte++)
    {
        byCode[iByte] = 0x00; //0xCC;
    }

#ifdef RMP_DEBUG
    lss << UL::DEBUG << ulhex(pbyDest[0]) << L(" ") << ulhex(pbyDest[1]) << L(" ") << ulhex(pbyDest[2]) << L(" ") << ulhex(pbyDest[3]);
    lss <<   L("  ") << ulhex(pbyDest[4]) << L(" ") << ulhex(pbyDest[5]) << L(" ") << ulhex(pbyDest[6]) << L(" ") << ulhex(pbyDest[7]) << UL::ENDL;
    lss << UL::DEBUG << ulhex(byCode[0])  << L(" ") << ulhex(byCode[1])  << L(" ") << ulhex(byCode[2])  << L(" ") << ulhex(byCode[3]);
    lss <<   L("  ") << ulhex(byCode[4])  << L(" ") << ulhex(byCode[5])  << L(" ") << ulhex(byCode[6])  << L(" ") << ulhex(byCode[7]) << UL::ENDL;
#endif //RMP_DEBUG

    // can we write?
    bSetAllAccess = FALSE;
    for(iByte = 0; iByte < chunkSize; iByte++)
    {
        if(IsWritableMemory(&pbyDest[iByte]) == FALSE)
        {
            bSetAllAccess = TRUE;
        }
    }
    if(bSetAllAccess != FALSE)
    {
        dwMemMode = SetMemoryAllAccess(pbyDest, chunkSize);
    }
    // write code chunk
    for(iByte = 0; iByte < chunkSize; iByte++)
    {
        pbyDest[iByte] = byCode[iByte];
    }
    // restore memory access mode (if changed)
    if(bSetAllAccess != FALSE)
    {
        RestoreMemoryAccess(pbyDest, chunkSize, dwMemMode);
    }
}



//////// //////// //////// //////// GTA connectors //////// //////// //////// //////// 

DWORD Gta30Connect(DWORD gtaVersionId, 
    G30Interface* pgi, ScriptEventListener* pSEL, 
    SIHDR* pSignHeaders, DWORD* pSignIndices, DWORD numSignIndices, 
    SIDATAPTR* pDataPtrs, SITH* pTailHooks)
{
    DWORD gtaVersion = GTA_UNKNOWN;

#ifdef SI_DEBUG
    lss << UL::DEBUG;
    lss <<  L("137 ") << pSignHeaders[pSignIndices[SIGI_G30_Opcode0137]].numMatchLocs;
    lss << L(" 33F ") << pSignHeaders[pSignIndices[SIGI_G30_Opcode033F]].numMatchLocs;
    lss << L(" RSP ") << pSignHeaders[pSignIndices[SIGI_G30_RunningScriptProcess]].numMatchLocs;
    lss << L(" GAP ") << pSignHeaders[pSignIndices[SIGI_G30_GameProcess]].numMatchLocs;
    lss << L(" TSI ") << pSignHeaders[pSignIndices[SIGI_G30_TheScriptsInit]].numMatchLocs;
    lss << L(" SNV ") << pSignHeaders[pSignIndices[SIGI_G30_SetNextVehicleVariation]].numMatchLocs;
    lss << L(" PMS ") << pSignHeaders[pSignIndices[SIGI_G30_PlaceMarkerSet]].numMatchLocs;
    lss << UL::ENDL;
#endif //SI_DEBUG

    if(signCheck(pSignHeaders, pSignIndices, numSignIndices) != FALSE)
    {
        gtaVersion = gtaVersionId;

        INJDBG(lss << UL::DEBUG << L("gconn: cvp: ") << SCEI_G30_VEHICLE_POOL << L(", ") << ulhex(&pDataPtrs[SCEI_G30_VEHICLE_POOL]) << UL::ENDL);
        pgi->ppVehiclePool = (BYTE**) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_VEHICLE_POOL]);
        INJDBG(lss << UL::DEBUG << L("gconn: vpg: ") << SCEI_G30_VEHPOOL_GET_AT << L(", ") << ulhex(&pDataPtrs[SCEI_G30_VEHPOOL_GET_AT]) << UL::ENDL);
        pgi->pfnVehPoolGetAt = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_VEHPOOL_GET_AT]);
        INJDBG(lss << UL::DEBUG << L("gconn: cti: ") << SCEI_G30_CURRENT_TEXTINFO << L(", ") << ulhex(&pDataPtrs[SCEI_G30_CURRENT_TEXTINFO]) << UL::ENDL);
        pgi->pwCurrentTextInfo = (WORD*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_CURRENT_TEXTINFO]);
        INJDBG(lss << UL::DEBUG << L("gconn: tip: ") << SCEI_G30_TEXTINFO_POOL << L(", ") << ulhex(&pDataPtrs[SCEI_G30_TEXTINFO_POOL]) << UL::ENDL);
        pgi->pTextInfoPool = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_TEXTINFO_POOL]);
        INJDBG(lss << UL::DEBUG << L("gconn: gat: ") << SCEI_G30_GAME_TIME << L(", ") << ulhex(&pDataPtrs[SCEI_G30_GAME_TIME]) << UL::ENDL);
        pgi->pdwGameTime = (DWORD*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_GAME_TIME]);
        INJDBG(lss << UL::DEBUG << L("gconn: poc: ") << SCEI_G30_SCRIPT_PROCESS_ONE_COMMAND << L(", ") << ulhex(&pDataPtrs[SCEI_G30_SCRIPT_PROCESS_ONE_COMMAND]) << UL::ENDL);
        pgi->pfnProcessOneCommand = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_SCRIPT_PROCESS_ONE_COMMAND]);
        INJDBG(lss << UL::DEBUG << L("gconn: tsp: ") << SCEI_G30_THESCRIPTS_PROCESS << L(", ") << ulhex(&pDataPtrs[SCEI_G30_THESCRIPTS_PROCESS]) << UL::ENDL);
        pgi->pfnTheScriptsProcess = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_THESCRIPTS_PROCESS]);
        INJDBG(lss << UL::DEBUG << L("gconn: tsi: ") << SCEI_G30_THESCRIPTS_INIT << L(", ") << ulhex(&pDataPtrs[SCEI_G30_THESCRIPTS_INIT]) << UL::ENDL);
        pgi->pfnTheScriptsInit = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_THESCRIPTS_INIT]);
        INJDBG(lss << UL::DEBUG << L("gconn: sdb: ") << SCEI_G30_SCRIPT_DATA_BUFFER << L(", ") << ulhex(&pDataPtrs[SCEI_G30_SCRIPT_DATA_BUFFER]) << UL::ENDL);
        pgi->pbyScriptDataBuffer = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_SCRIPT_DATA_BUFFER]);
        INJDBG(lss << UL::DEBUG << L("gconn: nv1: ") << SCEI_G30_VEH_NEXT_VARIATION1 << L(", ") << ulhex(&pDataPtrs[SCEI_G30_VEH_NEXT_VARIATION1]) << UL::ENDL);
        pgi->pbyNextVehVariation1 = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_VEH_NEXT_VARIATION1]);
        INJDBG(lss << UL::DEBUG << L("gconn: nv2: ") << SCEI_G30_VEH_NEXT_VARIATION2 << L(", ") << ulhex(&pDataPtrs[SCEI_G30_VEH_NEXT_VARIATION2]) << UL::ENDL);
        pgi->pbyNextVehVariation2 = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_VEH_NEXT_VARIATION2]);
        INJDBG(lss << UL::DEBUG << L("gconn: mps: ") << SCEI_G30_MARKER_PLACE_SET << L(", ") << ulhex(&pDataPtrs[SCEI_G30_MARKER_PLACE_SET]) << UL::ENDL);
        pgi->pbyPlaceMarkerSet = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_MARKER_PLACE_SET]);
        INJDBG(lss << UL::DEBUG << L("gconn: mpo: ") << SCEI_G30_MARKER_PLACE_ONE << L(", ") << ulhex(&pDataPtrs[SCEI_G30_MARKER_PLACE_ONE]) << UL::ENDL);
        pgi->pbyPlaceMarker1 = (BYTE*) signGetOffsetData(pSignHeaders, &pDataPtrs[SCEI_G30_MARKER_PLACE_ONE]);

#ifdef SI_DEBUG
        lss << UL::DEBUG;
        lss <<  L("137 ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_Opcode0137]].matchLocs[0]);
        lss << L(" 33F ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_Opcode033F]].matchLocs[0]);
        lss << L(" RSP ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_RunningScriptProcess]].matchLocs[0]);
        lss << L(" GAP ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_GameProcess]].matchLocs[0]);
        lss << L(" TSI ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_TheScriptsInit]].matchLocs[0]);
        lss << L(" SNV ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_SetNextVehicleVariation]].matchLocs[0]);
        lss << L(" PMS ") << ulhex(pSignHeaders[pSignIndices[SIGI_G30_PlaceMarkerSet]].matchLocs[0]);
        lss << UL::ENDL;

        lss << UL::DEBUG;
        lss <<   L("CTI ") << ulhex(pgi->pwCurrentTextInfo);
        lss << L(", TIP ") << ulhex(pgi->pTextInfoPool);
        lss << L(", IGT ") << ulhex(pgi->pdwGameTime);
        lss << UL::ENDL;
        lss << UL::DEBUG;
        lss <<   L("POC ") << ulhex(pgi->pfnProcessOneCommand);
        lss << L(", TSP ") << ulhex(pgi->pfnTheScriptsProcess);
        lss << L(", TSI ") << ulhex(pgi->pfnTheScriptsInit);
        lss << L(", SDB ") << ulhex(pgi->pbyScriptDataBuffer);
        lss << UL::ENDL;
        lss << UL::DEBUG;
        lss <<   L("CVP ") << ulhex(pgi->ppVehiclePool);
        lss << L(", VPG ") << ulhex(pgi->pfnVehPoolGetAt);
        lss << L(", NV1 ") << ulhex(pgi->pbyNextVehVariation1);
        lss << L(", NV2 ") << ulhex(pgi->pbyNextVehVariation2);
        lss << UL::ENDL;
        lss << UL::DEBUG;
        lss <<   L("PMS ") << ulhex(pgi->pbyPlaceMarkerSet);
        lss << L(", PMK ") << ulhex(pgi->pbyPlaceMarker1);
        lss << UL::ENDL;
#endif //SI_DEBUG
        
        // install d2 & d0 sensors
        BYTE* pbyTH;
        ScriptEventListener scEventListener;
        GetGlobalScriptEventListener(&scEventListener);
        SITH* psithTSProcess = &pTailHooks[STHI_G30_TheScriptsProcess];
        SITH* psithTSInit = &pTailHooks[STHI_G30_TheScriptsInit];

#ifdef SI_DEBUG
        pbyTH = pgi->pfnTheScriptsProcess + psithTSProcess->tailOffset;
        lss << UL::DEBUG << L("xTSP ") << ulhex(pbyTH) << UL::ENDL;
#endif //SI_DEBUG

        WriteTailCallHook(pgi->pfnTheScriptsProcess, psithTSProcess, scEventListener.pfnProcess);

#ifdef SI_DEBUG
        pbyTH = pgi->pfnTheScriptsInit + psithTSInit->tailOffset;
        lss << UL::DEBUG << L("xTSI ") << ulhex(pbyTH) << UL::ENDL;
#endif //SI_DEBUG

        WriteTailCallHook(pgi->pfnTheScriptsInit, psithTSInit, scEventListener.pfnInit);

        SetScriptEventListener(pSEL);
    }
    return gtaVersion;
}



//////// //////// //////// //////// GTA III scanner //////// //////// //////// //////// 

DWORD GtaScan(ScriptEventListener* pSEL, BYTE* pBase, DWORD dwSize)
{
    DWORD gtaVer = GTA_UNKNOWN;
    BYTE* pMemStart = pBase;
    BYTE* pMemStop = pBase + dwSize;
    // load signatures
    SIDBG(lss << UL::INFO << L("numSigs: ") << NUM_GTA_SIGNATURES << UL::ENDL);
    signInit(GSign, GMask, GOff, GtaSignatures, NUM_GTA_SIGNATURES);
    // scan memory
    signScan(pMemStart, pMemStop, GtaSignatures, NUM_GTA_SIGNATURES);

    // is it III?
    if(gtaVer == GTA_UNKNOWN)
    {
        gtaVer = Gta30Connect(GTA_III, &g3i, pSEL, GtaSignatures, G3SignIndices, NUM_G30_SIGNINDICES, G3CodeEntries, G3TailHooks);
    }
    // is it Vice?
    if(gtaVer == GTA_UNKNOWN)
    {
        gtaVer = Gta30Connect(GTA_VC, &g3i, pSEL, GtaSignatures, VCSignIndices, NUM_G30_SIGNINDICES, VCCodeEntries, VCTailHooks);
    }
    // 
    if(gtaVer == GTA_UNKNOWN)
    {
#ifdef SI_DEBUG
        lss << UL::DEBUG;
        lss <<   L("HTG ") << GtaSignatures[SIGN_IV_HashTableGet].numMatchLocs;
        lss << L(", LSC ") << GtaSignatures[SIGN_IV_LoadScript].numMatchLocs;
        lss << UL::ENDL;
        lss << UL::DEBUG;
        lss <<   L("HTG ") << GtaSignatures[SIGN_IV_HashTableGet].matchLocs[0] << L(" ") << GtaSignatures[SIGN_IV_HashTableGet].matchLocs[1];
        lss << L(", LSC ") << GtaSignatures[SIGN_IV_LoadScript].matchLocs[0];
        lss << UL::ENDL;
#endif //SI_DEBUG
    }

    switch(gtaVer)
    {
    case GTA_III:
        lss << UL::INFO << L("GI: III") << UL::ENDL;
        break;
    case GTA_VC:
        lss << UL::INFO << L("GI: VC") << UL::ENDL;
        break;
    case GTA_SA:
        lss << UL::INFO << L("GI: SA") << UL::ENDL;
        break;
    case GTA_IV:
        lss << UL::INFO << L("GI: IV") << UL::ENDL;
        break;
    case GTA_UNKNOWN:
        lss << UL::INFO << L("GI: unknown") << UL::ENDL;
        break;
    }

    return gtaVer;
}



