///////////////////////////////////////////////////////////////
//
// gtaVehInfo: vehicle database
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   23.09.2014 10:46 - created
//   04.10.2014 06:25 - added VEHTYPES & GetVehicleTypes()
//   25.11.2014 20:36 - moved out GTAVERSION
//   26.11.2014 17:52 - added VC vehicle model id's
//   19.12.2014 08:17 - rewritten VEHTYPES
//   10.07.2016 23:29 - cleaned up a bit
//   26.11.2016 08:44 - split VT_LAND_CYCLE & moved WTOL's to AMPHIBIOUS flag
//


#include <ulib/uDef.h>

#include "gtaVersion.h"

enum VEHTYPES {
    VT_LAND_BIKE = 0x01,        // all cycles (+ tricycles & quadricycles) [human-powered]
    VT_LAND_MOTORBIKE = 0x02,   // all motorcycles (+ tricycles & quadricycles) [non-human-powered]
    VT_LAND_WHEELED = 0x04,     // all automobiles
    VT_LAND_RAILROAD = 0x08,    // locomotives & cars
    VT_WATER_BOAT = 0x10,       // including catamarans & submarines
    VT_AMPHIBIOUS = 0x20,       // water takeoff/landing vehicle ability [WTOL]
    VT_AIR_CTOL = 0x40,         // "regular" planes (land takeoff/landing) [Conventional TOL]
    VT_AIR_VTOL = 0x80,         // helis & vtol planes (land takeoff/landing) [Vertical TOL]
};

