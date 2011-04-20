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

#if !defined(OF_PLATFORM_H_)
#define OF_PLATFORM_H_

using namespace std;
#include <ofos.h>

/** /class OFPlatform ofplatform.h

    Provides utility functions that are not openFabric specific.
    These functions deal with inconsistencies between OS implememtations.
*/

class OFSYS_API OFPlatform
{
 public:
    OFPlatform() {;}
    ~OFPlatform() {;}

    /** Retrieve the OS enum
     */
    static ofuint32 currentOSEnum();

    /** Correct the supplied file name.
        Checks for any illegal characters in the supplied path, and replaces them with the supplied character.
        \warning This method will NOT replace slashes (forward and backward.)
    */
    static void manglePath( char *path, char replace = '_' );

    /** Fix the path.
        This function will correct any incorrect slashes, for example
        a backslash in a unix environment, and remove any duplicate
        slashes.
    */
    static void fixPath( std::string &path );

    /** Fix the path.
        This function will correct any incorrect slashes, for example
        a backslash in a unix environment, and remove any duplicate
        slashes.
    */
    static void fixPath( char *path );

    /** Fix the path forced to a platform type
        \param path The path to be fixed.
        \param unix_dos Non-zero for unix, 0 for dos
    */
    static void fixPath( char *path, char unix_dos );

    /** Retrieve the configuration path for the current platform.
        Checks for the environment variable OFCONFIGPATH and uses that if found. Then
        checks for OPENFABRIC, and uses the value if found.
        If not uses a platform specific default. On Win32, the default Windows
        directory is used (if found, "C:\" if not) and "/etc" on Unix platforms.
    */
    static void configPath( char *path );

    /** Retrieve the log directory for the current platform.
        Checks for the presence of the environment variable, OFLOGPATH
        and uses that if found. Then checks for the OPENFABRIC environment variable,
        and if found, uses that to open the default configuration file, and read the openfabric|logpath
        setting.
    
        If neither environment variables can be found, sets \c logpath to a platform
        log path. On Win32 platforms it is the default Windows directory (if found)
        or "C:\" if not. On Unix it is "/var/log".
    */
    static void logPath( char *logpath );

    /** Retrieve the current users home directory.
     */
    static void homeDirectory( char *directory );

    /** Turn the supplied name into a valid shared library name.
        Involves adding ".so" on unix and ".dll" on Win32
    */
    static void validSharedLib( std::string &libName );

    /** Checks for the existence of the supplied filename.
        Returns true if the file does exist. Note that this
        method will return false if the file does exist, but
        the current user has no access to it.
    */
    static bool doesFileExist( const char * fileName );
    
    /** Delete the specified file.
        Note on unix platforms, the file may not be immediately deleted,
        if any processes have the file open, the file will remain until
        they shutdown, or close those handles.
        \param fileName The path to the file to be deleted.
        \return 0 on success, -1 on error.
    */
    static ofint32 deleteFile( const char *fileName );

    /** Flush the file to disk

        \return Zero on success.  This means that the Win32
        implementation is converted, as it returns zero on failure.
    */
    static ofint32 flushFile(ofint32 handle);

    /** Copy files
        If \c deleteSrc is non-zero, the file is moved.
    */
    static ofint32 copyFile( const char* dest, const char* src, char deleteSrc );

    /** Retrieve the file name portion of a path.
        \param path A null-terminated string containing the full-path.
        \param filename A buffer which will contain the file name portion.
        \param length The length of the buffer supplied in \c filename.
        \return 0 on success. -1 on error.
    */
    static ofint32 retrieveFileName( char *path, char *filename, ofuint32 length );

    /** Retrieve the file name portion of a path.
        This version of the method accepts a const parameter for the path,
        creating it's own copy internally for path fixing.
        \param path A null-terminated string containing the full-path.
        \param filename A buffer which will contain the file name portion.
        \param length The length of the buffer supplied in \c filename.
        \return 0 on success. -1 on error.
    */
    static ofint32 retrieveFileName2 (const char *path, char *filename, ofuint32 length );

