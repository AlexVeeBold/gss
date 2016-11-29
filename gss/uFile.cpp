///////////////////////////////////////////////////////////////
//
// uFile: [micro] File routines
//
// Copyright © 2013-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   04.01.2013 06:49 - created (copied from uCrt)
//   25.08.2014 18:32 - memory stuff moved out
//

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "tunic.h"
#include "uDef.h"
#include "uMemory.h"


EXTERN_C_BEGIN



void* fileLoad(CTSTR tszFileName, DWORD* pdwFileSize)
{
    HANDLE hFile;
    void* pFileData;
    DWORD dwSize;
    ULONG dwtmp;

    // open file
    hFile = ::CreateFile(tszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    // check length
    dwSize = ::GetFileSize(hFile, NULL);

    // read file
    pFileData = memAlloc(dwSize + 1);
    if(pFileData != NULL)
    {
        ::ReadFile(hFile, pFileData, dwSize, &dwtmp, NULL);
        if(pdwFileSize != NULL)
        {
            *pdwFileSize = dwSize;
        }
    }
    ::CloseHandle(hFile);

    return pFileData;
}

void fileUnload(void* pFileData)
{
    memFree(pFileData);
}

BOOL fileStore(void* pFileData, CTSTR tszFileName, DWORD dwFileSize)
{
    HANDLE hFile;
    ULONG dwtmp;

    //open file
    hFile = ::CreateFile(tszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    // write file
    ::WriteFile(hFile, pFileData, dwFileSize, &dwtmp, NULL);
    ::CloseHandle(hFile);

    return TRUE;
}



EXTERN_C_END
