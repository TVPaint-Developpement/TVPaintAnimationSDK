/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-filter.h
*  @ingroup  sdk_TVPA
*
*  @brief  The plugin description and the shared library entries.
*/

#ifndef __pi_filter_HH
#define __pi_filter_HH


#include "pi-basics.h"

#if defined(_WIN64)
#include <Windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIPlugin  Stuff related to the plugin itself.

    Stuff affecting the plugin as a whole.
*/


//===============================================================================================================


/** @brief  Flags for a plugin.
    "Or" them together in the FilterFlags field of PIFilter.

    @ingroup  sdk_PIPlugin
*/
typedef  enum  ePIPluginFlags
{
    /** @brief  The plugin will not be visible in the "plugin" menu of TVPaint. */
    kPIPluginFlags_Hidden = (1<<0)
} ePIPluginFlags;


//===============================================================================================================


/** @brief  The structure describing a plugin.
 
    It is passed around to almost every function call, so take care of it, don't loose it, don't break it...

    @ingroup  sdk_PIPlugin
*/
typedef  struct  PIPlugin
{
///@name  Host Info
///@{
    /** @brief  TVPaint plugins interface version number. */
    PIInt32  Version;

    /** @brief  TVPaint plugins interface revision number. */
    PIInt32  Revision;

    /** @brief  TVPaint plugins interface host name. */
    char  ProgName[128];
///@}

///@name  Plugin Info
///@{
    /** @brief  This plugin's version number.
        MUST be set in PI_Open().
     */
    PIInt32  PIVersion;

    /** @brief  This plugin's revision number.
        MUST be set in PI_Open().
     */
    PIInt32  PIRevision;

    /** @brief  This plugin's name.
        MUST be set in PI_Open().
     */
    char  PIName[128];
///@}

///@name  Stuff...
///@{
    /** @brief  Compatibility with old Plugins, DON'T USE !
     */
    void*  UserParametersPtr;

    /** @brief  Compatibility with old Plugins, DON'T USE !
     */
    int  UserParametersSize;

#ifdef _WIN64
    FARPROC  CallBack;
    INTPTR  TVWindow; // Windows HWND
#else
    /** @brief  Private field for the host. Never ever call this directly ! */
    INTPTR  (*CallBack)( struct PIPlugin*, INTPTR, INTPTR* );

    /** @brief  Contains the OS specific pointer to the window.
        @todo //CHECK: Does this still make sense in multi-window mode ?
     */
    void*  TVWindow;
#endif

    /** @brief  Screen width in pixels.
        This doesn't make sense anymore in multi-window mode.
     */
    PIInt32  ScreenWidth;

    /** @brief  Screen height in pixels.
        This doesn't make sense anymore in multi-window mode.
     */
    PIInt32  ScreenHeight;

    /** @brief  Project width in pixel. */
    PIInt32  ImageWidth;

    /** @brief  Project height in pixel. */
    PIInt32  ImageHeight;

    /** @brief  Reserved, don't touch. */
    PIInt32  Reserved0;

    /** @brief  Working area of the filter.
        The same as the working image when you run the filter in TVPaint BUT it can be different when you run the filter from an external program.
        (X1,Y1) upper left corner, (X2,Y2) lower right corner.

        You should never write outside this area. In case of doubt, use TVWriteLayerData(), it will clip safely your writes.
     */
    PIInt32  WorkArea_x1;

    /** @brief  Working area of the filter. */
    PIInt32  WorkArea_y1;

    /** @brief  Working area of the filter. */
    PIInt32  WorkArea_x2;

    /** @brief  Working area of the filter. */
    PIInt32  WorkArea_y2;

    /** @brief  Index of the red component in a pixel components array. */
    char  PlaneOffsetRed;

    /** @brief  Index of the green component in a pixel components array. */
    char  PlaneOffsetGreen;

    /** @brief  Index of the blue component in a pixel components array. */
    char  PlaneOffsetBlue;

    /** @brief  Index of the alpha component in a pixel components array. */
    char  PlaneOffsetAlpha;

    /** @brief  The current image of the current layer in TVPaint.
        This is the image your plugin should modify when being applied.
     */
    PIBlock*  Current;

    /** @brief  A copy of the current image of the current layer in TVPaint.
        This image is strictly READ-ONLY !
     */
    const PIBlock*  Undo;

    /** @brief  Path to the host folder. */
    char*  ProgDir;

    /** @brief  Path to the temporary folder of the host. */
    char*  TempDir;

    /** @brief  The width of the current brush. */
    PIInt32  BrushWidth;

    /** @brief  The height of the current brush. */
    PIInt32  BrushHeight;

    /** @brief  Reserved, don't touch. */
    void*  Reserved1;

    /** @brief  Flags affecting the plugin, MUST be set in PI_Open().
        Currently only PIFILTER_FLAG_HIDDEN is available, if set it will hide the plugin in the "plugin" menu of TVPaint.
     */
    PIInt32  FilterFlags;
///@}
} PIPlugin;


