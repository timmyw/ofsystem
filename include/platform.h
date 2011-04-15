/* $Revision: 2.1 $

$Id: platform.h,v 2.1 2006-09-06 07:03:28 timmy Exp $
*/

#if !defined(_PLATFORM_H_)
#define _PLATFORM_H_

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
