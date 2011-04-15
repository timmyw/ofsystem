/* $Revision: 2.4 $

$Id: offile.h,v 2.4 2010-03-21 17:37:03 timmy Exp $
*/

#if !defined(_OF_FILE_H_)
#define _OF_FILE_H_

#include <ofos.h>

/**
 * \par  Created
 * Apr-00 by Timmy Whelan
 * \par  Last u.test update
 * dd/mm/yy by ------
 * \par  Last code review
 * dd/mm/yy by ------
 */

/** \class OFFile offile.h
    Provides a platform-independant API for accessing files.

    \nosubgrouping
*/
class OFSYS_API OFFile
{
 public:

    /** \name Constructors and destructor.
     */
    //@{

    /** Empty constructor.
     */
    OFFile( );

    /** Copy constructor.
        \warning At present this constructor does
        not duplicate theofint32ernal file handle. At some
        point we will probably have to call dup() on it.
    */
    OFFile( const OFFile &copy );

    /** Constructor.
        Constructs and opens the specified file name. Use the isOpen() method to
        determine if the open was successful.

        \param fileName The name of the file to open. If this parameter is NULL,
        a temporary file is opened.
        \param mode The file permissions to use if the file is created.
        \param flags Various flags that affect how the file is opened/created.
        These are a combination of the following:
        \c OFFILE_CREATE_ALWAYS    The file is always created, truncated if it already existed.
        \c OFFILE_OPEN_EXISTING    Open an existing file. Fails if the file does not exist.
        \c OFFILE_OPEN_ALWAYS  Open the file if it exists, create it if it doesn't.
        \c OFFILE_READWRITE        Open the file read and write.
        \c OFFILE_READONLY     Open the file read only.

        \b Platforms:
        Win32: The mode parameter is ignored.
    */
    OFFile( const char * fileName,ofint32 mode = 0664,ofint32 flags = OFFILE_OPEN_ALWAYS | OFFILE_READWRITE );

    /** Destructor.
        Automatically closes the file if it was open.
    */
    virtual ~OFFile( );

    //@}

    /** \name Open and close methods.
     */
    //@{

    /** Open the specified file.
        \param fileName The name of the file to open. If this parameter is NULL,
        a temporary file is opened.
        \param mode The file permissions to use if the file is created.
        \param flags Various flags that affect how the file is opened/created.
        These are a combination of the following:
        \c OFFILE_CREATE_ALWAYS    The file is always created, truncated if it already existed.
        \c OFFILE_OPEN_EXISTING    Open an existing file. Fails if the file does not exist.
        \c OFFILE_OPEN_ALWAYS  Open the file if it exists, create it if it doesn't.
        \c OFFILE_READWRITE        Open the file read and write.
        \c OFFILE_READONLY     Open the file read only.

        \b Platforms:
        Win32: The mode parameter is ignored.
        \returns 1 on success and zero on failure.
    */
    ofint32 open( const char *fileName,ofint32 mode = 0664,ofint32 flags = OFFILE_OPEN_ALWAYS | OFFILE_READWRITE  );

    /** Close the file if open.
     */
    virtual ofint32 close( );

    /** Truncate the file.
       
        Truncates the file to zero length. This can also be
        accomplished by passing in O_CREAT to the constructor or
        open().
    */
    int truncate( );
  
    //@}

    /** \name Data member accessor methods.
     */
    //@{

    /** Return the internal handle.
        \warning The returned handle is not duplicated, and is still owned by
        the instance of OFFile.
    */
    const OFOS::of_handle_t handle() const;

    /** Set the handle for the file.
        A close is done on any previously opened file.
    */
    void handle( OFOS::of_handle_t handle );

    /** Return the open flag.
        \return True if the file was successfully open.
    */
    bool isOpen() const;

    /** Retrieve the file name.
        \return A const pointer to a null-terminated string containing the file path.
    */
    const char *name( ) const;

    //@}

    /** \name Read and write methods.
     */
    //@{

    /** Write the buffer to the file.
        The data is written at the current file position.
        \return The number of bytes actually written to the file.
    */
    virtual ofint32 write( const void * buffer, ofuint32 length );

    /** Write a line to the file
     */
    virtual ofint32 write(const char* line);

    /** Read from the file (at the current file position)ofint32o the supplied buffer.
        Returns the actual number of bytes read.
    */
    virtual ofint32 read( void *buffer, ofuint32 length );

    //@}

    /** \name File attribute methods.
     */
    //@{

    /** Set the file position.
        \return The new position.
    */
    ofint32 position( ofuint32 newpos,ofint32 from = OFFILE_POSITION_BEGIN );

    /** Retrieve the file position.
        \return The current file position.
    */
    ofint32 position( );

    /** Retrieve the file size.
        \return The size of the file.
    */
    ofuint32 size() const;

    /** Retrieve the file type.
        Returns OFFILE_FILETYPE_FILE or OFFILE_FILETYPE_DIR.
    */
    ofint32 type( );

    /** Retrieve extended file information
        Returns 
    */
    static ofint32 getExtFileInfo( char* file, OFOS::of_stat *buf );
    /*
#if defined(OFOPSYS_WIN32)
    ofint32 getExtFileInfo( char* file, struct _stat *buf );
#else
    ofint32 getExtFileInfo( char* file, struct stat *buf );
    #endif*/

    //@}

    /** \name Temporary path management.
     */
    //@{

    /** Set the temporary path.
     */
    static void setTempPath( const char *newPath );

    /** Retrieve the temporary path.
     */
    static const char *getTempPath( );

    //@}

    static bool doesFileExist (const char *path);

    static bool doesPathExist(const char* path);

    static ofuint32 type(const char *path);

    /** Retrieve the filename and extensions separately.

        Any path information is maintained in the name portion.
     */
    static void split_name_ext(const char* full
                               , char* name
                               , char* ext);

    /** Generate a unique file name.

        Given a starting filename (which should include the path - if
        any) it will use a counter to ensure that a file is unique -
        appending 0001, 0002 etc to do so.  The counter is appended at
        the end of the filename portion, before the extension (if any)
     */
    static void make_unique(char *filename);

    //@}

 protected:
    
    char *m_name;
    OFOS::of_handle_t m_handle;
    bool m_open;

    /** Path for temporary files.
     */
    static char m_tempPath[OF_MAX_PATH_LEN+1];

private:

    /** operator = (not implemented)
     */
    OFFile& operator = ( const OFFile& );
};

#endif // #if !defined(_OF_FILE_H_)
