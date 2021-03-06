﻿///////////////////////////////////////////////////////////////
//
// gtaVehInfoData: vehicle database (data)
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   28.11.2014 00:58 - created
//

// GTA = common GTA vehicle
// MHC = Manned, HydroCarbon powered vehicle
// MEL = Manned, ELectric powered vehicle
// UEL = Unmanned, ELectric powered vehicle
// URCEL = Unmanned, Remote-Controlled, ELectric powered vehicle
//
VEHID(  FAKE_DODO_PLANE,            VMD_AIR,    VTA_CTOL,       VAC_PLANE,      VSZ_SMALLEST,   VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_UNCONTROLLABLE)
VEHID(  FAKE_AIRTRAIN_PLANE,        VMD_AIR,    VTA_CTOL,       VAC_PLANE,      VSZ_LARGEST,    VCL_PUBLIC,         VPW_HYDROCARBON,    VCT_MANNED, VAD_UNCONTROLLABLE)
VEHID(  FAKE_ESCAPE_HELI,           VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_UNCONTROLLABLE)
VEHID(  FAKE_POLICE_HELI,           VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_UNCONTROLLABLE)
//
VEHID(  GTA_FIRETRUCK,                  VMD_LAND,   VTL_WHEELED,    VLW_RIGIDTRUCK, VSZ_LARGE,      VCL_EMERGENCY,      VPW_HYDROCARBON,    VCT_MANNED, VAD_WATER_CANNON | VAD_SIREN_WITH_LIGHTS)
VEHID(  GTA_AMBULANCE,                  VMD_LAND,   VTL_WHEELED,    VLW_BOXTRUCK,   VSZ_LARGE,      VCL_EMERGENCY,      VPW_HYDROCARBON,    VCT_MANNED, VAD_SIREN_WITH_LIGHTS)
VEHID(  GTA_TANK,                       VMD_LAND,   VTL_OTHER,      VLO_COMBAT,     VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_ARMORED | VAD_ARMED)
//
VEHID(  MHC_SMALL_BASIC_HARDTOP,            VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_SMALL,      VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_SMALL_BASIC_SUV,                VMD_LAND,   VTL_WHEELED,    VLW_SUV,        VSZ_SMALL,      VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_BASIC_HARDTOP,           VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ADVANCED_HARDTOP,        VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_SUPERIOR_HARDTOP,        VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_SUPERIOR,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_RACING_HARDTOP,          VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_RACING,         VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_CRIMINAL_HARDTOP,        VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_CRIMINAL,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_SERVICE_HARDTOP,         VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_SERVICE,        VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_SUPERIOR_CABRIO,         VMD_LAND,   VTL_WHEELED,    VLW_SOFTTOP,    VSZ_MEDIUM,     VCL_SUPERIOR,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_LIMOUSINE,               VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_LIMOUSINE,      VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_BASIC_MPV,               VMD_LAND,   VTL_WHEELED,    VLW_MPV,        VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ADVANCED_MPV,            VMD_LAND,   VTL_WHEELED,    VLW_MPV,        VSZ_MEDIUM,     VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_CARGO_VAN,               VMD_LAND,   VTL_WHEELED,    VLW_VAN,        VSZ_MEDIUM,     VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_CRIMINAL_VAN,            VMD_LAND,   VTL_WHEELED,    VLW_VAN,        VSZ_MEDIUM,     VCL_CRIMINAL,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ADVANCED_SUV,            VMD_LAND,   VTL_WHEELED,    VLW_SUV,        VSZ_MEDIUM,     VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_RACING_SUV,              VMD_LAND,   VTL_WHEELED,    VLW_SUV,        VSZ_MEDIUM,     VCL_RACING,         VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ENFORCEMENT_SUV,         VMD_LAND,   VTL_WHEELED,    VLW_SUV,        VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_CARGO_PICKUP,            VMD_LAND,   VTL_WHEELED,    VLW_PICKUP,     VSZ_MEDIUM,     VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_CRIMINAL_PICKUP,         VMD_LAND,   VTL_WHEELED,    VLW_PICKUP,     VSZ_MEDIUM,     VCL_CRIMINAL,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_LIMOUSINE,                VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_LARGE,      VCL_LIMOUSINE,      VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_CARGO_STEPVAN,            VMD_LAND,   VTL_WHEELED,    VLW_STEPVAN,    VSZ_LARGE,      VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_CARGO_BOXTRUCK,           VMD_LAND,   VTL_WHEELED,    VLW_BOXTRUCK,   VSZ_LARGE,      VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_CRIMINAL_BOXTRUCK,        VMD_LAND,   VTL_WHEELED,    VLW_BOXTRUCK,   VSZ_LARGE,      VCL_CRIMINAL,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_CARGO_SEMITRAILER,        VMD_LAND,   VTL_WHEELED,    VLW_SEMITRAILER, VSZ_LARGE,     VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_CARGO_RIGIDTRUCK,         VMD_LAND,   VTL_WHEELED,    VLW_RIGIDTRUCK, VSZ_LARGE,      VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_UTILITY_RIGIDTRUCK,       VMD_LAND,   VTL_WHEELED,    VLW_RIGIDTRUCK, VSZ_LARGE,      VCL_UTILITY,        VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_ENFORCEMENT_RIGIDTRUCK,   VMD_LAND,   VTL_WHEELED,    VLW_RIGIDTRUCK, VSZ_LARGE,      VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGEST_BUS,                    VMD_LAND,   VTL_WHEELED,    VLW_BUS,        VSZ_LARGEST,    VCL_PUBLIC,         VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGEST_CARGO_RIGIDTRUCK,       VMD_LAND,   VTL_WHEELED,    VLW_RIGIDTRUCK, VSZ_LARGEST,    VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_BASIC_MOTORCYCLE,               VMD_LAND,   VTL_CYCLE,      VLC_BICYCLE,    VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_ADVANCED_MOTORCYCLE,            VMD_LAND,   VTL_CYCLE,      VLC_BICYCLE,    VSZ_MEDIUM,     VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_CARGO_MOTORCYCLE,               VMD_LAND,   VTL_CYCLE,      VLC_BICYCLE,    VSZ_MEDIUM,     VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_CRIMINAL_MOTORCYCLE,            VMD_LAND,   VTL_CYCLE,      VLC_BICYCLE,    VSZ_MEDIUM,     VCL_CRIMINAL,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
//
VEHID(  MHC_SMALL_BASIC_INFLATABLE_BOAT,    VMD_WATER,  VTW_BOAT,       VWB_INFLATABLE, VSZ_SMALL,      VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_BASIC_FISHING_BOAT,      VMD_WATER,  VTW_BOAT,       VWB_FISHING,    VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ADVANCED_SPEED_BOAT,     VMD_WATER,  VTW_BOAT,       VWB_SPEED,      VSZ_MEDIUM,     VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_ADVANCED_SPEED_BOAT,      VMD_WATER,  VTW_BOAT,       VWB_SPEED,      VSZ_LARGE,      VCL_ADVANCED,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_SUPERIOR_MOTOR_BOAT,      VMD_WATER,  VTW_BOAT,       VWB_MOTOR,      VSZ_LARGE,      VCL_SUPERIOR,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_SUPERIOR_SAIL_BOAT,       VMD_WATER,  VTW_BOAT,       VWB_SAIL,       VSZ_LARGE,      VCL_SUPERIOR,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_LARGE_SUPERIOR_SAIL_CATAMARAN,  VMD_WATER,  VTW_CATAMARAN,  VWB_SAIL,       VSZ_LARGE,      VCL_SUPERIOR,       VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_EMERGENCY_SPEED_BOAT,    VMD_WATER,  VTW_BOAT,       VWB_SPEED,      VSZ_MEDIUM,     VCL_EMERGENCY,      VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_SMALLEST_BASIC_PLANE,           VMD_AIR,    VTA_CTOL,       VAC_PLANE,      VSZ_SMALLEST,   VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_SMALLEST_BASIC_HELI,            VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_SMALLEST,   VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_SMALL_BASIC_HELI,               VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_SMALL,      VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_BASIC_HELI,              VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_MEDIUM,     VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_MEDIUM_ENFORCEMENT_HELI,        VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
//
VEHID(  MEL_SMALLEST_UTILITY_CABRIO,        VMD_LAND,   VTL_WHEELED,    VLW_SOFTTOP,    VSZ_SMALLEST,   VCL_UTILITY,        VPW_ELECTRIC,       VCT_MANNED,     VAD_NONE)
VEHID(  MEL_SMALLEST_SERVICE_HARDTOP,       VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_SMALLEST,   VCL_SERVICE,        VPW_ELECTRIC,       VCT_MANNED,     VAD_NONE)
VEHID(  UEL_TRAIN_CAR,                      VMD_LAND,   VTL_RAIL,       VLR_CAR,        VSZ_MEDIUM,     VCL_PUBLIC,         VPW_ELECTRIC,       VCT_MANNED,     VAD_UNCONTROLLABLE)
VEHID(  URCEL_RACE_CAR,                     VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_SMALLEST,   VCL_RACING,         VPW_ELECTRIC,       VCT_UNMANNED,   VAD_REMOTE_CONTROLLED)
VEHID(  URCEL_PLANE,                        VMD_AIR,    VTA_CTOL,       VAC_PLANE,      VSZ_SMALLEST,   VCL_BASIC,          VPW_ELECTRIC,       VCT_UNMANNED,   VAD_REMOTE_CONTROLLED)
VEHID(  URCEL_HELI,                         VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_SMALLEST,   VCL_BASIC,          VPW_ELECTRIC,       VCT_UNMANNED,   VAD_REMOTE_CONTROLLED)
//
VEHID(  MHC_MEDIUM_PUBLIC_HARDTOP_WITH_TAXILIGHTS,      VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_PUBLIC,         VPW_HYDROCARBON,    VCT_MANNED, VAD_TAXI_LIGHTS)
VEHID(  MHC_LARGE_CARGO_VAN_ARMORED,                    VMD_LAND,   VTL_WHEELED,    VLW_VAN,        VSZ_LARGE,      VCL_CARGO,          VPW_HYDROCARBON,    VCT_MANNED, VAD_ARMORED)
VEHID(  MHC_MEDIUM_ENFORCEMENT_HARDTOP_WITH_SIREN,      VMD_LAND,   VTL_WHEELED,    VLW_GENERIC,    VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_SIREN_WITH_LIGHTS)
VEHID(  MHC_LARGE_ENFORCEMENT_BOXTRUCK_WITH_SIREN,      VMD_LAND,   VTL_WHEELED,    VLW_BOXTRUCK,   VSZ_LARGE,      VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_SIREN_WITH_LIGHTS)
VEHID(  MHC_MEDIUM_ENFORCEMENT_SPEED_BOAT_WITH_GUN,     VMD_WATER,  VTW_BOAT,       VWB_SPEED,      VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_ARMED)
VEHID(  MHC_MEDIUM_ENFORCEMENT_HELI_WITH_GUN,           VMD_AIR,    VTA_VTOL,       VAV_HELI,       VSZ_MEDIUM,     VCL_ENFORCEMENT,    VPW_HYDROCARBON,    VCT_MANNED, VAD_ARMED)
VEHID(  MHC_SMALLEST_BASIC_AMPHIBIOUS_PLANE,            VMD_AIR_AMPHIBIOUS, VTA_CTOL, VAC_PLANE,    VSZ_SMALLEST,   VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_NONE)
VEHID(  MHC_SMALLEST_BASIC_AMPHIBIOUS_HELI_WITH_GUN,    VMD_AIR_AMPHIBIOUS, VTA_VTOL, VAV_HELI,     VSZ_SMALLEST,   VCL_BASIC,          VPW_HYDROCARBON,    VCT_MANNED, VAD_ARMED)
//
