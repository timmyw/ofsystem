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
#include <ofplatform.h>
#include <ofos.h>
#include <ofconfig.h>
#include <offile.h>
#include <ofutility.h>
#include <ofdirent.h>

#if defined(linux)
extern char *cuserid(char *);
#endif

#if defined(OFOPSYS_WIN32)
#include <direct.h>
const char OFPlatform::PathSeparator = '\\';
const char OFPlatform::PathSeparatorS[2] = "\\";
const char *OFPlatform::sharedLibExt = ".dll";
const char *OFPlatform::pathDelimiter = ";";
const char *OFPlatform::CRLF = "\r\n";
#else
const char OFPlatform::PathSeparator = '/';
const char OFPlatform::PathSeparatorS[2] = "/";
const char *OFPlatform::sharedLibExt = ".so";
const char *OFPlatform::pathDelimiter = ":";
const char *OFPlatform::CRLF = "\n";
#endif // #if defined(OFOPSYS_WIN32)

void
OFPlatform::fixPath( string & path )
{
#if defined(OFOPSYS_WIN32)
    char badsep = '/';
#else
    char badsep = '\\';
#endif // #if defined(OFOPSYS_WIN32)
  
    char * temp = new char[path.size()+1];
    strcpy( temp, path.c_str() );
  
    // First find only the incorrect separators
    char * pos = temp;  
    while ( *pos )
    {
        if ( *pos == badsep )
            *pos = OFPlatform::PathSeparator;
        pos++;
    }
  
    // Now remove any duplicates.
    pos = temp;
    while ( *pos )
    {
        if ( *pos == OFPlatform::PathSeparator &&
             *(pos+1) == OFPlatform::PathSeparator )
        {
            // remove one of the slashes
            char temp2[1024];
            strcpy( temp2, pos+1 );
            strcpy( pos, temp2 );
        }
        else
            pos++;
    }
    path = temp;
    delete [] temp;
}

void
OFPlatform::fixPath( char * path )
{
#if defined(OFOPSYS_WIN32)
    char badsep = '/';
#else
    char badsep = '\\';
#endif // #if defined(OFOPSYS_WIN32)
  
    // First find only the incorrect separators
    char * pos = path;
    while ( *pos )
    {
        if ( *pos == badsep )
            *pos = OFPlatform::PathSeparator;
        pos++;
    }
  
    // Now remove any duplicates.
    pos = path;
    char *temp = new char[OFOS::strlen( path ) + 1];
    char *cur = temp;

    while ( *pos )
    {
        if ( !( *pos == OFPlatform::PathSeparator && *(pos+1) == OFPlatform::PathSeparator ) )
            *cur++ = *pos++;
        else
            pos++;
    }
    *cur = 0;
    OFOS::strcpy( path, temp );
    delete [] temp;
}

void
OFPlatform::fixPath (char * path, char unix_dos)
{
        char badsep = '/';
        char goodsep = '\\';
        if (unix_dos)
        {
                badsep='\\';
                goodsep='/';
        }
  
        // First find only the incorrect separators
        char * pos = path;
        while ( *pos )
        {
                if ( *pos == badsep )
                        *pos = goodsep;
                pos++;
        }
  
        // Now remove any duplicates.
        pos = path;
        char *temp = new char[OFOS::strlen( path ) + 1];
        char *cur = temp;

        while ( *pos )
        {
                if ( !( *pos == goodsep && *(pos+1) == goodsep ) )
                        *cur++ = *pos++;
                else
                        pos++;
        }
        *cur = 0;
        OFOS::strcpy( path, temp );
        delete [] temp;
}

/* static */
void
OFPlatform::manglePath( char *path, char replace /* = '_' */ )
{
        char *temp = new char[OFOS::strlen( path ) + 1];
        char *cur = temp;
        char *pos = path;
        while ( *pos )
        {
                if ( OFOS::strchr( "!\"$%^&*()?@", *pos ) )
                {
                        *cur++ = replace;
                        pos++;
                }
                else
                        *cur++ = *pos++;
        }
        *cur = 0;
}

