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
//   23.11.2014 18:35 - SIHDR updated (moving signatures into one array)
//   26.11.2014 04:10 - added SITH
//

#pragma once


enum SI_CONST {
    SI_DISPLACEMENT_SHIFT = 4,      // displacement added to address following it's 4 bytes: xx xx xx xx [from-here]
    SI_MAX_MATCH_LOCS = 8,
};
enum SI_OFFSETTYPE {
    SI_BYTE = 0x11,         // data byte
    SI_DWORD = 0x24,        // data dword
    SI_ADDRESS = 0x44,      // virtual address
    SI_DISPLACEMENT = 0x84, // signed displacement from first byte after displacement
    // helpers
    SI_SIZEMASK = 0x0F,     // offset data size mask
};
struct SIOFFSET {
    WORD type;      // BY / DW / VA / SD
    WORD codePos;   // BY / DW / VA / SD position in byte code
};
struct SIHDR {
    DWORD numRequiredLocs; // compile-time
    DWORD numOffsets;   // constant
    DWORD codeLen;      // constant
    DWORD codePos;      // run-time
    SIOFFSET* pOffset;  // compile-time
    BYTE* pSignature;   // compile-time
    BYTE* pMask;        // compile-time
    DWORD numMatchLocs; // run-time
    BYTE* matchLocs[SI_MAX_MATCH_LOCS]; // run-time
};

struct SITH {
    DWORD tailOffset;
    DWORD callInsPos;
    BYTE tail8Bytes[8];
};

enum SIDATAPTRTYPE : DWORD {
    SIDP_PLAIN,
    SIDP_OFFSET,
};
struct SIDATAPTR {
    DWORD signatureIndex;
    DWORD locationIndex;
    SIDATAPTRTYPE type;
    DWORD offsetIndex;
};


void signInit(BYTE* pSignData, BYTE* pMaskData, SIOFFSET* pOffsetData, SIHDR* pSignHeaders, DWORD numSignatures);

void signScan(BYTE* pMemStart, BYTE* pMemStop, SIHDR* pSignHeaders, DWORD numSignatures);

BOOL signCheck(SIHDR* pSignHeaders, DWORD* pSignIndices, DWORD numSignIndices);

BYTE* signGetOffsetData(SIHDR* pSignHeader, SIDATAPTR* pData);

