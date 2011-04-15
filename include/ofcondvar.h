/*
 * $OFPREPARED$
 *
 * $Revision: 2.0 $
 *
 * $Id: ofcondvar.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
 */

#if !defined(_OF_COND_VAR_H_)
#define _OF_COND_VAR_H_

class OFConditionVar; // forward
class OFMutex; // forward

typedef struct tagOFCONDVAR
{
    OFConditionVar *condvar;
    char name[OF_MAX_COND_VAR_NAME+1];
} OFCONDVAR;

#if defined(__GNUC__)
/* This is here to stop getting annoyed with the gnu compiler. */
class OFConditionVarFriend{};
#endif

/**
   \class OFConditionVar ofcondvar.h
   Cross platform implementation of a condition variable. This class
   uses pthread_cond_t on platforms which support pthreads and events 
   on Win32 platforms.

   A condition variable is used to force one or more threads to wait
   for a variable to be signalled (by another thread.) Only one
*/
class OFSYS_API OFConditionVar
{
 public:
     enum
	  {
	       CONDVAR_INFINITE = -1,
	       CONDVAR_TIMEOUT = 1,
	       CONDVAR_SIGNALLED = 2
	  };
	
     /** Constructor.
	 Creates a new condition variable.
     */
     OFConditionVar();

     /** Destructor.
     */
     ~OFConditionVar( );

     /** Signal the condition variable.
      */
     void signal( );

     /** Wait for the condition variable.
	 \param milli The number of milliseconds to wait for a signalled state.
	 If the parameter milli is CONDVAR_INFINITE, the method will
	 block and wait indefinitely.

	 \return CONDVAR_SIGNALLED if the condition variable is signalled or
	 CONDVAR_TIMEOUT if the wait timedout, or 0 if an error occurred.
     */
     ofint32 wait( OFMutex &mutex);
     ofint32 wait( OFMutex &mutex, ofuint32 milli );
	
 protected:
#if defined(OFOPSYS_WIN32)
    HANDLE m_cond;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
     pthread_cond_t m_cond;
#endif
};

#endif

