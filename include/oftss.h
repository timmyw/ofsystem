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

#if !defined(_OF_TSS_H_)
#define _OF_TSS_H_

/** \file oftss.h
    Contains the declaration of the OFTSS_T template and OFTSS class.
*/

#include <ofos.h>

/** \class OFTSS_T oftss.h
    Implementation of thread local storage (thread specific storage on pthreads.)
*/

template <class TYPE>
class OFSYS_API OFTSS_T
{
 public:
    /** Constructor.
        If the \c initial parameter is supplied it used to initialise the 
        value for the calling thread.
        \param initial The initial value for the TLS.
    */
    inline OFTSS_T( TYPE *initial = 0 );

    /** Destructor.
     */
    inline ~OFTSS_T( );

    /** Retrieve the TLS value.
     */
    inline TYPE *ts_object( );

    /** Set the TLS value.
     */
    inline void ts_object( TYPE *value );
 private:
    OFOS::of_tls_t m_index;
};

/**
   \class OFTSS oftss.h
   Implementation of thread local storage (thread specific storage on pthreads.)
*/
class OFSYS_API OFTSS
{
 public:
    /** Constructor.
        If the \c initial parameter is supplied it used to initialise the 
        value for the calling thread.
        \param initial The initial value for the TLS.
    */
    OFTSS( void *initial = 0 );

    /** Destructor.
     */
    ~OFTSS( );

    /** Retrieve the TLS value.
     */
    void *ts_object( );

    /** Set the TLS value.
     */
    void ts_object( void *value );
 private:
    OFOS::of_tls_t m_index;
};

#include <oftss.i>

#endif // #if !defined(_OF_TSS_H_)
