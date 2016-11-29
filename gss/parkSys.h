///////////////////////////////////////////////////////////////
//
// gtaParkSys: parking system
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   10.07.2016 19:48 - created
//

#include "parkDef.h"

void parkSysStateMachine(AStringStream& display, GtaPlayerState& playerState);
void parkSysInit(DWORD gtaVersion);
BOOL parkSysGetActiveParking(DWORD* piParkActive);
PARKLOTHDR* parkSysGetParkHdr(DWORD iPark, FloatVector4** ppfvPosSlots);