enum VEHMODEL_III {
    VM_III_LANDSTALKER = 90,
    VM_III_IDAHO = 91,
    VM_III_STINGER = 92,
    VM_III_LINERUNNER = 93,
    VM_III_PERENNIAL = 94,
    VM_III_SENTINEL = 95,
    VM_III_PATRIOT = 96,
    VM_III_FIRETRUCK = 97,
    VM_III_TRASHMASTER = 98,
    VM_III_STRETCH = 99,
    VM_III_MANANA = 100,
    VM_III_INFERNUS = 101,
    VM_III_BLISTA_MINIVAN = 102,
    VM_III_PONY = 103,
    VM_III_MULE = 104,
    VM_III_CHEETAH = 105,
    VM_III_AMBULANCE = 106,
    VM_III_KURUMA_FBI = 107,    // fbi kuruma
    VM_III_MOONBEAM = 108,
    VM_III_ESPERANTO = 109,
    VM_III_TAXI = 110,
    VM_III_KURUMA = 111,
    VM_III_BOBCAT = 112,
    VM_III_MRWHOOPEE = 113,
    VM_III_BF_INJECTION = 114,
    VM_III_MANANA_CORPSE = 115,
    VM_III_POLICE = 116,
    VM_III_ENFORCER = 117,
    VM_III_SECURICAR = 118,
    VM_III_BANSHEE = 119,
    VM_III_PREDATOR = 120,      // police boat
    VM_III_BUS = 121,
    VM_III_RHINO = 122,         // army tank
    VM_III_BARRACKS = 123,
    VM_III_TRAIN = 124,         // el train & subway train
    VM_III_FAKE_HELI_CHOPPER = 125, // police heli?
    VM_III_DODO = 126,          // plane
    VM_III_COACH = 127,
    VM_III_CABBIE = 128,
    VM_III_STALLION = 129,
    VM_III_RUMPO = 130,
    VM_III_RCBANDIT = 131,      // rc car
    VM_III_MULE_TRIAD = 132,        // (a.k.a. bellyup)
    VM_III_MULE_MRWONGS = 133,
    VM_III_SENTINEL_MAFIA = 134,
    VM_III_LOBO_YARDIE = 135,
    VM_III_STINGER_YAKUZA = 136,
    VM_III_STALLION_DIABLO = 137,
    VM_III_CRUISER_CARTEL = 138,    // (a.k.a. columb)
    VM_III_RUMPO_HOODS = 139,
    VM_III_FAKE_AIRTRAIN = 140,     // plane [background-flying-vehicle] ("air-bus", "air-train")
    VM_III_FAKE_DEADDODO = 141,     // plane [background-flying-vehicle] (without banners)
    VM_III_SPEEDER = 142,       // boat
    VM_III_REEFER = 143,        // boat
    VM_III_PONY_PANLANTIC = 144,
    VM_III_FLATBED = 145,
    VM_III_YANKEE = 146,
    VM_III_FAKE_HELI_ESCAPE = 147,  // catalina heli?
    VM_III_CABBIE_BORGNINE = 148,
    VM_III_PONY_TOYZ = 149,
    VM_III_GHOST = 150,         // boat (civilian variant of "predator")
};
enum VEHMODEL_VC {
    VM_VC_LANDSTALKER = 130,
    VM_VC_IDAHO = 131,
    VM_VC_STINGER = 132,
    VM_VC_LINERUNNER = 133,
    VM_VC_PERENNIAL = 134,
    VM_VC_SENTINEL = 135,
    VM_VC_RIO = 136,            // boat (catamaran)
    VM_VC_FIRETRUCK = 137,
    VM_VC_TRASHMASTER = 138,
    VM_VC_STRETCH = 139,
    VM_VC_MANANA = 140,
    VM_VC_INFERNUS = 141,
    VM_VC_VOODOO = 142,
    VM_VC_PONY = 143,
    VM_VC_MULE = 144,
    VM_VC_CHEETAH = 145,
    VM_VC_AMBULANCE = 146,
    VM_VC_WASHINGTON_FBI = 147, // fbi washington
    VM_VC_MOONBEAM = 148,
    VM_VC_ESPERANTO = 149,
    VM_VC_TAXI = 150,
    VM_VC_WASHINGTON = 151,
    VM_VC_BOBCAT = 152,
    VM_VC_MRWHOOPEE = 153,
    VM_VC_BF_INJECTION = 154,
    VM_VC_HUNTER = 155,         // army heli
    VM_VC_POLICE = 156,
    VM_VC_ENFORCER = 157,
    VM_VC_SECURICAR = 158,
    VM_VC_BANSHEE = 159,
    VM_VC_PREDATOR = 160,       // police boat
    VM_VC_BUS = 161,
    VM_VC_RHINO = 162,          // army tank
    VM_VC_BARRACKS = 163,
    VM_VC_HERMES_CUBAN = 164,
    VM_VC_FAKE_HELI_CHOPPER = 165,  // police heli?
    VM_VC_ANGEL = 166,          // bike
    VM_VC_COACH = 167,
    VM_VC_CABBIE = 168,
    VM_VC_STALLION = 169,
    VM_VC_RUMPO = 170,
    VM_VC_RCBANDIT = 171,       // r.c. car
    VM_VC_ROMERO = 172,
    VM_VC_PACKER = 173,
    VM_VC_SENTINEL_XS = 174,
    VM_VC_ADMIRAL = 175,
    VM_VC_SQUALO = 176,         // boat
    VM_VC_SEASPARROW = 177,     // amphibious armed heli
    VM_VC_FAGGIO_PIZZABOY = 178,
    VM_VC_BURRITO_GANG = 179,
    VM_VC_FAKE_AIRTRAIN = 180,      // plane [background-flying-vehicle] ("air-bus", "air-train")
    VM_VC_FAKE_DEADDODO = 181,      // plane [background-flying-vehicle] (banner plane)
    VM_VC_SPEEDER = 182,        // boat
    VM_VC_REEFER = 183,         // boat
    VM_VC_TROPIC = 184,         // boat
    VM_VC_FLATBED = 185,
    VM_VC_YANKEE = 186,
    VM_VC_CADDY = 187,
    VM_VC_CABBIE_ZEBRA = 188,
    VM_VC_PONY_TOPFUN = 189,
    VM_VC_SKIMMER = 190,        // amphibious plane
    VM_VC_PCJ600 = 191,         // bike
    VM_VC_FAGGIO = 192,         // bike
    VM_VC_FREEWAY = 193,        // bike
    VM_VC_RCBARON = 194,        // r.c. plane
    VM_VC_RCRAIDER = 195,       // r.c. heli (green)
    VM_VC_GLENDALE = 196,
    VM_VC_OCEANIC = 197,
    VM_VC_SANCHEZ = 198,        // bike
    VM_VC_SPARROW = 199,        // heli
    VM_VC_PATRIOT = 200,
    VM_VC_LOVEFIST = 201,
    VM_VC_COASTGUARD = 202,     // boat
    VM_VC_DINGHY = 203,         // boat
    VM_VC_HERMES = 204,
    VM_VC_SABRE = 205,
    VM_VC_SABRE_TURBO = 206,
    VM_VC_PHOENIX = 207,
    VM_VC_WALTON = 208,
    VM_VC_REGINA = 209,
    VM_VC_COMET = 210,
    VM_VC_DELUXO = 211,
    VM_VC_BURRITO = 212,
    VM_VC_MULE_SPANDEX = 213,
    VM_VC_MARQUIS = 214,        // boat
    VM_VC_BAGGAGE = 215,
    VM_VC_CABBIE_KAUFMAN = 216,
    VM_VC_MAVERICK = 217,       // heli
    VM_VC_MAVERICK_VCN = 218,   // heli
    VM_VC_RANCHER = 219,
    VM_VC_RANCHER_FBI = 220,
    VM_VC_VIRGO = 221,
    VM_VC_GREENWOOD = 222,
    VM_VC_JETMAX = 223,         // boat
    VM_VC_HOTRING = 224,
    VM_VC_SANDKING = 225,
    VM_VC_BLISTA_COMPACT = 226,
    VM_VC_MAVERICK_VCPD = 227,  // police heli
    VM_VC_BOXVILLE = 228,
    VM_VC_BENSON = 229,
    VM_VC_MESA = 230,
    VM_VC_RCGOBLIN = 231,       // r.c. heli (red)
    VM_VC_HOTRING_A = 232,
    VM_VC_HOTRING_B = 233,
    VM_VC_BLOODRING_A = 234,
    VM_VC_BLOODRING_B = 235,
    VM_VC_CHEETAH_VCPD = 236,   // undercover police
};



struct VehInfoLink8;
typedef VehInfoLink8 GtaVehInfoLink;    // model id -> vehicle info
struct VehInfoData;

class GtaVehInfo {
private:
    DWORD m_gtaVersion;
    DWORD m_indexMin;
    DWORD m_indexMax;
    GtaVehInfoLink* m_pInfoLink;
    VehInfoData* m_pInfoData;
    BYTE getVehicleTypes(const VehInfoData& VIP);
public:
    GtaVehInfo()
        : m_gtaVersion(GTA_UNKNOWN)
    {
    }
    void init(DWORD gtaVersion);
    BYTE getVehicleTypes(DWORD vehicleModel);
};


