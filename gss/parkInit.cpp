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
//


#include "uDef.h"
#include "tunic.h"

#include "gssDef.h"

#include "parkDef.h"

#include "gtaVersion.h"
#include "gtaVehicle.h"
#include "gtaVehInfo.h"



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
};


#define MJOIN2(a,b)         a##b
#define MJOIN3(a,b,c)       a##b##c
#define MJOIN4(a,b,c,d)     a##b##c##d


#define GTAPID(gid,pid)     MJOIN3(gid,_,pid)
#define GTAPARKID(pid)      GTAPID(GTAID,pid)


#define E_LANE_COUNT(pid)       MJOIN2(COUNT_LANES_,pid)
#define E_POS_COUNT(pid)        MJOIN2(COUNT_POS_,pid)
#define E_SPACE_COUNT(pid)      MJOIN2(COUNT_SPACES_,pid)

#define LANE_ENUM_NAME(pid,lid)     MJOIN4(LANE_,pid,_,lid)
#define LANE_ROW(id,t,n,vp,vi)      LANE_ENUM_NAME(PARKID,id),
//#define LANE_POINTSn(id,t,n,vp0,vp1,)   LANE_ENUM_NAME(PARKID,id)
#define LANE_NONE                   //nothing
#define LANE_COUNT_NAME(gpid)       MJOIN2(NUM_LANES_,gpid)
#define LANE_COUNT(pid)             LANE_COUNT_NAME(GTAPARKID(pid))

#define POS_ENUM_NAME(pid,i)    MJOIN4(NUM_POS_,pid,_,i)
#define POS_ENUM_ENTRY(i,n)     POS_ENUM_NAME(PARKID,i) = n,
#define POS_TOTAL_NAME2(gpid)   MJOIN2(TOTAL_POS_,gpid)
#define POS_TOTAL_NAME(pid)     POS_TOTAL_NAME2(GTAPARKID(pid))
#define POS0_ROW(id,t,n,vp,vi)      POS_ENUM_ENTRY(0,NUM_POS_IN_A_ROW)
#define POS1_ROW(id,t,n,vp,vi)      POS_ENUM_ENTRY(1,NUM_POS_IN_A_ROW)
#define POS2_ROW(id,t,n,vp,vi)      POS_ENUM_ENTRY(2,NUM_POS_IN_A_ROW)
#define POS3_ROW(id,t,n,vp,vi)      POS_ENUM_ENTRY(3,NUM_POS_IN_A_ROW)
//#define POSx_POINTSn(id,t,n,vp0,vp1,)   POS_ENUM_ENTRY(x,n)
//      POS0_NONE       -> error
#define POS1_NONE                   POS_ENUM_ENTRY(1,0)
#define POS2_NONE                   POS_ENUM_ENTRY(2,0)
#define POS3_NONE                   POS_ENUM_ENTRY(3,0)
#define POS_SUM(pid)        POS_TOTAL_NAME(pid) = POS_ENUM_NAME(pid,0) + POS_ENUM_NAME(pid,1) + POS_ENUM_NAME(pid,2) + POS_ENUM_NAME(pid,3)

#define SPC_ENUM_NAME(pid,i)    MJOIN4(NUM_SPACES_,pid,_,i)
#define SPC_ENUM_ENTRY(i,n)     SPC_ENUM_NAME(PARKID,i) = n,
#define SPC_TOTAL_NAME2(gpid)   MJOIN2(TOTAL_SPACES_,gpid)
#define SPC_TOTAL_NAME(pid)     SPC_TOTAL_NAME2(GTAPARKID(pid))
#define SPACES0_ROW(id,t,n,vp,vi)   SPC_ENUM_ENTRY(0,n)
#define SPACES1_ROW(id,t,n,vp,vi)   SPC_ENUM_ENTRY(1,n)
#define SPACES2_ROW(id,t,n,vp,vi)   SPC_ENUM_ENTRY(2,n)
#define SPACES3_ROW(id,t,n,vp,vi)   SPC_ENUM_ENTRY(3,n)
//#define SPACESx_POINTSn(id,t,n,vp0,vp1,)   SPC_ENUM_ENTRY(x,n)
//      SPACES0_NONE    -> error
#define SPACES1_NONE                SPC_ENUM_ENTRY(1,0)
#define SPACES2_NONE                SPC_ENUM_ENTRY(2,0)
#define SPACES3_NONE                SPC_ENUM_ENTRY(3,0)
#define SPACES_SUM(pid)     SPC_TOTAL_NAME(pid) = SPC_ENUM_NAME(pid,0) + SPC_ENUM_NAME(pid,1) + SPC_ENUM_NAME(pid,2) + SPC_ENUM_NAME(pid,3),


#define E_TOTAL_PARKINGS(gid)           MJOIN2(TOTAL_PARKINGS_,gid)
#define NUM_TOTAL_PARKING_LOTS(gid)     MJOIN2(NUM_TOTAL_PARKING_LOTS_,gid)
#define E_TOTAL_PARK_SPACES(gid)        MJOIN2(TOTAL_PARK_SPACES_,gid)
#define NUM_TOTAL_SPACES(gid)           MJOIN2(NUM_TOTAL_SPACES_,gid)
#define E_TOTAL_DATA_SIZE(gid)          MJOIN2(TOTAL_DATA_SIZE_,gid)
#define TOTAL_IPD_SIZE(gid)             MJOIN2(TOTAL_IPD_SIZE_,gid)

#define IPD_VEHSPEC_SIZE(gid)       MJOIN2(VEHSPEC_SIZE_,gid)



// count used lanes, pos. slots, veh. spaces (for each parking lot)

