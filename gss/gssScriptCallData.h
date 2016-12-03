///////////////////////////////////////////////////////////////
//
// gssScriptCallData: lightweight scripting system
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   28.08.2014 19:47 - created [moved from Main]
//   16.11.2016 05:05 - eliminated pre-processed macros
//   17.11.2016 23:37 - cleaned up & renamed to gssScriptCallData
//


/*      HOW IT WORKS:

            fake index macro-unwrapping:

SNCxFAKExVOID(DRAW_SPHERE,      0x03A1, 0x769F6E66, void,       SPOOF(index),   NAR(3v1a, ARPK( FVEC3(pos), FP(radius) )))

SNC_TYPE(void) DRAW_SPHERE(SNC_FIDX_ARG(FID(index)), SNC_AD_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))) {       SNC_FIDX_CALL(FID(index)); SNC_CALL(SNC_NAME(DRAW_SPHERE), SNC_CA_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))); }
SNC_TYPE(void) DRAW_SPHERE(     FIDX_DEF_FID(index), SNC_AD_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))) { SNFIDXCALL(FIDX_ARG_FID(index)); SNC_CALL(SNC_NAME(DRAW_SPHERE), SNC_CA_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))); }
SNC_TYPE(void) DRAW_SPHERE(             DWORD index, SNC_AD_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))) {             stc3OffsetIP(index); SNC_CALL(SNC_NAME(DRAW_SPHERE), SNC_CA_NAR(3v1a, ARPK( FVEC3(pos), FP(radius) ))); }


            function declaration macro-unwrapping:

SNCxVOID(DELETE_CAR,            0x00A6, 0x7F71342D,  void,      NAR(1a, ARPK( VEHICLE_CLEAR(pVehicle) )))

    SNC_TYPE(void) DELETE_CAR(SNC_AD_NAR(1a, ARPK( VEHICLE_CLEAR(pVehicle) ))) { SNC_CALL(SNC_NAME(DELETE_CAR), SNC_CA_NAR(1a, ARPK( VEHICLE_CLEAR(pVehicle) ))); }
    SNC_TYPE(void) DELETE_CAR( AD_N(ARGC_1a, ARPK( VEHICLE_CLEAR(pVehicle) ))) { SNC_CALL(SNC_NAME(DELETE_CAR),  CA_N(ARGC_1a, ARPK( VEHICLE_CLEAR(pVehicle) ))); }
    SNC_TYPE(void) DELETE_CAR(     AD_NUM(1, ARPK( VEHICLE_CLEAR(pVehicle) ))) { SNC_CALL(SNC_NAME(DELETE_CAR),      CA_NUM(1, ARPK( VEHICLE_CLEAR(pVehicle) ))); }
    SNC_TYPE(void) DELETE_CAR(          DEF1_ARPK( VEHICLE_CLEAR(pVehicle) ))  { SNC_CALL(SNC_NAME(DELETE_CAR),           ARG1_ARPK( VEHICLE_CLEAR(pVehicle) )); }
    SNC_TYPE(void) DELETE_CAR(              SNC_AD(VEHICLE_CLEAR(pVehicle)))   { SNC_CALL(SNC_NAME(DELETE_CAR),               SNC_CA(VEHICLE_CLEAR(pVehicle) )); }
    SNC_TYPE(void) DELETE_CAR(             SNC_DEF_VEHICLE_CLEAR(pVehicle))    { SNC_CALL(SNC_NAME(DELETE_CAR),              SNC_ARG_VEHICLE_CLEAR(pVehicle) ); }
    SNC_TYPE(void) DELETE_CAR(                          Vehicle* pVehicle)     { SNC_CALL(SNC_NAME(DELETE_CAR),                 SNC_ARG_HANDLE_PTR(pVehicle) ); }
    SNC_TYPE(void) DELETE_CAR(                          Vehicle* pVehicle)     { SNC_CALL(SNC_NAME(DELETE_CAR),                         (DWORD) &(*pVehicle) ); }
__forceinline void DELETE_CAR(                          Vehicle* pVehicle)     { stc3Call(    SNCMD_DELETE_CAR,                         (DWORD) &(*pVehicle) ); }


            script command arguments' types macro-unwrapping:

SNCxFAKExVOID(DRAW_CORONA,      0x024F, 0x39ED0C43, void,       SPOOF(index),   NAR(3v3a3v, ARPK( FVEC3(pos), FP(radius), IV(type), BYBOOL(drawLensflares), BYVEC3(rgb) )))

{0x024F, 0x39ED0C43, SNC_ART_NAR(3v3a3v,  ARPK(          FVEC3(pos),           FP(radius),         IV(type),         BYBOOL(drawLensflares),         BYVEC3(rgb) )) },
{0x024F, 0x39ED0C43,        AT_N( 5, 9,   ARPK(          FVEC3(pos),           FP(radius),         IV(type),         BYBOOL(drawLensflares),         BYVEC3(rgb) )) },
{0x024F, 0x39ED0C43,             9, { TY5_ARPK(          FVEC3(pos),           FP(radius),         IV(type),         BYBOOL(drawLensflares),         BYVEC3(rgb) ) }},
{0x024F, 0x39ED0C43,             9, {             SNC_TY(FVEC3(pos)),   SNC_TY(FP(radius)), SNC_TY(IV(type)), SNC_TY(BYBOOL(drawLensflares)), SNC_TY(BYVEC3(rgb)) }},
{0x024F, 0x39ED0C43,             9, {            SNC_TYP_FVEC3(pos),   SNC_TYP_FP(radius), SNC_TYP_IV(type), SNC_TYP_BYBOOL(drawLensflares), SNC_TYP_BYVEC3(rgb)  }},
{0x024F, 0x39ED0C43,             9, { SNAT_IN_FLOAT, SNAT_IN_FLOAT, SNAT_IN_FLOAT, SNAT_IN_FLOAT, SNAT_IN_INT, SNAT_IN_INT, SNAT_IN_INT, SNAT_IN_INT, SNAT_IN_INT }},
                                      '=================pos====================='  '==radius==='  '==type==='  '=flares=='  '===============rgb================='
*/


