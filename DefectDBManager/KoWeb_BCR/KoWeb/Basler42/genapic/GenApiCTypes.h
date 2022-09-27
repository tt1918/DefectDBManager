/*-----------------------------------------------------------------------------
(c) 2009 by Basler Vision Technologies
Section: Basler Components
Project: PYLON
Author:  AH, TK
-----------------------------------------------------------------------------*/

/**
 * \file
 * \brief    Platform-dependent type definitions.
 * Define a subset of the types and macros that are provided by the
 * stdint.h header on systems that comply with the C99 standard. This
 * is meant to aid in portability.
**/

#ifndef GENAPIC_TYPES_H
#define GENAPIC_TYPES_H

#if __STDC_VERSION__ >= 199901L

/* Compiler is c99-compliant. */
#   define  __STDC_LIMIT_MACROS
#   define  __STDC_CONSTANT_MACROS
#   include <inttypes.h>
#   include <stdint.h>

#elif defined (_WIN32)

/*! Portable 8 bit-wide signed integer type. */
typedef __int8 int8_t;
/*! Portable 8 bit-wide unsigned integer type. */
typedef unsigned __int8 uint8_t;

/*! Portable 16 bit-wide signed integer type. */
typedef __int16 int16_t;
/*! Portable 16 bit-wide unsigned integer type. */
typedef unsigned __int16 uint16_t;

/*! Portable 32 bit-wide signed integer type. */
typedef __int32 int32_t;
/*! Portable 32 bit-wide unsigned integer type. */
typedef unsigned __int32 uint32_t;

/*! Portable 64 bit-wide signed integer type. */
typedef __int64 int64_t;
/*! Portable 64 bit-wide unsigned integer type. */
typedef unsigned __int64 uint64_t;

#if defined(_WIN64)
/*! Portable signed integer type capable of holding a pointer. */
typedef __int64 intptr_t;
/*! Portable unsigned integer type capable of holding a pointer. */
typedef unsigned __int64 uintptr_t;
#else
/*! Portable signed integer type capable of holding a pointer. */
typedef __int32 intptr_t;
/*! Portable unsigned integer type capable of holding a pointer. */
typedef unsigned __int32 uintptr_t;
#endif

/*! Mimic c99 built-in _Bool type. */
typedef unsigned char _Bool;

#else
#   error No platform-specific type definitions
#endif /* __STDC_VERSION__ >= 199901L */



#ifndef INT64_MAX
#   define INT64_MAX   0x7fffffffffffffffLL    /* Maximum signed int64 value. */
#endif

#ifndef INT64_MIN
#   define INT64_MIN   0x8000000000000000LL    /* Minimum signed int64 value. */
#endif

#ifndef UINT64_MAX
#   define UINT64_MAX  0xffffffffffffffffULL   /* Maximum unsigned int64 value. */
#endif

#ifndef INT32_MAX
#   define INT32_MAX   0x000000007fffffffLL    /* Maximum signed int32 value. */
#endif

#ifndef INT32_MIN
#   define INT32_MIN   0xffffffff80000000LL    /* Minimum signed int32 value. */
#endif

#ifndef UINT32_MAX
#   define UINT32_MAX  0x00000000ffffffffULL   /* Maximum unsigned int32 value. */
#endif

#ifndef INT8_MAX
#   define INT8_MAX    0x000000000000007fLL    /* Maximum signed int8 value. */
#endif

#ifndef INT8_MIN
#   define INT8_MIN    0xffffffffffffff80LL    /* Minimum signed int8 value. */
#endif

#ifndef UINT8_MAX
#   define UINT8_MAX   0x00000000000000ffULL   /* Maximum unsigned int8 value. */
#endif


#endif /* GENAPIC_TYPES_H */
