/*
 * $OFPREPARED$
 *
 * $Revision: 2.1 $
 *
 * $Id: ofdirent.h,v 2.1 2007-05-23 07:08:32 timmy Exp $
 */

#if !defined(_OF_DIRENT_H_)
#define _OF_DIRENT_H_

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
#include <dirent.h>
#endif

/** \file ofdirent.h
    Contains the declaration of the OFDirent class and supporting structures.
*/

enum
    {
        OFDIR_TYPE_FILE = 1,
        OFDIR_TYPE_DIR = 2
    };

/** \struct OFDIRENT
    Contains information about a single directory entry.
    The enumerated types in this structure are manipulated to find common ground
    between the different platforms.
*/
typedef struct tagOFDIRENT
{
    /** The entry type.
     */
    ofint16 type;

    /** The entry name.
     */
    char name[OF_MAX_PATH_LEN+1];

} OFDIRENT;

/** \class OFDirent ofdirent.h
    Provides the means for cycling through a set of directory entries.
*/
class OFSYS_API OFDirent
{
 public:
    /** Constructor.
        Uses the supplied directory path to open the internal data structures.
    */
    OFDirent( const char *path );

    /** Destructor.
     */
    ~OFDirent( );

    /** Read a directory entry.

        \param direntry A pointer to an OFDIRENT structure that will
        be filled with the entry's details.

        \return Non-zero on success. A return value of zero indicates
        the end of the directory entry list.
    */
    ofuint32 read( OFDIRENT *direntry );

 protected:
    /** The directory path.
     */
    char m_path[OF_MAX_PATH_LEN+1];

    /** The directory itself.
     */
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    DIR *m_dir;
#elif defined(OFOPSYS_WIN32)
    // Finddata ?
    HANDLE m_dir;
    WIN32_FIND_DATA m_data;
    char m_first;
#else
#error Undefined platform
#endif

};

#endif // #if !defined(_OF_DIRENT_H_)
