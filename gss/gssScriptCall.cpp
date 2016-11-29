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
//   18.11.2016 01:00 - cleaned up scriptCallData stuff
//


#include "memory.h"


#include "uDef.h"
#include "tunic.h"
#include "tstr.h"
#include "uLog.h"


#include "gssDef.h"
#include "gssInterface.h"
#include "gssScriptCall.h"

#include "gtaVersion.h"


enum STCARGACTION {
    SAA_VALUE_READ = 0x01,
    SAA_VALUE_WRITE = 0x02,
    SAA_VALUE_CLEAR = 0x04,
    SAA_CMD_LVAR = 0x10,
    SAA_CMD_STR8 = 0x20,
};
enum STCARGTYPE {
    SAT_IN_INT32 = SAA_VALUE_READ | SAA_CMD_LVAR,
    SAT_IN_FLT32 = SAA_VALUE_READ | SAA_CMD_LVAR,
    SAT_IN_BOOL32 = SAA_VALUE_READ | SAA_CMD_LVAR,
    SAT_IN_STR64 = SAA_CMD_STR8,
    SAT_INCLEAR_INT32 = SAA_VALUE_READ | SAA_CMD_LVAR | SAA_VALUE_CLEAR,
//    SAT_OUT_INT32 = SAA_CMD_LVAR | SAA_VALUE_WRITE,
    SAT_OUT_INT32 = SAA_VALUE_READ | SAA_CMD_LVAR | SAA_VALUE_WRITE, // special for ADD_BLIP_FOR_*
    SAT_OUT_FLT32 = SAA_CMD_LVAR | SAA_VALUE_WRITE,
};



enum ScriptCallArgsConst {
    MAX_SCRIPT_CALL_ARGS = 16,
};

struct SNCOMMAND {
    DWORD code;
    DWORD hash;
    DWORD numArgs;
    BYTE argType[MAX_SCRIPT_CALL_ARGS];
};

#define SNCxFAKExVOID(sname, code, hash, rtype, spoof, nargs)  SNC_ARRAY_ENTRY(sname, code, hash, rtype, nargs)
#define SNCxVOID(sname, code, hash, rtype, nargs) SNC_ARRAY_ENTRY(sname, code, hash, rtype, nargs)
#define SNCxBOOL(sname, code, hash, rtype, nargs) SNC_ARRAY_ENTRY(sname, code, hash, rtype, nargs)

#pragma message("SNC: building command array")

SNCOMMAND snCmds[NUM_SN_COMMANDS] = {

#define SNC_ARRAY_ENTRY(sname, code, hash, rtype, nargs)    {code, hash, SNC_TY_##nargs},
#include "gssScriptCallData.h"
#undef SNC_ARRAY_ENTRY

};

#undef SNCxFAKExVOID
#undef SNCxVOID
#undef SNCxBOOL







union G30ScriptLVar {
    DWORD dword;
    INT sint;
    FLOAT flt;
};

enum G30ScriptConst {
    // HARDCODED // DO NOT CHANGE //
    G30_SCRIPT_NAME_LEN = 7,
    G30_SCRIPT_NAME_SIZE = G30_SCRIPT_NAME_LEN + 1,
    G30_SCRIPT_STACK_SIZE = 6,
    G30_SCRIPT_NUM_LV_VARS = 16,
    G30_SCRIPT_NUM_LV_TIMERS = 2,
    G30_SCRIPT_NUM_LVARS_TOTAL = G30_SCRIPT_NUM_LV_VARS + G30_SCRIPT_NUM_LV_TIMERS,
    // HARDCODED // DO NOT CHANGE //
};

union G3VCCONFLICTBLOCK {
    struct {
        BOOLBYTE condResult;
        BOOLBYTE bIsMissionThread;
        BOOLBYTE bWaitMessage;
        BOOLBYTE z_unused7B;
    } G3;
    struct {
        BOOLBYTE bIsActive;
        BOOLBYTE condResult;
        BOOLBYTE bIsMissionThread;
        BOOLBYTE bWaitMessage;
    } VC;
};

