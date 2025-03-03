/* 
 * dhcpcd - DHCP client daemon
 * Copyright (c) 2006-2011 Roy Marples <roy@marples.name>
 * All rights reserved

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef COMMON_H
#define COMMON_H

#include <sys/time.h>
#include <stdio.h>

#include "config.h"
#include "defs.h"

/*junbinwang add 20140402*/
#define DHCP_USE_NEW_SEND_MSG_TIME

#define UNCONST(a)		((void *)(unsigned long)(const void *)(a))

#define timeval_to_double(tv) ((tv)->tv_sec * 1.0 + (tv)->tv_usec * 1.0e-6)
#define timernorm(tvp)							\
	do {								\
		while ((tvp)->tv_usec >= 1000000) {			\
			(tvp)->tv_sec++;				\
			(tvp)->tv_usec -= 1000000;			\
		}							\
	} while (0 /* CONSTCOND */);

#if __GNUC__ > 2 || defined(__INTEL_COMPILER)
# define _noreturn __attribute__((__noreturn__))
# define _packed   __attribute__((__packed__))
# define _unused   __attribute__((__unused__))
#else
# define _noreturn
# define _packed
# define _unused
#endif

/* We don't really need this as our supported systems define __restrict
 * automatically for us, but it is here for completeness. */
#ifndef __restrict
# if defined(__lint__)
#  define __restrict
# elif __STDC_VERSION__ >= 199901L
#  define __restrict restrict
# elif !(2 < __GNUC__ || (2 == __GNU_C && 95 <= __GNUC_VERSION__))
#  define __restrict
# endif
#endif

int set_cloexec(int);
int set_nonblock(int);
char *get_line(FILE * __restrict);
extern int clock_monotonic;
int get_monotonic(struct timeval *);
ssize_t setvar(char ***, const char *, const char *, const char *);
ssize_t setvard(char ***, const char *, const char *, int);
time_t uptime(void);
int writepid(int, pid_t);
void *xrealloc(void *, size_t);
void *xmalloc(size_t);
void *xzalloc(size_t);
char *xstrdup(const char *);

/* Uncomment the #def below to send DHCPCD syslog messages to Android's logcat
 * instead.  */
/* #define REDIRECT_SYSLOG_TO_ANDROID_LOGCAT */
#ifdef REDIRECT_SYSLOG_TO_ANDROID_LOGCAT

#define LOG_TAG "DHCPCD"
#include <utils/Log.h>

#undef LOG_EMERG
#undef LOG_ALERT
#undef LOG_CRIT
#undef LOG_ERR
#undef LOG_WARNING
#undef LOG_NOTICE
#undef LOG_INFO
#undef LOG_DEBUG

#define LOG_EMERG   ANDROID_LOG_FATAL
#define LOG_ALERT   ANDROID_LOG_FATAL
#define LOG_CRIT    ANDROID_LOG_FATAL
#define LOG_ERR     ANDROID_LOG_ERROR
#define LOG_WARNING ANDROID_LOG_WARN
#define LOG_NOTICE  ANDROID_LOG_WARN
#define LOG_INFO    ANDROID_LOG_INFO
#define LOG_DEBUG   ANDROID_LOG_DEBUG
#define syslog(a, b...) android_printLog(a, LOG_TAG, b)

#endif  /* REDIRECT_SYSLOG_TO_ANDROID_LOGCAT */

#endif
