///////////////////////////////////////////////////////////////
//
// gtaVehInfo: vehicle database
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   23.09.2014 10:26 - created (moved out from gssMain)
//   04.10.2014 07:05 - added GetVehicleTypes()
//   26.11.2014 21:43 - added new vehicle flags/enums while adding VC vehicles
//   26.11.2014 23:40 - incorporated new veh.info macro system (easy readable values + no data duplication)
//   28.11.2014 01:55 - packed veh.info record into nibbles
//   19.12.2014 08:30 - updated VehInfoGetTypes()
//   11.01.2014 09:08 - fixed VehInfoGetTypes() motorcycles bug
//   10.07.2016 23:31 - cleaned up, moved veh.info comments to text file
//   26.11.2016 08:50 - encapsulated into a class
//


#include "uDef.h"
#include "tunic.h"

#include "uLog.h"

#include "gtaVersion.h"
#include "gtaVehInfo.h"



enum VEHMEDIA {     // bitfield (8 bits)
    // usual media
    VMD_AIR = 0x1,
    VMD_LAND = 0x2,
    VMD_WATER = 0x4,
    // extra media
    VMD_SPACE = 0x10,
    VMD_UNDERGROUND = 0x20,
    VMD_UNDERWATER = 0x40,
    // multi-media
    VMD_LAND_AMPHIBIOUS = VMD_LAND | VMD_WATER,     // LAND with water ability
    VMD_AIR_AMPHIBIOUS = VMD_AIR | VMD_WATER,       // AIR with water ability
    VMD_SUBMERSIBLE = VMD_WATER | VMD_UNDERWATER,   // WATER with underwater ability
};
enum VEHTYPE {      // enum (4 bits)
    VT_OTHER = 0x1,     // 
    // VMD_AIR
    VTA_VTOL = 0x2,     // vertical take-off and landing aircraft (vtol aircraft, heli)
    VTA_CTOL = 0x3,     // conventional take-off and landing [using runways/water] aircraft (plane, seaplane)
    // VMD_LAND
    VTL_CYCLE = 0x4,    // land wheeled vehicle, steered by handlebar (bicycle, quad bike, motorcycle)
    VTL_WHEELED = 0x5,  // land wheeled vehicle, steered by steering wheel (automobile, go-kart, van, bus, truck)
    VTL_RAIL = 0x6,     // rail-road vehicle (train, tram, subway, etc.)
    VTL_OTHER = 0x7,    // (tank, hovercraft, etc.)
    // VMD_WATER
    VTW_BOAT = 0x8,         // boat (single-hulled vessel)
    VTW_CATAMARAN = 0x9,    // multihulled vessel (vice city rio is catamaran sailboat)
};
enum VEHSUBTYPE {   // enum (4 bits)
    // VMD_AIR + VTA_VTOL
    VAV_HELI = 0x1,             // vertical take-off and landing rotorcraft (heli)
    VAV_VTOL = 0x2,             // Vertical Take-Off and Landing fixed-wing aircraft (plane)
    // VMD_AIR + VTA_CTOL
    VAC_PLANE = 0x1,            // conventional take-off and landing aircraft (plane)
    // VMD_LAND + VTL_WHEELED
    VLW_GENERIC = 0x1,          // sedan/hatchback/etc. (hardtop)(fixed car roof)(cabrio w. roof up and w/o in-game roof control)
    VLW_SOFTTOP = 0x2,          // cabrio/roofless/convertible/etc.
    VLW_MPV = 0x3,              // Multi-Purpose Vehicle (including minivan & station wagon)
    VLW_VAN = 0x4,              // cargo van, passenger van
    VLW_STEPVAN = 0x5,          // delivery van, a.k.a. multi-stop truck (hot dog van, ice-cream van, etc.)
    VLW_BUS = 0x6,              // bus (school, interstate, etc.)
    VLW_SUV = 0x7,              // Sport Utility Vehicle, also known as off-road 4x4
    VLW_PICKUP = 0x8,           // pickup truck
    VLW_BOXTRUCK = 0x9,         // box truck
    VLW_RIGIDTRUCK = 0xA,       // rigid truck (tow truck, utility truck, car transporter truck, tanker truck, etc.)
    VLW_SEMITRAILER = 0xB,      // semi-trailer truck
    VLW_TRAILER = 0xC,          // trailer
    VLW_OTHERVEHICLE = 0xD,     // (tractor, combine harvester, etc.)
    // VMD_LAND + VTL_CYCLE
    VLC_MONOCYCLE = 0x1,        // 
    VLC_BICYCLE = 0x2,          // 2 wheels
    VLC_TRICYCLE = 0x3,         // 
    VLC_QUADRICYCLE = 0x4,      // 4 wheels
    // VMD_LAND + VTL_RAIL
    VLR_LOCOMOTIVE = 0x1,
    VLR_CAR = 0x2,
    // VMD_LAND + VTL_OTHER
    VLO_COMBAT = 0x1,           // Armored Fighting Vehicle (tank)
    VLO_HOVERCRAFT = 0x2,
    // VMD_WATER + VTW_BOAT  &
    // VMD_WATER + VTW_CATAMARAN
    VWB_SPEED = 0x1,        // speed boat / go-fast boat / patrol boat
    VWB_MOTOR = 0x2,        // motor boat / motor yacht (tropic)
    VWB_SAIL = 0x3,         // sailboat (marquis, vc's rio)
    VWB_FISHING = 0x4,      // fishing boat (reefer)
    VWB_INFLATABLE = 0x5,   // inflatable boat (dinghy)
};
//
// size:           [all bikes/bicycles are medium-sized][size other vehicles is set individually]
// smallest     microcar                smallest plane          smallest heli           smallest boat
//              (issi, golf car)        (dodo, stuntplane)      (sparrow, news)         (jetski, seashark)
// small        minicar                                                                 small boat
//              (blista compact, club)                                                  (dinghy, floater)
// medium       regular car             medium plane            medium heli             medium boat
//              (bullet, savanna)       (shamal, nevada)        (maverick)              (speeder, predator)
// large        large car                                                               large boat
//              (barracks, journey)                                                     (marquis, tropic)
// largest      largest car             largest plane           largest heli            largest boat
//              (coach, dumper)         (airtrain, at-400)      (cargobob, leviathan)   (tugboat, colonel's yacht)
enum VEHSIZE {      // enum (4 bits)
    VSZ_SMALLEST = 0x1,     // (smaller)       ... issi
    VSZ_SMALL = 0x2,        // blista compact  ... mesa(VC/SA)
    VSZ_MEDIUM = 0x3,       // kuruma          ... burrito/pony/rumpo
    VSZ_LARGE = 0x4,        // boxville/benson ... barracks
    VSZ_LARGEST = 0x5,      // bus(III)/coach  ... (larger)
};
// (VCL_BASIC = poorfamily,worker,normal,ignore) < (VCL_ADVANCED = richfamily) < (VCL_SUPERIOR = executive)
enum VEHCLASS {     // enum (4 bits)
    VCL_BASIC = 0x1,        // [cheap] personal/family
    VCL_ADVANCED = 0x2,     // [expensive] premium/tuner/muscle/lowrider
    VCL_SUPERIOR = 0x3,     // supercar, grand tourier
    VCL_LIMOUSINE = 0x4,    // limousine
    VCL_RACING = 0x5,       // rally etc. [NOT street-racing]
    VCL_MOTORHOME = 0x6,    // (journey)
    VCL_CARGO = 0x7,        // commercial
    VCL_PUBLIC = 0x8,       // public transport
    VCL_UTILITY = 0x9,      // street cleaner, airport service (NOT tourist bus), tow truck, etc.
    VCL_EMERGENCY = 0xA,    // fireguard, ambulance
    VCL_ENFORCEMENT = 0xB,  // police, fbi, swat, military
    VCL_CRIMINAL = 0xC,     // gang, mafia, cartel
    VCL_SERVICE = 0xD,      // tourist bus, funeral vehicle
};
//
enum VEHPOWER {         // enum (4 bits)
    // power
    VPW_UNPOWERED = 0x0,    // (trailer)
    VPW_HUMANPOWERED = 0x1, // (bicycle)
    VPW_HYDROCARBON = 0x2,  // diesel, gasoline, kerosene, natural gas, hydrogen
    VPW_ELECTRIC = 0x3,     // battery or solar-powered
    VPW_HYBRID = 0x4,       // hydrocarbon/electric
};
enum VEHCONTROL {       // enum (4 bits)
    // control
    VCT_MANNED = 0x1,       // (automobile, bicycle, etc.)
    VCT_UNMANNED = 0x2,     // radio-controlled vehicle, autonomous vehicle, trailer
};
//
enum VEHADDITIONAL {    // bitfield (32 bits)
    VAD_NONE = 0x0,
    VAD_ARMED = 0x1,                // have WORKING guns (hydra, hunter, rustler, etc.)
    VAD_ARMORED = 0x2,              // have VISIBLE armor (bank van) or heavy armor (swat tank, military tank) [armor may be "visible-only", without anti-damage effect]
    VAD_WATER_CANNON = 0x4,         // have water cannon (firetruck, swat tank)
    VAD_ADJUSTABLE_PARTS = 0x8,     // dozer (scoop), forklift (lift), packer (upper ramp), hydra (jet outlets), etc.
    VAD_TRAILER_ATTACHMENT = 0x10,  // semi-trailer truck (and it's trailer attachment IS working)
    VAD_HOOK_AND_CHAIN = 0x20,      // can tow vehicle (automobile, heli, plane, boat, etc.)
    VAD_SIREN_WITH_LIGHTS = 0x40,   // have WORKING siren with lights
    VAD_TAXI_LIGHTS = 0x80,         // have working taxi light(s)
    VAD_REMOTE_CONTROLLED = 0x100,  // remote controlled vehicle (r.c. car, r.c. heli, r.c. plane, r.c. tank, etc.)
    VAD_UNCONTROLLABLE = 0x200,     // player can't drive/control this vehicle (III's airtrain, SA's tram, IV's subway, etc.)
};



