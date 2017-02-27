///////////////////////////////////////////////////////////////
//
// gtaParkSys: parking system
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   10.07.2016 19:07 - created (moved from gssMain)
//   12.07.2016 21:44 - moving park system into a class (and refactoring a-little)
//   20.11.2016 17:39 - updated for use new ulog subsystem
//   24.11.2016 22:50 - updated to conform new vehicle interface
//   12.12.2016 23:20 - updated with new initial park data format
//


#include <ulib/uDef.h>
#include <ulib/tunic.h>
#include <ulib/uLog.h>
#include <ulib/uFile.h>
#include <ulib/uMemory.h>

#include "gssDef.h"
#include <internal/gssInterface.h>
#include <internal/gssScriptCall.h>

#include "mdlQueue.h"
#include <internal/gtaMisc.h>

#include <internal/gtaVehicle.h>
#include <internal/gtaVehInfo.h>

#include "parkDef.h"

#include "mathUtil.h"



enum MARKERCOLOR {
    MC_OPAQUE_RED = 0xFF003FFF,
    MC_OPAQUE_GREEN = 0xFF1FFF1F,
    MC_OPAQUE_BLUE = 0xFFFF3F00,
    MC_TRANSP_BLUE = 0x5FFF3F00,
    MC_OPAQUE_WHITE = 0xFFFFFFFF,
    MC_OPAQUE_BLACK = 0xFF000000,
};
enum MARKERSETTYPE {
    MST_ONE_MARKER = FALSE,
    MST_MARKER_SET = TRUE,
};
struct PARKSPACEMARKERCFG {
    DWORD type;
    FLOAT radiusMultiplier;
    ByteVector4 color;
    FLOAT pulsation;
    BOOL markerSet;     // set of 3 markers
    DWORD zero1;
    DWORD zero2;
    DWORD zero3;
};

enum PARKSPACEMARKERTYPE {
    PSM_AVAILABLE,
    PSM_LOAD_FAILED,
    PSM_VEH_APPROVED,
    PSM_VEH_REJECTED,
    PSM_CLEAR_START,
    PSM_CLEAR_CONFIRM,
    //
    NUM_PARKSPACEMARKERTYPES,
};
PARKSPACEMARKERCFG parkMarkerCfg[NUM_PARKSPACEMARKERTYPES] = {
//    {G3MT_CYLINDER, 2.75f, {MC_TRANSP_BLUE},  0.0f,     MST_ONE_MARKER}, // PSM_AVAILABLE
    {G3MT_CYLINDER, 0.5f, {MC_OPAQUE_BLUE},  0.0f,     MST_ONE_MARKER}, // PSM_AVAILABLE
    {G3MT_CYLINDER, 1.0f,  {MC_OPAQUE_BLACK}, 0.25f,    MST_MARKER_SET}, // PSM_LOAD_FAILED
    {G3MT_CYLINDER, 2.75f, {MC_OPAQUE_GREEN}, 0.0f,     MST_ONE_MARKER}, // PSM_VEH_APPROVED
    {G3MT_CYLINDER, 2.75f, {MC_OPAQUE_RED},   0.09375f, MST_ONE_MARKER}, // PSM_VEH_REJECTED
    {G3MT_CYLINDER, 2.75f, {MC_OPAQUE_WHITE}, 0.09375f, MST_MARKER_SET}, // PSM_CLEAR_START
    {G3MT_CYLINDER, 3.5f,  {MC_OPAQUE_RED},   0.25f,    MST_MARKER_SET}, // PSM_CLEAR_CONFIRM
};

