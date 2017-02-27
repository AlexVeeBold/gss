///////////////////////////////////////////////////////////////
//
// gtaVehicle: vehicle stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:39 - created (moved from gssMain)
//   27.11.2016 11:49 - fixed recently added VC CVehicle layout
//   03.12.2016 22:54 - renamed VehicleG3 to VehicleIII
//   04.12.2016 01:44 - recreated GenerateFreeVehicleSpec
//   12.12.2016 23:13 - now using new packed-rotation routines
//


#include <ulib/uDef.h>
#include <ulib/tunic.h>
#include <ulib/uLog.h>
#include <ulib/uRandom.h>

#include "gssDef.h"
#include "gssInterface.h"
#include "gssScriptCall.h"

#include "gtaVersion.h"

#include "gtaVehInfo.h"

#include "parkDef.h"

#include "gtaVehicle.h"

#include "gtaMisc.h"



struct GtaMatrix4 {
    FLOAT m11;
    FLOAT m12;
    FLOAT m13;
    FLOAT m14;
    FLOAT m21;
    FLOAT m22;
    FLOAT m23;
    FLOAT m24;
    FLOAT m31;
    FLOAT m32;
    FLOAT m33;
    FLOAT m34;
    FLOAT m41;
    FLOAT m42;
    FLOAT m43;
    FLOAT m44;
};


struct G30CEntity {
    // vmt
    BYTE zz0000[0x4];
    // placeable
    GtaMatrix4 matrix;      // [0004]
    BYTE zz0044[0x8];
    // entity
    BYTE zz004C[0x4];
    BYTE flags50;           // [0050]
    BYTE flags51;           // [0051]
    BYTE proof_flags52;     // [0052] 0x2 explosion-proof
    BYTE proof_flags53;     // [0053] 0x1 bulletproof, 0x2 fireproof, 0x4 collision-proof, 0x8 melee-proof
    BYTE zz0054[0x8];
    WORD model;             // [005C]
    BYTE zz005E[0x6];
};

struct GCVehicleIII {
    G30CEntity entity;
    // physical
    BYTE zz0064[0xC4];
    // vehicle
    BYTE zz0128[0x24];
    DWORD creationTime;     // [014C] in-game time
    BYTE zz0150[0x4C];
    BYTE color1;            // [019C]
    BYTE color2;            // [019D]
    BYTE variation1;        // [019E]
    BYTE variation2;        // [019F]
    WORD alarmDuration;     // [01A0] "1 second" is about 1000      (automobiles, not boats)
    BYTE zz01A2[0x2];
    BYTE* pDriver;          // [01A4]
    BYTE* pPassenger[8];    // [01A8]
    BYTE numPassengers;     // [01C8]
    BYTE zz01C9[0x3];
    BYTE maxPassengers;     // [01CC]
    BYTE zz01CD[0x27];
    BYTE spawnType;         // [01F4] 1:traffic/garage, 2:mission(scripted), 3:parked(cargen)
    BYTE equipmentFlags;    // [01F5] 0x10 engine is on, 0x20 handbrake is on, 0x40 lights is on (visible with engine enabled)
    BYTE zz01F6[0xA];
    FLOAT health;           // [0200] 0.0 ... 1000.0
    BYTE gear;              // [0204] 0:"rear", 1,2,3..:"drive"
    BYTE zz0205[0x1F];
    DWORD doorStatus;       // [0224] 1:unlocked, ..., 5:outside-locked (cop car doors), ...
    BYTE zz0228[0x1];
    BYTE radiostation;      // [0229]
    BYTE zz022A[0x5A];
    DWORD vehicleType;      // [0284] 0:VEHTYPE_CAR, 1:VEHTYPE_BOAT, 2:VEHTYPE_TRAIN, 3:VEHTYPE_HELI, 4:VEHTYPE_PLANE, 5:VEHTYPE_BIKE
};

struct GCVehicleVC {
    G30CEntity entity;
    // physical
    BYTE zz0064[0xBC];
    // vehicle
    BYTE zz0120[0x28];
    DWORD aiCommandTime;    // [0148] in-game time
    BYTE zz014C[0x54];
    BYTE color1;            // [01A0]
    BYTE color2;            // [01A1]
    BYTE variation1;        // [01A2]
    BYTE variation2;        // [01A3]
    WORD alarmDuration;     // [01A4] "1 second" is about 1000      (automobiles + bikes, not boats/helicopters/planes)
    BYTE zz01A6[0x2];
    BYTE* pDriver;          // [01A8]
    BYTE* pPassenger[8];    // [01AC]
    BYTE numPassengers;     // [01CC]
    BYTE zz01CD[0x3];
    BYTE maxPassengers;     // [01D0]
    BYTE zz01D1[0x27];
    BYTE spawnType;         // [01F8] 1:traffic/garage, 2:mission(scripted), 3:parked(cargen)
    BYTE equipmentFlags;    // [01F9] 0x10 engine is on, 0x20 handbrake is on, 0x40 lights is on (visible with engine enabled)
    BYTE zz01FA[0x3];
    BYTE tiresFlags;        // [01FD]  0x02 tires unvulnerable
    BYTE bombFlags;         // [01FE] 0x07: bomb type (bitfield)
    BYTE zz01FF[0x5];
    FLOAT health;           // [0204] 0.0 ... 1000.0
    BYTE gear;              // [0208] 0:"rear", 1,2,3..:"drive"
    BYTE zz0209[0x27];
    DWORD doorStatus;       // [0230] 1:unlocked, ..., 5:outside-locked (cop car doors), ...
    BYTE zz0234[0x8];
    BYTE radiostation;      // [023C]
    BYTE zz023D[0x5F];
    DWORD vehicleType;      // [029C] 0:CAR/HELI, 1:BOAT/PLANE,  5:BIKE
};

