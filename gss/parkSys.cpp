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
//


#include "uDef.h"
#include "tunic.h"
#include "tstr.h"
#include "uLog.h"
#include "uFile.h"
#include "uMemory.h"

#include "gssDef.h"
#include "gssInterface.h"
#include "gssScriptCall.h"

#include "mdlQueue.h"
#include "gtaMisc.h"

#include "gtaVehicle.h"
#include "gtaVehInfo.h"

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
    {G3MT_CYLINDER, 2.75f, {MC_TRANSP_BLUE},  0.0f,     MST_ONE_MARKER}, // PSM_AVAILABLE
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
    BYTE z3;
    FLOAT markerRadius;
    Vehicle vehicleStored; // spawned car [need to be released]
    Vehicle vehicleToSave; // player's car [no need to release]
    FloatVector3 pos;
    FLOAT angle;
};

enum PARKINGLOTSTATE {
    PLS_INACTIVE,   // player is outside zoneSpawn
    PLS_ACTIVATING, // player entered zoneSpawn, waiting for vehicle models to load
    PLS_ACTIVE,     // player is inside zoneClear
};
struct PARKINGLOT {     // RUN-TIME parking info
    PARKLOTHDR* pSrcHdr;
    DWORD numSpaces;
    DataBuffer vehSpecRegistry;
    FloatZOBBox3 bbEnter;   // inner
    FloatZOBBox3 bbLeave;   // outer
    FloatVector4* pPosSlots;
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
    BOOL parkingLotFinalizeActivation(void);
    void parkingLotDeactivate(void);
public:
    SSPSX()
        : m_pParkActive(nullptr)
        , m_iParkActive(-1)
        , m_state(PLS_INACTIVE)
        , pIVehicle(nullptr)
        , VehInfo()
    {
    }
    void init(DWORD gtaVersion);
    void parkFSM(AStringStream& display, GtaPlayerState& playerState);
    BOOL getActiveParkingLot(DWORD* piParkActive);
    PARKLOTHDR* getParkingLot(DWORD iPark, FloatVector4** ppfvPosSlots);
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

BOOL parkSysGetActiveParking(DWORD* piParkActive)
{
    return sspsx.getActiveParkingLot(piParkActive);
}

PARKLOTHDR* parkSysGetParkHdr(DWORD iPark, FloatVector4** ppfvPosSlots)
{
    return sspsx.getParkingLot(iPark, ppfvPosSlots);
}



BOOL SSPSX::getActiveParkingLot(DWORD* piParkActive)
{
    BOOL bActive;
    bActive = (m_iParkActive >= 0);
    if(bActive != FALSE)
    {
        *piParkActive = m_iParkActive;
    }
    return bActive;
}

PARKLOTHDR* SSPSX::getParkingLot(DWORD iPark, FloatVector4** ppfvPosSlots)
{
    *ppfvPosSlots = m_park[iPark].pPosSlots;
    return m_park[iPark].pSrcHdr;
}

void SSPSX::init(DWORD gtaVersion)
{
    lss << UL::DEBUG << L("ParkSys_init: ") << ulhex(gtaVersion) << UL::ENDL;

    BOOL bIsInitialParkData = FALSE;
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
    ssFileName << T("VSS.") << std::hex << gtaVersion << T(".bin");     // Vehicle Storage and Supply system   [Vintorez rulz]
    m_strDataFileName = ssFileName.str();
    lss << UL::DEBUG << L("data file name: ") << LString(m_strDataFileName.begin(), m_strDataFileName.end()) << UL::ENDL;

    m_srcData.ptr = (BYTE*) fileLoad(m_strDataFileName.c_str(), &m_srcData.size);
    if(m_srcData.ptr == nullptr)
    {
        getInitialParkingData(gtaVersion, m_srcData);
        bIsInitialParkData = TRUE;
    }
    this->parkSysDataLoad();
    if(bIsInitialParkData != FALSE)
    {
        this->parkSysGrantFreeVehicles();
        this->parkSysUpdateRegistry();
    }
    fileStore(this, T("SSPSX.binary.data"), sizeof(*this));
}

void SSPSX::parkSysGrantFreeVehicles(void)
{
/*    DWORD iPark;
    PARKINGLOT* pPark;
    VEHLIC31* pFreeVehLicense;
    BYTE vehTypeFree;
    BYTE vehTypesAllowed;
    // Hey! It's a free vehicle day!
    for(iPark = 0; iPark < this->numParks; iPark++)
    {
        pPark = &this->park[iPark];
        pFreeVehLicense = GetFreeVehicleLicense();
        if(pFreeVehLicense != nullptr)
        {
            vehTypeFree = VehicleGetTypes(pFreeVehLicense->model);
            vehTypesAllowed = pPark->space[0].vehTypesAllowed;
            ulogf(UL_INFO, T("grantFreeVehicle(%d): (%02X&%02X)==%02X"), iPark, vehTypesAllowed, vehTypeFree, (vehTypesAllowed & vehTypeFree));
            if((vehTypesAllowed & vehTypeFree) != 0)
            {
                pPark->vehLicense[0] = *pFreeVehLicense;
            }
        }
    }*/
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
    // unpack pos
    FloatVector4* pStartPos;
    FloatVector4* pIncPos;
    FloatVector4* pPointPos;
    DWORD iPos = 0;
    DWORD iDstSpace = 0;
    for(DWORD iLane = 0; iLane < park.pSrcHdr->numLanes; iLane++)
    {
        PARKLANEINFO& laneInfo = park.pSrcHdr->laneInfo[iLane];
        BYTE vehTypes = laneInfo.vehTypes;
        BYTE laneNumSpaces = laneInfo.laneType & PSD_LANE_NUMSPACES_MASK;
        BYTE laneType = laneInfo.laneType & PSD_LANE_TYPE_MASK;
        //lss << UL::INFO << L("loadpark[") << ulpad(3, iPark) << L(".") << iLane << L("]: ")
        //    << ulhex(vehTypes) << L(",") << ulhex(pLaneInfo->laneType)
        //    << L(" [N") << ulpad(laneNumSpaces) << L(" T") << ulpad(laneType) << L("]") << UL::ENDL;
        switch(laneType)
        {
        case PSD_LANE_ROW:
            pStartPos = &park.pPosSlots[iPos];
            iPos++;
            pIncPos = &park.pPosSlots[iPos];
            iPos++;
            for(DWORD iSrcSpace = 0; iSrcSpace < laneNumSpaces; iSrcSpace++)
            {
                FLOAT fSpaceIndex = static_cast<FLOAT>(iSrcSpace);
                PARKSPACEINFO& space = park.space[iDstSpace];
                iDstSpace++;
                space.vehTypesAllowed = vehTypes;
                space.pos.X = pStartPos->X + (pIncPos->X * fSpaceIndex);
                space.pos.Y = pStartPos->Y + (pIncPos->Y * fSpaceIndex);
                space.pos.Z = pStartPos->Z + (pIncPos->Z * fSpaceIndex);
                space.angle = pStartPos->A + (pIncPos->A * fSpaceIndex);
                this->parkingSpaceInitMarkerInfo(space);
            }
            break;
        case PSD_LANE_POINTS:
            for(DWORD iSrcSpace = 0; iSrcSpace < laneNumSpaces; iSrcSpace++)
            {
                pPointPos = &park.pPosSlots[iPos];
                iPos++;
                PARKSPACEINFO& space = park.space[iDstSpace];
                iDstSpace++;
                space.vehTypesAllowed = vehTypes;
                space.pos.X = pPointPos->X;
                space.pos.Y = pPointPos->Y;
                space.pos.Z = pPointPos->Z;
                space.angle = pPointPos->A;
                this->parkingSpaceInitMarkerInfo(space);
            }
            break;
        }
    }
}

DWORD SSPSX::parkSysDataLoadParking(PARKINGLOT& park, BYTE* pData)
{
    BYTE* pStart = pData;
    // setup pointers
    park.pSrcHdr = static_cast<PARKLOTHDR*>(static_cast<void*>(pData));
    pData += sizeof(PARKLOTHDR);
    park.pPosSlots = static_cast<FloatVector4*>(static_cast<void*>(pData));
    pData += sizeof(FloatVector4) * park.pSrcHdr->numPosSlots;
    park.vehSpecRegistry.ptr = pData;
    park.vehSpecRegistry.size = sizeof(GVeh30) * park.pSrcHdr->numVehicles;
    pData += park.vehSpecRegistry.size;
    // load header
    park.numSpaces = park.pSrcHdr->numVehicles;
    park.bbEnter = park.pSrcHdr->bbEnter;
    park.bbLeave = park.pSrcHdr->bbLeave;

    // unpack pos
    this->parkSysDataLoadParkingLanes(park);
    // fill rest of info
    for(DWORD iVeh = 0; iVeh < park.numSpaces; iVeh++)
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
    fileStore(m_srcData.ptr, m_strDataFileName.c_str(), m_srcData.size);
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
        dist = distance3d(space.pos, playerState.fvPosVehicle);
    }