void DrawParkingMarker(DWORD type, FLOAT radius, DWORD iParkingSpace, FloatVector3* pPos)
{
    DWORD markIndex;
    FLOAT markRadius;
    PARKSPACEMARKERCFG* psmc;

    psmc = &parkMarkerCfg[type];
    markRadius = radius * psmc->radiusMultiplier;
    switch(type)
    {
    case PSM_AVAILABLE:
        markIndex = iParkingSpace;
        break;
    case PSM_LOAD_FAILED:
        markIndex = (PSD_MAX_LANE_SPACES * 2) + 2;
        break;
    case PSM_VEH_APPROVED:
        markIndex = PSD_MAX_LANE_SPACES + iParkingSpace; // N blue markers + N green/red markers
        break;
    case PSM_VEH_REJECTED:
        markIndex = PSD_MAX_LANE_SPACES + iParkingSpace; // N blue markers + N green/red markers
        break;
    case PSM_CLEAR_START:
        markIndex = (PSD_MAX_LANE_SPACES * 2) + 0;
        break;
    case PSM_CLEAR_CONFIRM:
        markIndex = (PSD_MAX_LANE_SPACES * 2) + 1;
        break;
    }

    switch(psmc->markerSet)
    {
    case MST_ONE_MARKER:
        GtaDrawMarker1(markIndex, psmc->type, pPos, markRadius, psmc->pulsation, &psmc->color, 0x800, 1);
        break;
    case MST_MARKER_SET:
        GtaDrawMarkerSet(markIndex, psmc->type, pPos, markRadius, psmc->pulsation, &psmc->color, 0x800);
        break;
    }
}




enum PARKINGSPACESTATE {
    PSS_EMPTY,              // parking space is free
    PSS_STORING_ALLOWED,    // new vehicle is on parking space (and it's type matches park.space type)
    PSS_STORING_DENIED,     // new vehicle is on parking space (and it's type is NOT matching park.space type)
    PSS_RESERVED,           // vehicle is assigned to parking space
    // parking space is cleared by pressing "horn" twice when player is in ANY vehicle at this parking space
    PSS_CLEARING1,      // player pressed horn [once] to clear this parking space
    PSS_CLEARING2,      // player pressed and released horn [once] to clear this parking space
};
enum VEHICLEHANDLE {
    VEHICLE_NONE = 0,
};
struct PARKSPACEINFO {
    BYTE state;
    BYTE vehTypesAllowed;
    BYTE mqIndex;
    BYTE z03;
    FLOAT markerRadius;
    Vehicle vehicleStored; // spawned car [need to be released]
    Vehicle vehicleToSave; // player's car [no need to release]
//    FloatVector3 pos;
//    FLOAT angle;
    FloatVector3 position;
    SByteVector3 rotation;
    BYTE z1F;
};

enum PARKINGLOTSTATE {
    PLS_INACTIVE,   // player is outside zoneSpawn
    PLS_ACTIVATING, // player entered zoneSpawn, waiting for vehicle models to load
    PLS_ACTIVE,     // player is inside zoneClear
};
struct PARKINGLOT {     // RUN-TIME parking info
    PARKLOTHDR* pSrcHdr;
    PARKLANE* pSrcLanes;
//    DWORD numSpaces;
    DataBuffer vehSpecRegistry;
//    FloatZOBBox3 bbEnter;   // inner
//    FloatZOBBox3 bbLeave;   // outer
//    FloatVector4* pPosSlots;
    PARKSPACEINFO space[PSD_MAX_PARKING_VEHICLES];
    GVeh30 vehSpecLocal[PSD_MAX_PARKING_VEHICLES];
};



class SSPSX {
private:
    DWORD m_state;
    IVehicle* pIVehicle;
    GtaVehInfo VehInfo;
    DWORD m_numParks;
    PARKINGLOT* m_pParkActive;
    INT m_iParkActive;
    DataBuffer m_srcData;
    MODELQUEUE m_mdlQueue;
    PARKINGLOT m_park[PSD_MAX_PARKINGS];
    UString m_strDataFileName;
    //
    void parkingSpaceInitMarkerInfo(PARKSPACEINFO& space);
    void parkSysDataLoadParkingLanes(PARKINGLOT& park);
    DWORD parkSysDataLoadParking(PARKINGLOT& park, BYTE* pData);
    void parkSysDataLoad(void);
    void parkSysGrantFreeVehicles(void);
    void parkSysUpdateRegistry(void);
    void parkingSpaceTestVehicle(PARKSPACEINFO& space, Vehicle vehicle);
    void parkingSpaceScanVehicle(PARKSPACEINFO& space, VehSpec vehSpec);
    void parkingSpaceClear(PARKSPACEINFO& space, VehSpec vehSpec);
    void parkingSpaceFSM(AStringStream& display, DWORD iSpace, GtaPlayerState& playerState);
    void parkingLotInitializeActivation(DWORD parkIndex);
    bool parkingLotFinalizeActivation(void);
    void parkingLotDeactivate(void);
public:
    SSPSX() :
        m_pParkActive(nullptr),
        m_iParkActive(-1),
        m_state(PLS_INACTIVE),
        pIVehicle(nullptr),
        VehInfo()
    {
    }
    void init(DWORD gtaVersion);
    void parkFSM(AStringStream& display, GtaPlayerState& playerState);
    bool getActiveParkingLot(DWORD* piParkActive);
//    PARKLOTHDR* getParkingLot(DWORD iPark, FloatVector4** ppfvPosSlots);
};

