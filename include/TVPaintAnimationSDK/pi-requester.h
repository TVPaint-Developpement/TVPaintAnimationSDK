/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-requester.h
*  @ingroup  sdk_TVPA
*
*  @brief  Everything to make a nice window for your plugin.
*/


#ifndef __pi_requester_HH
#define __pi_requester_HH


#include "pi-basics.h"
#include "pi-filter.h"
#include "pi-pixel.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIRequester  Everything you need to make a nice window for your plugin.

    @note  For historical reasons, a window is called a requester in TVPaint. This dates back to the Amiga times...
*/


//===============================================================================================================


///@name  User Interface / Window.
///@{

/** @brief  The flags that define the look and behavior of a requester.

    @ingroup  sdk_PIRequester
 */
enum  ePIRequesterFlags
{
    /** The requester has the "standard" look (a drag bar and a close button) */
    bPIRequesterFlags_Standard    = 0,
    /** The requester will be modal. */
    bPIRequesterFlags_Modal       = 1<<0,
    /** The requester has no drag bar. */
    bPIRequesterFlags_NoDrag      = 1<<1,
    /** @deprecated  Don't use. */
    bPIRequesterFlags_NoDepth     TVDEPRECATED( "Doesn't make any sense with the new GUI of TVPaint 12" ) = 1<<2,
    /** The requester has no close button. */
    bPIRequesterFlags_NoClose     = 1<<3,
    /** the requester has no "system" buttons. */
    bPIRequesterFlags_NoSysbutton = bPIRequesterFlags_NoClose|/*bPIRequesterFlags_NoDepth|*/bPIRequesterFlags_NoDrag,
    /** The requester will open centered on the screen. */
    bPIRequesterFlags_Center      = 1<<4,
    /** @deprecated  Don't use. */
    bPIRequesterFlags_KeyGrab     TVDEPRECATED( "Doesn't make any sense with the new GUI of TVPaint 12" ) = 1<<5,
    /** The requester will be resizable. */
    bPIRequesterFlags_Resize      = 1<<6,
    /** The requester will be hidden. */
    bPIRequesterFlags_Hidden      = 1<<19,
    /** The requester will have a button to collapse it. */
    bPIRequesterFlags_Collapsable = 1<<20,
    /** The requester is collapsed. */
    bPIRequesterFlags_Collapsed   = 1<<21,
    /** The requester is binable. */
    bPIRequesterFlags_Binable     = 1<<23,
};


/** @brief  The flags that define the (additional) look and behavior of a filter requester .

    @ingroup  sdk_PIRequester
 */
enum  ePIFilterFlags
{
    /** Doesn't add a Path menu entry (for exporting paths). */
    bPIFilterFlags_NoPath     = 1<<1,
    /** Doesn't add the Load/Save entries in the menu. */
    bPIFilterFlags_NoFile     = 1<<2,
    /** Doesn't add the top bar (with the buttons to manage keys). */
    bPIFilterFlags_NoTopBar   = 1<<3,
    /** Add a menubar entry to export the keys as a LightWave compatible file. */
    bPIFilterFlags_Export     = 1<<4,
    /** Replaces the Path menu entry with a Motion entry. */
    bPIFilterFlags_Motion     = 1<<5,
    /** Will call PICBREQ_FILE_LOAD/PICBREQ_FILE_SAVE instead of using the default load/save method. */
    bPIFilterFlags_CustomIO   = 1<<6,
    /** The requester will only be created but not shown. You can show it later by calling TVDisplayReq(). */
    bPIFilterFlags_CreateOnly = 1<<7,
};


/** @brief  The alignment of the text of a button.
    You CAN "OR" one of the left/right with one of the top/bottom.

    @todo //TODO: add the graphical stuff from old doc....

    @ingroup  sdk_PIRequester
*/
enum  ePIButtonTextAlign
{
    bPIButtonTextAlign_Center    = 0,     // centered

    bPIButtonTextAlign_InLeft    = 1<<0,  // inside left
    bPIButtonTextAlign_InRight   = 1<<1,  // inside right
    bPIButtonTextAlign_OutLeft   = 1<<2,  // outside left
    bPIButtonTextAlign_OutRight  = 1<<3,  // outside right

    bPIButtonTextAlign_InTop     = 1<<4,  // inside top
    bPIButtonTextAlign_InBottom  = 1<<5,  // inside bottom
    bPIButtonTextAlign_OutTop    = 1<<6,  // outside top
    bPIButtonTextAlign_OutBottom = 1<<7,  // outside bottom
};


/** @brief  The type of an event handler.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iEvent   The event, @see ePIEvents.
    @param[in]  iReq     The window on which the event happended.
    @param[in]  iArgs    The arguments, specific to the event.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
typedef  int  (*tEventHandler)( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs );


//===============================================================================================================


/** @brief  Opens a window.

    The associated messages will be sent to the global PI_Msg() function.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iWidth   The width of the new window.
    @param[in]  iHeight  The height of the new window.
    @param[in]  iX       The x position of the new window (ignored if PIRF_CENTER_REQ was specified).
    @param[in]  iY       The y position of the new window (ignored if PIRF_CENTER_REQ was specified).
    @param[in]  iFlags   The properties of the window, @see ePIRequesterFlags.
    @param[in]  iTitle   The title of the window, displayed in the dragbar (the Host makes a copy of the string).

    @return  The ID of the window. If ePIRequester_Lock was specified, returns NULL.

    @see  TVCloseReq().

    @ingroup  sdk_PIRequester
*/
INTPTR  TVOpenReq( PIPlugin*  iPlugin, int  iWidth, int  iHeight, int  iX, int  iY, INTPTR  iFlags, const char*  iTitle );