enum G3BITS {
    III_0053_BULLET_PROOF = 0x01,
    III_0053_FIRE_PROOF = 0x02,
    III_0052_EXPLOSION_PROOF = 0x02,
    III_0053_COLLISION_PROOF = 0x04,
    III_0053_MELEE_PROOF = 0x08,
    III_01F5_ENGINE_IS_ON = 0x10,
    III_01F5_LIGHTS_ARE_ON = 0x40,
    III_04D9_BOMB_MASK = 0x07,

    VC_0053_BULLET_PROOF = 0x02,
    VC_0053_FIRE_PROOF = 0x04,
    VC_0052_EXPLOSION_PROOF = 0x02,
    VC_0053_COLLISION_PROOF = 0x08,
    VC_0053_MELEE_PROOF = 0x10,
    VC_01FD_PROTECTED_TIRES = 0x02,
    VC_01F9_ENGINE_IS_ON = 0x10,
    VC_01F9_LIGHTS_ARE_ON = 0x40,
    VC_01FE_BOMB_MASK = 0x07,
};

enum G3CVehicleSpawnType {  // aka missionType
    VEHSPAWNTYPE_REGULAR = 1,   // vehicle will be removed as regular traffic vehicle
    VEHSPAWNTYPE_SCRIPTED = 2,  // vehicle will be removed after being unreferenced by script
    VEHSPAWNTYPE_PARKED = 3,    // vehicle will be removed as regular traffic vehicle
};
enum GCVehicleTypeIII {   // class type, may be different from "real" vehicle type (i.e. plane could be a "boat" class type)
    G3VT_AUTOMOBILE,
    G3VT_BOAT,
};

struct GCAutomobileIII {
    GCVehicleIII vehicle;
    // automobile
    BYTE zz0288[0x251];
    BYTE equipmentFlags;    // [04D9] 0x80 watertight (no harm peds in drowning car, except player)   0x20 NOT resprayable   0x08 taxi lights bit   0x07 bomb type bitfield
    BYTE zz04DA[0xCE];
};

//struct GCBoatIII {
//    GCVehicleIII vehicle;
//    // boat
//    BYTE zz0288[0x39];
//    BYTE isAnchored;        // [02C1] set by 0323_ANCHOR_BOAT
//    BYTE zz02C2[0x1C2];
//};



void VehicleLoadMatrix(GtaMatrix4& mat, const GVeh30& vehInfo)
{
//    FLOAT scaleFactor = 100.0f;
    FloatVector3 rotation;
//    rotation.X = static_cast<FLOAT>(vehInfo.rotation.X) / scaleFactor;
//    rotation.Y = static_cast<FLOAT>(vehInfo.rotation.Y) / scaleFactor;
//    rotation.Z = static_cast<FLOAT>(vehInfo.rotation.Z) / scaleFactor;
    GtaUnpackRotation(rotation, vehInfo.rotation);
    mat.m11 = rotation.Y;
    mat.m12 = -rotation.X;
    mat.m13 = 0.0f;
    mat.m21 = rotation.X;
    mat.m22 = rotation.Y;
    mat.m23 = rotation.Z;
    mat.m31 = 0.0f;
    mat.m32 = 0.0f;
    mat.m33 = 1.0f;
    mat.m41 = vehInfo.position.X;
    mat.m42 = vehInfo.position.Y;
    mat.m43 = vehInfo.position.Z;
}

void VehicleStoreMatrix(GVeh30& vehInfo, const GtaMatrix4& mat)
{
//    FLOAT scaleFactor = 100.0f;
    FloatVector3 rotation;
    vehInfo.position.X = mat.m41;
    vehInfo.position.Y = mat.m42;
    vehInfo.position.Z = mat.m43;
//    vehInfo.rotation.X = static_cast<SBYTE>(mat.m21 * scaleFactor);
//    vehInfo.rotation.Y = static_cast<SBYTE>(mat.m22 * scaleFactor);
//    vehInfo.rotation.Z = static_cast<SBYTE>(mat.m23 * scaleFactor);
    rotation.X = mat.m21;
    rotation.Y = mat.m22;
    rotation.Z = mat.m23;
    GtaPackRotation(vehInfo.rotation, rotation);
}


