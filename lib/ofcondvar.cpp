/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofcondvar.cpp,v 2.0 2006-01-01 19:26:31 timmy Exp $
 */

#include <ofsys.h>
#include <ofcondvar.h>
#include <ofmutex.h>

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
#include <sys/time.h>
#endif


OFConditionVar::OFConditionVar() 
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
     pthread_cond_init( &m_cond, NULL );
#elif defined(OFOPSYS_WIN32)
     m_cond = CreateEvent( NULL, false, false, NULL );
#else
#error Undefined platform
#endif // #if defined(OFOPSYS_WIN32)
}

OFConditionVar::~OFConditionVar( )
{
#if defined(OFOPSYS_WIN32)
    CloseHandle( m_cond );
#endif // defined(OFOPSYS_WIN32)
}

void
OFConditionVar::signal( )
{
#if defined(OFOPSYS_WIN32)
    SetEvent( m_cond );
#else
    pthread_cond_signal( &m_cond );
#endif
}

ofint32
OFConditionVar::wait(OFMutex &mutex)
{
     return wait (mutex, (unsigned)CONDVAR_INFINITE);
}

ofint32 
OFConditionVar::wait(OFMutex &mutex, ofuint32 milli )
{
    ofint32 ret = 0;

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)

     struct timeval now;
     struct timespec timeout;
     ofint32 rc;

     if ( milli != (unsigned)CONDVAR_INFINITE )
     {
	  gettimeofday( &now, NULL );
	  timeout.tv_sec = now.tv_sec + milli/1000;
	  timeout.tv_nsec = now.tv_usec + milli % 1000;
	
	  rc = pthread_cond_timedwait( &m_cond, &mutex.m_mutex, &timeout );
     }
     else
	  rc = pthread_cond_wait( &m_cond, &mutex.m_mutex );

     if ( rc == ETIMEDOUT )
	  ret = OFConditionVar::CONDVAR_TIMEOUT;
     if ( rc == 0 )
	  ret = CONDVAR_SIGNALLED;
		
#elif defined(OFOPSYS_WIN32)

     mutex.release ();
     ofint32 rc = WaitForSingleObject( m_cond, milli );
     if ( rc == WAIT_OBJECT_0 )
	  ret = CONDVAR_SIGNALLED;
     if ( rc == WAIT_TIMEOUT )
	  ret = CONDVAR_TIMEOUT;
     mutex.acquire ();
#else
#error Undefined platform
#endif

     return ret;
}