#define SNCMD_NAME(sname)         SNCMD_##sname

#define SNC_TYPE(rtype)         __forceinline rtype
#define SNC_CALL                SNCALL

// fake ip stuff
#define SNC_FIDX_ARG(fake)      FIDX_DEF_##fake
#define SNC_FIDX_CALL(fake)     SNFIDXCALL(FIDX_ARG_##fake)
#define FIDX_DEF_FID(fk)        DWORD fk
#define FIDX_ARG_FID(fk)        fk


#define SNC_AD_NAR(num, args)   AD_N(SNC_ARGC_##num, args)
#define SNC_CA_NAR(num, args)   CA_N(SNC_ARGC_##num, args)
#define SNC_TY_NAR(num, args)   TY_N(SNC_ARGC_##num, SNC_VALC_##num, args)

// SNC_ARGC_xx -> x       argument count (vector3 = 1)
#define SNC_ARGC_1a             1
#define SNC_ARGC_2a             2
#define SNC_ARGC_3a             3
#define SNC_ARGC_1a3v           2
#define SNC_ARGC_3v1a           2
#define SNC_ARGC_3v3v           2
#define SNC_ARGC_1a3v1a         3
#define SNC_ARGC_3v3a3v         5

// VC_xx -> x       value count (vector3 = 3)
#define SNC_VALC_1a             1
#define SNC_VALC_2a             2
#define SNC_VALC_3a             3
#define SNC_VALC_1a3v           4
#define SNC_VALC_3v3v           6
#define SNC_VALC_3v1a           4
#define SNC_VALC_1a3v1a         5
#define SNC_VALC_3v3a3v         9

#define AD_N(num, args)         AD_NUM(num, args)
#define CA_N(num, args)         CA_NUM(num, args)
#define TY_N(ac, vc, args)      TY_NUM(ac, vc, args)

#define AD_NUM(num, args)       AD##num##_##args
#define CA_NUM(num, args)       CA##num##_##args
#define TY_NUM(ac, vc, args)    vc, { TY##ac##_##args }

#define AD1_ARPK(a)                 SNC_AD(a)
#define AD2_ARPK(a, b)              SNC_AD(a), SNC_AD(b)
#define AD3_ARPK(a, b, c)           SNC_AD(a), SNC_AD(b), SNC_AD(c)
#define AD5_ARPK(a, b, c, d, e)     SNC_AD(a), SNC_AD(b), SNC_AD(c), SNC_AD(d), SNC_AD(e)
#define CA1_ARPK(a)                 SNC_CA(a)
#define CA2_ARPK(a, b)              SNC_CA(a), SNC_CA(b)
#define CA3_ARPK(a, b, c)           SNC_CA(a), SNC_CA(b), SNC_CA(c)
#define CA5_ARPK(a, b, c, d, e)     SNC_CA(a), SNC_CA(b), SNC_CA(c), SNC_CA(d), SNC_CA(e)
#define TY1_ARPK(a)                 SNC_TY(a)
#define TY2_ARPK(a, b)              SNC_TY(a), SNC_TY(b)
#define TY3_ARPK(a, b, c)           SNC_TY(a), SNC_TY(b), SNC_TY(c)
#define TY5_ARPK(a, b, c, d, e)     SNC_TY(a), SNC_TY(b), SNC_TY(c), SNC_TY(d), SNC_TY(e)

