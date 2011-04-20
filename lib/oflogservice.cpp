/*
  Copyright (C) 1997-2011 by Suntail.com AS, Tim Whelan

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <ofsys.h>
#include <oflogservice.h>
#include <ofos.h>

#if !defined(OFOPSYS_WIN32)
#include <syslog.h>
#include <stdarg.h>
#endif

string OFLogService::m_appName = "ofsystem";

void OFLogService::writeLine(const char* format, ...)
{
    va_list arglist;
    va_start( arglist, format );
    char line[OF_LOG_LINE_LENGTH+10];
#if defined(OFOPSYS_WIN32)
    ::_vsnprintf
#else
        ::vsnprintf
#endif
        ( line, OF_LOG_LINE_LENGTH - 3, format, arglist );

    va_end( arglist );

    if (!OFOS::strstr (line, "\n"))
        OFOS::strcat( line, "\n" );
    write_(LOG_NOTICE, line);
}

void OFLogService::errorLine(const char* format, ...)
{
    va_list arglist;
    va_start( arglist, format );
    char line[OF_LOG_LINE_LENGTH+10];
#if defined(OFOPSYS_WIN32)
    ::_vsnprintf
#else
        ::vsnprintf
#endif
        ( line, OF_LOG_LINE_LENGTH - 3, format, arglist );

    va_end( arglist );

    if (!OFOS::strstr (line, "\n"))
        OFOS::strcat( line, "\n" );
    write_(LOG_ERR, line);
}

void OFLogService::debugLine(const char* format, ...)
{
#if !defined(NDEBUG)
#else
    va_list arglist;
    va_start( arglist, format );
    char line[OF_LOG_LINE_LENGTH+10];
#if defined(OFOPSYS_WIN32)
    ::_vsnprintf
#else
        ::vsnprintf
#endif
        ( line, OF_LOG_LINE_LENGTH - 3, format, arglist );

    va_end( arglist );

    if (!OFOS::strstr (line, "\n"))
        OFOS::strcat( line, "\n" );
    write_(LOG_DEBUG, line);
#endif
}

void OFLogService::write_(ofuint32 sev, const string& line)
{
#if !defined(OFOPSYS_WIN32)
    if (sev != LOG_DEBUG)
    {
        openlog(m_appName.c_str(), LOG_PID | LOG_CONS, LOG_USER );
        syslog(sev, "%s", line.c_str());
        closelog();
    }
#if !defined(NDEBUG)
        cerr << line ;
#endif // NDEBUG
#endif
#if defined(OFOPSYS_WIN32)
        //ReportEvent( m_eventSource, EVENTLOG_INFORMATION_TYPE, 1, OF_DEBUG_EVENT, NULL, 1, 0, &line, 0 );
        OFLockGuard<OFMutex> lock( &m_mutexFile );

        char path[OF_MAX_PATH_LEN+1];
        OFOS::snprintf( path, OF_MAX_PATH_LEN, "c:\\%s.log", m_appName.c_str());
        OFFile file(path, 0664, OFFILE_OPEN_ALWAYS | OFFILE_READWRITE);
        file.position(0, OFFILE_POSITION_END);
        file.write(line.c_str(), line.length());
#if !defined(NDEBUG)
        cout << "[" << GetCurrentThreadId() << "] " << line ;
#endif // NDEBUG
#endif
}