#include <ulib/iimpl.h>
class VehicleIII : public IVehicle {
private:
    // no data members
public:
    IENTRY DWORD ICALL getVehSpecSize(void) IPURE;
    IENTRY void ICALL clearSpec(VehSpec vehSpec) IPURE;
    IENTRY void ICALL describe(VehSpec vehSpec, Vehicle vehicle) IPURE;
    IENTRY Vehicle ICALL spawn(VehSpec vehSpec) IPURE;
    IENTRY DWORD ICALL getModel(Vehicle vehicle) IPURE;
    IENTRY void ICALL doAlarmShort(Vehicle vehicle) IPURE;   // if possible for this vehicle type
    IENTRY void ICALL stopEngine(Vehicle vehicle) IPURE;
    IENTRY void ICALL release(Vehicle* pVehicle) IPURE;
};

#include <ulib/iimpl.h>
class VehicleVC : public VehicleIII {
private:
    // no data members
public:
    //IENTRY DWORD ICALL getVehSpecSize(void) IPURE;
    //IENTRY void ICALL clearSpec(VehSpec vehSpec) IPURE;
    IENTRY void ICALL describe(VehSpec vehSpec, Vehicle vehicle) IPURE;
    IENTRY Vehicle ICALL spawn(VehSpec vehSpec) IPURE;
    //IENTRY DWORD ICALL getModel(Vehicle vehicle) IPURE;
    IENTRY void ICALL doAlarmShort(Vehicle vehicle) IPURE;   // if possible for this vehicle type
    IENTRY void ICALL stopEngine(Vehicle vehicle) IPURE;
    //IENTRY void ICALL release(Vehicle* pVehicle) IPURE;
};


#define VEH_DEBUG
//#undef VEH_DEBUG
//#define VEH_DEBUG_MORE
#undef VEH_DEBUG_MORE

#ifdef VEH_DEBUG
#define VLOG(x)     x;
#   ifdef VEH_DEBUG_MORE
#   define VLOGX(x)     x;
#   else
#   define VLOGX(x)     // nothing
#   endif
#else
#define VLOG(x)     // nothing
#define VLOGX(x)    // nothing
#endif

DWORD VehicleIII::getVehSpecSize(void)
{
    return sizeof(GVeh30);
}

// VC: reuse III



void VehicleIII::clearSpec(VehSpec vehSpec)
{
    memset(vehSpec, 0, sizeof(GVeh30));
}

// VC: reuse III



void VehicleIII::describe(VehSpec vehSpec, Vehicle vehicle)
{
    VLOG(lss << UL::DEBUG << L("VehG3_describe: ") << ulhex(vehicle) << UL::ENDL);
    GCVehicleIII* pCVehicle = static_cast<GCVehicleIII*>(GtaGetCVehiclePtr(vehicle));
    GVeh30* pVeh30 = static_cast<GVeh30*>(vehSpec);

    VLOG(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
    VLOG(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);
    VehicleStoreMatrix(*pVeh30, pCVehicle->entity.matrix);

    VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);

    VLOG(lss << UL::DEBUG << L(".mdl: ") << ulhex(pCVehicle->entity.model) << UL::ENDL);
    pVeh30->model = pCVehicle->entity.model;

    VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->creationTime) << UL::ENDL);
    pVeh30->creationTime = pCVehicle->creationTime;

    VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".var1: ") << ulhex(pCVehicle->variation1) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".var2: ") << ulhex(pCVehicle->variation2) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);
    pVeh30->color1 = pCVehicle->color1;
    pVeh30->color2 = pCVehicle->color2;
    pVeh30->variation1 = pCVehicle->variation1;
    pVeh30->variation2 = pCVehicle->variation2;
    pVeh30->radiostation = pCVehicle->radiostation;

    VLOGX(lss << UL::DEBUG << L(".f1F5equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);

    VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);
    BYTE proofFlags = 0;
    if(pCVehicle->entity.proof_flags53 & III_0053_BULLET_PROOF)
    {
        proofFlags |= GVP_BULLET_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & III_0053_FIRE_PROOF)
    {
        proofFlags |= GVP_FIRE_PROOF;
    }
    if(pCVehicle->entity.proof_flags52 & III_0052_EXPLOSION_PROOF)
    {
        proofFlags |= GVP_EXPLOSION_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & III_0053_COLLISION_PROOF)
    {
        proofFlags |= GVP_COLLISION_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & III_0053_MELEE_PROOF)
    {
        proofFlags |= GVP_MELEE_PROOF;
    }
    pVeh30->proofs = proofFlags;

    pVeh30->autoBombType = 0;
    if(pCVehicle->vehicleType == G3VT_AUTOMOBILE)
    {
        GCAutomobileIII* pCAutomobile = static_cast<GCAutomobileIII*>(static_cast<void*>(pCVehicle));
        VLOGX(lss << UL::DEBUG << L(".f4D9bomb: ") << ulhex(pCAutomobile->equipmentFlags) << UL::ENDL);
        pVeh30->autoBombType = pCAutomobile->equipmentFlags & III_04D9_BOMB_MASK;
    }
    VLOG(lss << UL::DEBUG << L("VehG3_describe done") << UL::ENDL);
}

