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
#include <ofdll.h>

OFDLL::OFDLL( ) :
    m_autoClose( false ),
    m_open( false )
{
}

OFDLL::OFDLL( const char *path, bool autoClose /* = true */ ) :
    m_autoClose( autoClose ),
    m_open( false )
{
    open( path, autoClose );
}

OFDLL::~OFDLL( )
{
    if ( m_open && m_autoClose )
        close( );
}

ofint32 
OFDLL::open( const char *path, bool autoClose /* = true */ )
{
    ofint32 ret = 0;

#if defined(OFOPSYS_WIN32)
    m_handle = LoadLibrary( path );
    ret = (m_handle != NULL );
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    //      cout << "Opening:" << path << endl;
#if !defined(NDEBUG)
    m_handle = dlopen( path, RTLD_LAZY );
#else
    m_handle = dlopen( path, RTLD_NOW );
#endif
    ret = ( m_handle != 0 );
#else
#error Undefined platform
#endif
    if ( ret )
        m_open = true;

    return ret;
}

bool 
OFDLL::isOpen() const
{
    return m_open;
}

void *
OFDLL::symbol( const char *funcname )
{
    OF_DLL_FUNC f = 0;

#if defined(OFOPSYS_WIN32)
    f = (OF_DLL_FUNC)GetProcAddress( m_handle, funcname );
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    f = (OF_DLL_FUNC)dlsym( m_handle, funcname );
#else
#error Undefined platform
#endif

    return (void *)f;
}

void
OFDLL::close( )
{
    if ( m_open )
    {
#if defined(OFOPSYS_WIN32)
        if ( FreeLibrary( m_handle ) )
            m_open = false;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
        if ( !dlclose( m_handle ) )
            m_open = false;
#else
#error Undefined platform
#endif

    }
}
