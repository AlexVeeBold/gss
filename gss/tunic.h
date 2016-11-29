///////////////////////////////////////////////////////////////
//
// Tiny Unicode helper
//
// Copyright © 2008-2012, 2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   13.04.2008 06:36 - created
//   22.04.2008 09:02 - added strict-defined types
//   28.04.2008 16:54 - typedef's replaced with #define's
//   08.06.2009 21:12 - rearrangement & optimization (LP* disabled, char -> unsigned char)
//   09.06.2009 06:33 - unsigned char -> char (all ansi api's are "signed char")
//   17.06.2009 10:04 - added CTCHAR, CACHAR, CWCHAR, misc. char constants, killed LP?STR's
//   26.06.2009 03:18 - disabled header guard for tstr.cpp double compilation (ansi/wide)
//                    and updated misc. char constants for same reason
//   22.08.2011 16:34 - added undefined (reference) char constants
//   25.08.2011 22:25 - added (re)definition of file id macro (_TUNIC_H_INC_)
//   09.05.2012 20:57 - renamed ZEROCHAR to NULLCHAR
//   19.05.2012 01:41 - added abbreviated control char names
//

//#ifndef _TUNIC_H_INC_
#undef _TUNIC_H_INC_
#define _TUNIC_H_INC_

/*  how to use:
  original          replacement                 
  ----------------------------------------------
  char              TCHAR                       
  const char        CTCHAR                      
  char *            TSTR     or  TCHAR*         
  const char *      CTSTR    or  CTCHAR*        
  x = 'a'           x = T('a')                  
  x = "string"      x = T("string")             
*/

// discard all previous definitions
#undef T
#undef TCHAR
#undef CTCHAR
#undef TSTR
#undef CTSTR
#undef ACHAR
#undef WCHAR
#undef CACHAR
#undef CWCHAR
#undef ASTR
#undef WSTR
#undef CASTR
#undef CWSTR

// strict-defined types
#define ACHAR   char   // Ansi
#define WCHAR   wchar_t //unsigned short  // Wide (Unicode)
#define CACHAR  const ACHAR
#define CWCHAR  const WCHAR
#define ASTR    ACHAR*
#define WSTR    WCHAR*
#define CASTR   const ACHAR*
#define CWSTR   const WCHAR*

// base type
#ifndef UNICODE
#define T(x)    x
#define TCHAR   ACHAR
#else
#define T(x)    L ## x
#define TCHAR   WCHAR
#endif // !UNICODE

// derived types
#define CTCHAR      const TCHAR
#define TSTR        TCHAR*
#define CTSTR       const TCHAR*

