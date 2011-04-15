/*
 */

#if !defined(_OFTYPES_H_)
#define _OFTYPES_H_


typedef short ofint16;
typedef unsigned short ofuint16;

typedef int ofint;
typedef unsigned int ofuint;

#if defined(OFPLATFORM_I386) || defined(OFPLATFORM_PPCMAC) || defined(OFPLATFORM_SPARC)
typedef long ofint32;
typedef unsigned long ofuint32;
typedef unsigned long ofpointer;
typedef unsigned long oflong;
#define LFORMAT "l"
#endif

#if defined(OFPLATFORM_X86_64)
typedef int ofint32;
typedef unsigned ofuint32;
typedef unsigned long long ofpointer;
typedef unsigned long long oflong;
#define OFLLONG long long
#define LFORMAT ""
#endif

typedef unsigned char ofchar;

#if defined(OFOPSYS_WIN32)
typedef unsigned __int64 ofuint64;
typedef __int64 ofint64;
#define OFLLONG __int64
#else
typedef unsigned long long ofuint64;
typedef long long ofint64;
#define OFLLONG long long
#endif // OFPLATFORM_WIN32

/** \file ofsys.h
    Entry point header file for the ofsys library.
    Also contains some simple declarations.
*/

/// simple portable max function    
template<class T> const T& maximum(const T& x, const T& y) {return x > y ? x : y;}

/// simple portable min function    
template<class T> const T& minimum(const T& x, const T& y) {return x < y ? x : y;}

#if !defined(OFOPSYS_WIN32)
typedef void *OF_THR_RETTYPE;
#define OF_THREADAPI
typedef OF_THR_RETTYPE (OF_THREADAPI *OF_THR_FUNC)(void *);
#else
typedef ofuint32 OF_THR_RETTYPE;
#define OF_THREADAPI __stdcall
typedef OF_THR_RETTYPE (OF_THREADAPI *OF_THR_FUNC)(void *);
#endif

/** \typedef OFCHARSTARLIST ofsys.h
    Typedef for a vector of char * pointers.
*/
typedef std::vector<char *> OFCHARSTARLIST;

typedef OFCHARSTARLIST::iterator OFCHARSTARIterator;
typedef OFCHARSTARLIST::const_iterator OFCHARSTARIteratorC;

/** \typedef STRINGMAP ofsys.h
    Map of two strings
*/
typedef std::map<std::string,std::string> STRINGMAP;

#if defined(OFOPSYS_WIN32)
#define bcopy(src,dest,n) \
	memmove( dest, src, n );
#endif

#endif // _OFTYPES_H_

