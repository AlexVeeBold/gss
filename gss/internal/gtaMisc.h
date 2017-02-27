///////////////////////////////////////////////////////////////
//
// gtaMisc: miscellaneous stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:42 - created
//


void GtaPackRotation(SByteVector3& packed, const FloatVector3& unpacked);
void GtaUnpackRotation(FloatVector3& unpacked, const SByteVector3& packed);
void GtaMakeRotation(FloatVector3& rotation, const FLOAT heading);
void GtaMakePackedRotation(SByteVector3& rotation, const FLOAT heading);

// get ground pos. for pPos, store into pGroundPos
void gtaWorldGetGroundPos(FloatVector3* pPos, FloatVector3* pGroundPos);
// get ground pos. for pPos, store into pPos
void gtaWorldGetGroundZ(FloatVector3* pPos);


BOOL gtaIsPlayerPressingHorn(void);