void 
OFPlatform::configPath( char *path )
{
        char *lp = getenv( "OFCONFIGPATH" );
        if ( lp )
        {
                OFOS::strcpy( path, lp );
                return;
        }
        lp = getenv( "OPENFABRIC" );
        if ( lp )
        {
                OFOS::strcpy( path, lp );
                return;
        }
#if defined(OFOPSYS_WIN32)
        char winPath[MAX_PATH+1];

        if ( GetEnvironmentVariable( "WINDIR", winPath, MAX_PATH ) )
                OFOS::strcpy( path, winPath );
        else
                OFOS::strcpy( path, "C:\\" );

#else
        if ( ::getenv( "HOME" ) )
                OFOS::strcpy( path, ::getenv( "HOME" ) );
        else
                path[0] = 0;
        if ( !OFOS::strlen( path ) )
        {
                OFOS::strcpy( path, "/home" );
                char *temp = ::getenv( "LOGNAME" );
                OFOS::strcat( path, temp );
        }
        if ( !OFOS::stricmp( path, "/home" ) )
                OFOS::strcpy( path, "/etc" );
#endif
  
}

void
OFPlatform::logPath( char * logpath )
{
        char *lp = getenv( "OFLOGPATH" );
        if ( lp )
        {
                OFOS::strcpy( logpath, lp );
                return;
        }
        lp = getenv( "OPENFABRIC" );
        if ( lp )
        {
                char temp[1024];
                sprintf( temp, "%s/openfabric.conf", lp );
                OFConfigurator conf( temp, true, false );
        
                conf.readValue( "openfabric", "logpath", temp, "" );

                if ( OFOS::strlen( temp ) )
                {
                        OFOS::strcpy( logpath, temp );
                        return;
                }
        }
#if defined(OFOPSYS_WIN32)
        char winPath[MAX_PATH+1];
        // Try the registry. Ukkkkk!!!!

        HKEY hkey;
        ofuint32 dwDisp;
        if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Suntail.com\\openFabric\\Server", 0, "REG_SZ", 0, KEY_QUERY_VALUE, NULL, &hkey, &dwDisp ) == ERROR_SUCCESS )
        {
                ofint32 len=MAX_PATH;
                if ( RegQueryValue( hkey, "LogPath", winPath, &len ) == ERROR_SUCCESS )
                {
                        OFOS::strcpy( logpath, winPath );
                }
        }
        else
        {
                if ( GetEnvironmentVariable( "WINDIR", winPath, MAX_PATH ) )
                        OFOS::strcpy( logpath, winPath );
                else
                        OFOS::strcpy( logpath, "C:\\" );
        }
#else
        OFOS::strcpy( logpath, "/var/log" );
#endif
}

void
OFPlatform::homeDirectory( char *directory )
{
        const char *homeDir = NULL;
#if !defined(OFOPSYS_WIN32)
        homeDir = getenv( "HOME" );
#else
        homeDir = getenv( "USERPROFILE" );
#endif
        if ( homeDir )
                OFOS::strcpy( directory, homeDir );
}

void
OFPlatform::validSharedLib( string &libName )
{
#if defined(OFOPSYS_WIN32)

    if (libName.find( ".dll" ) == string::npos && libName.find( ".DLL" ) == string::npos)
    {
        libName += ".dll";
    }
    
#else

    if ( (libName.find( ".so" ) == string::npos) && (libName.find( ".SO" ) == string::npos) )
    {
        libName += ".so";
    }

#endif // #if defined(OFOPSYS_WIN32)
}

bool
OFPlatform::doesFileExist( const char * fileName )
{
        /* Check to see if the file exists */
        bool found = false;
        OFFile file( fileName, 0664, OFFILE_READONLY | OFFILE_OPEN_EXISTING );
        found = file.isOpen( );
        if ( found )
                file.close( );
        return found;
}

ofint32  
OFPlatform::deleteFile( const char *fileName )
{
#if defined(OFOPSYS_WIN32)
    return (DeleteFile(fileName) == TRUE)?0:-1;
#else
    return remove( fileName );
#endif

}

ofint32
OFPlatform::copyFile( const char* dest, const char* src, char deleteSrc )
{
        OFFile srcFile (src, 0664, OFFILE_OPEN_EXISTING|OFFILE_READONLY);
        if (!srcFile.isOpen())
                return ERR_FILE_OPEN_ERR;
        OFFile destFile (dest, 0664, OFFILE_CREATE_ALWAYS|OFFILE_READWRITE);
        if (!destFile.isOpen())
                return ERR_FILE_OPEN_ERR;

#define bufsize 65536

        char buffer[bufsize];
        ofuint32 chunk = 0;
        while ((chunk = srcFile.read (buffer, bufsize))>0)
                destFile.write (buffer,chunk);
        if (deleteSrc)
                OFPlatform::deleteFile (src);
        return ERR_SUCCESS;
}