/** @brief  Opens a window, specifying the message handling function.

    @param[in]  iPlugin        This plugin's reference.
    @param[in]  iWidth         The width of the new window.
    @param[in]  iHeight        The height of the new window.
    @param[in]  iX             The x position of the new window (ignored if PIRF_CENTER_REQ was specified).
    @param[in]  iY             The y position of the new window (ignored if PIRF_CENTER_REQ was specified).
    @param[in]  iFlags         The properties of the window, @see ePIRequesterFlags.
    @param[in]  iTitle         The title of the window, displayed in the dragbar (the Host makes a copy of the string).
    @param[in]  iEventHandler  The event handling function.

    @return  The ID of the window. If ePIRequester_Lock was specified, returns NULL.

    @see  TVCloseReq().
*/
INTPTR  TVOpenReqEx( PIPlugin*  iPlugin, int  iWidth, int  iHeight, int  iX, int  iY, INTPTR  iFlags, const char*  iTitle, tEventHandler  iEventHandler );


/** @brief  Opens a window for filters, specifying the message handling function.
    There is at most a single filter window open at any given time.
    This is due to a current limitation of TVPain's timeline which can only display one key set.

    @param[in]  iPlugin        This plugin's reference.
    @param[in]  iWidth         The width of the new window.
    @param[in]  iHeight        The height of the new window.
    @param[in]  iEventHandler  The event handling function.
    @param[in]  iKeys          The set of keyframes.
    @param[in]  iReqFlags      The properties of the window, @see ePIRequesterFlags.
    @param[in]  iMenuFlags     The properties of the bar at the top of the window, @see ePIFilterFlags.

    @return  The ID of the window.

    @see  TVCloseReq().

    @ingroup  sdk_PIRequester
*/
INTPTR  TVOpenFilterReqEx( PIPlugin*  iPlugin, int  iWidth, int  iHeight, tEventHandler  iEventHandler, PIKeys*  iKeys, INTPTR  iReqFlags, INTPTR  iMenuFlags );


/** @brief  Closes a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window to close.

    @see  TVOpenReq(), TVOpenReqEx(), TVOpenFilterReqEx().

    @ingroup  sdk_PIRequester
*/
void  TVCloseReq( PIPlugin*  iPlugin, INTPTR  iReq );


/** @brief  Changes the title of a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iTitle   The new title of the window (the Host makes a copy of the string).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVSetReqTitle( PIPlugin*  iPlugin, INTPTR  iReq, const char*  iTitle );


/** @brief  Moves a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iX       The x position of the window.
    @param[in]  iY       The y position of the window.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVMoveReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY );


/** @brief  Resizes (and moves...) a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iX       The x position of the window.
    @param[in]  iY       The y position of the window.
    @param[in]  iW       The width of the window.
    @param[in]  iH       The height of the window.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVResizeReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH );


/** @brief  Gets a window's position and size.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  oX       The x position of the window.
    @param[in]  oY       The y position of the window.
    @param[in]  oW       The width of the window.
    @param[in]  oH       The height of the window.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVInfoReq( PIPlugin*  iPlugin, INTPTR  iReq, int*  oX, int*  oY, int*  oW, int*  oH );


/** @brief  Moves a window in front of all other windows.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVReqToFront( PIPlugin*  iPlugin, INTPTR  iReq );

/** @brief  Moves a window behind all other windows.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVReqToBack( PIPlugin*  iPlugin, INTPTR  iReq );

/** @brief  Shows or hides the iReq requester.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The requester.
    @param[in]  iShow    0 = Hide, 1 = Show, 2 = Return current state.

    @return  The visible state of the requester.
*/
int  TVDisplayReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iShow );

/** @brief  Saves the state of requester iReq to the user preference file. @todo //CHECK: pref file or project file ?!?

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iReq         The requester the state of which we want to save.
    @param[in]  iSection     The name of the section.
    @param[in]  iSubSection  The name of the subsection.

    @see  TVLoadReqState()
*/
void  TVSaveReqState( PIPlugin*  iPlugin, INTPTR  iReq, const char*  iSection, const char*  iSubSection );

/** @brief  Loads the state of requester iReq from the user preference file. @todo //CHECK: pref file or project file ?!?

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iReq         The requester the state of which we want to load.
    @param[in]  iSection     The name of the section.
    @param[in]  iSubSection  The name of the subsection.

    @see  TVSaveReqState()
*/
void  TVLoadReqState( PIPlugin*  iPlugin, INTPTR  iReq, const char*  iSection, const char*  iSubSection );

///@}


//===============================================================================================================


///@name  User Interface / Buttons.
///@{


/** @brief  The flags that define the look and behavior of a button.
    You can OR them together. Obviously some combinaisons don't make sense...

    @todo //TODO: document all these enumerators !!!!!!!!

    @ingroup  sdk_PIRequester
 */