Vehicle VehicleIII::spawn(VehSpec vehSpec)
{
    VLOG(lss << UL::DEBUG << L("VehG3_spawn: [") << ulhex(vehSpec) << L("]") << UL::ENDL);
    Vehicle vehicle = 0;
    GVeh30* pVeh30 = static_cast<GVeh30*>(vehSpec);
    GtaSetNextVehicleVariation(pVeh30->variation1, pVeh30->variation2);
    SN::VEHICLE::CREATE_CAR(pVeh30->model, pVeh30->position, &vehicle);
    GCVehicleIII* pCVehicle = static_cast<GCVehicleIII*>(GtaGetCVehiclePtr(vehicle));
    if(pCVehicle != nullptr)
    {
        VLOG(lss << UL::DEBUG << L(".mdl: ") << ulhex(pCVehicle->entity.model) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);
        VehicleLoadMatrix(pCVehicle->entity.matrix, *pVeh30);
        VLOG(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
        VLOG(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);
        pCVehicle->entity.flags50 &= 0x07; // by garage code
        pCVehicle->entity.flags50 |= 0x20; // by garage code
        VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->creationTime) << UL::ENDL);
        pCVehicle->creationTime = pVeh30->creationTime;
        VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->creationTime) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".drvr: ") << ulhex(pCVehicle->pDriver) << UL::ENDL);
        pCVehicle->pDriver = nullptr; // cleared by garage code
        VLOGX(lss << UL::DEBUG << L(".drvr: ") << ulhex(pCVehicle->pDriver) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
        pCVehicle->color1 = pVeh30->color1;
        VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
        pCVehicle->color2 = pVeh30->color2;
        VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);
        pCVehicle->radiostation = pVeh30->radiostation;
        VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".f1F5equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);
        pCVehicle->equipmentFlags &= ~0x80; // flag cleared by garage code
        pCVehicle->equipmentFlags &= ~III_01F5_ENGINE_IS_ON;     // turn off engine
        VLOGX(lss << UL::DEBUG << L(".f1F5equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);

        BYTE proofFlags;
        VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
        pCVehicle->entity.proof_flags52 &= ~III_0052_EXPLOSION_PROOF;
        if(pVeh30->proofs & GVP_EXPLOSION_PROOF)
        {
            pCVehicle->entity.proof_flags52 |= III_0052_EXPLOSION_PROOF;
        }
        VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);
        proofFlags = pCVehicle->entity.proof_flags53 & ~(III_0053_BULLET_PROOF | III_0053_FIRE_PROOF | III_0053_COLLISION_PROOF | III_0053_MELEE_PROOF);
        if(pVeh30->proofs & GVP_BULLET_PROOF)
        {
            proofFlags |= III_0053_BULLET_PROOF;
        }
        if(pVeh30->proofs & GVP_FIRE_PROOF)
        {
            proofFlags |= III_0053_FIRE_PROOF;
        }
        if(pVeh30->proofs & GVP_COLLISION_PROOF)
        {
            proofFlags |= III_0053_COLLISION_PROOF;
        }
        if(pVeh30->proofs & GVP_MELEE_PROOF)
        {
            proofFlags |= III_0053_MELEE_PROOF;
        }
        pCVehicle->entity.proof_flags53 = proofFlags;
        VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);

        if(pCVehicle->vehicleType == G3VT_AUTOMOBILE)
        {
            GCAutomobileIII* pCAutomobile = static_cast<GCAutomobileIII*>(static_cast<void*>(pCVehicle));
            VLOGX(lss << UL::DEBUG << L(".f4D9bomb: ") << ulhex(pCAutomobile->equipmentFlags) << UL::ENDL);
            pCAutomobile->equipmentFlags = (pCAutomobile->equipmentFlags & ~III_04D9_BOMB_MASK) | (pVeh30->autoBombType & III_04D9_BOMB_MASK);
            VLOGX(lss << UL::DEBUG << L(".f4D9bomb: ") << ulhex(pCAutomobile->equipmentFlags) << UL::ENDL);
        }
    }
    else
    {
        vehicle = 0;
    }

    VLOG(lss << UL::DEBUG << L("VehG3_spawn: ") << ulhex(vehicle) << UL::ENDL);
    return vehicle;
}


// // // // // // // // // // // // // // // // VICE // // // // // // // // // // // // // // // //