ofint32
OFPlatform::retrieveFileName( char *path, char *filename, ofuint32 length )
{
    char delim = OFPlatform::PathSeparator;

        OFPlatform::fixPath( path );
        const char *pos = OFOS::strrchr( path, delim );
        if ( pos && OFOS::strlen( pos+1 ) < length )
                OFOS::strcpy( filename, pos+1 );
        else
                OFOS::strcpy( filename, path );
        return 0;
}

ofint32
OFPlatform::retrieveFileName2 (const char *path, char *filename, ofuint32 length)
{
    char delim = OFPlatform::PathSeparator;

        char temp_path[OF_MAX_PATH_LEN+1];
        OFOS::strncpy2 (temp_path, path, OF_MAX_PATH_LEN);
        OFPlatform::fixPath (temp_path);
        const char *pos = OFOS::strrchr (temp_path, delim );
        if ( pos && OFOS::strlen( pos+1 ) < length )
                OFOS::strcpy( filename, pos+1 );
        else
                OFOS::strcpy( filename, temp_path );
        return 0;
}

/*static*/
ofint32
OFPlatform::retrieveDirectory( const char *path, char *directory, ofuint32 length, char fix_path /*=1*/, char force_delim /*=0*/ )
{
    char delim = OFPlatform::PathSeparator;

        if (force_delim)
                delim = force_delim;

        char *t = new char[OFOS::strlen( path ) + 1];
        OFOS::strcpy( t, path );
        if (fix_path)
                OFPlatform::fixPath (t);
        char* p = (char*)OFOS::strrchr (t, delim);
        if (p)
        {
                *(p+1) = 0;
        }
        OFOS::strcpy (directory, t);
        delete [] t;
        return 0;
}

string OFPlatform::get_filename_from_path(const string& path)
{
    string d, f;
    OFUtility::split_string_on_last_delim(path, OFPlatform::PathSeparatorS, d, f);
    return f;
}

string OFPlatform::get_ext_from_path(const string& path)
{
    string f, e;
    OFUtility::split_string_on_last_delim(path, ".", f, e);
    return e;
}

string OFPlatform::get_dir_from_path(const string& path)
{
    string d, f;
    OFUtility::split_string_on_last_delim(path, OFPlatform::PathSeparatorS, d, f);
    return d;
}

void OFPlatform::split_dir_filename_from_path(const string& path, string& dir, string& filename)
{
    OFUtility::split_string_on_last_delim(path, OFPlatform::PathSeparatorS, dir, filename);
}

void OFPlatform::split_filename_ext_from_path(const string& path, string& filename, string& ext)
{
    OFUtility::split_string_on_last_delim(path, ".", filename, ext);
}

ofint32
OFPlatform::mkdir (const char* dirname)
{
#if defined(OFOPSYS_WIN32)
        return CreateDirectory (dirname, NULL)?0:-1;
#else
        return ::mkdir(dirname, 0774);
#endif
}

ofint32
OFPlatform::rmtree_(const char* path)
{
    OFDirent dir(path);
    OFDIRENT de;
    while (dir.read(&de))
    {
        char new_path[OF_MAX_PATH_LEN+1];
        OFOS::snprintf(new_path, OF_MAX_PATH_LEN,
                       "%s/%s", path, de.name);
        OFPlatform::fixPath(new_path);
        switch (de.type)
        {
        case OFDIR_TYPE_DIR:
            if (OFOS::stricmp(de.name, ".") && OFOS::stricmp(de.name, ".."))
            {
                rmtree_(new_path);
            }
            break;
        case OFDIR_TYPE_FILE:
            OFPlatform::deleteFile(new_path);
            break;
        }
    }
    OFPlatform::rmdir(path);
    return 0;
}

ofint32
OFPlatform::rmtree(const char* path)
{
    return rmtree_(path);
}

ofint32
OFPlatform::mkpath(const char* path)
{
    char work_path[OF_MAX_PATH_LEN+1];
    RESETSTRING(work_path);
    char* token = 0;
    ofuint32 token_length = 0, start = 0;
    char separator[] = { OFPlatform::PathSeparator, 0 };
    while (OFUtility::str_token3(path, separator, "\"\"", &start, &token, &token_length))
    {
        OFOS::strcat(work_path, separator);
        OFOS::strcat(work_path, token);
        if (!OFFile::doesPathExist(work_path))
            OFPlatform::mkdir(work_path);
        delete[] token;
    }
    return 0;
}

