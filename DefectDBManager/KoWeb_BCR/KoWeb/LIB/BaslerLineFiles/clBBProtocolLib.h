//==============================================================================
//  (c) 2009 - 2010 by Basler Vision Technologies
//==============================================================================
/**
\file  

\brief Functions and structures to use clBBProtocol-API

\mainpage clBBProtocolLib - Basler Binary Protocol Library

This library provides convenience functions to access Basler Camera Link cameras using the clAllSerial/clSerial API.

To use the functions in this library, you should be familiar with the clAllSerial/clSerial API
defined in Appendix&nbsp;B of the Camera Link Standard&nbsp;1.1 or later.

This library contains two sets of functions.
Depending on which of the two cl-APIs (clAllSerial.dll or clSerXXX.dll) you use in your application, you should use \e exactly one set of functions.\n
All functions in this library are prefixed by \c clbbp_ to distinguish them from cl-API functions.

\section clAllSerialFunctions clAllSerial functions
If you are using the \e clAllSerial API functions in your code, you can use the following set of functions:
\li clbbp_AllSerialReadRegister() to read data from a register.
\li clbbp_AllSerialWriteRegister() to write data to a register.
\li clbbp_AllGetErrorText() to get a text representation of a cl return code.

\section clSerialFunctions clSerial functions
If you are using the \e clSerial API functions in your code, you can use the following set of functions:
\li clbbp_SerialReadRegister() to read data from a register.
\li clbbp_SerialWriteRegister() to write data to a register.
\li clbbp_GetErrorText() to get a text representation of a cl return code.

When using a clSerial DLL directly, you must pass the read/write functions of the DLL you are using
to the \c clbbp_ functions. Please refer to the tag_CL_SER_FUNCPOINTERS documentation for more information.

\section Timeouts Timeouts
When calling clbbp_ functions, the timeout parameter specifies the transfer frame timeout in milliseconds.
You do not specify the absolute timeout for the complete operation, but rather the timeout for one transfer frame.\n
A transfer frame has a maximum length of 268 bytes (13 bytes of protocol data + max 255 bytes of payload).
So the timeout you specify should be long enough to transfer 268 bytes at the current baud rate.
For each byte transferred, 10 bits are actually send over the serial line (one start bit + the eight data bits + one stop bit),
so you should multiply the number of bytes by 10 to get the number of bits to be transferred.\n
The formula for the timeout as it depends on your baud rate is:
<tt>(2680&nbsp;/&nbsp;\<baud&nbsp;rate\>)&nbsp;*&nbsp;1000</tt>.\n
For 9600 baud the transfer frame timeout would be about (2680/9600)&nbsp;*&nbsp;1000&nbsp;=&nbsp;280&nbsp;ms.
Depending on your system load, we recommend adding some additional time for safety: 350&nbsp;ms would be a good value
for the 9600 baud case.
When using 115200 baud, the calculated value is 23&nbsp;ms. The suggested timeout with added safety would be 50&nbsp;ms.

\section Using Using clBBProtocolLib in your application
To use clBBProtocolLib in your application during development, you must add <tt>\$(CLBBP_ROOT)/include</tt>
to your include path and <tt>\$(CLBBP_ROOT)/lib</tt> to your library path.
You can do this by setting the <tt>Additional&nbsp;Include&nbsp;Directories</tt>/<tt>Additional&nbsp;Library&nbsp;Directories</tt>
in the properties of your project, where the value of \$(CLBBP_ROOT) points to the installation directory of clBBProtocolLib
(typically <tt>c:\\Program&nbsp;Files\\clBBProtocolLib</tt>). This environment variable is set during the setup of clBBProtocolLib.

To use clBBProtocolLib you just need to include the clBBProtocol.h header in your cpp file.
The header will automatically add the neccessary import-libraries to the linkers list of libraries.

\section Using64 Using clBBProtocolLib in 64-bit applications
clBBProtocolLib supports 64-bit applications. All 64-bit specifc files have the postfix "_w64" in their filename.

If you are using clAllSerial_w64.dll you must make sure you have a 64-bit clSerXXX_w64.dll available in the folder specified by the CLSERIALPATH value in the registry.
The clAllSerial_w64.dll will only use DLLs named clSer???_w64.dll.

\note You cannot use 32-bit clSerXXX.dlls in a 64-bit application.

\section Distributing Distributing clBBProtocol with your application
To distribute clBBProtocolLib with your application, you must ensure that clBBProtocol.dll
can be found by your application. The default LoadLibrary DLL search semantics apply.
One way to ensure this is to place the DLL in the same directory as your application's exe file.
If that is not desired you can also add the directory where the DLL is stored to the PATH variable of the end users system.

\note When compiling for 64-bit you must distribute the 64-bit versions (clBBProtocol_w64.dll, clallserial_w64.dll) instead.

*/