SSPSX sspsx;



void parkSysStateMachine(AStringStream& display, GtaPlayerState& playerState)
{
    sspsx.parkFSM(display, playerState);
}
void parkSysInit(DWORD gtaVersion)
{
    sspsx.init(gtaVersion);
}

bool parkSysGetActiveParking(DWORD* piParkActive)
{
    return sspsx.getActiveParkingLot(piParkActive);
}

//PARKLOTHDR* parkSysGetParkHdr(DWORD iPark, FloatVector4** ppfvPosSlots)
//{
//    return sspsx.getParkingLot(iPark, ppfvPosSlots);
//}



bool SSPSX::getActiveParkingLot(DWORD* piParkActive)
{
    bool bActive;
    bActive = (m_iParkActive >= 0);
    if(bActive != false)
    {
        *piParkActive = m_iParkActive;
    }
    return bActive;
}

//PARKLOTHDRX* SSPSX::getParkingLot(DWORD iPark, FloatVector4** ppfvPosSlots)
//{
//    *ppfvPosSlots = m_park[iPark].pPosSlots;
//    return m_park[iPark].pSrcHdr;
//}

void SSPSX::init(DWORD gtaVersion)
{
    lss << UL::DEBUG << L("ParkSys_init: ") << ulhex(gtaVersion) << UL::ENDL;

    bool bIsInitialParkData = false;
    if(PSD_MAX_PARKING_VEHICLES > MDLQUEUE_SIZE)
    {
        lss << UL::ERROR << L("Parking vehicles maximum (") << PSD_MAX_PARKING_VEHICLES << L(") exceeds model queue length (") << MDLQUEUE_SIZE << L(")") << UL::ENDL;
        return;
    }
    this->pIVehicle = GtaVehicleInit(gtaVersion);
    if(this->pIVehicle == nullptr)
    {
        lss << UL::ERROR << L("Couldn't get vehicle interface") << UL::ENDL;
        return;
    }

    this->VehInfo.init(gtaVersion);

    UStringStream ssFileName;
    ssFileName << T("VSS.") << std::hex << gtaVersion << T(".bin");
    m_strDataFileName = ssFileName.str();
    lss << UL::DEBUG << L("data file name: ") << LString(m_strDataFileName.begin(), m_strDataFileName.end()) << UL::ENDL;

    m_srcData.ptr = (BYTE*) fileLoad(m_strDataFileName, &m_srcData.size);
    if(m_srcData.ptr == nullptr)
    {
        getInitialParkingData(gtaVersion, m_srcData);
        bIsInitialParkData = true;
    }
    this->parkSysDataLoad();
    if(bIsInitialParkData != false)
    {
        this->parkSysGrantFreeVehicles();
        this->parkSysUpdateRegistry();
    }
    fileStore(this, T("SSPSX.binary.data"), sizeof(*this));
}

void SSPSX::parkSysGrantFreeVehicles(void)
{
    for(DWORD iPark = 0; iPark < this->m_numParks; iPark++)
    {
        PARKINGLOT& park = m_park[iPark];
        PARKSPACEINFO& space = park.space[0];
        VehSpec vehSpec = &park.vehSpecLocal[0];
        DWORD model = GenerateFreeVehicleSpec(vehSpec, space.position, space.rotation);
        BYTE vehTypes = this->VehInfo.getVehicleTypes(model);
        BYTE allowedTypes = space.vehTypesAllowed;
//        lss << UL::INFO << L("grantFreeVehicle[") << iPark << L("]: (") << ulhex(vehTypes) << L("&") << ulhex(allowedTypes) << L(")==") << ulhex(static_cast<BYTE>(vehTypes & allowedTypes)) << UL::ENDL;
        if((vehTypes & allowedTypes) == 0)
        {
            this->pIVehicle->clearSpec(vehSpec);
        }
    }
}