ofint32
OFPlatform::chdir(const char* dirname)
{
#if defined(OFOPSYS_WIN32)
    return chdir(dirname);
#else
    return ::chdir(dirname);
#endif
}

/*static*/
char *
OFPlatform::getcwd(char* buffer, ofuint32 bufferSize)
{
#if defined(OFOPSYS_WIN32)
    return ::_getcwd(buffer, bufferSize);
#else
    return ::getcwd(buffer, bufferSize);
#endif
}

ofint32
OFPlatform::rmdir( const char* dirname )
{
#if defined(OFOPSYS_WIN32)
        return RemoveDirectory(dirname)?0:-1;
#else
        return ::rmdir( dirname );
#endif
}

ofint32 
OFPlatform::threadId()
{
#if defined(OFOPSYS_WIN32)
        return GetCurrentThreadId( );
#else
        return getpid();
#endif
}

ofuint32
OFPlatform::currentOSEnum()
{
#if defined(OFOPSYS_LINUX)
    return OS_LINUX;
#elif defined(OFOPSYS_WIN32)
    return OS_WIN2K;
#elif defined(OFOPSYS_FREEBSD)
    return OS_FREEBSD;
#elif defined(OFOPSYS_DARWIN)
    return OS_DARWIN;
#elif defined(OFOPSYS_SOLARIS)
    return OS_SOLARIS;
#else
#error Undefined platform
#endif
}

ofuint32
OFPlatform::startServer( const char* siteName,
                         char startDaemon,
                         char waitForStart )
{
    ofuint32 ret = ERR_SUCCESS;
    if ( startDaemon )
    {
#if defined(OFOPSYS_WIN32)
        ret = startServerDaemon_win32( siteName, waitForStart );
#endif
#if defined(OFOPSYS_LINUX)
#endif
    }
    else
    {
#if defined(OFOPSYS_WIN32)
        ret = startServer_win32( siteName );
#endif
    }
    return ret;
}

ofuint32
OFPlatform::stopServer( const char* siteName,
                        char stopDaemon,
                        char waitForStop )
{
    ofuint32 ret = ERR_SUCCESS;
    if ( stopDaemon )
    {
#if defined(OFOPSYS_WIN32)
        ret = stopServerDaemon_win32( siteName, waitForStop );
#endif
#if defined(OFOPSYS_LINUX)
#endif
    }
    else
    {
#if defined(OFOPSYS_WIN32)
        ret = stopServer_win32( siteName, waitForStop );
#endif
    }
    return ret;
}


#if defined(OFOPSYS_WIN32)
ofuint32
OFPlatform::startServer_win32 (const char* site)
{
        char cmd[1025];
        char param[129] = "-site";
        char ofbin[1024];

        if ( getenv( "OFBIN" ) )
                OFOS::strcpy( ofbin, getenv( "OFBIN" ) );
        OFOS::snprintf( cmd, 1024, "%s/ofserver", ofbin );
        return _execl( cmd, cmd, param, site, 0 );
}

ofuint32
OFPlatform::startServerDaemon_win32( const char* site, char waitForStart )
{
        ofuint32 werr = 0;
        SC_HANDLE hSCM=OpenSCManager (NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS );
        if ( hSCM )
        {
                SC_HANDLE hService = OpenService (hSCM, "ofserver", SERVICE_START);
                if (hService)
                {
                        StartService (hService, 0, 0);
                        CloseServiceHandle (hService);
                }
                else
                        werr = GetLastError ();
                CloseServiceHandle (hSCM);
        }
        else
                werr = GetLastError ();

        if (werr)
                return OFUtility::translateSystemError (werr);

        if (waitForStart)
        {
                hSCM = OpenSCManager (NULL, NULL, GENERIC_READ);
                SC_HANDLE hService = OpenService (hSCM, "ofserver", SERVICE_QUERY_STATUS);
                SERVICE_STATUS ss;
                while (QueryServiceStatus (hService, &ss) && ss.dwCurrentState != SERVICE_RUNNING)
                {
                        Sleep(100);
                }
                if (ss.dwCurrentState != SERVICE_RUNNING)
                        return OFUtility::translateSystemError (GetLastError());
        }

        return ERR_SUCCESS;
}

