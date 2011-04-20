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

#if !defined(_OF_DEFINES_H_)
#define _OF_DEFINES_H_

#include "platform.h"
#include <algorithm>

/** \file ofdefines.h
    Platform independant typedefs and macros.
    On Win32 platforms assumes that windows.h is already included.
*/

#if defined(NDEBUG)
#define OFINLINE inline
#else
#define OFINLINE
#endif

#define DECLSTRING(a,b) char a[b]; ::memset(a, 0, b);
#define RESETSTRING(a) (a[0]=0)

#define SHOULD_NEVER_GET_HERE   0

# if defined (ghs) || defined (__GNUC__) || defined (__hpux) || defined (__sgi) || defined (DEC_CXX) || defined (__KCC) || defined (__rational__) || (__USLC__)
#   define OF_UNUSED_ARG(a) {if (&a) /* null */ ;}
# else
#   define OF_UNUSED_ARG(a) (a)
# endif /* ghs */

#if !defined(OF_FUNC_CAST)
#define OF_FUNC_CAST(a,b) (a)b
#endif

#include <oftypes.h>

#if defined(OFOPSYS_WIN32)
#define MAKEULL(a) a##ui64
#define MAKELL(a) a##i64
#define LLFORMAT "%I64"
#define LLFORMAT2 "I64"
#define DFORMAT "%ld"
#define DFORMAT2 "ld"
#define UFORMAT "%lu"
#define XFORMAT "%lx"
#else
#define MAKEULL(a) a##ULL
#define MAKELL(a) a##LL

/** \macro LLFORMAT
    
   Defines a macro which supplies the current platform's correct
    printf specifier for long long integer (64-bit)
 */
#define LLFORMAT "%ll"

/** \macro LLFORMAT2

   Similar to the LLFORMAT macro, this macro does not supply the
   leading '%' character for format specifiers that require additional
   flags or width elements.
 */
#define LLFORMAT2 "ll"

#if defined(WORDSIZE64)
  #define DFORMAT "%d"
  #define DFORMAT2 "d"
  #define UFORMAT "%u"
  #define XFORMAT "%x"
#else

/** \macro DFORMAT
    
   The DFORMAT macro defines the printf (fprintf) etc format specifier
   for the standard integer type used in openFabric, typedef'ed as
   ofint32.  This needs to be %ld on 32-bit platforms and %d on 64-bit
   platforms.
 */
#define DFORMAT "%ld"

/** \macro DFORMAT2
    
   The DFORMAT macro defines the printf (fprintf) etc format specifier
   for the standard integer type used in openFabric, typedef'ed as
   ofint32.  This is the same as the \c DFORMAT macro, except the
   leading '%' sign is omitted for specifiers that require additional
   width modifiers.
 */
#define DFORMAT2 "ld"

/** \macro UFORMAT
    
   The DFORMAT macro defines the printf (fprintf) etc format specifier
   for the standard unsigned integer type used in openFabric,
   typedef'ed as ofuint32.  This needs to be %ld on 32-bit platforms
   and %d on 64-bit platforms.
 */
#define UFORMAT "%lu"

/** \macro XFORMAT

    The XFORMAT macro defines {,f}printf format specifier for a
    hexadecimal representation of a standard integer.
 */
#define XFORMAT "%lx"

#endif // WORDSIZE64
#endif // OFOPSYS_WIN32

#if defined(OFNULL)
#undef OFNULL
#endif
#define OFNULL  0

#if defined(OF_NULL_ID)
#undef OF_NULL_ID
#endif
#define OF_NULL_ID  "00000000000000000000000000000000000000"

#if defined(OF_THIS_ID)
#undef OF_THIS_ID
#endif
#define OF_THIS_ID  "ffffffffffffffffffffffffffffffffffffff"

#define HTTP_VERSION "HTTP/1.1"

#define OF_CONFIG_FILE "openfabric.conf"
#define OF_DEFAULT_PORT 18009
#define OF_DEFAULT_SSL_PORT 18010

