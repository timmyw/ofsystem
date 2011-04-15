/*
 * $Revision: 2.5 $
 *
 * $Id: ofsys.h,v 2.5 2008-11-28 13:57:44 timmy Exp $
 */

#if !defined(_OF_SYS_H_)
#define _OF_SYS_H_

#include "platform.h"

#if defined(OFOPSYS_WIN32)
#pragma warning( disable: 4251 4786)
#define NOMINMAX

#include <winsock2.h>
#include <windows.h>
#include <assert.h>

#if defined(OF_STATIC_BUILD)
#  define OFSYS_API
#else // STATIC_BUILD
#  if defined(OFEXPORTS)
#    define OFSYS_API  __declspec(dllexport)
#  else
#    define OFSYS_API  __declspec(dllimport)
#  endif
#endif // STATIC_BUILD
#else // non Win32
#  define OFSYS_API
#endif

#if defined(UNIT_TEST)
#undef OFSYS_API
#define OFSYS_API
#endif

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#endif

#if defined(OFOPSYS_SOLARIS)
#include <stdarg.h>
#endif

#if defined(OFOPSYS_WIN32)
#include <process.h>
#endif

/*
#include <ofsys/oflimit.h>
#include <ofsys/oferr.h>
*/

#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <time.h>

#include <oftypes.h>
#include <offlags.h>
#include <oflimits.h>

#endif // #if !defined(_OF_SYS_H_)
