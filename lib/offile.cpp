/*
 * Created            : Apr-00 by Timmy Whelan
 * Last u.test update : dd/mm/yy by ------
 * Last code review   : dd/mm/yy by ------

 $Revision: 2.3 $

 $Id: offile.cpp,v 2.3 2010-03-21 17:37:03 timmy Exp $
*/

#include <ofsys/ofsys.h>
#include <ofsys/offile.h>
#include <ofsys/ofutility.h>

#if defined(unix) | defined(linux) | defined(__CYGWIN__)
#include <unistd.h>
#   include <sys/stat.h>
#endif

#if defined(OFOPSYS_WIN32)
#include <io.h>
#include <errno.h>
#endif


char OFFile::m_tempPath[OF_MAX_PATH_LEN+1] = "";

void OFFile::make_unique(char *filename)
{
    char name[OF_MAX_PATH_LEN+1], testname[OF_MAX_PATH_LEN+1];
    char ext[OF_MAX_PATH_LEN+1];
    split_name_ext(filename, name, ext);
    ofuint32 j = 0;
    OFOS::snprintf(testname, OF_MAX_PATH_LEN
                   , "%s.%s", name, ext);
    while (OFFile::doesFileExist(testname)) {
        j++;
        OFOS::snprintf(testname, OF_MAX_PATH_LEN
                       , "%s-%04"DFORMAT2".%s"
                       , name, j, ext);
    }
    OFOS::strcpy(filename, testname);
}

void OFFile::split_name_ext(const char* full
                            , char* name
                            , char* ext)
{
    string fn, e;
    OFUtility::split_string_on_last_delim(full, ".", fn, e);
    OFOS::strcpy(name, fn.c_str());
    OFOS::strcpy(ext, e.c_str());
}

bool
OFFile::doesPathExist(const char* path)
{
    struct stat s;
    int r = stat(path, &s);
    if (r == -1 && errno == ENOENT)
        return false;
    return true;
}

ofuint32
OFFile::type(const char *path)
{
    struct stat s;
    int r = stat(path, &s);
    if (r == -1 && errno == ENOENT)
        return 0;
    
#if defined(OFOPSYS_WIN32)
    if (s.st_mode & 0x4000)
        return OFFILE_FILETYPE_DIR;
#else
    if (S_ISDIR(s.st_mode))
        return OFFILE_FILETYPE_DIR;
#endif
    return OFFILE_FILETYPE_FILE;
}

bool OFFile::doesFileExist (const char *path)
{
#if defined(OFOPSYS_WIN32)
    bool found = false;
    OFFile file( path, 0664, OFFILE_READONLY | OFFILE_OPEN_EXISTING );
    found = file.isOpen( );
    if ( found )
        file.close( );
    return found;
#else
     return ( access( path, F_OK ) == 0 );
#endif
}

OFFile::OFFile() :
    m_name( 0 ),
    m_handle(0),
    m_open(false)
	{
}

OFFile::OFFile( const char *fileName /* = NULL */,ofint32 mode /* = 0664*/,ofint32 flags /*= OFFILE_OPEN_ALWAYS | OFFILE_READWRITE */ ) :
    m_name( 0 ),
    m_handle( 0 ),
    m_open( false )
{
    open( fileName, mode, flags );
}

OFFile::OFFile( const OFFile &copy )
{
    m_open = copy.m_open;
    m_handle = copy.m_handle;
    m_name = new char[ OFOS::strlen( copy.m_name )+1];
    OFOS::strcpy( m_name, copy.m_name );
}

OFFile::~OFFile( )
{
    close( );
    if ( m_name )
    {
        delete [] m_name;
    }
}

