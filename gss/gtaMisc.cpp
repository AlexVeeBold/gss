///////////////////////////////////////////////////////////////
//
// gtaMisc: miscellaneous stuff
//
// Copyright © 2014,2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:38 - created (moved from gssMain)
//


#include "uDef.h"

#include "gssDef.h"
#include "gssScriptCall.h"


void gtaWorldGetGroundPos(FloatVector3* pPos, FloatVector3* pGroundPos)
{
    pGroundPos->X = pPos->X;
    pGroundPos->Y = pPos->Y;
    pGroundPos->Z = pPos->Z;
    SN::WORLD::GET_GROUND_Z_FOR_3D_COORD(*pPos, &pGroundPos->Z);
}

void gtaWorldGetGroundZ(FloatVector3* pPos)
{
    SN::WORLD::GET_GROUND_Z_FOR_3D_COORD(*pPos, &pPos->Z);
}


BOOL gtaIsPlayerPressingHorn(void)
{
    //!! CALLER SHOULD TEST IF PLAYER IS IN CAR BEFORE THIS !!//
    return (SN::PLAYER::IS_BUTTON_PRESSED(0, PB_VEHICLE_HORN) != FALSE);
}

