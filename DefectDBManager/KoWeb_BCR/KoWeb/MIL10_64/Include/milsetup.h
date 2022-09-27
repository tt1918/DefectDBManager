/************************************************************************/
/*
*
* Filename     :  MILSETUP.H
* Revision     :  10.30.0595
* Content      :  This file contains definitions for specifying the target
*                 compile environment and the default state to set for
*                 MIL (Matrox Imaging Library). It also defines the
*                 MappAllocDefault() and MappFreeDefault() macros.
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2018.
* All Rights Reserved
*************************************************************************/
#ifndef __MILSETUP_H__
#define __MILSETUP_H__

/************************************************************************/
/* MIL COMPILE VERSION                                                  */
/* These defines are for Matrox internal purpose only and are NOT meant */
/* to be edited by the client.                                          */
/************************************************************************/
#if !defined(MIL_COMPILE_VERSION_HEX) || (MIL_COMPILE_VERSION_HEX== 0)
   #ifdef MIL_COMPILE_VERSION_HEX
      #undef MIL_COMPILE_VERSION_HEX
   #endif
   #define MIL_COMPILE_VERSION_HEX 0x1030
#endif

// Let's put the hex value in decimal.
#define MIL_COMPILE_VERSION ((((MIL_COMPILE_VERSION_HEX >> 12) & 0xf) * 1000) + (((MIL_COMPILE_VERSION_HEX >> 8) & 0xf) * 100) + (((MIL_COMPILE_VERSION_HEX >> 4) & 0xf) * 10) + (((MIL_COMPILE_VERSION_HEX) & 0xf) ))


/************************************************************************/
/* COMPILATION FLAG                                                     */
/* One and only one flag must be active                                 */
/************************************************************************/
#ifndef M_MIL_USE_OS
   #define M_MIL_USE_OS               1

#ifndef M_MIL_USE_RT

#if defined(UNDER_RTSS)
#define M_MIL_USE_RT    1
#else
#define M_MIL_USE_RT    0
#endif

#endif

   #if defined(__linux__)
      #if defined(__arm__) || defined(__aarch64__)
         #define M_MIL_USE_ARM        1
      #else
         #define M_MIL_USE_ARM        0
      #endif
      #define M_MIL_USE_WINDOWS       0
      #define M_MIL_USE_LINUX         1
      #define M_MIL_USE_NEW_STDHEADERS 1
      #if defined(__x86_64__) || defined(__aarch64__)
         #define M_MIL_USE_64BIT      1
      #else
         #define M_MIL_USE_64BIT      0
      #endif

      #ifndef M_LINUX_KERNEL
         #define M_LINUX_KERNEL       0
      #endif

      #ifdef __INTEL_COMPILER
         #define M_MIL_USE_LINTEL     1
      #endif
   #else
      #define M_MIL_USE_WINDOWS       1
      #define M_MIL_USE_LINUX         0
      #define M_MIL_USE_ARM           0
      #if defined(_WIN64) || defined(WIN64)
         #define M_MIL_USE_64BIT      1
      #else
         #define M_MIL_USE_64BIT      0
      #endif

      #if (_MSC_VER >= 1400) && !defined(M_MIL_USING_SAFE_CRT)
         #define M_MIL_USING_SAFE_CRT       1
      #endif
   #endif

   #if defined(__POWERPC__)
#error "PowerPC not supported"
   #endif
#endif

#if !(M_MIL_USE_WINDOWS || M_MIL_USE_LINUX)
#error "You must set at least one compilation flag."
#endif

#if ( (M_MIL_USE_WINDOWS && M_MIL_USE_LINUX) )
#error "You must select one and only one compilation flag."
#endif


#if M_MIL_USE_RT
#define MIL_DLLFUNC //__declspec(dllimport)
#else
#define MIL_DLLFUNC
#endif
/************************************************************************/
/*This flag indicates if MIL has been compiled without UNICODE support  */
/*If MIL supports UNICODE, user can define UNICODE to use UNICODE API   */
/************************************************************************/
#if !defined(M_MIL_USE_UNICODE) && !M_MIL_USE_LINUX
   #define M_MIL_USE_UNICODE 1
#elif !defined (M_MIL_USE_UNICODE)
   #define M_MIL_USE_UNICODE 0
#endif

#if  M_MIL_USE_UNICODE
   #if defined(UNICODE)
      #define M_MIL_UNICODE_API 1
   #else
#if M_MIL_USE_RT
#error "The project must be compiled with Unicode character set"
#endif
      #define M_MIL_UNICODE_API 0
   #endif
#endif

#if M_MIL_USE_LINUX || M_MIL_UNICODE_API
#define M_MIL_USE_TTF_UNICODE 1
#endif

/************************************************************************/
/*This flag enable or disable deprecated defines.                       */
/************************************************************************/
#if !defined(M_MIL_WARN_ON_DEPRECATED)
#if (defined(_MSC_VER) && (_MSC_VER >= 1400)) || defined(__INTEL_COMPILER)
#define M_MIL_WARN_ON_DEPRECATED 1
#else
#define M_MIL_WARN_ON_DEPRECATED 0
#endif
#endif