void SSPSX::parkingSpaceInitMarkerInfo(PARKSPACEINFO& space)
{
    FLOAT radius = 1.0;
    BYTE vehTypes;
    vehTypes = space.vehTypesAllowed;
    //if((vehTypes & VT_LAND_CYCLE) != 0)
    //{
    //    radius = 1.125;
    //}
    if((vehTypes & VT_LAND_BIKE) != 0)
    {
        radius = 1.125;
    }
    if((vehTypes & VT_LAND_MOTORBIKE) != 0)
    {
        radius = 1.375;
    }
    if((vehTypes & VT_LAND_WHEELED) != 0)
    {
        radius = 1.625;
    }
    if((vehTypes & VT_WATER_BOAT) != 0)
    {
        radius = 3.5;//4.0;
    }
    //if((vehTypes & (VT_AIR_VTOL | VT_AIR_CTOL | VT_AMPHIBIOUS_VTOL | VT_AMPHIBIOUS_CTOL)) != 0)
    if((vehTypes & (VT_AIR_VTOL | VT_AIR_CTOL)) != 0)
    {
        radius = 4.0;//5.0;
    }
    //if((vehTypes & (VT_AIR_VTOL | VT_AIR_CTOL)) != 0)
    //{
    //    radius = 6.0;
    //}
    space.markerRadius = radius;
}

void SSPSX::parkSysDataLoadParkingLanes(PARKINGLOT& park)
{
    DWORD iDstSpace = 0;
    for(DWORD iLane = 0; iLane < park.pSrcHdr->numLanes; iLane++)
    {
        PARKLANE& lane = park.pSrcLanes[iLane];
        FLOAT angle = LANE_UNPACK_ANGLE(lane.spawnAngle);
        SByteVector3 rotation;
        GtaMakePackedRotation(rotation, angle);
        for(DWORD iSrcSpace = 0; iSrcSpace < lane.numSpaces; iSrcSpace++)
        {
            FLOAT fSpaceIndex = static_cast<FLOAT>(iSrcSpace);
            PARKSPACEINFO& space = park.space[iDstSpace];
            iDstSpace++;
            space.vehTypesAllowed = lane.vehTypes;
            space.position.X = lane.fvStart.X + (lane.fvInc.X * fSpaceIndex);
            space.position.Y = lane.fvStart.Y + (lane.fvInc.Y * fSpaceIndex);
            space.position.Z = lane.fvStart.Z + (lane.fvInc.Z * fSpaceIndex);
            space.rotation = rotation;
            this->parkingSpaceInitMarkerInfo(space);
        }        
    }
}

DWORD SSPSX::parkSysDataLoadParking(PARKINGLOT& park, BYTE* pData)
{
    BYTE* pStart = pData;
    // setup pointers
    park.pSrcHdr = static_cast<PARKLOTHDR*>(static_cast<void*>(pData));
    pData += sizeof(PARKLOTHDR);
    park.pSrcLanes = static_cast<PARKLANE*>(static_cast<void*>(pData));
    pData += sizeof(PARKLANE) * park.pSrcHdr->numLanes;
    park.vehSpecRegistry.ptr = pData;
    park.vehSpecRegistry.size = sizeof(GVeh30) * park.pSrcHdr->numSpaces;
    pData += park.vehSpecRegistry.size;

    // unpack pos
    this->parkSysDataLoadParkingLanes(park);
    // fill rest of info
    for(DWORD iVeh = 0; iVeh < park.pSrcHdr->numSpaces; iVeh++)
    {
        PARKSPACEINFO& space = park.space[iVeh];
        space.state = PSS_EMPTY;
        space.mqIndex = 0;
        space.vehicleStored = VEHICLE_NONE;
        space.vehicleToSave = VEHICLE_NONE;
        // load vehicle information
        memcpy(park.vehSpecLocal, park.vehSpecRegistry.ptr, park.vehSpecRegistry.size);
    }
    return (pData - pStart);
}

