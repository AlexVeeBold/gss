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
//   27.02.2017 12:51 - moved to std types
//

// using tunic.h

#ifndef _MICROMEMFILE_H_INC_
#define _MICROMEMFILE_H_INC_

#include <cstdint>


// file functions
void* fileLoad(const WString& fileName, std::uint32_t* pFileSize);
void fileUnload(void* pFileData);
bool fileStore(void* pFileData, const WString& fileName, std::uint32_t fileSize);



#endif //_MICROMEMFILE_H_INC_
