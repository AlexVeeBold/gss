///////////////////////////////////////////////////////////////
//
// uFile: [micro] File routines
//
// Copyright © 2013,2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   04.01.2013 06:49 - created (copied from uCrt)
//   20.11.2016 03:31 - changed void* to BYTE*
//

// using tunic.h

#ifndef _MICROMEMFILE_H_INC_
#define _MICROMEMFILE_H_INC_



// file functions
EXTERN_C BYTE* fileLoad(CTSTR tszFileName, DWORD* pdwFileSize);
EXTERN_C void fileUnload(BYTE* pbyFile);
EXTERN_C BOOL fileStore(void* pbyFile, CTSTR tszFileName, DWORD dwFileSize);



#endif //_MICROMEMFILE_H_INC_
