///////////////////////////////////////////////////////////////
//
// uString: [micro] String type
//
// Copyright © 2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   18.11.2016 02:38 - created
//

#ifndef _MICROSTRING_H_INC_
#define _MICROSTRING_H_INC_

#include "tunic.h"

#include <string>
#include <sstream>
#include <iomanip>

typedef std::string AString;
typedef std::wstring WString;
#define AStringFrom std::to_string
#define WStringFrom std::to_wstring

#ifndef UNICODE
typedef std::string UString;
#define UStringFrom std::to_string
#else
typedef std::wstring UString;
#define UStringFrom std::to_wstring
#endif // !UNICODE

typedef std::basic_stringstream<ACHAR> AStringStream;
typedef std::basic_stringstream<WCHAR> WStringStream;
typedef std::basic_stringstream<TCHAR> UStringStream;



#endif //_MICROSTRING_H_INC_
