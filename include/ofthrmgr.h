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

#if !defined(_OF_THR_MGR_H_)
#define _OF_THR_MGR_H_

#include <ofos.h>

/** \file ofthrmgr.h
    Contains the declaration of the OFThreadManager class.
*/

struct OFTHREAD_IDENT
{
    /** A thread id.
     */
    OFOS::of_thread_t m_tid;
};

/** \class OFThreadManager ofthrmgr.h
    Provides a platform-independant method to fire (spawn) off new threads.
*/
class OFSYS_API OFThreadManager
{
 public:
    enum
        {
            OFTHREAD_DETACHED = 1
        };

    /** Fire off a new thread.
     */
    static ofuint32 spawnThread( OFOS::of_thrfunc_t entry, void *param, OFTHREAD_IDENT *tid, ofuint32 flags = 0 );

    /** Wait for a thread to finish.
     */
    static ofuint32 waitForThread( OFTHREAD_IDENT *tid );

    /** End the current thread.
     */
    static void endThread( ofuint32 exitCode );

};

#endif // #if !defined(_OF_THR_MGR_H_)
