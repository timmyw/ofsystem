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

#endif // _OFLIMITS_H_