typedef  enum  ePIButtonFlags
{
    bPIButtonFlags_Normal        = 1<<0,
    bPIButtonFlags_Immediate     = 1<<1,
    bPIButtonFlags_Invert        = 1<<2,
    bPIButtonFlags_Hide          = 1<<3,
    bPIButtonFlags_Quiet         = 1<<4,
    bPIButtonFlags_Right         = 1<<5,
    bPIButtonFlags_Repeat        = 1<<6,
    bPIButtonFlags_Select        = 1<<7,
    bPIButtonFlags_Popup         = 1<<8,
    bPIButtonFlags_Flat          = 1<<9,
    bPIButtonFlags_Text          = 1<<10,
    bPIButtonFlags_TextInteger   = 1<<11,
    bPIButtonFlags_TextActive    = 1<<12,
    bPIButtonFlags_Action        = 1<<13,
    bPIButtonFlags_Drag          = 1<<14,
    bPIButtonFlags_Dialog        = 1<<15,
    bPIButtonFlags_Stat          = 1<<16,
    bPIButtonFlags_TextLock      = 1<<17,
    bPIButtonFlags_Check         = 1<<18,
    bPIButtonFlags_Minislider    = 1<<19,
    bPIButtonFlags_HSeparator    = 1<<20,
    bPIButtonFlags_VSeparator    = 1<<21,
    bPIButtonFlags_Frame         = 1<<22,
} ePIButtonFlags;


/** @brief  Adds a button to a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iX       The upper left corner of the button (relative to the useable part of the window, excluding any border, title bar, etc...)
    @param[in]  iY       The upper left corner of the button (relative to the useable part of the window, excluding any border, title bar, etc...)
    @param[in]  iW       The width of the button.
    @param[in]  iH       The height of the button.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iFlags   Set of flags for the button. @see ePIButtonFlags.
    @param[in]  iText    The text displayed in the button (how it is displayed depends on the kind of button).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVRemoveButtonReq(), TVChangeButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVAddButtonReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, INTPTR  iFlags, const char*  iText );


/** @brief  Removes a button from a window.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @bug  Some refresh problems with check or minislider buttons.

    @see  TVAddButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVRemoveButtonReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iID );


/** @brief  Changes a button in a window.
    Modifies the type (iFlags) and/or the text (iText) of a button.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iFlags   Set of flags for the button. @see ePIButtonFlags.
    @param[in]  iText    The text displayed in the button (how it is displayed depends on the kind of button).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVRemoveButtonReq(), TVChangeButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVChangeButtonReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, INTPTR  iFlags, const char*  iText );


/** @brief  Sets/changes the button help text.
    The help text is displayed in a help 'bubble' when the mouse is hovered above the button.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iReq       The ID of the window.
    @param[in]  iID        The unique (per window) ID of the button.
    @param[in]  iInfoText  The help text.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVSetButtonInfoText( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, const char*  iInfoText );


/** @brief  Modifies the type of a button.
    See TVAddButtonReq() for an explanation of the flags.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iFlags   The new flags.  @see ePIButtonFlags.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIRequester
*/
int  TVChangeButtonFlags( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, INTPTR  iFlags );


/** @brief  Modifies the text and/or the text position of a button.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iText    The text displayed in the button (how it is displayed depends on the kind of button).
    @param[in]  iFlags   The new flags. @see ePIButtonTextAlign.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @bug  Doesn’t work correctly with the button flag bPIButtonFlags_Popup.

    @ingroup  sdk_PIRequester
*/
int  TVChangeButtonName( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, const char*  iText, INTPTR  iFlags );


/** @brief  Gets the current button text.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iReq       The ID of the window.
    @param[in]  iID        The unique (per window) ID of the button.
    @param[in]  oString    A buffer to be filled.
    @param[in]  iMaxChars  The size of the buffer.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVPutButtonString()

    @ingroup  sdk_PIRequester
*/
int  TVGetButtonString( PIPlugin*  iPlugin, INTPTR  iReq,  int  iID, char*  oString, int  iMaxChars );


/** @brief  Sets the button text.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iString  The new string.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVGetButtonString(), TVPutButtonStringUnit()

    @ingroup  sdk_PIRequester
*/
int  TVPutButtonString( PIPlugin*  iPlugin, INTPTR  iReq,  int  iID, const char*  iString );


/** @brief  Sets the button unit text.
    The unit text is display after the button text, and is not changeable by the user if the button allows text editing.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iUnit    The new unit string.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVPutButtonString()

    @ingroup  sdk_PIRequester
*/
int  TVPutButtonStringUnit( PIPlugin*  iPlugin, INTPTR  iReq,  int  iID, const char*  iUnit );


/** @brief  Gets the image of a button.
    If you modify this image, you MUST call TVRefreshButtonImage() for the button to be notified.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iFlags   0 = normal button image, 1 = selected button image.

    @return  The requested image, or NULL if the button doesn't have an image or an error occured.  You are NOT the owner of the returned image !

    @see  TVPutButtonImage(), TVFreePIBlock()

    @ingroup  sdk_PIRequester
*/
PIBlock*  TVGetButtonImage( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, PIFlags  iFlags );


/** @brief  Sets the image of a button.
    The iImage will become owned by the Host and you MUST NOT free it, nor change it.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iImage   The new image.
    @param[in]  iFlags   0 = normal button image, 1 = selected button image.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVGetButtonImage(), TVAllocPIBlock()

    @ingroup  sdk_PIRequester
*/
int  TVPutButtonImage( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, PIBlock*  iImage, PIFlags  iFlags );


/** @brief  Refreshes an area of an image in a button.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iX       The upper left corner of the area to refresh.
    @param[in]  iY       The upper left corner of the area to refresh.
    @param[in]  iW       The width of the area to refresh.
    @param[in]  iH       The height of the area to refresh.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVGetButtonImage(), TVAllocPIBlock()

    @ingroup  sdk_PIRequester
*/
int  TVRefreshButtonImage( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, int  iX, int  iY, int  iW, int  iH );