//===============================================================================================================


/** @brief  These are the different events that can be sent from the Host to PI_Msg().

    @note  Some coordinates are provided as fixed-point values.
    To represent the float value as a fixed-point value, multiply it by 65536 to get a 32 bits integer.
    If you want the integer value: @code int i = fix_val >> 16; @endcode 
    If you want the float value : @code float f = fix_val / 65536.0; @endcode

    @note  Beware, the enumerators values aren't in order !! Not that you should ever depend on them, but you've been warned...

    @ingroup  sdk_PIPlugin
*/
enum  ePIEvents
{
// Window events...

    /** @brief  The iReq requester has been opened.
        Arguments (iArgs) are :
        - x, y, w, h:  They are the position and size of the requester.
     */
    kPIEvents_WindowOpen = 100,

    /** @brief  The iReq requester has been closed.
        Arguments (iArgs) are :
        - x, y, w, h:  They are the position and size of the requester.
        - shutdown:  0 = the user closed the requester, 1 = TVPaint is shutting down.
     */
    kPIEvents_WindowClose = 101,

    /** @brief  The iReq requester has been moved.
        Arguments (iArgs) are :
        - x, y:  They are the new position of the requester.
        - dropped: The last event of a series, the window has been dropped here.
     */
    kPIEvents_WindowDragged = 102,

    /** @brief  The iReq requester is being dynamically resized.
        Arguments (iArgs) are :
        - w, h, x, y:  They are the new size and position of the requester (beware the order !!!).
     */
    kPIEvents_WindowResizing = 105,

    /** @brief  The iReq requester has been resized.
        Arguments (iArgs) are :
        - w, h, x, y:  They are the new size and position of the requester (beware the order !!!).
     */
    kPIEvents_WindowResized = 107,

    /** @brief  The window has been hidden.
        No arguments (iArgs).
     */
    kPIEvents_WindowHidden = 113,

    /** @brief  The window has been shown.
        No arguments (iArgs).
     */
    kPIEvents_WindowShown = 114,

    /** @brief  The window has been collapsed.
        No arguments (iArgs).
     */

    kPIEvents_WindowCollapsed = 115,
    /** @brief  The window has been uncollapsed.
        No arguments (iArgs).
     */
    kPIEvents_WindowUncollapsed = 116,

    /** @brief  A key has been pressed.
        Arguments (iArgs) are :
        - char:  The character corresponding to the key.
        - timer: A timer in milliseconds.
     */
    kPIEvents_WindowKeyDown = 103,

    /** @brief  A function key has been pressed.
        Arguments (iArgs) are :
        - fkey:  The index of the pressed function key (i.e. F1 -> 1, F12 -> 12).
        - timer: A timer in milliseconds.
     */
    kPIEvents_WindowFunctionKeyDown = 104,

    /** @brief  Current project has changed (after a stroke, filters apply, ...).
        No arguments (iArgs).
     */
    kPIEvents_ProjectChanged = 110,

    /** @brief  The current project has been inactivated (most likely another project has been selected...)
        No arguments (iArgs).
     */
    kPIEvents_ProjectInactivated = 111,

    /** @brief  The current project has been activated (it is most likely another project than the last one...)
        No arguments (iArgs).
     */
    kPIEvents_ProjectActivated = 112,

    /** @brief  Reset plugin has been called.
        The plugin keys have been reset.
        You should set default keys values here.
        No arguments (iArgs).
     */
    kPIEvents_KeysetReset = 120,

    /** @brief  Load plugin has been called.
        The plugin parameters have to be loaded, you should update anything that depends on them.
        No arguments (iArgs).
     */
    kPIEvents_KeysetLoad = 121,

    /** @brief  Save plugin has been called.
        The plugin parameters have to be saved.
        No arguments (iArgs).
     */
    kPIEvents_KeysetSave = 122,

    /** @brief  Save as... plugin has been called.
        The plugin parameters have to be saved.
        No arguments (iArgs).
     */
    kPIEvents_KeysetSaveAs = 123,

