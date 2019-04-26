/* ERT_LOG.h Varadic logging macros
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a
 * compiled binary, for any purpose, commercial or non-commercial, and
 * by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * Contains varadic macros for variable levels of logging. Verbosity
 * of logging is dictated by the value of LOGLEVEL.
 *
 *     #include <ert_log.h>
 *
 *     log_err(const char *fmt, ...);
 *     log_warn(const char *fmt, ...);
 *     log_info(const char *fmt, ...);
 *     log_debug(const char *fmt, ...);
 *
 *     LOGLEVEL == 0, only error messages.
 *     LOGLEVEL == 1, extra warning messages.
 *     LOGLEVEL == 2, extra verbose information (all with colour).
 *     LOGLEVEL == 3, extra debugging information (all with colour).
 *
 * This header provides the function-like macros listed above. The
 * macros write outputs (to stderr or stdout dependant on the
 * function) according to a format string that specifies how
 * subsequent arguments are converted for output (see man(3)
 * printf).
 *
 * The output is prefixed with a descriptive tag, followed by function,
 * file and line number. When LOGLEVEL is greater than 1, logs have
 * appropriate ANSI colour formatting. Error and warning macros print
 * and reset errno to zero.
 *
 * log_err(const char *fmt, ...)
 *
 * Outputs to stderr prefixed with [ERROR] all all values of LOGLEVEL
 * or if it is not defined. Font colour is red if LOGLEVEL <
 * 1. Outputs errno when non zero, outputs "No errno" when zero.
 *
 * log_warn(const char *fmt, ...)
 *
 * Outputs to stderr prefixed with [WARN] if LOGLEVEL > 0. Font colour
 * is orange if LOGLEVEL > 2. Outputs errno when non zero, outputs "No
 * errno" when zero.
 *
 * log_info(const char *fmt, ...)
 *
 * Outputs to stdout prefixed with [INFO] if LOGLEVEL > 1. Font colour
 * is green.
 *
 * log_info(const char *fmt, ...)
 *
 * Outputs to stdout prefixed with [DEBUG] if LOGLEVEL > 2. Font
 * colour is blue.
 *
 */

#ifndef ERT_LOG_H
#define ERT_LOG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>		/* for strerror() */

#ifndef LOGLEVEL
#define LOGLEVEL = 0
#endif	/* LOGLEVEL */

#if LOGLEVEL > 1		/* Define ANSI terminal colours */
#define RED        "\x1b[1;31m"
#define YELLOW     "\x1b[1;33m"
#define GREEN      "\x1b[1;32m"
#define CYAN       "\x1b[1;36m"
#define BOLD       "\x1b[1m"
#define ANSI_RESET "\x1b[0m"
#endif	/* LOGLEVEL > 1 */

#if LOGLEVEL < 2		/* Do not define colours */
#define RED          ""
#define YELLOW	     ""
#define GREEN	     ""
#define CYAN	     ""
#define BOLD	     ""
#define ANSI_RESET   ""
#endif	/* LOGLEVEL < 2 */

/* Defaults to LOGLEVEL 0 (plain text errors only) */
#ifndef LOGLEVEL
#define LOGLEVEL 0
#endif

/* Temporary storage for errno is required to avoid fprintf() altering
   it. See man(3) errno . */
static int __errno = 0;

/**
    Macros used internally
**/

#define ERT_LOG(STREAM, TITLE, MSG, ...) do {				\
	fprintf(STREAM, TITLE						\
		" in " BOLD "%s" ANSI_RESET				\
		" at " BOLD "%s:%d: " ANSI_RESET  MSG,			\
		__func__, __FILE__, __LINE__,				\
		##__VA_ARGS__);						\
    } while (0)

#define STORE_ERRNO __errno = errno
#define CLEAN_ERRNO errno = 0; __errno = 0

#define ERRNO_MSG() do {					\
	fprintf(stderr, BOLD);					\
	fprintf(stderr, " (%s)\n",				\
		__errno == 0 ? "No errno" : strerror(__errno));	\
	fprintf(stderr, ANSI_RESET);				\
	CLEAN_ERRNO;						\
    } while (0)						

/**
   Level 0 - Errors
**/

#define log_err(MSG, ...) do {						\
	_Pragma("GCC diagnostic ignored \"-Wformat-zero-length\"");	\
	STORE_ERRNO;							\
	ERT_LOG(stderr, RED "[ERROR]" ANSI_RESET,			\
		MSG, ##__VA_ARGS__);					\
	ERRNO_MSG();							\
	_Pragma("GCC diagnostic error \"-Wformat-zero-length\"");	\
    } while (0)

/**
   Level 1 - Warnings 
**/

#if LOGLEVEL >= 1		
#define log_warn(MSG, ...) do{						\
	_Pragma("GCC diagnostic ignored \"-Wformat-zero-length\"");	\
	STORE_ERRNO;							\
	ERT_LOG(stderr, YELLOW "[WARN]" ANSI_RESET,			\
		MSG, ##__VA_ARGS__);					\
	ERRNO_MSG();							\
	_Pragma("GCC diagnostic error \"-Wformat-zero-length\"");	\
    } while (0)
#else
#define log_warn(MSG, ...) do { } while (0)
#endif	/* LOGLEVEL >= 1 */

/**
   Level 2 - Information 
**/

#if LOGLEVEL >= 2		
#define log_info(MSG, ...) do {				\
	ERT_LOG(stdout, GREEN "[INFO]" ANSI_RESET,	\
		MSG "\n", ##__VA_ARGS__);		\
    } while(0)
#else
#define log_info(MSG, ...) do { } while (0)
#endif	/* LOGLEVEL >= 2 */

/**
   Level 3 - Debugging logs
**/

#if LOGLEVEL >= 3
#define log_debug(MSG, ...) do {			\
	ERT_LOG(stdout, CYAN "[DEBUG]" ANSI_RESET,	\
		MSG "\n", ##__VA_ARGS__);		\
    } while (0)
#else
#define log_debug(MSG, ...) do { } while (0)
#endif	/* LOGLEVEL >= 3 */

#endif	/* ERT_LOG_H */
