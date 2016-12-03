///////////////////////////////////////////////////////////////
//
// gssInterface: Interface to internal stuff
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   13.09.2014 03:02 - created (moved out from gssMain)
//   23.11.2014 18:27 - moving signature data into one array (while adding VC signatures)
//   25.11.2014 16:20 - added more VC signatures
//   29.11.2016 22:27 - moved signatures & connector to gtaScan
//   02.12.2016 00:26 - added GtaExecScriptCommand
//   03.12.2016 23:01 - added GtaGetScriptDataBuffer & GtaGetGameTime
//


#include "uDef.h"
#include "tunic.h"
#include "tstr.h"
#include "uLog.h"


#include "gssDef.h"
#include "gssSign.h"
#include "gssInterface.h"

#include "gtaVersion.h"

#include "gtaVehicle.h"


G30Interface g3i;


//////// //////// //////// //////// access to internal stuff //////// //////// //////// //////// 

struct G30TextInfo {
    FLOAT scaleWidth;
    FLOAT scaleHeight;
    ByteVector4 textColor;
    BOOLBYTE bJustified;
    BOOLBYTE bCentered;
    BOOLBYTE bBackground;
    BOOLBYTE bBackgroundOnlyText;
    FLOAT horizWrapSize;
    FLOAT centerWrapSize;
    ByteVector4 backColor;
    BOOLBYTE bProportional;
    BOOLBYTE bDrawBeforeFade;
    BOOLBYTE bRightAligned;
    BYTE z_unused1F;
    DWORD font;
    FLOAT posX;
    FLOAT posY;
    WORD textToDisplay[500];
};

void GtaDrawText(const BYTE* szText, DWORD font)
{
    G30TextInfo* gti;
    DWORD i;
    BYTE by;
    const BYTE* pszSrc;
    BYTE* pszDst;
    //BYTE* pbyMagic;
//    if(*g3i.pwCurrentTextInfo == 0)
    {
        //pbyMagic = (BYTE*) 0x95CD7C;
        //*pbyMagic = 1;
        gti = (G30TextInfo*) g3i.pTextInfoPool;
        gti->scaleWidth = 0.25;
        gti->scaleHeight = 1.0;
        gti->textColor.dw = 0x80FFFFFF;
        gti->bJustified = FALSE;
        gti->bCentered = FALSE;
        gti->bBackground = FALSE;
        gti->bBackgroundOnlyText = FALSE;
        gti->horizWrapSize = 480.0;
        gti->centerWrapSize = 512.0;
        gti->backColor.dw = 0x80808080;
        gti->bProportional = TRUE;
        gti->bDrawBeforeFade = TRUE;
        gti->bRightAligned = FALSE;
        gti->font = font; //0;
        gti->posX = 20.0;
        gti->posY = 15.0;
        pszSrc = &szText[0];
        pszDst = (BYTE*) &gti->textToDisplay[0];
        for(i = 0; i < 128; i++)
        {
            by = *pszSrc;
            *pszDst = by;
            if(by != '\0')
            {
                pszSrc += 1;
                pszDst += 2;
                continue;
            }
            break;
        }
        *pszDst = '\0';
        *g3i.pwCurrentTextInfo = 1;
    }
}

void GtaDrawString(AString str, DWORD font)
{
    GtaDrawText((BYTE*)str.c_str(), font);
}


void* GtaGetCVehiclePtr(Vehicle vehicle)
{
    BYTE* pfnVPG;
    BYTE* pVehPool;
    BYTE* pVeh;
    pfnVPG = g3i.pfnVehPoolGetAt;
    pVehPool = *g3i.ppVehiclePool;
    __asm {
        mov eax, vehicle;
        mov ecx, pVehPool;
        push eax;
        call pfnVPG;
        mov pVeh, eax;
        // no stack correction
    }
    // pVeh can be null
    return pVeh;
}

void GtaSetNextVehicleVariation(BYTE variation1, BYTE variation2)
{
    *g3i.pbyNextVehVariation1 = variation1;
    *g3i.pbyNextVehVariation2 = variation2;
}



void GtaDrawMarkerSet(DWORD markerIndex, DWORD markerType, FloatVector3* pPos, FLOAT size, FLOAT pulsation, ByteVector4* pRgba, DWORD flags)
{
    DWORD mkSize;
    DWORD mkPulsation;
    DWORD mkRed, mkGreen, mkBlue, mkAlpha;
    BYTE* pfnPMS;
    mkSize = *(DWORD*)&size;
    mkPulsation = *(DWORD*)&pulsation;
    mkRed = (DWORD)pRgba->R; // 0xFF;
    mkGreen = (DWORD)pRgba->G; // 0x7F;
    mkBlue = (DWORD)pRgba->B; // 0x5F;
    mkAlpha = (DWORD)pRgba->A; // 0xFF;
    pfnPMS = g3i.pbyPlaceMarkerSet;
    __asm {
        push 0;
        push mkPulsation;
        push flags;
        push mkAlpha;
        push mkBlue;
        push mkGreen;
        push mkRed;
        push mkSize;
        push pPos;
        push markerType;
        push markerIndex;
        call pfnPMS;
        add esp, 0x2C;  // correct stack ptr
    }
}

