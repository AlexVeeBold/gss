///////////////////////////////////////////////////////////////
//
// parkInit: initial (predefined) parking lot data
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   30.11.2014 07:13 - created (moved from gssMain)
//   12.12.2016 06:31 - improved lane data storage (draft)
//   12.12.2016 23:09 - done with new data storage format
//   27.12.2016 03:56 - cleaned up a little
//


#ifndef PREPROC_PASS

#include <ulib/uDef.h>
#include <ulib/tunic.h>

#include "gssDef.h"

#include "parkDef.h"

#include <internal/gtaVersion.h>
#include <internal/gtaVehicle.h>
#include <internal/gtaVehInfo.h>

#endif //PREPROC_PASS


#define VEHSPEC_SIZE_III    sizeof(GVeh30)
#define VEHSPEC_SIZE_VC     sizeof(GVeh30)


enum IPD_DEFS {
    NUM_POS_IN_A_ROW = 2,   // start_pos + inc_pos
    // III's parking lane vehicle types
    IPD_VT_LAND_III = VT_LAND_WHEELED,
    IPD_VT_WATER_III = VT_WATER_BOAT,
    // VC's parking lane vehicle types
    IPD_VT_LAND_VC = VT_LAND_WHEELED | VT_LAND_MOTORBIKE,
    IPD_VT_WATER_VC = VT_WATER_BOAT | VT_AMPHIBIOUS,
    IPD_VT_BOAT_VC = VT_WATER_BOAT,
    IPD_VT_HELI_VC = VT_AIR_VTOL,
    // IV's parking lane vehicle types
    IPD_VT_LAND_IV = VT_LAND_WHEELED | VT_LAND_MOTORBIKE,
    IPD_VT_WATER_IV = VT_WATER_BOAT,
    IPD_VT_HELI_IV = VT_AIR_VTOL,
};


#define MJOIN2(a,b)         a##b
#define MJOIN3(a,b,c)       a##b##c
#define MJOIN4(a,b,c,d)     a##b##c##d


#define GTAPID(gid,pid)     MJOIN3(gid,_,pid)
#define GTAPARKID(pid)      GTAPID(GTAID,pid)


#define LANE_COUNT_NAME(gpid)       MJOIN2(NUM_LANES_,gpid)
#define LANE_COUNT(pid)             LANE_COUNT_NAME(GTAPARKID(pid))
#define SPACE_COUNT_NAME(gpid)      MJOIN2(NUM_SPACES_,gpid)
#define SPACE_COUNT(pid)            SPACE_COUNT_NAME(GTAPARKID(pid))


#define E_TOTAL_PARKINGS(gid)           MJOIN2(TOTAL_PARKINGS_,gid)
#define NUM_TOTAL_PARKING_LOTS(gid)     MJOIN2(NUM_TOTAL_PARKING_LOTS_,gid)
#define E_TOTAL_PARK_SPACES(gid)        MJOIN2(TOTAL_PARK_SPACES_,gid)
#define NUM_TOTAL_SPACES(gid)           MJOIN2(NUM_TOTAL_SPACES_,gid)
#define E_TOTAL_DATA_SIZE(gid)          MJOIN2(TOTAL_DATA_SIZE_,gid)
#define TOTAL_IPD_SIZE(gid)             MJOIN2(TOTAL_IPD_SIZE_,gid)

#define IPD_VEHSPEC_SIZE(gid)       MJOIN2(VEHSPEC_SIZE_,gid)



// count used lanes, pos. slots, veh. spaces (for each parking lot)

#define COUNT_LANE_ROW(id,t,n,vs,vi)    1
#define COUNT_LANE_POINT(id,t,vs)       1
#define COUNT_LANE_NONE                 0

#define COUNT_SPACE_ROW(id,t,n,vs,vi)   n
#define COUNT_SPACE_POINT(id,t,vs)      1
#define COUNT_SPACE_NONE                0

