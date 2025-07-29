/** @copyright (c) 2001-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-videodevice.h
*  @ingroup  sdk_TVPA
*
*  @brief  An input video device, like a wecam.
*/

#ifndef __pi_videodevice_HH
#define __pi_videodevice_HH


#include  "pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIVideoDevice  An input video device, like a webcam.
*/


//===============================================================================================================


/** @brief  The different types of "video" pixels.

    @ingroup  sdk_PIVideoDevice
*/
enum  ePixelType
{
    PIXTYPE_BGRA32 = 0,  ///< RGBA 32 bits
    PIXTYPE_UYVY   = 1,  ///< 16 bits YUYV FOURCC('UYVY')
    PIXTYPE_YUY2   = 2,  ///< 16 bits UYVY FOURCC('YUY2')
    PIXTYPE_BGR32  = 3   ///< RGB 32 bits
};


/** @brief  Description of one video mode of one device.

    @ingroup  sdk_PIVideoDevice
*/
typedef  struct  PIVideoInfo
{
    /** @brief  Name of the video mode.
        Something like "Toaster Display In"
     */
    char  Name[1024];
    /** @brief  Width of the images in pixels. */
    int  Width;
    /** @brief  Height of the images in pixels. */
    int  Height;
    /** @brief  Number of bytes per pixel. */
    int  BytesPerPixel;
    /** @brief  The type of pixels, one of the ePixelType enumerators.*/
    int  PixelType;
    /** @brief  Aspect ratio of the pixels.
        For compatibility, 1.0 can also be represented with 0.0, AVOID this in new code. 
     */
    double  AspectRatio;
    /** @brief  The frame rate of this mode. */
    double  FrameRate;
    /** @brief  The order of the fields.
        0 = none (progressive)
        1 = lower/odd first
        2 = upper/even first
     */
    int  FieldMode;

    /** @brief  Reserved. Leave it alone ! Set to 0 at creation time. */
    int Reserved[64];
} PIVideoInfo;


//===============================================================================================================


/** @brief  A video block.
    This is like a PIBlock, but specialized for the quirks of video frames "close to the hardware".

    @ingroup  sdk_PIVideoDevice
*/
typedef  struct  PIVideoBlock
{
    /** @brief  Width of the PIVideoBlock in pixels. */
    int  Width;

    /** @brief  Height of the PIVideoBlock in pixels. */
    int  Height;

    /** @brief  Which fields are present in this block ?
        - 0 = Progressive frame.
        - 1 = Odd field only.
        - 2 = Even field only.
        - 3 = Even and Odd fields (interlaced frame).
     */
    int  Field;

    /** @brief  Number of bytes per pixel. */
    int  BytesPerPixel;

    /** @brief  Modulo in bytes for the color components.
        If it's a negative value, it means the VideoBlock is reversed (from bottom to top).
     */
    int  Pitch;

    /** @brief  The type of pixels, one of the ePixelType enumerators.*/
    int  PixelType;

    /** @deprecated  Use the Alpha0,Alpha1,Field0,Field1 fields instead.*/
    PIUInt8*  Data;

    /** @brief  Modulo in bytes for the alpha components.
        If it's a negative value, it means the VideoBlock is reversed (from bottom to top).
     */
    int  AlphaPitch;

    /** @brief  Alpha channel of the first pixel of the first field. */
    PIUInt8*  Alpha0;

    /** @brief  Alpha channel of the first pixel of the second field. */
    PIUInt8*  Alpha1;

    /** @brief  Color channel of the first pixel of the first field. */
    PIUInt8*  Field0;

    /** @brief  Color channel of the first pixel of the second field. */
    PIUInt8*  Field1;

    /** @brief  The order of the fields.
        0 = none (progressive)
        1 = lower/odd first
        2 = upper/even first
     */
    int  FieldMode;

    /** @brief  Aspect ratio of the pixels.
        For compatibility, 1.0 can also be coded as 0.0, AVOID this in new code. 
     */
    double  AspectRatio;

    /** @brief  Reserved. Leave it alone ! Set to 0 at creation time. */
    int  Reserved[64];
} PIVideoBlock;



//===============================================================================================================


/** @brief  The ID of a video device.

    @ingroup  sdk_PIVideoDevice
*/
typedef  void*  PIVideoDeviceID;


//===============================================================================================================


/** @brief  The current version of the PIVideoDevice structure.

    @ingroup  sdk_PIVideoDevice
 */
#define  PIVIDEODEVICE_VERSION  2

//TVDEPRECATED( "Renamed as PIVIDEODEVICE_VERSION" )
/* * @deprecated  Renamed as PIVIDEODEVICE_VERSION. */
/** @brief  DEPRECATED : Use PIVIDEODEVICE_VERSION instead. */
#define  VIDEODEVICE_VERSION  PIVIDEODEVICE_VERSION