void SSPSX::parkSysDataLoad(void)
{
    m_state = PLS_INACTIVE;
    // parse data
    BYTE* ptr = m_srcData.ptr;
    PARKSYSHDR* pPSH = static_cast<PARKSYSHDR*>(static_cast<void*>(ptr));
    ptr += sizeof(PARKSYSHDR);
    m_numParks = pPSH->numParks;
    if(m_numParks > PSD_MAX_PARKINGS)
    {
        lss << UL::ERROR << L("Too many parkings(") << m_numParks << L("), maximum is ") << PSD_MAX_PARKINGS << UL::ENDL;
        return;
    }

    // init parking lots
    lss << UL::DEBUG << L("loading ") << m_numParks << L(" parkings") << UL::ENDL;
    m_pParkActive = nullptr;
    m_iParkActive = -1;
    for(DWORD iPark = 0; iPark < m_numParks; iPark++)
    {
        lss << UL::DEBUG << L("loading park[") << ulpad(3, iPark) << L("]") << UL::ENDL;
        ptr += this->parkSysDataLoadParking(m_park[iPark], ptr);
    }
}

void SSPSX::parkSysUpdateRegistry(void)
{
    for(DWORD iPark = 0; iPark < m_numParks; iPark++)
    {
        // store data back to registry
        PARKINGLOT& park = m_park[iPark];
        memcpy(park.vehSpecRegistry.ptr, park.vehSpecLocal, park.vehSpecRegistry.size);
    }
    // dump registry
    fileStore(m_srcData.ptr, m_strDataFileName, m_srcData.size);
}



void SSPSX::parkingSpaceTestVehicle(PARKSPACEINFO& space, Vehicle vehicle)
{
    DWORD model = this->pIVehicle->getModel(vehicle);
    //BYTE vehTypes = VehicleGetTypes(model);
    BYTE vehTypes = this->VehInfo.getVehicleTypes(model);
    lss << UL::DEBUG << "vehTest: got " << ulhex(vehTypes) << ", allowed " << ulhex(space.vehTypesAllowed) << UL::ENDL;
    if((vehTypes & space.vehTypesAllowed) != 0)
    {
        space.vehicleToSave = vehicle;  // temporary save player vehicle [no need to release]
        space.state = PSS_STORING_ALLOWED;
    }
    else
    {
        space.state = PSS_STORING_DENIED;
    }
}

void SSPSX::parkingSpaceScanVehicle(PARKSPACEINFO& space, VehSpec vehSpec)
{
    this->pIVehicle->describe(vehSpec, space.vehicleToSave);
    this->parkSysUpdateRegistry();
    this->pIVehicle->doAlarmShort(space.vehicleToSave);
    this->pIVehicle->stopEngine(space.vehicleToSave);
    space.vehicleToSave = VEHICLE_NONE;  // no need to release -> no need to save handle
    space.state = PSS_RESERVED;
}

void SSPSX::parkingSpaceClear(PARKSPACEINFO& space, VehSpec vehSpec)
{
    this->pIVehicle->release(&space.vehicleStored);    // release vehicle if it was created by parking lot & nullify vehicle handle (nullptr value is okay)
    if(vehSpec)
    {
        this->pIVehicle->clearSpec(vehSpec);
        this->parkSysUpdateRegistry();
    }
    space.vehicleToSave = VEHICLE_NONE;
    space.state = PSS_EMPTY;
}