void VehicleVC::describe(VehSpec vehSpec, Vehicle vehicle)
{
    VLOG(lss << UL::DEBUG << L("VehVC_describe: ") << ulhex(vehicle) << UL::ENDL);
    GCVehicleVC* pCVehicle = static_cast<GCVehicleVC*>(GtaGetCVehiclePtr(vehicle));
    GVeh30* pVeh30 = static_cast<GVeh30*>(vehSpec);

    VLOG(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
    VLOG(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);
    VehicleStoreMatrix(*pVeh30, pCVehicle->entity.matrix);

    VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);

    VLOG(lss << UL::DEBUG << L(".mdl: ") << ulhex(pCVehicle->entity.model) << UL::ENDL);
    pVeh30->model = pCVehicle->entity.model;

    VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->aiCommandTime) << UL::ENDL);
    pVeh30->creationTime = pCVehicle->aiCommandTime;

    VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".var1: ") << ulhex(pCVehicle->variation1) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".var2: ") << ulhex(pCVehicle->variation2) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);
    pVeh30->color1 = pCVehicle->color1;
    pVeh30->color2 = pCVehicle->color2;
    pVeh30->variation1 = pCVehicle->variation1;
    pVeh30->variation2 = pCVehicle->variation2;
    pVeh30->radiostation = pCVehicle->radiostation;

    VLOGX(lss << UL::DEBUG << L(".f1F9equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);

    VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L(".f1FDtires: ") << ulhex(pCVehicle->tiresFlags) << UL::ENDL);
    BYTE proofFlags = 0;
    if(pCVehicle->entity.proof_flags53 & VC_0053_BULLET_PROOF)
    {
        proofFlags |= GVP_BULLET_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & VC_0053_FIRE_PROOF)
    {
        proofFlags |= GVP_FIRE_PROOF;
    }
    if(pCVehicle->entity.proof_flags52 & VC_0052_EXPLOSION_PROOF)
    {
        proofFlags |= GVP_EXPLOSION_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & VC_0053_COLLISION_PROOF)
    {
        proofFlags |= GVP_COLLISION_PROOF;
    }
    if(pCVehicle->entity.proof_flags53 & VC_0053_MELEE_PROOF)
    {
        proofFlags |= GVP_MELEE_PROOF;
    }
    if(pCVehicle->tiresFlags & VC_01FD_PROTECTED_TIRES)
    {
        proofFlags |= GVP_PROTECTED_TIRES;
    }
    pVeh30->proofs = proofFlags;

    pVeh30->autoBombType = 0;
    if(pCVehicle->vehicleType == G3VT_AUTOMOBILE)
    {
        VLOGX(lss << UL::DEBUG << L(".f1FEbomb: ") << ulhex(pCVehicle->bombFlags) << UL::ENDL);
        pVeh30->autoBombType = pCVehicle->bombFlags & VC_01FE_BOMB_MASK;
    }
    VLOG(lss << UL::DEBUG << L("VehVC_describe done") << UL::ENDL);
}

