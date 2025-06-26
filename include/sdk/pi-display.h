/** @copyright (c) 2001-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-display.h
*  @ingroup  sdk_TVPA
*
*  @brief  A display device.
*/

#ifndef __pi_display_HH
#define __pi_display_HH


/** @brief  The version of the PIDisplay API. */
#define  PIDISPLAY_VERSION  300


#include "pi-videodevice.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIDisplay  Stuff related to PIDisplay.

    A display device, i.e. some hardware (usually) which can display an image, other that the "normal" screens attached to the computer.
*/


//===============================================================================================================


/** @brief  The drawing modes.

    @ingroup  sdk_PIDisplay
*/
enum  ePIDisplayDrawingMode
{
    kPIDisplayDrawingMode_Copy = 0,
    kPIDisplayDrawingMode_Xor  = 1
};


//===============================================================================================================


/** @brief  A Display Device.

    @ingroup  sdk_PIDisplay
 */
typedef  struct  PIDisplay
{
    /** @brief  The structure version number.
        Set it to PIDISPLAY_VERSION.
     */
    int  Version;
    /** @brief  Name of the Display Device. */
    const char*  DeviceName;
    /** @brief  Width of the Display Device in pixels. */
    int  Width;
    /** @brief  Height of the Display Device in pixels. */
    int  Height;
    /** @brief  Minimum Width of the Display Device in pixels. */
    int  MinWidth;
    /** @brief  Minimum Height of the Display Device in pixels. */
    int  MinHeight;
    /** @brief  Maximum Width of the Display Device in pixels. */
    int  MaxWidth;
    /** @brief  Maximum Height of the Display Device in pixels. */
    int  MaxHeight;
    /** @brief  Aspect ratio of the pixels of the Display Device.
        @code y_bitmap * AspectRatio = y_screen @endcode
     */
    double  AspectRatio;

    /** @brief  Not yet used, MUST be 0. */
    PIUInt32  Flags; // for compatibility with older versions of TVPaint, this isn't a PIFlags...

    /** @brief  Opens a display.
        @todo //CHECK: CB ?!
     */
    int     (*OpenDisplay) ( struct PIDisplay*  iDisplay, int  iWidth, int  iHeight );
    /** @brief  Closes a display.
        @todo //CHECK: CB ?!
     */
    void    (*CloseDisplay)( struct PIDisplay*  iDisplay );
    /** @brief  Writes a PIBlock.
       @todo //CHECK: CB ?!
    */
    int     (*WritePIBlock)( struct PIDisplay*  iDisplay, const PIBlock*  iSource, int  iSrcX, int  iSrcY, int  iDstX, int  iDstY, int  iWidth, int  iHeight, PIUInt32  iDrawMode );
    /** @brief  Writes a single pixel.
       @todo //CHECK: CB ?!
    */
    int     (*WritePixel)  ( struct PIDisplay*  iDisplay, int  iDstX, int  iDstY, PIPixel32  iColor,  PIUInt32  iDrawMode );
    /** @brief  Clears a display.
       @todo //CHECK: CB ?!
    */
    int     (*Clear)       ( struct PIDisplay*  iDisplay, PIPixel32  iColor );
    /** @brief  Draws a filled rectangle.
       @todo //CHECK: CB ?!
    */
    int     (*RectFill)    ( struct PIDisplay*  iDisplay, int  iDstX, int  iDstY, int  iWidth, int  iHeight, PIPixel32  iColor, PIUInt32  iDrawMode );

    // Since AURA2.1: Version 200
    /** @brief  Sends commands to TVPaint.
        If cmd equal "Enable" then the Display Device has to be enable.
        If cmd equal "Disable" then the Display Device has to be disable.
        If cmd equal "KeyOn" then the Display Device has to blend the TVPaint graphic over the Display Device input.
        If cmd equal "KeyOff" then the Display Device has to display only the TVPaint graphic.
       @todo //CHECK: CB ?!
    */
    int     (*Cmd)              ( struct PIDisplay*  iDisplay, const char*  iCmd, char*  oResult );
    /** @brief  Writes a PIVideoBlock.
       @todo //CHECK: CB ?!
    */
    int     (*WriteVideoBlock)  ( struct PIDisplay*  iDisplay, const PIVideoBlock*  iSrc, int  iSrcX, int  iSrcY, int  iDstX, int  iDstY, int  iWidth, int  iHeight );
    /** @brief  Stretches and writes a PIVideoBlock.
       @todo //CHECK: CB ?!
    */
    int     (*StretchVideoBlock)( struct PIDisplay*  iDisplay, const PIVideoBlock*  iSrc, double  iSrcX, double  iSrcY, int  iDstX, int  iDstY, int  iDstW, int  iDstH, double  iRatioX, double  iRatioY );

    // Synchronous output
    /** @brief  Plays a video stream.
       @todo //CHECK: CB ?!
    */
    int     (*OpenOutput)      ( struct PIDisplay*  iDisplay );
    /** @brief  Adds an image to the video stream.
       @todo //CHECK: CB ?!
    */
    double  (*OutputVideoBlock)( struct PIDisplay*  iDisplay, const PIVideoBlock*  iSrc, int  iDstW, int  iDstH );
    /** @brief  Closes a video stream.
       @todo //CHECK: CB ?!
    */
    int     (*CloseOutput)     ( struct PIDisplay*  iDisplay );

    // get the best video mode
    /** @brief  Returns the most suitable video mode.
       @todo //CHECK: CB ?!
    */
    int     (*BestVideoMode)( struct PIDisplay*  iDisplay, PIVideoInfo*  oInfo );

    // Since Mirage1.2: Version 300
    //Enum video mode
    /// @todo //CHECK: CB ?!
    int     (*EnumVideoMode)( struct PIDisplay*  iDisplay, int  iIndex, PIVideoInfo*  oInfo );
    /// @todo //CHECK: CB ?!
    int     (*GetCurrentVideoMode)( struct PIDisplay*  iDisplay );
    /// @todo //CHECK: CB ?!
    void    (*SetCurrentVideoMode)( struct PIDisplay*  iDisplay, int  iIndex );

    /** @brief  Reserved, don't modify ! must be 0 ! */
    PIUInt32  reserved[1017];
} PIDisplay;