ofuint32
OFPlatform::stopVFSDaemon_win32()
{
        ofuint32 werr = 0;
        SERVICE_STATUS ss;

        SC_HANDLE hSCM=OpenSCManager (NULL, NULL, SC_MANAGER_CONNECT |
                SC_MANAGER_ENUMERATE_SERVICE |
                SC_MANAGER_QUERY_LOCK_STATUS );
        if (hSCM)
        {
                SC_HANDLE hService = OpenService (hSCM, "ofinterface", SERVICE_STOP);
                if (hService)
                {
                        ControlService (hService, SERVICE_CONTROL_STOP|SERVICE_QUERY_STATUS, &ss);

                        ofuint32 i = 0;
                        while (QueryServiceStatus (hService, &ss) && ss.dwCurrentState != SERVICE_STOPPED && i < 60 )
                        {
                                //cout << "Waiting: " << i << endl;
                                OFOS::os_sleep(1);
                                i++;
                        }
                        if (ss.dwCurrentState != SERVICE_STOPPED)
                                return OFUtility::translateSystemError (GetLastError());
                        CloseServiceHandle (hService);
                
                }
                else
                        werr = GetLastError ();
                CloseServiceHandle (hSCM);
        }
        else
                werr = GetLastError ();

        if (werr)
                return OFUtility::translateSystemError (werr);
        return ERR_SUCCESS;
}

ofuint32
OFPlatform::stopServerDaemon_win32 (const char* site, char waitForStop)
{
        ofuint32 werr = 0;
        SERVICE_STATUS ss;

        //cout << "Stopping the VFS daemon" << endl;
        OFPlatform::stopVFSDaemon_win32();

        //cout << "opening SCM" << endl;
        SC_HANDLE hSCM=OpenSCManager (NULL, NULL,
                                SC_MANAGER_CONNECT |
                                SC_MANAGER_ENUMERATE_SERVICE | 
                                SC_MANAGER_QUERY_LOCK_STATUS);
        if (hSCM)
        {
                SC_HANDLE hService = OpenService (hSCM, "ofserver", SERVICE_STOP);
                if (hService)
                {
                        //cout << "Stopping the service" << endl;
                        if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss))
                                werr = GetLastError();
                        CloseServiceHandle (hService);
                }
                else
                {
                        werr = GetLastError ();
                        //cout << "Error from openservice:" << werr << endl;
                }
                CloseServiceHandle (hSCM);
        }
        else
        {
                werr = GetLastError ();
                //cout << "Error from openSCManageR:" << werr << endl;
        }

        if (werr)
                return OFUtility::translateSystemError (werr);

        if (waitForStop)
        {
                //cout << "Waiting for service" << endl;
                hSCM = OpenSCManager (NULL, NULL, GENERIC_READ);
                if (!hSCM)
                {
                        werr = GetLastError();
                        //cout << "Error opening SCM" << endl;
                        return OFUtility::translateSystemError (werr);
                }
                SC_HANDLE hService = OpenService (hSCM, "ofserver", SERVICE_QUERY_STATUS);
                if (!hService)
                {
                        werr = GetLastError();
                        //cout << "Error opening service" << endl;
                        return OFUtility::translateSystemError (werr);
                }
                ofuint32 i = 0;
                while (QueryServiceStatus (hService, &ss) && ss.dwCurrentState != SERVICE_STOPPED && i < 60 )
                {
                        //cout << "Waiting: " << i << endl;
                        OFOS::os_sleep(1);
                        i++;
                }
                if (ss.dwCurrentState != SERVICE_STOPPED)
                        return OFUtility::translateSystemError (GetLastError());
                CloseServiceHandle (hService);
                CloseServiceHandle (hSCM);
        }

        return ERR_SUCCESS;
}

