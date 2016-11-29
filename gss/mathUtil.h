///////////////////////////////////////////////////////////////
//
// mathUtil: math stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:11 - created
//   04.12.2014 18:22 - added distance2d
//   10.12.2014 17:19 - added fAbs
//   20.11.2016 04:13 - replaced non-const ptrs with const refs
//


// abs
FLOAT fAbs(FLOAT f);

// min, max
FLOAT fmin(FLOAT fa, FLOAT fb);
FLOAT fmax(FLOAT fa, FLOAT fb);

// round f.p.value to nearest unsigned integer, lower than f.p.value
DWORD uiFloor(FLOAT f);

// crop f.p.value to given number of binary digits
FLOAT fCrop(FLOAT f, DWORD numPointDigits);

// set f.p.value to nearest multiple of "align" value
FLOAT fAlign(FLOAT f, FLOAT fAlign);

// get hypotenuse from two legs
FLOAT rightTriangleHypotenuse(FLOAT legA, FLOAT legB);
// get leg from hypotenuse and other leg
FLOAT rightTriangleLeg(FLOAT otherLeg, FLOAT hypotenuse);

// get distance between two points
FLOAT distance2d(const FloatVector3& fvA, const FloatVector3& fvB);   // ignoring z
FLOAT distance3d(const FloatVector3& fvA, const FloatVector3& fvB);

// Axis-Aligned Bounding Box test
BOOL aabbInside3d(const FloatVector3& fvPos, const FloatVector3& fvAABBMin, const FloatVector3& fvAABBMax);
BOOL aabbOutside3d(const FloatVector3& fvPos, const FloatVector3& fvAABBMin, const FloatVector3& fvAABBMax);

// Z-Oriented Bounding Box test
BOOL zobbInside3d(const FloatVector3& fvPos, const FloatZOBBox3& ZOBBox);
BOOL zobbInside3dX(AStringStream& display, const FloatVector3& fvPos, const FloatZOBBox3& ZOBBox);

//// "AABB, then ZOBB" test
//BOOL IsInsideZOBB(FloatVector3* pfvPos, FloatZOBBox3* pZOBBox);


