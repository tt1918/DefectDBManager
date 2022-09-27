/***************************************************************************/
/*

    Filename:  MILVECTOR.H
    Owner   :  Matrox Imaging
    Revision:  10.30.0595
    Content :  This file contains the std::vector overload for the Matrox
    Imaging Library (MIL)

    Copyright © Matrox Electronic Systems Ltd., 1992-2018.
    All Rights Reserved
    */
/***************************************************************************/
#ifndef __MILPROTO_H
#error milproto.h required
#else

#ifndef __MILVECTOR_H__
#define __MILVECTOR_H__

/************************************************************************/
/* SUPPORT FOR std::vector                                              */
/************************************************************************/
#ifndef M_MIL_USE_VECTOR
#if (!defined(M_LINUX_KERNEL) || !M_LINUX_KERNEL) && (!defined(M_WINDOWS_NT_KERNEL_MODE) || !M_WINDOWS_NT_KERNEL_MODE) && (!defined(M_WINDOWS_CE_KERNEL_MODE) || !M_WINDOWS_CE_KERNEL_MODE) &&  defined(__cplusplus)
#define M_MIL_USE_VECTOR 1
#else
#define M_MIL_USE_VECTOR 0
#endif
#endif

/************************************************************************/
/* SUPPORT FOR std::vector                                              */
/************************************************************************/
#if M_MIL_USE_VECTOR
#include <vector>

// Struct used for getting the MIL data type flag of a data type.
template <typename UserType>
struct MilTraits;

#define DECLARE_MIL_TRAITS(Type, MilDataTypeFlag, MilBufferTypeFlag)       \
template <>                                                                \
struct MilTraits<Type>                                                     \
   {                                                                       \
   static const MIL_INT64 TypeFlag = MilDataTypeFlag;                      \
   static const MIL_INT64 BufferTypeFlag = MilBufferTypeFlag;              \
   };

#define RUN_BUFINQUIRE_RETURNIFFAIL(MacroBufId, MacroInquireType, MacroUserVarptr)   \
   MbufInquire(MacroBufId, MacroInquireType, MacroUserVarptr);                       \
   if(*MacroUserVarptr == 0) {                                                        \
      return;                                                                        \
      }                                                                              

DECLARE_MIL_TRAITS(MIL_INT8  , M_TYPE_CHAR      , M_SIGNED   + 8 );
DECLARE_MIL_TRAITS(MIL_UINT8 , M_TYPE_MIL_UINT8 , M_UNSIGNED + 8 );
DECLARE_MIL_TRAITS(MIL_UINT16, M_DEFAULT        , M_UNSIGNED + 16);
DECLARE_MIL_TRAITS(MIL_UINT32, M_DEFAULT        , M_UNSIGNED + 32);
DECLARE_MIL_TRAITS(MIL_INT16 , M_TYPE_MIL_INT16 , M_SIGNED   + 16);
DECLARE_MIL_TRAITS(MIL_INT32 , M_TYPE_MIL_INT32 , M_SIGNED   + 32);
DECLARE_MIL_TRAITS(MIL_INT64 , M_TYPE_MIL_INT64 , M_SIGNED   + 64);
DECLARE_MIL_TRAITS(MIL_FLOAT , M_TYPE_MIL_FLOAT , M_FLOAT    + 32);
DECLARE_MIL_TRAITS(MIL_DOUBLE, M_TYPE_MIL_DOUBLE, M_FLOAT    + 64);

// Class used as any array parameter in C++ overloads of any functions that takes std::vector.
class CMilArrayParamOut
   {
   public:
      inline CMilArrayParamOut(int Value): m_pParamVector(M_NULL), m_ParamValue(Value), m_MilDataType(M_NULL) {}
#if M_MIL_USE_NULLPTR
      inline CMilArrayParamOut(std::nullptr_t) : m_pParamVector(M_NULL), m_ParamValue(M_NULL), m_MilDataType(M_NULL) {}
#endif
      inline CMilArrayParamOut(std::vector<MIL_INT8  >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT8  >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_UINT8 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_UINT8 >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_UINT16>& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_UINT16>::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_UINT32>& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_UINT32>::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_INT16 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT16 >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_INT32 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT32 >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_INT64 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT64 >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_FLOAT >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_FLOAT >::TypeFlag) {}
      inline CMilArrayParamOut(std::vector<MIL_DOUBLE>& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_DOUBLE>::TypeFlag) {}

      template <typename UserType>
      inline void Resize(const MIL_INT& Size) { if(m_pParamVector) { (reinterpret_cast<std::vector<UserType>*>(m_pParamVector))->resize(Size); } }

      template <typename UserType>
      inline UserType* GetData() { return m_pParamVector ? &(*reinterpret_cast<std::vector<UserType>*>(m_pParamVector))[0] : M_NULL; }

      inline bool IsValidParam() { return m_pParamVector ? true : m_ParamValue ? false : true; }

      // For debug purpose.
      inline bool IsNullPtr() { return m_pParamVector ? false : true; }

      inline MIL_INT64 GetMilDataType() { return m_MilDataType; }

   private:
      void* m_pParamVector;
      int m_ParamValue;
      MIL_INT64 m_MilDataType;
   };

class CMilArrayParamIn
   {
   public:
      inline CMilArrayParamIn(int Value): m_pParamVector(M_NULL), m_ParamValue(Value), m_MilDataType(M_NULL) {}
#if M_MIL_USE_NULLPTR
      inline CMilArrayParamIn(std::nullptr_t) : m_pParamVector(M_NULL), m_ParamValue(M_NULL), m_MilDataType(M_NULL) {}
#endif
      inline CMilArrayParamIn(const std::vector<MIL_INT8  >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT8  >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_UINT8 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_UINT8 >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_UINT16>& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_UINT16>::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_INT16 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT16 >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_INT32 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT32 >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_INT64 >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_INT64 >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_FLOAT >& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_FLOAT >::TypeFlag) {}
      inline CMilArrayParamIn(const std::vector<MIL_DOUBLE>& Vec) : m_pParamVector(&Vec), m_ParamValue(M_NULL), m_MilDataType(MilTraits<MIL_DOUBLE>::TypeFlag) {}

      template <typename UserType>
      inline MIL_INT Size() { return m_pParamVector ? (MIL_INT)((reinterpret_cast<const std::vector<UserType>*>(m_pParamVector))->size()) : 0; }

      template <typename UserType>
      inline const UserType* GetData() { return m_pParamVector ? &(*reinterpret_cast<const std::vector<UserType>*>(m_pParamVector))[0] : M_NULL; }

      inline bool IsValidParam() { return m_pParamVector ? true : m_ParamValue ? false : true; }

      // For debug purpose.
      inline bool IsNullPtr() { return m_pParamVector ? false : true; }

      inline MIL_INT64 GetMilDataType() { return m_MilDataType; }

      template <typename UserType>
      inline bool IsEmpty() { return m_pParamVector ? (reinterpret_cast<const std::vector<UserType>*>(m_pParamVector))->empty() : false; }

   private:
      const void* m_pParamVector;
      int m_ParamValue;
      MIL_INT64 m_MilDataType;
   };