//===============================================================================================================


/** @brief  Add a new display device.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iDevice  The new device.
    @param[in]  iFlags   Unused for now, set it to 0
 
    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIDisplay
*/
int  TVAddDisplayDevice( PIPlugin*  iPlugin, PIDisplay*  iDevice, PIFlags  iFlags );


/** @brief  Enumerate the display devices.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iIndex   The index of the device.
    @param[in]  iFlags   Unused for now, set it to 0

    @return  The name of the device at iIndex, or NULL of there is no device at iIndex.

    @note  The buffer returned by this function is owned by TVPaint, not the plugin.
           You MUST NOT free that buffer.

    @ingroup  sdk_PIDisplay
*/
const char*  TVEnumDisplayDevice( PIPlugin*  iPlugin, int  iIndex, PIFlags  iFlags );


/** @brief  Locks a display device.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iName    The name of the device.
    @param[in]  iFlags   Unused for now, set it to 0

    @return  The display device of the given name, or NULL of there is no such device.

    @note  When you lock a display device, it's hidden to the TVEnumDisplayDevice() function, and closed.

    @see  TVUnlockDisplayDevice().

    @ingroup  sdk_PIDisplay
*/
PIDisplay*  TVLockDisplayDevice( PIPlugin*  iPlugin, const char*  iName, PIFlags  iFlags );


/** @brief  Unlocks a display device.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iDevice  The device.

    @note  When you unlock a display device, it is again available to the TVEnumDisplayDevice() function, and opened.

    @see  TVLockDisplayDevice().

    @ingroup  sdk_PIDisplay
*/
void  TVUnlockDisplayDevice( PIPlugin*  iPlugin, PIDisplay*  iDevice );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIDisplayDrawingMode instead. */
enum  TVDEPRECATED( "Use ePIDisplayDrawingMode instead" )  PIDISPLAY_MODE_DEPRECATED
{
    PIDISPLAY_MODE_COPY  TVDEPRECATED( "Use kPIDisplayDrawingMode_Copy instead" ) = kPIDisplayDrawingMode_Copy,
    PIDISPLAY_MODE_EOR   TVDEPRECATED( "Use kPIDisplayDrawingMode_Xor instead"  ) = kPIDisplayDrawingMode_Xor
};


#ifdef __cplusplus
};
#endif


#endif // __pi_display_HH
