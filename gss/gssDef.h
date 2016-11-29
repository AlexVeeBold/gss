///////////////////////////////////////////////////////////////
//
// gssDef: script system basic types
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   13.09.2014 04:02 - created
//   23.09.2014 15:51 - added FloatVector4
//   02.12.2014 01:35 - tuned ByteVector for initialization with dword
//   10.12.2014 16:43 - added FloatVector2
//   04.01.2015 10:16 - added FloatAngledVector3
//   22.11.2016 14:05 - added SByteVector3
//   25.11.2016 07:01 - added DataBuffer
//


#pragma once


/*  gta coord space:

                UP    0°
                 ^   N
        (Z-axis) |  ^
                 | / (Y-axis)
   90°           |/            270°
      W ---------+---------> E
                /|         (X-axis)
               / '
          180°    

rotX,rotY,rotZ: normalized forward vector:
( 0.0,  1.0,  0.0) north
(-0.7,  0.7,  0.0) north-west
(-1.0,  0.0,  0.0) west
(-0.7, -0.7,  0.0) south-west
( 0.0, -1.0,  0.0) south
( 0.7, -0.7,  0.0) south-east
( 1.0,  0.0,  0.0) east
( 0.7,  0.7,  0.0) north-east
( x.x,  x.x, <0.0) going down
( x.x,  x.x, >0.0) going up
*/

struct DataBuffer {
    BYTE* ptr;
    DWORD size;
};


enum FVAXIS {
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z,
    AXIS_W,
    AXIS_V,
    AXIS_U, // etc.
};
enum AXISEND {
    AXIS_MIN,
    AXIS_MAX,
    //
    NUM_AXIS_ENDS
};

union FloatVector2 {
    struct {
        FLOAT X;
        FLOAT Y;
    };
    FLOAT f[2]; // x, y
    DWORD dw[2]; // x, y
};

union FloatVector3 {
    struct {
        FLOAT X;
        FLOAT Y;
        FLOAT Z;
    };
    FLOAT f[3]; // x, y, z
    DWORD dw[3]; // x, y, z
};

union FloatVector4 {
    struct {
        FLOAT X;
        FLOAT Y;
        FLOAT Z;
        FLOAT A;
    };
    FLOAT f[4]; // x, y, z, heading
    DWORD dw[4]; // x, y, z, heading
};

union FloatAngledVector3 {
    struct {
        FloatVector3 pos;
        FLOAT angle;
    };
    FLOAT f[4]; // x, y, z, heading
    DWORD dw[4]; // x, y, z, heading
};

union FloatBox3 {
    struct {
        FloatVector3 min;
        FloatVector3 max;
    };
    FloatVector3 fv[NUM_AXIS_ENDS];
};


struct FloatZOBBox3 {     // Z-Oriented Bounding Box
    union {
        struct {
            FloatVector3 min;
            FloatVector3 max;
        };
        FloatVector3 fv[NUM_AXIS_ENDS];
    };
    FloatVector2 offset;    // x offset (added to min.x) for corner with max.y;   y offset (added to min.y) for corner with max.x;   for other two corners same offset subtracted from max.coord
};


enum QUADSIDES {
    SIDE_NORTH,
    SIDE_EAST,
    SIDE_SOUTH,
    SIDE_WEST,
    //
    NUM_QUAD_SIDES
};
union FloatInnerQuad3 {
    struct {
        FloatVector3 west;  // x-
        FloatVector3 east;  // x+
        FloatVector3 south; // y-
        FloatVector3 north; // y+
    };
    FloatVector3 fv[NUM_QUAD_SIDES];
};


union SByteVector3 {
//    DWORD dw;
    SBYTE sby[3];
    struct {
        SBYTE X;
        SBYTE Y;
        SBYTE Z;
    };
};


union ByteVector3 {
    DWORD dw;
    BYTE by[3];
    struct {
        BYTE R;
        BYTE G;
        BYTE B;
    };
};

union ByteVector4 {
    DWORD dw;
    BYTE by[4];
    struct {
        BYTE R;
        BYTE G;
        BYTE B;
        BYTE A;
    };
};


#define ScriptHandle    DWORD
#define Unknown         DWORD
typedef DWORD           Player;
typedef ScriptHandle    Ped;
typedef ScriptHandle    Vehicle;
typedef ScriptHandle    Blip;
//typedef ScriptHandle    Checkpoint
//typedef ScriptHandle    Pickup
//typedef ScriptHandle    Object
//typedef ScriptHandle    Group


struct GtaPlayerState {
    Ped charPlayer;
    FloatVector3 fvPosChar;
    BOOL bIsInCar;
    Vehicle vehiclePlayer;
    FloatVector3 fvPosVehicle;
    FLOAT fHeadingVehicle;
};


