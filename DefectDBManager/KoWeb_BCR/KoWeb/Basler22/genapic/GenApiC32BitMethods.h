/**
  \file
  \brief GenApi C bindings  - 32 bit functions.
*/

#ifndef GenApiC32BitMethods_h__
#define GenApiC32BitMethods_h__

#include <genapic/GenApiCDefines.h>
#include <genapic/GenApiCTypes.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiNodeGetPollingTimeInt32( NODE_HANDLE hNode,  int32_t *pollingTime );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerSetValueInt32( NODE_HANDLE hNode, int32_t value );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerSetValueExInt32( NODE_HANDLE hNode, _Bool verify, int32_t value );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerGetValueInt32( NODE_HANDLE hNode,  int32_t* pValue );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerGetValueExInt32( NODE_HANDLE hNode, _Bool verify, int32_t* pValue );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerGetMinInt32( NODE_HANDLE hNode,  int32_t* pValue );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerGetMaxInt32( NODE_HANDLE hNode,  int32_t* pValue );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiIntegerGetIncInt32( NODE_HANDLE hNode,  int32_t* pValue );
        GENAPIC_API GENAPIC_RESULT GENAPIC_CC GenApiNodeMapPollInt32( NODEMAP_HANDLE hMap, int32_t timestamp);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // GenApiC32BitMethods_h__
