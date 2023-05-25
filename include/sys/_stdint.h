#ifndef MACHINE_STDINT_H
#define MACHINE_STDINT_H

#include <stddef.h>

#ifndef _SIZE_T_DECLARED
typedef	size_t __size_t;
#define	_SIZE_T_DECLARED
#endif

#ifndef _SSIZE_T_DECLARED
typedef short ssize_t;
typedef	ssize_t __ssize_t;
#define	_SSIZE_T_DECLARED
#endif

typedef signed char __int_least8_t;
typedef short __int_least16_t;
typedef long __int_least32_t;
typedef long long __int_least64_t;

typedef unsigned char __uint_least8_t;
typedef unsigned short __uint_least16_t;
typedef unsigned long __uint_least32_t;
typedef unsigned long long __uint_least64_t;

typedef signed char __int_fast8_t;
typedef short __int_fast16_t;
typedef long __int_fast32_t;
typedef long long __int_fast64_t;

typedef unsigned char __uint_fast8_t;
typedef unsigned short __uint_fast16_t;
typedef unsigned long __uint_fast32_t;
typedef unsigned long long __uint_fast64_t;

typedef long __intmax_t;
typedef unsigned long __uintmax_t;

typedef short __intptr_t;
typedef unsigned short __uintptr_t;

#endif