struct G30ScriptThreadContext {
    // HARDCODED // DO NOT CHANGE //
    G30ScriptThreadContext* pNext;
    G30ScriptThreadContext* pPrev;
    BYTE szName[G30_SCRIPT_NAME_SIZE];
    DWORD scriptIP;
    DWORD scriptStack[G30_SCRIPT_STACK_SIZE];
    WORD scriptSP;
    WORD z_unused2E;
    G30ScriptLVar scriptLocalVar[G30_SCRIPT_NUM_LVARS_TOTAL];
    G3VCCONFLICTBLOCK Flags;
    DWORD wakeTime;
    WORD condOp;
    BOOLBYTE condNotFlag;
    BOOLBYTE bWastedBustedCheck;
    BOOLBYTE bWastedBustedFlag;
    BOOLBYTE bIsLoadedMissionThread;
    WORD z_unused86;
    // HARDCODED // DO NOT CHANGE //
};


G30ScriptThreadContext stc3;

BOOLBYTE* pSTC3condResult;


//#define STC_DEBUG

void stc3Init(DWORD gtaVersion)
{
    DWORD i;
    stc3.pNext = NULL;
    stc3.pPrev = NULL;
    aszncpy((ASTR)&stc3.szName[0], "GSS", G30_SCRIPT_NAME_LEN);
    stc3.scriptIP = 0;
    for(i = 0; i < G30_SCRIPT_STACK_SIZE; i++)
    {
        stc3.scriptStack[i] = 0;
    }
    stc3.scriptSP = 0;
    stc3.z_unused2E = 0;
    for(i = 0; i < G30_SCRIPT_NUM_LVARS_TOTAL; i++)
    {
        stc3.scriptLocalVar[i].dword = 0;
    }
    if(gtaVersion == GTA_III)
    {
        stc3.Flags.G3.condResult = FALSE;
        stc3.Flags.G3.bIsMissionThread = FALSE;
        stc3.Flags.G3.bWaitMessage = FALSE;
        stc3.Flags.G3.z_unused7B = FALSE;
        pSTC3condResult = &stc3.Flags.G3.condResult;
    }
    else if(gtaVersion == GTA_VC)
    {
        stc3.Flags.VC.bIsActive = TRUE;
        stc3.Flags.VC.condResult = FALSE;
        stc3.Flags.VC.bIsMissionThread = FALSE;
        stc3.Flags.VC.bWaitMessage = FALSE;
        pSTC3condResult = &stc3.Flags.VC.condResult;
    }
    stc3.wakeTime = 0;
    stc3.condOp = 0;
    stc3.condNotFlag = FALSE;
    stc3.bWastedBustedCheck = TRUE;
    stc3.bWastedBustedFlag = FALSE;
    stc3.bIsLoadedMissionThread = FALSE;
    stc3.z_unused86 = 0;
}

void stc3Exec(void)
{
    BYTE* pfnPOC;
    pfnPOC = g3i.pfnProcessOneCommand;
#ifdef STC_DEBUG
    lss << UL::DEBUG << L("== Exec ") << ulhex(pfnPOC) << L("(") << ulhex(&stc3) << L(") ==") << UL::ENDL;
#endif //STC_DEBUG
    __asm {
        mov ecx, offset stc3;
        call pfnPOC;
        // no stack correction
    }
}

union ScriptArgPtr {
    DWORD ptr;
    DWORD* pdw;
    FLOAT* pflt;
    BOOL* pbool;
    BYTE* pby;
};



DWORD dwCmdOffsetIP = 0;
BYTE byCmdBuffer[128] = {0, }; // 2 + 16*(1+2)

void stc3OffsetIP(DWORD offset)
{
    if(offset < 32)
    {
        dwCmdOffsetIP = offset;
    }
}


#ifdef STC_DEBUG
void stc3debugCmdBuf(void)
{
    lss << UL::DEBUG << L("CMDBUF:");
    for(DWORD iByte = 0; iByte < 24; iByte += 4)
    {
        lss << L("  ") << ulhex(byCmdBuffer[iByte + 0]) << L(" ") << ulhex(byCmdBuffer[iByte + 1]) << L(" ") << ulhex(byCmdBuffer[iByte + 2]) << L(" ") << ulhex(byCmdBuffer[iByte + 3]);
    }
    lss << UL::ENDL;
}

void stc3debugLVars(void)
{
    lss << UL::DEBUG << L("LVAR:");
    for(DWORD iVar = 0; iVar < G30_SCRIPT_NUM_LV_VARS; iVar += 4)
    {
        lss << L("  ") << ulhex(stc3.scriptLocalVar[iVar + 0].dword) << L(" ") << ulhex(stc3.scriptLocalVar[iVar + 1].dword);
        lss << L(" ") << ulhex(stc3.scriptLocalVar[iVar + 2].dword) << L(" ") << ulhex(stc3.scriptLocalVar[iVar + 3].dword);
    }
    lss << UL::ENDL;
    lss << UL::DEBUG << L("CondRes: ") << static_cast<DWORD>(*pSTC3condResult) << UL::ENDL;
}
#endif //STC_DEBUG


