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
#include <ofmutex.h>

#if defined(UNIT_TEST)
#include <iostream>
#endif

OFMutex::OFMutex( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    pthread_mutexattr_t mutattr;
    pthread_mutexattr_init( &mutattr );
#if defined(OFOPSYS_LINUX)
    pthread_mutexattr_setkind_np( &mutattr, PTHREAD_MUTEX_RECURSIVE_NP );
#endif // OFOPSYS_LINUX
#if defined(OFOPSYS_FREEBSD)
    pthread_mutexattr_settype( &mutattr, PTHREAD_MUTEX_RECURSIVE );
#endif
    pthread_mutex_init( &m_mutex, &mutattr );
#endif // #if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD)
    
#if defined(OFOPSYS_WIN32)
    m_mutex = CreateMutex( NULL, FALSE, NULL );
#endif
    
}

OFMutex::~OFMutex( )
{

#if defined(UNIT_TEST)
    cout << "OFMutex::~OFMutex()" << endl;
#endif

#if defined(OFOPSYS_WIN32)
    CloseHandle( m_mutex );
#endif // #if defined(OFOPSYS_WIN32)

}

void
OFMutex::acquire( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD)
    pthread_mutex_lock( &m_mutex );
#endif
#if defined(OFOPSYS_WIN32)
    WaitForSingleObject( m_mutex, INFINITE );
#endif
}

ofint32 
OFMutex::tryAcquire( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD)
    ofint32 rc = pthread_mutex_trylock( &m_mutex );
    return ( rc == EBUSY ) ? 1 : 0;
#endif
#if defined(OFOPSYS_WIN32)
    ofint32 rc = WaitForSingleObject( m_mutex, 0 );
    return (rc == WAIT_TIMEOUT ) ? 1: 0;
#endif
}

void
OFMutex::release( )
{
#if defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD)
    pthread_mutex_unlock( &m_mutex );
#endif
#if defined(OFOPSYS_WIN32)
    while ( ReleaseMutex( m_mutex ) ) ;
#endif
}


#if defined(UNIT_TEST)

#include "UnitTest.h"

ofint32  main( ofint32 argc, char *argv[] )
{
    UT_BATCH_START( "OFMutex" );
    
    UT_TEST_START( 1, "static create (named)" );
    UT_TEST_END( 1 );
    OFMutex *mutex1 = OFMutex::create( "mutex_one" );
    UT_EVAL_EXPR( 1, "static create (named)", ( !OFOS::strcmp( mutex1->name(), "mutex_one" ) ) );
    
    UT_TEST_START( 2, "static create (named)" );
    OFMutex *mutex2 = OFMutex::create( "mutex_one" );

    UT_EVAL_EXPR( 2, "static create (named)", ( !OFOS::strcmp( mutex2->name(), "mutex_one" ) ) );
    UT_EVAL_EXPR( 2, "static create (named)", ( mutex2->equals( mutex1 ) ) );

    UT_TEST_END( 2 );

    UT_TEST_START( 3, "try acquire" );

    cout << "acquire mutex1" << endl;
    ofint32 rc;
    mutex1->acquire( );
    
    cout << "tryacquire mutex2" << endl;
    
    rc = mutex2->tryAcquire( );

    UT_EVAL_EXPR( 3, "try acquire", ( rc == 0 ) );

    cout << "release mutex1" << endl;
    mutex1->release( );
    cout << "tryacquire mutex2" << endl;
    rc = mutex2->tryAcquire( );

    UT_EVAL_EXPR( 3, "try acquire", ( rc ==0 ) );

    mutex2->release( );
    UT_TEST_END( 3 );
    
    OFMutex::destroy( mutex1 );
    OFMutex::destroy( mutex2 );
    
    UT_BATCH_END( "OFMutex" );
    
    return 0;
}

#endif

