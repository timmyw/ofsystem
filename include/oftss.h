/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: oftss.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
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