//==============================================================================
//==============================================================================


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CLBBPLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CLBBPLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef CLBBPROTOCOLLIB_H_INCLUDED_
#define CLBBPROTOCOLLIB_H_INCLUDED_

#include <ClAllSerial.h>
#include "CLBBProtocolDefs.h"

// add import library to linkers library list.
// You must specify the correct Additional Library Directories
// in the linkers options of your project for this to work
#if !defined(CLBBP_EXPORTS)
#   if defined(_WIN64)
#       pragma comment(lib, "ClBBProtocolLib_w64.lib")
#   else
#       pragma comment(lib, "ClBBProtocolLib.lib")
#   endif
#endif

#if defined(_WIN64)
#   pragma comment(lib, "clallserial_w64.lib")
#else
#   pragma comment(lib, "clallserial.lib")
#endif

// make everything "C" linkage
#if defined(__cplusplus)
#   define CLBPP_EXTC extern "C"
#else
#   define CLBPP_EXTC
#endif

#define CLBBP_CC __cdecl

// typelib attribute stuff
#if defined(CLBBP_EXPORTS)
#   define CLBBP_API CLBPP_EXTC __declspec(dllexport)
#else
#   define CLBBP_API CLBPP_EXTC __declspec(dllimport)
#endif

// only the newest cl-headers have CLINT64, so we add it here so the current clallserial header be used
#ifndef _CL_INT64_DEFINED_
#   define _CL_INT64_DEFINED_
    typedef __int64 CLINT64;
#endif


/**
\defgroup clSerialFuncs Functions to use with clSerXXX
*/

//////////////////////////////////////////////////////////////////////////////
// function pointers for clSerial Read, Write, Flush & GetErrorText functions

/// Function pointer type for clSerialRead of clSerXXX
typedef CLINT32 (__cdecl *PFN_CLSERIALREAD)( hSerRef serialRef, CLINT8* buffer,
                                             CLUINT32 *bufferSize,
                                             CLUINT32 serialTimeout);
/// Function pointer type for clSerialRead of clSerXXX
typedef CLINT32 (__cdecl *PFN_CLSERIALWRITE)(hSerRef serialRef, CLINT8* buffer,
                                             CLUINT32 *bufferSize,
                                             CLUINT32 serialTimeout);
/// Function pointer type for clSerialRead of clSerXXX
typedef CLINT32 (__cdecl *PFN_CLFLUSHPORT)(hSerRef serialRef);
/// Function pointer type for clSerialRead of clSerXXX
typedef CLINT32 (__cdecl *PFN_CLGETERRORTEXT)(CLINT32 errorCode,
                                              CLINT8* errorText,
                                              CLUINT32* errorTextSize);

