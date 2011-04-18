/*
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

#if !defined(NDEBUG)
void OFLogService::debugLine(const char* format, ...)
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
    write_(LOG_DEBUG, line);
}
#endif

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
