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
//   03.12.2016 19:42 - moved stc3 into IScriptThread class tree
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




#define ARG_PTR_AFTER(x)        ((BYTE*)&(x) + sizeof(x))   //!! 32-bit stack machine code (va_start) !!//

union ScriptArgPtr {
    DWORD dword;
    DWORD ptr;
    DWORD* pdw;
    FLOAT* pflt;
    BOOL* pbool;
    BYTE* pby;
    CASTR casz;
};





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
    G30_SCRIPT_VALUE_TYPE_LVAR_INDEX = 0x03,
    G30_SCRIPT_STR8_SIZE = 8,
    // HARDCODED // DO NOT CHANGE //
};

struct GScriptThreadContextIII {
    GScriptThreadContextIII* pNext;
    GScriptThreadContextIII* pPrev;
    BYTE szName[G30_SCRIPT_NAME_SIZE];
    DWORD scriptIP;
    DWORD scriptStack[G30_SCRIPT_STACK_SIZE];
    WORD scriptSP;
    WORD zz002E;
    G30ScriptLVar scriptLocalVar[G30_SCRIPT_NUM_LVARS_TOTAL];
    BOOLBYTE condResult;
    BOOLBYTE bIsMissionThread;
    BOOLBYTE bWaitMessage;
    BOOLBYTE zz007B;
    DWORD wakeTime;
    WORD condOp;
    BOOLBYTE condNotFlag;
    BOOLBYTE bWastedBustedCheck;
    BOOLBYTE bWastedBustedFlag;
    BOOLBYTE bIsLoadedMissionThread;
    WORD zz0086;
};

struct GScriptThreadContextVC {
    GScriptThreadContextVC* pNext;
    GScriptThreadContextVC* pPrev;
    BYTE szName[G30_SCRIPT_NAME_SIZE];
    DWORD scriptIP;
    DWORD scriptStack[G30_SCRIPT_STACK_SIZE];
    WORD scriptSP;
    WORD zz002E;
    G30ScriptLVar scriptLocalVar[G30_SCRIPT_NUM_LVARS_TOTAL];
    BOOLBYTE bIsActive;
    BOOLBYTE condResult;
    BOOLBYTE bIsMissionThread;
    BOOLBYTE bWaitMessage;
    DWORD wakeTime;
    WORD condOp;
    BOOLBYTE condNotFlag;
    BOOLBYTE bWastedBustedCheck;
    BOOLBYTE bWastedBustedFlag;
    BOOLBYTE bIsLoadedMissionThread;
    WORD zz0086;
};

union GScriptThreadContext {
    GScriptThreadContextIII iii;
    GScriptThreadContextVC vc;
};


#include "idecl.h"

// GTA III Script Model
class IScriptThread30 : public IScriptThread {
public:
    IENTRY DWORD ICALL getLVar(DWORD index) IPURE;
    IENTRY void ICALL setLVar(DWORD index, DWORD value) IPURE;
    IENTRY BOOL ICALL getCmdResult(void) IPURE;
    IENTRY void ICALL pushOpCode(WORD code) IPURE;
    IENTRY void ICALL pushLVarIndex(WORD index) IPURE;
    IENTRY void ICALL pushStr8(CASTR str) IPURE;
    IENTRY void ICALL execute(void) IPURE;
};

// GTA IV Script Model
class IScriptThread40 : public IScriptThread {
public:
    // IScriptThread::invoke() only
};



#include "iimpl.h"

class Script30 : public IScriptThread30 {
protected:
    GScriptThreadContext m_context;
    BYTE m_cmdBuffer[128];
    DWORD m_cmdOffsetIP;
    DWORD m_cmdPos;
    SNCOMMAND* m_pCmd;
    DWORD* m_pArgs;
#ifdef STC_DEBUG
    void debugLVars(void);
    void debugCmdBuf(void);
#endif //STC_DEBUG
    void addToCmdBuffer(DWORD prefix, size_t prefixSize, const void* dataPtr, size_t dataSize);
    void cmdStart(SNCOMMAND* pcmd, DWORD* pArg);
    BOOL cmdFinish(void);
public:
    IENTRY void ICALL setOffsetIP(DWORD offset) IPURE;
    IENTRY BOOL ICALLVA invoke(DWORD commandIndex, ...) IPURE;
};


