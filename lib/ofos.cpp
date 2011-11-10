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
#include <ofos.h>
#include <ofplatform.h>
#include <ctype.h>
#include <stdarg.h>
#include <signal.h>

#define LIBVER_MAJOR_   3
#define LIBVER_MINOR_   0
#define LIBVER_RELEASE_ 1
const char * LIBVER_DESC_ofsys = "openFabric System library"; 
extern "C" ofint32 libraryversion_ofsys( ) { return ( LIBVER_MAJOR_ << 16) | LIBVER_MINOR_; } 
extern "C" ofint32 libraryrelease_ofsys( ) {    return LIBVER_RELEASE_; } 
extern "C" const char *librarydescription_ofsys( ) { return LIBVER_DESC_ofsys; }

#if !defined(NDEBUG)
#include <ofos.i>
#endif

#if defined(OFOPSYS_WIN32)
const double OFOS::PI = 3.14159265358979323846;
const double OFOS::E = 2.7182818284590452354;
#else
const double OFOS::PI = M_PI;
const double OFOS::E = M_E;
#endif

char * 
OFOS::mktemp( const char *mask, char *buffer, char *prefix /*= NULL*/ )
{
#if defined(OFOPSYS_WIN32)
    char tempmask[OF_MAX_PATH_LEN+1];
    char newmask[OF_MAX_PATH_LEN+1];
    char newpref[OF_MAX_PATH_LEN+1];

    OFOS::strcpy( tempmask, mask );
    OFPlatform::fixPath( tempmask );
    OFOS::strcpy( newpref, tempmask );

    if ( OFOS::strrchr( tempmask, '\\' ) )
    {
        OFOS::strcpy( newmask, OFOS::strrchr( tempmask, '\\' ) );
        *(char *)OFOS::strrchr( newpref, '\\' ) = 0;
    }
    else
        OFOS::strcpy( newmask, "OCAL" );


    /*    if ( !prefix )
          GetTempPath( OF_MAX_PATH_LEN, newpref );
          else
          OFOS::strcpy( newpref, prefix );
    */
	
    GetTempFileName( newpref, newmask, 0, buffer );
    return buffer;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    OFOS::strcpy( buffer, mask );
    ofint32 fd = ::mkstemp( buffer );
    if ( fd != -1 )
        close( fd );
    return buffer;
#else
#error Undefined platform
#endif
}

OFOS::of_handle_t
OFOS::openFile( const char *name, ofuint32 mode, ofuint32 flags )
{
    OFOS::of_handle_t fh = (OFOS::of_handle_t)-1;
#if defined(OFOPSYS_WIN32)
        ofuint32 accflags = ( flags & (OFFILE_READONLY|OFFILE_READWRITE) ) ? GENERIC_READ : 0;
        accflags |= ( flags & (OFFILE_WRITEONLY|OFFILE_READWRITE) ) ? GENERIC_WRITE : 0;
        ofuint32 dispflags = (( flags & OFFILE_CREATE_ALWAYS)?CREATE_ALWAYS:0) | ((flags & OFFILE_OPEN_ALWAYS)?OPEN_ALWAYS:0) | ((flags & OFFILE_OPEN_EXISTING)?OPEN_EXISTING:0); 
        ofuint32 shareflags = FILE_SHARE_READ | ((flags & OFFILE_READWRITE )?FILE_SHARE_WRITE:0); 
        ofuint32 fileAttrs = (flags & OFFILE_TEMPORARY)?FILE_ATTRIBUTE_TEMPORARY:FILE_ATTRIBUTE_NORMAL;
        fh = CreateFile( name, accflags, shareflags, NULL, dispflags, fileAttrs, NULL );
#else
        ofint32 nflags = 0;
        if ( flags & OFFILE_READONLY ) nflags |= O_RDONLY;
        if ( flags & OFFILE_WRITEONLY ) nflags |= O_WRONLY;
        if ( flags & OFFILE_READWRITE ) nflags |= O_RDWR;
        if ( flags & OFFILE_CREATE_ALWAYS ) nflags |= (O_TRUNC | O_CREAT);
        if ( flags & OFFILE_OPEN_ALWAYS) nflags |= O_CREAT;
        fh = ::open( name, nflags, mode );
#endif
    return fh;
}

void
OFOS::closeFile( OFOS::of_handle_t fh )
{
#if defined(OFOPSYS_WIN32)
    CloseHandle( fh );
#else
    close( fh );
#endif
}

/*static */
int 
OFOS::snprintf( char *buffer, ofuint32 bufferSize, const char *format, ... )
{
    va_list arglist;
    va_start( arglist, format );
    int ret = 
#if defined(OFOPSYS_WIN32)
		::_vsnprintf
#else
		::vsnprintf
#endif
		( buffer, bufferSize, format, arglist );
    va_end( arglist );
    return ret;
}

int 
OFOS::vsnprintf( char *buffer, ofuint32 bufferSize, const char *format, ... )
{
    va_list arglist;
    va_start( arglist, format );
    int ret = 
#if defined(OFOPSYS_WIN32)
		::_vsnprintf
#else
		::vsnprintf
#endif
		( buffer, bufferSize, format, arglist );
    va_end( arglist );
	return ret;
}

int
OFOS::kill( OFOS::of_thread_t pid, int sig )
{
#if defined(OFOPSYS_WIN32)
    return raise( sig );
#else
#if defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    return pthread_kill( pid, sig );
#else
    return ::kill( pid, sig );
#endif
#endif
}

bool
OFOS::isalphabetic( char c )
{
	return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' );
}

bool 
OFOS::isnumeric (char c)
{
	return ( c >= '0' && c <= '9' );
}

bool 
OFOS::isalphanumeric (char c)
{
	return OFOS::isalphabetic( c ) || OFOS::isnumeric( c );
}

void
OFOS::inet_ntoa(struct in_addr in, char* buffer, ofuint32 bufferLength)
{
#if defined(OFOPSYS_WIN32)
    OFOS::strncpy2(buffer, ::inet_ntoa(in), bufferLength);
#else
    inet_ntop(AF_INET,
              &in,
              buffer,
              bufferLength);
#endif // defined(OFOPSYS_WIN32)
}

int OFOS::sscanf(const char *str, const char *format, ...)
{
    va_list arglist;
    va_start(arglist, format);
#if defined(OFOPSYS_WIN32)
    int ret = ::sscanf(str, format, arglist);
#else
    int ret = ::sscanf(str, format, arglist);
#endif
    va_end( arglist );
    return ret;
}