#define M_INQUIRE_SIZE_BYTE      2L // Used in the MxxxStreamForStdVector function defined in milproto.h

/***************************************************************************/
/* MxxxStream function PROTOTYPE                                           */
/***************************************************************************/
typedef void(MFTYPE* M_XXX_STREAM_FUNCTION)(MilStreamParam MemPtrOrFileName,
                                            MIL_ID         SysId,
                                            MIL_INT64      Operation,
                                            MIL_INT64      StreamType,
                                            MIL_DOUBLE     Version,
                                            MIL_INT64      ControlFlag,
                                            MIL_ID*        MxxxIdPtr,
                                            MIL_INT*       SizeByteVarPtr);
// ----------------------------------------------------------
// Overloads for std::vector in MxxxStream.
inline void MFTYPE MxxxStreamForStdVector(std::vector<MIL_UINT8> &MemPtrOrFileName,
                                          MIL_ID                 SysId,
                                          MIL_INT64              Operation,
                                          MIL_INT64              StreamType,
                                          MIL_DOUBLE             Version,
                                          MIL_INT64              ControlFlag,
                                          MIL_ID*                MxxxIdPtr,
                                          MIL_INT*               SizeByteVarPtr,
                                          M_XXX_STREAM_FUNCTION  MxxxStream)
   {
#if M_MIL_USE_SAFE_TYPE
   if(StreamType != M_MEMORY)
      {
      SafeTypeError(MIL_TEXT("MxxxStream"), MIL_TEXT("StreamType parameter must be set to M_MEMORY with the std::vector overloads."));
      }
#endif

   switch(Operation)
      {
      case M_LOAD:
      case M_RESTORE:
      {
#if M_MIL_USE_SAFE_TYPE
      if(MemPtrOrFileName.empty())
         {
         SafeTypeError(MIL_TEXT("MxxxStream"), MIL_TEXT("The Mem vector cannot be empty with M_LOAD or M_RESTORE operation."));
         }
#endif
      (*MxxxStream)(&MemPtrOrFileName[0],
                    SysId,
                    Operation,
                    StreamType,
                    Version,
                    ControlFlag,
                    MxxxIdPtr,
                    SizeByteVarPtr);
      }
      break;
      case M_SAVE:
      {
      MIL_INT NumberOfBytes = -1;
      (*MxxxStream)(M_NULL, M_NULL, M_INQUIRE_SIZE_BYTE, StreamType, Version, ControlFlag, MxxxIdPtr, &NumberOfBytes);

      if(NumberOfBytes > 0)
         {
         MemPtrOrFileName.resize(NumberOfBytes);

         (*MxxxStream)(&MemPtrOrFileName[0],
                       SysId,
                       Operation,
                       StreamType,
                       Version,
                       ControlFlag,
                       MxxxIdPtr,
                       SizeByteVarPtr);
         }
      }
      break;
      default:
#if M_MIL_USE_SAFE_TYPE
      {
      SafeTypeError(MIL_TEXT("MxxxStream"), MIL_TEXT("Invalid Operation with the std::vector overloads."));
      }
#endif
      break;
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MgraCopy.
inline void MFTYPE MgraCopy(MIL_ID                     SrcListGraId,
                            MIL_ID                     DstListGraId,
                            MIL_INT64                  Operation,
                            MIL_INT                    InsertLocation,
                            MIL_INT                    NumGraphics,
                            CMilArrayParamIn           SrcIndexOrLabelArrayPtr,
                            CMilArrayParamOut          DstLabelArrayPtr,
                            MIL_INT64                  ControlFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(NumGraphics <= 0)
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("NumGraphics parameter must be greater than zero."));
      }
   if(!DstLabelArrayPtr.IsValidParam() || !SrcIndexOrLabelArrayPtr.IsValidParam())
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("SrcIndexOrLabelArrayPtr or DstLabelArrayPtr cannot be an non zero integer."));
      }
   if(SrcIndexOrLabelArrayPtr.IsEmpty<MIL_INT>())
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("The input vector cannot be empty."));
      }
   if(!SrcIndexOrLabelArrayPtr.IsNullPtr() && SrcIndexOrLabelArrayPtr.GetMilDataType() != MilTraits<MIL_INT>::TypeFlag)
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("The data type of SrcIndexOrLabelArrayPtr must be MIL_INT."));
      }
   if(!DstLabelArrayPtr.IsNullPtr() && DstLabelArrayPtr.GetMilDataType() != MilTraits<MIL_INT>::TypeFlag)
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("The data type of DstLabelArrayPtr must be MIL_INT."));
      }
   if(!SrcIndexOrLabelArrayPtr.IsNullPtr() && NumGraphics > (MIL_INT)SrcIndexOrLabelArrayPtr.Size<MIL_INT>() && NumGraphics != M_DEFAULT && NumGraphics != M_ALL)
      {
      SafeTypeError(MIL_TEXT("MgraCopy"), MIL_TEXT("NumGraphics parameter must be smaller or equal (M_DEFAULT) to the size of input vector."));
      }
