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

#if !defined(_OFSYS_H_INCLUDED_)
#error "Please do not include this file directly.  Include <ofsys.h>"
#endif

#if !defined(_OFLOGSERVICE_H_)
#define _OFLOGSERVICE_H_

#include <string>
using namespace std;

#if defined(OFOPSYS_WIN32)

#include <ofmutex.h>

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

    /** Write a line to stdout
     */
    static void dumpLine(const char* format, ...);

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

#if defined(OFOPSYS_WIN32)
	/** Mutex for file loggin
	*/
	static OFMutex m_mutexFile;
#endif

};

#endif // _OFLOGSERVICE_H_
