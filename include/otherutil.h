/*
* $OFPREPARED$
*
* $Revision: 2.0 $
*
* $Id: otherutil.h,v 2.0 2006-01-01 20:08:32 timmy Exp $
*/

#if !defined(_OTHER_UTIL_H_)
#define _OTHER_UTIL_H_

#if !defined(OFSYS_API)
# if defined(_WIN32)
#  if defined(OFEXPORTS)
#   define OFSYS_API  __declspec(dllexport)
#  else
#   define OFSYS_API  __declspec(dllimport)
#  endif
# else // non Win32
#  define OFSYS_API
# endif
#endif

/* template<class T> T reversebits( T in ); */
/* template<class T> void printbin( T x ); */

#include <ofsys/otherutil.i>

#endif // _OTHER_UTIL_H_