#define SNC_AD(am)              SNC_DEF_##am
#define SNC_CA(am)              SNC_ARG_##am
#define SNC_TY(am)              SNC_TYP_##am

#define SNC_DEF_BYBOOL(name)        BOOLBYTE name
#define SNC_DEF_IBY(name)           DWORD name
#define SNC_DEF_IBY_PTR(name)       DWORD* name
#define SNC_DEF_IV(name)            DWORD name
#define SNC_DEF_IV_PTR(name)        DWORD* name
#define SNC_DEF_FP(name)            FLOAT name
#define SNC_DEF_FP_PTR(name)        FLOAT* name
#define SNC_DEF_PED(name)           Ped name
#define SNC_DEF_PED_PTR(name)       Ped* name
#define SNC_DEF_VEHICLE(name)       Vehicle name
#define SNC_DEF_VEHICLE_PTR(name)   Vehicle* name
#define SNC_DEF_VEHICLE_CLEAR(name) Vehicle* name
#define SNC_DEF_BLIP(name)          Blip name
#define SNC_DEF_BLIP_PTR(name)      Blip* name
#define SNC_DEF_BLIP_CLEAR(name)    Blip* name
#define SNC_DEF_BYVEC3(name)        ByteVector3 name
#define SNC_DEF_FVEC3(name)         FloatVector3 name
#define SNC_DEF_FVEC3_PTR(name)     FloatVector3* name

#define SNC_ARG_BYBOOL(name)        SNARG_ASDWORD(name)
#define SNC_ARG_IBY(name)           SNARG_ASDWORD(name)
#define SNC_ARG_IBY_PTR(name)       SNARG_PTRCHECK(name)
#define SNC_ARG_IV(name)            SNARG_PLAIN(name)
#define SNC_ARG_IV_PTR(name)        SNARG_PTRCHECK(name)
#define SNC_ARG_FP(name)            SNARG_BITCOPY(name)
#define SNC_ARG_FP_PTR(name)        SNARG_PTRCHECK(name)
#define SNC_ARG_PED(name)           SNARG_HANDLE(name)
#define SNC_ARG_PED_PTR(name)       SNARG_HANDLE_PTR(name)
#define SNC_ARG_VEHICLE(name)       SNARG_HANDLE(name)
#define SNC_ARG_VEHICLE_PTR(name)   SNARG_HANDLE_PTR(name)
#define SNC_ARG_VEHICLE_CLEAR(name) SNARG_HANDLE_PTR(name)
#define SNC_ARG_BLIP(name)          SNARG_HANDLE(name)
#define SNC_ARG_BLIP_PTR(name)      SNARG_HANDLE_PTR(name)
#define SNC_ARG_BLIP_CLEAR(name)    SNARG_HANDLE_PTR(name)
#define SNC_ARG_BYVEC3(name)        SNARG_ASDWORD(name.by[0]), SNARG_ASDWORD(name.by[1]), SNARG_ASDWORD(name.by[2])
#define SNC_ARG_FVEC3(name)         SNARG_PLAIN(name.dw[0]), SNARG_PLAIN(name.dw[1]), SNARG_PLAIN(name.dw[2])
#define SNC_ARG_FVEC3_PTR(name)     SNARG_REFTO(name->dw[0]), SNARG_REFTO(name->dw[1]), SNARG_REFTO(name->dw[2])