void GtaDrawMarker1(DWORD markerIndex, DWORD markerType, FloatVector3* pPos, FLOAT size, FLOAT pulsation, ByteVector4* pRgba, DWORD flags, INT markerMovement)
{
    DWORD mkSize;
    DWORD mkPulsation;
    DWORD mkRed, mkGreen, mkBlue, mkAlpha;
    BYTE* pfnPM1;
    mkSize = *(DWORD*)&size;
    mkPulsation = *(DWORD*)&pulsation;
    mkRed = (DWORD)pRgba->R;
    mkGreen = (DWORD)pRgba->G;
    mkBlue = (DWORD)pRgba->B;
    mkAlpha = (DWORD)pRgba->A;
    pfnPM1 = g3i.pbyPlaceMarker1;
    __asm {
        push markerMovement;    // -1, 1, 2
        push mkPulsation;
        push flags;
        push mkAlpha;
        push mkBlue;
        push mkGreen;
        push mkRed;
        push mkSize;
        push pPos;
        push markerType;
        push markerIndex;
        call pfnPM1;
        add esp, 0x2C;  // correct stack ptr
    }
}


void GtaExecScriptCommand(void* pScriptContext)
{
    BYTE* pfnPOC = g3i.pfnProcessOneCommand;
#ifdef STC_DEBUG
    lss << UL::DEBUG << L("== Exec ") << ulhex(pfnPOC) << L("(") << ulhex(pScriptContext) << L(") ==") << UL::ENDL;
#endif //STC_DEBUG
    __asm {
        mov ecx, pScriptContext;
        call pfnPOC;
        // no stack correction
    }
}

BYTE* GtaGetScriptDataBuffer(void)
{
    return g3i.pbyScriptDataBuffer;
}

DWORD GtaGetGameTime(void)
{
    return *g3i.pdwGameTime;
}






//////// //////// //////// //////// TheScripts signal interceptor //////// //////// //////// //////// 

//#define SC_DEBUG


#ifdef SC_DEBUG
#define SCDBG(x)    x;
#else
#define SCDBG(x)    // nothing
#endif




// State Table
//                  Init()                  Process()
// unknown          -> initializing         
// initializing                             -> initialized
// initialized                              -> working
// working          -> initializing         
//
enum class ScriptState : DWORD {
    WORKING = 0,    // real 'process' was done at least 1 time
    INITIALIZED,    // after-init 'process' call was done
    INITIALIZING,   // 'init' was done
    UNKNOWN,        // entry state
};
ScriptState scState = ScriptState::UNKNOWN;

SCDBG(DWORD nScriptProcessCalls = 0);


ScriptEventListener scEventListener = {NULL, NULL};

void ScriptProcess(void)
{
    switch(scState)
    {
    case ScriptState::WORKING:
    case ScriptState::INITIALIZED:
        scState = ScriptState::WORKING;
        SCDBG(nScriptProcessCalls++);
        SCDBG(lss << UL::INFO << L("tsProcess called ") << nScriptProcessCalls << UL::ENDL);
        scEventListener.pfnProcess();
        break;

    case ScriptState::INITIALIZING:
        scState = ScriptState::INITIALIZED;
        SCDBG(lss << UL::INFO << L("tsProcess [skip]") << UL::ENDL);
        scEventListener.pfnInit();
        break;

    case ScriptState::UNKNOWN:
        // do-nothing
        break;
    }
}

void ScriptInit(void)
{
    switch(scState)
    {
    case ScriptState::WORKING:
    case ScriptState::UNKNOWN:
        scState = ScriptState::INITIALIZING;
        SCDBG(nScriptProcessCalls = 0);
        SCDBG(lss << UL::INFO << L("tsInit called") << UL::ENDL);
        break;

    case ScriptState::INITIALIZING:
    case ScriptState::INITIALIZED:
        // do-nothing
        SCDBG(lss << UL::INFO << L("tsInit [skip]") << UL::ENDL);
        break;
    }
}

void GetGlobalScriptEventListener(ScriptEventListener* pSEL)
{
    pSEL->pfnInit = &ScriptInit;
    pSEL->pfnProcess = &ScriptProcess;
}

void SetScriptEventListener(ScriptEventListener* pSEL)
{
    // store receiver interface
    scEventListener.pfnInit = pSEL->pfnInit;
    scEventListener.pfnProcess = pSEL->pfnProcess;
}