Vehicle VehicleVC::spawn(VehSpec vehSpec)
{
    VLOG(lss << UL::DEBUG << L("VehVC_spawn: [") << ulhex(vehSpec) << L("]") << UL::ENDL);
    Vehicle vehicle = 0;
    GVeh30* pVeh30 = static_cast<GVeh30*>(vehSpec);
    GtaSetNextVehicleVariation(pVeh30->variation1, pVeh30->variation2);
    SN::VEHICLE::CREATE_CAR(pVeh30->model, pVeh30->position, &vehicle);
    GCVehicleVC* pCVehicle = static_cast<GCVehicleVC*>(GtaGetCVehiclePtr(vehicle));
    if(pCVehicle != nullptr)
    {
        VLOG(lss << UL::DEBUG << L(".mdl: ") << ulhex(pCVehicle->entity.model) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);
        VehicleLoadMatrix(pCVehicle->entity.matrix, *pVeh30);
        VLOG(lss << UL::DEBUG << L(".pos: ") << pCVehicle->entity.matrix.m41 << ", " << pCVehicle->entity.matrix.m42 << ", " << pCVehicle->entity.matrix.m43 << UL::ENDL);
        VLOG(lss << UL::DEBUG << L(".rot: ") << pCVehicle->entity.matrix.m21 << ", " << pCVehicle->entity.matrix.m22 << ", " << pCVehicle->entity.matrix.m23 << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);
        pCVehicle->entity.flags50 &= 0x07; // by garage code
        pCVehicle->entity.flags50 |= 0x20; // by garage code
        VLOGX(lss << UL::DEBUG << L(".f50: ") << ulhex(pCVehicle->entity.flags50) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->aiCommandTime) << UL::ENDL);
        pCVehicle->aiCommandTime = pVeh30->creationTime;
        VLOGX(lss << UL::DEBUG << L(".crtime: ") << ulhex(pCVehicle->aiCommandTime) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".drvr: ") << ulhex(pCVehicle->pDriver) << UL::ENDL);
        pCVehicle->pDriver = nullptr; // cleared by garage code
        VLOGX(lss << UL::DEBUG << L(".drvr: ") << ulhex(pCVehicle->pDriver) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
        pCVehicle->color1 = pVeh30->color1;
        VLOGX(lss << UL::DEBUG << L(".c1: ") << ulhex(pCVehicle->color1) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
        pCVehicle->color2 = pVeh30->color2;
        VLOGX(lss << UL::DEBUG << L(".c2: ") << ulhex(pCVehicle->color2) << UL::ENDL);
        VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);
        pCVehicle->radiostation = pVeh30->radiostation;
        VLOGX(lss << UL::DEBUG << L(".radio: ") << ulhex(pCVehicle->radiostation) << UL::ENDL);

        VLOGX(lss << UL::DEBUG << L(".f1F9equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);
        pCVehicle->equipmentFlags &= ~0x80; // flag cleared by garage code
        pCVehicle->equipmentFlags &= ~VC_01F9_ENGINE_IS_ON;     // turn off engine
        VLOGX(lss << UL::DEBUG << L(".f1F9equip: ") << ulhex(pCVehicle->equipmentFlags) << UL::ENDL);

        BYTE proofFlags;
        VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
        pCVehicle->entity.proof_flags52 &= ~VC_0052_EXPLOSION_PROOF;
        if(pVeh30->proofs & GVP_EXPLOSION_PROOF)
        {
            pCVehicle->entity.proof_flags52 |= VC_0052_EXPLOSION_PROOF;
        }
        VLOGX(lss << UL::DEBUG << L(".f52: ") << ulhex(pCVehicle->entity.proof_flags52) << UL::ENDL);
        //
        VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);
        proofFlags = pCVehicle->entity.proof_flags53 & ~(VC_0053_BULLET_PROOF | VC_0053_FIRE_PROOF | VC_0053_COLLISION_PROOF | VC_0053_MELEE_PROOF);
        if(pVeh30->proofs & GVP_BULLET_PROOF)
        {
            proofFlags |= VC_0053_BULLET_PROOF;
        }
        if(pVeh30->proofs & GVP_FIRE_PROOF)
        {
            proofFlags |= VC_0053_FIRE_PROOF;
        }
        if(pVeh30->proofs & GVP_COLLISION_PROOF)
        {
            proofFlags |= VC_0053_COLLISION_PROOF;
        }
        if(pVeh30->proofs & GVP_MELEE_PROOF)
        {
            proofFlags |= VC_0053_MELEE_PROOF;
        }
        pCVehicle->entity.proof_flags53 = proofFlags;
        VLOGX(lss << UL::DEBUG << L(".f53: ") << ulhex(pCVehicle->entity.proof_flags53) << UL::ENDL);
        //
        VLOGX(lss << UL::DEBUG << L(".f1FDtires: ") << ulhex(pCVehicle->tiresFlags) << UL::ENDL);
        pCVehicle->tiresFlags &= ~VC_01FD_PROTECTED_TIRES;
        if(pVeh30->proofs & GVP_PROTECTED_TIRES)
        {
            pCVehicle->tiresFlags |= VC_01FD_PROTECTED_TIRES;
        }
        VLOGX(lss << UL::DEBUG << L(".f1FDtires: ") << ulhex(pCVehicle->tiresFlags) << UL::ENDL);

        if(pCVehicle->vehicleType == G3VT_AUTOMOBILE)
        {
            VLOGX(lss << UL::DEBUG << L(".f1FEbomb: ") << ulhex(pCVehicle->bombFlags) << UL::ENDL);
            pCVehicle->bombFlags = (pCVehicle->bombFlags & ~VC_01FE_BOMB_MASK) | (pVeh30->autoBombType & VC_01FE_BOMB_MASK);
            VLOGX(lss << UL::DEBUG << L(".f1FEbomb: ") << ulhex(pCVehicle->bombFlags) << UL::ENDL);
        }
    }
    else
    {
        vehicle = 0;
    }

    VLOG(lss << UL::DEBUG << L("VehVC_spawn: ") << ulhex(vehicle) << UL::ENDL);
    return vehicle;
}



DWORD VehicleIII::getModel(Vehicle vehicle)
{
    DWORD model = 0;
    G30CEntity* pCEntity = static_cast<G30CEntity*>(GtaGetCVehiclePtr(vehicle));
    if(pCEntity != nullptr)
    {
        model = pCEntity->model;
    }
    return model;
}

// VC: reuse III



void VehicleIII::doAlarmShort(Vehicle vehicle)
{
    // make some noise
    GCVehicleIII* pCVehicle = static_cast<GCVehicleIII*>(GtaGetCVehiclePtr(vehicle));
    pCVehicle->alarmDuration = 1005 / 2;
}

void VehicleVC::doAlarmShort(Vehicle vehicle)
{
    // make some noise
    GCVehicleVC* pCVehicle = static_cast<GCVehicleVC*>(GtaGetCVehiclePtr(vehicle));
    pCVehicle->alarmDuration = 1005 / 2;
}



