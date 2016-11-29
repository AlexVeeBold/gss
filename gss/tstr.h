///////////////////////////////////////////////////////////////
//
// String functions (use with Tiny Unicode helper)
//
// Copyright © 2009-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   17.06.2009 11:02 - created
//   26.06.2009 03:30 - double-compile version (ansi/unicode)
//   09.07.2009 12:19 - code moved out to hack ms-code-hint-bug
//   29.10.2009 19:11 - tszlen split into tszlen/tsznlen
//   30.10.2009 10:52 - tszlwr added
//   12.08.2010 22:57 - minor changes (StrToInt & IntToStr)
//   26.08.2010 06:07 - t**lwr removed, tsznlen added
//   26.08.2010 16:14 - tchlwr added
//   15.07.2011 05:11 - changed StrToInt to return count of chars read
//   01.02.2014 06:38 - tszChar added (host for tzrchr/tszrchr)
//   01.02.2014 07:00 - added option for StrToInt, history log recovered
//   01.02.2014 07:07 - removed "all non-decimal are unsigned" policy
//

#ifndef _TSTR_H_INC_
#define _TSTR_H_INC_


#ifdef __cplusplus
#define EXTERN_C            extern "C"          // simple definition
#define EXTERN_C_BEGIN      extern "C" {        // block definition
#define EXTERN_C_END        }
#else //__cplusplus
#define EXTERN_C            
#define EXTERN_C_BEGIN      
#define EXTERN_C_END        
#endif //__cplusplus


EXTERN_C_BEGIN //extern "C" {


//////////////// chrlwr, strlwr ////////////////

// Converts character to lowercase
//      returns lowercase character
ACHAR achlwr(ACHAR ch);
WCHAR wchlwr(WCHAR ch);

inline TCHAR tchlwr(TCHAR ch)
{
#ifndef UNICODE
    return achlwr(ch);
#else //UNICODE
    return wchlwr(ch);
#endif //UNICODE
}

//////////////// strchr, strrchr ////////////////

#undef tszChar
#ifndef UNICODE
#define tszChar     aszChar
#else //UNICODE
#define tszChar     wszChar
#endif //UNICODE

// [INTERNAL, DO NOT USE DIRECTLY] main function for tszchar() / tszrchar()
ASTR aszChar(CASTR string, ACHAR ch, BOOL first);
WSTR wszChar(CWSTR string, WCHAR ch, BOOL first);

// Searches a string for a first occurence of given character, 
// which may be the null character '\0'.
//      returns a pointer to the found occurrence of ch in the string
//      returns NULL if ch does not occurr in the string
inline TSTR tszchr(CTSTR string, TCHAR ch)
{
    return tszChar(string, ch, TRUE);
}
inline ASTR aszchr(CASTR string, ACHAR ch)
{
    return aszChar(string, ch, TRUE);
}
inline WSTR wszchr(CWSTR string, WCHAR ch)
{
    return wszChar(string, ch, TRUE);
}

// Searches a string for a last occurence of given character, 
// which may be the null character '\0'.
//      returns a pointer to the found occurrence of ch in the string
//      returns NULL if ch does not occurr in the string
inline TSTR tszrchr(CTSTR string, TCHAR ch)
{
    return tszChar(string, ch, FALSE);
}
inline ASTR aszrchr(CASTR string, ACHAR ch)
{
    return aszChar(string, ch, FALSE);
}
inline WSTR wszrchr(CWSTR string, WCHAR ch)
{
    return wszChar(string, ch, FALSE);
}


//////////////// strlen, strnlen ////////////////

#undef tszLength
#ifndef UNICODE
#define tszLength   aszLength
#else //UNICODE
#define tszLength   wszLength
#endif //UNICODE

// [INTERNAL, DO NOT USE DIRECTLY] main function for tszlen() / tsznlen()
DWORD aszLength(CASTR str, DWORD count);
DWORD wszLength(CWSTR str, DWORD count);

// Finds the length in characters of the given string, 
// excluding the final null character.
//      returns length of the str, without of the final null char
inline DWORD tszlen(CTSTR str)
{
    return tszLength(str, 0xFFFFFFFF);
}
inline DWORD aszlen(CASTR str)
{
    return aszLength(str, 0xFFFFFFFF);
}
inline DWORD wszlen(CWSTR str)
{
    return wszLength(str, 0xFFFFFFFF);
}

