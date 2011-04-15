/*
 * $OFPREPARED$
 *
 * $Revision: 2.1 $
 *
 * $Id: ofthrmgr.cpp,v 2.1 2007-09-13 09:32:41 timmy Exp $
 */

#include <ofsys/ofsys.h>
#include <ofsys/ofthrmgr.h>

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
extern "C" {
    typedef void*(*THR_ENTRY)(void*);
}
#endif

ofuint32
OFThreadManager::spawnThread( OFOS::of_thrfunc_t entry, 
                              void *param, 
                              OFTHREAD_IDENT *tident, 
                              ofuint32 flags /* = 0 */ )
{
    ofuint32 ret = 0;
    OFOS::of_thread_t tid;

#if defined(OFOPSYS_WIN32)
    DWORD threadid;
    tid = CreateThread( NULL, 0, entry, param, 0, &threadid );
    if ( tid != 0 && tident )
        tident->m_tid = tid;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_attr_t ta;
    pthread_attr_init( &ta );
    pthread_attr_setdetachstate( &ta, (flags == OFTHREAD_DETACHED)?PTHREAD_CREATE_DETACHED:0 );

    ret = pthread_create( &tid, &ta, (THR_ENTRY)entry, param );
    if ( tident )
        tident->m_tid = tid;
#else
#error Undefined platform
#endif

    return ret;
}

ofuint32
OFThreadManager::waitForThread( OFTHREAD_IDENT *tid )
{
    ofuint32 ret = 0;
#if defined(OFOPSYS_WIN32)
    ofuint32 exitCode = STILL_ACTIVE;
    while ( exitCode == STILL_ACTIVE )
        GetExitCodeThread( tid->m_tid, &exitCode );
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_join( tid->m_tid, 0 );
#else
#error Undefined platform
#endif
    return ret;
}

/*static*/
void
OFThreadManager::endThread (ofuint32 exitCode)
{
#if defined(OFOPSYS_WIN32)
        ExitThread (exitCode);
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
        pthread_exit ((void*)(ofpointer)exitCode);
#else
#error Undefined platform
#endif
}

