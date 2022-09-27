#ifndef PYLON_ENUMS_H
#define PYLON_ENUMS_H

#include <genapic/GenApiCDefines.h>

/**
 * \file
 * \brief pylon enumeration types.
 *
 * Definitions of enumerated types used by Pylon C functions.
 */

/** 
 * \addtogroup pylon pylon Interface
 * @{
 */

/**
 * \brief The reason why a wait operation terminated.
 */
IDL_PUBLIC
typedef enum
{
    waitex_timeout    =  0, /*!< \brief The time-out interval elapsed */
    waitex_signaled   =  1, /*!< \brief The wait operation completed successfully */
    waitex_abandoned  =  2, /*!< \brief Windows only (see MSDN for more information) */
    waitex_alerted    = -1 /*!< \brief The wait was interrupted (Windows: queued APC or I/O completion routine; UNIX: signal) */
} EPylonWaitExResult;

/**
 * \brief The grab status.
 */
IDL_PUBLIC
typedef enum
{
    UndefinedGrabStatus = -1,   /*!< \brief status not defined */
    Idle,                       /*!< \brief currently not used */
    Queued,                     /*!< \brief in the input queue */
    Grabbed,                    /*!< \brief filled with data */
    Canceled,                   /*!< \brief request was canceled */
    Failed                      /*!< \brief request failed */
} EPylonGrabStatus;

/**
  * \brief PayloadType for PylonGrabResult_t
  */
IDL_PUBLIC
typedef enum
{
    PayloadType_Undefined = -1,          /*!< \brief The buffer content is undefined. */
    PayloadType_Image,                   /*!< \brief The buffer contains image data. */
    PayloadType_RawData,                 /*!< \brief The buffer contains raw data. */
    PayloadType_File,                    /*!< \brief The buffer contains file data. */
    PayloadType_ChunkData,               /*!< \brief The buffer contains chunk data. */
    PayloadType_DeviceSpecific = 0x8000  /*!< \brief The buffer contains device specific data. */
} EPylonPayloadType;

#define PIXEL_MONO          0x01000000
#define PIXEL_COLOR         0x02000000
#define PIXEL_BIT_COUNT(n)  ((n) << 16)

