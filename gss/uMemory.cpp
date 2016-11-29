///////////////////////////////////////////////////////////////
//
// uMemory: [micro] Memory routines
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   24.08.2014 19:32 - created (moved from gssMain)
//   25.08.2014 18:33 - uMemFile stuff moved in
//   22.11.2016 20:40 - updated to conform ulogss changes
//

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>


#include "tunic.h"
#include "uLog.h"
#include "uDef.h"


EXTERN_C_BEGIN

//#define UMEMDBG



HANDLE g_hHeap = NULL;

void* memAlloc(DWORD dwSize)
{
    if(g_hHeap != NULL)
    {
        return ::HeapAlloc(g_hHeap, HEAP_ZERO_MEMORY, dwSize);
    }
    else
    {
        g_hHeap = GetProcessHeap();
        return ::HeapAlloc(g_hHeap, HEAP_ZERO_MEMORY, dwSize);
    }
}

void* memReAlloc(void* pMem, DWORD dwSize)
{
    if(g_hHeap != NULL)
    {
        return ::HeapReAlloc(g_hHeap, HEAP_ZERO_MEMORY, pMem, dwSize);
    }
    else
    {
        g_hHeap = GetProcessHeap();
        return ::HeapReAlloc(g_hHeap, HEAP_ZERO_MEMORY, pMem, dwSize);
    }
}

BOOL memFree(void* pMem)
{
    if(g_hHeap != NULL)
    {
        return ::HeapFree(g_hHeap, 0, pMem);
    }
    else
    {
        g_hHeap = GetProcessHeap();
        return ::HeapFree(g_hHeap, 0, pMem);
    }
}



#ifdef UMEMDBG
extern "C++" LString WinProtectToLString(DWORD protect)
{
    LString str("");
    switch(protect)
    {
    case PAGE_EXECUTE:
        str = L("X");
        break;
    case PAGE_EXECUTE_READ:
        str = L("RX");
        break;
    case PAGE_EXECUTE_READWRITE:
        str = L("RWX");
        break;
    case PAGE_EXECUTE_WRITECOPY:
        str = L("RcX");
        break;
    case PAGE_NOACCESS:
        str = L("N/A");
        break;
    case PAGE_READONLY:
        str = L("R");
        break;
    case PAGE_READWRITE:
        str = L("RW");
        break;
    case PAGE_WRITECOPY:
        str = L("Rc");
        break;
    }
    return str;
}
#endif //UMEMDBG

struct MemRegion {
    BYTE* pStart;       // first byte of region
    BYTE* pStop;        // first byte of other region
    BOOL bReadable;
    BOOL bWritable;
};

MemRegion mr = {NULL, NULL, FALSE};

void WinProtectToBool(DWORD protect, MemRegion* pmr)
{
    pmr->bReadable = FALSE;
    switch(protect)
    {
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
    case PAGE_READONLY:
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
        pmr->bReadable = TRUE;
        break;
    }
    pmr->bWritable = FALSE;
    switch(protect)
    {
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
        pmr->bWritable = TRUE;
        break;
    }
}

void QueryMemory(BYTE* pMem)
{
    ::_MEMORY_BASIC_INFORMATION mbi;
    if(::VirtualQuery(pMem, &mbi, sizeof(mbi)) != 0)
    {
#ifdef UMEMDBG
        //ulogf(UL_DEBUG, T("%04X:%08X:%d:%s"), mbi.BaseAddress, mbi.RegionSize, mbi.Protect, WinProtectToStr(mbi.Protect));
        lss << UL::DEBUG << ulhex(mbi.BaseAddress) << L(":") << ulhex(reinterpret_cast<VOIDPTR>(mbi.RegionSize)) << L(":") << mbi.Protect  << L(":") << WinProtectToLString(mbi.Protect) << UL::ENDL;
#endif //UMEMDBG
        mr.pStart = (BYTE*)mbi.BaseAddress;
        mr.pStop = mr.pStart + mbi.RegionSize;
        WinProtectToBool(mbi.Protect, &mr);
    }
}

BYTE* GetNextMemoryRegion(void)
{
    return mr.pStop;
}

BOOL IsNextMemoryRegionReadable(BYTE** ppNextMem)
{
    *ppNextMem = mr.pStop;
    QueryMemory(mr.pStop);
    return mr.bReadable;
}

BOOL IsNextMemoryRegionWritable(BYTE** ppNextMem)
{
    *ppNextMem = mr.pStop;
    QueryMemory(mr.pStop);
    return mr.bWritable;
}

BOOL IsReadableMemory(BYTE* pMem)
{
    BOOL bQuery;
    bQuery = (pMem < mr.pStart) | (pMem >= mr.pStop);
    if(bQuery != FALSE) 
    {
        QueryMemory(pMem);
    }
///    ulogf(UL_INFO, T("IsReadable: %08X = %d"), pMem, mr.bReadable);
    return mr.bReadable;
}

BOOL IsWritableMemory(BYTE* pMem)
{
    BOOL bQuery;
    bQuery = (pMem < mr.pStart) | (pMem >= mr.pStop);
    if(bQuery != FALSE) 
    {
        QueryMemory(pMem);
    }
///    ulogf(UL_INFO, T("IsWritable: %08X = %d"), pMem, mr.bWritable);
    return mr.bWritable;
}

DWORD SetMemoryAllAccess(BYTE* pMem, DWORD dwSize)
{
    DWORD dwProtOld;
    ::VirtualProtect(pMem, dwSize, PAGE_EXECUTE_READWRITE, (ULONG*)&dwProtOld);
    return dwProtOld;
}

void RestoreMemoryAccess(BYTE* pMem, DWORD dwSize, DWORD accessMode)
{
    DWORD dwProtOld;
    ::VirtualProtect(pMem, dwSize, accessMode, (ULONG*)&dwProtOld);
}



EXTERN_C_END
