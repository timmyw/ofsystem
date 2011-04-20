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

#if !defined(_OF_MUTEX_H_)
#define _OF_MUTEX_H_

/** \file ofmutex.h
    Contains the declaration of the OFMutex class.
*/

#include <ofsys.h>
#include <ofos.h>

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