#define PARKCOUNTER(n, l0, l1, l2, l3)      \
    n##_COUNT(PARKID) =         \
    COUNT_##n##_##l0 +          \
    COUNT_##n##_##l1 +          \
    COUNT_##n##_##l2 +          \
    COUNT_##n##_##l3            

#define PARKCOUNTERS(l0, l1, l2, l3)    \
    PARKCOUNTER(LANE, l0, l1, l2, l3),  \
    PARKCOUNTER(SPACE, l0, l1, l2, l3),

#define PARK(bb, l0, l1, l2, l3)    PARKCOUNTERS(l0, l1, l2, l3)

#pragma message("IPD: counting lanes/pos/spaces")

enum PARK_COUNTERS {

#define GTAID       III
#include "parkInitDataIII.h"
#undef GTAID

#define GTAID       VC
#include "parkInitDataVC.h"
#undef GTAID

};

#undef PARK



// count parking lots

#define PARKING(pid)            MJOIN2(PARKING_,pid)

#define PARK(bb, l0, l1, l2, l3)        \
    PARKING(PARKID),

#pragma message("IPD: counting parkings")

#define GTAID       III
enum E_TOTAL_PARKINGS(GTAID) {
#include "parkInitDataIII.h"
    NUM_TOTAL_PARKING_LOTS(GTAID)
};
#undef GTAID

#define GTAID       VC
enum E_TOTAL_PARKINGS(GTAID) {
#include "parkInitDataVC.h"
    NUM_TOTAL_PARKING_LOTS(GTAID)
};
#undef GTAID

#undef PARK



// count veh. spaces (total)

#define PARK(bb, l0, l1, l2, l3)        SPACE_COUNT(PARKID) +

#pragma message("IPD: counting total spaces")

#define GTAID       III
enum E_TOTAL_PARK_SPACES(GTAID) {
    NUM_TOTAL_SPACES(GTAID) = 
#include "parkInitDataIII.h"
    0   // The Terminator
};
#undef GTAID

#define GTAID       VC
enum E_TOTAL_PARK_SPACES(GTAID) {
    NUM_TOTAL_SPACES(GTAID) = 
#include "parkInitDataVC.h"
    0   // The Terminator
};
#undef GTAID

#undef PARK



// count total size of initial parking data

#define PARK(bb, l0, l1, l2, l3)                \
    sizeof(PARKLOTHDR) +                        \
    (sizeof(PARKLANE) * LANE_COUNT(PARKID)) +   \
    (IPD_VEHSPEC_SIZE(GTAID) * SPACE_COUNT(PARKID)) + 

#pragma message("IPD: counting total ipd size")

#define GTAID       III
enum E_TOTAL_DATA_SIZE(GTAID) {
    TOTAL_IPD_SIZE(GTAID) = sizeof(PARKSYSHDR) + 
#include "parkInitDataIII.h"
    0   // Terminator 2
};
#undef GTAID

#define GTAID       VC
enum E_TOTAL_DATA_SIZE(GTAID) {
    TOTAL_IPD_SIZE(GTAID) = sizeof(PARKSYSHDR) + 
#include "parkInitDataVC.h"
    0   // Terminator 2
};
#undef GTAID

#undef PARK



// build initial parking data (type definition AND data initialization)

#define PDEF_STRINGIFY(x)       #x

#define PDEF_DIMS(pid)              PARKLOTHDR_SIGN, PDEF_STRINGIFY(pid), LANE_COUNT(pid), 0, SPACE_COUNT(pid), 0
#define PDEF_BBOX(x0n,y0n,z0n,x0m,y0m,z0m, x1n,y1n,z1n,x1m,y1m,z1m)                         {x0n, y0n, z0n, x0m, y0m, z0m, 0.0f, 0.0f}, {x1n, y1n, z1n, x1m, y1m, z1m, 0.0f, 0.0f}
#define PDEF_ZOBBOX(x0n,y0n,z0n,x0m,y0m,z0m,x0o,y0o, x1n,y1n,z1n,x1m,y1m,z1m,x1o,y1o)       {x0n, y0n, z0n, x0m, y0m, z0m,  x0o,  y0o}, {x1n, y1n, z1n, x1m, y1m, z1m,  x1o,  y1o}