    /** @brief  Sent for each mouse/stylus movement.
        Arguments (iArgs) are :
        - fpx, fpy :  The position in the current project (from the upper left corner). In fixed-point.
        - fsx, fsy :  The position in the TVPaint window (from the upper left corner). In fixed-point. @deprecated DOESN'T MAKE ANY SENSE IN MULTI-WINDOW MODE.
        - pressure :  The pressure if using a stylus (0-255).
        - timer :  A timer in thousands of a second.
        - ctrlkey :  1 if the ctrl key is pressed.
        - rx, ry :  The position in the requester (@todo //TODO: which one ?!?!) (from the upper left corner).
     */
    kPIEvents_Coordinates = 210,

    /** @brief  The is called regularly, more or less 60 times a second (but the timing is very unreliable).
        Arguments (iArgs) are the same as those to PICBREQ_COORD, with the addition to this one at the very end :
        - reduced :  1 if TVPaint is iconified.
     */
    kPIEvents_Ticks = 211,

    /** @brief  The plugin should redraw its HUD if it has any.
        You should only call TVHud**() functions and 'getters' here...
        No arguments (iArgs).
     */
    kPIEvents_RedrawHud = 300,

    /** @brief  This is called repeatedly after a call to TV2**Pick**Color(), until the mouse button is released.
        Arguments (iArgs) are :
        - x, y :  The mouse position.
        - pressure :  The pressure if using a stylus (0-255).
        - ctrlkey :  1 if the ctrl key is pressed.
        - color :  The picked color.
     */
    kPIEvents_ColorPicked = 310,

    /** @brief  The user has changed room.
        Arguments (iArgs) are :
        - name :  The name of the new active room.
     */
    kPIEvents_RoomChanged = 320,



// Button events

    /** @brief  The button has been pressed (but not yet released).
        Arguments (iArgs) are :
        - id:  The button's ID.
        - x, y :  The coordinate of the click relative to the button.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - pressure :  The pressure if using a stylus (0-255).
        - timer :  A timer in thousands of a second.
        - ctrlkey :  1 if the ctrl key is pressed.
        - fx, fy :  Same as x, y but in fixed-point.
     */
    kPIEvents_ButtonDown = 200,

    /** @brief  The button has been released.
        Arguments (iArgs) are exactly the same as those to PICBREQ_BUTTON_DOWN, with the following addition :
        - mousebutton : 4 = [ENTER] key.
     */
    kPIEvents_ButtonUp = 201,

    /** @brief  The text string of the button has changed.
        Arguments (iArgs) are :
        - id:  The button's ID.
     */
    kPIEvents_ButtonText = 106,

    /** @brief  The minislider button has been dragged.
        Arguments (iArgs) are :
        - id:  The button's ID.
        - x, y :  The mouse/stylus position in the button (@todo //TODO: really? I suppose it's the DELTA OF THE VALUE of the slider).
        - timer :  A timer in thousands of a second.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_ButtonMiniSlider = 212,

    /** @brief  The slider button has been moved.
        Arguments (iArgs) are :
        - id:  The button's ID.
        - delta :  The distance betwen the current mouse position and the previou one.
        - timer :  A timer in thousands of a second.
        - ctrlkey :  1 if the ctrl key is pressed.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
     */
    kPIEvents_ButtonSliderDragged = 213,

    /** @brief  The slider button has been released.
        Arguments (iArgs) are exactly the same as those to PICBREQ_SLIDER_MOVE.
     */
    kPIEvents_ButtonSliderReleased = 214,

    /** @brief  The current tab has changed.
        Arguments (iArgs) are :
        - id:  The button's ID.
        - tab :  The new selected tab index.
        - timer :  A timer in thousands of a second.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_ButtonTabChanged = 215,

    /** @brief  The mouse/stylus has been moved.
        Arguments (iArgs) are exactly the same as those to PICBREQ_BUTTON_DOWN.
     */
    kPIEvents_MouseMove = 202,



// meta function messages

    /** @brief  The MetaFunction has been closed (@see TVInstallFunction()).
        No arguments (iArgs).
     */
    kPIEvents_MetaClosed = 1000,