#define SNC_TYP_BYBOOL(name)        SNTYP_IN_INT
#define SNC_TYP_IBY(name)           SNTYP_IN_INT
#define SNC_TYP_IBY_PTR(name)       SNTYP_OUT_INT
#define SNC_TYP_IV(name)            SNTYP_IN_INT
#define SNC_TYP_IV_PTR(name)        SNTYP_OUT_INT
#define SNC_TYP_FP(name)            SNTYP_IN_FLOAT
#define SNC_TYP_FP_PTR(name)        SNTYP_OUT_FLOAT
#define SNC_TYP_PED(name)           SNTYP_IN_HANDLE
#define SNC_TYP_PED_PTR(name)       SNTYP_OUT_HANDLE
#define SNC_TYP_VEHICLE(name)       SNTYP_IN_HANDLE
#define SNC_TYP_VEHICLE_PTR(name)   SNTYP_OUT_HANDLE
#define SNC_TYP_VEHICLE_CLEAR(name) SNTYP_INCLEAR_HANDLE
#define SNC_TYP_BLIP(name)          SNTYP_IN_HANDLE
#define SNC_TYP_BLIP_PTR(name)      SNTYP_OUT_HANDLE
#define SNC_TYP_BLIP_CLEAR(name)    SNTYP_INCLEAR_HANDLE
#define SNC_TYP_BYVEC3(name)        SNTYP_IN_INT, SNTYP_IN_INT, SNTYP_IN_INT
#define SNC_TYP_FVEC3(name)         SNTYP_IN_FLOAT, SNTYP_IN_FLOAT, SNTYP_IN_FLOAT
#define SNC_TYP_FVEC3_PTR(name)     SNTYP_OUT_FLOAT, SNTYP_OUT_FLOAT, SNTYP_OUT_FLOAT

#define SNARG_PLAIN(arg)                        arg     
#define SNARG_REFTO(arg)                       &arg     
#define SNARG_ASDWORD(arg)           (DWORD)    arg     
#define SNARG_BITCOPY(arg)         *((DWORD*)  &arg  )  
#define SNARG_PTRCHECK(arg)          (DWORD) &(*arg)        // pointer type should be checked via unref-ref sequence (compile-time check)
#define SNARG_HANDLE(arg)           SNARG_PLAIN(arg)        // don't use directly
#define SNARG_HANDLE_PTR(arg)       SNARG_PTRCHECK(arg)     // don't use directly

#define SNTYP_IN_INT                SAT_IN_INT32
#define SNTYP_IN_FLOAT              SAT_IN_FLT32
#define SNTYP_IN_HANDLE             SAT_IN_INT32
#define SNTYP_INCLEAR_HANDLE        SAT_INCLEAR_INT32
#define SNTYP_OUT_INT               SAT_OUT_INT32
#define SNTYP_OUT_FLOAT             SAT_OUT_FLT32
#define SNTYP_OUT_HANDLE            SAT_OUT_INT32