class ScriptIII : public Script30 {
public:
    IENTRY DWORD ICALL getLVar(DWORD index) IPURE;
    IENTRY void ICALL setLVar(DWORD index, DWORD value) IPURE;
    IENTRY BOOL ICALL getCmdResult(void) IPURE;
    IENTRY void ICALL pushOpCode(WORD code) IPURE;
    IENTRY void ICALL pushLVarIndex(WORD index) IPURE;
    IENTRY void ICALL pushStr8(CASTR str) IPURE;
    IENTRY void ICALL execute(void) IPURE;
};

class ScriptVC : public ScriptIII {
public:
    IENTRY DWORD ICALL getLVar(DWORD index) IPURE;
    IENTRY void ICALL setLVar(DWORD index, DWORD value) IPURE;
    IENTRY BOOL ICALL getCmdResult(void) IPURE;
    //IENTRY void ICALL pushOpCode(WORD code) IPURE;
    //IENTRY void ICALL pushLVarIndex(WORD index) IPURE;
    //IENTRY void ICALL pushStr8(CASTR str) IPURE;
    IENTRY void ICALL execute(void) IPURE;
};



//#define STC_DEBUG


ScriptIII scIII;
ScriptVC scVC;

IScriptThread* g_pIScript = nullptr;

void GtaScriptInit(DWORD GtaVersion)
{
    switch(GtaVersion)
    {
    case GTA_III:
        g_pIScript = &scIII;
        break;
    case GTA_VC:
        g_pIScript = &scVC;
        break;
    }
}




#ifdef STC_DEBUG
void Script30::debugCmdBuf(void)
{
    lss << UL::DEBUG << L("CMDBUF: ");
    for(DWORD iByte = 0; iByte < m_cmdPos; iByte++)
    {
        lss << L(" ") << ulhex(m_cmdBuffer[iByte]);
    }
    lss << UL::ENDL;
}

void Script30::debugLVars(void)
{
    lss << UL::DEBUG << L("LVAR:");
    for(DWORD iVar = 0; iVar < G30_SCRIPT_NUM_LV_VARS; iVar += 4)
    {
        lss << L("  ") << ulhex(this->getLVar(iVar + 0)) << L(" ") << ulhex(this->getLVar(iVar + 1)) << L(" ") << ulhex(this->getLVar(iVar + 2)) << L(" ") << ulhex(this->getLVar(iVar + 3));
    }
    lss << UL::ENDL;
    lss << UL::DEBUG << L("CondRes: ") << this->getCmdResult() << UL::ENDL;
}
#endif //STC_DEBUG


void Script30::setOffsetIP(DWORD offset)
{
    // executing cmd buffer with ip hack:
    //
    // gss:   ip = gssCmdBuffer - scriptDataBuffer
    //
    // gta: byte = scriptDataBuffer[ip]
    //           = *(ip + scriptDataBuffer)
    //           = *((gssCmdBuffer - scriptDataBuffer) + scriptDataBuffer)
    //           = *(gssCmdBuffer)
    //
    if(offset < (sizeof(m_cmdBuffer) / 2))
    {
        m_cmdOffsetIP = offset;
    }
}

void Script30::addToCmdBuffer(DWORD prefix, size_t prefixSize, const void* dataPtr, size_t dataSize)
{
    if((m_cmdPos + prefixSize + dataSize) < sizeof(m_cmdBuffer))
    {
        if(prefixSize > 0)
        {
            memcpy(&m_cmdBuffer[m_cmdPos], &prefix, prefixSize);
            m_cmdPos += prefixSize;
        }
        if(dataSize > 0)
        {
            memcpy(&m_cmdBuffer[m_cmdPos], dataPtr, dataSize);
            m_cmdPos += dataSize;
        }
    }
}

void Script30::cmdStart(SNCOMMAND* pcmd, DWORD* pArg)
{
    m_pCmd = pcmd;
    m_pArgs = pArg;

    m_cmdPos = m_cmdOffsetIP;

    // set opcode
    this->pushOpCode(m_pCmd->code);

    // load all args to lvars
    for(DWORD iArg = 0; iArg < m_pCmd->numArgs; iArg++)
    {
        ScriptArgPtr argPtr;
        argPtr.dword = m_pArgs[iArg];
        DWORD cmdArgType = pcmd->argType[iArg];

        if((cmdArgType & SAA_CMD_LVAR) != 0)
        {
            if((cmdArgType & SAA_VALUE_READ) != 0)
            {
                if((cmdArgType & SAA_VALUE_CLEAR) == 0)
                {
                    this->setLVar(iArg, argPtr.dword);  // int or float
                }
                else
                {
                    this->setLVar(iArg, *argPtr.pdw);   // pointer to value to be cleared
                }
            }

            this->pushLVarIndex(iArg);
        }

        if((cmdArgType & SAA_CMD_STR8) != 0)
        {
            this->pushStr8(argPtr.casz);
        }

#ifdef STC_DEBUG
        this->debugCmdBuf();
#endif //STC_DEBUG
    }
#ifdef STC_DEBUG
    this->debugLVars();
#endif //STC_DEBUG
}