void VehicleIII::stopEngine(Vehicle vehicle)
{
    GCVehicleIII* pCVehicle = static_cast<GCVehicleIII*>(GtaGetCVehiclePtr(vehicle));
    pCVehicle->equipmentFlags &= ~III_01F5_ENGINE_IS_ON;     // turn off engine
}

void VehicleVC::stopEngine(Vehicle vehicle)
{
    GCVehicleVC* pCVehicle = static_cast<GCVehicleVC*>(GtaGetCVehiclePtr(vehicle));
    pCVehicle->equipmentFlags &= ~VC_01F9_ENGINE_IS_ON;     // turn off engine
}



void VehicleIII::release(Vehicle* pVehicle)
{
    if(*pVehicle != 0)
    {
        VLOG(lss << UL::DEBUG << "VehG3_release: " << ulhex(*pVehicle) << UL::ENDL);
        SN::VEHICLE::MARK_CAR_AS_NO_LONGER_NEEDED(pVehicle);    // (G4?) will write NULL after vehicle release
        *pVehicle = 0;
    }
}

// VC: reuse III





VehicleIII vehIII;
VehicleVC vehVC;

DWORD VehicleGtaVersion = GTA_UNKNOWN;

IVehicle* GtaVehicleInit(DWORD GtaVersion)
{
    VLOGX(lss << UL::DEBUG << L("g3.f05Cmodel: ") << ulhex(OFFSETOF(GCVehicleIII, entity.model)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("g3.f14Ctime: ") << ulhex(OFFSETOF(GCVehicleIII, creationTime)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("g3.f19Ccolor1: ") << ulhex(OFFSETOF(GCVehicleIII, color1)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("g3.f1F5equip: ") << ulhex(OFFSETOF(GCVehicleIII, equipmentFlags)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("g3.f4D9bomb: ") << ulhex(OFFSETOF(GCAutomobileIII, equipmentFlags)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("vc.f05Cmodel: ") << ulhex(OFFSETOF(GCVehicleVC, entity.model)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("vc.f148time: ") << ulhex(OFFSETOF(GCVehicleVC, aiCommandTime)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("vc.f1A0color1: ") << ulhex(OFFSETOF(GCVehicleVC, color1)) << UL::ENDL);
    VLOGX(lss << UL::DEBUG << L("vc.f1F9equip: ") << ulhex(OFFSETOF(GCVehicleVC, equipmentFlags)) << UL::ENDL);

    VehicleGtaVersion = GtaVersion;

    IVehicle* pIVehicle = nullptr;
    switch(VehicleGtaVersion)
    {
    case GTA_III:
        pIVehicle = &vehIII;
        break;
    case GTA_VC:
        pIVehicle = &vehVC;
        break;
    }
    return pIVehicle;
}



struct FreeVeh30 {
    FLOAT modelCenterHeight;
    DWORD model;
    BYTE color1;
    BYTE color2;
    BYTE variation1;
    BYTE variation2;
    BYTE proofs;
    BYTE radiostation;
    WORD zpadding;
};

FreeVeh30 kuruma = {
    0.90625f,
    VM_III_KURUMA,  // Kuruma
    0x3A, 0x01,     // aqua / white
    0xFF, 0xFF,     // no variations
    0x00, 0x0B,     // no proofs, radio off
};
FreeVeh30 cheetah = {
    0.828125f,
    VM_VC_CHEETAH,  // Cheetah
    0x34, 0x34,     // dark blue / dark blue
    0x01, 0xFF,     // with side mirrors
    0x00, 0x0A,     // no proofs, radio off
};
FreeVeh30 stingerYellow = {
    0.796875f,
    VM_VC_STINGER,  // Stinger
    0x06, 0x06,     // yellow / yellow
    0xFF, 0xFF,     // no variations
    0x00, 0x0A,     // no proofs, radio off
};
FreeVeh30 stingerPurple = {
    0.796875f,
    VM_VC_STINGER,  // Stinger
    0x05, 0x05,     // purple / purple
    0xFF, 0xFF,     // no variations
    0x00, 0x0A,     // no proofs, radio off
};

//DWORD GenerateFreeVehicleSpec(VehSpec vehSpec, const FloatAngledVector3& pos)
DWORD GenerateFreeVehicleSpec(VehSpec vehSpec, const FloatVector3& position, const SByteVector3& rotation)
{
    FreeVeh30* pFreeVeh = nullptr;
    DWORD model = 0;
    switch(VehicleGtaVersion)
    {
    case GTA_III:
        pFreeVeh = &kuruma;
        break;
    case GTA_VC:
        switch(randomValueUpTo(3))
        {
        case 0:
            lss << UL::DEBUG << L("genVeh: vc 0") << UL::ENDL;
            pFreeVeh = &cheetah;
            break;
        case 1:
            lss << UL::DEBUG << L("genVeh: vc 1") << UL::ENDL;
            pFreeVeh = &stingerYellow;
            break;
        case 2:
            lss << UL::DEBUG << L("genVeh: vc 2") << UL::ENDL;
            pFreeVeh = &stingerPurple;
            break;
        }
        break;
    }

    if(pFreeVeh != nullptr)
    {
  //      FLOAT angleRad = static_cast<FLOAT>(pos.angle / 180.0f * M_PI);
  //      GtaMatrix4 mat;
  //      mat.m41 = pos.pos.X;
  //      mat.m42 = pos.pos.Y;
  //      mat.m43 = pos.pos.Z + pFreeVeh->modelCenterHeight;
  //      // rot x = { 0°:  0.0, 90°: -1.0, 180°:  0.0, 270°: +1.0 } = -sin()
  //      // rot y = { 0°: +1.0, 90°:  0.0, 180°: -1.0, 270°:  0.0 } =  cos()
  //      // rot z = 0.0
  //      mat.m21 = -sinf(angleRad);
  //      mat.m22 = cosf(angleRad);
  //      mat.m23 = 0.0f;
  //      mat.m21 = rotation.X;
  //      mat.m22 = rotation.Y;
  //      mat.m23 = rotation.Z;
//        lss << UL::DEBUG << L("genVeh: p:(") << pos.pos.X << L(", ") << pos.pos.Y << L(", ") << pos.pos.Z << L(")");
//        lss << L("  a:") << pos.angle << L(" aRad:") << angleRad;
//        lss << L("  r:(") << mat.m21 << L(", ") << mat.m22 << L(", ") << mat.m23 << L(")") << UL::ENDL;

 //       SByteVector3 packedRotation;
 //       GtaMakePackedRotation(packedRotation, pos.angle);
        GVeh30* pVeh30 = static_cast<GVeh30*>(vehSpec);
        pVeh30->model = pFreeVeh->model;
   //     VehicleStoreMatrix(*pVeh30, mat);
 //       pVeh30->position = pos.pos;
        pVeh30->position = position;
        pVeh30->position.Z += pFreeVeh->modelCenterHeight;
 //       pVeh30->rotation = packedRotation;
        pVeh30->rotation = rotation;
        pVeh30->proofs = pFreeVeh->proofs;
        pVeh30->creationTime = GtaGetGameTime();
        pVeh30->color1 = pFreeVeh->color1;
        pVeh30->color2 = pFreeVeh->color2;
        pVeh30->variation1 = pFreeVeh->variation1;
        pVeh30->variation2 = pFreeVeh->variation2;
        pVeh30->radiostation = pFreeVeh->radiostation;
        pVeh30->autoBombType = 0;
        pVeh30->unused0 = 0;
        model = pFreeVeh->model;
    }
    return model;
}

/*
CONST DWORD freeVehCountIII = 1;
VEHLIC31 freeVehiclesIII[freeVehCountIII] = {
    {
        PARKSPCHDR_SIGN,
        VM_III_KURUMA,  // Kuruma
        0xFF, 0xFF,     // no variations
        0x3A, 0x01,     // aqua / white
        0x0B, 0x00,     // radio off, no bomb
        0x00, 0x00,     // no proofs
    },
};
CONST DWORD freeVehCountVC = 3;
VEHLIC31 freeVehiclesVC[freeVehCountVC] = {
    {
        PARKSPCHDR_SIGN,
        VM_VC_CHEETAH,  // Cheetah
        0x01, 0xFF,     // with side mirrors
        0x34, 0x34,     // dark blue / dark blue
        0x0A, 0x00,     // radio off, no bomb
        0x00, 0x00,     // no proofs
    },
    {
        PARKSPCHDR_SIGN,
        VM_VC_STINGER,  // Stinger
        0xFF, 0xFF,     // no variations
        0x06, 0x06,     // yellow / yellow
        0x0A, 0x00,     // radio off, no bomb
        0x00, 0x00,     // no proofs
    },
    {
        PARKSPCHDR_SIGN,
        VM_VC_STINGER,  // Stinger
        0xFF, 0xFF,     // no variations
        0x05, 0x05,     // purple / purple
        0x0A, 0x00,     // radio off, no bomb
        0x00, 0x00,     // no proofs
    },
};

VEHLIC31* GetFreeVehicleLicense(void)
{
    VEHLIC31* pVehLicense = NULL;
    DWORD iVehLicense;
    if(VehicleGtaVersion == GTA_III)
    {
        //pVehLicense = &greenKuruma;
        iVehLicense = randomValueUpTo(freeVehCountIII);
        pVehLicense = &freeVehiclesIII[iVehLicense];
        ulogf(UL_DEBUG, T("getFreeVeh: %d"), iVehLicense);
    }
    else if(VehicleGtaVersion == GTA_VC)
    {
        //pVehLicense = &blueCheetah;
        iVehLicense = randomValueUpTo(freeVehCountVC);
        pVehLicense = &freeVehiclesVC[iVehLicense];
        ulogf(UL_DEBUG, T("getFreeVeh: %d"), iVehLicense);
    }
    return pVehLicense;
}
*/


