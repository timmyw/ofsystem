/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofdll.cpp,v 2.0 2006-01-01 19:26:31 timmy Exp $
 */

#include <ofsys/ofsys.h>
#include <ofsys/ofdll.h>

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