#define PLDS_FV4(x,y,z,a)               LANE_PACK_ANGLE(a), {x, y, z}
#define PLDI_FV4(x,y,z,a)               0, {x, y, z}
#define LANEDATA_ROW(id,t,n,vps,vpi)    {n, t, 0, PLDS_##vps, PLDI_##vpi},
#define LANEDATA_POINT(id,t,vps)        {1, t, 0, PLDS_##vps, },
#define LANEDATA_NONE                   //{0,},

#define IPD_TYPE(gid)           MJOIN2(INITIALPARKDATA_,gid)
#define IPD_NAME(gid)           MJOIN2(initialParkData,gid)


#define IPD_TYPEDEF             PARKSYSHDR sysHdr

#define IPD_TYPEINIT                        \
    {PARKSYSHDR_SIGN1, PARKSYSHDR_SIGN2,    \
    NUM_TOTAL_PARKING_LOTS(GTAID), NUM_TOTAL_SPACES(GTAID), TOTAL_IPD_SIZE(GTAID)}


#define IPD_PARK_HDR(pid)       PARKLOTHDR MJOIN2(lotHdr,pid)
#define IPD_PARK_LANES(pid)     PARKLANE MJOIN2(lane,pid) [ LANE_COUNT(pid) ]
#define IPD_PARK_SPACES(pid)    BYTE MJOIN2(vehSpec,pid) [ IPD_VEHSPEC_SIZE(GTAID) * SPACE_COUNT(pid) ]

#define PARK_TYPEDEF(bb, l0, l1, l2, l3)    \
    IPD_PARK_HDR(PARKID);                   \
    IPD_PARK_LANES(PARKID);                 \
    IPD_PARK_SPACES(PARKID);

#define PARK_TYPEINIT(bb, l0, l1, l2, l3)   \
    {                                       \
        PDEF_DIMS(PARKID),                  \
        PDEF_##bb,                      \
    },                                  \
    {                                   \
        LANEDATA_##l0               \
        LANEDATA_##l1               \
        LANEDATA_##l2               \
        LANEDATA_##l3           \
    },                          \
    {{0,},},


#pragma message("IPD: building data structures")

#define GTAID       III

struct IPD_TYPE(GTAID) {
    IPD_TYPEDEF;
#define PARK(bb, l0, l1, l2, l3)        PARK_TYPEDEF(bb, l0, l1, l2, l3)
#include "parkInitDataIII.h"
#undef PARK
} IPD_NAME(GTAID) = {
    IPD_TYPEINIT,
#define PARK(bb, l0, l1, l2, l3)        PARK_TYPEINIT(bb, l0, l1, l2, l3)
#include "parkInitDataIII.h"
#undef PARK
};

#undef GTAID

#define GTAID       VC

struct IPD_TYPE(GTAID) {
    IPD_TYPEDEF;
#define PARK(bb, l0, l1, l2, l3)        PARK_TYPEDEF(bb, l0, l1, l2, l3)
#include "parkInitDataVC.h"
#undef PARK
} IPD_NAME(GTAID) = {
    IPD_TYPEINIT,
#define PARK(bb, l0, l1, l2, l3)        PARK_TYPEINIT(bb, l0, l1, l2, l3)
#include "parkInitDataVC.h"
#undef PARK
};

#undef GTAID




void getInitialParkingData(DWORD GtaVersion, DataBuffer& ipd)
{
    switch(GtaVersion)
    {
    case GTA_III:
        ipd.ptr = static_cast<BYTE*>(static_cast<void*>(&initialParkDataIII));
        ipd.size = sizeof(initialParkDataIII);
        break;
    case GTA_VC:
        ipd.ptr = static_cast<BYTE*>(static_cast<void*>(&initialParkDataVC));
        ipd.size = sizeof(initialParkDataVC);
        break;
    default:
        ipd.ptr = NULL;
        ipd.size = 0;
        break;
    }
}

