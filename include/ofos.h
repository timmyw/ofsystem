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

#if !defined(_OF_OS_H_)
#define _OF_OS_H_

#if defined(OFOPSYS_LINUX)
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/** \file ofos.h
    Contains the declaration of the OFOS class.
*/

//#include <ofsys/oflimit.h>

#define ofstrlen(a) ::strlen(a)
#define ofstrcmp(a,b) ::strcmp(a,b)
#define ofstrcpy(a,b) ::strcpy(a,b)
#define ofstrchr(src,ch) ::strchr(src,ch)
#define ofstrrchr(src,ch) ::strrchr(src,ch)
#define ofstrcat(dest,cat) ::strcat(dest,cat)

#if defined(OFOPSYS_WIN32)
#define ofstricmp(a,b) ::stricmp(a,b)
#define ofstrnicmp(a,b,c) ::strnicmp(a,b)
#else
#define ofstricmp(a,b) strcasecmp(a,b)
#define ofstrnicmp(a,b,c) strncasecmp(a,b,c)
#endif

/** \class OFOS ofos.h
    Wraps operating system standard C calls. All methods in this
    class are static and the class itself should never be instantiated.

    \nosubgrouping
    \ingroup ofsyscls
*/
class OFSYS_API OFOS
{
 public:

    /** \name Type definitions.
     */
    //@{

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    typedef ofint32 of_socket_t;
    enum { OF_INVALID_SOCKET = -1 };
    typedef socklen_t of_socklen_t;
    typedef ofint32 of_handle_t;
    typedef pid_t of_pid_t;
    typedef ssize_t of_ssize_t;
    typedef time_t of_time_t;
    typedef struct timeval of_timeval_t;
    typedef struct stat of_stat;
#elif defined(__CYGWIN__)
    typedef SOCKET of_socket_t;
    enum { OF_INVALID_SOCKET = -1 };
    typedef OFINT32 of_socklen_t;
    typedef HANDLE of_handle_t;
    typedef ofuint32 of_pid_t;
    typedef ssize_t of_ssize_t;
#elif defined(OFOPSYS_WIN32)
    typedef SOCKET of_socket_t;
    enum { OF_INVALID_SOCKET = INVALID_SOCKET };
    typedef int of_socklen_t;
    typedef HANDLE of_handle_t;
    typedef ofuint32 of_pid_t;
    typedef size_t of_ssize_t;
    typedef time_t of_time_t;
    typedef SYSTEMTIME of_timeval_t;
    typedef struct _stat of_stat;
#endif

    /* This must be modified for the correct size. */
    typedef ofuint64 of_ipv6_t;

    typedef size_t of_size_t;

    typedef ofuint64 of_ts_t;

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_DARWIN)
    typedef pthread_key_t of_tls_t;
    typedef pthread_t of_thread_t;
    typedef void*(*of_thrfunc_t)(void *);
#endif

#if defined(OFOPSYS_WIN32)
    typedef ofuint32 of_tls_t;
    typedef HANDLE of_thread_t;
    typedef DWORD(__stdcall *of_thrfunc_t)(void *);
#endif

    //@}

#if defined(QT_VERSION)
    // bastard Qt redefines all these
#   undef stricmp
#   undef strnicmp
#   undef strcmp
#   undef strlen

