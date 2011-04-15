/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofthrmgr.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

#if !defined(_OF_THR_MGR_H_)
#define _OF_THR_MGR_H_

#include <ofsys/ofos.h>

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