#endif

   if(!SrcIndexOrLabelArrayPtr.IsNullPtr() && NumGraphics != M_ALL && (NumGraphics == M_DEFAULT || NumGraphics > (MIL_INT)SrcIndexOrLabelArrayPtr.Size<MIL_INT>()))
      {
      NumGraphics = (MIL_INT)SrcIndexOrLabelArrayPtr.Size<MIL_INT>();
      }

   if(NumGraphics == M_ALL)
      {
      MIL_INT InternalNumGraphicsForStdVector = 0;

      MgraInquireList(SrcListGraId, M_LIST, M_DEFAULT, M_NUMBER_OF_GRAPHICS + M_TYPE_MIL_INT, &InternalNumGraphicsForStdVector);

      DstLabelArrayPtr.Resize<MIL_INT>(InternalNumGraphicsForStdVector);

      if(InternalNumGraphicsForStdVector <= 0)
         {
         return;
         }
      }
   else
      {
      DstLabelArrayPtr.Resize<MIL_INT>(NumGraphics);
      }

   MgraCopy(SrcListGraId,
            DstListGraId,
            Operation,
            InsertLocation,
            NumGraphics,
            SrcIndexOrLabelArrayPtr.GetData<MIL_INT>(),
            DstLabelArrayPtr.GetData<MIL_INT>(),
            ControlFlag);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MgraDots.
template <typename UserType>
inline void MgraDots(MIL_ID                      ContextGraId,
                     MIL_ID                      DstImageBufOrListGraId,
                     MIL_INT                     NumberOfDots,
                     const std::vector<UserType> &PosXArrayPtr,
                     const std::vector<UserType> &PosYArrayPtr,
                     MIL_INT64                   ControlFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(NumberOfDots <= 0)
      {
      SafeTypeError(MIL_TEXT("MgraDots"), MIL_TEXT("NumberOfDots parameter must be greater than zero."));
      }
   if(PosXArrayPtr.empty())
      {
      SafeTypeError(MIL_TEXT("MgraDots"), MIL_TEXT("The input vectors cannot be empty."));
      }
   if(PosXArrayPtr.size() != PosYArrayPtr.size())
      {
      SafeTypeError(MIL_TEXT("MgraDots"), MIL_TEXT("The input vectors must have the same size."));
      }
   static bool IsSameTypeFlag = MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT32 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT64 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_DOUBLE;
   if(IsSameTypeFlag)
      {
      SafeTypeError(MIL_TEXT("MgraDots"), MIL_TEXT("The data type must be MIL_INT32, MIL_INT64 or MIL_DOUBLE."));
      }
   if(NumberOfDots > (MIL_INT)PosXArrayPtr.size() && NumberOfDots != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MgraDots"), MIL_TEXT("NumberOfDots parameter must be smaller or equal (M_DEFAULT) to the size of input vectors."));
      }
#endif

   if(NumberOfDots == M_DEFAULT || NumberOfDots > (MIL_INT)PosXArrayPtr.size())
      {
      NumberOfDots = (MIL_INT)PosXArrayPtr.size();
      }

   MgraDots(ContextGraId,
            DstImageBufOrListGraId,
            NumberOfDots,
            &PosXArrayPtr[0],
            &PosYArrayPtr[0],
            ControlFlag);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MgraLines.
template <typename UserType>
inline void MgraLines(MIL_ID                      ContextGraId,
                      MIL_ID                      DstImageBufOrListGraId,
                      MIL_INT                     NumberOfLinesOrVertices,
                      const std::vector<UserType> &XPtr,
                      const std::vector<UserType> &YPtr,
                      CMilArrayParamIn            X2Ptr,
                      CMilArrayParamIn            Y2Ptr,
                      MIL_INT64                   ControlFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(!X2Ptr.IsValidParam() || !Y2Ptr.IsValidParam())
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("X2Ptr or Y2Ptr cannot be an non zero integer!"));
      }
   if(XPtr.size() != YPtr.size() ||
      ((MIL_INT)XPtr.size() != X2Ptr.Size<UserType>() && !X2Ptr.IsNullPtr()) ||
      ((MIL_INT)XPtr.size() != Y2Ptr.Size<UserType>() && !Y2Ptr.IsNullPtr()) ||
      ((MIL_INT)YPtr.size() != X2Ptr.Size<UserType>() && !X2Ptr.IsNullPtr()) ||
      ((MIL_INT)YPtr.size() != Y2Ptr.Size<UserType>() && !Y2Ptr.IsNullPtr()) ||
      (X2Ptr.Size<UserType>() != Y2Ptr.Size<UserType>() && !X2Ptr.IsNullPtr() && !Y2Ptr.IsNullPtr()))
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("The input vectors must have the same size!"));
      }
   if(XPtr.empty())
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("The input vectors cannot be empty."));
      }
   if(NumberOfLinesOrVertices <= 0)
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("NumberOfLinesOrVertices parameter must be greater than zero."));
      }
   static bool IsSameTypeFlag = MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT32 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT64 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_DOUBLE;
   if(IsSameTypeFlag)
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("The data type must be MIL_INT32, MIL_INT64 or MIL_DOUBLE."));
      }
   if((!X2Ptr.IsNullPtr() && X2Ptr.GetMilDataType() != MilTraits<UserType>::TypeFlag) ||
      (!Y2Ptr.IsNullPtr() && Y2Ptr.GetMilDataType() != MilTraits<UserType>::TypeFlag))
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("The data type of X2Ptr or Y2Ptr must be the same as XPtr or YPtr."));
      }
   if(NumberOfLinesOrVertices > (MIL_INT)XPtr.size() && NumberOfLinesOrVertices != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MgraLines"), MIL_TEXT("NumberOfLinesOrVertices parameter must be smaller or equal (M_DEFAULT) to the size of input vectors."));
      }
#endif

   if(NumberOfLinesOrVertices == M_DEFAULT || NumberOfLinesOrVertices > (MIL_INT)XPtr.size())
      {
      NumberOfLinesOrVertices = (MIL_INT)XPtr.size();
      }

   MgraLines(ContextGraId,
             DstImageBufOrListGraId,
             NumberOfLinesOrVertices,
             &XPtr[0],
             &YPtr[0],
             X2Ptr.GetData<UserType>(),
             Y2Ptr.GetData<UserType>(),
             ControlFlag);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MgraVectors.
