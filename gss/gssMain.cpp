///////////////////////////////////////////////////////////////
//
// gss: GTA script system
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   28.02.2014 04:12 - created
//   10.03.2014 22:48 - signature code address retriever (SCAR) tested
//   11.03.2014 14:48 - signature code address retriever (SCAR) improved & tested (again)
//   12.03.2014 17:44 - written scan logic, fixed fill-me-harder bug in fillSiHdrStruct()
//   13.03.2014 02:27 - SCAR finished (hopefully)
//   18.03.2014 00:00 - signatures are moved out
//   18.03.2014 07:51 - written combined(multi-source) GtaThread data layout
//   19.03.2014 04:09 - written combined(multi-source) GtaThread class interface
//   20.03.2014 02:35 - SCAR moved out, GtaThread improved
//   21.03.2014 03:07 - introduced gta interface (GI)
//   22.03.2014 07:18 - written & tested inject-thread-by-hook system
//   22.03.2014 11:27 - added GetNativeAddress()
//   22.03.2014 18:10 - introduced ultra-fast native-script-caller SNCall()
//   23.03.2014 13:31 - everything looks good (and working as expected)
//   31.03.2014 23:30 - written DrawString & DrawStringSimple
//   03.04.2014 20:10 - first park place (mid. N flats) working (Red Dilettante spawner)
//   06.04.2014 11:35 - written & tested parkStateMachine
//   07.04.2014 13:45 - parking code working good (just not saving to file yet [it's easy])
//   08.04.2014 07:01 - parking code improved, enabled park data saving
//   11.04.2014 17:55 - added "no-wanted" area in airport to get on 3 helicopter landing spots
//   11.04.2014 21:32 - finished & tested parking places (now 22, including 3 multi-store parkings)
//   17.04.2014 13:27 - moved into g4ss project (removed everything above ZionStateMachine)
//   24.08.2014 04:01 - moved into gss project (GTA III/VC/SA/IV/EFLC unified script system)
//   28.08.2014 23:14 - invented new macro system for native call database
//   29.08.2014 01:06 - finished up & tested new macro system & native caller for III
//   31.08.2014 07:19 - added zone marker box drawing
//   01.09.2014 04:39 - written zone edge manual movement
//   03.09.2014 08:46 - tested III's car spawn
//   06.09.2014 17:14 - written veh.read & veh.spawn using just-written brand-new vehicle data direct access (with car variations & alarm)
//   08.09.2014 14:14 - zone adjustment system rewritten (user experience improved)
//   11.09.2014 03:29 - tested III's marker drawing
//   11.09.2014 23:39 - incorporated direct access to internal marker drawing code (with custom marker colors)
//   13.09.2014 04:37 - cleaned up a little, moved stuff to different modules
//   14.09.2014 04:56 - gathered vehicle classification information for database
//   14.09.2014 09:23 - written small part of III's vehicle database (draft)
//   22.09.2014 22:15 - finished III's vehicle database
//   23.09.2014 18:12 - first III's parking tested (kuruma spawner)
//   25.09.2014 13:12 - tested III's Portland parking lots
//   26.09.2014 23:46 - parking lot code reorganized & tested (saving vehicles, but no saving-to-file yet)
//   27.09.2014 19:48 - refactored parking lot code (not tested yet)
//   28.09.2014 02:42 - improved zone editor a bit (predefined parking zone loading [from memory])
//   29.09.2014 02:36 - added III's Staunton parking lots
//   29.09.2014 23:06 - written basic parking data file structures
//   30.09.2014 04:35 - written III's initial parking data (portland + staunton)
//   01.10.2014 00:28 - improved initial parking data, added runtime data structures
//   02.10.2014 01:12 - written main runtime data structure + inital data loader
//   02.10.2014 04:50 - incorporated data file loading and saving procedures
//   02.10.2014 20:30 - updated parking and zone routines to work with new data file procedures
//   04.10.2014 08:14 - added & tested III's Shoreside parking lots, added "free vehicle day"
//   07.10.2014 08:31 - added III's docks
//   07.10.2014 16:18 - tested all III's parking zones
//   09.10.2014 09:20 - written basic macros for automatic initial parking data generation
//   11.10.2014 23:52 - incorporated advanced macro system for initial parking data
//   12.10.2014 15:15 - finished advanced macro system for initial parking data
//   26.11.2014 05:18 - tested VC interoperation, everything seems work fine! (except issues with III's parking coords & III's vehinfo)
//   28.11.2014 02:52 - tested VC again (with new veh.info) - not so good, III's gta.vehicle.internal is definitely need to be rewritten
//   28.11.2014 23:12 - upgraded veh.internal & expanded vehinfo3 structure from 8 to 16 bytes (and changed some magic signatures)
//   29.11.2014 00:19 - another VC test: veh.model is not loading -> conditional script code is broken and definitely needs to be rewritten
//   29.11.2014 05:27 - added VC structures to script code, parking looks like working now
//   02.12.2014 05:09 - moved zone, vehicle, math, misc.gta into their own modules & tested it afterwards
//   19.12.2014 08:44 - switched parking to use z-oriented bounding box
//   04.01.2015 11:36 - added correct z-oriented bounding box test
//   10.07.2016 18:30 - cleaning things up (it's been a while, huh?)
//   11.11.2016 18:56 - adding IV back (a-little)
//   13.11.2016 04:33 - moved project from VS6 to VS11 (coz of whole bunch of useful features)
//


