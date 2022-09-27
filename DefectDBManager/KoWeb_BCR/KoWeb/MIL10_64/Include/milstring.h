/***************************************************************************/
/*

    Filename:  MILSTRING.H
    Owner   :  Matrox Imaging
    Revision:  10.30.0595
    Content :  This file contains the MIL_STRING overload for the Matrox
    Imaging Library (MIL)

    Copyright © Matrox Electronic Systems Ltd., 1992-2018.
    All Rights Reserved
    */
/***************************************************************************/
#ifndef __MILPROTO_H
#error milproto.h required
#else

#ifndef __MILSTRING_H__
#define __MILSTRING_H__

#ifndef __MIL_AVX_H__

#if (!defined(M_LINUX_KERNEL) || !M_LINUX_KERNEL) && (!defined(M_WINDOWS_NT_KERNEL_MODE) || !M_WINDOWS_NT_KERNEL_MODE)  &&  defined(__cplusplus)

#define M_MIL_USE_STRING = (!defined(M_LINUX_KERNEL) || !M_LINUX_KERNEL) && (!defined(M_WINDOWS_NT_KERNEL_MODE) || !M_WINDOWS_NT_KERNEL_MODE)  &&  defined(__cplusplus)

#if M_MIL_USE_LINUX
#include <string.h>
#endif
#include <string>
#include <sstream>

#define M_STATIC_STRING_SIZE 64

typedef std::basic_string<MIL_TEXT_CHAR, std::char_traits<MIL_TEXT_CHAR>, std::allocator<MIL_TEXT_CHAR> >
MIL_STRING;

typedef std::basic_stringstream<MIL_TEXT_CHAR, std::char_traits<MIL_TEXT_CHAR>, std::allocator<MIL_TEXT_CHAR> >
MIL_STRING_STREAM;

#if (MIL_COMPILE_VERSION < 1060)
#define milstring       MIL_STRING
#define milstringstream MIL_STRING_STREAM
#endif

#if (M_MIL_USE_UNICODE && M_MIL_UNICODE_API)
#if (!defined(M_WINDOWS_NT_KERNEL_MODE) || !M_WINDOWS_NT_KERNEL_MODE)
#define     MOs_UseDeprecatedStrLength   wcslen
#endif
#elif (M_MIL_USE_WINDOWS)
#if !(defined(M_WINDOWS_NT_KERNEL_MODE) && M_WINDOWS_NT_KERNEL_MODE) 
#define     MOs_UseDeprecatedStrLength   strlen
#endif // !M_WINDOWS_NT_KERNEL_MODE 
#elif M_MIL_USE_LINUX
#define     MOs_UseDeprecatedStrLength   strlen
#endif

/***************************************************************************/
/* FUNC MODULE                                                             */
/***************************************************************************/
inline void MFTYPE MfuncParamValue(MIL_ID ContextFuncId, MIL_INT ParamIndex, MIL_STRING &ParamValuePtr)
   {
   //No need to inquire text since MfuncParamMilText will return the address of the string content
   MfuncParamValue(ContextFuncId, ParamIndex, const_cast<MIL_TEXT_PTR>(ParamValuePtr.c_str()));
   }

#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline void MFTYPE MfuncParamMilTextW(MIL_ID ContextFuncId, MIL_INT ParamIndex, MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   //No need to inquire text since MfuncParamMilText will return the address of the string content
   MfuncParamMilTextW(ContextFuncId, ParamIndex, const_cast<MIL_TEXT_PTR>(ParamValuePtr.c_str()), NumOfItems, Attribute);
   }