// misc. char constants
#undef NULLCHAR
#define NULLCHAR        T('\0')     // U0000  NUL  null
//
//                                  // U0001  SOH  start of heading
//
//                                  // U0002  STX  start of text
//
//                                  // U0003  ETX  end of text
//
//                                  // U0004  EOT  end of transmission
//
//                                  // U0005  ENQ  enquiry
//
//                                  // U0006  ACK  acknowledge
#undef BELLCHAR
#define BELLCHAR        T('\a')     // U0007  BEL  bell
#undef BACKSPACECHAR
#define BACKSPACECHAR   T('\b')     // U0008  BS   backspace
#undef HTABCHAR
#define HTABCHAR        T('\t')     // U0009  HT   horizontal tabulation
#undef LFCHAR
#define LFCHAR          T('\n')     // U000A  LF   line feed / new line
#undef VTABCHAR
#define VTABCHAR        T('\v')     // U000B  VT   vertical tabulation
#undef FORMFEEDCHAR
#define FORMFEEDCHAR    T('\f')     // U000C  FF   form feed
#undef CRCHAR
#define CRCHAR          T('\r')     // U000D  CR   carriage return
//
//                                  // U000E  SO   shift out
//
//                                  // U000F  SI   shift in
//
//                                  // U0010  DLE  data link escape
//
//                                  // U0011  DC1  device control one
//
//                                  // U0012  DC2  device control two
//
//                                  // U0013  DC3  device control three
//
//                                  // U0014  DC4  device control four
//
//                                  // U0015  NAK  negative acknowlegde
//
//                                  // U0016  SYN  synchronous idle
//
//                                  // U0017  ETB  end of transmission block
//
//                                  // U0018  CAN  cancel
//
//                                  // U0019  EM   end of meduim
//
//                                  // U001A  SUB  substitube
//
//                                  // U001B  ESC  escape
//
//                                  // U001C  FS   information separator four
//
//                                  // U001D  GS   information separator three
//
//                                  // U001E  RS   information separator two
//
//                                  // U001F  US   information separator one
#undef SPACECHAR
#define SPACECHAR       T(' ')      // U0020  SP   space
#undef EXCLAMCHAR
#define EXCLAMCHAR      T('!')      // U0021  exclamation mark
#undef DQUOTECHAR
#define DQUOTECHAR      T('\"')     // U0022  quotation mark
#undef NUMBERCHAR
#define NUMBERCHAR      T('#')      // U0023  number sign
#undef DOLLARCHAR
#define DOLLARCHAR      T('$')      // U0024  dollar sign
#undef PERCENTCHAR
#define PERCENTCHAR     T('%')      // U0025  percent sign
#undef AMPERSANDCHAR
#define AMPERSANDCHAR   T('&')      // U0026  ampersand
#undef APOSTRCHAR
#define APOSTRCHAR      T('\'')     // U0027  apostrophe
#undef OPENPARCHAR
#define OPENPARCHAR     T('(')      // U0028  opening parenthesis (round bracket)
#undef CLOSEPARCHAR
#define CLOSEPARCHAR    T(')')      // U0029  closing parenthesis (round bracket)
#undef STARCHAR
#define STARCHAR        T('*')      // U002A  asterisk / star
#undef PLUSCHAR
#define PLUSCHAR        T('+')      // U002B  plus
#undef COMMACHAR
#define COMMACHAR       T(',')      // U002C  comma
#undef HYPHENCHAR
#define HYPHENCHAR      T('-')      // U002D  hyphen / minus
#undef MINUSCHAR
#define MINUSCHAR       HYPHENCHAR
#undef DOTCHAR
#define DOTCHAR         T('.')      // U002E  full stop / period / dot / decimal point
#undef SLASHCHAR
#define SLASHCHAR       T('/')      // U002F  solidus / slash / virgule
#undef NUMZEROCHAR
#define NUMZEROCHAR     T('0')      // U0030  digit zero
#undef NUMNINECHAR
#define NUMNINECHAR     T('9')      // U0039  digit nine
#undef COLONCHAR
#define COLONCHAR       T(':')      // U003A  colon
#undef SEMICOLONCHAR
#define SEMICOLONCHAR   T(';')      // U003B  semicolon
#undef LESSCHAR
#define LESSCHAR        T('<')      // U003C  less than sign / opening angle bracket (diamond bracket, cone bracket, chevron)
#undef EQUALSCHAR
#define EQUALSCHAR      T('=')      // U003D  equals sign
#undef GREATERCHAR
#define GREATERCHAR     T('>')      // U003E  greater than sign / closing angle bracket (diamond bracket, cone bracket, chevron)
#undef QUESTIONCHAR
#define QUESTIONCHAR    T('?')      // U003F  question mark
#undef ATCHAR
#define ATCHAR          T('@')      // U0040  commercial at sign
#undef CAPACHAR
#define CAPACHAR        T('A')      // U0041  capital letter a
#undef CAPFCHAR
#define CAPFCHAR        T('F')      // U0046  capital letter f
#undef CAPZCHAR
#define CAPZCHAR        T('Z')      // U005A  capital letter z
#undef OPENSQRCHAR
#define OPENSQRCHAR     T('[')      // U005B  opening square bracket (box bracket)
#undef BACKSLASHCHAR
#define BACKSLASHCHAR   T('\\')     // U005C  reverse solidus / backslash
#undef CLOSESQRCHAR
#define CLOSESQRCHAR    T(']')      // U005D  closing square bracket (box bracket)
#undef CIRCUMCHAR
#define CIRCUMCHAR      T('^')      // U005E  circumflex accent
#undef UNDERCHAR
#define UNDERCHAR       T('_')      // U005F  low line / underscore
#undef GRAVECHAR
#define GRAVECHAR       T('`')      // U0060  grave accent
#undef SMALLACHAR
#define SMALLACHAR      T('a')      // U0061  small letter a
#undef SMALLFCHAR
#define SMALLFCHAR      T('f')      // U0066  small letter f
#undef SMALLZCHAR
#define SMALLZCHAR      T('z')      // U007A  small letter z
#undef OPENCURCHAR
#define OPENCURCHAR     T('{')      // U007B  opening curly bracket (brace)
#undef VLINECHAR
#define VLINECHAR       T('|')      // U007C  vertical line / vertical bar
#undef CLOSECURCHAR
#define CLOSECURCHAR    T('}')      // U007D  closing curly bracket (brace)
#undef TILDECHAR
#define TILDECHAR       T('~')      // U007E  tilde
//
//                                  // U007F  DEL  delete
//#define CHAR      T('')      // U00


//#endif //_TUNIC_H_INC_
