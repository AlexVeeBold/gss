///////////////////////////////////////////////////////////////
//
// uDef: [micro] run-time library defines
//
// Copyright © 2011-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   06.02.2011 19:35 - created
//   22.12.2012 17:09 - added INTPTR, UINTPTR
//   04.01.2013 11:01 - fixed DWORD and INT win32 compatibility issues
//   18.04.2013 11:09 - added ULONG, tweaked DWORD and INT (win32 compatibility issues again)
//   04.04.2014 17:41 - added SBYTE, moved CHAR to synonyms
//   24.08.2014 04:03 - added BOOLBYTE to synonyms
//   25.08.2014 18:36 - EXTERN_C moved in
//   26.08.2014 23:39 - DWBYTE arrived
//   02.10.2014 03:13 - ROUNDUP added
//   22.11.2016 15:58 - moved public types to typedefs (discarded later)
//

// You can #define UNICODE before including this header

#ifndef _MICRODEF_H_INC_
#define _MICRODEF_H_INC_


//// simple types ////

// INTERNAL types (DO NOT USE directly)
#define _SI_008     signed char
#define _SI_016     signed short
#define _SI_032     signed int // win32 compatibility ?
#define _SL_032     signed long // win32 compatibility ?
#define _SI_064     signed __int64      //ms-specific
#define _UI_008     unsigned char
#define _UI_016     unsigned short
#define _UI_032     unsigned int // win32 compatibility
#define _UL_032     unsigned long // win32 compatibility
#define _UI_064     unsigned __int64    //ms-specific
#define _FP_032     float
#define _FP_064     double

// public types
#define BYTE        _UI_008
#define WORD        _UI_016
#define DWORD       _UI_032
#define QWORD       _UI_064
#define SBYTE       _SI_008
#define SHORT       _SI_016
#define INT         _SI_032
#define LONGINT     _SI_064
#define ULONG       _UL_032
#define LONG        _SL_032
#define FLOAT       _FP_032
#define LONGFLOAT   _FP_064
#define BOOL        _UI_032
//typedef _UI_008 BYTE;
//typedef _UI_016 WORD;
//typedef _UI_032 DWORD;
//typedef _UI_064 QWORD;
//typedef _SI_008 SBYTE;
//typedef _SI_016 SHORT;
//typedef _SI_032 INT;
//typedef _SI_064 LONGINT;
//typedef _UL_032 ULONG;
//typedef _SL_032 LONG;
//typedef _FP_032 FLOAT;
//typedef _FP_064 LONGFLOAT;
//typedef _UI_032 BOOL;
// synonyms
#define CHAR        SBYTE
#define BOOLBYTE    BYTE
#define SINGLE      FLOAT
#define DOUBLE      LONGFLOAT
#define VOID        void
#define VOIDPTR     void*
#define CONST       const

// pointer to integer mapping
//#if sizeof(DWORD) == sizeof(void*)            // yeah... dreaming too much...
#define INTPTR      INT
#define UINTPTR     DWORD
//#elif sizeof
//#else
//#error check me
//#endif

// null
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif //__cplusplus

// boolean
#define FALSE   0
#define TRUE    1
// check boolean conditions
#if ((FALSE == FALSE) != TRUE)
#error Boolean is not supported.
#endif


//// union types ////

struct DWBYTE {
    union {
        INT sint;
        DWORD dword;
        BYTE byte[4];
        BYTE* pby;
    };
};


//// miscellaneous ////

#ifdef __cplusplus
#define EXTERN_C            extern "C"          // simple definition
#define EXTERN_C_BEGIN      extern "C" {        // block definition
#define EXTERN_C_END        }
#else //__cplusplus
#define EXTERN_C            
#define EXTERN_C_BEGIN      
#define EXTERN_C_END        
#endif //__cplusplus


// x: unsigned integer, align: power of 2 (2,4,8...)
#define ROUNDUP(x,align)    ((x + (align-1)) & (~(align-1)))

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)    (sizeof(a) / sizeof(a[0]))
#endif

#define OFFSETOF(type,member)   &(((type*)0)->member)



#endif //_MICRODEF_H_INC_