inline void MFTYPE MfuncParamConstMilTextW(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamConstMilTextW(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }


inline void MFTYPE MfuncParamFilenameW(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING& ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamFilenameW(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }
inline MIL_ID MFTYPE MfuncAllocW(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr, const MIL_STRING &SlaveFunctionDLLName,
                                 const MIL_STRING &SlaveFunctionName, MIL_INT SlaveFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *CBasedContextFuncIdPtr)
   {
   return MfuncAllocW(FunctionName.c_str(), ParameterNum, SlaveFunctionPtr, SlaveFunctionDLLName.c_str(), SlaveFunctionName.c_str(), SlaveFunctionOpcode, InitFlag, CBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScriptW(const MIL_STRING &FunctionName, MIL_INT ParameterNum, const MIL_STRING &InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                       const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScriptW(FunctionName.c_str(), ParameterNum, InterpreterLanguage.c_str(), ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                            ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScriptW(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_CONST_TEXT_PTR InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                       const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScriptW(FunctionName.c_str(), ParameterNum, InterpreterLanguage, ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                            ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_INT MFTYPE MfuncErrorReportW(MIL_ID ContextFuncId, MIL_INT ErrorCode, const MIL_STRING &ErrorMessage, const MIL_STRING &ErrorSubMessage1,
                                        const MIL_STRING &ErrorSubMessage2, const MIL_STRING &ErrorSubMessage3)
   {
   return MfuncErrorReportW(ContextFuncId, ErrorCode, ErrorMessage.c_str(), ErrorSubMessage1.c_str(), ErrorSubMessage2.c_str(), ErrorSubMessage3.c_str());
   }

inline MIL_INT MFTYPE MfuncPrintMessageW(MIL_INT Mode, const MIL_STRING &Str1, const MIL_STRING &Str2, const MIL_STRING &Str3, const MIL_STRING &Str4, const MIL_STRING &Str5)
   {
   return MfuncPrintMessageW(Mode, Str1.c_str(), Str2.c_str(), Str3.c_str(), Str4.c_str(), Str5.c_str());
   }
#else
inline void MFTYPE MfuncParamMilTextA(MIL_ID ContextFuncId, MIL_INT ParamIndex, MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   //No need to inquire text since MfuncParamMilText will return the address of the string content
   MfuncParamMilTextA(ContextFuncId, ParamIndex, (MIL_TEXT_PTR)ParamValuePtr.c_str(), NumOfItems, Attribute);
   }

inline void MFTYPE MfuncParamConstMilTextA(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamConstMilTextA(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }

inline void MFTYPE MfuncParamFilenameA(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING& ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamFilenameA(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }

inline MIL_ID MFTYPE MfuncAllocA(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr, const MIL_STRING &SlaveFunctionDLLName,
                                 const MIL_STRING &SlaveFunctionName, MIL_INT SlaveFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *CBasedContextFuncIdPtr)
   {
   return MfuncAllocA(FunctionName.c_str(), ParameterNum, SlaveFunctionPtr, SlaveFunctionDLLName.c_str(), SlaveFunctionName.c_str(), SlaveFunctionOpcode, InitFlag, CBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScriptA(const MIL_STRING &FunctionName, MIL_INT ParameterNum, const MIL_STRING &InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                       const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScriptA(FunctionName.c_str(), ParameterNum, InterpreterLanguage.c_str(), ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                            ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScriptA(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_CONST_TEXT_PTR InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                       const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScriptA(FunctionName.c_str(), ParameterNum, InterpreterLanguage, ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                            ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_INT MFTYPE MfuncErrorReportA(MIL_ID ContextFuncId, MIL_INT ErrorCode, const MIL_STRING &ErrorMessage, const MIL_STRING &ErrorSubMessage1,
                                        const MIL_STRING &ErrorSubMessage2, const MIL_STRING &ErrorSubMessage3)
   {
   return MfuncErrorReportA(ContextFuncId, ErrorCode, ErrorMessage.c_str(), ErrorSubMessage1.c_str(), ErrorSubMessage2.c_str(), ErrorSubMessage3.c_str());
   }

inline MIL_INT MFTYPE MfuncPrintMessageA(MIL_INT Mode, const MIL_STRING &Str1, const MIL_STRING &Str2, const MIL_STRING &Str3, const MIL_STRING &Str4, const MIL_STRING &Str5)
   {
   return MfuncPrintMessageA(Mode, Str1.c_str(), Str2.c_str(), Str3.c_str(), Str4.c_str(), Str5.c_str());
   }
#endif /* M_MIL_UNICODE_API */
#else 

inline void MFTYPE MfuncParamMilText(MIL_ID ContextFuncId, MIL_INT ParamIndex, MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   //No need to inquire text since MfuncParamMilText will return the address of the string content
   MfuncParamMilText(ContextFuncId, ParamIndex, const_cast<MIL_TEXT_PTR>(ParamValuePtr.c_str()), NumOfItems, Attribute);
   }

inline void MFTYPE MfuncParamConstMilText(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING &ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamConstMilText(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }

inline void MFTYPE MfuncParamFilename(MIL_ID ContextFuncId, MIL_INT ParamIndex, const MIL_STRING& ParamValuePtr, MIL_INT NumOfItems, MIL_INT Attribute)
   {
   MfuncParamFilename(ContextFuncId, ParamIndex, ParamValuePtr.c_str(), NumOfItems, Attribute);
   }

inline MIL_ID MFTYPE MfuncAlloc(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr, const MIL_STRING &SlaveFunctionDLLName,
                                const MIL_STRING &SlaveFunctionName, MIL_INT SlaveFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *CBasedContextFuncIdPtr)
   {
   return MfuncAlloc(FunctionName.c_str(), ParameterNum, SlaveFunctionPtr, SlaveFunctionDLLName.c_str(), SlaveFunctionName.c_str(), SlaveFunctionOpcode, InitFlag, CBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScript(const MIL_STRING &FunctionName, MIL_INT ParameterNum, const MIL_STRING &InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                      const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScript(FunctionName.c_str(), ParameterNum, InterpreterLanguage.c_str(), ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                           ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_ID MFTYPE MfuncAllocScript(const MIL_STRING &FunctionName, MIL_INT ParameterNum, MIL_CONST_TEXT_PTR InterpreterLanguage, const MIL_STRING &ScriptFileName,
                                      const MIL_STRING &ScriptFunctionName, MIL_INT ScriptFunctionOpcode, MIL_INT64 InitFlag, MIL_ID *ScriptBasedContextFuncIdPtr)
   {
   return MfuncAllocScript(FunctionName.c_str(), ParameterNum, InterpreterLanguage, ScriptFileName.c_str(), ScriptFunctionName.c_str(),
                           ScriptFunctionOpcode, InitFlag, ScriptBasedContextFuncIdPtr);
   }

inline MIL_INT MFTYPE MfuncErrorReport(MIL_ID ContextFuncId, MIL_INT ErrorCode, const MIL_STRING &ErrorMessage, const MIL_STRING &ErrorSubMessage1,
                                       const MIL_STRING &ErrorSubMessage2, const MIL_STRING &ErrorSubMessage3)
   {
   return MfuncErrorReport(ContextFuncId, ErrorCode, ErrorMessage.c_str(), ErrorSubMessage1.c_str(), ErrorSubMessage2.c_str(), ErrorSubMessage3.c_str());
   }

inline MIL_INT MFTYPE MfuncPrintMessage(MIL_INT Mode, const MIL_STRING &Str1, const MIL_STRING &Str2, const MIL_STRING &Str3, const MIL_STRING &Str4, const MIL_STRING &Str5)
   {
   return MfuncPrintMessage(Mode, Str1.c_str(), Str2.c_str(), Str3.c_str(), Str4.c_str(), Str5.c_str());
   }
#endif /* M_MIL_USE_UNICODE */
//// MfuncInquireSafeType is not activated because too many undocumented InquireTypes with various pointer types.
//#if M_MIL_USE_SAFE_TYPE
//inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_STRING    &UserVarPtr)
//   {
//   MIL_INT RetValue = 0;
//
//   //Test if InquireType is in string range
//   if(M_IN_FUNC_INQUIRE_STRING_RANGE(InquireType))
//      {
//      MIL_INT InternalStringSize = 0;
//      MfuncInquireSafeType(ObjectId, InquireType + M_STRING_SIZE, &InternalStringSize);
//
//      if(InternalStringSize > 0)
//         {
//         UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
//         RetValue = MfuncInquireSafeTypeExecute(ObjectId, InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
//         UserVarPtr.resize(InternalStringSize - 1);
//         }
//      }
//   else
//      {
//      SafeTypeError(MIL_TEXT("MfuncInquire: Inquire type not in string range"));
//      }
//   return RetValue;
//   }
//
//#else
inline MIL_INT MFTYPE MfuncInquire(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_STRING    &UserVarPtr)
   {
   MIL_INT RetValue = 0;
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MfuncInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif

   MIL_INT InternalStringSize = 0;
   MfuncInquire(ObjectId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MfuncInquire(ObjectId, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
//#endif

/***************************************************************************/
/* GRAPHIC MODULE                                                          */
/***************************************************************************/
#if M_MIL_USE_64BIT
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline void MFTYPE MgraTextW(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, const MCppParameter& XStart, const MCppParameter& YStart, MIL_STRING StringPtr)
   {
   MgraTextW(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#else
inline void MFTYPE MgraTextA(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, const MCppParameter& XStart, const MCppParameter& YStart, MIL_STRING StringPtr)
   {
   MgraTextA(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#endif /* M_MIL_USE_UNICODE_API */
#else
inline void MFTYPE MgraText(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, const MCppParameter& XStart, const MCppParameter& YStart, MIL_STRING StringPtr)
   {
   MgraText(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#endif /* M_MIL_USE_UNICODE */
#else //different prototype are used for 32-bit
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline void MFTYPE MgraTextW(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, MIL_DOUBLE XStart, MIL_DOUBLE YStart, MIL_STRING StringPtr)
   {
   MgraTextW(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#else
inline void MFTYPE MgraTextA(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, MIL_DOUBLE XStart, MIL_DOUBLE YStart, MIL_STRING StringPtr)
   {
   MgraTextA(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#endif /* M_MIL_USE_UNICODE_API */
#else
inline void MFTYPE MgraText(MIL_ID ContextGraId, MIL_ID DstImageBufOrListGraId, MIL_DOUBLE XStart, MIL_DOUBLE YStart, MIL_STRING StringPtr)
   {
   MgraText(ContextGraId, DstImageBufOrListGraId, XStart, YStart, (MIL_CONST_TEXT_PTR)StringPtr.c_str());
   }
#endif /* M_MIL_USE_UNICODE */

#endif //M_MIL_USE_64BIT

/***************************************************************************/
/* BUFFER MODULE                                                          */
/***************************************************************************/
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API

inline void MFTYPE MbufExportSequenceW(const MIL_STRING& FileName, MIL_INT64 FileFormat, const MIL_ID * BufArrayPtrOrSystemIdPtr, MIL_INT NumOfIds, MIL_DOUBLE FrameRate, MIL_INT64 ControlFlag)
   {
   return MbufExportSequenceW(FileName.c_str(), FileFormat, BufArrayPtrOrSystemIdPtr, NumOfIds, FrameRate, ControlFlag);
   }

inline void MFTYPE MbufExportW(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_ID SrcBufId)
   {
   return MbufExportW(FileName.c_str(), FileFormat, SrcBufId);
   }

inline void MFTYPE MbufImportSequenceW(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID *BufArrayPtr, MIL_INT StartImage, MIL_INT NumberOfImages, MIL_INT64 ControlFlag)
   {
   return MbufImportSequenceW(FileName.c_str(), FileFormat, Operation, SystemId, BufArrayPtr, StartImage, NumberOfImages, ControlFlag);
   }

inline MIL_ID MFTYPE MbufImportW(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufImportW(FileName.c_str(), FileFormat, Operation, SystemId, BufIdPtr);
   }

inline void MFTYPE MbufLoadW(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufLoadW(FileName.c_str(), BufId);
   }

inline MIL_ID MFTYPE MbufRestoreW(const MIL_STRING& FileName, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufRestoreW(FileName.c_str(), SystemId, BufIdPtr);
   }

inline void MFTYPE MbufSaveW(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufSaveW(FileName.c_str(), BufId);
   }
#else

inline void MFTYPE MbufExportSequenceA(const MIL_STRING& FileName, MIL_INT64 FileFormat, const MIL_ID * BufArrayPtrOrSystemIdPtr, MIL_INT NumOfIds, MIL_DOUBLE FrameRate, MIL_INT64 ControlFlag)
   {
   return MbufExportSequenceA(FileName.c_str(), FileFormat, BufArrayPtrOrSystemIdPtr, NumOfIds, FrameRate, ControlFlag);
   }

inline void MFTYPE MbufExportA(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_ID SrcBufId)
   {
   return MbufExportA(FileName.c_str(), FileFormat, SrcBufId);
   }

inline void MFTYPE MbufImportSequenceA(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID *BufArrayPtr, MIL_INT StartImage, MIL_INT NumberOfImages, MIL_INT64 ControlFlag)
   {
   return MbufImportSequenceA(FileName.c_str(), FileFormat, Operation, SystemId, BufArrayPtr, StartImage, NumberOfImages, ControlFlag);
   }

inline MIL_ID MFTYPE MbufImportA(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufImportA(FileName.c_str(), FileFormat, Operation, SystemId, BufIdPtr);
   }

inline void MFTYPE MbufLoadA(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufLoadA(FileName.c_str(), BufId);
   }

inline MIL_ID MFTYPE MbufRestoreA(const MIL_STRING& FileName, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufRestoreA(FileName.c_str(), SystemId, BufIdPtr);
   }

inline void MFTYPE MbufSaveA(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufSaveA(FileName.c_str(), BufId);
   }
#endif /* M_MIL_UNICODE_API*/
#else

inline void MFTYPE MbufExportSequence(const MIL_STRING& FileName, MIL_INT64 FileFormat, const MIL_ID * BufArrayPtrOrSystemIdPtr, MIL_INT NumOfIds, MIL_DOUBLE FrameRate, MIL_INT64 ControlFlag)
   {
   return MbufExportSequence(FileName.c_str(), FileFormat, BufArrayPtrOrSystemIdPtr, NumOfIds, FrameRate, ControlFlag);
   }

inline void MFTYPE MbufExport(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_ID SrcBufId)
   {
   return MbufExport(FileName.c_str(), FileFormat, SrcBufId);
   }

inline void MFTYPE MbufImportSequence(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID *BufArrayPtr, MIL_INT StartImage, MIL_INT NumberOfImages, MIL_INT64 ControlFlag)
   {
   return MbufImportSequence(FileName.c_str(), FileFormat, Operation, SystemId, BufArrayPtr, StartImage, NumberOfImages, ControlFlag);
   }

inline MIL_ID MFTYPE MbufImport(const MIL_STRING& FileName, MIL_INT64 FileFormat, MIL_INT64 Operation, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufImport(FileName.c_str(), FileFormat, Operation, SystemId, BufIdPtr);
   }

inline void MFTYPE MbufLoad(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufLoad(FileName.c_str(), BufId);
   }

inline MIL_ID MFTYPE MbufRestore(const MIL_STRING& FileName, MIL_ID SystemId, MIL_ID* BufIdPtr)
   {
   return MbufRestore(FileName.c_str(), SystemId, BufIdPtr);
   }

inline void MFTYPE MbufSave(const MIL_STRING& FileName, MIL_ID BufId)
   {
   return MbufSave(FileName.c_str(), BufId);
   }
#endif /* M_MIL_USE_UNICODE */

#if M_MIL_USE_SAFE_TYPE
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) { return MbufDiskInquireSafeTypeW(FileName.c_str(), InquireType, UserVarPtr); }
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) { return MbufDiskInquireSafeTypeW(FileName.c_str(), InquireType, UserVarPtr); }

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) { return MbufDiskInquireSafeTypeW(FileName.c_str(), InquireType, UserVarPtr); }
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) { return MbufDiskInquireSafeTypeA(FileName.c_str(), InquireType, UserVarPtr); }
#endif
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(const MIL_STRING& FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   return MbufDiskInquireSafeTypeW(FileName.c_str(), InquireType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(const MIL_STRING& FileName, MIL_INT64 InquireType, std::nullptr_t)
   {
   return MbufDiskInquireSafeTypeW(FileName.c_str(), InquireType, NULL);
   }
#endif
#else
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) { return MbufDiskInquireSafeTypeA(FileName.c_str(), InquireType, UserVarPtr); }
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const MIL_STRING   &FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) { return MbufDiskInquireSafeTypeA(FileName.c_str(), InquireType, UserVarPtr); }

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const MIL_STRING&        FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   return MbufDiskInquireSafeTypeA(FileName.c_str(), InquireType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const MIL_STRING&        FileName, MIL_INT64 InquireType, std::nullptr_t)
   {
   return MbufDiskInquireSafeTypeA(FileName.c_str(), InquireType, NULL);
   }
#endif
#endif /* M_MIL_UNICODE_API */
#else

inline MIL_INT MFTYPE MbufDiskInquireSafeType(const MIL_STRING& FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   if(UserVarPtr != M_NULL)
      SafeTypeError(MIL_TEXT("MbufDiskInquire"));

   return MbufDiskInquire(FileName.c_str(), InquireType, M_NULL);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline MIL_INT MFTYPE MbufDiskInquireSafeType(const MIL_STRING& FileName, MIL_INT64 InquireType, std::nullptr_t)
   {
   return MbufDiskInquire(FileName.c_str(), InquireType, M_NULL);
   }
#endif
inline MIL_INT MFTYPE MbufDiskInquireUnsafe(const MIL_STRING& FileName, MIL_INT64 InquireType, void        *UserVarPtr) { return MbufDiskInquireUnsafe(FileName.c_str(), InquireType, UserVarPtr); }
inline MIL_INT MFTYPE MbufDiskInquireSafeType(const MIL_STRING& FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) { return MbufDiskInquireSafeTypeExecute(FileName.c_str(), InquireType, UserVarPtr, M_TYPE_MIL_INT); }
inline MIL_INT MFTYPE MbufDiskInquireSafeType(const MIL_STRING& FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) { return MbufDiskInquireSafeTypeExecute(FileName.c_str(), InquireType, UserVarPtr, M_TYPE_DOUBLE); }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeType(const MIL_STRING& FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) { return MbufDiskInquireSafeTypeExecute(FileName.c_str(), InquireType, UserVarPtr, M_TYPE_MIL_INT); }
#endif

#endif /* M_MIL_USE_UNICODE */

#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING     &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);

   MIL_INT InternalStringSize = 0;
   MbufInquireFeatureSafeTypeW(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));

      MbufInquireFeatureExecuteW(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);

      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MbufInquireFeatureExecuteW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MbufInquireFeatureExecuteW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MbufInquireFeatureExecuteW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MbufInquireFeatureExecuteW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MbufInquireFeature"));

   MbufInquireFeatureW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t            )
   {
   return MbufInquireFeatureSafeTypeW(BufId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING&     UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64       *UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE      *UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32       *UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8       *UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool            *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MbufControlFeature"));

   MbufControlFeatureW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int              UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, UserVarType);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t                    )
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MbufControlFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64      *UserVarPtr)
   {
   MbufControlFeatureExecuteW(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeW(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* MIL_COMPILE_VERSION >= 1020 */
#else
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING&    UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MbufInquireFeatureSafeTypeA(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MbufInquireFeatureExecuteA(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING&             UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str());
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64              *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE             *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32              *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8              *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool                   *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int                     UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t                          )
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, NULL);
   } 
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MbufControlFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64             *UserVarPtr)
   {
   return MbufControlFeatureSafeTypeA(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeTypeA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   return MbufInquireFeatureSafeTypeA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#endif /* M_MIL_UNICODE_API */
#else

inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING    &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MbufInquireFeatureSafeType(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MbufInquireFeature"));

   MbufInquireFeature(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MbufInquireFeature"));

   MbufInquireFeature(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t            )
   {
   MbufInquireFeature(BufId, InquireType, FeatureName.c_str(), UserVarType, M_NULL);
   }
#endif
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING&        UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64        *UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE       *UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32        *UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8        *UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool             *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MbufControlFeature"));

   MbufControlFeature(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int               UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, UserVarType);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t                   )
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

inline void MFTYPE MbufControlFeatureSafeType(MIL_ID BufId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64       *UserVarPtr)
   {
   MbufControlFeatureExecute(BufId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }

inline void MFTYPE MbufInquireFeatureSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MbufInquireFeatureExecute(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
#endif
#endif /* M_MIL_USE_UNICODE*/

#else /* MIL_SAFE_TYPE */
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline MIL_INT MFTYPE MbufDiskInquireW(const MIL_STRING& FileName, MIL_INT64 InquireType, void * UserVarPtr)
   {
   return MbufDiskInquireW(FileName.c_str(), InquireType, UserVarPtr);
   }
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MbufInquireFeatureW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MbufInquireFeatureW(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeatureW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MbufInquireFeatureW(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MbufInquireFeatureW(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MbufControlFeatureW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MbufControlFeatureW(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MbufControlFeatureW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MbufControlFeatureW(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }
#endif /*(MIL_COMPILE_VERSION >= 1020) */
#else
inline MIL_INT MFTYPE MbufDiskInquireA(const MIL_STRING& FileName, MIL_INT64 InquireType, void * UserVarPtr)
   {
   return MbufDiskInquireA(FileName.c_str(), InquireType, UserVarPtr);
   }
#if (MIL_COMPILE_VERSION >= 1020)   
inline void MFTYPE MbufInquireFeatureA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MbufInquireFeatureA(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MbufInquireFeatureA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MbufInquireFeatureA(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MbufInquireFeatureA(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MbufControlFeatureA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MbufControlFeatureA(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MbufControlFeatureA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MbufControlFeatureA(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }
#endif /* (MIL_COMPILE_VERSION >= 1020)*/
#endif /* M_MIL_UNICODE_API */
#else
inline MIL_INT MFTYPE MbufDiskInquire(const MIL_STRING& FileName, MIL_INT64 InquireType, void * UserVarPtr)
   {
   return MbufDiskInquire(FileName.c_str(), InquireType, UserVarPtr);
   }
inline void MFTYPE MbufInquireFeature(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MbufInquireFeature(BufId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MbufInquireFeature(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MbufInquireFeature(BufId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MbufInquireFeature(BufId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MbufControlFeature(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MbufControlFeature(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MbufControlFeature(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MbufControlFeature(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }
#endif /* M_MIL_USE_UNICODE */

#endif /* M_MIL_USE_SAFETYPE */   

/***************************************************************************/
/* APP MODULE                                                              */
/***************************************************************************/
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline void MFTYPE MappOpenConnectionW(const MIL_STRING& ConnectionDescriptor, MIL_INT64 InitFlag, MIL_INT64 ControlFlag, MIL_ID* RemoteContextAppIdPtr)
   {
   return MappOpenConnectionW(ConnectionDescriptor.c_str(), InitFlag, ControlFlag, RemoteContextAppIdPtr);
   }

inline MIL_ID MFTYPE MappAllocW(const MIL_STRING& ServerDescription, MIL_INT64 InitFlag, MIL_ID* ContextAppIdPtr)
   {
   return MappAllocW(ServerDescription.c_str(), InitFlag, ContextAppIdPtr);
   }

inline void MFTYPE MappTraceW(MIL_ID ContextAppId, MIL_INT64 TraceType, MIL_INT64 TraceTag, MIL_INT64 TraceValue, const MIL_STRING &TraceString)
   {
   return MappTraceW(ContextAppId, TraceType, TraceTag, TraceValue, TraceString.c_str());
   }

#else
inline void MFTYPE MappOpenConnectionA(const MIL_STRING& ConnectionDescriptor, MIL_INT64 InitFlag, MIL_INT64 ControlFlag, MIL_ID* RemoteContextAppIdPtr)
   {
   return MappOpenConnectionA(ConnectionDescriptor.c_str(), InitFlag, ControlFlag, RemoteContextAppIdPtr);
   }

inline MIL_ID MFTYPE MappAllocA(const MIL_STRING& ServerDescription, MIL_INT64 InitFlag, MIL_ID* ContextAppIdPtr)
   {
   return MappAllocA(ServerDescription.c_str(), InitFlag, ContextAppIdPtr);
   }

inline void MFTYPE MappTraceA(MIL_ID ContextAppId, MIL_INT64 TraceType, MIL_INT64 TraceTag, MIL_INT64 TraceValue, const MIL_STRING &TraceString)
   {
   return MappTraceA(ContextAppId, TraceType, TraceTag, TraceValue, TraceString.c_str());
   }

#endif /* M_MIL_UNICODE_API */
#else
inline void MFTYPE MappOpenConnection(const MIL_STRING& ConnectionDescriptor, MIL_INT64 InitFlag, MIL_INT64 ControlFlag, MIL_ID* RemoteContextAppIdPtr)
   {
   return MappOpenConnection(ConnectionDescriptor.c_str(), InitFlag, ControlFlag, RemoteContextAppIdPtr);
   }

inline MIL_ID MFTYPE MappAlloc(const MIL_STRING& ServerDescription, MIL_INT64 InitFlag, MIL_ID* ContextAppIdPtr)
   {
   return MappAlloc(ServerDescription.c_str(), InitFlag, ContextAppIdPtr);
   }

inline void MFTYPE MappTrace(MIL_ID ContextAppId, MIL_INT64 TraceType, MIL_INT64 TraceTag, MIL_INT64 TraceValue, const MIL_STRING &TraceString)
   {
   return MappTrace(ContextAppId, TraceType, TraceTag, TraceValue, TraceString.c_str());
   }

#endif /* M_MIL_USE_UNICODE */

#if M_MIL_USE_SAFE_TYPE
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_STRING&      ErrorPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((ErrorType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappGetError"), MIL_TEXT("Error type not supported with MIL_STRING."));
      }
#endif

   MIL_INT InternalStringSize = 0;
   MIL_INT RetValue = 0;
   MappGetErrorSafeType(ErrorType + M_STRING_SIZE, &InternalStringSize);
   if(InternalStringSize > 0)
      {
      ErrorPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetErrorSafeTypeExecute(ErrorType, &ErrorPtr[0], M_TYPE_TEXT_CHAR);
      ErrorPtr.resize(MOs_UseDeprecatedStrLength(ErrorPtr.c_str()));
      }

   return RetValue;
   }

inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_STRING      &ErrorPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((ErrorType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappGetError"), MIL_TEXT("Error type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MappGetErrorSafeType(ContextAppId, ErrorType + M_STRING_SIZE, &InternalStringSize);
   if(InternalStringSize > 0)
      {
      ErrorPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetErrorSafeTypeExecute(ContextAppId, ErrorType, &ErrorPtr[0], M_TYPE_TEXT_CHAR);
      ErrorPtr.resize(MOs_UseDeprecatedStrLength(ErrorPtr.c_str()));
      }

   return RetValue;
   }

inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING       &ErrorPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InfoType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappGetHookInfo"), MIL_TEXT("Info type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;
   //Resize to M_ERROR_MESSAGE_SIZE first
   MappGetHookInfoSafeType(EventId, InfoType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ErrorPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetHookInfoSafeTypeExecute(EventId, InfoType, &ErrorPtr[0], M_TYPE_MIL_TEXT);
      ErrorPtr.resize(MOs_UseDeprecatedStrLength(ErrorPtr.c_str()));
      }

   return RetValue;
   }

inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING       &ResultPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InfoType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappGetHookInfo"), MIL_TEXT("Info type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MappGetHookInfoSafeType(ContextAppId, EventId, InfoType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, &ResultPtr[0], M_TYPE_MIL_TEXT);
      ResultPtr.resize(MOs_UseDeprecatedStrLength(ResultPtr.c_str()));
      }

   return RetValue;
   }

inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_STRING      &UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MappInquireSafeType(InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappInquireSafeTypeExecute(InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(MOs_UseDeprecatedStrLength(UserVarPtr.c_str()));
      }

   return RetValue;
   }

inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_STRING&      UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MappInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MappInquireSafeType(ContextAppId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappInquireSafeTypeExecute(ContextAppId, InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(MOs_UseDeprecatedStrLength(UserVarPtr.c_str()));
      }

   return RetValue;
   }
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline void MFTYPE MappFileOperationSafeTypeW(MIL_ID                  Comp1ContextAppId,
                                              const MIL_STRING&        Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              const MIL_STRING&        Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeW(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeW(MIL_ID                  Comp1ContextAppId,
                                              int                     Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              const MIL_STRING&        Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeW(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeW(MIL_ID                 Comp1ContextAppId,
                                              const MIL_STRING&       Comp1FileName,
                                              MIL_ID                 Comp2ContextAppId,
                                              int                    Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeW(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeW(MIL_ID                     Comp1ContextAppId,
                                              const MIL_STRING&           Comp1FileName,
                                              MIL_ID                     Comp2ContextAppId,
                                              int                        Comp2FileName,
                                              MIL_INT64                  Operation,
                                              MIL_INT64                  OperationFlag,
                                              MIL_INT*                   OperationDataPtr)
   {
   MappFileOperationSafeTypeW(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
#else
inline void MFTYPE MappFileOperationSafeTypeA(MIL_ID                  Comp1ContextAppId,
                                              const MIL_STRING&        Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              const MIL_STRING&        Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeA(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeA(MIL_ID                  Comp1ContextAppId,
                                              int                     Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              const MIL_STRING&        Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeA(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeA(MIL_ID                  Comp1ContextAppId,
                                              const MIL_STRING&        Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              int                     Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeTypeA(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeA(MIL_ID                     Comp1ContextAppId,
                                              const MIL_STRING&           Comp1FileName,
                                              MIL_ID                     Comp2ContextAppId,
                                              int                        Comp2FileName,
                                              MIL_INT64                  Operation,
                                              MIL_INT64                  OperationFlag,
                                              MIL_INT*                   OperationDataPtr)
   {
   MappFileOperationSafeTypeA(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }

#endif /* M_MIL_UNICODE_API */

#else //!M_MIL_USE_UNICODE
inline void MFTYPE MappFileOperationUnsafe(MIL_ID                  Comp1ContextAppId,
                                           const MIL_STRING&        Comp1FileName,
                                           MIL_ID                  Comp2ContextAppId,
                                           const MIL_STRING&        Comp2FileName,
                                           MIL_INT64               Operation,
                                           MIL_INT64               OperationFlag,
                                           void*                   OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }

inline void MFTYPE MappFileOperationSafeType(MIL_ID                  Comp1ContextAppId,
                                             const MIL_STRING&        Comp1FileName,
                                             MIL_ID                  Comp2ContextAppId,
                                             const MIL_STRING&        Comp2FileName,
                                             MIL_INT64               Operation,
                                             MIL_INT64               OperationFlag,
                                             int                     OperationDataPtr)
   {
   MappFileOperationSafeType(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeType(MIL_ID                  Comp1ContextAppId,
                                             int                     Comp1FileName,
                                             MIL_ID                  Comp2ContextAppId,
                                             const MIL_STRING&             Comp2FileName,
                                             MIL_INT64               Operation,
                                             MIL_INT64               OperationFlag,
                                             int                     OperationDataPtr)
   {
   MappFileOperationSafeType(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeA(MIL_ID                  Comp1ContextAppId,
                                              const MIL_STRING&        Comp1FileName,
                                              MIL_ID                  Comp2ContextAppId,
                                              int                     Comp2FileName,
                                              MIL_INT64               Operation,
                                              MIL_INT64               OperationFlag,
                                              int                     OperationDataPtr)
   {
   MappFileOperationSafeType(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }

inline void MFTYPE MappFileOperationSafeType(MIL_ID                     Comp1ContextAppId,
                                             const MIL_STRING&           Comp1FileName,
                                             MIL_ID                     Comp2ContextAppId,
                                             int                        Comp2FileName,
                                             MIL_INT64                  Operation,
                                             MIL_INT64                  OperationFlag,
                                             MIL_INT*                   OperationDataPtr)
   {
   MappFileOperationSafeType(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperation(MIL_ID             Comp1ContextAppId,
                                     const MIL_STRING& Comp1FileName,
                                     MIL_ID            Comp2ContextAppId,
                                     const MIL_STRING& Comp2FileName,
                                     MIL_INT64        Operation,
                                     MIL_INT64        OperationFlag,
                                     void*            OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }

inline void MFTYPE MappFileOperation(MIL_ID             Comp1ContextAppId,
                                     const MIL_STRING& Comp1FileName,
                                     MIL_ID            Comp2ContextAppId,
                                     int Comp2FileName,
                                     MIL_INT64        Operation,
                                     MIL_INT64        OperationFlag,
                                     void*            OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, (MIL_CONST_TEXT_PTR)(MIL_UINT)Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperation(MIL_ID                  Comp1ContextAppId,
                                     MIL_CONST_TEXT_PTR      Comp1FileName,
                                     MIL_ID                  Comp2ContextAppId,
                                     int                     Comp2FileName,
                                     MIL_INT64               Operation,
                                     MIL_INT64               OperationFlag,
                                     void*                   OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId,(MIL_CONST_TEXT_PTR)(MIL_UINT)Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
#endif /* M_MIL_USE_UNICODE */
#else /* !M_MIL_USE_SAFE_TYPE */

inline MIL_INT MFTYPE MappGetHookInfo(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING       &ErrorPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;
   MappGetHookInfo(ContextAppId, EventId, InfoType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ErrorPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetHookInfo(ContextAppId, EventId, InfoType, &ErrorPtr[0]);
      ErrorPtr.resize(MOs_UseDeprecatedStrLength(ErrorPtr.c_str()));
      }


   return RetValue;
   }

inline MIL_INT MFTYPE MappGetHookInfo(MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING       &ErrorPtr)
   {
   return MappGetHookInfo(M_DEFAULT, EventId, InfoType, ErrorPtr);
   }

inline MIL_INT MFTYPE MappInquire(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_STRING&      UserVarPtr)
   {
   MIL_INT RetValue = 0;

   MIL_INT InternalStringSize = 0;
   MappInquire(ContextAppId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappInquire(ContextAppId, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(MOs_UseDeprecatedStrLength(UserVarPtr.c_str()));
      }


   return RetValue;
   }

inline MIL_INT MFTYPE MappInquire(MIL_INT64 InquireType, MIL_STRING      &UserVarPtr)
   {
   return MappInquire(M_DEFAULT, InquireType, UserVarPtr);
   }
inline MIL_INT MFTYPE MappGetError(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_STRING&      ErrorPtr)
   {
   MIL_INT RetValue = 0;


   MIL_INT InternalStringSize = 0;
   MappGetError(ContextAppId, ErrorType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ErrorPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MappGetError(ContextAppId, ErrorType, &ErrorPtr[0]);
      ErrorPtr.resize(MOs_UseDeprecatedStrLength(ErrorPtr.c_str()));
      }


   return RetValue;
   }

inline MIL_INT MFTYPE MappGetError(MIL_INT64 ErrorType, MIL_STRING&      ErrorPtr)
   {
   return MappGetError(M_DEFAULT, ErrorType, ErrorPtr);
   }


inline void MFTYPE MappFileOperation(MIL_ID             Comp1ContextAppId,
                                      const MIL_STRING& Comp1FileName,
                                      MIL_ID            Comp2ContextAppId,
                                      const MIL_STRING& Comp2FileName,
                                      MIL_INT64        Operation,
                                      MIL_INT64        OperationFlag,
                                      void*            OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, Comp2FileName.c_str(), Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperation(MIL_ID             Comp1ContextAppId,
                                      const MIL_STRING& Comp1FileName,
                                      MIL_ID            Comp2ContextAppId,
                                      int Comp2FileName,
                                      MIL_INT64        Operation,
                                      MIL_INT64        OperationFlag,
                                      void*            OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName.c_str(), Comp2ContextAppId, (MIL_CONST_TEXT_PTR)(MIL_UINT)Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperation(MIL_ID                  Comp1ContextAppId,
                                     MIL_CONST_TEXT_PTR      Comp1FileName,
                                     MIL_ID                  Comp2ContextAppId,
                                     int                     Comp2FileName,
                                     MIL_INT64               Operation,
                                     MIL_INT64               OperationFlag,
                                     void*                   OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId,(MIL_CONST_TEXT_PTR)(MIL_UINT)Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }

#endif /* M_MIL_USE_SAFE_TYPE*/

/***************************************************************************/
/* SYS MODULE                                                              */
/***************************************************************************/
inline void MsysControl(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& ControlValue)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((ControlType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MsysControl"), MIL_TEXT("Control type not supported with MIL_STRING."));
      }
#endif

   MsysControl(SysId, ControlType, ControlValue.c_str());
   }
   
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline MIL_ID MFTYPE MsysAllocW(MIL_ID ContextAppId, const MIL_STRING &SystemDescriptor, MIL_INT SystemNum, MIL_INT64 InitFlag, MIL_ID* SysIdPtr)
   {
   return MsysAllocW(ContextAppId, SystemDescriptor.c_str(), SystemNum, InitFlag, SysIdPtr);
   }
#else
inline MIL_ID MFTYPE MsysAllocA(MIL_ID ContextAppId, const MIL_STRING &SystemDescriptor, MIL_INT SystemNum, MIL_INT64 InitFlag, MIL_ID* SysIdPtr)
   {
   return MsysAllocA(ContextAppId, SystemDescriptor.c_str(), SystemNum, InitFlag, SysIdPtr);
   }
#endif
#else
inline MIL_ID MFTYPE MsysAlloc(MIL_ID ContextAppId, const MIL_STRING &SystemDescriptor, MIL_INT SystemNum, MIL_INT64 InitFlag, MIL_ID* SysIdPtr)
   {
   return MsysAlloc(ContextAppId, SystemDescriptor.c_str(), SystemNum, InitFlag, SysIdPtr);
   }
#endif /* M_MIL_USE*/

#if M_MIL_USE_SAFE_TYPE
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING      &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeatureSafeTypeW(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeatureExecuteW(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif

inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING       &UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64       *UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE      *UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32       *UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8       *UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool            *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MsysControlFeature"));

   MsysControlFeatureW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int              UserVarPtr)
   {
   if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MsysControlFeature"));
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

inline void MFTYPE MsysControlFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64      *UserVarPtr)
   {
   MsysControlFeatureExecuteW(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }

inline void MFTYPE MsysInquireFeatureSafeTypeW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#else
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING    &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeatureSafeTypeA(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeatureExecuteA(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);

      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif

inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING              &UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str());
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64              *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE             *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32              *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8              *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool                   *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int                     UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif


#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MsysControlFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64             *UserVarPtr)
   {
   MsysControlFeatureSafeTypeA(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MsysInquireFeatureSafeTypeA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MsysInquireFeatureSafeTypeA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#endif /* M_MIL_UNICODE_API */
#else
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING      &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeatureSafeType(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), UserVarType, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MsysInquireFeature"));

   MsysInquireFeature(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MsysInquireFeature"));

   MsysInquireFeature(SysId, InquireType, FeatureName.c_str(), UserVarType, M_NULL);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysInquireFeature(SysId, InquireType, FeatureName.c_str(), UserVarType, M_NULL);
   }
#endif

inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING       &UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64        *UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE       *UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32        *UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8        *UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool             *UserVarPtr)
   {
   if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MsysControlFeature"));

   MsysControlFeature(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int               UserVarPtr)
   {
   if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MsysControlFeature"));
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

inline void MFTYPE MsysControlFeatureSafeType(MIL_ID SysId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64       *UserVarPtr)
   {
   MsysControlFeatureExecute(SysId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }

inline void MFTYPE MsysInquireFeatureSafeType(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MsysInquireFeatureExecute(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
#endif
#endif /* M_MIL_USE_UNICODE */

inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_STRING&       UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MsysIoInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif

   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MsysIoInquireSafeType(IoObjectId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

#if (MIL_COMPILE_VERSION >= 1020)
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SysId, MIL_ID EventId, MIL_INT64 InquireType, MIL_STRING       &ResultPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MsysGetHookInfo"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif

   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MsysGetHookInfoSafeType(SysId, EventId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysGetHookInfoSafeTypeExecute(SysId, EventId, InquireType, &ResultPtr[0], M_TYPE_STRING_PTR);
      ResultPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
#endif /* (MIL_COMPILE_VERSION >= 1020) */

inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SysId, MIL_INT64 InquireType, MIL_STRING       &ResultPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MsysInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif

   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MsysInquireSafeType(SysId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysInquireSafeTypeExecute(SysId, InquireType, &ResultPtr[0], M_TYPE_TEXT_CHAR);
      ResultPtr.resize(MOs_UseDeprecatedStrLength(ResultPtr.c_str()));
      }

   return RetValue;
   }
#else /* !M_MIL_USE_SAFE_TYPE*/

inline MIL_INT MFTYPE MsysIoInquire(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_STRING        UserVarPtr)
   {
   MIL_INT RetValue = 0;

   MIL_INT InternalStringSize = 0;
   MsysIoInquire(IoObjectId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysIoInquire(IoObjectId, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

#if (MIL_COMPILE_VERSION >= 1020)
inline MIL_INT MFTYPE MsysGetHookInfo(MIL_ID SysId, MIL_ID EventId, MIL_INT64 InquireType, MIL_STRING       &ResultPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MsysGetHookInfo(SysId, EventId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysGetHookInfo(SysId, EventId, InquireType, &ResultPtr[0]);
      ResultPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
#endif /* (MIL_COMPILE_VERSION >= 1020)*/    
inline MIL_INT MFTYPE MsysInquire(MIL_ID SysId, MIL_INT64 InquireType, MIL_STRING       &ResultPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MsysInquire(SysId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MsysInquire(SysId, InquireType, &ResultPtr[0]);
      ResultPtr.resize(MOs_UseDeprecatedStrLength(ResultPtr.c_str()));
      }

   return RetValue;
   }

#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MsysInquireFeatureW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MsysInquireFeatureW(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MsysControlFeatureW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MsysControlFeatureW(SysId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }


inline void MFTYPE MsysControlFeatureW(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MsysControlFeatureW(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }

inline void MFTYPE MsysInquireFeatureW(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeatureW(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeatureW(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#else
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MsysInquireFeatureA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MsysInquireFeatureA(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MsysControlFeatureA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MsysControlFeatureA(SysId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }


inline void MFTYPE MsysControlFeatureA(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MsysControlFeatureA(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }

inline void MFTYPE MsysInquireFeatureA(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeatureA(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeatureA(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#endif /* M_MIL_UNICODE_API */
#else
inline void MFTYPE MsysInquireFeature(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MsysInquireFeature(SysId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MsysControlFeature(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MsysControlFeature(SysId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }


inline void MFTYPE MsysControlFeature(MIL_ID BufId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MsysControlFeature(BufId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }

inline void MFTYPE MsysInquireFeature(MIL_ID SysId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MsysInquireFeature(SysId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MsysInquireFeature(SysId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
#endif /* M_MIL_USE_UNICODE*/

#endif
/***************************************************************************/
/* OBJ MODULE                                                              */
/***************************************************************************/
inline MIL_INT MFTYPE MobjInquire(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_STRING  &UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MobjInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MobjInquire(ObjectId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MobjInquire(ObjectId, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

/***************************************************************************/
/* DIG MODULE                                                              */
/***************************************************************************/
inline void MdigControl(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& ControlValue)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((ControlType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MdigControl"), MIL_TEXT("Control type not supported with MIL_STRING."));
      }
#endif

   MdigControl(DigId, ControlType, M_PTR_TO_DOUBLE(ControlValue.c_str()));
   }

#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
inline MIL_ID MFTYPE MdigAllocW(MIL_ID SystemId, MIL_INT DigNum, const MIL_STRING& DataFormat, MIL_INT64 InitFlag, MIL_ID* DigIdPtr)
   {
   return MdigAllocW(SystemId, DigNum, DataFormat.c_str(), InitFlag, DigIdPtr);
   }

#else
inline MIL_ID MFTYPE MdigAllocA(MIL_ID SystemId, MIL_INT DigNum, const MIL_STRING& DataFormat, MIL_INT64 InitFlag, MIL_ID* DigIdPtr)
   {
   return MdigAllocA(SystemId, DigNum, DataFormat.c_str(), InitFlag, DigIdPtr);
   }
#endif /* M_MIL_UNICODE_API */
#else
inline MIL_ID MFTYPE MdigAlloc(MIL_ID SystemId, MIL_INT DigNum, const MIL_STRING& DataFormat, MIL_INT64 InitFlag, MIL_ID* DigIdPtr)
   {
   return MdigAlloc(SystemId, DigNum, DataFormat.c_str(), InitFlag, DigIdPtr);
   }
#endif /* M_MIL_USE_UNICODE */

#if M_MIL_USE_SAFE_TYPE
#if (MIL_COMPILE_VERSION >= 1020)
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING&     ResultPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InfoType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MdigGetHookInfo"), MIL_TEXT("Info type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MdigGetHookInfoSafeType(EventId, InfoType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MdigGetHookInfoSafeTypeExecute(EventId, InfoType, &ResultPtr[0], M_TYPE_STRING_PTR);
      ResultPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
#endif
#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING&     UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MdigInquireFeatureSafeTypeW(DigId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING      &UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE      *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool            *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int              UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, UserVarType);
   }

#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64      *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }

inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeW(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#else
#if (MIL_COMPILE_VERSION >= 1020)
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING&      UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MdigInquireFeatureSafeTypeA(DigId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);

      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING&               UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str());
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64              *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE             *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32              *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8              *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool                   *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int                     UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, NULL);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64             *UserVarPtr)
   {
   MdigControlFeatureSafeTypeA(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureSafeTypeA(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#endif
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#endif /* M_MIL_UNICODE_API */
#else

inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_STRING&      UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MdigInquireFeatureSafeType(DigId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeature(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureSafeType(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigInquireFeatureSafeType(DigId, InquireType, FeatureName.c_str(), UserVarType, M_NULL);
   }
#endif
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_STRING&       UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr.c_str(), M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT64        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_DOUBLE       *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_INT32        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT8        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_UINT8);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const bool             *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeature(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, int               UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, UserVarType);
   }
#if M_MIL_SAFE_TYPE_M_NULL_PTR_TYPE_EXISTS
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, std::nullptr_t)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, M_NULL, UserVarType);
   }
#endif

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, const MIL_UINT64       *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }

inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING& FeatureName, MIL_INT64 UserVarType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr, M_TYPE_INT64);
   }
#endif
#endif   /* M_MIL_USE_UNICODE */

inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_STRING      &UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MdigInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif

   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MdigInquireSafeType(DigId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MdigInquireExecute(DigId, InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

#else
inline MIL_INT MFTYPE MdigInquire(MIL_ID DigId, MIL_INT64 InquireType, MIL_STRING      &UserVarPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MdigInquire(DigId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MdigInquire(DigId, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

inline void MFTYPE MdigInquireFeature(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, void           *UserVarPtr)
   {
   MdigInquireFeature(DigId, InquireType, FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MdigInquireFeature(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, MIL_STRING &UserVarPtr)
   {
   UNREFERENCED_PARAMETER(UserVarType);
   MIL_INT InternalStringSize = 0;
   MdigInquireFeature(DigId, InquireType + M_STRING_SIZE, FeatureName.c_str(), M_TYPE_MIL_INT, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      MdigInquireFeature(DigId, InquireType, FeatureName.c_str(), M_TYPE_STRING + M_FEATURE_USER_ARRAY_SIZE(InternalStringSize), &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }
   }

inline void MFTYPE MdigControlFeature(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const void *UserVarPtr)
   {
   MdigControlFeature(DigId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, UserVarPtr);
   }

inline void MFTYPE MdigControlFeature(MIL_ID DigId, MIL_INT64 InquireType, const MIL_STRING &FeatureName, MIL_INT64 UserVarType, const MIL_STRING &UserVarPtr)
   {
   MdigControlFeature(DigId, InquireType, (MIL_CONST_TEXT_PTR)FeatureName.c_str(), UserVarType, (MIL_CONST_TEXT_PTR)UserVarPtr.c_str());
   }

#if (MIL_COMPILE_VERSION >= 1020)
inline MIL_INT MFTYPE MdigGetHookInfo(MIL_ID EventId, MIL_INT64 InfoType, MIL_STRING&    ResultPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;
   MdigGetHookInfo(EventId, InfoType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MdigGetHookInfo(EventId, InfoType, &ResultPtr[0]);
      ResultPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
#endif /* (MIL_COMPILE_VERSION >= 1020) */
#endif /* M_MIL_USE_SAFE_TYPE */

/***************************************************************************/
/* COM MODULE                                                              */
/***************************************************************************/
#ifdef  __MILCOM_H__
inline MIL_INT MFTYPE McomInquire(MIL_ID ComId, MIL_INT64  InquireType, MIL_STRING &UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("McomInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   McomInquire(ComId, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      ResultPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = McomInquire(ComId, InquireType, &ResultPtr[0]);
      ResultPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }
#endif

/***************************************************************************/
/* SEQ MODULE                                                              */
/***************************************************************************/

#if !M_MIL_USE_LINUX
inline void MFTYPE MseqDefine(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 SequenceType, const MIL_STRING& Param1Ptr, MIL_DOUBLE Param2)
   {
   MseqDefine(ContextSeqId, SequenceIndex, SequenceType, Param1Ptr.c_str(), Param2);
   }

#if M_MIL_USE_SAFE_TYPE
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_STRING       &UserVarPtr)
   {
#if MIL_COMPILE_VERSION >= 1060 && M_MIL_USE_SAFE_TYPE
   if((InquireType & M_CLIENT_ENCODING) != M_CLIENT_ENCODING)
      {
      SafeTypeError(MIL_TEXT("MseqInquire"), MIL_TEXT("Inquire type not supported with MIL_STRING."));
      }
#endif
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MseqInquireSafeType(ContextSeqId, SequenceIndex, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, &UserVarPtr[0], M_TYPE_TEXT_CHAR);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

#else
inline MIL_INT MFTYPE MseqInquire(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_STRING       &UserVarPtr)
   {
   MIL_INT RetValue = 0;
   MIL_INT InternalStringSize = 0;

   MseqInquire(ContextSeqId, SequenceIndex, InquireType + M_STRING_SIZE, &InternalStringSize);

   if(InternalStringSize > 0)
      {
      UserVarPtr.assign(InternalStringSize, MIL_TEXT('\0'));
      RetValue = MseqInquire(ContextSeqId, SequenceIndex, InquireType, &UserVarPtr[0]);
      UserVarPtr.resize(InternalStringSize - 1);
      }

   return RetValue;
   }

#endif

//Undefined MOs_UseDeprecatedStrLength
#ifdef  MOs_UseDeprecatedStrLength
#undef  MOs_UseDeprecatedStrLength
#endif


#endif //#!M_MIL_USE_LINUX
#endif /*(!defined(M_LINUX_KERNEL) || !M_LINUX_KERNEL) && (!defined(M_WINDOWS_NT_KERNEL_MODE) || !M_WINDOWS_NT_KERNEL_MODE) &&  defined(__cplusplus)*/
#endif /* __MIL_AVX_H__*/
#endif /* __MILSTRING_H__ */	
#endif /* __MILPROTO_H */
