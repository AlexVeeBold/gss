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
//   27.02.2017 12:22 - replaced os-specific code with file streams
//

#include <cstdint>
#include <fstream>

#include "uString.h"



void* fileLoad(const WString& fileName, std::uint32_t* pFileSize)
{
    void* pFileData;

    // open file
    std::ifstream file(fileName, std::ios::binary);
    if(file.is_open() == false)
    {
        return nullptr;
    }

    // check length
    file.seekg(0, std::ios_base::end);
    std::intmax_t fileSize = file.tellg();
    file.seekg(0, std::ios_base::beg);
    if((fileSize > std::numeric_limits<std::size_t>::max()) ||
        (fileSize > std::numeric_limits<std::uint32_t>::max()))
    {
        return nullptr;
    }
    std::size_t allocSize = static_cast<std::size_t>(fileSize);
    std::uint32_t readSize = static_cast<std::uint32_t>(fileSize);

    // read file
    pFileData = malloc(allocSize);
    if(pFileData != NULL)
    {
        file.read(reinterpret_cast<char*>(pFileData), fileSize);
        if(pFileSize != NULL)
        {
            *pFileSize = readSize;
        }
    }
    file.close();

    return pFileData;
}

void fileUnload(void* pFileData)
{
    free(pFileData);
}

bool fileStore(void* pFileData, const WString& fileName, std::uint32_t fileSize)
{
    // open file
    std::ofstream file(fileName, std::ios::binary);
    if(file.is_open() == false)
    {
        return false;
    }

    // write file
    file.write(reinterpret_cast<char*>(pFileData), fileSize);
    file.close();

    return true;
}


