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
#include <ofthrmgr.h>

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
    oflong exitCode = STILL_ACTIVE;
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

