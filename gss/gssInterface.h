///////////////////////////////////////////////////////////////
//
// gssInterface: Interface to internal stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   13.09.2014 03:45 - created (moved out from gssMain)
//   25.11.2014 20:37 - GTAVERSION moved in
//   26.11.2014 04:20 - GTAVERSION taken care of
//   29.11.2016 22:25 - added ScriptEventListener
//

#pragma once

#include "gtaVehicle.h"


void GtaDrawString(AString str, DWORD font);

void* GtaGetCVehiclePtr(Vehicle vehicle);

void GtaSetNextVehicleVariation(BYTE variation1, BYTE variation2);

// these should be wrapped
void GtaDrawMarkerSet(DWORD markerIndex, DWORD markerType, FloatVector3* pPos, FLOAT size, FLOAT pulsation, ByteVector4* pRgba, DWORD flags);
void GtaDrawMarker1(DWORD markerIndex, DWORD markerType, FloatVector3* pPos, FLOAT size, FLOAT pulsation, ByteVector4* pRgba, DWORD flags, INT markerMovement);



struct ScriptEventListener {
    void (*pfnInit)(void);      // state machine reset
    void (*pfnProcess)(void);   // state machine step
};

void GetGlobalScriptEventListener(ScriptEventListener* pSEL);
void SetScriptEventListener(ScriptEventListener* pSEL);



struct G30Interface {
    DWORD* pdwGameTime;
    BYTE* pfnProcessOneCommand;
    BYTE* pfnTheScriptsProcess;
    BYTE* pfnTheScriptsInit;
    BYTE* pbyScriptDataBuffer;
    WORD* pwCurrentTextInfo;
    BYTE* pTextInfoPool;
    BYTE** ppVehiclePool;
    BYTE* pfnVehPoolGetAt;
    BYTE* pbyNextVehVariation1;
    BYTE* pbyNextVehVariation2;
    BYTE* pbyPlaceMarkerSet;
    BYTE* pbyPlaceMarker1;
};
extern G30Interface g3i;