ofuint32
OFPlatform::stopServer_win32 (const char* site, char waitForStop)
{
        ofuint32 pid = 0;

        if ( waitForStop )
        {
                char lockpath[OF_MAX_PATH_LEN+1];

                const char* dp = getenv( "OPENFABRIC" );
                if ( !dp )
                        return ERR_SUCCESS_WITH_INFO;

                OFOS::snprintf( lockpath, OF_MAX_PATH_LEN, "%s/ofserver-%s.lock", dp, site );
                OFFile file( lockpath, 0664, OFFILE_OPEN_EXISTING );
                if ( !file.isOpen() )
                        return ERR_FILE_OPEN_ERR;

                char line[1024];
                ofuint32 linelen = file.read( line, 1024 );
                line[linelen] = 0;
                pid = atol( line );
        }

        OFFile file( "C:\\stopserver" );
        file.close();

        if ( waitForStop )
        {
                HANDLE ph = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, pid );
                if ( ph )
                {
                        ofuint32 exitcode = STILL_ACTIVE;
                        while ( exitcode == STILL_ACTIVE )
                        {
                                Sleep( 100 );
                                GetExitCodeProcess( ph, &exitcode );
                        }
                }
        }
        return ERR_SUCCESS;

}

#endif // defined(OFOPSYS_WIN32)

ofuint32
OFPlatform::restrictedFile( const char* filename )
{
        const char* bindir = getenv("OFBIN");
        if (bindir && !OFOS::strnicmp(filename, bindir, OFOS::strlen(bindir)))
                return ERR_INVALID_FILE;
        if (OFOS::strstr(filename,".ofor"))
                return ERR_INVALID_FILE;
        return ERR_SUCCESS;
}

OFOS::of_ipv6_t
OFPlatform::convertIP(const char* ipAddress)
{
    if (!ipAddress || !OFOS::strlen(ipAddress))
        return 0;
    OFOS::of_ipv6_t ip = 0;
#if defined(OFOPSYS_WIN32)
    if ((ip = inet_addr(ipAddress)) == -1)
        ip = 0;
#else
    struct in_addr address;
#if defined(OFOPSYS_SOLARIS)
    if (inet_pton(AF_INET, ipAddress, &address))
#else
    if (inet_aton(ipAddress, &address))
#endif // defined(OFPLATFORM_SOLARIS)
        ip = address.s_addr;
#endif // defined(OFPLATFORM_WIN32)
    return ip;
}

OFOS::of_ipv6_t
OFPlatform::getHostIP(const char* hostname)
{
    ofuint32 ip = 0;

#if defined(OFOPSYS_WIN32)
    hostent* he = gethostbyname(hostname);
    if (he)
        ip = *(ofuint32*)he->h_addr_list[0];
#else

    struct addrinfo* res = 0;
    struct addrinfo hints;
    struct sockaddr_in sa;

    //memset(&hints, 0, sizeof(hints));
    //hints.ai_flags = AI_CANONNAME;
    //int rc=getaddrinfo(hostname, 0, &hints, &res);
    int rc=getaddrinfo(hostname, 0, 0, &res);
    if (!rc)
    {
        memcpy(&sa, res[0].ai_addr, sizeof(sa));
        ip = *(ofuint32*)&sa.sin_addr;
        freeaddrinfo(res);
    }
    else
    {
        cout << "getaddrinfo:" << rc << ":" << retrieveMsg(OFUtility::translateSystemError(rc)) << endl;
    }
#endif

    return ip;
}

ofint32
OFPlatform::flushFile(ofint32 handle)
{
#if defined(OFOPSYS_WIN32)
    return !FlushFileBuffers( (HANDLE)handle );
#elif defined(OFOPSYS_DARWIN)
    return fsync(handle);
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_SOLARIS)
    return fdatasync(handle);
#else
#error Undefined platform
#endif
}

#ifdef OFOPSYS_WIN32
bool
isThisSystemNTor2000()
{
    ofuint32 dwVersion = GetVersion();

    bool answer = false;

    // Get major and minor version numbers of Windows
    ofuint32 dwWindowsMajorVersion = ( ofuint32 ) ( LOBYTE( LOWORD( dwVersion ) ) );
    ofuint32 dwWindowsMinorVersion = ( ofuint32 ) ( HIBYTE( LOWORD( dwVersion ) ) );

    // Get build numbers for Windows NT or Win32s
    if ( dwVersion < 0x80000000 )   // Windows NT/2000
    {
        answer = true;
    }
    else if ( dwWindowsMajorVersion < 4 )   // Win32s
    {
    }
    else                        // Windows 95 -- No build numbers provided
    {
    }

    return answer;
}
#endif

