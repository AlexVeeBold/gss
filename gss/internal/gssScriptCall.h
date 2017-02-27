///////////////////////////////////////////////////////////////
//
// gssScriptCall: Script Command Execution
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   13.09.2014 03:56 - created (moved out from gssMain)
//   02.12.2014 01:50 - added GTA3CORONATYPE
//   02.12.2014 04:01 - added GTA3MARKERTYPE
//   17.11.2016 23:56 - cleaned up scriptCallData stuff
//

#pragma once

enum GTAPADBUTTON_III {
    PB_CAMERA_CHANGE_VIEW = 13,         // on-foot  targeting  in-vehicle   
    PB_FIRE_WEAPON = 17,                // on-foot  targeting  in-vehicle   
    PB_PED_TURN_LEFTRIGHT = 2,          // on-foot                          
    PB_PED_PREV_WEAPON = 5,             // on-foot                          
    PB_PED_NEXT_WEAPON = 7,             // on-foot                          
    PB_PED_GO_FORWARD = 8,              // on-foot                          
    PB_PED_GO_BACK = 9,                 // on-foot                          
    PB_PED_GO_LEFT = 10,                // on-foot                          
    PB_PED_GO_RIGHT = 11,               // on-foot                          
    PB_PED_JUMP = 14,                   // on-foot                          
    PB_PED_SPRINT = 16,                 // on-foot                          
    PB_PED_LOOK_BEHIND = 19,            // on-foot                          
    PB_PEDAIM_TARGET = 6,               // on-foot  targeting               
    PB_PEDAIM_LOOK_UP = 8,              //          targeting               
    PB_PEDAIM_LOOK_DOWN = 9,            //          targeting               
    PB_PEDAIM_LOOK_LEFT = 10,           //          targeting               
    PB_PEDAIM_LOOK_RIGHT = 11,          //          targeting               
    PB_PEDAIM_ZOOM_IN = 14,             //          targeting               
    PB_PEDAIM_ZOOM_OUT = 16,            //          targeting               
    PB_VEHICLE_TURRET_LEFTRIGHT = 2,    //                     in-vehicle   
    PB_VEHICLE_TURRET_UPDOWN = 3,       //                     in-vehicle   
    PB_VEHICLE_NEXT_RADIO_STATION = 4,  //                     in-vehicle   
    PB_VEHICLE_LOOK_LEFT = 5,           //                     in-vehicle   
    PB_VEHICLE_HANDBRAKE = 6,           //                     in-vehicle   
    PB_VEHICLE_LOOK_RIGHT = 7,          //                     in-vehicle   
    PB_VEHICLE_STEER_LEFT = 10,         //                     in-vehicle   
    PB_VEHICLE_STEER_RIGHT = 11,        //                     in-vehicle   
    PB_VEHICLE_BRAKE_REVERSE = 14,      //                     in-vehicle   
    PB_VEHICLE_ENTER_EXIT = 15,         // on-foot             in-vehicle   
    PB_VEHICLE_ACCELERATE = 16,         //                     in-vehicle   
    PB_VEHICLE_HORN = 18,               //                     in-vehicle   
    PB_VEHICLE_SUBMISSION = 19,         //                     in-vehicle   
};

enum GTA3CORONATYPE {
    G3CT_STAR = 0,
    G3CT_CORONA, // 1
    G3CT_MOON, // 2
    G3CT_REFLECTION, // 3
    G3CT_HEADLIGHT, // 4
    G3CT_HEX, // 5
    G3CT_CIRCLE, // 6
    G3CT_RING, // 7
};

enum GTA3MARKERTYPE {
    G3MT_unk0 = 0,
    G3MT_ARROW, // 1
    G3MT_unk2, // 2
    G3MT_unk3, // 3
    G3MT_CYLINDER, // 4
    G3MT_INVALID = 0x101,
};



#include <ulib/idecl.h>
class IScriptThread {
public:
    IENTRY void ICALL init(void) IPURE;
    IENTRY BOOL ICALLVA invoke(DWORD commandIndex, ...) IPURE;
    IENTRY void ICALL setOffsetIP(DWORD offset) IPURE;
};

extern IScriptThread* g_pIScript;

void GtaScriptInit(DWORD GtaVersion);
BOOL GtaScriptInvoke(DWORD dwCommandIndex, ...);
void GtaScriptSetOffsetIP(DWORD offset);

#define SNFIDXCALL              g_pIScript->setOffsetIP
#define SNCALL                  g_pIScript->invoke


// // // // COMMAND NAMES // // // //

#define SNCxFAKExVOID(sname, code, hash, rtype, spoof, nargs)  SNC_ENUM_ENTRY(sname)
#define SNCxVOID(sname, code, hash, rtype, nargs) SNC_ENUM_ENTRY(sname)
#define SNCxBOOL(sname, code, hash, rtype, nargs) SNC_ENUM_ENTRY(sname)

enum SNCOMMANDINDEX {

#define SNC_ENUM_ENTRY(sname)           SNCMD_NAME(sname),
#include "gssScriptCallData.h"
#undef SNC_ENUM_ENTRY

    NUM_SN_COMMANDS,
};

#undef SNCxFAKExVOID
#undef SNCxVOID
#undef SNCxBOOL


// // // // COMMAND PROCEDURES // // // //

#define SNCxFAKExVOID(sname, code, hash, rtype, fi, nargs)  SNC_TYPE(void)  sname(SNC_FIDX_ARG(fi), SNC_AD_##nargs) { SNC_FIDX_CALL(fi);        SNC_CALL(SNCMD_NAME(sname), SNC_CA_##nargs); }
#define      SNCxVOID(sname, code, hash, rtype,     nargs)  SNC_TYPE(void)  sname(                  SNC_AD_##nargs) {                           SNC_CALL(SNCMD_NAME(sname), SNC_CA_##nargs); }
#define      SNCxBOOL(sname, code, hash, rtype,     nargs)  SNC_TYPE(rtype) sname(                  SNC_AD_##nargs) {                    return SNC_CALL(SNCMD_NAME(sname), SNC_CA_##nargs); }

#define SND_PROC_NAMESPACE
#include "gssScriptCallData.h"
#undef SND_PROC_NAMESPACE

#undef SNCxFAKExVOID
#undef SNCxVOID
#undef SNCxBOOL