    /** @brief  The MetaFunction FreeHandDot has been called.
        Arguments (iArgs) are :
        - fpx, fpy :  The position in the current project (from the upper left corner). In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - unused : well... unused.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaPoint = 1001,

    /** @brief  The MetaFunction FreeHandLine has been called.
        This is called over and over until a button release.
        Arguments (iArgs) are :
        - fpx, fpy :  The position in the current project (from the upper left corner). In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - pressure :  The pressure if using a stylus (0-255).
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaFreehand = 1002,

    /** @brief  The MetaFunction Line has been called.
        Arguments (iArgs) are :
        - fpx1, fpy1 :  The start position in the current project (from the upper left corner). In fixed-point.
        - fpx2, fpy2 :  The end position in the current project (from the upper left corner). In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - pressure :  The pressure if using a stylus (0-255).  @todo //CHECK: at which moment ?!
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaLine = 1003,

    /** @brief  The MetaFunction Rectangle has been called.
        Arguments (iArgs) are :
        - fpx1, fpy1 :  The start position in the current project (from the upper left corner). In fixed-point.
        - fpx2, fpy2 :  The end position in the current project (from the upper left corner). In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaRectangle = 1004,

    /** @brief  The MetaFunction Circle has been called.
        Arguments (iArgs) are :
        - fpx, fpy :  The center of the circle in the current project (from the upper left corner). In fixed-point.
        - fradius :  The radius of the circle. In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaCircle = 1005,

    /** @brief  The MetaFunction Ellipse has been called.
        Arguments (iArgs) are :
        - fpx, fpy :  The center of the elipse in the current project (from the upper left corner). In fixed-point.
        - fa, fb :  The two "radiuses" of the ellipse. In fixed-point.
        - mousebutton :  1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.
        - ctrlkey :  1 if the ctrl key is pressed.
     */
    kPIEvents_MetaEllipse = 1006,

    /** @brief  Something aftecting the preview has changed.
        You must redraw your preview.
        Arguments (iArgs) are :
        - x, y, w, h:  They are the position and size of the area to redraw, in the current project coordinates (from the upper left corner)
     */
    kPIEvents_MetaPreview = 1007,

    /** @brief  The user pressed a key.
        Arguments (iArgs) are :
        - iCode :  The keycode.  @todo //TODO: where is the list ?!??!
        - iBuffer :  The buffer containing the types characters in UTF8.
        - iBufferCount :  The length of the buffer.
        - iQualifier :  The qualifiers of the key (shift,ctrl,etc...).   @todo //TODO: where is the list ?!??!
        - iMouseButton :  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
     */
    kPIEvents_MetaKeyboard = 1008
};


//===============================================================================================================


/** @brief  Initializes the plugin itself.
    It is the function called FIRST by the host to let the filter initialize its parameters.
    Plugin must set PiFilter->PIName, PiFilter->PIVersion and PiFilter->PIRevision.

    @param[in]  iPlugin  This plugin's reference.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_Open( PIPlugin*  iPlugin );


/** @brief  Displays miscellaneous informations (Plugin author's, version, revision, ...).
    In the Plugin window's or in a new window that the filter opens,
    or in a warning window (with TVWarning() function, etc).
    It can be called (or not...) when selecting a special About button.

    @param[in]  iPlugin  This plugin's reference.

    @see  calling_sequence
    @ingroup  sdk_PIPlugin
*/
void  FAR PASCAL  PI_About( PIPlugin*  iPlugin );


