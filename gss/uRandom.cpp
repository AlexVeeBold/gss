///////////////////////////////////////////////////////////////
//
// uRandom: [micro] random number generator
//
// Copyright © 2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   11.07.2016 16:40 - created
//

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "uDef.h"
#include "tunic.h"
#include "uLog.h"


DWORD rngValue = 7;


void randomSeed(void)
{
    SYSTEMTIME st;
    ::GetSystemTime(&st);     // UTC
    DWORD sum;
    // 00..23 + 00..59 + 00..59 + 000..999 = 000..1140
    sum = (DWORD)st.wHour + (DWORD)st.wMinute + (DWORD)st.wSecond + (DWORD)st.wMilliseconds;
    lss << UL::DEBUG << L("rSeed: ") << sum << UL::ENDL;
    rngValue = sum;    
}

DWORD randomValue(void)
{
    // Lehmer random number generator a.k.a. Park�Miller random number generator
    rngValue = (DWORD)( ((QWORD)rngValue * 48271) % 2147483647 );
    return rngValue;
}

DWORD randomValueUpTo(DWORD boundary)
{
    return randomValue() % boundary;
}