ofuint32
OFPlatform::doesUserHaveAdminPrivileges()
{
    bool ok = false;

#ifdef OFOPSYS_WIN32
    const int MAX_NAME = 256;

    if ( isThisSystemNTor2000() )
    {
        // uses the NT help example code function SearchTokenGroupsForSID()

        ofuint32 i, dwSize = 0, dwResult = 0;
        HANDLE hToken;
        PTOKEN_GROUPS pGroupInfo;
        SID_NAME_USE SidType;
        char lpName[MAX_NAME];
        char lpDomain[MAX_NAME];
        BYTE sidBuffer[100];
        PSID pSID = ( PSID ) & sidBuffer;   // SID Security identifier
        SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;

        // Open a handle to the access token for the calling process.

        if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
        {
            return false;
        }

        // Call GetTokenInformation to get the buffer size.

        if ( !GetTokenInformation( hToken, TokenGroups, NULL, dwSize, &dwSize ) )
        {
            dwResult = GetLastError();
            if ( dwResult != ERROR_INSUFFICIENT_BUFFER )
            {
                //trace( WARNING,  "GetTokenInformation Error %u\n", dwResult );
                return false;
            }
        }

        // Allocate the buffer.

        pGroupInfo = ( PTOKEN_GROUPS ) GlobalAlloc( GPTR, dwSize );

        // Call GetTokenInformation again to get the group information.

        if ( !GetTokenInformation( hToken, TokenGroups, pGroupInfo, dwSize, &dwSize ) )
        {
            return false;
        }

        // Loop through the group SIDs looking for the administrator SID.

        for ( i = 0; i < pGroupInfo->GroupCount && !ok; i++ )
        {
            if ( true || EqualSid( pSID, pGroupInfo->Groups[i].Sid ) )  // Put in true ||
            {
                // Lookup the account name.

                dwSize = MAX_NAME;
                if ( !LookupAccountSid( NULL, pGroupInfo->Groups[i].Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType ) )
                {
                    dwResult = GetLastError();
                    if ( dwResult == ERROR_NONE_MAPPED )
                    {
						OFOS::strcpy( lpName, "NONE_MAPPED");
                    }
                    else
                    {
                        return false;
                    }
                }

                if (OFOS::strstr( lpName, "Administrators"))    //TODO is this OK on non-english machines?
                {
                    ok = true;
                    break;
                }
            }
        }

        if ( pGroupInfo )
            GlobalFree( pGroupInfo );
    }
    else
    {
        // everyone has full access on win 9x
        ok = true;
    }

#else // UNIX

    /*
    char aUserName[128];
    getEffectiveUserName( aUserName );
    if ( aUserName != NULL && OFOS::strcmp( aUserName, "root" ) == 0 )
        ok = true;
    */
#endif

    return ok;
}

#if defined(OFOPSYS_WIN32)
double
OFPlatform::getTimeDifference(SYSTEMTIME *t_s, SYSTEMTIME *t_e)
{
    FILETIME ft_s;
    SystemTimeToFileTime(t_s, &ft_s);
    LARGE_INTEGER li_s;
    li_s.LowPart = ft_s.dwLowDateTime;
    li_s.HighPart = ft_s.dwHighDateTime;

    FILETIME ft_e;
    SystemTimeToFileTime(t_e, &ft_e);
    LARGE_INTEGER li_e;
    li_e.LowPart = ft_e.dwLowDateTime;
    li_e.HighPart = ft_e.dwHighDateTime;

    double dd = (li_e.QuadPart - li_s.QuadPart);
    // Convert from 100 nanos to seconds
    dd = dd / 10000000;

    return dd;
}
#else
double
OFPlatform::getTimeDifference(struct timeval *t_s, struct timeval *t_e)
{
    double dd;
    double d_e = t_e->tv_sec + ((double)t_e->tv_usec)/1000;
    double d_s = t_s->tv_sec + ((double)t_s->tv_usec)/1000;
    dd = d_e - d_s;
    return dd;
}
#endif

#if defined(UNIT_TEST)
#include "UnitTest.h"

int main(int argc, char* argv[])
{
    UT_BATCH_START("ofplatform");

    UT_TEST_START(1, "OFPlatform::mkpath(const char* path)");
    const char* path1 = "/code/banana/apple";
    OFPlatform::mkpath(path1);
    UT_EVAL_EXPR(OFFile::doesPathExist(path1));
    OFPlatform::rmdir("/code/banana/apple");
    OFPlatform::rmdir("/code/banana");
    UT_TEST_END;
    UT_BATCH_END;
    return 0;
}

#endif // UNIT_TEST
