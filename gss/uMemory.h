///////////////////////////////////////////////////////////////
//
// uMemory: [micro] Memory routines
//
// Copyright © 2014-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   24.08.2014 19:35 - created
//


EXTERN_C_BEGIN


void* memAlloc(DWORD dwSize);
void* memReAlloc(void* pMem, DWORD dwSize);
BOOL memFree(void* pMem);


BYTE* GetNextMemoryRegion(void);

BOOL IsNextMemoryRegionReadable(BYTE** ppNextMem);
BOOL IsNextMemoryRegionWritable(BYTE** ppNextMem);

BOOL IsReadableMemory(BYTE* pMem);
BOOL IsWritableMemory(BYTE* pMem);

DWORD SetMemoryAllAccess(BYTE* pMem, DWORD dwSize);
void RestoreMemoryAccess(BYTE* pMem, DWORD dwSize, DWORD accessMode);


EXTERN_C_END

