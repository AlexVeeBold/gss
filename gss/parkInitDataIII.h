///////////////////////////////////////////////////////////////
//
// parkInitData: initial (predefined) parking lots data
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   12.10.2014 04:48 - created
//   12.10.2014 15:58 - re-written all III's parking lots
//   19.12.2014 08:45 - updated PVSS to use z-oriented bbox
//   12.07.2016 05:40 - renamed some stuff
//   13.11.2016 07:07 - removed tailing commas for compability with msvs11
//   24.11.2016 11:36 - replaced all PARKID macros with park id (name) only
//   27.12.2016 03:58 - replaced 1-place ROWs with POINTs
//



#undef PARKID

//
// Portland
//

#define PARKID  RLDB     // rldb     Red lights district, flats block [inside a VERY secluded area]
PARK(
    BBOX(
        852.625, -246.375, 2.0,  899.5, -173.75, 22.0, 
        845.375, -264.125, 1.0,  901.0, -138.25, 27.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 4,
        FV4( 883.4375, -192.5, 2.5, 180.0 ),
        FV4( +3.90625, 0.0, +0.15625, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  PBCH     // pbch     Portland Beach
PARK(
    BBOX(
        1420.0, -600.0, -1.0,  1670.0, -310.0, 52.0,
        1410.0, -610.0, -2.0,  1680.0, -300.0, 57.0
    ),
    //ROW(
    //    D0, IPD_VT_WATER_III, 1,
    //    FV4( 1524.625, -467.75, -0.5, 15.0 ),
    //    FV4( 0.0, 0.0, +8.0, 0.0 )
    //),
    POINT(
        D0, IPD_VT_WATER_III,
        FV4( 1524.625, -467.75, -0.5, 15.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  PVSS     // pvss     Portland View, Supa Save
PARK(
    ZOBBOX(
        1210.0,   -722.125, 12.0,  1397.5,  -529.75,  22.0,  58.25, 54.75,
        1183.625, -747.625, 11.0,  1412.25, -513.875, 27.0,  77.25, 72.5
    ),
    ROW(
        P0, IPD_VT_LAND_III, 5,
        FV4( 1265.0, -620.5, 11.125, 180.0 ),
        FV4( +3.75, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  CPQS     // cpqs     Callahan Point, quayside
PARK(
    BBOX(
        820.0, -1158.0, 8.0,  942.0, -1049.0, 22.0,
        800.0, -1164.0, 7.0,  968.0, -1037.0, 27.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 8,
        FV4( 898.1875, -1098.3125, 12.125, 90.0 ),
        FV4( 0.0, -4.625, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

//
// Staunton
//

#define PARKID  RCGH     // rcgh     Rockford, Carson General Hospital
PARK(
    BBOX(
        145.0, -51.0, 10.0,  380.0, 63.0, 22.0,
        120.0, -66.0,  9.0,  405.0, 78.0, 27.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( 276.375, 0.0, 15.125, 330.0 ),   // zero y !
        FV4( +4.375, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  AWBD     // awbd     Aspatria, west beach, docks
PARK(
    BBOX(
        -390.0, -250.0, -1.0,  -210.0, -10.0, 32.0,
        -400.0, -260.0, -2.0,  -200.0,   0.0, 37.0
    ),
    ROW(
        P0, IPD_VT_WATER_III, 2,
        FV4( -297.25, -114.0, -0.5, 187.5 ),
        FV4( +0.875, -20.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  ALMC     // almc     Aspatria, Liberty Memorial Coliseum
PARK(
    BBOX(
         -88.0, -397.0, 14.0,  12.0, -250.0, 32.0,
        -108.0, -417.0, 13.0,  32.0, -230.0, 37.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( -29.75, -358.375, 15.125, 270.0 ),
        FV4( 0.0, +2.875, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  NMSPL   // nmspl    Newport Multistory Car Park (low)
PARK(
    BBOX(
        267.0,  -611.25, 25.0,  346.5,  -479.75, 30.375,
        266.75, -611.5,  24.0,  346.75, -479.5,  30.5
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( 331.0, -566.5, 28.0, 270.0 ),
        FV4( 0.0, +4.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID
//
//  Staunton, Newport, Multistory Car Park
//      35.234      ______                  +2.402
//                        \____x_ 32.832    +2.401
//      30.431      ______/                 +2.402
//                        \___x__ 28.029    +2.901
//      25.128 ____ ______/                  0.0
//
//    low  lane z:  24.000  | 25.000 ... 30.375 |  30.500
//    high lane z:  30.375  | 30.500 ... 42.000 |  47.000
//
#define PARKID  NMSPH    // nmsph    Newport Multistory Car Park (high)
PARK(
    BBOX(
        267.0,  -611.25, 30.5,    346.5,  -479.75, 42.0,
        266.75, -611.5,  30.375,  346.75, -479.5,  47.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( 342.25, -544.375, 32.8125, 90.0 ),
        FV4( 0.0, -4.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

//  Staunton, Newport, east coast flats block
//      34.978    _________________     double highway (road level)
//                                      
//      24.978     _________________    highway to casino (road level)
//                              _       
//      14.978   ______x_____.-`        flats (road level)
//                                      
//       0.0     ,,x,,,,x,,,,,,,,,,     docks (water level)
//
#define PARKID  NESF     // nesf     Newport, east side flats block + jetty
PARK(
    BBOX(
        407.0, -910.0, -1.0,  690.0, -580.0, 23.0,
        397.0, -920.0, -2.0,  700.0, -570.0, 24.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 5,
        FV4( 491.0, -644.0, 15.0625, 180.0 ),
        FV4( +4.5, -0.0625, 0.0, 0.0 )
    ),
    ROW(
        D0, IPD_VT_WATER_III, 3,
        FV4( 556.75, -685.0, -0.5, 180.0 ),
        FV4( +25.625, 0.0, 0.0, 0.0 )
    ),
    ROW(
        D1, IPD_VT_WATER_III, 3,
        FV4( 529.75, -788.0, -0.5, 180.0 ),
        FV4( +25.625, 0.0, 0.0, 0.0 )
    ),
    NONE
)
#undef PARKID

#define PARKID  BPLT     // bplt     Bedford Point, Liberty Tree tower backyard + Underground parking area
PARK(
    BBOX(
        -120.0, -1521.0, 18.0,  18.0, -1398.0, 42.0,
        -131.0, -1535.0, 17.0,  37.0, -1372.0, 47.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 7,
        FV4( -101.375, -1444.0, 25.0625, 180.0 ),
        FV4( +4.390625, 0.0, 0.0, 0.0 )
    ),
    ROW(
        P1, IPD_VT_LAND_III, 3,
        FV4( -10.0, -1466.5, 18.75, 270.0 ),
        FV4( -12.75, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE
)
#undef PARKID

#define PARKID  TAMU     // tamu     Torrington, AM Petroleum Company (AmCo) underground car park
PARK(
    BBOX(
        207.0, -1261.0, 18.0,  263.0, -1169.0, 25.0,
        197.0, -1272.0, 17.0,  264.0, -1168.0, 26.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 3,
        FV4( 258.5, -1224.75, 19.375, 90.0 ),
        FV4( 0.0, -3.375, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

//
// Shoreside Vale
//

// Shoreside Vale, Cochrane Dam
//      67.195    _________________     top (road level)
//                                      
//      22.571              ________    middle (road level)
//               _                      
//       2.671    `-.___x____           ground (road level)
//                                      
//       0.0       ,,,,x,,,,,,,,,,      dock (water level)
//
#define PARKID  CDAM    // cdam     Cochrane Dam (parking + jetty)
PARK(
    BBOX(
        -1210.0, 150.0, -1.0,  -950.0, 450.0, 17.0,
        -1220.0, 140.0, -2.0,  -940.0, 460.0, 18.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 2,
        FV4( -1198.0, 316.0, 2.375, 270.0 ),
        FV4( 0.0, +5.0, 0.0, 0.0 )
    ),
    POINT(
        D0, IPD_VT_WATER_III,
        FV4( -1109.0, 279.0, -0.5, 270.0 )
    ),
    NONE,
    NONE
)
#undef PARKID

#define PARKID  WGPS     // wgps     Wichita Gardens, Park, car park site
PARK(
    BBOX(
        -300.0, 190.0,  0.0,  -175.0, 340.0, 22.0,
        -310.0, 180.0, -1.0,  -165.0, 350.0, 27.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( -224.0, 307.0, 2.4375, 90.0 ),
        FV4( 0.0, -4.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  CGTH     // cgth     Cedar Grove, Twisty Highway (middle level)
PARK(
    BBOX(
        -800.0, 107.0, 22.0,  -650.0, 207.0, 39.0,
        -810.0, 105.0, 20.0,  -640.0, 212.0, 41.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 8,
        FV4( -750.125, 202.75, 27.625, 180.0 ),
        FV4( +5.0625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

#define PARKID  FIAP     // fiap     Francis International Airport, car park
PARK(
    BBOX(
        -945.0, -455.0, 8.0,  -760.0, -335.0, 32.0,
        -955.0, -465.0, 6.0,  -750.0, -325.0, 37.0
    ),
    ROW(
        P0, IPD_VT_LAND_III, 10,
        FV4( -879.0, -392.0, 10.0625, 180.0 ),
        FV4( +3.21875, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID


