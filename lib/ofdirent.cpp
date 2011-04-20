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
#include <ofdirent.h>
#include <ofplatform.h>
#include <ofos.h>

OFDirent::OFDirent( const char *path )
{
    OFOS::strncpy2( m_path, path, OF_MAX_PATH_LEN );
    OFPlatform::fixPath( m_path );
#if defined(OFOPSYS_WIN32)
    char realpath[OF_MAX_PATH_LEN+1];
    OFOS::snprintf( realpath, OF_MAX_PATH_LEN, "%s\\*.*", m_path );
    m_dir = FindFirstFile( realpath, &m_data );
    if (m_dir == INVALID_HANDLE_VALUE)
        m_dir = 0;
    m_first = 1;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    m_dir = opendir( m_path );
#endif

}

OFDirent::~OFDirent( )
{
#if defined(OFOPSYS_WIN32)
    FindClose( m_dir );
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    closedir (m_dir);
#endif
}

ofuint32
OFDirent::read( OFDIRENT *direntry )
{
    if (!m_dir)
        return 0;

    memset( direntry, 0, sizeof(OFDIRENT) );

#if defined(OFOPSYS_WIN32)
    if ( m_first )
        m_first = 0;
    else
    {
        if ( !FindNextFile( m_dir, &m_data ) )
            return 0;
    }
    OFOS::strncpy2( direntry->name, m_data.cFileName, OF_MAX_PATH_LEN );
    direntry->type = (m_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?OFDIR_TYPE_DIR:OFDIR_TYPE_FILE;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    struct dirent *de = readdir( m_dir );
    if ( de )
    {
#if defined(OFOPSYS_SOLARIS)
        direntry->type = 0;
#endif
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_FREEBSD)
        OFOS::strncpy2( direntry->name, de->d_name, OF_MAX_PATH_LEN );
        //             struct stat* buf;
        //             OFFile::getExtFileInfo( fullpath, buf );
        if ( de->d_type == DT_REG || de->d_type == DT_LNK )
            direntry->type = OFDIR_TYPE_FILE;
        if ( de->d_type == DT_DIR )
            direntry->type = OFDIR_TYPE_DIR;
#endif
    }
    else
        return 0;
#else
#error Undefined platform
#endif
    return 1;
}