void SSPSX::parkingSpaceFSM(AStringStream& display, DWORD iSpace, GtaPlayerState& playerState)
{
    PARKSPACEINFO& space = m_pParkActive->space[iSpace];
    VehSpec vehSpec = &m_pParkActive->vehSpecLocal[iSpace];
    BOOL bCancelClear = TRUE;
    FLOAT dist = 1024.0f;   // far away
    if(playerState.bIsInCar)
    {
        dist = distance3d(space.position, playerState.fvPosVehicle);
    }

    switch(space.state)
    {
    case PSS_EMPTY:
        if(playerState.bIsInCar)
        {
            // if player arrived to marker in a vehicle...
            DrawParkingMarker(PSM_AVAILABLE, space.markerRadius, iSpace, &space.position);
            if(dist < space.markerRadius)
            {
                // is it allowed to be stored here?
                this->parkingSpaceTestVehicle(space, playerState.vehiclePlayer);
            }
        }
        break;

    case PSS_STORING_ALLOWED:
        if(playerState.bIsInCar)
        {
            // if player leaved marker in a vehicle...
            DrawParkingMarker(PSM_VEH_APPROVED, space.markerRadius, iSpace, &space.position);
            if(dist >= space.markerRadius)
            {
                this->parkingSpaceClear(space, nullptr);
            }
        }
        else    // ...or he got out and leaved vehicle for us to scan?
        {
            this->parkingSpaceScanVehicle(space, vehSpec);
        }
        break;

    case PSS_STORING_DENIED:
        if(playerState.bIsInCar)
        {
            // player is in inappropriate vehicle... is he leaved (in a vehicle)?
            DrawParkingMarker(PSM_VEH_REJECTED, space.markerRadius, iSpace, &space.position);
            if(dist >= space.markerRadius)
            {
                this->parkingSpaceClear(space, nullptr);
            }
        }
        else    // ...or he got out and we won't scan this definitely inappropriate vehicle?
        {
            this->parkingSpaceClear(space, nullptr);
        }
        break;

    case PSS_RESERVED:
        if(playerState.bIsInCar)
        {
            // the player is in car on a place that has a vehicle scanned... player wants to clear it?
            if(dist < space.markerRadius)
            {
                DrawParkingMarker(PSM_CLEAR_START, space.markerRadius, iSpace, &space.position);
                if(gtaIsPlayerPressingHorn() != FALSE)
                {
                    space.state = PSS_CLEARING1;
                }
            }
        }
        break;

    case PSS_CLEARING1:
        if(playerState.bIsInCar)
        {
            // the player is still in car on a place that has a vehicle scanned... player wants to clear it for sure?
            if(dist < space.markerRadius)
            {
                bCancelClear = FALSE;
                if(gtaIsPlayerPressingHorn() == FALSE)
                {
                    space.state = PSS_CLEARING2;
                }
            }
        }
        if(bCancelClear != FALSE)   // ...or he changed his mind?
        {
            space.state = PSS_RESERVED;
        }
        break;

    case PSS_CLEARING2:
        if(playerState.bIsInCar)
        {
            // the player is still in car on a place that has a vehicle scanned & wants to clear it...
            if(dist < space.markerRadius)
            {
                bCancelClear = FALSE;
                DrawParkingMarker(PSM_CLEAR_CONFIRM, space.markerRadius, iSpace, &space.position);
                if(gtaIsPlayerPressingHorn() != FALSE)
                {
                    this->parkingSpaceClear(space, vehSpec);
                }
            }
        }
        if(bCancelClear != FALSE)   // ...or he changed his mind in a last second?
        {
            space.state = PSS_RESERVED;
        }
        break;
    }
}




void SSPSX::parkingLotInitializeActivation(DWORD parkIndex)
{
    // request all models
    lss << UL::DEBUG << L("plInit: mqInit") << UL::ENDL;
    mqInit(&m_mdlQueue);
    lss << UL::DEBUG << L("plInit: clear & queue models") << UL::ENDL;
    PARKINGLOT& park = m_park[parkIndex];
    for(DWORD iSpace = 0; iSpace < park.pSrcHdr->numSpaces; iSpace++)
    {
        PARKSPACEINFO& space = park.space[iSpace];

        // clear area for parking space
        FloatVector3 fvMin;
        FloatVector3 fvMax;
        FLOAT fBoundary = 5.0f;
        //fvMin.X = park.pPosSlots[iSpace].X - fBoundary;
        //fvMin.Y = park.pPosSlots[iSpace].Y - fBoundary;
        //fvMin.Z = park.pPosSlots[iSpace].Z - fBoundary;
        //fvMax.X = park.pPosSlots[iSpace].X + fBoundary;
        //fvMax.Y = park.pPosSlots[iSpace].Y + fBoundary;
        //fvMax.Z = park.pPosSlots[iSpace].Z + fBoundary;
        fvMin.X = space.position.X - fBoundary;
        fvMin.Y = space.position.Y - fBoundary;
        fvMin.Z = space.position.Z - fBoundary;
        fvMax.X = space.position.X + fBoundary;
        fvMax.Y = space.position.Y + fBoundary;
        fvMax.Z = space.position.Z + fBoundary;
        SN::WORLD::CLEAR_AREA_OF_CARS(fvMin, fvMax);

        // request model (if any)
        DWORD model = park.vehSpecLocal[iSpace].model;
        DWORD spaceState = PSS_EMPTY;
        if(model != 0)
        {
            spaceState = PSS_RESERVED;
            lss << UL::DEBUG << L("plInit: ") << iSpace << " : " << ulhex(model) << UL::ENDL;
            space.mqIndex = mqEnqueueModel(&m_mdlQueue, model);
        }
        space.state = spaceState;
        space.vehicleToSave = VEHICLE_NONE;
        space.vehicleStored = VEHICLE_NONE;
    }
    //ulogf(UL_DEBUG, T("plInit:3"));
    lss << UL::DEBUG << L("plInit: done") << UL::ENDL;
    m_pParkActive = &park;
    m_iParkActive = parkIndex;
    m_state = PLS_ACTIVATING;
}