/*! \brief List all possible pixel formats. See the camera User's Manual for a detailed description of the available pixel formats. */
IDL_PUBLIC
typedef enum
{
    PixelType_Undefined         = -1, /*!< \brief Indicates that the pixel format is undefined or not valid. */
    PixelType_Mono8             = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x0001, /*!< \brief Indicates the Mono 8 pixel format. */
    PixelType_Mono8signed       = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x0002, /*!< \brief Indicates the Mono 8 Signed pixel format. */
    PixelType_Mono10            = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0003, /*!< \brief Indicates the Mono 10 pixel format. */
    PixelType_Mono10packed      = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x0004, /*!< \brief Indicates the Mono 10 Packed pixel format. */
    PixelType_Mono12            = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0005, /*!< \brief Indicates the Mono 12 pixel format. */
    PixelType_Mono12packed      = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x0006, /*!< \brief Indicates the Mono 12 Packed pixel format. */
    PixelType_Mono16            = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0007, /*!< \brief Indicates the Mono 16 pixel format. */

    PixelType_BayerGR8          = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x0008, /*!< \brief Indicates the Bayer GR 8 pixel format. */
    PixelType_BayerRG8          = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x0009, /*!< \brief Indicates the Bayer RG 8 pixel format. */
    PixelType_BayerGB8          = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x000a, /*!< \brief Indicates the Bayer GB 8 pixel format. */
    PixelType_BayerBG8          = PIXEL_MONO | PIXEL_BIT_COUNT(8)  | 0x000b, /*!< \brief Indicates the Bayer BG 8 pixel format. */

    PixelType_BayerGR10         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x000c, /*!< \brief Indicates the Bayer GR 10 pixel format. */
    PixelType_BayerRG10         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x000d, /*!< \brief Indicates the Bayer RG 10 pixel format. */
    PixelType_BayerGB10         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x000e, /*!< \brief Indicates the Bayer GB 10 pixel format. */
    PixelType_BayerBG10         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x000f, /*!< \brief Indicates the Bayer BG 10 pixel format. */

    PixelType_BayerGR12         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0010, /*!< \brief Indicates the Bayer GR 12 pixel format. */
    PixelType_BayerRG12         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0011, /*!< \brief Indicates the Bayer RG 12 pixel format. */
    PixelType_BayerGB12         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0012, /*!< \brief Indicates the Bayer GB 12 pixel format. */
    PixelType_BayerBG12         = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0013, /*!< \brief Indicates the Bayer BG 12 pixel format. */

    PixelType_RGB8packed        = PIXEL_COLOR | PIXEL_BIT_COUNT(24) | 0x0014, /*!< \brief Indicates the RGB 8 Packed pixel format. */
    PixelType_BGR8packed        = PIXEL_COLOR | PIXEL_BIT_COUNT(24) | 0x0015, /*!< \brief Indicates the BGR 8 Packed pixel format. */

    PixelType_RGBA8packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(32) | 0x0016, /*!< \brief Indicates the RGBA 8 Packed pixel format. */
    PixelType_BGRA8packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(32) | 0x0017, /*!< \brief Indicates the BGRA 8 Packed pixel format. */

    PixelType_RGB10packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x0018, /*!< \brief Indicates the RGB 10 Packed pixel format. */
    PixelType_BGR10packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x0019, /*!< \brief Indicates the BGR 10 Packed pixel format. */

    PixelType_RGB12packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x001a, /*!< \brief Indicates the RGB 12 Packed pixel format. */
    PixelType_BGR12packed       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x001b, /*!< \brief Indicates the BGR 12 Packed pixel format. */

    PixelType_BGR10V1packed     = PIXEL_COLOR | PIXEL_BIT_COUNT(32) | 0x001c, /*!< \brief Indicates the BGR 10 V1 Packed pixel format. */
    PixelType_BGR10V2packed     = PIXEL_COLOR | PIXEL_BIT_COUNT(32) | 0x001d, /*!< \brief Indicates the BGR 10 V2 Packed pixel format. */

    PixelType_YUV411packed      = PIXEL_COLOR | PIXEL_BIT_COUNT(12) | 0x001e, /*!< \brief Indicates the YUV 411 Packed pixel format. */
    PixelType_YUV422packed      = PIXEL_COLOR | PIXEL_BIT_COUNT(16) | 0x001f, /*!< \brief Indicates the YUV 422 Packed pixel format. */
    PixelType_YUV444packed      = PIXEL_COLOR | PIXEL_BIT_COUNT(24) | 0x0020, /*!< \brief Indicates the YUV 444 Packed pixel format. */

    PixelType_RGB8planar        = PIXEL_COLOR | PIXEL_BIT_COUNT(24) | 0x0021, /*!< \brief Indicates the RGB 8 Planar pixel format. */
    PixelType_RGB10planar       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x0022, /*!< \brief Indicates the RGB 10 Planar pixel format. */
    PixelType_RGB12planar       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x0023, /*!< \brief Indicates the RGB 12 Planar pixel format. */
    PixelType_RGB16planar       = PIXEL_COLOR | PIXEL_BIT_COUNT(48) | 0x0024, /*!< \brief Indicates the RGB 16 Planar pixel format. */

    PixelType_YUV422_YUYV_Packed = PIXEL_COLOR | PIXEL_BIT_COUNT(16) | 0x0032, /*!< \brief Indicates the pixel format YUV 422 (YUYV) Packed. */

    PixelType_BayerGR12Packed = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x002A, /*!< \brief Indicates the pixel format Bayer GR 12 Packed. */
    PixelType_BayerRG12Packed = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x002B, /*!< \brief Indicates the pixel format Bayer RG 12 Packed. */
    PixelType_BayerGB12Packed = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x002C, /*!< \brief Indicates the pixel format Bayer GB 12 Packed. */
    PixelType_BayerBG12Packed = PIXEL_MONO | PIXEL_BIT_COUNT(12) | 0x002D, /*!< \brief Indicates the pixel format Bayer BG 12 Packed. */

    PixelType_BayerGR16       = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x002E, /*!< \brief Indicates the pixel format Bayer GR 16. */
    PixelType_BayerRG16       = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x002F, /*!< \brief Indicates the pixel format Bayer RG 16. */
    PixelType_BayerGB16       = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0030, /*!< \brief Indicates the pixel format Bayer GB 16. */
    PixelType_BayerBG16       = PIXEL_MONO | PIXEL_BIT_COUNT(16) | 0x0031, /*!< \brief Indicates the pixel format Bayer BG 16. */

    PixelType_RGB12V1packed   = PIXEL_COLOR | PIXEL_BIT_COUNT(36) | 0X0034, /*!< \brief Indicates the pixel format RGB 12 V1 packed. */

    // custom codes
    PixelType_Double            =  0x80000000 | PIXEL_MONO | PIXEL_BIT_COUNT(48) | 0x100 /*!< \brief Indicates the pixel format Double. */
} EPylonPixelType;

/** 
 * @}
 */

#endif /* PYLON_ENUMS_H */
