#ifndef PylonCDefines_h__
#define PylonCDefines_h__

/**
  \file
  \brief pylon C bindings - macro definitions.
*/


#include <genapic/GenApiCDefines.h>

#ifdef GENAPIC_WIN_BUILD
#  ifdef PYLONC_EXPORTS
#    define PYLONC_API __declspec(dllexport)
#  else
#    define PYLONC_API __declspec(dllimport)
#  endif
#  define PYLONC_CC _stdcall
#elif defined GENAPIC_LINUX_BUILD
#    define PYLONC_API
#    define PYLONC_CC
#else
#  error Unsupported platform
#endif

#define PYLON_DEVINFO_FULL_NAME_KEY "FullName"
#define PYLON_DEVINFO_FRIENDLY_NAME_KEY "FriendlyName"
#define PYLON_DEVINFO_VENDOR_NAME_KEY "VendorName"
#define PYLON_DEVINFO_DEVICE_CLASS_KEY "DeviceClass"
#define PYLON_DEVINFO_SERIAL_NUMBER_KEY "SerialNumber"
#define PYLON_DEVINFO_USER_DEFINED_NAME_KEY "UserDefinedName"
#define PYLON_DEVINFO_MODEL_NAME_KEY "ModelName"
#define PYLON_DEVINFO_DEVICE_FACTORY_KEY "DeviceFactory"
#define PYLON_DEVINFO_DEVICE_VERSION_KEY "DeviceVersion"



#endif // PylonCDefines_h__
