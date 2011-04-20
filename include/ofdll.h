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