bool SSPSX::parkingLotFinalizeActivation(void)
{
    lss << UL::DEBUG << L("plFinInit: 1") << UL::ENDL;
    PARKINGLOT& park = *(m_pParkActive);
    bool bStillLoading = (mqAreModelsStillLoading(&m_mdlQueue) != false);

    for(DWORD iSpace = 0; iSpace < park.pSrcHdr->numSpaces; iSpace++)
    {
        PARKSPACEINFO& space = park.space[iSpace];
        VehSpec vehSpec = &park.vehSpecLocal[iSpace];
        if(space.state == PSS_RESERVED)
        {
            if(m_mdlQueue.mqEntry[space.mqIndex].state == MDS_LOADED)
            {
                space.vehicleStored = this->pIVehicle->spawn(vehSpec);
    //            ulogf(UL_INFO, T("spawned car for place %d"), iSpace);
                lss << UL::DEBUG << L("space[") << iSpace << L("] spawned vehicle ") << ulhex(space.vehicleStored) << UL::ENDL;
            }
        }
    }

    lss << UL::DEBUG << L("plFinInit: 2") << UL::ENDL;
    if(bStillLoading)
    {
        mqReleaseLoadedModels(&m_mdlQueue);
    }
    else
    {
        m_state = PLS_ACTIVE;
    }
    return (bStillLoading == false);
}

void SSPSX::parkingLotDeactivate(void)
{
    PARKINGLOT& park = *(m_pParkActive);
    for(DWORD iSpace = 0; iSpace < park.pSrcHdr->numSpaces; iSpace++)
    {
        PARKSPACEINFO& space = park.space[iSpace];
        switch(space.state)
        {
        case PSS_RESERVED:
            this->parkingSpaceClear(space, nullptr);      // release all created vehicles, so they can be destroyed by the game
            break;
        }
    }

    m_pParkActive = nullptr;
    m_iParkActive = -1;
    m_state = PLS_INACTIVE;
}

void SSPSX::parkFSM(AStringStream& display, GtaPlayerState& playerState)
{
    switch(m_state)
    {
    case PLS_INACTIVE:
        display << " inactive";
        for(DWORD iPark = 0; iPark < m_numParks; iPark++)
        {
            PARKINGLOT& park = m_park[iPark];
            if(zobbInside3d(playerState.fvPosChar, park.pSrcHdr->bbEnter))
            {
                lss << UL::INFO << L("player entered spawn zone [") << iPark << L("]") << UL::ENDL;
                this->parkingLotInitializeActivation(iPark);
            }
        }
        break;

    case PLS_ACTIVATING:
        if(this->parkingLotFinalizeActivation() != FALSE)
        {
            lss << UL::INFO << L("spawn finished") << UL::ENDL;
        }
        break;

    case PLS_ACTIVE:
        display << " active(" << m_iParkActive << ")";
        PARKINGLOT& park = *(m_pParkActive);
        if(zobbInside3d(playerState.fvPosChar, park.pSrcHdr->bbLeave))
        {
            for(DWORD iSpace = 0; iSpace < park.pSrcHdr->numSpaces; iSpace++)
            {
                this->parkingSpaceFSM(display, iSpace, playerState);
            }
        }
        else
        {
            lss << UL::INFO << L("player left park zone") << UL::ENDL;
            this->parkingLotDeactivate();
        }
        break;
    }
}