//////////////////////////////////////////////////////////////////////////////
// simple container holding function pointer to use when using clSerXXX directly with clbpp_XXX functions.
#pragma pack(push, 4)
/**

\ingroup clSerialFuncs
\brief Simple container to pass clSerial functions to clbbp_ functions.

This structure contains function pointers to clSerXXX functions needed by ClBBProtocol.dll.\n
If you link dynamically to a clSerXXX.dll, you can use GetProcAddress(hDLL, "<cl-function name>") to obtain the value.
In case you use an import lib, you can simply set the value by using the address-of operator on the function.\n
We recommend initializing this structure once at initialization and then passing it to each call to clbpp_ functions.\n
\note You don't need to fill a CL_SER_FUNCT struct when you use clbbp_All-functions.

Use this code to initialize the members when dynamically linking to your clSerXXX.dll:
\code
// dynamically load the dll and use GetProcAddress
HMODULE hDLL = LoadLibrary("ClSerCOM.dll");

CL_SER_FUNCPOINTERS funcs;
funcs.pfnFlushPort =    (PFN_CLFLUSHPORT)   GetProcAddress(hDLL, "clFlushPort");
funcs.pfnSerialRead =   (PFN_CLSERIALREAD)  GetProcAddress(hDLL, "clSerialRead");
funcs.pfnSerialWrite =  (PFN_CLSERIALWRITE) GetProcAddress(hDLL, "clSerialWrite");
funcs.pfnGetErrorText = (PFN_CLGETERRORTEXT)GetProcAddress(hDLL, "clGetErrorText");
\endcode

If you're using an import library (.lib) to access clSerXXX functions, you can use this code to initialize the members:
\code
// use an import library specified in the project settings
CL_SER_FUNCPOINTERS funcs;
funcs.pfnFlushPort =    (PFN_CLFLUSHPORT)   &clFlushPort;
funcs.pfnSerialRead =   (PFN_CLSERIALREAD)  &clSerialRead;
funcs.pfnSerialWrite =  (PFN_CLSERIALWRITE) &clSerialWrite;
funcs.pfnGetErrorText = (PFN_CLGETERRORTEXT)&clGetErrorText;
\endcode
    
*/
typedef struct tag_CL_SER_FUNCPOINTERS
{
    PFN_CLFLUSHPORT     pfnFlushPort;   ///< pointer to clFlushPort. Use GetProcAddress(hDLL, "clFlushPort") to obtain this value.
    PFN_CLSERIALREAD    pfnSerialRead;  ///< pointer to clSerialRead. Use GetProcAddress(hDLL, "clSerialRead") to obtain this value.
    PFN_CLSERIALWRITE   pfnSerialWrite; ///< pointer to clSerialWrite. Use GetProcAddress(hDLL, "clSerialWrite") to obtain this value.
    PFN_CLGETERRORTEXT  pfnGetErrorText;///< pointer to clGetErrorText. Use GetProcAddress(hDLL, "clGetErrorText") to obtain this value.
} CL_SER_FUNCPOINTERS;

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////////
// clSerXXX functions

//////////////////////////////////////////////////////////////////////////////
/// Writes BufferSize amount of bytes from pBuffer to the camera at address Address using clSerXXX functions passed in pFuncs.
CLBBP_API CLINT32 CLBBP_CC clbbp_SerialReadRegister(hSerRef serialRefPtr, const CLINT64 Address, CLINT8* pBuffer, const CLINT64 BufferSize, const CLUINT32 FrameTimeOut, const CL_SER_FUNCPOINTERS* pFuncs);

//////////////////////////////////////////////////////////////////////////////
/// Reads BufferSize amount of bytes from the camera at address Address and stores them in the buffer pBuffer using clSerXXX functions passed in pFuncs.
CLBBP_API CLINT32 CLBBP_CC clbbp_SerialWriteRegister(hSerRef serialRefPtr, const CLINT64 Address, const CLINT8* pBuffer, const CLINT64 BufferSize, const CLUINT32 FrameTimeOut, const CL_SER_FUNCPOINTERS* pFuncs);

//////////////////////////////////////////////////////////////////////////////
/// Translates the errorCode into a descriptive message using clSerXXX functions passed in pFuncs.
CLBBP_API CLINT32 CLBBP_CC clbbp_GetErrorText(CLINT32 errorCode, char* errorText, CLUINT32* errorTextSize, const CL_SER_FUNCPOINTERS* pFuncs);


//////////////////////////////////////////////////////////////////////////////
// clAllSerial Functions

/**
\defgroup clAllSerialFuncs Functions to use with clAllSerial
*/

//////////////////////////////////////////////////////////////////////////////
/// Reads BufferSize amount of bytes from the camera at address Address and stores them in the buffer pBuffer using clAllSerial.
CLBBP_API CLINT32 CLBBP_CC clbbp_AllSerialReadRegister(hSerRef serialRefPtr, const CLINT64 Address, CLINT8* pBuffer, const CLINT64 BufferSize, const CLUINT32 FrameTimeOut);

//////////////////////////////////////////////////////////////////////////////
/// Writes BufferSize amount of bytes from pBuffer to the camera at address Address using clAllSerial.
CLBBP_API CLINT32 CLBBP_CC clbbp_AllSerialWriteRegister(hSerRef serialRefPtr, const CLINT64 Address, const CLINT8* pBuffer, const CLINT64 BufferSize, const CLUINT32 FrameTimeOut);

//////////////////////////////////////////////////////////////////////////////
/// Translates the errorCode into a descriptive message using clAllSerial
CLBBP_API CLINT32 CLBBP_CC clbbp_AllGetErrorText(const char* manuName, CLINT32 errorCode, char* errorText, CLUINT32* errorTextSize);


#endif // #ifndef CLBBPROTOCOLLIB_H_INCLUDED_
