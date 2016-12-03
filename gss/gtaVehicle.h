///////////////////////////////////////////////////////////////
//
// gtaVehicle: vehicle stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   02.12.2014 02:44 - created
//   10.07.2016 18:34 - moved in Vehicle stuff from parkDef.h
//

#pragma once



struct GVeh30 {
    DWORD model;
    FloatVector3 position;
    SByteVector3 rotation;
    BYTE proofs;        // BP, FP, EP, CP, MP
    DWORD creationTime;
    BYTE color1;
    BYTE color2;
    BYTE variation1;
    BYTE variation2;
    BYTE radiostation;
    BYTE autoBombType;
    WORD unused0;
};
enum GVEH_PROOFS {
    GVP_BULLET_PROOF = 0x01,
    GVP_FIRE_PROOF = 0x02,
    GVP_EXPLOSION_PROOF = 0x04,
    GVP_COLLISION_PROOF = 0x08,
    GVP_MELEE_PROOF = 0x10,
    GVP_PROTECTED_TIRES = 0x20,
};



typedef void* VehSpec;

#include "idecl.h"
class IVehicle {
public:
    IENTRY DWORD ICALL getVehSpecSize(void) IPURE;
    IENTRY void ICALL clearSpec(VehSpec vehSpec) IPURE;
    IENTRY void ICALL describe(VehSpec vehSpec, Vehicle vehicle) IPURE;
    IENTRY Vehicle ICALL spawn(VehSpec vehSpec) IPURE;
    IENTRY DWORD ICALL getModel(Vehicle vehicle) IPURE;
    IENTRY void ICALL doAlarmShort(Vehicle vehicle) IPURE;   // if possible for this vehicle type
    IENTRY void ICALL release(Vehicle* pVehicle) IPURE;
};


IVehicle* GtaVehicleInit(DWORD GtaVersion);

DWORD GenerateFreeVehicleSpec(VehSpec vehSpec, const FloatAngledVector3& pos);


/*
VEHLIC31* GetFreeVehicleLicense(void);
*/

