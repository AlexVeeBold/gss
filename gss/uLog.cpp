///////////////////////////////////////////////////////////////
//
// uLog: [micro] log file support
//
// Copyright © 2009-2016, Alexander Boldyrev
// The copyright in this software is being made available 
// under the 2-Clause BSD License.
//
//
//   17.06.2009 13:27 - created
//   07.09.2009 16:32 - added more options (multifile, header, etc.)
//   06.03.2014 09:36 - cleaned up a little, added ULOG_LEVEL
//   19.04.2014 14:37 - added ulogInit()
//   18.11.2016 05:55 - added ulogss class [ulogf() clone]
//   19.11.2016 12:21 - removed ulogf()
//   20.11.2016 14:02 - polished & cleaned up
//

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>


#include "uLog.h"


ulogss lss;


::CRITICAL_SECTION g_csLog = {0, };

const LString uLogMsgType[] = {
    L("DEBUG"), 
    L("INFO"), 
    L("WARN"), 
    L("ERROR"), 
    L("FATAL")
};

void ulogss::init(LString programName, DWORD flags)
{
    ::InitializeCriticalSection(&g_csLog);
    m_flags = static_cast<UL_FLAGS>(flags);
    UStringStream ss;
    UString name(programName.begin(), programName.end());
    ss << name;
    ss << L("_log");

    if(m_flags & UL_FLAGS::MULTI_FILE)
    {
        ::SYSTEMTIME st;
        if(m_flags & UL_FLAGS::USE_UTC)
        {
            ::GetSystemTime(&st);
        }
        else
        {
            ::GetLocalTime(&st);
        }
        ss << std::setfill(NUMZEROCHAR);
        ss << UNDERCHAR << std::setw(4) << st.wYear << std::setw(2) << st.wMonth << std::setw(2) << st.wDay;
        ss << HYPHENCHAR << std::setw(2) << st.wHour << std::setw(2) << st.wMinute << std::setw(2) << st.wSecond;
        ss << HYPHENCHAR << std::setw(3) << st.wMilliseconds;
    }

    ss << L(".txt");
    m_fileName.assign(ss.str());
    m_programName.assign(programName);

    m_strStream.fill(L('0'));
    m_strStream.setf(std::ios::fixed);
    m_strStream.precision(12);
}

void ulogss::logStart(UL level)
{
    m_level = level;
}

void ulogss::logEnd(void)
{
    LString eol(L("\r\n"));
    LStringStream ss;
    ss << std::setfill(L('0'));

    ::EnterCriticalSection(&g_csLog);
    ::HANDLE hLogFile;
    hLogFile = ::CreateFile(m_fileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwDummy = ::GetLastError();
    ::SetLastError(0);    // clear "file already exists" error
    if(hLogFile != INVALID_HANDLE_VALUE)
    {
        SYSTEMTIME st;
        if(m_flags & (UL_FLAGS::PRINT_HEADER | UL_FLAGS::PRINT_TIME))
        {
            if(m_flags & UL_FLAGS::USE_UTC)
            {
                ::GetSystemTime(&st);
            }
            else
            {
                ::GetLocalTime(&st);
            }
        }
        
        if((m_flags & UL_FLAGS::SINGLE_FILE_APPEND) || (m_bFirstOutput == FALSE))
        {
            ::SetFilePointer(hLogFile, 0, NULL, FILE_END);  // move to end of file
        }

        if(m_bFirstOutput != FALSE)
        {
            if(m_flags & UL_FLAGS::SINGLE_FILE_APPEND)
            {
                DWORD dwSize = ::GetFileSize(hLogFile, NULL);
                if(dwSize != 0)
                {
                    ss << eol << eol;
                }
            }

            if(m_flags & UL_FLAGS::PRINT_HEADER)
            {
                ss << L("=== ") << m_programName << L(" started on ") << std::setw(4) << st.wYear << L('.') << std::setw(2) << st.wMonth << L('.') << std::setw(2) << st.wDay << L(' ');
                ss << std::setw(2) << st.wHour << L(':') << std::setw(2) << st.wMinute << L(':') << std::setw(2) << st.wSecond << L('.') << std::setw(3) << st.wMilliseconds;
                ss << L(" ===") << eol;
            }

            m_bFirstOutput = FALSE;
        }

        if(m_flags & UL_FLAGS::PRINT_TIME)
        {
            //print time
            ss << std::setw(2) << st.wHour << L(':') << std::setw(2) << st.wMinute << L(':') << std::setw(2) << st.wSecond << L('.') << std::setw(3) << st.wMilliseconds;

            if(m_flags & UL_FLAGS::PRINT_THREADID)
            {
                //print space before thread id
                ss << L(' ');
            }
        }

        if(m_flags & UL_FLAGS::PRINT_THREADID)
        {
            //print thread id
            ss << L("tid_") << std::setw(4) << ::GetCurrentThreadId();
        }

        if(m_flags & (UL_FLAGS::PRINT_TIME | UL_FLAGS::PRINT_THREADID))
        {
            //print colon before message
            ss << L(": ");
        }

        //print type
        ss << L('[') << uLogMsgType[static_cast<std::size_t>(m_level)] << L(']') << L(' ');
        //print message
        ss << m_strStream.str();

        if(m_flags & UL_FLAGS::ADD_ENDL)
        {
            //print end-of-line
            ss << eol;
        }

        AString outStr(ss.str());
        int nLen;
        nLen = outStr.size();
        //write buffer to file
        ULONG ulDummy;
        ::WriteFile(hLogFile, outStr.c_str(), outStr.size(), &ulDummy, NULL);
        ::CloseHandle(hLogFile);

        // clear stringstream
        this->m_strStream.str(L(""));
    }

    ::LeaveCriticalSection(&g_csLog);
}


