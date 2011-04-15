/*
 * $OFPREPARED$
 *
 * $Revision: 2.1 $
 *
 * $Id: ofdll.h,v 2.1 2007-09-27 12:29:37 timmy Exp $
 */

#if !defined(_OF_DLL_H_)
#define _OF_DLL_H_

/** \file ofdll.h
    Contains the declaration of the OFDLL class.
*/

/** \class OFDLL ofdll.h

    Implements a wrapper around handling shared libraries (DLLs)
    Applications can dynamically load shared libraries at run-time,
    retrieve the entry poofint32 for a function, call that function,
    and if necessary, unload the library.

    \bNote Libraries created under linux must use the -fPIC flag to
    create position independant code.
*/

class OFSYS_API OFDLL
{
 public:
    typedef ofint32 (OF_THREADAPI *OF_DLL_FUNC)(void);

    /** Constructor.
        Zeros the member properties.
    */
    OFDLL( );

    /** Opening constructor.
        Opens the specified library.
        \param path Specifies the path to the shared library. (Note. On Win32
        platforms, the extension ".dll" is not necessary, and will be appended
        automatically.)
        \param autoClose If true, the library is automatically unloaded when the
        class is deleted or goes out of scope.
    */
    OFDLL( const char *path, bool autoClose = true );

    /** Destructor.
        If the library has been loaded, and the autoClose parameter
        to the constructor or open() was true, the library will be
        unloaded.
    */
    ~OFDLL( );

    /** Opens the specified library.
        \param path Specifies the path to the shared library. (Note. On Win32
        platforms, the extension ".dll" is not necessary, and will be appended
        automatically.)
        \param autoClose If true, the library is automatically unloaded when the
        class is deleted or goes out of scope.
        \return 1 if successful.
    */
    ofint32 open( const char *path, bool autoClose = true );

    /** Checks if the library is currently open.
        \return True if the library was successfully opened.
    */
    bool isOpen() const;

    /** Retrieve the procedure address for the specified function.
        \param funcname The name of the function. (Note. On Win32 platforms
        this name must match (case-sensitive) the name defined in the exports table,
        normally in the .def file.)
    */
    void * symbol( const char *funcname );

    /** Close the library.
        Unloads the library. Note that any function pointers
        retrieved from this library will be invalid. (Note. This may not physically
        unload the library, as it may have been loaded more than once by the 
        process. Only when the process has unloaded for every time it loaded,
        will the library be removed from memory.)
    */
    void close( );

 protected:
    char m_name[OF_MAX_DLL_NAME+1];

#if defined(OFOPSYS_WIN32)
    HINSTANCE m_handle;
#elif defined(OFOPSYS_LINUX) || defined(OFOPSYS_SOLARIS) || defined(OFOPSYS_FREEBSD) || defined(OFOPSYS_DARWIN)
    void *m_handle;
#else
#error Undefined platform
#endif
    bool m_autoClose;
    bool m_open;
};

#endif // #if !defined(_OF_DLL_H_)