/** @brief  Direction of a slider. */
enum  ePISliderFlags
{
    ePISliderFlags_Horizontal = (1<<0),
    ePISliderFlags_Vertical   = (1<<1),
};


/** @brief  Creates a slider in a window.

@todo blah about 2D, how to set up to get 1D...

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iX       The upper left corner of the slider.
    @param[in]  iY       The upper left corner of the slider.
    @param[in]  iW       The width of the slider.
    @param[in]  iH       The height of the slider.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iFlags   Set of flags for the slider. Not yet used, pass 0.
    @param[in]  iMaxX    Maximum steps of the slider in the X direction.
    @param[in]  iNumX    Size of the bubble (in steps) in the X direction.
    @param[in]  iPosX    Current value of the slider in the X direction.
    @param[in]  iMaxY    Maximum steps of the slider in the Y direction.
    @param[in]  iNumY    Size of the bubble (in steps) in the Y direction.
    @param[in]  iPosY    Current value of the slider in the Y direction.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVChangeSliderReq(), TVRemoveButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVAddSliderReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, PIFlags  iFlags, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY );


/** @brief  Modifies the parameters of a slider.

 @todo blah about 2D, how to set up to get 1D...

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button.
    @param[in]  iMaxX    Maximum steps of the slider in the X direction.
    @param[in]  iNumX    Size of the bubble (in steps) in the X direction.
    @param[in]  iPosX    Current value of the slider in the X direction.
    @param[in]  iMaxY    Maximum steps of the slider in the Y direction.
    @param[in]  iNumY    Size of the bubble (in steps) in the Y direction.
    @param[in]  iPosY    Current value of the slider in the Y direction.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVAddSliderReq(), TVRemoveButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVChangeSliderReq( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY );


/** @brief  Creates tabs in a window.

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iReq         The ID of the window.
    @param[in]  iX           The upper left corner of the tabs.
    @param[in]  iY           The upper left corner of the tabs.
    @param[in]  iW           The width of the tabs.
    @param[in]  iH           The height of the tabs.
    @param[in]  iID          The unique (per window) ID of the button.
    @param[in]  iTabCount    Number of tabs.
    @param[in]  iCurrentTab  Current tab index.
    @param[in]  iTabNames    Array of tab names.

    If you need many tabs, you may create two (or more...) rows of tabs.
    In this case you can use -1 for iCurrentTab for all but one of the rows to select nothing.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVChangeTabs(), TVRemoveButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVAddTabs( PIPlugin*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, int  iTabCount,  int  iCurrentTab,  const char**  iTabNames );


/** @brief  Modifies the tabs.

 @todo blah about 2D, how to set up to get 1D...

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iReq         The ID of the window.
    @param[in]  iID          The unique (per window) ID of the button.
    @param[in]  iCurrentTab  Current tab index (@see TVAddTabs).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVAddTabs(), TVRemoveButtonReq()

    @ingroup  sdk_PIRequester
*/
int  TVChangeTabs( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, int  iCurrentTab );


///@}


//===============================================================================================================


///@name  User Interface / Drawing.
///@{


/** @brief  Returns the size of a string in pixels.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iText    The text string.
    @param[in]  iCount   The number of charaters in iText to use.

    @return  Length of the string in pixels, or 0 if it fails.

    @ingroup  sdk_PIRequester
*/
int  TVTextLength( PIPlugin*  iPlugin, INTPTR  iReq, const char*  iText, int  iCount );


///@}


//===============================================================================================================


///@name  User Interface / Requester.
///@{

/** @brief  Opens a warning window with a message and an 'OK' button for the user to press to continue.

    @param[in]  iPlugin       This plugin's reference.
    @param[in]  iWarningText  The text to display to the user.

    @ingroup  sdk_PIRequester
*/
void  TVWarning( PIPlugin*  iPlugin, const char*  iWarningText );


/** @brief  Displays a requester with a title (usually a question) and two buttons at the bottom of the requester.
    It waits until one of the buttons is selected.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iMessage  The text to display to the user (usually a question).
    @param[in]  iYes      The text in the "yes" (left) button.
    @param[in]  iNo       The text in the "no" (right) button.

    @return  1 if the "yes" button has been pressed, 0 otherwise.

    @see  TVDemandInt(), TVDemandDouble(), TVDemandText()

    @ingroup  sdk_PIRequester
*/
int  TVDemand( PIPlugin*  iPlugin, const char*  iMessage, const char*  iYes, const char*  iNo );


/** @brief  Displays a requester with a title, a digit input area (with minislider button) and a selection button (OK).
    It enables to enter a number (and only a number).

    @param[in]      iPlugin   This plugin's reference.
    @param[in]      iMessage  The text to display to the user.
    @param[in,out]  ioValue   The default value to display, and also the return value (@todo //CHECK: ?!?!?!).
    @param[in]      iMin      The minimum allowed value.
    @param[in]      iMax      The maximum allowed value.

    @return  Returns the number displayed by the requester when it is closed (@todo //CHECK: ?!?!?!).

    @see  TVDemand(), TVDemandDouble(), TVDemandText()

    @ingroup  sdk_PIRequester
*/
int  TVDemandInt( PIPlugin*  iPlugin, const char*  iMessage, int*  ioValue, int  iMin, int  iMax );