/** @brief  Initializes the settings of the parameters.
    It handles the string iArgs which can be sent by an external program,
    OR it opens the user interface TVOpenReq() or TVOpenFilterReqEx().

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iArgs    Optional argument string.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_Parameters( PIPlugin*  iPlugin, const char*  iArgs );


/** @brief  Handles messages.
    receives all PICMETA messages, and PICBREQ messages for windows without a specific message processing function.
    This function (and window specific message processing functions) is the 'heart' of the plugin.

    iArgs is specific to the event, see the description of each event for details.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iEvent   The event, @see ePIEvents.
    @param[in]  iReq     The window on which the event happended.
    @param[in]  iArgs    The arguments, specific to the event.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs );


/** @brief  Close the plugin completly.
    Called when TVPaint shuts down.

    @param[in]  iPlugin  This plugin's reference.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
void  FAR PASCAL  PI_Close( PIPlugin*  iPlugin );


/** @brief  This is the first function called by TVExecute().

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iCount   The number of images in the selected image sequence.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_SequenceStart( PIPlugin*  iPlugin, int  iCount );


/** @brief  This is called before processing each frame in the sequence.

    iPos and iSize have to be ignored for filters on still frame images.
    iPos and iSize are parameters designed to control animations.
    An animation will always have n images: iPos 0.0 is the first images and iPos 1.0 is the last.
    Host will transmit to the filter the position (iPos) in the animation of the image to render and the size of this image.
    If you have 100 images and you want to render the image 50, Host will transmit iPos=0.5 (1.*50/100.)
    and iSize=0.01 (1 real image is equal to 0.01 "reference images")   (Number of images to render= 1./iSize).

    If you understood this gibberish, please tell me :)   @todo //TODO: some better explanation !!!!!!

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iPos     The position of the image to render.
    @param[in]  iSize    The "size" of the image to render.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_Start( PIPlugin*  iPlugin, double  iPos, double  iSize );


/** @brief  Does the work to render the current image.

    This is the only function where you are allowed to modify iPlugin->Current.

    @param[in]  iPlugin  This plugin's reference.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
int  FAR PASCAL  PI_Work( PIPlugin*  iPlugin );


/** @brief  This is called after the processing of each frame in the sequence.

    @param[in]  iPlugin  This plugin's reference.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
void  FAR PASCAL  PI_Finish( PIPlugin*  iPlugin );


/** @brief  This is called after the processing of the whole sequence.

    @param[in]  iPlugin  This plugin's reference.

    @see  calling_sequence

    @ingroup  sdk_PIPlugin
*/
void  FAR PASCAL  PI_SequenceFinish( PIPlugin*  iPlugin );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIEvents instead. */
enum  TVDEPRECATED( "Use ePIEvents instead" )  PIEVENT_DEPRECATED
{
    PICBREQ_OPEN               TVDEPRECATED( "Use kPIEvents_WindowOpen instead" )            = kPIEvents_WindowOpen,
    PICBREQ_CLOSE              TVDEPRECATED( "Use kPIEvents_WindowClose instead" )           = kPIEvents_WindowClose,
    PICBREQ_DRAG               TVDEPRECATED( "Use kPIEvents_WindowDragged instead" )         = kPIEvents_WindowDragged,
    PICBREQ_RESIZE             TVDEPRECATED( "Use kPIEvents_WindowResizing instead" )        = kPIEvents_WindowResizing,
    PICBREQ_RESIZED            TVDEPRECATED( "Use kPIEvents_WindowResized instead" )         = kPIEvents_WindowResized,
    PICBREQ_HIDE               TVDEPRECATED( "Use kPIEvents_WindowHidden instead" )          = kPIEvents_WindowHidden,
    PICBREQ_SHOW               TVDEPRECATED( "Use kPIEvents_WindowShown instead" )           = kPIEvents_WindowShown,
    PICBREQ_COLLAPSE           TVDEPRECATED( "Use kPIEvents_WindowCollapsed instead" )       = kPIEvents_WindowCollapsed,
    PICBREQ_UNCOLLAPSE         TVDEPRECATED( "Use kPIEvents_WindowUncollapsed instead" )     = kPIEvents_WindowUncollapsed,
    PICBREQ_KEY_DOWN           TVDEPRECATED( "Use kPIEvents_WindowKeyDown instead" )         = kPIEvents_WindowKeyDown,
    PICBREQ_FKEY_DOWN          TVDEPRECATED( "Use kPIEvents_WindowFunctionKeyDown instead" ) = kPIEvents_WindowFunctionKeyDown,
    PICBREQ_PROJECT_CHANGE     TVDEPRECATED( "Use kPIEvents_ProjectChanged instead" )        = kPIEvents_ProjectChanged,
    PICBREQ_PROJECT_INACTIVATE TVDEPRECATED( "Use kPIEvents_ProjectInactivated instead" )    = kPIEvents_ProjectInactivated,
    PICBREQ_PROJECT_ACTIVATE   TVDEPRECATED( "Use kPIEvents_ProjectActivated instead" )      = kPIEvents_ProjectActivated,
    PICBREQ_FILE_RESET         TVDEPRECATED( "Use kPIEvents_KeysetReset instead" )           = kPIEvents_KeysetReset,
    PICBREQ_FILE_LOAD          TVDEPRECATED( "Use kPIEvents_KeysetLoad instead" )            = kPIEvents_KeysetLoad,
    PICBREQ_FILE_SAVE          TVDEPRECATED( "Use kPIEvents_KeysetSave instead" )            = kPIEvents_KeysetSave,
    PICBREQ_FILE_SAVE_AS       TVDEPRECATED( "Use kPIEvents_KeysetSaveAs instead" )          = kPIEvents_KeysetSaveAs,    
    PICBREQ_COORD              TVDEPRECATED( "Use kPIEvents_Coordinates instead" )           = kPIEvents_Coordinates,
    PICBREQ_TICKS              TVDEPRECATED( "Use kPIEvents_Ticks instead" )                 = kPIEvents_Ticks,
    PICB_HUD_REDRAW            TVDEPRECATED( "Use kPIEvents_RedrawHud instead" )             = kPIEvents_RedrawHud,
    PICB_PICK_COLOR            TVDEPRECATED( "Use kPIEvents_ColorPicked instead" )           = kPIEvents_ColorPicked,
    PICBREQ_ROOM_CHANGE        TVDEPRECATED( "Use kPIEvents_RoomChanged instead" )           = kPIEvents_RoomChanged,