    switch(space.state)
    {
    case PSS_EMPTY:
        if(playerState.bIsInCar)
        {
            // if player arrived to marker in a vehicle...
            DrawParkingMarker(PSM_AVAILABLE, space.markerRadius, iSpace, &space.pos);
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
            DrawParkingMarker(PSM_VEH_APPROVED, space.markerRadius, iSpace, &space.pos);
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
            DrawParkingMarker(PSM_VEH_REJECTED, space.markerRadius, iSpace, &space.pos);
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
                DrawParkingMarker(PSM_CLEAR_START, space.markerRadius, iSpace, &space.pos);
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
                DrawParkingMarker(PSM_CLEAR_CONFIRM, space.markerRadius, iSpace, &space.pos);
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
    for(DWORD iSpace = 0; iSpace < park.numSpaces; iSpace++)
    {
        PARKSPACEINFO& space = park.space[iSpace];

        // clear area for parking space
        FloatVector3 fvMin;
        FloatVector3 fvMax;
        FLOAT fBoundary = 5.0f;
        fvMin.X = park.pPosSlots[iSpace].X - fBoundary;
        fvMin.Y = park.pPosSlots[iSpace].Y - fBoundary;
        fvMin.Z = park.pPosSlots[iSpace].Z - fBoundary;
        fvMax.X = park.pPosSlots[iSpace].X + fBoundary;
        fvMax.Y = park.pPosSlots[iSpace].Y + fBoundary;
        fvMax.Z = park.pPosSlots[iSpace].Z + fBoundary;
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

BOOL SSPSX::parkingLotFinalizeActivation(void)
{
    lss << UL::DEBUG << L("plFinInit: 1") << UL::ENDL;
    PARKINGLOT& park = *(m_pParkActive);
    BOOL bFinishedLoading = (mqAreModelsStillLoading(&m_mdlQueue) == FALSE);

    for(DWORD iSpace = 0; iSpace < park.numSpaces; iSpace++)
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
    if(bFinishedLoading == FALSE)
    {
    //    ulogf(UL_INFO, T("spawn finished"));
        mqReleaseLoadedModels(&m_mdlQueue);
    }
    else
    {
        m_state = PLS_ACTIVE;
    }
    return bFinishedLoading;
}

void SSPSX::parkingLotDeactivate(void)
{
    PARKINGLOT& park = *(m_pParkActive);
    for(DWORD iSpace = 0; iSpace < park.numSpaces; iSpace++)
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
            if(zobbInside3d(playerState.fvPosChar, park.bbEnter))
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
        if(zobbInside3d(playerState.fvPosChar, park.bbLeave))
        {
            for(DWORD iSpace = 0; iSpace < park.numSpaces; iSpace++)
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