#define OF_OBJECT_SEPARATOR '/'
#define OF_METHOD_SEPARATOR '.'
#define OF_PROPERTY_SEPARATOR '.'
#define OF_OBJECT_SEPARATOR_STR "/"
#define OF_METHOD_SEPARATOR_STR "."

#define OF_LITTLE_ENDIAN 1234
#define OF_BIG_ENDIAN 4321

/** The version (and magic number) for user private data formats
 */
#define PRIVATE_DATA_VERSION 2

#if defined(__BYTE_ORDER)
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define OF_BYTE_ORDER OF_LITTLE_ENDIAN
# else // __BYTE_ORDER == __LITTLE_ENDIAN
#  define OF_BYTE_ORDER OF_BIG_ENDIAN
# endif // __BYTE_ORDER == __LITTLE_ENDIAN
#else // defined(__BYTE_ORDER)
// Assume non GCC platform on Win32
#define OF_BYTE_ORDER OF_LITTLE_ENDIAN
#endif // defined(__BYTE_ORDER)


template<typename T>
inline
void
delete_elem(T* t)
{
    delete &t;
}

template<typename T>
inline
void
delete_elem_array(T* t)
{
    delete [] t;
}

#define OFSTD_PARAM( APPNAME, ARGS, HOST, PORT, SSLPORT, SILENT) {\
    if ( ARGS.doesArgumentExist( "help" ) ) { version(APPNAME); usage(APPNAME); exit( 0 ); } \
    if ( ARGS.doesArgumentExist( "version" ) ) { version(APPNAME); exit( 0 ); } \
    if ( ARGS.doesArgumentExist( "host" ) ) { OFOS::strncpy2( HOST, ARGS.getArgumentValue( "host" ), 1024 ); } \
    if ( ARGS.doesArgumentExist( "port" ) ) { PORT = atol( ARGS.getArgumentValue( "port" ) ); } \
    if ( ARGS.doesArgumentExist( "sslport" ) ) { SSLPORT = atol( ARGS.getArgumentValue( "sslport" ) ); } \
    if ( ARGS.doesArgumentExist( "silent" ) ) { SILENT = 1; } \
    OFConfigurator *config;\
    if ( ARGS.doesArgumentExist( "config" ) ) \
         config = new OFConfigurator( ARGS.getArgumentValue( "config" ), true, false ); \
    else \
        config = new OFConfigurator( "client" );\
    SSLContext.initFromFile( config, false );\
    delete config;\
    }

#define OFCUSTOM_PARAM_FLAG( ARGS, PARAM, FLAGVAR ) { \
    if ( ARGS.doesArgumentExist( PARAM ) ) FLAGVAR=1; else FLAGVAR = 0; }

#define OFCUSTOM_PARAM_INT32( ARGS, PARAM, INTVAR ) { \
    if ( ARGS.doesArgumentExist( PARAM ) ) INTVAR = atol( ARGS.getArgumentValue( PARAM ) ); }

#define OFCUSTOM_PARAM_STR(ARGS, PARAM, STRVAR, STRLEN) { \
    if (ARGS.doesArgumentExist(PARAM)) OFOS::strncpy2(STRVAR, ARGS.getArgumentValue(PARAM), STRLEN); }

#define OFSTD_VERSION \
   void version(const char* appname) { DISPLAY_VERSION(appname) }

#define OFHANDLE_INVAL_PARAMS(ARGC, ARGV, CMDARG, INVALMSG) { \
    ofuint32 __found_inval = 0; \
    for (ofint32 __j = 1; __j < ARGC; __j++) \
        if ((CMDARG).unrecognised()[__j]) { \
            cerr << "Unrecognised command line option: " << ARGV[__j] << endl; \
            __found_inval = 1; \
        } \
    if (__found_inval) { \
        cerr << INVALMSG << endl; \
        return -1; \
    } }
    

#endif
