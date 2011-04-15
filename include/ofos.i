/*
* $OFPREPARED$
*
* $Revision: 2.3 $
*
* $Id: ofos.i,v 2.3 2008-03-13 17:36:42 timmy Exp $
*/

#if defined(__GNUC__)
#if defined(__GLIBCPP__) && __GLIBCPP__ >= 20001117
#include <ctype.h>
#endif
#endif

#include <stdlib.h>
#include <sys/timeb.h>

OFINLINE
ofint32 
OFOS::strcmp( const char *one, const char *two )
{
	return ::strcmp( one, two );
}

OFINLINE
ofint32 
OFOS::stricmp( const char *one, const char *two )
{
#if defined(OFOPSYS_WIN32)
	return ::stricmp( one, two );
#else
	return ::strcasecmp( one, two );
#endif // OFOPSYS_WIN32
}

OFINLINE
ofint32  
OFOS::strncmp( const char *one, const char *two, size_t count )
{
	return ::strncmp( one, two, count );
}

OFINLINE
ofint32  
OFOS::strnicmp( const char *one, const char *two, size_t count )
{
#if defined(OFOPSYS_WIN32)
	return ::strnicmp( one, two, count );
#else
	return ::strncasecmp( one, two, count );
#endif // OFOPSYS_WIN32
}

OFINLINE
char *
OFOS::strcpy( char *dest, const char *src )
{
	assert( src );
	assert( dest );
	return ::strcpy( dest, src );
}

OFINLINE
char *
OFOS::strncpy( char *dest, const char *src, size_t count )
{
    assert( src );
    assert( dest );
    char *ret = ::strncpy( dest, src, count );
    dest[count] = 0;
    return ret;
}

OFINLINE
char *
OFOS::strncpy2( char *dest, const char *src, size_t count )
{
    assert( src );
    assert( dest );
    ofuint32 end = minimum<ofuint32>( count, OFOS::strlen( src ) );
    return OFOS::strncpy( dest, src, end );
}

OFINLINE
const char * 
OFOS::strstr( const char *src, const char *sub )
{
	assert( src );
	assert( sub );
	return ::strstr( src, sub );
}

OFINLINE
OFOS::of_size_t 
OFOS::strlen( const char *src )
{
	assert( src );
	return ::strlen( src );
}

OFINLINE
const char *
OFOS::strchr( const char *src, ofint32 ch )
{
	assert( src );
	return ::strchr( src, ch );
}

OFINLINE
const char *
OFOS::strrchr( const char *src, ofint32 ch )
{
    assert(src);
    return ::strrchr(src, ch);
}

OFINLINE
char * 
OFOS::strcat( char *dest, const char *cat )
{
	assert( dest );
	assert( cat );
	return ::strcat( dest, cat );
}

OFINLINE
char *
OFOS::strupr( char *instr )
{
    assert( instr );
    char *pos = instr;
    while ( *pos )
    {
        *pos = toupper( *pos );
        pos++;
    }
    return instr;
}

OFINLINE
char *
OFOS::strlwr( char *instr )
{
    assert( instr );
    char *pos = instr;
    while ( *pos )
    {
        *pos = tolower( *pos );
        pos++;
    }
    return pos;
}

OFINLINE
void
OFOS::os_sleep( ofuint32 seconds )
{
#if defined(OFOPSYS_WIN32)
        Sleep (seconds * 1000);
#else
        ::sleep (seconds);
#endif
}

OFINLINE
OFOS::of_thread_t
OFOS::getpid( )
{
#if defined(OFOPSYS_WIN32)
    return (OFOS::of_thread_t)_getpid( );
#else
#if defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    return pthread_self();
#else
    return ::getpid( );
#endif
#endif
}

/*static*/
OFINLINE
ofuint32
OFOS::random( )
{
#if defined(OFOPSYS_WIN32)
    return ::rand( );
#else
    return ::random( );
#endif
}

OFINLINE
ofint32
OFOS::atol (const char* str)
{
        return ::atol (str);
}

OFINLINE
ofint64 
OFOS::atoll (const char *str)
{
#if defined(OFOPSYS_WIN32)
        return ::_atoi64( str );
#else
#if defined(OFOPSYS_FREEBSD)
        return (ofint64)::atof( str );
#else
        return ::atoll( str );
#endif
#endif
}


OFINLINE
ofuint16
OFOS::hton16( ofuint16 x )
{
    return htons( x );
}

OFINLINE
ofuint32
OFOS::hton32( ofuint32 x )
{
    return htonl( x );
}

OFINLINE
ofuint64
OFOS::hton64( ofuint64 x )
{
#if (OF_BYTE_ORDER==OF_BIG_ENDIAN)
    return x;
#else
    return ( ((ofuint64)(htonl((x) & 0xffffffff)) << 32) | htonl(((x) >> 32) & 0xffffffff ) );
#endif
}

OFINLINE
ofuint16
OFOS::ntoh16( ofuint16 x )
{
    return ntohs( x );
}

OFINLINE
ofuint32
OFOS::ntoh32( ofuint32 x )
{
    return ntohl( x );
}

OFINLINE
ofuint64
OFOS::ntoh64( ofuint64 x )
{
#if (OF_BYTE_ORDER==OF_BIG_ENDIAN)
    return x;
#else
    return OFOS::hton64( x );
#endif
}

OFINLINE
OFOS::of_ts_t
OFOS::getnow( )
{
#if defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN) || defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS)
    struct timeval tp;
    struct timezone tz;
    gettimeofday( &tp, &tz );
    return (OFOS::of_ts_t)( tp.tv_sec * 60 - tz.tz_minuteswest) * 1000 + tp.tv_usec;
#else
    struct timeb b;

    ftime( &b );
    OFOS::of_ts_t n = ( b.time - b.timezone * 60 ) * 1000 + b.millitm ;
    return n;
#endif
}

OFINLINE
int
OFOS::memcmp(const void* v1, const void* v2, size_t c)
{
    return ::memcmp(v1, v2, c);
}

OFINLINE
/*static*/
void
OFOS::getsystemtime(OFOS::of_timeval_t* t)
{
#if defined(OFOPSYS_WIN32)
    GetSystemTime(t);
#else
    gettimeofday(t,0);
#endif // defined(OFOPSYS_WIN32)
}