#if M_MIL_WARN_ON_DEPRECATED
   // Define true (1) or false (2) depends on MIL_COMPILE_VERSION and START_VERSION.
   #if MIL_COMPILE_VERSION >= 1000
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1000 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1000 0
   #endif
   #if MIL_COMPILE_VERSION >= 1010
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1010 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1010 0
   #endif
   #if MIL_COMPILE_VERSION >= 1020
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1020 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1020 0
   #endif
   #if MIL_COMPILE_VERSION >= 1030
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1030 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1030 0
   #endif
   #if MIL_COMPILE_VERSION >= 1040
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1040 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1040 0
   #endif
   #if MIL_COMPILE_VERSION >= 1060
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1060 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1060 0
   #endif
   #if MIL_COMPILE_VERSION >= 1100
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1100 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1100 0
   #endif

   #define PRIMITIVE_CAT(X, ...)                     X ## __VA_ARGS__
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO(X) PRIMITIVE_CAT(MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_, X)
   #define IIF(X)                                    PRIMITIVE_CAT(IIF_, X)
   #define IIF_0(X, ...)                             __VA_ARGS__
   #define IIF_1(X, ...)                             X

   #define MIL_DEPRECATED(IDENTIFIER, START_VERSION) IIF(MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO(START_VERSION)) (__pragma(deprecated(#IDENTIFIER)), /*nothing*/)
#else
   #define MIL_DEPRECATED(IDENTIFIER, START_VERSION)
#endif


#ifndef M_MIL_WARN_ON_DEPRECATED_MOS
   #define M_MIL_WARN_ON_DEPRECATED_MOS 1
#endif


#ifndef M_MILIM_FUNCTION_DEPRECATION_WARNING
   #if defined(OldDefinesSupport) && OldDefinesSupport
      #define M_MILIM_FUNCTION_DEPRECATION_WARNING 1
   #else
      #define M_MILIM_FUNCTION_DEPRECATION_WARNING 0
   #endif
#endif

/************************************************************************/
/* DEFAULT STATE INITIALIZATION FLAG                                    */
/************************************************************************/

//Defines for INSTALLDIR support
#define M_INSTALL_DIR    MIL_TEXT("///INSTALLDIR///")
#if MIL_COMPILE_VERSION < 1060
#define INSTALL_DIR    M_INSTALL_DIR
#endif
#define M_TEMP_DIR     MIL_TEXT("///MILTEMPDIRDIR///")
#define M_USER_DLL_DIR MIL_TEXT("///MILUSERDLLDIR///")

#if   M_MIL_USE_LINUX
   #define M_IMAGE_PATH               M_INSTALL_DIR MIL_TEXT("images/")
   #define M_CONTEXT_PATH             M_INSTALL_DIR MIL_TEXT("contexts/")
#else
   #define M_IMAGE_PATH               M_INSTALL_DIR MIL_TEXT("images\\")
   #define M_CONTEXT_PATH             M_INSTALL_DIR MIL_TEXT("contexts\\")
#endif

/************************************************************************/
/* DEFAULT SYSTEM SPECIFICATIONS                                        */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_SYSTEM_NUM           M_DEFAULT
   MIL_DEPRECATED(M_DEF_SYSTEM_NUM, 1000)
   #define M_SYSTEM_SETUP             M_DEF_SYSTEM_TYPE
   MIL_DEPRECATED(M_SYSTEM_SETUP, 1000)
   #define M_DEF_SYSTEM_TYPE          M_SYSTEM_DEFAULT
   MIL_DEPRECATED(M_DEF_SYSTEM_TYPE, 1000)
#endif

/************************************************************************/
/* DEFAULT DIGITIZER SPECIFICATIONS                                     */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_DIGITIZER_INIT       M_DEFAULT
   MIL_DEPRECATED(M_DEF_DIGITIZER_INIT, 1000)
   #define M_DEF_DIGITIZER_NUM        M_DEFAULT
   MIL_DEPRECATED(M_DEF_DIGITIZER_NUM, 1000)
   #define M_CAMERA_SETUP             M_DEF_DIGITIZER_FORMAT
   MIL_DEPRECATED(M_CAMERA_SETUP, 1000)
   #define M_DEF_DIGITIZER_FORMAT     MIL_TEXT("M_DEFAULT")
   MIL_DEPRECATED(M_DEF_DIGITIZER_FORMAT, 1000)
#endif

/************************************************************************/
/* DEFAULT DISPLAY SPECIFICATIONS                                       */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_DISPLAY_FORMAT               MIL_TEXT("M_DEFAULT")
   #define M_DISPLAY_SETUP                    M_DEF_DISPLAY_FORMAT
   MIL_DEPRECATED(M_DISPLAY_SETUP, 1000)
   MIL_DEPRECATED(M_DEF_DISPLAY_FORMAT, 1000)
   #define M_DEF_DISPLAY_INIT                 M_DEFAULT
   MIL_DEPRECATED(M_DEF_DISPLAY_INIT, 1000)
   #define M_DEF_DISPLAY_NUM                  M_DEFAULT
   MIL_DEPRECATED(M_DEF_DISPLAY_NUM, 1000)
#endif

/************************************************************************/
/* DEFAULT IMAGE BUFFER SPECIFICATIONS                                  */
/************************************************************************/
#if OldDefinesSupport
   #define M_DEF_IMAGE_NUMBANDS_MIN   1
   MIL_DEPRECATED(M_DEF_IMAGE_NUMBANDS_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_X_MIN     50
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_X_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_Y_MIN     50
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_Y_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_X_MAX     4000000
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_X_MAX, 1000)
   #define M_DEF_IMAGE_SIZE_Y_MAX     4000000
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_Y_MAX, 1000)
   #define M_DEF_IMAGE_TYPE           (8+M_UNSIGNED)
   MIL_DEPRECATED(M_DEF_IMAGE_TYPE, 1000)
   #define M_DEF_IMAGE_ATTRIBUTE_MIN  (M_IMAGE+M_PROC)
   MIL_DEPRECATED(M_DEF_IMAGE_ATTRIBUTE_MIN, 1000)
#endif

/************************************************************************/
/* Enables DMIL monitoring auto-publish mode. 0 == Disabled             */
/************************************************************************/
#define M_DEF_AUTO_PUBLISH  0


#endif /*__MILSETUP_H__*/
