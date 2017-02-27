///////////////////////////////////////////////////////////////
//
// parkInitData: initial (predefined) parking lots data
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   27.12.2016 06:22 - created (copying data from g4park)
//   29.12.2016 17:29 - done copying g4park data
//



#undef PARKID

//
// Broker/Dukes
//

// Dukes, FIA, heli pads
#define PARKID  FIAH
PARK(
    BBOX(
        2120.0, 620.0, 3.0,  2330.0, 840.0, 75.0,
        2100.0, 600.0, 1.0,  2350.0, 860.0, 95.0
    ),
    ROW(
        P0, IPD_VT_HELI_IV, 3,
        FV4( 2214.3125, 718.6875, 4.875, 270.0 ),
        FV4( +16.5625, +18.71875, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// right E parking (FIA)
//   ____ 16.21 ____
//  /               \ _14.50
//  \____ 11.30 ____/
//  /               \ __9.25
//  \_____ 6.50 ____/
//               \________ 4.91

// x[] 2216.75 2280.375    y[] 327.0 408.875       y() 437.811 298.047    y(/) 312.0 424.0    z() 9.259 14.501
// N (8 places): 180.0   2221.5     404.0   zzz    step: +4.390625 0.0 0.0
// S (8 places):   0.0   2275.65625 331.625 zzz    step: -4.390625 0.0 0.0

// Dukes, FIA, Parking (roof) (north)
#define PARKID  FIA3
PARK(
    BBOX(
        2120.0, 300.0, 15.0,  2320.0, 500.0, 70.0,
        2100.0, 280.0, 14.0,  2340.0, 520.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 8,
        FV4( 2221.5, 404.0, 16.375, 180.0 ),
        FV4( +4.390625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Dukes, FIA, Parking (middle) (south)
#define PARKID  FIA2
PARK(
    BBOX(
        2216.75, 327.0, 10.0,  2280.375, 409.0, 14.0,
        2215.75, 312.0,  9.0,  2281.375, 424.0, 15.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 8,
        FV4( 2275.65625, 331.625, 11.5, 0.0 ),
        FV4( -4.390625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Dukes, FIA, Parking (ground) (north)
#define PARKID  FIA1
PARK(
    BBOX(
        2216.75, 317.0, 5.5,  2300.0, 409.0, 14.0f,     // extended bounding box for drive-in ways
        2215.75, 312.0, 4.0,  2303.0, 424.0, 15.0f
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 8,
        FV4( 2221.5, 404.0, 6.625, 180.0 ),
        FV4( +4.390625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Dukes, Willis, North, open parking
#define PARKID  WINO
PARK(
    BBOX(
        1680.0, 670.0, 16.0,  1860.0, 830.0, 70.0,
        1660.0, 650.0, 12.0,  1880.0, 850.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 8,
        FV4( 1764.8125, 761.5, 24.28125, 180.0 ),
        FV4( +3.21875, 0.0, +0.03125, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Broker, Schottler, 24H park
#define PARKID  BS24
PARK(
    BBOX(
        1111.0, 102.0, 24.0,  1180.0, 201.0, 70.0,
        1081.0,  82.0, 20.0,  1200.0, 221.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 7,
        FV4( 1144.0, 136.0, 32.125, 0.0 ),
        FV4( -3.5, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Broker, Hove Beach, at Niko's house (Mohawk Ave / Bart St)
#define PARKID  BHOB
PARK(
    BBOX(
        897.0, -555.0, 12.0,  970.0, -475.0, 55.0,
        877.0, -575.0,  7.0,  990.0, -455.0, 75.0
    ),
    POINT(
        P0, IPD_VT_LAND_IV,
        FV4( 944.0, -500.0, 14.5, 90.0 ),
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

//
// Algonquin
//

// Algonquin, North Holland, at Louis' house (Galveston Ave / Wardite St)
#define PARKID  ANHL
PARK(
    BBOX(
        -447.0, 1374.0, 12.0,  -415.0, 1386.0, 40.0,
        -467.0, 1354.0,  7.0,  -395.0, 1406.0, 60.0
    ),
    POINT(
        P0, IPD_VT_LAND_IV,
        FV4( -433.0, 1382.0, 14.9375, 90.0 ),
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, North Holland, Flats block open parking
#define PARKID  ANHF
PARK(
    BBOX(
        -434.0, 1200.0, 20.0f,  -388.0, 1287.0, 75.0,
        -454.0, 1180.0, 10.0f,  -368.0, 1307.0, 95.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 9,
        FV4( -417.0, 1207.875, 21.9375, 270.0 ),
        FV4( 0.0, +3.1875, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, North Holland, small dock
#define PARKID  ANHD
PARK(
    BBOX(
        -770.0, 1050.0, -2.0,  -610.0, 1350.0, 10.0,
        -790.0, 1030.0, -3.0,  -590.0, 1370.0, 12.0
    ),
    ROW(
        D0, IPD_VT_WATER_IV, 2,
        FV4( -691.0, 1202.875, -0.5, 75.0 ),
        FV4( 0.0, -10.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// [ADD] Algonquin, East Holland, Flats block
//#define PARKID  AENH
//PARK(
//    BBOX(
//    ),
//    ROW(
//        P0, IPD_VT_LAND_IV, ,
//        FV4(  ),
//        FV4( , 0.0 )
//    ),
//    NONE,
//    NONE,
//    NONE
//)
//#undef PARKID

// mid W parking
//        _____ 20.5 _____
//       /                \
//      / _____ 12.9 ______\__
//      \/                    \
//     _/\______ 4.1 __________\___
//                                

// Algonquin, Middle Park East, at Niko's house (Albany Ave / Pyrite St)
#define PARKID  AMPE
PARK(
    BBOX(
        74.0, 810.0, 11.0,  187.0f, 922.0f, 65.0f,
        54.0, 790.0,  6.0,  207.0f, 942.0f, 85.0f
    ),
    POINT(
        P0, IPD_VT_LAND_IV,
        FV4( 132.0, 865.0, 13.75, 0.0 ),
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, Purgatory, Parking (roof)
#define PARKID  PUR3
PARK(
    BBOX(
        -855.0, 395.0, 18.0,  -615.0, 575.0, 75.0,
        -875.0, 375.0, 16.0,  -595.0, 595.0, 95.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -754.9375, 488.375, 20.5, 180.0 ),
        FV4( +3.1875, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, Purgatory, Parking (middle)
#define PARKID  PUR2
PARK(
    BBOX(
        -762.5, 449.0, 11.0,  -572.0, 493.0, 14.0,
        -764.0, 447.0,  9.0,  -570.0, 495.0, 19.0,  // closed-space bounding box: +2 m
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -754.9375, 488.375, 12.9375, 180.0 ),
        FV4( +3.1875, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, Purgatory, Parking (ground)
#define PARKID  PUR1
PARK(
    BBOX(
        -762.5, 449.0, 3.0,  -572.0, 493.0,  5.0,
        -764.0, 447.0, 1.0,  -570.0, 495.0, 10.0    // closed-space bounding box: +2 m
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -754.9375, 488.375, 4.125, 180.0 ),
        FV4( +3.1875, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, The Meat Quarter, docks
#define PARKID  MEQA
PARK(
    BBOX(
        -880.0, -175.0, -2.0,  -605.0, 25.0, 50.0,
        -900.0, -195.0, -3.0,  -585.0, 45.0, 70.0
    ),
    ROW(
        D0, IPD_VT_WATER_IV, 5,
        FV4( -745.0, -102.75, -0.5, 270.0 ),
        FV4( 0.0, +13,0625, 0.0, 0.0 )
    ),
    ROW(
        D1, IPD_VT_WATER_IV, 5,
        FV4( -769.5, -102.75, -0.5, 90.0 ),
        FV4( 0.0, +13,0625, 0.0, 0.0 )
    ),
    NONE,
    NONE
)
#undef PARKID

// Algonquin, The Exchange, open parking    (actually it's in Castle Garden City boundaries)
#define PARKID  ACGC
PARK(
    BBOX(
        -321.0, -700.0, 3.0,  -191.0, -540.0, 60.0,
        -341.0, -720.0, 0.0,  -171.0, -520.0, 80.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 8,
        FV4( -293.5625, -601.609375, 3.875, 210.0 ),
        FV4( +3.640625, +2.09375, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Algonquin, Hatton Gardens, parking
#define PARKID  HGAR
PARK(
    BBOX(
        70.0, 460.0, 9.0,  200.0, 710.0, 65.0,
        50.0, 440.0, 4.0,  220.0, 730.0, 85.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 9,
        FV4( 142.25, 602.0, 13.75, 90.0 ),
        FV4( 0.0, -3.1875, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

//
// Alderney
//

// left N parking
//  40.9 ____
//           \____ 38.6
//  36.3 ____x
//           \____ 34.0
//  31.7 ____/
//           x____ 29.3
//  27.0 ____/
//           \____ 24.7
//              \_______ 23.3

// Alderney, Westdyke, Parking (roof)
#define PARKID  WEP3
PARK(
    BBOX(
        -1145.0, 1330.0, 38.25,  -905.0, 1570.0, 70.0,
        -1165.0, 1310.0, 37.25,  -885.0, 1590.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -1026.125, 1453.25, 40.9375, 180.0 ),
        FV4( +4.5625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Westdyke, Parking (middle)
#define PARKID  WEP2
PARK(
    BBOX(
        -1036.0, 1401.0, 31.5,  -946.0, 1457.0, 36.75,
        -1037.0, 1400.0, 30.5,  -945.0, 1458.0, 37.75   // closed-space bounding box: +1 m
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -956.0, 1405.5, 34.0, 0.0 ),
        FV4( -4.5625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Westdyke, Parking (ground)
#define PARKID  WEP1
PARK(
    BBOX(
        -1036.0, 1401.0, 24.5,  -946.0, 1457.0, 30.0,
        -1037.0, 1400.0, 23.0,  -945.0, 1458.0, 31.0    // closed-space bounding box: +1 m
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 10,
        FV4( -1026.125, 1453.25, 27.125, 180.0 ),
        FV4( +4.5625, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Alderney City, at Niko's house (Mahesh Ave)
#define PARKID  AMAV
PARK(
    BBOX(
        -1036.0, 894.0, 10.0,  -967.0, 947.0, 70.0,
        -1056.0, 874.0,  5.0,  -947.0, 967.0, 90.0
    ),
    POINT(
        P0, IPD_VT_LAND_IV,
        FV4( -990.0, 925.0, 12.75, 180.0 ),
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Normandy, open parking near port entrance
#define PARKID  NOPE
PARK(
    BBOX(
        -1242.0, 547.0, 2.0,  -1127.0, 629.0, 50.0,
        -1262.0, 527.0, 0.0,  -1107.0, 649.0, 70.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 6,
        FV4( -1200.375, 594.5, 3.5, 180.0 ),
        FV4( +3.09375, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Acter, med.center parking
#define PARKID  AMCP
PARK(
    BBOX(
        -1600.0, 325.0, 19.0,  -1510.0, 435.0, 70.0,
        -1620.0, 305.0, 15.0,  -1490.0, 455.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 5,
        FV4( -1565.375, 380.375, 22.5, 270.0 ),
        FV4( 0.0, +3.15625, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Tudor (east), Diner parking
#define PARKID  TDIN
PARK(
    BBOX(
        -1507.0, -160.0, 3.0,  -1346.0, 20.0, 40.0,
        -1527.0, -180.0, 1.0,  -1326.0, 40.0, 60.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 9,
        FV4( -1448.125, -79.9375, 5.5, 270.0 ),
        FV4( 0.0, +3.203125, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// Alderney, Tudor (west), Flats block
#define PARKID  TWFB
PARK(
    BBOX(
        -2220.0, -125.0, 0.0,  -2100.0, -60.0, 70.0,
        -2240.0, -145.0, 0.0,  -2080.0, -40.0, 90.0
    ),
    ROW(
        P0, IPD_VT_LAND_IV, 5,
        FV4( -2145.125, -71.0, 5.9375, 180.0 ),
        FV4( +4.125, 0.0, 0.0, 0.0 )
    ),
    NONE,
    NONE,
    NONE
)
#undef PARKID

// [ADD] Alderney, Tudor, at Johnny's house (Hardtack Ave | Tinderbox Ave)
//#define PARKID  THAT
//PARK(
//    BBOX(
//    ),
//    POINT(
//        P0, IPD_VT_LAND_IV,
//        FV4(  ),
//    ),
//    NONE,
//    NONE,
//    NONE
//)
