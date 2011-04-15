/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofmutex.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

#if !defined(_OF_MUTEX_H_)
#define _OF_MUTEX_H_

/** \file ofmutex.h
    Contains the declaration of the OFMutex class.
*/

#include <ofsys/ofsys.h>
#include <ofsys/ofos.h>

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_SOLARIS)
#include <pthread.h>
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_FREEBSD)
extern "C" int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
#endif
#endif

/** \class OFMutex ofmutex.h
    Provides a platform-independant implementation of a process wide
    mutex.
*/
class OFSYS_API OFMutex
{
 public:
    /** Constructor.
     */
    OFMutex( );

    /** Destructor.
     */
    ~OFMutex ();
    
    /** Acquire the mutex.
        This will block until the mutex is free and 
        can be acquired.
    */
    void acquire( );
    
    /** Try and acquire the mutex.
        This is a non-blocking call, and will return an error if
        the mutex is not available.
    
        \return 0 if successful in acquiring the mutex, 1 if not.
    */
    ofint32 tryAcquire( );
    
    /** Release the mutex.
     */
    void release( );

 protected:

     friend class OFConditionVar;

#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_mutex_t m_mutex;
#elif defined(OFOPSYS_WIN32)
    OFOS::of_handle_t m_mutex;
#else
#error Undefined platform
#endif

};

#endif // #if !defined(_OF_MUTEX_H_)

