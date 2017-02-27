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
//   12.12.2016 22:47 - added packed-rotation routines
//


#define _USE_MATH_DEFINES
#include <cmath>

#include <ulib/uDef.h>

#include "gssDef.h"
#include "gssScriptCall.h"


void GtaPackRotation(SByteVector3& packed, const FloatVector3& unpacked)
{
    FLOAT scaleFactor = 100.0f;
    packed.X = static_cast<SBYTE>(unpacked.X * scaleFactor);
    packed.Y = static_cast<SBYTE>(unpacked.Y * scaleFactor);
    packed.Z = static_cast<SBYTE>(unpacked.Z * scaleFactor);
}

void GtaUnpackRotation(FloatVector3& unpacked, const SByteVector3& packed)
{
    FLOAT scaleFactor = 100.0f;
    unpacked.X = static_cast<FLOAT>(packed.X) / scaleFactor;
    unpacked.Y = static_cast<FLOAT>(packed.Y) / scaleFactor;
    unpacked.Z = static_cast<FLOAT>(packed.Z) / scaleFactor;
}

void GtaMakeRotation(FloatVector3& rotation, const FLOAT heading)
{
    FLOAT angleRad = static_cast<FLOAT>(heading / 180.0f * M_PI);
    // rot x = { 0°:  0.0, 90°: -1.0, 180°:  0.0, 270°: +1.0 } = -sin()
    // rot y = { 0°: +1.0, 90°:  0.0, 180°: -1.0, 270°:  0.0 } =  cos()
    // rot z = 0.0
    rotation.X = -sinf(angleRad);
    rotation.Y = cosf(angleRad);
    rotation.Z = 0.0f;
}

void GtaMakePackedRotation(SByteVector3& rotation, const FLOAT heading)
{
    //FLOAT angleRad = static_cast<FLOAT>(heading / 180.0f * M_PI);
    FloatVector3 unpackedRotation;
    //// rot x = { 0°:  0.0, 90°: -1.0, 180°:  0.0, 270°: +1.0 } = -sin()
    //// rot y = { 0°: +1.0, 90°:  0.0, 180°: -1.0, 270°:  0.0 } =  cos()
    //// rot z = 0.0
    //unpackedRotation.X = -sinf(angleRad);
    //unpackedRotation.Y = cosf(angleRad);
    //unpackedRotation.Z = 0.0f;
    GtaMakeRotation(unpackedRotation, heading);
    GtaPackRotation(rotation, unpackedRotation);
}


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

