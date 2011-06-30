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

#if !defined(_OFSYS_H_INCLUDED_)
#error "Please do not include this file directly.  Include <ofsys.h>"
#endif

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
