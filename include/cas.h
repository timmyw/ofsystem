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

#ifndef CAS_H
#define CAS_H

#if defined(OFOPSYS_WIN32)

inline
int
compare_and_swap_int( volatile ofuint32 *loc, // Destination
					  ofuint32 oldV, // Comperand
					  ofuint32 newV // Exchange
					  )
{
	volatile long *locX = (long *)loc;
	ofuint32 ret = InterlockedCompareExchange( locX, newV, oldV );
	return (int)(ret == oldV);
}

inline
int
compare_and_swap_address( void *loc, void *oldV, void *newV )
{
	void *ret = InterlockedCompareExchangePointer( (void **)loc, newV, oldV );
	return (int)( ret == oldV );
}

#endif // defined(OFOPSYS_WIN32)

#if defined(OFPLATFORM_I386)
#if !defined(OFOPSYS_WIN32)
#define compare_and_swap_int(a,b,c) \
({ \
   volatile ofuint32 *loc = (a); \
   ofuint32 oldV = (b); \
   ofuint32 newV = (c); \
   unsigned char actV; \
   asm volatile ( \
      "  lock ; cmpxchgl %1, (%2); setz %0" \
      : "=A" (actV)  \
      : "r" (newV), "r" (loc), "a" (oldV) \
      : "memory" \
      ); \
   actV; \
})

#define compare_and_swap_address(a,b,c) \
({ \
   volatile void *loc = (a); \
   void *oldV = (b); \
   void *newV = (c); \
   unsigned char actV; \
   asm ( \
      "  lock ; cmpxchgl %1, (%2); setz %0" \
      : "=A" (actV) \
      : "r" (newV), "r" (loc), "a" (oldV) \
      : "memory" \
      ); \
   actV; \
})

#include <sched.h>

#define yield() sched_yield();
#endif // OFOPSYS_WIN32
#endif // OFPLATFORM_I386

#if defined(OFPLATFORM_PPCMAC)

inline int compare_and_swap_int(volatile ofuint32* ptr,
                         ofuint32 oldv,
                         ofuint32 newv)
{
    int prev;
    __asm__ __volatile__ ("1:      lwarx   %0,0,%2\n"
                          "        cmpw    0,%0,%3\n"
                          "        bne     2f\n"
                          "        stwcx.  %4,0,%2\n"
                          "        bne-    1b\n"
                          "        sync\n"
                          "2:"
                          : "=&r" (prev), "=m" (*ptr)
                          : "r" (ptr), "r" (oldv), "r" (newv), "m" (*ptr)
                          : "cc", "memory");
  
    return prev;
}

inline int compare_and_swap_pointer(volatile ofuint32* ptr,
                             ofuint32* oldv,
                             ofuint32* newv)
{
    int prev;
    __asm__ __volatile__ ("1:      lwarx   %0,0,%2\n"
                          "        cmpw    0,%0,%3\n"
                          "        bne     2f\n"
                          "        stwcx.  %4,0,%2\n"
                          "        bne-    1b\n"
                          "        sync\n"
                          "2:"
                          : "=&r" (prev), "=m" (*ptr)
                          : "r" (ptr), "r" (oldv), "r" (newv), "m" (*ptr)
                          : "cc", "memory");
  
    return prev;
}


#include <sched.h>

#define yield() sched_yield();

#endif // OFPLATFORM_PPCMAC

#if defined(OFPLATFORM_X86_64)

#if defined(OFOPSYS_SOLARIS)
#include <atomic.h>
#define compare_and_swap_int(a,b,c) \
    atomic_cas_uint(a,b,c)

#define compare_and_swap_address(a,b,c) \
    atomic_cas_ptr(a,b,c)

#else // OFOPSYS_SOLARIS
#define compare_and_swap_int(a,b,c) \
({ \
   volatile ofuint32 *loc = (a); \
   ofuint32 oldV = (b); \
   ofuint32 newV = (c); \
   unsigned char actV; \
   asm volatile ( \
      "  lock ; cmpxchg %1, (%2); setz %0" \
      : "=A" (actV)  \
      : "r" (newV), "r" (loc), "a" (oldV) \
      : "memory" \
      ); \
   actV; \
})