#define PARK(bb, l0, l1, l2, l3)        \
enum E_LANE_COUNT(GTAPARKID(PARKID)) {       \
    LANE_##l0               \
    LANE_##l1               \
    LANE_##l2               \
    LANE_##l3               \
    LANE_COUNT(PARKID)      \
};                          \
enum E_POS_COUNT(GTAPARKID(PARKID)) {         \
    POS0_##l0               \
    POS1_##l1               \
    POS2_##l2               \
    POS3_##l3               \
    POS_SUM(PARKID)         \
};                          \
enum E_SPACE_COUNT(GTAPARKID(PARKID)) {      \
    SPACES0_##l0            \
    SPACES1_##l1            \
    SPACES2_##l2            \
    SPACES3_##l3            \
    SPACES_SUM(PARKID)      \
};

#pragma message("IPD: counting lanes/pos/spaces")

#define GTAID       III
#include "parkInitDataIII.h"
#undef GTAID

#define GTAID       VC
#include "parkInitDataVC.h"
#undef GTAID

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

#define PARK(bb, l0, l1, l2, l3)        \
    SPC_TOTAL_NAME(PARKID) + 

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

#define PARK(bb, l0, l1, l2, l3)        \
    sizeof(PARKLOTHDR) + (sizeof(FloatVector4) * POS_TOTAL_NAME(PARKID)) + (IPD_VEHSPEC_SIZE(GTAID) * SPC_TOTAL_NAME(PARKID)) + 

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

#define PD_STRINGIFY(x)         #x

#define PD_DIMS(pid)                    PARKLOTHDR_SIGN, PD_STRINGIFY(pid), LANE_COUNT(pid), POS_TOTAL_NAME(pid), SPC_TOTAL_NAME(pid), 0
#define LANEINFO_ROW(id,t,n,vp,vi)      {t, PSD_LANE_ROW | n},
//#define LANEINFO_POINTSn((id,t,n,vp0,vp1,)   {t, PSD_LANE_POINTS | n},
#define LANEINFO_NONE                   //nothing
#define PD_BBOX(x0n,y0n,z0n,x0m,y0m,z0m, x1n,y1n,z1n,x1m,y1m,z1m)                   {x0n, y0n, z0n, x0m, y0m, z0m, 0.0f, 0.0f},  {x1n, y1n, z1n, x1m, y1m, z1m, 0.0f, 0.0f}
#define PD_ZOBBOX(x0n,y0n,z0n,x0m,y0m,z0m,x0o,y0o, x1n,y1n,z1n,x1m,y1m,z1m,x1o,y1o)   {x0n, y0n, z0n, x0m, y0m, z0m, x0o, y0o},    {x1n, y1n, z1n, x1m, y1m, z1m, x1o, y1o}
#define PLD_FV4(x,y,z,a)                {x, y, z, a}
#define LANEDATA_ROW(id,t,n,vp,vi)      PLD_##vp, PLD_##vi,
//#define LANEDATA_POINTSn((id,t,n,vp0,vp1,)   PLD_##vp0, PLD_##vp1, ... PLD_##vpN
#define LANEDATA_NONE                   //nothing


#define IPD_TYPE(gid)           MJOIN2(INITIALPARKDATA_,gid)
#define IPD_NAME(gid)           MJOIN2(initialParkData,gid)


#define IPD_TYPEDEF             PARKSYSHDR sysHdr;

#define IPD_TYPEINIT                        \
    {PARKSYSHDR_SIGN1, PARKSYSHDR_SIGN2,    \
    NUM_TOTAL_PARKING_LOTS(GTAID), NUM_TOTAL_SPACES(GTAID), TOTAL_IPD_SIZE(GTAID)}


#define IPD_PARK_HDR(pid)       PARKLOTHDR MJOIN2(lotHdr,pid);
#define IPD_PARK_POS(pid)       FloatVector4 MJOIN2(pos,pid) MJOIN3([,POS_TOTAL_NAME(PARKID),]);
#define IPD_PARK_SPC(pid)       BYTE MJOIN2(vehSpec,pid) MJOIN3([,IPD_VEHSPEC_SIZE(GTAID),]) MJOIN3([,SPC_TOTAL_NAME(PARKID),]);

#define PARK_TYPEDEF(bb, l0, l1, l2, l3)    \
    IPD_PARK_HDR(PARKID)                    \
    IPD_PARK_POS(PARKID)                    \
    IPD_PARK_SPC(PARKID)

#define PARK_TYPEINIT(bb, l0, l1, l2, l3)   \
    {                                       \
        PD_DIMS(PARKID),                    \
        {                                   \
            LANEINFO_##l0                   \
            LANEINFO_##l1               \
            LANEINFO_##l2               \
            LANEINFO_##l3               \
        },                          \
        PD_##bb,                    \
    },                              \
    {                           \
        LANEDATA_##l0           \
        LANEDATA_##l1           \
        LANEDATA_##l2       \
        LANEDATA_##l3       \
    },                      \
    {{0,},},


#pragma message("IPD: building data structures")

#define GTAID       III

struct IPD_TYPE(GTAID) {
    IPD_TYPEDEF
#define PARK        PARK_TYPEDEF
#include "parkInitDataIII.h"
#undef PARK
} IPD_NAME(GTAID) = {
    IPD_TYPEINIT,
#define PARK        PARK_TYPEINIT
#include "parkInitDataIII.h"
#undef PARK
};

#undef GTAID

#define GTAID       VC

struct IPD_TYPE(GTAID) {
    IPD_TYPEDEF
#define PARK        PARK_TYPEDEF
#include "parkInitDataVC.h"
#undef PARK
} IPD_NAME(GTAID) = {
    IPD_TYPEINIT,
#define PARK        PARK_TYPEINIT
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