#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <psapi.h>

#include "tunic.h"
#include "tstr.h"
#include "uDef.h"
#include "uLog.h"
#include "uFile.h"
#include "uMemory.h"
#include "uRandom.h"


#include "gss.h"

#include "gssDef.h"

#include "gssInterface.h"
#include "gssScriptCall.h"

#include "gtaVersion.h"
#include "gtaScan.h"

#include "parkSys.h"



DWORD gtaFont;

DWORD gssGtaVer;

void gssReset(void)
{
    //dwTimePrev = 0;
}
void gssProcess(void)
{
    AStringStream ss("");
    ss.precision(3);
    ss.setf(std::ios::fixed);
    DWORD playerIndex;

    GtaPlayerState playerState;

    playerIndex = 0;
    SN::PLAYER::GET_PLAYER_CHAR(playerIndex, &playerState.charPlayer);
    ss << "Ped:" << playerState.charPlayer;
    if(playerState.charPlayer != 0)
    {
        SN::PED::GET_CHAR_COORDINATES(playerState.charPlayer, &playerState.fvPosChar);
        ss << " (" << playerState.fvPosChar.X << ", " << playerState.fvPosChar.Y << ", " << playerState.fvPosChar.Z << ")";
        playerState.bIsInCar = SN::PED::IS_CHAR_IN_ANY_CAR(playerState.charPlayer);
        //ss << ", inCar:" << playerState.bIsInCar;
        if(playerState.bIsInCar)
        {
            SN::VEHICLE::STORE_CAR_CHAR_IS_IN_NO_SAVE(playerState.charPlayer, &playerState.vehiclePlayer);
            ss << ", Car:" << playerState.vehiclePlayer;

            void* pCVehicle;
            pCVehicle = GtaGetCVehiclePtr(playerState.vehiclePlayer);
            ss << " CVeh:" << std::hex << (DWORD)(pCVehicle) << std::dec;
            ss << " m:" << std::hex << std::setfill('0') << std::setw(4) << *(WORD*)((BYTE*)pCVehicle + 0x5C) << std::dec;

            INT type = -1;
            if(gssGtaVer == GTA_III)
            {
                type = *(INT*)((BYTE*)pCVehicle + 0x284);
            }
            if(gssGtaVer == GTA_VC)
            {
                type = *(INT*)((BYTE*)pCVehicle + 0x29C);
            }
            ss << " T:" << type;

            DWORD gear = 0;
            if(gssGtaVer == GTA_III)
            {
                gear = *(BYTE*)((BYTE*)pCVehicle + 0x204);
            }
            if(gssGtaVer == GTA_VC)
            {
                gear = *(BYTE*)((BYTE*)pCVehicle + 0x208);
            }
            ss << " G:" << gear;
            //FLOAT speed;
            //FLOAT pedal;
            //speed = *(FLOAT*)((BYTE*)pCVehicle + 0x514);
            //pedal = *(FLOAT*)((BYTE*)pCVehicle + 0x594);
            //ss << "(" << speed << ")(" << pedal << ")";
            FLOAT rotX;
            FLOAT rotY;
            FLOAT rotZ;
            rotX = *(FLOAT*)((BYTE*)pCVehicle + 0x14);
            rotY = *(FLOAT*)((BYTE*)pCVehicle + 0x18);
            rotZ = *(FLOAT*)((BYTE*)pCVehicle + 0x1C);
            ss << " R(" << rotX << ")(" << rotY << ")(" << rotZ << ")";

            playerState.bIsInCar = TRUE;
            SN::VEHICLE::GET_CAR_COORDINATES(playerState.vehiclePlayer, &playerState.fvPosVehicle);
            SN::VEHICLE::GET_CAR_HEADING(playerState.vehiclePlayer, &playerState.fHeadingVehicle);
            //ss << ", pos(" << playerState.fvPosVehicle.X << ", " << playerState.fvPosVehicle.Y << ", " << playerState.fvPosVehicle.Z << "; " << playerState.fHeadingVehicle << ")";
        }

        ss << ";";

        parkSysStateMachine(ss, playerState);
        //zoneStateMachine(ss, playerState);
    }
    //
    GtaDrawString(ss.str(), gtaFont);
}