template <typename UserType>
inline void MFTYPE MgraVectors(MIL_ID                      ContextGraId,
                               MIL_ID                      DstImageBufOrListGraId,
                               MIL_INT                     NumVectors,
                               const std::vector<UserType> &XArrayPtr,
                               const std::vector<UserType> &YArrayPtr,
                               const std::vector<UserType> &UArrayPtr,
                               const std::vector<UserType> &VArrayPtr,
                               MIL_INT64                   ScaleMode,
                               MIL_DOUBLE                  ScaleValue,
                               MIL_INT64                   ControlFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(NumVectors <= 0)
      {
      SafeTypeError(MIL_TEXT("MgraVectors"), MIL_TEXT("NumVectors parameter must be greater than zero."));
      }
   if(XArrayPtr.empty())
      {
      SafeTypeError(MIL_TEXT("MgraVectors"), MIL_TEXT("The input vectors cannot be empty."));
      }
   if(XArrayPtr.size() != YArrayPtr.size() ||
      XArrayPtr.size() != UArrayPtr.size() ||
      XArrayPtr.size() != VArrayPtr.size() ||
      YArrayPtr.size() != UArrayPtr.size() ||
      YArrayPtr.size() != VArrayPtr.size() ||
      UArrayPtr.size() != VArrayPtr.size())
      {
      SafeTypeError(MIL_TEXT("MgraVectors"), MIL_TEXT("The input vectors must have the same size."));
      }
   static bool IsSameTypeFlag = MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT32 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_INT64 &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_FLOAT &&
                                MilTraits<UserType>::TypeFlag != M_TYPE_MIL_DOUBLE;
   if(IsSameTypeFlag)
      {
      SafeTypeError(MIL_TEXT("MgraVectors"), MIL_TEXT("The data type must be MIL_INT32, MIL_INT64, MIL_FLOAT or MIL_DOUBLE."));
      }
   if(NumVectors > (MIL_INT)XArrayPtr.size() && NumVectors != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MgraVectors"), MIL_TEXT("NumVectors parameter must be smaller or equal (M_DEFAULT) to the size of input vectors."));
      }
#endif

   if(NumVectors == M_DEFAULT || NumVectors > (MIL_INT)XArrayPtr.size())
      {
      NumVectors = (MIL_INT)XArrayPtr.size();
      }

   MgraVectors(ContextGraId,
               DstImageBufOrListGraId,
               NumVectors,
               &XArrayPtr[0],
               &YArrayPtr[0],
               &UArrayPtr[0],
               &VArrayPtr[0],
               ScaleMode,
               ScaleValue,
               ControlFlag);
   }

#if !M_MIL_USE_LINUX || M_COMPILING_MILDLL
// ----------------------------------------------------------
// Overloads for std::vector in MseqDefine.
inline void MFTYPE MseqDefine(MIL_ID                    ContextSeqId,
                              MIL_INT                   SequenceIndex,
                              MIL_INT64                 SequenceType,
                              const std::vector<MIL_ID> &Param1Ptr,
                              MIL_DOUBLE                Param2)
   {
#if M_MIL_USE_SAFE_TYPE
   if(Param2 <= 0)
      {
      SafeTypeError(MIL_TEXT("MseqDefine"), MIL_TEXT("Param2 parameter must be greater than zero."));
      }
   if(Param1Ptr.empty())
      {
      SafeTypeError(MIL_TEXT("MseqDefine"), MIL_TEXT("The input vector cannot be empty."));
      }
   if(SequenceType != M_BUFFER_LIST)
      {
      SafeTypeError(MIL_TEXT("MseqDefine"), MIL_TEXT("Invalid SequenceType with the std::vector overload."));
      }
   if(Param2 > (MIL_INT)Param1Ptr.size() && Param2 != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MseqDefine"), MIL_TEXT("Param2 parameter must be smaller or equal (M_DEFAULT) to the size of input vector."));
      }
#endif

   if(Param2 == M_DEFAULT || Param2 > (MIL_INT)Param1Ptr.size())
      {
      Param2 = (MIL_DOUBLE)Param1Ptr.size();
      }

   MseqDefine(ContextSeqId,
              SequenceIndex,
              SequenceType,
              &Param1Ptr[0],
              Param2);
   }
#endif // !M_MIL_USE_LINUX || M_COMPILING_MILDLL

//Overload for MbufGet to support vector inquire type
template <typename UserType>
inline void MFTYPE  MbufGet(MIL_ID SrcBufId, std::vector<UserType> &UserArrayPtr)
   {
   MIL_INT InternalBufSizeX = 0;
   MIL_INT InternalBufSizeY = 0; 
   MIL_INT InternalSizeBand = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_X, &InternalBufSizeX);
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_Y, &InternalBufSizeY);
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = (InternalBufSizeX*InternalBufSizeY);

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

#if M_MIL_USE_SAFE_TYPE
   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGet"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufGet"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