// indices to veh.info array
enum VIDNAMES {
#define VEHID(name,media,type,subtype,vsize,vclass,power,control,additional)    \
    name,
#include "gtaVehInfoData.h"
#undef VEHID
};


// fields swapped for high-nibble-first order
struct VehInfoData {
    BYTE vehMedia;      // land / water / etc
    BYTE vehSubType:4;  // bicycle / sailboat / etc
    BYTE vehType:4;     // wheeled / boat / etc
    BYTE vehClass:4;
    BYTE vehSize:4;
    BYTE vehControl:4;
    BYTE vehPower:4;
    DWORD vehAdditional;
};
// veh.info array
VehInfoData gtaVehInfoData[] = {
#define VEHID(name,media,type,subtype,vsize,vclass,power,control,additional)    \
    { media, subtype, type, vclass, vsize, control, power, additional },
#include "gtaVehInfoData.h"
#undef VEHID
    {0,} // removing intellisense warnings
};


struct VehInfoLink8 {
    BYTE id;
    BYTE infoIndex;
};
struct VehInfoLink16 {
    WORD id;
    WORD infoIndex;
};
struct VehInfoLink32 {
    DWORD id;
    DWORD infoIndex;
};

// using typedef VehInfoLink <- GtaVehInfoLink

// links to veh.info array
GtaVehInfoLink gtaVehInfoLinkIII[] = {
    // (only land vehicles have horns, alarms & sirens)
    {VM_III_LANDSTALKER,        MHC_MEDIUM_ADVANCED_SUV}, //
    {VM_III_IDAHO,              MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_III_STINGER,            MHC_MEDIUM_SUPERIOR_CABRIO}, //
    {VM_III_LINERUNNER,         MHC_LARGE_CARGO_SEMITRAILER}, //
    {VM_III_PERENNIAL,          MHC_MEDIUM_BASIC_MPV}, //
    {VM_III_SENTINEL,           MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_III_PATRIOT,            MHC_MEDIUM_ENFORCEMENT_SUV}, //
    {VM_III_FIRETRUCK,          GTA_FIRETRUCK}, //
    {VM_III_TRASHMASTER,        MHC_LARGE_UTILITY_RIGIDTRUCK}, //
    {VM_III_STRETCH,            MHC_LARGE_LIMOUSINE}, //
    {VM_III_MANANA,             MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_III_INFERNUS,           MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_III_BLISTA_MINIVAN,     MHC_MEDIUM_ADVANCED_MPV}, //
    {VM_III_PONY,               MHC_MEDIUM_CARGO_VAN}, //
    {VM_III_MULE,               MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_III_CHEETAH,            MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_III_AMBULANCE,          GTA_AMBULANCE}, //
    {VM_III_KURUMA_FBI,         MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN}, //
    {VM_III_MOONBEAM,           MHC_MEDIUM_BASIC_MPV}, //
    {VM_III_ESPERANTO,          MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_III_TAXI,               MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_III_KURUMA,             MHC_MEDIUM_ADVANCED_HARDTOP}, //
    {VM_III_BOBCAT,             MHC_MEDIUM_CARGO_PICKUP}, //
    {VM_III_MRWHOOPEE,          MHC_LARGE_CARGO_STEPVAN}, //
    {VM_III_BF_INJECTION,       MHC_MEDIUM_ADVANCED_SUV}, //
    {VM_III_MANANA_CORPSE,      MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_III_POLICE,             MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN}, //
    {VM_III_ENFORCER,           MHC_LARGE_ENFORCEMENT_BOXTRUCK_WITH_SIREN}, //
    {VM_III_SECURICAR,          MHC_LARGE_CARGO_VAN_ARMORED}, //
    {VM_III_BANSHEE,            MHC_MEDIUM_SUPERIOR_CABRIO}, //
    {VM_III_PREDATOR,           MHC_MEDIUM_ENFORCEMENT_SPEED_BOAT_WITH_GUN}, //
    {VM_III_BUS,                MHC_LARGEST_BUS}, //
    {VM_III_RHINO,              GTA_TANK}, //
    {VM_III_BARRACKS,           MHC_LARGE_ENFORCEMENT_RIGIDTRUCK}, //
    {VM_III_TRAIN,              UEL_TRAIN_CAR}, //
    {VM_III_FAKE_HELI_CHOPPER,  FAKE_POLICE_HELI}, //
    {VM_III_DODO,               MHC_SMALLEST_BASIC_PLANE}, //
    {VM_III_COACH,              MHC_LARGEST_BUS}, //
    {VM_III_CABBIE,             MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_III_STALLION,           MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_III_RUMPO,              MHC_MEDIUM_CARGO_VAN}, //
    {VM_III_RCBANDIT,           URCEL_RACE_CAR}, //
    {VM_III_MULE_TRIAD,         MHC_LARGE_CRIMINAL_BOXTRUCK}, //
    {VM_III_MULE_MRWONGS,       MHC_LARGE_CRIMINAL_BOXTRUCK}, //
    {VM_III_SENTINEL_MAFIA,     MHC_MEDIUM_CRIMINAL_HARDTOP}, //
    {VM_III_LOBO_YARDIE,        MHC_MEDIUM_CRIMINAL_HARDTOP}, //
    {VM_III_STINGER_YAKUZA,     MHC_MEDIUM_CRIMINAL_HARDTOP}, //
    {VM_III_STALLION_DIABLO,    MHC_MEDIUM_CRIMINAL_HARDTOP}, //
    {VM_III_CRUISER_CARTEL,     MHC_MEDIUM_CRIMINAL_PICKUP}, //
    {VM_III_RUMPO_HOODS,        MHC_MEDIUM_CRIMINAL_VAN}, //
    {VM_III_FAKE_AIRTRAIN,      FAKE_AIRTRAIN_PLANE}, //
    {VM_III_FAKE_DEADDODO,      FAKE_DODO_PLANE}, //
    {VM_III_SPEEDER,            MHC_LARGE_ADVANCED_SPEED_BOAT}, //
    {VM_III_REEFER,             MHC_MEDIUM_BASIC_FISHING_BOAT}, //
    {VM_III_PONY_PANLANTIC,     MHC_MEDIUM_CARGO_VAN}, //
    {VM_III_FLATBED,            MHC_LARGE_CARGO_RIGIDTRUCK}, //
    {VM_III_YANKEE,             MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_III_FAKE_HELI_ESCAPE,   FAKE_ESCAPE_HELI}, //should be catalina heli
    {VM_III_CABBIE_BORGNINE,    MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_III_PONY_TOYZ,          MHC_MEDIUM_CARGO_VAN}, //
    {VM_III_GHOST,              MHC_MEDIUM_ADVANCED_SPEED_BOAT}, //
};
GtaVehInfoLink gtaVehInfoLinkVC[] = {
    // (boats have no horn & no siren)(?)
    {VM_VC_LANDSTALKER,         MHC_MEDIUM_ADVANCED_SUV}, //
    {VM_VC_IDAHO,               MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_STINGER,             MHC_MEDIUM_SUPERIOR_CABRIO}, //
    {VM_VC_LINERUNNER,          MHC_LARGE_CARGO_SEMITRAILER}, //
    {VM_VC_PERENNIAL,           MHC_MEDIUM_BASIC_MPV}, //
    {VM_VC_SENTINEL,            MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_VC_RIO,                 MHC_LARGE_SUPERIOR_SAIL_CATAMARAN}, //
    {VM_VC_FIRETRUCK,           GTA_FIRETRUCK}, //catamaran, has wind detector (propeller, always looking NE?)
    {VM_VC_TRASHMASTER,         MHC_LARGE_UTILITY_RIGIDTRUCK}, //
    {VM_VC_STRETCH,             MHC_LARGE_LIMOUSINE}, //
    {VM_VC_MANANA,              MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_INFERNUS,            MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_VC_VOODOO,              MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_PONY,                MHC_MEDIUM_CARGO_VAN}, //
    {VM_VC_MULE,                MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_VC_CHEETAH,             MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_VC_AMBULANCE,           GTA_AMBULANCE}, //
    {VM_VC_WASHINGTON_FBI,      MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN}, // lights are NOT working
    {VM_VC_MOONBEAM,            MHC_MEDIUM_BASIC_MPV}, //
    {VM_VC_ESPERANTO,           MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_TAXI,                MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_VC_WASHINGTON,          MHC_MEDIUM_ADVANCED_HARDTOP}, //
    {VM_VC_BOBCAT,              MHC_MEDIUM_CARGO_PICKUP}, //
    {VM_VC_MRWHOOPEE,           MHC_LARGE_CARGO_STEPVAN}, //
    {VM_VC_BF_INJECTION,        MHC_MEDIUM_ADVANCED_SUV}, //
    {VM_VC_HUNTER,              MHC_MEDIUM_ENFORCEMENT_HELI_WITH_GUN}, //
    {VM_VC_POLICE,              MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN}, //
    {VM_VC_ENFORCER,            MHC_LARGE_ENFORCEMENT_BOXTRUCK_WITH_SIREN}, //
    {VM_VC_SECURICAR,           MHC_LARGE_CARGO_VAN_ARMORED}, //
    {VM_VC_BANSHEE,             MHC_MEDIUM_SUPERIOR_CABRIO}, //
    {VM_VC_PREDATOR,            MHC_MEDIUM_ENFORCEMENT_SPEED_BOAT_WITH_GUN}, //
    {VM_VC_BUS,                 MHC_LARGEST_BUS}, //
    {VM_VC_RHINO,               GTA_TANK}, //
    {VM_VC_BARRACKS,            MHC_LARGE_ENFORCEMENT_RIGIDTRUCK}, //
    {VM_VC_HERMES_CUBAN,        MHC_MEDIUM_CRIMINAL_HARDTOP}, //
    {VM_VC_FAKE_HELI_CHOPPER,   FAKE_POLICE_HELI}, // fake heli, flying around, no collision
    {VM_VC_ANGEL,               MHC_CRIMINAL_MOTORCYCLE}, //
    {VM_VC_COACH,               MHC_LARGEST_BUS}, //
    {VM_VC_CABBIE,              MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_VC_STALLION,            MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_RUMPO,               MHC_MEDIUM_CARGO_VAN}, //
    {VM_VC_RCBANDIT,            URCEL_RACE_CAR}, //
    {VM_VC_ROMERO,              MHC_MEDIUM_SERVICE_HARDTOP}, //
    {VM_VC_PACKER,              MHC_LARGEST_CARGO_RIGIDTRUCK}, //
    {VM_VC_SENTINEL_XS,         MHC_MEDIUM_SUPERIOR_HARDTOP}, //
    {VM_VC_ADMIRAL,             MHC_MEDIUM_ADVANCED_HARDTOP}, //
    {VM_VC_SQUALO,              MHC_LARGE_ADVANCED_SPEED_BOAT}, //
    {VM_VC_SEASPARROW,          MHC_SMALLEST_BASIC_AMPHIBIOUS_HELI_WITH_GUN}, //
    {VM_VC_FAGGIO_PIZZABOY,     MHC_CARGO_MOTORCYCLE}, //
    {VM_VC_BURRITO_GANG,        MHC_MEDIUM_CRIMINAL_VAN}, //
    {VM_VC_FAKE_AIRTRAIN,       FAKE_AIRTRAIN_PLANE}, // fake plane, flying around, no collision
    {VM_VC_FAKE_DEADDODO,       FAKE_DODO_PLANE}, // fake plane, flying around, no collision
    {VM_VC_SPEEDER,             MHC_LARGE_ADVANCED_SPEED_BOAT}, //
    {VM_VC_REEFER,              MHC_MEDIUM_BASIC_FISHING_BOAT}, //
    {VM_VC_TROPIC,              MHC_LARGE_SUPERIOR_MOTOR_BOAT}, //
    {VM_VC_FLATBED,             MHC_LARGE_CARGO_RIGIDTRUCK}, //
    {VM_VC_YANKEE,              MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_VC_CADDY,               MEL_SMALLEST_SERVICE_HARDTOP}, //
    {VM_VC_CABBIE_ZEBRA,        MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_VC_PONY_TOPFUN,         MHC_MEDIUM_CARGO_VAN}, //
    {VM_VC_SKIMMER,             MHC_SMALLEST_BASIC_AMPHIBIOUS_PLANE}, //
    {VM_VC_PCJ600,              MHC_ADVANCED_MOTORCYCLE}, //
    {VM_VC_FAGGIO,              MHC_BASIC_MOTORCYCLE}, //
    {VM_VC_FREEWAY,             MHC_BASIC_MOTORCYCLE}, //
    {VM_VC_RCBARON,             URCEL_PLANE}, //
    {VM_VC_RCRAIDER,            URCEL_HELI}, //
    {VM_VC_GLENDALE,            MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_OCEANIC,             MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_SANCHEZ,             MHC_BASIC_MOTORCYCLE}, //
    {VM_VC_SPARROW,             MHC_SMALLEST_BASIC_HELI}, //
    {VM_VC_PATRIOT,             MHC_MEDIUM_ENFORCEMENT_SUV}, //
    {VM_VC_LOVEFIST,            MHC_MEDIUM_LIMOUSINE}, //
    {VM_VC_COASTGUARD,          MHC_MEDIUM_EMERGENCY_SPEED_BOAT}, //
    {VM_VC_DINGHY,              MHC_SMALL_BASIC_INFLATABLE_BOAT}, //
    {VM_VC_HERMES,              MHC_MEDIUM_ADVANCED_HARDTOP}, //
    {VM_VC_SABRE,               MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_SABRE_TURBO,         MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_PHOENIX,             MHC_MEDIUM_ADVANCED_HARDTOP}, //
    {VM_VC_WALTON,              MHC_MEDIUM_CARGO_PICKUP}, //
    {VM_VC_REGINA,              MHC_MEDIUM_BASIC_MPV}, //
    {VM_VC_COMET,               MHC_MEDIUM_SUPERIOR_CABRIO}, //
    {VM_VC_DELUXO,              MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_BURRITO,             MHC_MEDIUM_CARGO_VAN}, //
    {VM_VC_MULE_SPANDEX,        MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_VC_MARQUIS,             MHC_LARGE_SUPERIOR_SAIL_BOAT}, //largest of "large" boats
    {VM_VC_BAGGAGE,             MEL_SMALLEST_UTILITY_CABRIO}, //
    {VM_VC_CABBIE_KAUFMAN,      MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS}, //
    {VM_VC_MAVERICK,            MHC_MEDIUM_BASIC_HELI}, //
    {VM_VC_MAVERICK_VCN,        MHC_SMALL_BASIC_HELI}, //
    {VM_VC_RANCHER,             MHC_MEDIUM_ADVANCED_SUV}, //
    {VM_VC_RANCHER_FBI,         MHC_MEDIUM_ENFORCEMENT_SUV}, //
    {VM_VC_VIRGO,               MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_GREENWOOD,           MHC_MEDIUM_BASIC_HARDTOP}, //
    {VM_VC_JETMAX,              MHC_LARGE_ADVANCED_SPEED_BOAT}, //
    {VM_VC_HOTRING,             MHC_MEDIUM_RACING_HARDTOP}, //
    {VM_VC_SANDKING,            MHC_MEDIUM_RACING_SUV}, //
    {VM_VC_BLISTA_COMPACT,      MHC_SMALL_BASIC_HARDTOP}, //
    {VM_VC_MAVERICK_VCPD,       MHC_MEDIUM_ENFORCEMENT_HELI}, //
    {VM_VC_BOXVILLE,            MHC_LARGE_CARGO_STEPVAN}, //
    {VM_VC_BENSON,              MHC_LARGE_CARGO_BOXTRUCK}, //
    {VM_VC_MESA,                MHC_SMALL_BASIC_SUV}, //
    {VM_VC_RCGOBLIN,            URCEL_HELI}, //
    {VM_VC_HOTRING_A,           MHC_MEDIUM_RACING_HARDTOP}, //
    {VM_VC_HOTRING_B,           MHC_MEDIUM_RACING_HARDTOP}, //
    {VM_VC_BLOODRING_A,         MHC_MEDIUM_RACING_HARDTOP}, //
    {VM_VC_BLOODRING_B,         MHC_MEDIUM_RACING_HARDTOP}, //
    {VM_VC_CHEETAH_VCPD,        MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN}, //
};