/** @brief  Displays a requester with a title, a digit input area (with minislider button) and a selection button (OK).
    It enables to enter a floating-point number (and only a number).

    @param[in]      iPlugin   This plugin's reference.
    @param[in]      iMessage  The text to display to the user.
    @param[in,out]  ioValue   The default value to display, and also the return value (@todo //CHECK: ?!?!?!).
    @param[in]      iMin      The minimum allowed value.
    @param[in]      iMax      The maximum allowed value.

    @return  Returns the number displayed by the requester when it is closed (@todo //CHECK: ?!?!?!).

    @see  TVDemand(), TVDemandInt(), TVDemandText()

    @ingroup  sdk_PIRequester
*/
int  TVDemandDouble( PIPlugin*  iPlugin, const char*  iMessage, double*  ioValue, double  iMin, double  iMax );


/** @brief  Displays a requester with a title, a text input area and a selection button (OK).
    It enables to enter a string of characters.

    @param[in]      iPlugin     This plugin's reference.
    @param[in]      iMessage    The text to display to the user.
    @param[in,out]  ioText      The default value to display, and also the return value (@todo //CHECK: ?!?!?!).
    @param[in]      iTextCount  The maximum number of characters ioText can hold.

    @return  The number displayed by the requester when it is closed (@todo //CHECK: ?!?!?!).

    @see  TVDemand(), TVDemandInt(), TVDemandDouble()

    @ingroup  sdk_PIRequester
*/
int  TVDemandText( PIPlugin*  iPlugin, const char*  iMessage, char*  ioText, int  iTextCount );

///@}


//===============================================================================================================


///@name  User Interface / Progression.
///@{

/** @brief  Opens the progression window of the filter.
    It will display "in real time" the progression (in %) of the filter.
    Only rarely needed with current hardware's speed...

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iText    The text to display to the user, something like "My ugly filter in progress".

    @see  TVWorkTrace(), TVWorkOff()

    @ingroup  sdk_PIRequester
*/
void  TVWorkOn( PIPlugin*  iPlugin, const char*  iText );


/** @brief  Updates the progress bar displayed by the progression window. It displays "in real time" the progression (in %) of the filter.
    It enables also to know if the user wants to stop the filter (returns 0 or 1).

    @note  This function can be called WITHOUT opening the progression window. In this case, iCurrent and iMax must be set to 0. In this case there is no display of the progression but TVWorkTrace() still enables you to detects a user stop.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iCurrent  Current value of the progression of the filter. The ratio iCurrent/iMax enables TVPaint to display a percentage.
    @param[in]  iMax      Maximum value of the progression of the filter.

    @return  0 if we should continue, 1 if the user wants to stop the filter.

    @see  TVWorkOn(), TVWorkOff()

    @ingroup  sdk_PIRequester
*/
int  TVWorkTrace( PIPlugin*  iPlugin, int  iCurrent, int  iMax );


/** @brief  Closes the progression window of the filter

    @param[in]  iPlugin  This plugin's reference.

    @see  TVWorkOn(), TVWorkTrace()

    @ingroup  sdk_PIRequester
*/
void  TVWorkOff( PIPlugin*  iPlugin );


///@}


//===============================================================================================================


///@name  User Interface / Gadget.
///@{

/** @brief  Opens the compass. This is a gadget used to display an angle in a more user freindly way than a number.
    Use it during the PICBREQ_BUTTON_DOWN on a minislider to open the compass.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The ID of the window.
    @param[in]  iID      The unique (per window) ID of the button this compass is liked to.
    @param[in]  iValue   The value of the angle in degrees.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVCompassChange(), TVCompassClose()

    @ingroup  sdk_PIRequester
*/
int  TVCompassOpen( PIPlugin*  iPlugin, INTPTR  iReq, int  iID, double  iValue );


/** @brief  Updates the compass.
    Use it during the PICBREQ_MINISLIDER to refresh the compass.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iValue   The value of the angle in degrees.

    @see  TVCompassOpen(), TVCompassClose()

    @ingroup  sdk_PIRequester
*/
void  TVCompassChange( PIPlugin*  iPlugin, double  iValue );


/** @brief  Closes the compass.
    Use it during the PICBREQ_BUTTON_UP to close the compass.

    @param[in]  iPlugin  This plugin's reference.

    @see  TVCompassOpen(), TVCompassChange()

    @ingroup  sdk_PIRequester
*/
void  TVCompassClose( PIPlugin*  iPlugin );

///@}


//===============================================================================================================


///@name  Meta/Hud.
///@{


/** @brief  The different shapes the meta function can handle.

    You MUST use one of kPIMeta_* values, and you MAY add one or more of the bPIMeta_* values with it by OR'ing it.

    @ingroup  sdk_PIRequester
 */
enum  ePIMetaFlags
{
    /** @brief  Freehand dot. */
    kPIMeta_Dot              = 0,
    /** @brief  Freehand line. */
    kPIMeta_Draw             = 1,
    /** @brief  Straight line. */
    kPIMeta_Line             = 2,
    /** @brief  Rectangle. */
    kPIMeta_Rectangle        = 3,
    /** @brief  Circle. */
    kPIMeta_Circle           = 4,
    /** @brief  Ellipse. */
    kPIMeta_Ellipse          = 5,
    /** @brief  Spline. */
    kPIMeta_Spline           = 6,
    /** @brief  Preview mode. */
    kPIMeta_Preview          = 7,

    /** @brief  The mask to isolate the function from the additionnal flags. */
    bPIMeta_Function_Mask    =  0xffff,