void gssInit(void)
{
    ScriptEventListener sel;
    sel.pfnInit = &gssReset;
    sel.pfnProcess = &gssProcess;
    ////
    MODULEINFO mi = {0, };
    //memset(&mi, 0, sizeof(mi));
    DWORD dwSize = 0;
    BYTE* pBase = nullptr;
    if(::GetModuleInformation(::GetCurrentProcess(), ::GetModuleHandle(NULL), &mi, sizeof(mi)))
    {
        dwSize = mi.SizeOfImage;
        pBase = (BYTE*)mi.lpBaseOfDll;
    }
    lss << UL::WARN << L("base: ") << ulhex(pBase) << L(", size: ") << ulhex(dwSize) << UL::ENDL;
    ////
    DWORD GtaVer;
    GtaVer = GtaScan(&sel, pBase, dwSize);
    if(GtaVer != GTA_UNKNOWN)
    {
        randomSeed();
        gssGtaVer = GtaVer;
//        stc3Init(GtaVer);
        GtaScriptInit(GtaVer);
        //GtaVehicleInit(GtaVer);//moved to parkSys
        parkSysInit(GtaVer);
        if(GtaVer == GTA_III)
        {
            gtaFont = 0;
        }
        if(GtaVer == GTA_VC)
        {
            gtaFont = 1;
        }
    }
}


//ULONG WINAPI ScanThreadProc(LPVOID lpParameter)
//{
//    // ::Sleep(2000);
//    gssInit();
//    return 0;
//}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH: 
        {
            lss.init(L("gss"), UL_FLAGS::SINGLE_FILE_APPEND | UL_FLAGS::PRINT_HEADER | UL_FLAGS::PRINT_TIME | UL_FLAGS::ADD_ENDL);
            lss << UL::INFO << L("Loaded.") << UL::ENDL;
            //ULONG tid;
            //::CreateThread(NULL, 0, &ScanThreadProc, NULL, 0, &tid);
            gssInit();
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            lss << UL::INFO << L("Unloaded.") << UL::ENDL;
            break;
        }
    }
    return TRUE;
}