// Finds the length in characters of the given string, 
// excluding the final null character.
// Search stops after count characters have been passed.
//      returns length of the str, without of the final null char

inline DWORD tsznlen(CTSTR str, DWORD count)
{
    return tszLength(str, count);
}
inline DWORD asznlen(CASTR str, DWORD count)
{
    return aszLength(str, count);
}
inline DWORD wsznlen(CWSTR str, DWORD count)
{
    return wszLength(str, count);
}


//////////////// strcat, strncat, strcpy, strncpy ////////////////

#undef tszConcat
#ifndef UNICODE
#define tszConcat   aszConcat
#else //UNICODE
#define tszConcat   wszConcat
#endif //UNICODE

// [INTERNAL, DO NOT USE DIRECTLY] main function for tsz*cat() / tsz*cpy()
DWORD aszConcat(ASTR dst, CASTR src, DWORD count, BOOL append);
DWORD wszConcat(WSTR dst, CWSTR src, DWORD count, BOOL append);

// Concatenates src onto the end of dest.
// Assumes enough space in dest.
//      returns count of characters copied
inline DWORD tszcat(TSTR dst, CTSTR src)
{
    return tszConcat(dst, src, 0xFFFFFFFF, TRUE);
}
inline DWORD aszcat(ASTR dst, CASTR src)
{
    return aszConcat(dst, src, 0xFFFFFFFF, TRUE);
}
inline DWORD wszcat(WSTR dst, CWSTR src)
{
    return wszConcat(dst, src, 0xFFFFFFFF, TRUE);
}

// Appends at most count characters of the src to the end of dest.
// If count is greater than the length of src, the length of src
// is used instead.
// (Unlike strncat, this routine does not terminate with null character, 
// if count is less than the length of src)
// (Unlike strncpy, this routine does not pad out to count characters)
//      returns count of characters copied
inline DWORD tszncat(TSTR dst, CTSTR src, DWORD count)
{
    return tszConcat(dst, src, count, TRUE);
}
inline DWORD aszncat(ASTR dst, CASTR src, DWORD count)
{
    return aszConcat(dst, src, count, TRUE);
}
inline DWORD wszncat(WSTR dst, CWSTR src, DWORD count)
{
    return wszConcat(dst, src, count, TRUE);
}

// Copies the string src into the spot specified by dest.
// Assumes enough room.
//      returns count of characters copied
inline DWORD tszcpy(TSTR dst, CTSTR src)
{
    return tszConcat(dst, src, 0xFFFFFFFF, FALSE);
}
inline DWORD aszcpy(ASTR dst, CASTR src)
{
    return aszConcat(dst, src, 0xFFFFFFFF, FALSE);
}
inline DWORD wszcpy(WSTR dst, CWSTR src)
{
    return wszConcat(dst, src, 0xFFFFFFFF, FALSE);
}

// Copies count characters from the source string to the destination.
// If count is less than the length of source, NO NULL CHARACTER is 
// put onto the end of the copied string.
// (Unlike strncpy, this routine does not pad out to count characters)
//      returns count of characters copied
inline DWORD tszncpy(TSTR dst, CTSTR src, DWORD count)
{
    return tszConcat(dst, src, count, FALSE);
}
inline DWORD aszncpy(ASTR dst, CASTR src, DWORD count)
{
    return aszConcat(dst, src, count, FALSE);
}
inline DWORD wszncpy(WSTR dst, CWSTR src, DWORD count)
{
    return wszConcat(dst, src, count, FALSE);
}


//////////////// strcmp, stricmp, strncmp, strnicmp ////////////////

#undef tszCompare
#ifndef UNICODE
#define tszCompare  aszCompare
#else //UNICODE
#define tszCompare  wszCompare
#endif //UNICODE

// [INTERNAL, DO NOT USE DIRECTLY] main function for tsz**cmp()
int aszCompare(CASTR str1, CASTR str2, DWORD count, BOOL ignoreCase);
int wszCompare(CWSTR str1, CWSTR str2, DWORD count, BOOL ignoreCase);

