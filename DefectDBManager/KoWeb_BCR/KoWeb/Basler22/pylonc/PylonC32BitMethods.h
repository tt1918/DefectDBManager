/*-----------------------------------------------------------------------------
(c) 2009 by Basler Vision Technologies
Section: Basler Components
Project: PYLON
Author:  AH, TK
-----------------------------------------------------------------------------*/

#ifndef PylonC32BitMethods_h__
#define PylonC32BitMethods_h__

#include <genapic/GenApiCDefines.h>
#include <genapic/GenApiCTypes.h>
#include <pylonc/PylonCDefines.h>
#include <pylonc/PylonC.h>

/**
  \file
  \brief pylon C bindings - 32 bit functions.
*/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

PYLONC_API GENAPIC_RESULT PYLONC_CC
PylonDeviceSetIntegerFeatureInt32(PYLON_DEVICE_HANDLE dev, const char *name, int32_t value);

PYLONC_API GENAPIC_RESULT PYLONC_CC
PylonDeviceGetIntegerFeatureInt32(PYLON_DEVICE_HANDLE dev, const char *name, int32_t *value);

PYLONC_API GENAPIC_RESULT PYLONC_CC
PylonDeviceGetIntegerFeatureMinInt32(PYLON_DEVICE_HANDLE dev, const char *name, int32_t *value);

PYLONC_API GENAPIC_RESULT PYLONC_CC
PylonDeviceGetIntegerFeatureMaxInt32(PYLON_DEVICE_HANDLE dev, const char *name, int32_t *value);

PYLONC_API GENAPIC_RESULT PYLONC_CC
PylonDeviceGetIntegerFeatureIncInt32(PYLON_DEVICE_HANDLE dev, const char *name, int32_t *value);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // PylonC32BitMethods_h__
