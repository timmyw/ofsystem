/*
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