// Compares two strings and returns an integer to indicate 
// whether the first is less than the second, the two are
// equal, or whether the first is greater than the second.
//      returns <0 if str1 <  str2
//      returns  0 if str1 == str2
//      returns >0 if str1 >  str2
inline int tszcmp(CTSTR str1, CTSTR str2)
{
    return tszCompare(str1, str2, 0xFFFFFFFF, FALSE);
}
inline int aszcmp(CASTR str1, CASTR str2)
{
    return aszCompare(str1, str2, 0xFFFFFFFF, FALSE);
}
inline int wszcmp(CWSTR str1, CWSTR str2)
{
    return wszCompare(str1, str2, 0xFFFFFFFF, FALSE);
}

// Compares two strings and returns an integer to indicate 
// whether the first is less than the second, the two are
// equal, or whether the first is greater than the second.
// Comparison stops after count characters have been compared.
//      returns <0 if str1 <  str2
//      returns  0 if str1 == str2
//      returns >0 if str1 >  str2
inline int tszncmp(CTSTR str1, CTSTR str2, DWORD count)
{
    return tszCompare(str1, str2, count, FALSE);
}
inline int aszncmp(CASTR str1, CASTR str2, DWORD count)
{
    return aszCompare(str1, str2, count, FALSE);
}
inline int wszncmp(CWSTR str1, CWSTR str2, DWORD count)
{
    return wszCompare(str1, str2, count, FALSE);
}

// Compares two strings, ignoring case, and returns an integer 
// to indicate whether the first is less than the second, the 
// two areequal, or whether the first is greater than the second.
// Upper case characters are converted to lower case.
//      returns <0 if str1 <  str2
//      returns  0 if str1 == str2
//      returns >0 if str1 >  str2
inline int tszicmp(CTSTR str1, CTSTR str2)
{
    return tszCompare(str1, str2, 0xFFFFFFFF, TRUE);
}
inline int aszicmp(CASTR str1, CASTR str2)
{
    return aszCompare(str1, str2, 0xFFFFFFFF, TRUE);
}
inline int wszicmp(CWSTR str1, CWSTR str2)
{
    return wszCompare(str1, str2, 0xFFFFFFFF, TRUE);
}

// Compares two strings, ignoring case, and returns an integer 
// to indicate whether the first is less than the second, the 
// two areequal, or whether the first is greater than the second.
// Comparison stops after count characters have been compared.
// Upper case characters are converted to lower case.
//      returns <0 if str1 <  str2
//      returns  0 if str1 == str2
//      returns >0 if str1 >  str2
inline int tsznicmp(CTSTR str1, CTSTR str2, DWORD count)
{
    return tszCompare(str1, str2, count, TRUE);
}
inline int asznicmp(CASTR str1, CASTR str2, DWORD count)
{
    return aszCompare(str1, str2, count, TRUE);
}
inline int wsznicmp(CWSTR str1, CWSTR str2, DWORD count)
{
    return wszCompare(str1, str2, count, TRUE);
}

//////////////// inttostr strtoint ////////////////

// Converts number to string. If number of output digits is smaller 
// than width, zeroes are added from left side. Radix can be any number 
// between 2 and 36 (inclusive).
//      returns count of characters written
DWORD aszIntToStr(ASTR szString, int nInt, DWORD nWidth, DWORD nRadix, BOOL bUpCase, BOOL bSigned);
DWORD wszIntToStr(WSTR szString, int nInt, DWORD nWidth, DWORD nRadix, BOOL bUpCase, BOOL bSigned);

inline DWORD tszIntToStr(TSTR szString, int nInt, DWORD nWidth, DWORD nRadix, BOOL bUpCase, BOOL bSigned)
{
#ifndef UNICODE
    return aszIntToStr(szString, nInt, nWidth, nRadix, bUpCase, bSigned);
#else //UNICODE
    return wszIntToStr(szString, nInt, nWidth, nRadix, bUpCase, bSigned);
#endif //UNICODE
}

// Converts string to number. Radix can be any number 
// between 2 and 36 (inclusive).
//      returns count of characters read
DWORD aszStrToInt(int* pInt, CASTR szString, DWORD nRadix, BOOL skipws);
DWORD wszStrToInt(int* pInt, CWSTR szString, DWORD nRadix, BOOL skipws);

inline DWORD tszStrToInt(int* pInt, CTSTR szString, DWORD nRadix, BOOL skipws)
{
#ifndef UNICODE
    return aszStrToInt(pInt, szString, nRadix, skipws);
#else //UNICODE
    return wszStrToInt(pInt, szString, nRadix, skipws);
#endif //UNICODE
}


EXTERN_C_END //} //extern "C"

#endif //_TSTR_H_INC_