    PICBREQ_BUTTON_DOWN        TVDEPRECATED( "Use kPIEvents_ButtonDown instead" )            = kPIEvents_ButtonDown,
    PICBREQ_BUTTON_UP          TVDEPRECATED( "Use kPIEvents_ButtonUp instead" )              = kPIEvents_ButtonUp,
    PICBREQ_BUTTON_TEXT        TVDEPRECATED( "Use kPIEvents_ButtonText instead" )            = kPIEvents_ButtonText,
    PICBREQ_MINISLIDER         TVDEPRECATED( "Use kPIEvents_ButtonMiniSlider instead" )      = kPIEvents_ButtonMiniSlider,
    PICBREQ_SLIDER_MOVE        TVDEPRECATED( "Use kPIEvents_ButtonSliderDragged instead" )   = kPIEvents_ButtonSliderDragged,
    PICBREQ_SLIDER_RELEASE     TVDEPRECATED( "Use kPIEvents_ButtonSliderReleased instead" )  = kPIEvents_ButtonSliderReleased,
    PICBREQ_TABS_CHANGE        TVDEPRECATED( "Use kPIEvents_ButtonTabChanged instead" )      = kPIEvents_ButtonTabChanged,
    PICBREQ_MOVE               TVDEPRECATED( "Use kPIEvents_MouseMove instead" )             = kPIEvents_MouseMove,

    PICMETA_CLOSE              TVDEPRECATED( "Use kPIEvents_MetaClosed instead" )            = kPIEvents_MetaClosed,
    PICMETA_POINT              TVDEPRECATED( "Use kPIEvents_MetaPoint instead" )             = kPIEvents_MetaPoint,
    PICMETA_DRAW               TVDEPRECATED( "Use kPIEvents_MetaFreehand instead" )          = kPIEvents_MetaFreehand,
    PICMETA_LINE               TVDEPRECATED( "Use kPIEvents_MetaLine instead" )              = kPIEvents_MetaLine,
    PICMETA_RECT               TVDEPRECATED( "Use kPIEvents_MetaRectangle instead" )         = kPIEvents_MetaRectangle,
    PICMETA_CIRCLE             TVDEPRECATED( "Use kPIEvents_MetaCircle instead" )            = kPIEvents_MetaCircle,
    PICMETA_ELLIPSE            TVDEPRECATED( "Use kPIEvents_MetaEllipse instead" )           = kPIEvents_MetaEllipse,
    PICMETA_PREVIEW            TVDEPRECATED( "Use kPIEvents_MetaPreview instead" )           = kPIEvents_MetaPreview,
    PICMETA_KEYBOARD           TVDEPRECATED( "Use kPIEvents_MetaKeyboard instead" )          = kPIEvents_MetaKeyboard,

    // deprecated, no modern equivalent...
    PICBREQ_VIDEO_CHANGE_PREV  TVDEPRECATED( "No equivalent, too bad..." )                   = 220,
    PICBREQ_VIDEO_CHANGE       TVDEPRECATED( "No equivalent, too bad..." )                   = 221,
};


/** @deprecated  Use PIPlugin instead. */
TVDEPRECATED("Use PIPlugin instead.")  typedef  PIPlugin  PIFilter;


/** @deprecated  Use ePIHudHandle instead. */
enum  TVDEPRECATED( "Use ePIHudHandle instead" )  PIFILTER_FLAG_DEPRECATED
{
    PIFILTER_FLAG_HIDDEN  TVDEPRECATED( "Use kPIPluginFlags_Hidden instead" ) = kPIPluginFlags_Hidden
};


#ifdef __cplusplus
}
#endif


#endif // __pi_filter
