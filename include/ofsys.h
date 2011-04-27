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
#  if defined(ofsystem_EXPORTS)
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
#include <ofdefines.h>
#include <oferr.h>

#include <cas.h>

using namespace std;

#endif // #if !defined(_OF_SYS_H_)
