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

#if !defined(_PLATFORM_H_)
#define _PLATFORM_H_

#include "platform.i"

#if defined(_WIN32)
#if !defined(OFOPSYS_WIN32)
#define OFOPSYS_WIN32
#endif
#if !defined(OFPLATFORM_I386)
#define OFPLATFORM_I386
#endif
#endif

#if !defined(OFPLATFORM_I386) && !defined(OFPLATFORM_X86_64) \
    && !defined(OFPLATFORM_PPCMAC) \
    && !defined(OFPLATFORM_SPARC)
#error Unsupported hardware platform
#endif

#if !defined(OFOPSYS_WIN32) && !defined(OFOPSYS_LINUX) && !defined(OFOPSYS_SOLARIS) && !defined(OFOPSYS_FREEBSD) && !defined(OFOPSYS_DARWIN)
#error Unsupported operating system
#endif

/* Set up a GCC version macro
 */
#if defined(__GNUC__)
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#endif // defined(__GNUC__)

/* Determines various include requirements
 */
#if defined(GCC_VERSION)
#if GCC_VERSION > 30000
#define INCLUDE_STDARG_H
#endif
#endif // defined(__GNUC__)

#endif // #if !defined(_PLATFORM_H_)
