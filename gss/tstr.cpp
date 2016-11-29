///////////////////////////////////////////////////////////////
//
// String functions (use with Tiny Unicode helper)
//
// Copyright © 2009-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   17.06.2009 10:56 - created
//   26.06.2009 03:30 - double-compile version (ansi/unicode)
//                    all code moved (and guarded) to header
//   26.08.2010 06:07 - copied into vkern project
//   01.02.2014 06:41 - rebound to micro-crt headers (uDef)
//

#include "uDef.h"
#include "tunic.h"


#define _TSTR_CLPASS_

// compile unicode functions
#ifndef UNICODE
#define UNICODE
#endif //UNICODE
#include "tunic.h"
#include "tstrp.h"

// compile ansi functions
#undef UNICODE
#include "tunic.h"
#include "tstrp.h"