#ifdef SND_PROC_NAMESPACE
namespace SN {

namespace ASSET {
#endif //SND_PROC_NAMESPACE

SNCxVOID(REQUEST_MODEL,                     0x0247, 0x502B5185, void,       NAR(1a, ARPK( IV(model) )))
SNCxBOOL(HAS_MODEL_LOADED,                  0x0248, 0x4E61480A, BOOLBYTE,   NAR(1a, ARPK( IV(model) )))
SNCxVOID(MARK_MODEL_AS_NO_LONGER_NEEDED,    0x0249, 0x00FA0E33, void,       NAR(1a, ARPK( IV(model) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace ASSET

namespace WORLD {
#endif //SND_PROC_NAMESPACE

SNCxVOID(CLEAR_AREA_OF_CARS,         0x03BA, 0x24367E48, void,  NAR(3v3v, ARPK( FVEC3(cornerA), FVEC3(cornerB) )))
SNCxVOID(GET_GROUND_Z_FOR_3D_COORD,  0x02CE, 0x6D902EE3, void,  NAR(3v1a, ARPK( FVEC3(pos), FP_PTR(pGroundZ) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace WORLD

namespace MARKER {
#endif //SND_PROC_NAMESPACE

SNCxVOID(ADD_BLIP_FOR_CONTACT,  0x0189, 0x7C671162, void,       NAR(3v1a, ARPK( FVEC3(pos), BLIP_PTR(pBlip) )))
SNCxVOID(ADD_BLIP_FOR_COORD,    0x018A, 0x4CA708B9, void,       NAR(3v1a, ARPK( FVEC3(pos), BLIP_PTR(pBlip) )))
SNCxVOID(CHANGE_BLIP_DISPLAY,   0x018B, 0x3ACC1794, void,       NAR(2a, ARPK( BLIP(blip), IV(displayMode) )))
SNCxVOID(REMOVE_BLIP,           0x0164, 0x7BBF3625, void,       NAR(1a, ARPK( BLIP_CLEAR(pBlip) )))

SNCxFAKExVOID(DRAW_SPHERE,      0x03A1, 0x769F6E66, void,       FID(index),   NAR(3v1a, ARPK( FVEC3(pos), FP(radius) )))

// III: type = 0..7 (0=star 1=sun? 2=moon 3=reflection 4=headlights 5=octagon 6=circle 7=ring)
// IV: FVEC3(pos), FP(radius), IV(unk1), FP(unk2), BYVEC3(rgb)
SNCxFAKExVOID(DRAW_CORONA,      0x024F, 0x39ED0C43, void,       FID(index),   NAR(3v3a3v, ARPK( FVEC3(pos), FP(radius), IV(type), BYBOOL(drawLensflares), BYVEC3(rgb) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace MARKER

namespace PLAYER {
#endif //SND_PROC_NAMESPACE

SNCxVOID(GET_PLAYER_CHAR,       0x01F5, 0x511454A9, void,       NAR(2a, ARPK( IV(playerIndex), PED_PTR(pPed) )))
SNCxBOOL(IS_BUTTON_PRESSED,     0x00E1, 0x7FF21081, BOOLBYTE,   NAR(2a, ARPK( IV(padNumber), IV(button) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace PLAYER

namespace PED {
#endif //SND_PROC_NAMESPACE

SNCxVOID(GET_CHAR_COORDINATES,  0x00A0, 0x51457269, void,       NAR(1a3v, ARPK( PED(ped), FVEC3_PTR(pPos) )))
SNCxBOOL(IS_CHAR_IN_ANY_CAR,    0x00DF, 0x71184DA3, BOOLBYTE,   NAR(1a, ARPK( PED(ped) )))
SNCxBOOL(IS_CHAR_ON_FOOT,       0x044B, 0x10A86CF4, BOOLBYTE,   NAR(1a, ARPK( PED(ped) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace PED

namespace VEHICLE {
namespace EXT {
#endif //SND_PROC_NAMESPACE

SNCxVOID(CHANGE_CAR_COLOUR,     0x0229, 0x06441EAF, void,       NAR(3a, ARPK( VEHICLE(vehicle), IBY(colour1), IBY(colour2) )))   // color: 0..xx (gta4: 0..133) (tbogt: 0:136)
SNCxVOID(GET_CAR_COLOURS,       0x03F3, 0x6CAC3D62, void,       NAR(3a, ARPK( VEHICLE(vehicle), IBY_PTR(pColour1), IBY_PTR(pColour2) )))

#ifdef SND_PROC_NAMESPACE
} // namespace EXT

// namespace VEHICLE
#endif //SND_PROC_NAMESPACE

SNCxVOID(CREATE_CAR,            0x00A5, 0x2F1D6843, void,       NAR(1a3v1a, ARPK( IV(model), FVEC3(pos), VEHICLE_PTR(pVehicle) )))  // model should be enum VehicleModel
////SN1a3v2aVOID(CREATE_CAR,                0x2F1D6843,     void,       IV(nameHash), FVEC3(pos), VEHICLE_PTR(pVehicle), BYBOOL(unknownTrue))  // nameHash should be enum VehicleModel
SNCxVOID(DELETE_CAR,            0x00A6, 0x7F71342D,  void,      NAR(1a, ARPK( VEHICLE_CLEAR(pVehicle) )))
SNCxVOID(GET_CAR_COORDINATES,   0x00AA, 0x2D432EAB, void,       NAR(1a3v, ARPK( VEHICLE(vehicle), FVEC3_PTR(pPos) )))   // vehicle should be existing (or game will crash)
SNCxVOID(GET_CAR_HEADING,       0x0174, 0x46803CFA, void,       NAR(2a, ARPK( VEHICLE(vehicle), FP_PTR(pHeading) )))
SNCxVOID(GET_CAR_MODEL,         0x0441, 0x5FF84497, void,       NAR(2a, ARPK( VEHICLE(vehicle), IV_PTR(pModel) )))  // pModel should be enum VehicleModel
SNCxVOID(MARK_CAR_AS_NO_LONGER_NEEDED, 0x01C3, 0x20C76FD1, void,    NAR(1a, ARPK( VEHICLE_CLEAR(pVehicle) )))
SNCxVOID(SET_CAR_COORDINATES,   0x00AB, 0x567B6C56, void,       NAR(1a3v, ARPK( VEHICLE(vehicle), FVEC3(pos) )))
SNCxVOID(SET_CAR_HEADING,       0x0175, 0x75E40528, void,       NAR(2a, ARPK( VEHICLE(vehicle), FP(heading) )))
SNCxVOID(STORE_CAR_CHAR_IS_IN_NO_SAVE, 0x03C0, 0x21CC647F, void,    NAR(2a, ARPK( PED(ped), VEHICLE_PTR(pVehicle) )))

#ifdef SND_PROC_NAMESPACE
}; // namespace VEHICLE

}; // namespace SN
#endif //SND_PROC_NAMESPACE