/** @brief  A Video Device.
    This is a device which can acquire images, like a webcam.   

    @ingroup  sdk_PIVideoDevice
*/
typedef  struct  PIVideoDevice
{
    /** @brief  Version number of this structure. Use PIVIDEODEVICE_VERSION to initialize this.*/
    int  Version;

    /** @brief  Name of this device. */
    const char*  DeviceName;

    /** @brief  Enumerates video device inputs names. It must returns NULL if there is no Input at the specified iInputIndex. */
    const char*  (*EnumInputs)( struct PIVideoDevice*  iVideoDevice, int  iInputIndex );

    /** @brief  Enumerates video device modes.
        MUST fill in oVideoInfo if the combinaison of iInputIndex and iModeIndex is valid.
        @return  An error code: 0 = An error occured (most probably the combinaison of iInputIndex and iModeIndex isn't valid), 1 = Everything went well.
     */
    int  (*EnumModes)( struct PIVideoDevice*  iVideoDevice, int  iInputIndex, int  iModeIndex, PIVideoInfo*  oVideoInfo );

    /** @brief  Starts grabbing video frames.
        Should start a thread which will grab a frame, call iGrabThread with that frame, and loop until Stop is called.
        @return  A PIVideoDeviceID, which should be passed to Stop when time comes to stop.
     */
    PIVideoDeviceID  (*Start)( struct PIVideoDevice*  iVideoDevice, int  iInputIndex, int  iModeIndex, int  (*iGrabThread)( PIVideoBlock*  iBlock, void*  iUserData ), void*  iUserData );

    /** @brief  Stops grabbing video frames. */
    void  (*Stop)( struct PIVideoDevice*  iVideoDevice, PIVideoDeviceID  iID );

    /** @brief  Opens the configuration panel of this device. */
    void  (*Configure)( struct PIVideoDevice*  iVideoDevice );

    /** @brief  Does this device have a configuration panel ?
        @return  1 if it has one, 0 if it doesn't.
     */
    int  (*CanConfigure)( struct PIVideoDevice*  iVideoDevice );

    /** @brief  Enumerates the acquire modes supported by the video device.

        @param[in]  iVideoDevice  The video device.
        @param[in]  iIndex        The index of the mode to get.

        @return  The name of the mode, or NULL if there is no mode with index iIndex.
     */
    const char*  (*EnumAcquireMode)( struct PIVideoDevice*  iVideoDevice, int  iIndex );

    /** @brief  Acquires one image from the video device using the given mode.

        @param[in]  iVideoDevice  The video device.
        @param[in]  iIndex        The index of the mode to use to acquire the image.

        @return  The name of the mode, or NULL if there is no mode with index iIndex.
     */
    struct PIBlock*  (*AcquireImage)( struct PIVideoDevice*  iVideoDevice, int  iIndex );

    /** @brief  Reserved. Leave it alone ! Set to 0 at creation time. */
    PIUInt32  reserved[1022];
} PIVideoDevice;


//===============================================================================================================


/** @brief  This function returns the video device according to its index in the Video Device list.
    The Video Device list goes from 0 to some maximum number of devices.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iIndex   The index of the video device we wan't to get.

    @return  The video device at iIndex, or NULL if there isn't any at that index.

    @see  TVAddVideoDevice(), TVAddVideoButton().

    @ingroup  sdk_PIVideoDevice
*/
PIVideoDevice*  TVEnumVideoDevice( PIPlugin*  iPlugin, int  iIndex );


/** @brief  Adds a new video device.

    @param[in]  iPlugin       This plugin's reference.
    @param[in]  iVideoDevice  The new video device reference.

    @see  TVEnumVideoDevice(), TVAddVideoButton().

    @ingroup  sdk_PIVideoDevice
*/
void  TVAddVideoDevice( PIPlugin*  iPlugin, PIVideoDevice*  iVideoDevice );

/** @brief  Adds a button to the GUI of the plugin, displaying the input of the videodevice.

    @param[in]  iPlugin       This plugin's reference.
    @param[in]  iReq          The ID of the window.
    @param[in]  iX            The upper left corner of the area to refresh.
    @param[in]  iY            The upper left corner of the area to refresh.
    @param[in]  iW            The width of the area to refresh..
    @param[in]  iH            The height of the area to refresh..
    @param[in]  iID           The unique (per window) ID of the button.
    @param[in]  iVideoDevice  The video device to display in the button.
    @param[in]  iVideoInput   The input of the video device to use.
    @param[in]  iVideoMode    The mode of the video device to use.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVEnumVideoDevice(), TVAddVideoDevice().

    @ingroup  sdk_PIVideoDevice
*/
int  TVAddVideoButton( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, PIVideoDevice*  iVideoDevice, int  iVideoInput, int  iVideoMode );


//=============================================================================================================== Deprecations


TVDEPRECATED( "Renamed as PIVideoInfo" )
/** @deprecated  Renamed as PIVideoInfo. */
typedef  PIVideoInfo  VideoInfo;


TVDEPRECATED( "Renamed as PIVideoBlock" )
/** @deprecated  Renamed as PIVideoBlock. */
typedef  PIVideoBlock  VideoBlock;


TVDEPRECATED( "Renamed as PIVideoDeviceID" )
/** @deprecated  Renamed as PIVideoDeviceID. */
typedef  PIVideoDeviceID  VideoDevID;


TVDEPRECATED( "Renamed as PIVideoDevice" )
/** @deprecated  Renamed as PIVideoDevice. */
typedef  PIVideoDevice  VideoDevice;


#ifdef __cplusplus
};
#endif


#endif // __pi_videodevice_HH