#endif

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that SizeByteRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   if(InternalNbElementRequired > 0)
      {
      MbufGet(SrcBufId, &UserArrayPtr[0]);
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGet1d.
template <typename UserType>
inline void MFTYPE  MbufGet1d(MIL_ID SrcBufId, MIL_INT OffX, MIL_INT SizeX, std::vector<UserType> &UserArrayPtr)
   {
   MIL_INT InternalSizeBand = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = SizeX;

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

#if M_MIL_USE_SAFE_TYPE
   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGet1d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufGet1d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
#endif

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that SizeByteRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   // In case if SizeX = 0, call the original function which will pop an MIL error.
   if(InternalNbElementRequired > 0 || SizeX == 0)
      {
      MbufGet1d(SrcBufId, OffX, SizeX, &UserArrayPtr[0]);
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGet2d.
template <typename UserType>
inline void MFTYPE  MbufGet2d(MIL_ID SrcBufId, MIL_INT OffX, MIL_INT OffY, MIL_INT SizeX, MIL_INT SizeY, std::vector<UserType> &UserArrayPtr)
   {
   MIL_INT InternalSizeBand = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = (SizeX*SizeY);

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

#if M_MIL_USE_SAFE_TYPE
   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGet2d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufGet2d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
#endif

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   // In case if SizeX or SizeY = 0, call the original function which will pop an MIL error.
   if(InternalNbElementRequired > 0 || SizeX == 0 || SizeY == 0)
      {
      MbufGet2d(SrcBufId, OffX, OffY, SizeX, SizeY, &UserArrayPtr[0]);
      }
   }
// ----------------------------------------------------------
// Overloads for std::vector in MbufGetArc.
template <typename UserType>
inline MIL_INT MFTYPE MbufGetArc(MIL_ID ImageBufId,
                                 MIL_INT XCenter,
                                 MIL_INT YCenter,
                                 MIL_INT XRad,
                                 MIL_INT YRad,
                                 MIL_DOUBLE StartAngle,
                                 MIL_DOUBLE EndAngle,
                                 MIL_INT64 Mode,
                                 MIL_INT *NbPixelsPtr,
                                 std::vector<UserType> &ValueArrayPtr,
                                 CMilArrayParamOut     PosXArrayPtr,
                                 CMilArrayParamOut     PosYArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   if(!PosXArrayPtr.IsValidParam() || !PosYArrayPtr.IsValidParam())
      {
      SafeTypeError(MIL_TEXT("MbufGetArc"), MIL_TEXT("PosXArrayPtr and PosYArrayPtr cannot be an non zero integer."));
      }
   if(!PosXArrayPtr.IsNullPtr() && PosXArrayPtr.GetMilDataType() != MilTraits<MIL_INT>::TypeFlag)
      {
      SafeTypeError(MIL_TEXT("MbufGetArc"), MIL_TEXT("The data type of PosXArrayPtr must be MIL_INT."));
      }
   if(!PosYArrayPtr.IsNullPtr() && PosYArrayPtr.GetMilDataType() != MilTraits<MIL_INT>::TypeFlag)
      {
      SafeTypeError(MIL_TEXT("MbufGetArc"), MIL_TEXT("The data type of PosYArrayPtr must be MIL_INT."));
      }
#endif

   MIL_INT InternalNbElementRequired = 0;
   MbufGetArc(ImageBufId, XCenter, YCenter, XRad, YRad, StartAngle, EndAngle, Mode, &InternalNbElementRequired, M_NULL, M_NULL, M_NULL);

   if(InternalNbElementRequired == 0)
      {
      return 0;
      }

   MIL_INT InternalBufferType = 0;
   MbufInquire(ImageBufId, M_TYPE, &InternalBufferType);
   
   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

#if M_MIL_USE_SAFE_TYPE
   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGetArc"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufGetArc"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
#endif

   ValueArrayPtr.resize(InternalNbElementRequired);
   PosXArrayPtr.Resize<MIL_INT>(InternalNbElementRequired);
   PosYArrayPtr.Resize<MIL_INT>(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   if(InternalNbElementRequired > 0)
      {
      MbufGetArc(ImageBufId,
                 XCenter,
                 YCenter,
                 XRad,
                 YRad,
                 StartAngle,
                 EndAngle,
                 Mode,
                 NbPixelsPtr,
                 &ValueArrayPtr[0],
                 PosXArrayPtr.GetData<MIL_INT>(),
                 PosYArrayPtr.GetData<MIL_INT>());
      }
   
   return InternalNbElementRequired;
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGetColor.
template <typename UserType>
inline void MFTYPE MbufGetColor(MIL_ID SrcBufId, MIL_INT64 DataFormat, MIL_INT Band, std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      case M_PACKED + M_BGR32:
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGetColor"), MIL_TEXT("When the data format is M_PACKED, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
      break;
      default:
      break;
      }
#endif

   MIL_INT InternalBufSizeX    = 0;
   MIL_INT InternalBufSizeY    = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_X, &InternalBufSizeX);
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_Y, &InternalBufSizeY);
   
   MIL_INT InternalNbElementRequired = (InternalBufSizeX*InternalBufSizeY);

   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      {
      InternalNbElementRequired *= 2;
      }
      break;
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      {
      InternalNbElementRequired *= 3;
      }
      break;
      case M_PACKED + M_BGR32:
      {
      InternalNbElementRequired *= 4;
      }
      break;
      default:
      {
      MIL_INT InternalBufferType = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

      if(InternalBufferType == 1)
         {
         InternalNbElementRequired += 7;
         InternalNbElementRequired /= 8;
         }

#if M_MIL_USE_SAFE_TYPE
      if(InternalBufferType == 1)
         {
         bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
         if(InternalBufferTypeNotEqual)
            {
            SafeTypeError(MIL_TEXT("MbufGetColor"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
            }
         }
      else
         {
         if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
            {
            SafeTypeError(MIL_TEXT("MbufGetColor"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
            }
         }
#endif

      MIL_INT InternalSizeBand = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);
      InternalSizeBand = (Band == M_ALL_BANDS) ? InternalSizeBand : 1;

      InternalNbElementRequired *= InternalSizeBand;
      }
      break;
      }

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   if(InternalNbElementRequired > 0)
      {
      MbufGetColor(SrcBufId, DataFormat, Band, &UserArrayPtr[0]);
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGetColor2d.
template <typename UserType>
inline void MFTYPE  MbufGetColor2d(MIL_ID SrcBufId, MIL_INT64 DataFormat, MIL_INT Band, MIL_INT OffX, MIL_INT OffY, MIL_INT SizeX, MIL_INT SizeY, std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      case M_PACKED + M_BGR32:
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGetColor2d"), MIL_TEXT("When the data format is M_PACKED, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
      break;
      default:
      break;
      }
#endif

   MIL_INT InternalBufSizeX = 0;
   MIL_INT InternalBufSizeY = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_X, &InternalBufSizeX);
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_Y, &InternalBufSizeY);

   MIL_INT InternalNbElementRequired = (InternalBufSizeX*InternalBufSizeY);

   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      {
      InternalNbElementRequired *= 2;
      }
      break;
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      {
      InternalNbElementRequired *= 3;
      }
      break;
      case M_PACKED + M_BGR32:
      {
      InternalNbElementRequired *= 4;
      }
      break;
      default:
      {
      MIL_INT InternalBufferType = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

      if(InternalBufferType == 1)
         {
         InternalNbElementRequired += 7;
         InternalNbElementRequired /= 8;
         }

#if M_MIL_USE_SAFE_TYPE
      if(InternalBufferType == 1)
         {
         bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
         if(InternalBufferTypeNotEqual)
            {
            SafeTypeError(MIL_TEXT("MbufGetColor2d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
            }
         }
      else
         {
         if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
            {
            SafeTypeError(MIL_TEXT("MbufGetColor2d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
            }
         }
#endif

      MIL_INT InternalSizeBand = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);
      InternalSizeBand = (Band == M_ALL_BANDS) ? InternalSizeBand : 1;

      InternalNbElementRequired *= InternalSizeBand;
      }
      break;
      }

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   // In case if SizeX or SizeY = 0, call the original function which will pop an MIL error.
   if(InternalNbElementRequired > 0 || SizeX == 0 || SizeY == 0)
      {
      MbufGetColor2d(SrcBufId, DataFormat, Band, OffX, OffY, SizeX, SizeY, &UserArrayPtr[0]);
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGetLine.
template <typename UserType>
inline void MFTYPE MbufGetLine(MIL_ID ImageBufId, MIL_INT StartX, MIL_INT StartY, MIL_INT EndX, MIL_INT EndY, MIL_INT64 Mode, MIL_INT *NbPixelsPtr, std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   if(Mode != M_DEFAULT && MilTraits<UserType>::TypeFlag != MilTraits<MIL_INT>::TypeFlag)
      {
      SafeTypeError(MIL_TEXT("MbufGetLine"), MIL_TEXT("When Mode is not M_DEFAULT, the data type of UserArrayPtr must be MIL_INT."));
      }
#endif

   MIL_INT InternalNbElementRequired = 0;
   MbufGetLine(ImageBufId, StartX, StartY, EndX, EndY, Mode, &InternalNbElementRequired, NULL);

   if(Mode == M_DEFAULT)
      {
      MIL_INT InternalBufferType = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(ImageBufId, M_TYPE, &InternalBufferType);

      if(InternalBufferType == 1)
         {
         InternalNbElementRequired += 7;
         InternalNbElementRequired /= 8;
         }

#if M_MIL_USE_SAFE_TYPE
      if(InternalBufferType == 1)
         {
         bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
         if(InternalBufferTypeNotEqual)
            {
            SafeTypeError(MIL_TEXT("MbufGetLine"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
            }
         }
      else
         {
         if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
            {
            SafeTypeError(MIL_TEXT("MbufGetLine"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
            }
         }
#endif
      }

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   if(InternalNbElementRequired > 0)
      {
      MbufGetLine(ImageBufId, StartX, StartY, EndX, EndY, Mode, NbPixelsPtr, &UserArrayPtr[0]);
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufGetList.
template<typename UserType>
inline void MFTYPE MbufGetList(MIL_ID SrcBufId,
                               MIL_INT NumPixels,
                               const std::vector<MIL_DOUBLE> &PixXArrayPtr,
                               const std::vector<MIL_DOUBLE> &PixYArrayPtr,
                               MIL_INT64 InterpolationMode,
                               std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   if(PixXArrayPtr.empty())
      {
      SafeTypeError(MIL_TEXT("MbufGetList"), MIL_TEXT("The input vectors cannot be empty."));
      }
   if(PixXArrayPtr.size() != PixYArrayPtr.size())
      {
      SafeTypeError(MIL_TEXT("MbufGetList"), MIL_TEXT("The input vectors must have the same size."));
      }
   if(NumPixels > (MIL_INT)PixXArrayPtr.size() && NumPixels != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MbufGetList"), MIL_TEXT("NumPixels parameter must be smaller or equal (M_DEFAULT) to the size of input vectors."));
      }
#endif

   if(NumPixels == M_DEFAULT || NumPixels > (MIL_INT)PixXArrayPtr.size())
      {
      NumPixels = (MIL_INT)PixXArrayPtr.size();
      }

   MIL_INT InternalSizeBand = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = PixXArrayPtr.size();

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(SrcBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

#if M_MIL_USE_SAFE_TYPE
   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufGetList"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufGetList"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
#endif

   UserArrayPtr.resize(InternalNbElementRequired);

   //ensure that InternalNbElementRequired > 0 to avoid crash in case one of the MbufInquire returns an invalid data.
   // In case if NumPixels = 0, call the original function which will pop an MIL error.
   if(InternalNbElementRequired > 0 || NumPixels == 0)
      {
      MbufGetList(SrcBufId, NumPixels, &PixXArrayPtr[0], &PixYArrayPtr[0], InterpolationMode, &UserArrayPtr[0]);   
      }
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufPut.
template<typename UserType>
inline void MFTYPE MbufPut(MIL_ID DestBufId, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   MIL_INT InternalBufSizeX = 0;
   MIL_INT InternalBufSizeY = 0;
   MIL_INT InternalSizeBand = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_X, &InternalBufSizeX);
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_Y, &InternalBufSizeY);
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = (InternalBufSizeX*InternalBufSizeY);

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPut"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufPut"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }
   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPut"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPut(DestBufId, &UserArrayPtr[0]);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufPut1d.
template<typename UserType>
inline void MFTYPE MbufPut1d(MIL_ID DestBufId, MIL_INT OffX, MIL_INT SizeX, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   MIL_INT InternalSizeBand = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = SizeX;

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPut1d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufPut1d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPut1d"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPut1d(DestBufId, OffX, SizeX, &UserArrayPtr[0]);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufPut2d
template<typename UserType>
inline void MFTYPE MbufPut2d(MIL_ID DestBufId, MIL_INT OffX, MIL_INT OffY, MIL_INT SizeX, MIL_INT SizeY, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   MIL_INT InternalSizeBand = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = (SizeX*SizeY);

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPut2d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufPut2d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPut2d"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPut2d(DestBufId, OffX, OffY, SizeX, SizeY, &UserArrayPtr[0]);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufPutColor
template<typename UserType>
inline void MFTYPE MbufPutColor(MIL_ID DestBufId, MIL_INT64 DataFormat, MIL_INT Band, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      case M_PACKED + M_BGR32:
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPutColor"), MIL_TEXT("When the data format is M_PACKED, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
      break;
      default:
      break;
      }

   MIL_INT InternalBufSizeX = 0;
   MIL_INT InternalBufSizeY = 0;

   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_X, &InternalBufSizeX);
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_Y, &InternalBufSizeY);

   MIL_INT InternalNbElementRequired = (InternalBufSizeX*InternalBufSizeY);

   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      {
      InternalNbElementRequired *= 2;
      }
      break;
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      {
      InternalNbElementRequired *= 3;
      }
      break;
      case M_PACKED + M_BGR32:
      {
      InternalNbElementRequired *= 4;
      }
      break;
      default:
      {
      MIL_INT InternalBufferType = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

      if(InternalBufferType == 1)
         {
         InternalNbElementRequired += 7;
         InternalNbElementRequired /= 8;
         }

      if(InternalBufferType == 1)
         {
         bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
         if(InternalBufferTypeNotEqual)
            {
            SafeTypeError(MIL_TEXT("MbufPutColor"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
            }
         }
      else
         {
         if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
            {
            SafeTypeError(MIL_TEXT("MbufPutColor"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
            }
         }

      MIL_INT InternalSizeBand = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);
      InternalSizeBand = (Band == M_ALL_BANDS) ? InternalSizeBand : 1;

      InternalNbElementRequired *= InternalSizeBand;
      }
      break;
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPutColor"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPutColor(DestBufId, DataFormat, Band, &UserArrayPtr[0]);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MbufPutColor2d
template<typename UserType>
inline void MFTYPE MbufPutColor2d(MIL_ID DestBufId, MIL_INT64 DataFormat, MIL_INT Band, MIL_INT OffX, MIL_INT OffY, MIL_INT SizeX, MIL_INT SizeY, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      case M_PACKED + M_BGR32:
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPutColor2d"), MIL_TEXT("When the data format is M_PACKED, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
      break;
      default:
         break;
      }

   MIL_INT InternalNbElementRequired = (SizeX*SizeY);

   switch(DataFormat)
      {
      case M_PACKED + M_RGB15:
      case M_PACKED + M_RGB16:
      {
      InternalNbElementRequired *= 2;
      }
      break;
      case M_PACKED + M_BGR24:
      case M_PACKED + M_RGB24:
      {
      InternalNbElementRequired *= 3;
      }
      break;
      case M_PACKED + M_BGR32:
      {
      InternalNbElementRequired *= 4;
      }
      break;
      default:
      {
      MIL_INT InternalBufferType = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

      if(InternalBufferType == 1)
         {
         InternalNbElementRequired += 7;
         InternalNbElementRequired /= 8;
         }

      if(InternalBufferType == 1)
         {
         bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
         if(InternalBufferTypeNotEqual)
            {
            SafeTypeError(MIL_TEXT("MbufPutColor2d"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
            }
         }
      else
         {
         if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
            {
            SafeTypeError(MIL_TEXT("MbufPutColor2d"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
            }
         }

      MIL_INT InternalSizeBand = 0;
      RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);
      InternalSizeBand = (Band == M_ALL_BANDS) ? InternalSizeBand : 1;

      InternalNbElementRequired *= InternalSizeBand;
      }
      break;
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPutColor2d"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPutColor2d(DestBufId, DataFormat, Band, OffX, OffY, SizeX, SizeY, &UserArrayPtr[0]);
   }

// ----------------------------------------------------------
// Overloads for std::vector MbufPutLine
template<typename UserType>
inline void MFTYPE MbufPutLine(MIL_ID ImageBufId, MIL_INT StartX, MIL_INT StartY, MIL_INT EndX, MIL_INT EndY, MIL_INT64 Mode, MIL_INT *NbPixelsPtr, const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   MIL_INT InternalNbElementRequired = 0;
   MbufPutLine(ImageBufId, StartX, StartY, EndX, EndY, Mode, &InternalNbElementRequired, NULL);

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(ImageBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPutLine"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufPutLine"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPutLine"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   MbufPutLine(ImageBufId, StartX, StartY, EndX, EndY, Mode, NbPixelsPtr, &UserArrayPtr[0]);
   }
// ----------------------------------------------------------
// Overloads for std::vector in MbufPutList.
template<typename UserType>
inline void MFTYPE MbufPutList(MIL_ID DestBufId,
                               MIL_INT NumPixels,
                               const std::vector<MIL_DOUBLE> &PixXArrayPtr,
                               const std::vector<MIL_DOUBLE> &PixYArrayPtr,
                               MIL_INT64 OverscanMode,
                               const std::vector<UserType> &UserArrayPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   if(PixXArrayPtr.empty())
      {
      SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("The input vectors cannot be empty."));
      }
   if(PixXArrayPtr.size() != PixYArrayPtr.size())
      {
      SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("The input vectors must have the same size."));
      }
   if(NumPixels > (MIL_INT)PixXArrayPtr.size() && NumPixels != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("NumPixels parameter must be smaller or equal (M_DEFAULT) to the size of input vectors."));
      }

   MIL_INT InternalSizeBand = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_SIZE_BAND, &InternalSizeBand);

   MIL_INT InternalNbElementRequired = PixXArrayPtr.size();

   MIL_INT InternalBufferType = 0;
   RUN_BUFINQUIRE_RETURNIFFAIL(DestBufId, M_TYPE, &InternalBufferType);

   if(InternalBufferType == 1)
      {
      InternalNbElementRequired += 7;
      InternalNbElementRequired /= 8;
      }

   InternalNbElementRequired *= InternalSizeBand;

   if(InternalBufferType == 1)
      {
      bool InternalBufferTypeNotEqual = MilTraits<UserType>::BufferTypeFlag != MilTraits<MIL_UINT8>::BufferTypeFlag;
      if(InternalBufferTypeNotEqual)
         {
         SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("When the buffer type of the image is 1, the data type of UserArrayPtr must be MIL_UINT8."));
         }
      }
   else
      {
      if(InternalBufferType != MilTraits<UserType>::BufferTypeFlag)
         {
         SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("Vector data type does not match the buffer data type. Please try again with the right data type."));
         }
      }

   if((MIL_INT)UserArrayPtr.size() < InternalNbElementRequired)
      {
      SafeTypeError(MIL_TEXT("MbufPutList"), MIL_TEXT("Vector size is not big enough for the required size."));
      }
#endif

   if(NumPixels == M_DEFAULT || NumPixels > (MIL_INT)PixXArrayPtr.size())
      {
      NumPixels = (MIL_INT)PixXArrayPtr.size();
      }

   MbufPutList(DestBufId, NumPixels, &PixXArrayPtr[0], &PixYArrayPtr[0], OverscanMode, &UserArrayPtr[0]);
   }

//After discussing with Michel, MbufInquireFeature;MdigInquireFeature;MsysInquireFeature for now

// ----------------------------------------------------------
// Overloads for std::vector in MobjMessageWrite.
inline void MFTYPE MobjMessageWrite(MIL_ID MessageId, const std::vector<MIL_UINT8> &MessagePtr, MIL_INT64 MessageSize, MIL_INT64 MessageTag, MIL_INT64 OperationFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(MessageSize <= 0)
      {
      SafeTypeError(MIL_TEXT("MobjMessageWrite"), MIL_TEXT("MessageSize must be greater than zero."));
      }
   if(MessageSize > (MIL_INT64)MessagePtr.size() && MessageSize != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MobjMessageWrite"), MIL_TEXT("MessageSize parameter must be smaller or equal (M_DEFAULT) to the size of input vector."));
      }
#endif

   if(MessageSize == M_DEFAULT || MessageSize > (MIL_INT64)MessagePtr.size())
      {
      MessageSize = (MIL_INT64)MessagePtr.size();
      }

   MobjMessageWrite(MessageId, &MessagePtr[0], MessageSize, MessageTag, OperationFlag);
   }

// ----------------------------------------------------------
// Overloads for std::vector in MobjMessageRead.
inline MIL_INT64 MFTYPE MobjMessageRead(MIL_ID MessageId, std::vector<MIL_UINT8> &MessagePtr, MIL_INT64 MessageInSize, MIL_INT64 *MessageOutSizePtr, MIL_INT64 *MessageTagPtr, MIL_INT64 *StatusPtr, MIL_INT64  OperationFlag)
   {
#if M_MIL_USE_SAFE_TYPE
   if(MessageInSize <= 0)
      {
      SafeTypeError(MIL_TEXT("MobjMessageRead"), MIL_TEXT("MessageInSize must be greater than zero."));
      }
#endif

   if(MessageInSize == M_DEFAULT)
      {
      MessageInSize = 0;
      MessageInSize = MobjMessageRead(MessageId, M_NULL, M_NULL, M_NULL, M_NULL, StatusPtr, OperationFlag);
      }

   MessagePtr.resize((MIL_INT)MessageInSize);

   if(MessageInSize > 0)
      {
      MIL_INT64 InternalNeededSizeForStdVector = MobjMessageRead(MessageId, &MessagePtr[0], MessageInSize, MessageOutSizePtr, MessageTagPtr, StatusPtr, OperationFlag);

      if(InternalNeededSizeForStdVector < MessageInSize)
         {
         MessagePtr.resize((MIL_INT)InternalNeededSizeForStdVector);
         }
      return InternalNeededSizeForStdVector;
      }
   return 0;
   }

// ----------------------------------------------------------
// Overloads for std::vector in MthrInquireMp.
inline MIL_INT MFTYPE MthrInquireMp(MIL_ID ThrId, MIL_INT64 InquireType, MIL_INT64 TypeFlag, MIL_INT64 TypeValue, std::vector<MIL_UINT64> &ResultPtr)
   {
   MIL_INT RetVal = 0;

   if(InquireType == M_CORE_AFFINITY_MASK)
      {
      MIL_INT MaskArraySz = MthrInquireMp(ThrId, M_CORE_AFFINITY_MASK_ARRAY_SIZE, M_DEFAULT, M_DEFAULT, M_NULL);
      ResultPtr.resize(MaskArraySz);
      RetVal = MthrInquireMp(ThrId, InquireType, TypeFlag, TypeValue, &ResultPtr[0]);
      }
   else
      {
      ResultPtr.resize(1);
      RetVal = MthrInquireMp(ThrId, InquireType, TypeFlag, TypeValue, &ResultPtr[0]);
      }
   return RetVal;
   }

// ----------------------------------------------------------
// Overloads for std::vector in MappInquireConnection.
inline MIL_INT MFTYPE MappInquireConnection(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT64 ControlFlag, MIL_INT64 ExtraFlag, std::vector<MIL_ID> &UserVarPtr)
   {
   MIL_INT RetVal = 0;

   if(InquireType == M_DMIL_PUBLISHED_LIST)
      {
      MIL_INT PublishListSz = MappInquireConnection(ContextAppId, M_DMIL_PUBLISHED_LIST_SIZE, ControlFlag, ExtraFlag, M_NULL);
      UserVarPtr.resize(PublishListSz);
      if(PublishListSz > 0)
         {
         RetVal = MappInquireConnection(ContextAppId, InquireType, ControlFlag, ExtraFlag, &UserVarPtr[0]);
         }
      }
   else
      {
      UserVarPtr.resize(1);
      RetVal = MappInquireConnection(ContextAppId, InquireType, ControlFlag, ExtraFlag, &UserVarPtr[0]);
      }
   return RetVal;
   }

// ----------------------------------------------------------
// Overloads for std::vector in MappInquireMp.
inline MIL_INT MFTYPE MappInquireMp(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT64 TypeFlag, MIL_INT64 TypeValue, std::vector<MIL_UINT64> &UserVarPtr)
   {
   MIL_INT RetVal = 0;
   if((InquireType == M_CORE_AFFINITY_MASK) || (InquireType == M_CORE_AFFINITY_MASK_PROCESS) || (InquireType == M_MEMORY_BANK_AFFINITY_MASK) || (InquireType == M_MEMORY_BANK_CORE_AFFINITY_MASK))
      {
      MIL_INT AffinityMaskArraySize = MappInquireMp(ContextAppId, M_CORE_AFFINITY_MASK_ARRAY_SIZE, TypeFlag, TypeValue, M_NULL);
      UserVarPtr.resize(AffinityMaskArraySize);
      RetVal = MappInquireMp(ContextAppId, InquireType, TypeFlag, TypeValue, &UserVarPtr[0]);
      }
   else
      {
      UserVarPtr.resize(1);
      RetVal = MappInquireMp(ContextAppId, InquireType, TypeFlag, TypeValue, &UserVarPtr[0]);
      }
   return RetVal;
   }

// ----------------------------------------------------------
// Overloads for std::vector in MdigProcess.
inline void MFTYPE MdigProcess(MIL_ID DigId, const std::vector<MIL_ID> &DestImageArrayPtr, MIL_INT ImageCount, MIL_INT64 Operation, MIL_INT64 OperationFlag, MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void *UserDataPtr)
   {
#if M_MIL_USE_SAFE_TYPE
   if(ImageCount <= 0)
      {
      SafeTypeError(MIL_TEXT("MdigProcess"), MIL_TEXT("ImageCount must be greater than zero."));
      }
   if(ImageCount > (MIL_INT)DestImageArrayPtr.size() && ImageCount != M_DEFAULT)
      {
      SafeTypeError(MIL_TEXT("MdigProcess"), MIL_TEXT("ImageCount parameter must be smaller or equal (M_DEFAULT) to the size of input vector."));
      }
#endif

   if(ImageCount == M_DEFAULT || ImageCount > (MIL_INT)DestImageArrayPtr.size())
      {
      ImageCount = (MIL_INT)DestImageArrayPtr.size();
      }

   MdigProcess(DigId, &DestImageArrayPtr[0], ImageCount, Operation, OperationFlag, HookHandlerPtr, UserDataPtr);
   }   

#endif // M_MIL_USE_VECTOR

#endif /* __MILVECTOR_H__ */	
#endif /* __MILPROTO_H */
