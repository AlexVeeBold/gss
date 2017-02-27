///////////////////////////////////////////////////////////////
//
// gtaVersion: GTA game version
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   26.11.2014 04:19 - created
//   23.11.2016 07:27 - split version&generation enums
//

#pragma once


enum GTAGENERATION {
    GTAGEN_3 = 0x0300,
    GTAGEN_4 = 0x0400,
    GTAGEN_5 = 0x0500,
    GTA_GENERATION_MASK = 0xFF00,
};
enum GTAVERSION {
    GTA_UNKNOWN = 0x0000,
    GTA_III = GTAGEN_3 | 0x01,
    GTA_VC = GTAGEN_3 | 0x02,
    GTA_SA = GTAGEN_3 | 0x14,
    GTA_IV = GTAGEN_4 | 0x01,
    GTA_TLAD = GTAGEN_4 | 0x02,
    GTA_TBOGT = GTAGEN_4 | 0x03,
    GTA_EFLC = GTAGEN_4 | 0x14,   // tlad+tbogt release
    GTA_V = GTAGEN_5 | 0x01,
};
#define GTA_GENERATION(v)   (v & GTA_GENERATION_MASK)

