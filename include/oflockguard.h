/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: oflockguard.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
*/

#if !defined(_OF_LOCK_GUARD_H_)
#define _OF_LOCK_GUARD_H_

/** \file oflockguard.h
    Contains the declaration of the template class OFLockGuard
*/

/** \class OFLockGuard oflockguard.h
    Provides a guard pattern implementation for OFMutex type locks.

    To use this guard, simply declare an instance of the required type
    (on the stack.)  The constructor will acquire the lock, and the
    destructor will automatically release it, so you don't have to worry
    about how and where you exit the function (or code block.)
*/

template<class LOCK>
class OFSYS_API OFLockGuard
{
 public:
    OFLockGuard( LOCK *l ) { m_lock = l; m_lock->acquire(); }
    ~OFLockGuard( ) { m_lock->release(); }
 protected:
    LOCK *m_lock;
};

#endif // #if !defined(_OF_LOCK_GUARD_H_)