    /** @brief  Displays the brush. */
    bPIMeta_PreviewBrush     = 1<<16,
    /** @brief  Disable coordinates interpolation, enable a faster answer from function. */
    bPIMeta_NoInterpolation  = 1<<17,
    /** @brief  Sends also PICMETA_PREVIEW messages. */
    bPIMeta_RefreshPreview   = 1<<18,
    /** @brief  Sends also PICMETA_KEYBOARD events for each key press/release. */
    bPIMeta_Keyboard         = 1<<19,
};


/** @brief  Installs a metafunction.

    @todo //BLAH: how does this work ? => explain what msgs will be sent...

    For iFunction, you MUST use one of the kPIMeta_* enumerator, and you CAN 'OR' some of the bPIMeta_* enumerators to it to alter its function.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iFunction  Defines the function to install (@see ePIMetaFlags)

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVCloseFunction()

    @ingroup  sdk_PIRequester
*/
int  TVInstallFunction( PIPlugin*  iPlugin, PIFlags  iFunction );


/** @brief  Closes the current metafunction.

    @param[in]  iPlugin  This plugin's reference.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVInstallFunction()

    @ingroup  sdk_PIRequester
*/
int  TVCloseFunction( PIPlugin*  iPlugin );

///@}


//===============================================================================================================


///@name  Localisation.
///@{

/** @brief  Opens a locale file.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iFileName  Name of the locale file.
    @param[in]  iReserved  Don't use, pass 0.

    @return  An opaque pointer to the file, or NULL if failure to open the file.

    @see  TVGetLocalString(), TVCloseLocalFile()

    @ingroup  sdk_PIRequester
*/
void*  TVOpenLocalFile( PIPlugin*  iPlugin, const char*  iFileName, PIFlags  iReserved );


/** @brief  Gets a string in the locale file.

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iLocaleFile  The locale file, or NULL to get the default text (?!??!?!).
    @param[in]  iLine        Line number in the locale file.

    @return  The localized string. If the line doesn't exists, returns a valid empty string. Returns NULL on failure.

    @see  TVOpenLocalFile(), TVCloseLocalFile()

    @ingroup  sdk_PIRequester
*/
char*  TVGetLocalString( PIPlugin*  iPlugin, void*  iLocaleFile, int  iLine );


/** @brief  Close a locale file.

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iLocaleFile  The locale file.

    @see  TVOpenLocalFile(), TVGetLocalString()

    @ingroup  sdk_PIRequester
*/
void  TVCloseLocalFile( PIPlugin*  iPlugin, void*  iLocaleFile );

///@}


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIRequesterFlags instead. */
enum  TVDEPRECATED( "Use ePIRequesterFlags instead" )  PIRF_DEPRECATED
{
    PIRF_STANDARD_REQ      TVDEPRECATED( "Use bPIRequesterFlags_Standard instead"    ) = bPIRequesterFlags_Standard,
    PIRF_LOCK_REQ          TVDEPRECATED( "Use bPIRequesterFlags_Modal instead"       ) = bPIRequesterFlags_Modal,
    PIRF_NO_DRAG_REQ       TVDEPRECATED( "Use bPIRequesterFlags_NoDrag instead"      ) = bPIRequesterFlags_NoDrag,
    PIRF_NO_DEPTH_REQ      TVDEPRECATED( "Use bPIRequesterFlags_NoDepth instead"     ) = bPIRequesterFlags_NoDepth,
    PIRF_NO_CLOSE_REQ      TVDEPRECATED( "Use bPIRequesterFlags_NoClose instead"     ) = bPIRequesterFlags_NoClose,
    PIRF_NO_SYSBUTTON_REQ  TVDEPRECATED( "Use bPIRequesterFlags_NoSysbutton instead" ) = bPIRequesterFlags_NoSysbutton,
    PIRF_CENTER_REQ        TVDEPRECATED( "Use bPIRequesterFlags_Center instead"      ) = bPIRequesterFlags_Center,
    PIRF_KEYGRAB_REQ       TVDEPRECATED( "Use bPIRequesterFlags_KeyGrab instead"     ) = bPIRequesterFlags_KeyGrab,
    PIRF_RESIZE_REQ        TVDEPRECATED( "Use bPIRequesterFlags_Resize instead"      ) = bPIRequesterFlags_Resize,
    PIRF_HIDDEN_REQ        TVDEPRECATED( "Use bPIRequesterFlags_Hidden instead"      ) = bPIRequesterFlags_Hidden,
    PIRF_COLLAPSABLE_REQ   TVDEPRECATED( "Use bPIRequesterFlags_Collapsable instead" ) = bPIRequesterFlags_Collapsable,
    PIRF_COLLAPSED_REQ     TVDEPRECATED( "Use bPIRequesterFlags_Collapsed instead"   ) = bPIRequesterFlags_Collapsed,
    PIRF_BINABLE_REQ       TVDEPRECATED( "Use bPIRequesterFlags_Binable instead"     ) = bPIRequesterFlags_Binable,
};


/** @deprecated  Use ePIFilterFlags instead. */
enum  TVDEPRECATED( "Use ePIFilterFlags instead" )  FILTERREQ_DEPRECATED
{
    FILTERREQ_NO_PATH      TVDEPRECATED( "Use bPIFilterFlags_NoPath instead"     ) = bPIFilterFlags_NoPath,
    FILTERREQ_NO_FILE      TVDEPRECATED( "Use bPIFilterFlags_NoFile instead"     ) = bPIFilterFlags_NoFile,
    FILTERREQ_NO_TBAR      TVDEPRECATED( "Use bPIFilterFlags_NoTopBar instead"   ) = bPIFilterFlags_NoTopBar,
    FILTERREQ_EXPORT       TVDEPRECATED( "Use bPIFilterFlags_Export instead"     ) = bPIFilterFlags_Export,
    FILTERREQ_MOTION       TVDEPRECATED( "Use bPIFilterFlags_Motion instead"     ) = bPIFilterFlags_Motion,
    FILTERREQ_CUSTOM_IO    TVDEPRECATED( "Use bPIFilterFlags_CustomIO instead"   ) = bPIFilterFlags_CustomIO,
    FILTERREQ_CREATE_ONLY  TVDEPRECATED( "Use bPIFilterFlags_CreateOnly instead" ) = bPIFilterFlags_CreateOnly,
};


