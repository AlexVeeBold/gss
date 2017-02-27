///////////////////////////////////////////////////////////////
//
// parkDef: parking lots data defines & structures
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   30.11.2014 07:18 - created (moved from gssMain)
//   10.07.2016 18:34 - moved Vehicle stuff to gtaVehicle.h
//   27.12.2016 03:49 - cleaned up
//

#pragma once


#include <ulib/uString.h>


enum PARKSIGN {
    PARKSYSHDR_SIGN1 = 0x53535603,  // '<heart>VSS'
    PARKSYSHDR_SIGN2 = 0x00000003,  // '<heart>   '
    PARKLOTHDR_SIGN  = 0x00475453,  // 'STG\0'
    PARKSPCHDR_SIGN  = 0x00484556,  // 'VEH\0'
};



struct PARKSYSHDR {
    DWORD sign1;
    DWORD sign2;
    WORD numParks;
    WORD totalParkSpaces;
    DWORD totalDataSize;
};

enum PARKSYSDEFS {
    PSD_NAME_SIZE = 8,
    PSD_MAX_LANE_SPACES = 16,   // should be less than number set by PSD_LANE_NUMSPACES_MASK
    PSD_MAX_PARKING_LANES = 8,
    //!! PSD_MAX_PARKING_VEHICLES should be LESS-or-EQUAL than MDLQUEUE_SIZE !!//
    PSD_MAX_PARKING_VEHICLES = PSD_MAX_PARKING_LANES * PSD_MAX_LANE_SPACES,
    // III: shoreside(4) + staunton(8) + portland(4) = 16
    //  VC: mainland(14?) + beach(16?) = 30?
    PSD_MAX_PARKINGS = 32,
};


#define LANE_PACK_ANGLE(a)          (BYTE)( a / 360.0f * 256.0f )
#define LANE_UNPACK_ANGLE(a)        ( ((FLOAT)a) / 256.0f * 360.0f )

struct PARKLANE {
    BYTE numSpaces;
    BYTE vehTypes;
    BYTE z02;
    BYTE spawnAngle;
    FloatVector3 fvStart;
    DWORD z10;
    FloatVector3 fvInc;
};

struct PARKLOTHDR {
    DWORD sign;
    BYTE name[PSD_NAME_SIZE];
    BYTE numLanes;      // number of PARKLANE structures
    BYTE z0D;
    BYTE numSpaces;     // number of VEHINFO structures
    BYTE z0F;
    FloatZOBBox3 bbEnter;   // inner
    FloatZOBBox3 bbLeave;   // outer
};

/*      parking data file:      // every data record should be 16-byte aligned size (padded with zeroes when necessary)
    PARKSYSHDR syshdr
    (numParks){
        PARKLOTHDR lothdr
        PARKLANE[n] pos         // n = numLanes
        VEHSPEC[m] vehSpec      // m = sum ( numSpaces for each lane )
    }
*/


void getInitialParkingData(DWORD GtaVersion, DataBuffer& ipd);