BOOL Script30::cmdFinish(void)
{
    ScriptArgPtr argPtr;
    DWORD numArgs = m_pCmd->numArgs;

    // store lvars back to args
    for(DWORD iArg = 0; iArg < numArgs; iArg++)
    {
        argPtr.ptr = m_pArgs[iArg];
        DWORD cmdArgType = m_pCmd->argType[iArg];

        if((cmdArgType & SAA_VALUE_WRITE) != 0)
        {
            *argPtr.pdw = this->getLVar(iArg);
        }

        if((cmdArgType & SAA_VALUE_CLEAR) != 0)
        {
            *argPtr.pdw = 0;
        }
    }
#ifdef STC_DEBUG
    this->debugLVars();
#endif //STC_DEBUG

    m_cmdOffsetIP = 0;

    return this->getCmdResult();
}

BOOL Script30::invoke(DWORD commandIndex, ...)
{
    DWORD* pArg;
    SNCOMMAND* pcmd;

    // setup pointers
    pArg = (DWORD*) ARG_PTR_AFTER(commandIndex);
    pcmd = &snCmds[commandIndex];

#ifdef STC_DEBUG
    lss << UL::INFO << L("== III == invoke ==") << UL::ENDL;
#endif //STC_DEBUG

    this->cmdStart(pcmd, pArg);

    this->execute();

    return this->cmdFinish();
}



DWORD ScriptIII::getLVar(DWORD index)
{
    return m_context.iii.scriptLocalVar[index].dword;
}

DWORD ScriptVC::getLVar(DWORD index)
{
    return m_context.vc.scriptLocalVar[index].dword;
}



void ScriptIII::setLVar(DWORD index, DWORD value)
{
    m_context.iii.scriptLocalVar[index].dword = value;
}

void ScriptVC::setLVar(DWORD index, DWORD value)
{
    m_context.vc.scriptLocalVar[index].dword = value;
}



BOOL ScriptIII::getCmdResult(void)
{
    return m_context.iii.condResult;
}

BOOL ScriptVC::getCmdResult(void)
{
    return m_context.vc.condResult;
}



void ScriptIII::execute(void)
{
    // exec cmd buffer with ip hack
    m_context.iii.scriptIP = &m_cmdBuffer[m_cmdOffsetIP] - GtaGetScriptDataBuffer();     // apply one-time ip offset

#ifdef STC_DEBUG
    lss << UL::INFO << L("== III == executing buffer ") << ulhex(m_cmdBuffer) << L(" as ip ") << ulhex(m_context.iii.scriptIP) << L(" ==") << UL::ENDL;
#endif //STC_DEBUG

    GtaExecScriptCommand(&m_context.iii);
}

void ScriptVC::execute(void)
{
    // exec cmd buffer with ip hack
    m_context.vc.scriptIP = &m_cmdBuffer[m_cmdOffsetIP] - GtaGetScriptDataBuffer();     // apply one-time ip offset

#ifdef STC_DEBUG
    lss << UL::INFO << L("== VC == executing buffer ") << ulhex(m_cmdBuffer) << L(" as ip ") << ulhex(m_context.vc.scriptIP) << L(" ==") << UL::ENDL;
#endif //STC_DEBUG

    GtaExecScriptCommand(&m_context.vc);
}



void ScriptIII::pushOpCode(WORD code)
{
    this->addToCmdBuffer(code, sizeof(WORD), nullptr, 0);
}

// VC: reuse III


void ScriptIII::pushLVarIndex(WORD index)
{
    this->addToCmdBuffer(G30_SCRIPT_VALUE_TYPE_LVAR_INDEX, sizeof(BYTE), &index, sizeof(WORD));
}

// VC: reuse III


void ScriptIII::pushStr8(CASTR str)
{
    // [III][VC] String8 values are passed without type marker
    this->addToCmdBuffer(0, 0, str, G30_SCRIPT_STR8_SIZE);
}

// VC: reuse III