#define compare_and_swap_address(a,b,c) \
({ \
   volatile void *loc = (a); \
   void *oldV = (b); \
   void *newV = (c); \
   unsigned char actV; \
   asm ( \
      "  lock ; cmpxchg %1, (%2); setz %0" \
      : "=A" (actV) \
      : "r" (newV), "r" (loc), "a" (oldV) \
      : "memory" \
      ); \
   actV; \
})

#include <sched.h>

#define yield() sched_yield();

#endif // ! OFOPSYS_SOLARIS

#endif // OFPLATFORM_I86_64

#if defined(OFPLATFORM_SPARC)
#if defined(__GNUC__)

#define compare_and_swap_int(a,b,c) \
({ \
   volatile ofuint32 *loc = (a); \
   ofuint32 oldV = (b); \
   ofuint32 newV = (c); \
   ofuint32 actV; \
   asm ( \
      "cas [%2],%1,%0" \
      : "=" (actV) \
      : "g" (oldV), "g" (loc), "0" (newV) \
      ); \
   actV == oldV; \
})

#if defined(OFPLATFORM_64BIT_ADDRESSES)

#define compare_and_swap_address(a,b,c) \
({ \
   volatile void *loc = (a); \
   void *oldV = (b); \
   void *newV = (c); \
   void *actV; \
   asm ( \
      "casx [%2],%1,%0" \
      : "=" (actV) \
      : "g" (oldV), "g" (loc), "0" (newV) \
      ); \
   actV == oldV; \
})

#else // OFPLATFORM_64BIT_ADDRESSES

#define compare_and_swap_address(a,b,c) \
({ \
   volatile void *loc = (a); \
   void *oldV = (b); \
   void *newV = (c); \
   void *actV; \
   asm ( \
      "cas [%2],%1,%0" \
      : "=" (actV) \
      : "g" (oldV), "g" (loc), "0" (newV) \
      ); \
   actV == oldV; \
})

#endif // OFPLATFORM_64BIT_ADDRESSES

#else // __GNUC__

#endif // __GNUC__

#endif // OFPLATFORM_SOLARIS

// spinlocks

#ifndef UNIT_TEST

#if defined(OFOPSYS_WIN32)
#define spinLock(a,b,c) \
{ \
  ofuint32 *loct = (ofuint32 *)&(a); \
  ofuint32 flag = (b); \
  ofuint32 count = (c); \
  ofuint32 test; \
  for (;;) { \
    if (count == 0) \
		count = (c); \
    count--; \
    test = *loct; \
    if ( ( test & flag ) != 0) \
		continue; \
    if ( compare_and_swap_int( loct, test, test | flag ) ) \
		break; \
  } \
}
#else
#define spinLock(a,b,c) \
{ \
  ofuint32 *loct = (ofuint32 *)&(a); \
  ofuint32 flag = (b); \
  ofuint32 count = (c); \
  ofuint32 test; \
  for (;;) { \
    if (count == 0) { yield(); count = (c);} \
    count--; \
    test = *loct; \
    if ((test & flag) != 0) continue; \
    if (compare_and_swap_int(loct, test, test | flag)) break; \
  } \
}
#endif // OFOPSYS_WIN32
#else
#define spinLock(a,b,c) \
{ \
  ofuint32 *loct = (ofuint32 *)&(a); \
  ofuint32 flag = (b); \
  ofuint32 count = (c); \
  ofuint32 count1 = 0; \
  ofuint32 test; \
  for (;;) { \
    if (count == 0) { yield(); count = (c);if (++count1 & 10000 == 0) printf ("%d yields\n", (int)count1);} \
    count--; \
    test = *loct; \
    if ((test & flag) != 0) continue; \
    if (compare_and_swap_int(loct, test, test | flag)) break; \
  } \
}
#endif

#define spinUnlock(a,b) \
{ \
  ofuint32 *loct = (ofuint32 *)&(a); \
  ofuint32 flag = (b); \
  ofuint32 test; \
  for (;;) { \
    test = *loct; \
    if (compare_and_swap_int(loct, test, test & ~flag)) break; \
  } \
}

#endif // CAS_H