enum VEHMODELINDEXBOUNDARIES {
    VM_III_min = VM_III_LANDSTALKER,
    VM_III_max = VM_III_GHOST,
    VM_VC_min = VM_VC_LANDSTALKER,
    VM_VC_max = VM_VC_CHEETAH_VCPD,
};



void GtaVehInfo::init(DWORD gtaVersion)
{
    if(gtaVersion == GTA_III)
    {
        m_gtaVersion = gtaVersion;
        m_indexMin = VM_III_min;
        m_indexMax = VM_III_max;
        m_pInfoLink = gtaVehInfoLinkIII;
        m_pInfoData = gtaVehInfoData;
    }
    else if(gtaVersion == GTA_VC)
    {
        m_gtaVersion = gtaVersion;
        m_indexMin = VM_VC_min;
        m_indexMax = VM_VC_max;
        m_pInfoLink = gtaVehInfoLinkVC;
        m_pInfoData = gtaVehInfoData;
    }
}

BYTE GtaVehInfo::getVehicleTypes(const VehInfoData& VIP)
{
    BYTE vehTypes = 0;

    // set air flags
    if((VIP.vehMedia & VMD_AIR) != 0)
    {
        if((VIP.vehMedia & VMD_WATER) != 0)
        {
            vehTypes |= VT_AMPHIBIOUS;
        }

        switch(VIP.vehType)
        {
        case VTA_VTOL:
            vehTypes |= VT_AIR_VTOL;
            break;

        case VTA_CTOL:
            vehTypes |= VT_AIR_CTOL;
            break;
        }
    }

    // set land flags
    if((VIP.vehMedia & VMD_LAND) != 0)
    {
        switch(VIP.vehType)
        {
        case VTL_CYCLE:
            if(VIP.vehPower == VPW_HUMANPOWERED)
            {
                vehTypes |= VT_LAND_BIKE;
            }
            else
            {
                vehTypes |= VT_LAND_MOTORBIKE;
            }
            break;

        case VTL_WHEELED:
            vehTypes |= VT_LAND_WHEELED;
            break;

        case VTL_RAIL:
            vehTypes |= VT_LAND_RAILROAD;
            break;
        }
    }

    // set water flags
    if((VIP.vehMedia & VMD_WATER) != 0)
    {
        switch(VIP.vehType)
        {
        case VTW_BOAT:
        case VTW_CATAMARAN:
            vehTypes |= VT_WATER_BOAT;
            break;
        }
    }

    //// set underwater flags
    //if((VIP.vehMedia & VMD_UNDERWATER) != 0)
    //{
    //    vehTypes |= VT_UNDERWATER_VEHICLE;
    //}

    return vehTypes;
}

