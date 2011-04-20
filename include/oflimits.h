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

#if !defined(_OFLIMITS_H_)
#define _OFLIMITS_H_

#if defined(OFOPSYS_WIN32)
#define OF_MAX_PATH_LEN  _MAX_PATH
#else
#define OF_MAX_PATH_LEN  512
#endif // defined(OFOPSYS_WIN32)

#define DEFAULT_TEMP_SIZE 1024
#define DEFAULT_BIG_TEMP 16384

#define OF_MAX_MUTEX_NAME               255
#define OF_MAX_COND_VAR_NAME            255

#define OF_MAX_DLL_NAME                 OF_MAX_PATH_LEN

#define OF_MAX_LOG_NAME_LEN             128

#define OF_LOG_LINE_LENGTH 1024

/** Size of socket send buffer
 */
#define OF_SOCK_SEND_BUFSIZE            8192

/** Size of socket receive buffer
 */
#define OF_SOCK_RECV_BUFSIZE            8192

#endif // _OFLIMITS_H_
