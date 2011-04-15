/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: oftss.i,v 2.0 2006-01-01 20:08:32 timmy Exp $
*/

template <class TYPE>
OFTSS_T<TYPE>::OFTSS_T( TYPE *initial /* = 0 */ )
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

template <class TYPE>
OFTSS_T<TYPE>::~OFTSS_T( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_key_delete( m_index );
#elif defined(OFOPSYS_WIN32)
    TlsFree( m_index );
#else
#error Undefined platform
#endif

}

template<class TYPE>
TYPE *
OFTSS_T<TYPE>::ts_object( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    return (TYPE*)pthread_getspecific( m_index );
#elif defined(OFOPSYS_WIN32)
    return (TYPE *)TlsGetValue( m_index );
#else
#error Undefined platform
#endif

}

template <class TYPE>
void 
OFTSS_T<TYPE>::ts_object( TYPE *value )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_setspecific( m_index, value );
#elif defined(OFOPSYS_WIN32)
    TlsSetValue( m_index, value );
#else
#error Undefined platform
#endif

}