/** @deprecated  Use ePIButtonFlags instead. */
enum  TVDEPRECATED( "Use ePIButtonFlags instead" )  PIRBF_BUTTON_DEPRECATED
{
    PIRBF_BUTTON_NORMAL       TVDEPRECATED( "Use bPIButtonFlags_Normal instead"      ) = bPIButtonFlags_Normal,
    PIRBF_BUTTON_IMMEDIATE    TVDEPRECATED( "Use bPIButtonFlags_Immediate instead"   ) = bPIButtonFlags_Immediate,
    PIRBF_BUTTON_INVERT       TVDEPRECATED( "Use bPIButtonFlags_Invert instead"      ) = bPIButtonFlags_Invert,
    PIRBF_BUTTON_HIDE         TVDEPRECATED( "Use bPIButtonFlags_Hide instead"        ) = bPIButtonFlags_Hide,
    PIRBF_BUTTON_QUIET        TVDEPRECATED( "Use bPIButtonFlags_Quiet instead"       ) = bPIButtonFlags_Quiet,
    PIRBF_BUTTON_RIGHT        TVDEPRECATED( "Use bPIButtonFlags_Right instead"       ) = bPIButtonFlags_Right,
    PIRBF_BUTTON_REPEAT       TVDEPRECATED( "Use bPIButtonFlags_Repeat instead"      ) = bPIButtonFlags_Repeat,
    PIRBF_BUTTON_SELECT       TVDEPRECATED( "Use bPIButtonFlags_Select instead"      ) = bPIButtonFlags_Select,
    PIRBF_BUTTON_POPUP        TVDEPRECATED( "Use bPIButtonFlags_Popup instead"       ) = bPIButtonFlags_Popup,
    PIRBF_BUTTON_FLAT         TVDEPRECATED( "Use bPIButtonFlags_Flat instead"        ) = bPIButtonFlags_Flat,
    PIRBF_BUTTON_TEXT         TVDEPRECATED( "Use bPIButtonFlags_Text instead"        ) = bPIButtonFlags_Text,
    PIRBF_BUTTON_TEXT_INT     TVDEPRECATED( "Use bPIButtonFlags_TextInteger instead" ) = bPIButtonFlags_TextInteger,
    PIRBF_BUTTON_TEXT_ACTIVE  TVDEPRECATED( "Use bPIButtonFlags_TextActive instead"  ) = bPIButtonFlags_TextActive,
    PIRBF_BUTTON_ACTION       TVDEPRECATED( "Use bPIButtonFlags_Action instead"      ) = bPIButtonFlags_Action,
    PIRBF_BUTTON_DRAG         TVDEPRECATED( "Use bPIButtonFlags_Drag instead"        ) = bPIButtonFlags_Drag,
    PIRBF_BUTTON_DIALOG       TVDEPRECATED( "Use bPIButtonFlags_Dialog instead"      ) = bPIButtonFlags_Dialog,
    PIRBF_BUTTON_STAT         TVDEPRECATED( "Use bPIButtonFlags_Stat instead"        ) = bPIButtonFlags_Stat,
    PIRBF_BUTTON_TEXT_LOCK    TVDEPRECATED( "Use bPIButtonFlags_TextLock instead"    ) = bPIButtonFlags_TextLock,
    PIRBF_BUTTON_CHECK        TVDEPRECATED( "Use bPIButtonFlags_Check instead"       ) = bPIButtonFlags_Check,
    PIRBF_BUTTON_MINISLIDER   TVDEPRECATED( "Use bPIButtonFlags_Minislider instead"  ) = bPIButtonFlags_Minislider,
    PIRBF_BUTTON_HSEPARATOR   TVDEPRECATED( "Use bPIButtonFlags_HSeparator instead"  ) = bPIButtonFlags_HSeparator,
    PIRBF_BUTTON_VSEPARATOR   TVDEPRECATED( "Use bPIButtonFlags_VSeparator instead"  ) = bPIButtonFlags_VSeparator,
    PIRBF_BUTTON_FRAME        TVDEPRECATED( "Use bPIButtonFlags_Frame instead"       ) = bPIButtonFlags_Frame,
};