#endif // #if defined(QT_VERSION)


    /** \name String methods.
     */
    //@{

    /** strcmp
     */
    static ofint32 strcmp( const char *one, const char *two );
    /** stricmp
     */
    static ofint32 stricmp( const char *one, const char *two );

    /** strncmp
     */
    static ofint32 strncmp( const char *one, const char *two, size_t count );

    /** strnicmp
     */
    static ofint32 strnicmp( const char *one, const char *two, size_t count );

    /** OS strcpy
     */
    static char *strcpy( char *dest, const char *src );

    /** OS     strncpy
        This version of strncpy automatically adds the null-terminator.
    */
    static char *strncpy( char *dest, const char *src, size_t count );

    /** strncpy2
        Performs a strncpy on the supplied parameters, using minimum<> on
        the supplied buffer length and the length of \c src to 
        determine how much to copy.
        \param dest The destination buffer.
        \param src The null-terminated string to copy.
        \param count The size of the supplied buffer.
        \return A pointer to \c dest.
    */
    static char *strncpy2( char *dest, const char *src, size_t count );

    /** OS strstr
     */
    static const char * strstr( const char *src, const char *sub );

    /** OS strchr
     */
    static const char *strchr( const char *src, ofint32 ch );

    /** OS strrchr
     */
    static const char *strrchr( const char *src, ofint32 ch );

    /** OS strlen
     */
    static of_size_t strlen( const char *src );

    /** OS strcat
     */
    static char * strcat( char *dest, const char *cat );

    /** OS strupr
        This method is actually an implementation in itself. Not many
        standard libraries provide a string upper function.
        \param instr A null-terminated string to be converted to upper case.
        \return A pointer to \c instr.
    */
    static char *strupr( char *instr );

    /** OS strlwr.
        Again an implementation.
        \param instr A null-terminated string to be converted to lower case.
        \return A pointer to \c instr.
    */
    static char *strlwr( char *instr );

    //@}

    /** \name Memory functions
     */
    //@{

    /** Memcmp implementation
     */
    static int memcmp(const void* v1, const void* v2, size_t c);

    //@}

    /** \name Scan functions
     */
    //@{

    static int sscanf(const char *str, const char *format, ...);

    //@}

    /** \name Miscellaneous system calls.
     */
    //@{

    /** OS mktemp
     */
    static char * mktemp( const char *mask, char *buffer, char *prefix = NULL );

    /** OS random function.
     */
    static ofuint32 random( );

    /** snprintf
     */
    static int snprintf( char *buffer, ofuint32 bufferSize, const char *format, ... );

    /** vsnprintf
    */
    static int vsnprintf( char *buffer, ofuint32 bufferSize, const char *format, ... );

    //@}

    /** \name Conversion
     */
    //@{

    /** 32-bit atol
     */
    static ofint32 atol (const char*);

    /** 64-bit atol
     */
    static ofint64 atoll (const char *str);

	/** return true if c is A-Z or a-z
		(renamed from isalpha as this is a macro on some platforms)
     */
    static bool isalphabetic (char c);

	    /** return true if c is 0-9
		(renamed from isdigit as this is a macro on some platforms)
     */
    static bool isnumeric (char c);

    /** return true if c is A-Z,a-z or 0-9
		(renamed from isalnum as this is a macro on some platforms)
     */
    static bool isalphanumeric (char c);


    /** PI
     */
    static const double PI;

    /** E
     */
    static const double E;
    
    //@}

    /** \name File IO
     */
    //@{

    /** File open.
     */
    static OFOS::of_handle_t openFile( const char *name, ofuint32 mode, ofuint32 flags );

    /** File close.
     */
    static void closeFile( OFOS::of_handle_t fh );

    //@}

    /** \name Network and host order methods
     */
    //@{

    /** Host to network 16 bit
     */
    static ofuint16 hton16( ofuint16 x );

    /** Host to network 32 bit
     */
    static ofuint32 hton32( ofuint32 x );

    /** Host to network 64 bit
     */
    static ofuint64 hton64( ofuint64 x );

    /** Network to host 16 bit
     */
    static ofuint16 ntoh16( ofuint16 x );

    /** Network to host 32 bit
     */
    static ofuint32 ntoh32( ofuint32 x );

    /** Network to host 64 bit
     */
    static ofuint64 ntoh64( ofuint64 x );

    //@}

    /** \name Network address
     */
    //@{

    /** Retrieves the dot notation form of the supplied address.

    For Win32 platforms, uses the inet_ntoa, which is thread safe, and
    on other platforms uses inet_ntop.
    */
    static void inet_ntoa(struct in_addr in, char* buffer, ofuint32 bufferLength);

    //@}

    /** \name Process and signal
    */
    //@{

    /** OS sleep
     */
    static void os_sleep( ofuint32 seconds );

    /** OS getpid
     */
    static OFOS::of_thread_t getpid( );

    /** OS kill/raise
    */
    static int kill( OFOS::of_thread_t pid, int sig );

    //@}

    /** Time handling
     */
    //@{

    static of_ts_t getnow( );

    static void getsystemtime(OFOS::of_timeval_t* t);

    //@}
};

// Only include inline stuff for release
#if defined(NDEBUG)
#include <ofos.i>
#endif

#endif // #if !defined(_OF_OS_H_)
