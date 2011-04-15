/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: oftss.cpp,v 2.0 2006-01-01 19:26:31 timmy Exp $
 */

#include <ofsys/ofsys.h>
#include <ofsys/oftss.h>

OFTSS::OFTSS( void *initial /* = 0 */ )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_key_create( &m_index, 0 );
    pthread_setspecific( m_index, initial );
#elif defined(OFOPSYS_WIN32)
    m_index = TlsAlloc( );
    TlsSetValue( m_index, initial );
#else
#error Undefined platform
#endif

}

OFTSS::~OFTSS( )
{

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_key_delete( m_index );
#elif defined(OFOPSYS_WIN32)
    TlsFree( m_index );
#else
#error Undefined platform
#endif

}

void *
OFTSS::ts_object( )
{

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    return pthread_getspecific( m_index );
#elif defined(OFOPSYS_WIN32)
    return TlsGetValue( m_index );
#else
#error Undefined platform
#endif

}

void 
OFTSS::ts_object( void *value )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_setspecific( m_index, value );
#elif defined(OFOPSYS_WIN32)
    TlsSetValue( m_index, value );
#else
#error Undefined platform
#endif

}