/** @deprecated  Use ePIButtonTextAlign instead. */
enum  TVDEPRECATED( "Use ePIButtonTextAlign instead" )  BUTTON_TEXT_DEPRECATED
{
    BUTTON_TEXT_ILEFT    TVDEPRECATED( "Use bPIButtonTextAlign_InLeft instead"    ) = bPIButtonTextAlign_InLeft,
    BUTTON_TEXT_IRIGHT   TVDEPRECATED( "Use bPIButtonTextAlign_InRight instead"   ) = bPIButtonTextAlign_InRight,
    BUTTON_TEXT_OLEFT    TVDEPRECATED( "Use bPIButtonTextAlign_OutLeft instead"   ) = bPIButtonTextAlign_OutLeft,
    BUTTON_TEXT_ORIGHT   TVDEPRECATED( "Use bPIButtonTextAlign_OutRight instead"  ) = bPIButtonTextAlign_OutRight,
    BUTTON_TEXT_ITOP     TVDEPRECATED( "Use bPIButtonTextAlign_InTop instead"     ) = bPIButtonTextAlign_InTop,
    BUTTON_TEXT_IBOTTOM  TVDEPRECATED( "Use bPIButtonTextAlign_InBottom instead"  ) = bPIButtonTextAlign_InBottom,
    BUTTON_TEXT_OTOP     TVDEPRECATED( "Use bPIButtonTextAlign_OutTop instead"    ) = bPIButtonTextAlign_OutTop,
    BUTTON_TEXT_OBOTTOM  TVDEPRECATED( "Use bPIButtonTextAlign_OutBottom instead" ) = bPIButtonTextAlign_OutBottom,
};


/** @deprecated  Use ePIMetaFlags instead. */
enum  TVDEPRECATED( "Use ePIMetaFlags instead" )  PIDRAW_DEPRECATED
{
    PIDRAW_POINT             TVDEPRECATED( "Use kPIMeta_Dot instead"             ) = kPIMeta_Dot,
    PIDRAW_DRAW              TVDEPRECATED( "Use kPIMeta_Draw instead"            ) = kPIMeta_Draw,
    PIDRAW_LINE              TVDEPRECATED( "Use kPIMeta_Line instead"            ) = kPIMeta_Line,
    PIDRAW_RECT              TVDEPRECATED( "Use kPIMeta_Rectangle instead"       ) = kPIMeta_Rectangle,
    PIDRAW_CIRCLE            TVDEPRECATED( "Use kPIMeta_Circle instead"          ) = kPIMeta_Circle,
    PIDRAW_ELLIPSE           TVDEPRECATED( "Use kPIMeta_Ellipse instead"         ) = kPIMeta_Ellipse,
    PIDRAW_SPLINE            TVDEPRECATED( "Use kPIMeta_Spline instead"          ) = kPIMeta_Spline,
    PIDRAW_PREVIEW           TVDEPRECATED( "Use kPIMeta_Preview instead"         ) = kPIMeta_Preview,
    PIDRAW_FONC_MASK         TVDEPRECATED( "Use bPIMeta_Function_Mask instead"   ) = bPIMeta_Function_Mask,
    PIDRAW_PREVIEW_BRUSH     TVDEPRECATED( "Use bPIMeta_PreviewBrush instead"    ) = bPIMeta_PreviewBrush,
    PIDRAW_NO_INTERPOLATION  TVDEPRECATED( "Use bPIMeta_NoInterpolation instead" ) = bPIMeta_NoInterpolation,
    PIDRAW_REFRESH_PREVIEW   TVDEPRECATED( "Use bPIMeta_RefreshPreview instead"  ) = bPIMeta_RefreshPreview,
    PIDRAW_KEYBOARD          TVDEPRECATED( "Use bPIMeta_Keyboard instead"        ) = bPIMeta_Keyboard,
};


/** @deprecated  Use ePISliderFlags instead. */
enum  TVDEPRECATED( "Use ePISliderFlags instead" )  PISLIDER_DEPRECATED
{
    PIRBF_SLIDER_HORIZONTAL  TVDEPRECATED( "Use ePISliderFlags_Horizontal instead" ) = ePISliderFlags_Horizontal,
    PIRBF_SLIDER_VERTICAL    TVDEPRECATED( "Use ePISliderFlags_Vertical instead"   ) = ePISliderFlags_Vertical,
};


/** @deprecated  Use TVOpenFilterReqEx() instead. */
TVDEPRECATED( "Use TVOpenFilterReqEx() instead" )
INTPTR  TVOpenFilterReq( PIFilter*  iPlugin, int  iWidth, int  iHeight, tEventHandler  iEventHandler, PIKeys*  iKeys, INTPTR  iReqFlags );

/** @deprecated  No equivalent. */
enum  TVDEPRECATED( "No equivalent." )  PIGMODE_DEPRECATED
{
    PIGMODE_COLOR   TVDEPRECATED( "No equivalent" ) = (0),
    PIGMODE_TRANS   TVDEPRECATED( "No equivalent" ) = (1<<0),
    PIGMODE_REQBG   TVDEPRECATED( "No equivalent" ) = (1<<1),
    PIGMODE_BUTBG   TVDEPRECATED( "No equivalent" ) = (1<<2),
    PIGMODE_INVERT  TVDEPRECATED( "No equivalent" ) = (1<<3),
};
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
int TVTextReq(PIFilter*,INTPTR,int,int,const char*,INTPTR,PIPixel32,PIPixel32);
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
int TVTextReqClip(PIFilter*,INTPTR,int,int,const char*,INTPTR,PIPixel32,PIPixel32,int,int,int,int);
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
void TVAADrawReq(PIFilter*,INTPTR,double,double,double,double,int,int,INTPTR,PIPixel32,int,int,int,int);
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
int TVRectangleReq(PIFilter*,INTPTR,int,int,int,int,INTPTR,PIPixel32);
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
int TVReadReqData(PIFilter*,PIPixel*,int,int,int,int,int,INTPTR);
/** @deprecated  No equivalent, create a button corresponding to the intended meaning. */
TVDEPRECATED( "No equivalent, create a button corresponding to the intended meaning." )
int TVWriteReqData(PIFilter*,const PIPixel*,int,int,int,int,int,INTPTR);


#ifdef __cplusplus
}
#endif


#endif // __pi_requester_HH
