///////////////////////////////////////////////////////////////
//
// uLog: [micro] log file support
//
// Copyright © 2019,2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   17.06.2009 13:28 - created
//   18.11.2016 04:34 - adding StringStream'ed version
//   19.11.2016 09:09 - moving ulogf function into a ulogss class
//

#ifndef _MICROLOG_H_INC_
#define _MICROLOG_H_INC_

#include "tunic.h"
#include "uDef.h"
#include "uString.h"



#define ANSI_ONLY

#ifdef ANSI_ONLY
#define L(x)            x
#define LString         AString
#define LStringStream   AStringStream
#define LStringFrom     AStringFrom
#else //
#define L(x)            T(x)
#define LString         UString
#define LStringStream   UStringStream
#define LStringFrom     UStringFrom
#endif


#undef ERROR // wingdi.h macro

enum UL_FLAGS : DWORD {
    //////// single/multi file flags
                                // default: single file, overwrite old file
    SINGLE_FILE_APPEND = 0x01,  // append log to existing file
    MULTI_FILE = 0x02,          // create new file for each session
    //////// formatting flags
    PRINT_HEADER = 0x04,        // print message at session start
    PRINT_TIME = 0x08,          // print time
    USE_UTC = 0x10,             // use universal coordinated time instead of local time
    PRINT_THREADID = 0x20,      // print calling thread id
    ADD_ENDL = 0x40,            // append <cr><lf> after message
};


enum class UL {
    // levels
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    // end-of-line marker
    ENDL,
};

class ulogss
{
    typedef ulogss me;
    LStringStream m_strStream;
    BOOL m_bFirstOutput;
    UL m_level;
    UL_FLAGS m_flags;
    UString m_fileName;
    LString m_programName;
    void logStart(UL level);
    void logEnd(void);
public:
    ulogss(void) :
        m_bFirstOutput(TRUE)
    {
    }

    void init(LString programName, DWORD flags);

    // lss << UL::DEBUG
    me& operator<<(const UL level)
    {
        if(level != UL::ENDL)
        {
            this->logStart(level);
            return (*this);
        }
        this->logEnd();
        return (*this);
    }

    // lss << value
    template <typename T>
    me& operator<<(const T& arg)
    {
        m_strStream << arg;
        return (*this);
    }

    // lss << ulog::pad(value)
    class pad {
        friend me;
    private:
        QWORD m_value;
        DWORD m_width;
    public:
        pad(const BYTE& arg) : m_width(sizeof(BYTE) * 2), m_value(arg) { }
        pad(const WORD& arg) : m_width(sizeof(WORD) * 2), m_value(arg) { }
        pad(const DWORD& arg) : m_width(sizeof(DWORD) * 2), m_value(arg) { }
        pad(const QWORD& arg) : m_width(sizeof(QWORD) * 2), m_value(arg) { }
        pad(const VOIDPTR arg) : m_width(sizeof(VOIDPTR) * 2), m_value(reinterpret_cast<QWORD>(arg)) { }
        pad(const std::size_t& width, const BYTE& arg) : m_width(width), m_value(arg) { }
        pad(const std::size_t& width, const WORD& arg) : m_width(width), m_value(arg) { }
        pad(const std::size_t& width, const DWORD& arg) : m_width(width), m_value(arg) { }
        pad(const std::size_t& width, const QWORD& arg) : m_width(width), m_value(arg) { }
        pad(const std::size_t& width, const VOIDPTR arg) : m_width(width), m_value(reinterpret_cast<QWORD>(arg)) { }
    };
    me& operator<<(const pad& arg)
    {
        m_strStream.width(arg.m_width);
        m_strStream << arg.m_value;
        return (*this);
    }

    // lss << ulog::pad(value)
    class hex {
        friend me;
    private:
        QWORD m_value;
        DWORD m_width;
    public:
        hex(const BYTE& arg) : m_width(sizeof(BYTE) * 2), m_value(arg) { }
        hex(const WORD& arg) : m_width(sizeof(WORD) * 2), m_value(arg) { }
        hex(const DWORD& arg) : m_width(sizeof(DWORD) * 2), m_value(arg) { }
        hex(const QWORD& arg) : m_width(sizeof(QWORD) * 2), m_value(arg) { }
        hex(const VOIDPTR arg) : m_width(sizeof(VOIDPTR) * 2), m_value(reinterpret_cast<QWORD>(arg)) { }
    };
    me& operator<<(const hex& arg)
    {
        m_strStream.width(arg.m_width);
        m_strStream << std::hex;
        m_strStream << arg.m_value;
        m_strStream << std::dec;
        return (*this);
    }

    void _Add_vtordisp1() { } // Required to avoid VC++ warning C4250
    void _Add_vtordisp2() { } // Required to avoid VC++ warning C4250
};
//typedef ulogss ulog;
typedef ulogss::pad ulpad;
typedef ulogss::hex ulhex;


extern ulogss lss;



#endif //_MICROLOG_H_INC_
