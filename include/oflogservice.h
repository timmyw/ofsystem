/*
 */

#include <string>
using namespace std;

#if !defined(_OFLOGSERVICE_H_)
#define _OFLOGSERVICE_H_

#if defined(OFOPSYS_WIN32)
#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */
#endif // OFOPSYS_WIN32

/** Provides a simple cross platform logging service
 */
class OFSYS_API OFLogService
{
 public:
    /** \name Log writing methods
     */
    //@{

    /** Write a standard line to the log
     */
    static void writeLine(const char* format, ...);

    /** Write an error line to the log
     */
    static void errorLine(const char* format, ...);

    /** Write a debug line to the log

        Does nothing for a release build
    */
    static void debugLine(const char* format, ...);

    //@}

    /** \name Helper methods
     */
    //@{

    /** Set the current application name
     */
    void appName(const string& appName);

    //@}

 private:

    /** The name of the current application
     */
    static string m_appName;

    /** Write the supplied line to log
     */
    static void write_(ofuint32 sev, const string& line);

};

#endif // _OFLOGSERVICE_H_