#define ARG_PTR_AFTER(x)        ((BYTE*)&(x) + sizeof(x))   //!! 32-bit stack machine code (va_start) !!//

BOOL stc3Call(DWORD dwCommandIndex, ...)
{
    DWORD* pArg;
    DWORD iArg;
    DWORD argType;
    ScriptArgPtr argPtr;
    SNCOMMAND* pcmd;
    DWORD numArgs;
    BYTE* pArgType;
    DWORD iCmdPos;
    DWBYTE db;
    DWORD iByte;

    if(dwCommandIndex > NUM_SN_COMMANDS)
    {
        return FALSE;
    }

    // reset buffers
    memset(&byCmdBuffer[0], 0xCC, sizeof(byCmdBuffer));
    memset(&stc3.scriptLocalVar[0], 0xDD, sizeof(stc3.scriptLocalVar[0]) * G30_SCRIPT_NUM_LV_VARS);

    // setup pointers
    pcmd = &snCmds[dwCommandIndex];
    numArgs = pcmd->numArgs;
    pArgType = &pcmd->argType[0];
    pArg = (DWORD*) ARG_PTR_AFTER(dwCommandIndex);

    // load all args to lvars
    // build cmd buffer
    db.dword = pcmd->code;
    iCmdPos = dwCmdOffsetIP;    // apply one-time ip offset
    byCmdBuffer[iCmdPos+0] = db.byte[0];
    byCmdBuffer[iCmdPos+1] = db.byte[1];
    iCmdPos += 2;
    for(iArg = 0; iArg < numArgs; iArg++)
    {
        argPtr.ptr = pArg[iArg];
        argType = pArgType[iArg];

        if((argType & SAA_VALUE_READ) != 0)
        {
            if((argType & SAA_VALUE_CLEAR) == 0)
            {
                stc3.scriptLocalVar[iArg].dword = argPtr.ptr;
            }
            else    // "clear" arrives as pointer
            {
                stc3.scriptLocalVar[iArg].dword = *argPtr.pdw;
            }
        }

        if((argType & SAA_CMD_LVAR) != 0)
        {
            db.dword = iArg;
            byCmdBuffer[iCmdPos] = 0x03;    // value is LVar index (uint16)
            byCmdBuffer[iCmdPos+1] = db.byte[0];
            byCmdBuffer[iCmdPos+2] = db.byte[1];
            iCmdPos += 3;
        }

        if((argType & SAA_CMD_STR8) != 0)
        {
            // [GTA III] String8 values are passed without type marker
            for(iByte = 0; iByte < 8; iByte++)
            {
                byCmdBuffer[iCmdPos] = argPtr.pby[iByte];
                iCmdPos++;
            }
        }

#ifdef STC_DEBUG
        stc3debugCmdBuf();
#endif //STC_DEBUG
    }

    // exec cmd buffer with ip hack
    // gss:  ip = cmdBuffer - scriptDataBuffer
    // g3: byte = scriptDataBuffer[ip]
    //          = *(ip + scriptDataBuffer)
    //          = *((byCmdBuffer - scriptDataBuffer) + scriptDataBuffer)
    //          = *(byCmdBuffer)
    stc3.scriptIP = &byCmdBuffer[dwCmdOffsetIP] - g3i.pbyScriptDataBuffer;  // apply one-time ip offset

#ifdef STC_DEBUG
    stc3debugLVars();
    lss << UL::INFO << L("== executing buffer ") << ulhex(byCmdBuffer) << L(" as ip ") << ulhex(stc3.scriptIP) << L(" ==") << UL::ENDL;
#endif //STC_DEBUG

    stc3Exec();

#ifdef STC_DEBUG
    stc3debugLVars();
#endif //STC_DEBUG

    // store lvars back to args
    for(iArg = 0; iArg < numArgs; iArg++)
    {
        argPtr.ptr = pArg[iArg];
        argType = pArgType[iArg];

        if((argType & SAA_VALUE_WRITE) != 0)
        {
            *argPtr.pdw = stc3.scriptLocalVar[iArg].dword;
        }

        if((argType & SAA_VALUE_CLEAR) != 0)
        {
            *argPtr.pdw = 0;
        }
    }

    dwCmdOffsetIP = 0;

    return (BOOL) *pSTC3condResult;
}