    /** Retrieve the directory portion of a file name
        \param path The path to retrieve the directory from.
        \param directory A buffer to hold the directory.
        \param length The length of the supplied buffer.
        \param fix_path Non-zero to force the supplied path to be fixed (using OFPlatform::fixPath)
        before the directory is extracted.
        \param force_delim If non-zero then used as a path separator instead of the platform default.
    */
    static ofint32 retrieveDirectory (const char* path, char* directory, ofuint32 length, char fix_path = 1, char force_delim = 0);

    /** Retrieve the filename portion of a path.

        This includes the extension.
    */
    static string get_filename_from_path(const string& path);

    /** Retrieve the extension portion of a path
     */
    static string get_ext_from_path(const string& path);

    /** Retrieve the directory
     */
    static string get_dir_from_path(const string& path);

    /** Split the path into directory and filename
     */
    static void split_dir_filename_from_path(const string& path, string& dir, string& filename);

    /** Split filename and extension
     */
    static void split_filename_ext_from_path(const string& path, string& filename, string& ext);

    /** Create a directory.
     */
    static ofint32 mkdir( const char *dirname );

    /** Creates a full path

        Steps through each element of the supplied path, and makes
        sure each element exists (as a directory.)  Pretty much
        the same as unix command 'mkdir -p'.
    */
    static ofint32 mkpath(const char* path);

    /** Change the current directory

        \param dirname A null-terminated string containing the new
        directory name.

        \return The OS specific return code.  This can be passed to
        OFUtility::translateSystemError() to translate it to something
        more portable.  On some platforms you may need to query \c
        errno.
    */
    static ofint32 chdir(const char* dirname);

    /** Retrieve the current working directory.

        \param buffer A pointer to a buffer to receive the current
        directory.

        \param bufferSize The size of the supplied \c buffer.

        \return A pointer to \c buffer if successful, otherwise NULL.
    */
    static char* getcwd(char* buffer, ofuint32 bufferSize);

    /** Start the openfabric server
     */
    static ofuint32 startServer( const char* siteName,
                                 char startDaemon,
                                 char waitForStart );

    /** Stop the openfabric server
     */
    static ofuint32 stopServer( const char* siteName,
                                char stopDaemon,
                                char waitForStop );

    /** Remove a directory
     */
    static ofint32 rmdir( const char* dirname );

    /** Remove a tree
        
        Delete all child directories and files
    */
    static ofint32 rmtree(const char* path);

    /** Is a file restricted
     */
    static ofuint32 restrictedFile( const char* filename );

    /** Retrieve current thread identifier.
        On pthread platforms, this is simply the process id.
    */
    static ofint32 threadId();

    /** Does the current user have administative permissions.
        At present this method is only implemented for Win32 platforms
    */
    static ofuint32 doesUserHaveAdminPrivileges();

    /** \name Socket methods
     */
    //@{

    /** Lookup the ip address for the supplied hostname
     */
    static OFOS::of_ipv6_t getHostIP(const char* hostname);

    /** Convert the supplied IP address (in dot notation)
     */
    static OFOS::of_ipv6_t convertIP(const char* ipAddress);

    //@}

    static const char PathSeparator;
    static const char PathSeparatorS[2];

    static const char *pathDelimiter;

    static const char *CRLF;

    static const char *sharedLibExt;

    /** Returns time difference in seconds.
     */
#if defined(OFOPSYS_WIN32)
    static double getTimeDifference(SYSTEMTIME *t_s, SYSTEMTIME *t_e);
#else
    static double getTimeDifference(struct timeval *t_s, struct timeval *t_e);
#endif

 protected:

#if defined(OFOPSYS_WIN32)
    static ofuint32 startServerDaemon_win32 (const char* site, char waitForStart);
    //static ofuint32 startVFSDaemon_win32();
    static ofuint32 startServer_win32 (const char* site);

    static ofuint32 stopServerDaemon_win32 (const char* site, char waitForStop);
    static ofuint32 stopVFSDaemon_win32();
    static ofuint32 stopServer_win32 (const char* site, char waitForStop);
#endif

    static ofint32 rmtree_(const char* path);
};

#endif // #if defined(OF_PLATFORM_H_)