ofint32 
OFFile::open( const char *fileName /* = NULL */,ofint32 mode /* = 0664*/,ofint32 flags /*= OFFILE_OPEN_ALWAYS | OFFILE_READWRITE */  )
{
    close( );
    ofint32 retval = 1;

    if ( m_name )
    {
        delete [] m_name;
        m_name = 0;
    }

    if ( !fileName )
    {
#if !defined( NDEBUG )
        if ( m_tempPath[0] == 0 )
            cout << "WARNING: temp path not set for OFFile" << endl;
#endif
        char tempName[OF_MAX_PATH_LEN+1];
        sprintf( tempName, "%s/offileXXXXXX", m_tempPath );

#if defined(OFOPSYS_WIN32)
        _mktemp( tempName );
        m_name = new char[OFOS::strlen( tempName )+4];
        OFOS::strcpy( m_name, tempName );

        m_handle = CreateFile( m_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
        if ( m_handle != INVALID_HANDLE_VALUE )
        {
            m_open = true;
        }

        if ( !m_open )
        {
            retval = 0;
        }
        else
        {
            retval = GetLastError( );
        }
        
#else // OFOPSYS_WIN32
        m_handle = mkstemp( tempName );
        if ( m_handle != -1 )
        {
            m_name = new char[ OFOS::strlen( tempName )+4 ];
            OFOS::strcpy( m_name, tempName );
            retval = 0;
	    m_open = true;
        }
#endif // !OFOPSYS_WIN32
    }
    else
#if defined(OFOPSYS_WIN32)
    {
        m_name = new char[OFOS::strlen( fileName )+4];
        OFOS::strcpy( m_name, fileName );

        ofuint32 accflags = ( flags & (OFFILE_READONLY|OFFILE_READWRITE) ) ? GENERIC_READ : 0;
        accflags |= ( flags & (OFFILE_WRITEONLY|OFFILE_READWRITE) ) ? GENERIC_WRITE : 0;
        ofuint32 dispflags = (( flags & OFFILE_CREATE_ALWAYS)?CREATE_ALWAYS:0) | ((flags & OFFILE_OPEN_ALWAYS)?OPEN_ALWAYS:0) | ((flags & OFFILE_OPEN_EXISTING)?OPEN_EXISTING:0); 
        ofuint32 shareflags = FILE_SHARE_READ | ((flags & OFFILE_READWRITE )?FILE_SHARE_WRITE:0); 
		if ( flags & OFFILE_SHAREALL )
				shareflags |= FILE_SHARE_READ | FILE_SHARE_WRITE;

        m_handle = CreateFile( m_name, accflags, shareflags, NULL, dispflags, FILE_ATTRIBUTE_NORMAL, NULL );
        if ( m_handle != INVALID_HANDLE_VALUE )
            m_open = true;

        if ( m_open )
            retval = 0;
        else
            retval = GetLastError( );
    }
#else
    {
        m_name = new char[OFOS::strlen( fileName )+4];
        OFOS::strcpy( m_name, fileName );
        ofint32 nflags = 0;
        if ( flags & OFFILE_READONLY ) nflags |= O_RDONLY;
        if ( flags & OFFILE_WRITEONLY ) nflags |= O_WRONLY;
        if ( flags & OFFILE_READWRITE ) nflags |= O_RDWR;
        if ( flags & OFFILE_CREATE_ALWAYS ) nflags |= (O_TRUNC | O_CREAT);
        if ( flags & OFFILE_OPEN_ALWAYS) nflags |= O_CREAT;
        m_handle = ::open( m_name, nflags, mode );

        if ( m_handle > 0 )
        {
            m_open = true;
            retval = 0;
        }
        else
        {
            retval = errno;
            m_handle = 0;
        }
    }
#endif // #if defined(OFOPSYS_WIN32)

    return retval;
}

ofint32 
OFFile::close( )
{
    if ( !m_open )
        return 1;

#if defined(OFOPSYS_WIN32)
    BOOL b = CloseHandle( m_handle );
    if ( b )
        m_open = false;
    return b;
#else
    ofint32 r = ::close( m_handle );
    if ( !r )
        m_open = false;
    return ( !m_open );
#endif
}

ofint32 
OFFile::read(void *buffer, ofuint32 length)
{
    ofint32 bytesRead = -1;
  
#if defined(OFOPSYS_WIN32)
    ofuint32 actual;
    BOOL ok  = ReadFile( m_handle, buffer, length, &actual, NULL ) ;
    //printf( "error:%d\n", GetLastError() );
    if ( ok )
        bytesRead = actual;
#else
    bytesRead = ::read( m_handle, buffer, length );
#endif // #if defined(OFOPSYS_WIN32)

    return bytesRead;
}

ofint32 
OFFile::write( const void * buffer, ofuint32 length )
{

#if defined(OFOPSYS_WIN32)
    ofuint32 actual;
    bool ok = WriteFile( m_handle, buffer, length, &actual, NULL ) != 0;
    if ( ok )
        return actual;
    else
        return -1;
#else
    return ::write( m_handle, buffer, length );
#endif // #if defined(OFOPSYS_WIN32)

}

ofint32 OFFile::write(const char* line)
{
    ofint32 c = write(line, OFOS::strlen(line));
    c += write("\n", OFOS::strlen("\n"));
    return c;
}

ofint32 
OFFile::position( ofuint32 newpos,ofint32 from /* = OFFILE_POSITION_BEGIN */ )
{

#if defined(OFOPSYS_WIN32)
    ofuint32 move = FILE_BEGIN;
    if ( from == OFFILE_POSITION_END ) move = FILE_END;
    if ( from == OFFILE_POSITION_CURRENT ) move = FILE_CURRENT;
    return SetFilePointer( m_handle, newpos, NULL, move );
#else
    ofint32 origin = SEEK_SET;
    if ( from == OFFILE_POSITION_END ) origin = SEEK_END;
    if ( from == OFFILE_POSITION_CURRENT ) origin = SEEK_CUR;
    return lseek( m_handle, newpos, origin );
#endif // #if defined(OFOPSYS_WIN32)
}

ofint32 
OFFile::position( )
{
#if defined(OFOPSYS_WIN32)
    return SetFilePointer( m_handle, 0, NULL, FILE_CURRENT );
#else
    return lseek( m_handle, 0, SEEK_CUR );
#endif // #if defined(OFOPSYS_WIN32)
}

ofuint32 
OFFile::size() const
{
#if defined(OFOPSYS_WIN32)
    return GetFileSize( m_handle, NULL );
#else
    struct stat filestat;
    if ( lstat( m_name, &filestat) )
        return 0;
  
    return filestat.st_size;
#endif // #if defined(OFOPSYS_WIN32)

}

int
OFFile::truncate( )
{
#if defined(OFOPSYS_WIN32)
    SetFilePointer( m_handle, 0, NULL, FILE_BEGIN );
    SetEndOfFile( m_handle );
    return 0;
#else
    char tname[OF_MAX_PATH_LEN+1];
    OFOS::strncpy2( tname, m_name, OF_MAX_PATH_LEN );
    close( );
    int ret = ::truncate( tname, 0 );
    open( tname );
    return ret;
#endif // #if defined(OFOPSYS_WIN32)
}

bool
OFFile::isOpen() const
{
    return m_open;
}

const OFOS::of_handle_t
OFFile::handle() const
{
    return m_handle;
}

void
OFFile::handle( OFOS::of_handle_t handle )
{
    close( );
    m_handle = handle;
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    struct stat buf;
    if ( fstat( m_handle, &buf ) == 0 )
        m_open = true;
#elif defined(OFOPSYS_WIN32)
    ofuint32 hi;
    if ( GetHandleInformation( m_handle, &hi ) )
        m_open = true;
#else
#error Undefined platform
#endif
}

const char *
OFFile::name( ) const
{
    return m_name;
}

ofint32 
OFFile::type( )
{
    ofint32 t = 0;
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    struct stat statbuf;
    ofint32 rc = fstat( m_handle, &statbuf );
    if ( rc == -1 )
        t = -1;
    if ( S_ISDIR(statbuf.st_mode) )
        t = OFFILE_FILETYPE_DIR;
    if ( S_ISREG(statbuf.st_mode) )
        t = OFFILE_FILETYPE_FILE;
#elif defined(OFOPSYS_WIN32) && !defined(__CYGWIN__)
    BY_HANDLE_FILE_INFORMATION fi;
    GetFileInformationByHandle( m_handle, &fi );
    
    if ( fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        t = OFFILE_FILETYPE_DIR;
    if ( ( fi.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) || ( fi.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) ||
         ( fi.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) )
        t = OFFILE_FILETYPE_FILE;
#else
#error Undefined platform
#endif
    return t;
}

#if defined(OFOPSYS_WIN32)
ofint32  
OFFile::getExtFileInfo( char* file, OFOS::of_stat *buf )
{
    ofint32 result = 1;

    result = _stat( file , (struct _stat*)buf ) ;

    return result ;
}
#else
ofint32  
OFFile::getExtFileInfo( char* file, OFOS::of_stat *buf )
{
    ofint32 result = 1;

    result = stat( file , (struct stat*)buf ) ;

    return result ;
}
#endif // #defined (OFOPSYS_WIN32)

/* static */
void
OFFile::setTempPath( const char *newPath )
{
    OFOS::strncpy2( m_tempPath, newPath, OF_MAX_PATH_LEN );
}

/* static */
const char *
OFFile::getTempPath( )
{
    return m_tempPath;
}

#if defined(UNIT_TEST)
#   include <iostream>
#   include "UnitTest.h"

ofint32  main(ofint32 c, char *v[] )
{

    UT_BATCH_START( "OFFile" );
    
    UT_TEST_START( 1, "OFFile() create file" );

    OFFile *file = new OFFile( "test.txt", 0664, OFFile::OFFILE_CREATE_ALWAYS | OFFile::OFFILE_READWRITE );
    UT_EVAL_EXPR( 1, "OFFile() create file", ( file->isOpen() ) );

    UT_TEST_END( 1 );

    UT_TEST_START( 2, "OFFile::write()" );
    ofint32 rc = file->write( "hello\n", strlen("hello\n" ) );
    UT_EVAL_EXPR( 2, "OFFile::write()", ( rc == strlen("hello\n" ) ) );
    UT_TEST_END( 2 );

    UT_TEST_START( 3, "OFFile::position()" );
    UT_EVAL_EXPR( 3, "OFFile::position()", ( file->position( 0 ) == 0 ) );
    UT_TEST_END( 3 );

    UT_TEST_START( 4, "close()" );
    rc = file->close( );
    UT_EVAL_EXPR( 4, "close()", ( rc ) );
    UT_EVAL_EXPR( 4, "close()", (file->isOpen() == false ) );
    UT_TEST_END( 4 );

    UT_TEST_START( 5, "reopen via open()" );
    OFFile *file2 = new OFFile;
    rc = file2->open( "test.txt", 0664, O_RDWR );
    UT_EVAL_EXPR( 5, "reopen via open() return", ( rc ) );
    UT_EVAL_EXPR( 5, "reopen via open()", ( file2->position() == 0 ) );
    UT_EVAL_EXPR( 5, "reopen via open()", ( file2->isOpen() ) );
    UT_TEST_END( 5 );

    UT_TEST_START( 6, "OFFile::read()" );
    char temp[200];
    memset( temp, 0, 199 );
    rc = file2->read( temp, 199 );
    cout << "[" << temp << "]" << endl;
    UT_EVAL_EXPR( 6, "OFFile::read()", ( rc == strlen("hello\n") ) );
    UT_EVAL_EXPR( 6, "OFFile::read()", !strcmp( temp, "hello\n" ) );
    UT_TEST_END( 6 );

    UT_BATCH_END( "OFFile" );

    delete file2;
    delete file;

    return 0;
}

#endif