BYTE GtaVehInfo::getVehicleTypes(DWORD vehicleModel)
{
//    lss << L("GVI.GetTypes: gtaVer ") << ulhex(m_gtaVersion) << L(" mdl ") << ulhex(vehicleModel) << L(" min ") << ulhex(m_indexMin)  << L(" max ") << ulhex(m_indexMax) << UL::ENDL;
    if((m_gtaVersion == GTA_UNKNOWN) || (vehicleModel < m_indexMin) || (vehicleModel > m_indexMax))
    {
//        lss << L("GVI.GetTypes: 0 !") << UL::ENDL;
        return 0;
    }

    DWORD linkIndex = vehicleModel - m_indexMin;
    DWORD dataIndex = m_pInfoLink[linkIndex].infoIndex;
    VehInfoData& VIP = m_pInfoData[dataIndex];

    BOOL bIsPlayable;
    BYTE vehTypes = 0;
    bIsPlayable = (VIP.vehControl == VCT_MANNED);                     // no r.c. vehicles
    bIsPlayable &= ((VIP.vehAdditional & VAD_UNCONTROLLABLE) == 0);   // no dummy vehicles (III's airtrain [airliner], VC's blimp, SA's tram, etc.)
    if(bIsPlayable)
    {
        vehTypes = this->getVehicleTypes(VIP);
    }
//    lss << L("GVI.GetTypes: ") << ulhex(vehicleModel) << L(" -> ") << ulhex(&VIP) << L(" -> ") << ulhex(vehTypes) << UL::ENDL;
    return vehTypes;
}

