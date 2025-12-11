/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  SDK-EventHandlingMixIn.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A mix-in for handling events.
 */

#pragma once


#include "Types.h"
#include "PixelCoordinates.h"


#include "pi-pixel.h"  //TODO: //ZAP: !!!


#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2WindowEventHandlingMixIn


class  cTV2WindowEventHandlingMixIn
{
public:
    SDK_NON_COPYABLE( cTV2WindowEventHandlingMixIn );

public:
///@name  Construction/Destruction
///@{
    cTV2WindowEventHandlingMixIn();
///@}

public:
///@name  Event Dispatcher
///@{
    virtual  tErrorCode  HandleWindowEvent( INTPTR  iEvent, INTPTR*  iArgs );
///@}

public:
///@name  Events (to override)
///@{
    /** @brief  The window has been opened.

        @param[in]  iArea  The area used by the window.

        @note  Position is relative to : - TVPaint's window in single-window mode, - the screen in multi-window mode.
     */
    virtual  tErrorCode  EventWindowOpen( const cPixelRectangle&  iArea );

    /** @brief  The window has been closed.

        @param[in]  iArea   The area used by the window.
        @param[in]  iFinal  The window has been closed because TVPaint is shutting down.

        @note  Position is relative to : - TVPaint's window in single-window mode, - the screen in multi-window mode.
     */
    virtual  tErrorCode  EventWindowClose( const cPixelRectangle&  iArea, bool  iFinal );

    /** @brief  The window has been dragged.

        @param[in]  iPosition  The new position of the window.
        @param[in]  iDropped   It has been dropped in place, last event of this "chain" of events.

        @note  Position is relative to : - TVPaint's window in single-window mode, - the screen in multi-window mode.
     */
    virtual  tErrorCode  EventWindowDragged( const cPixelPosition&  iPosition, bool  iDropped );

    /** @brief  The window is being dynamically resized.

        @param[in]  iArea  The new area occupied by the window.

        @note  Position is relative to : - TVPaint's window in single-window mode, - the screen in multi-window mode.
     */
    virtual  tErrorCode  EventWindowResizing( const cPixelRectangle&  iArea );

    /** @brief  The window has been resized.

        @param[in]  iArea  The new area occupied by the window.

        @note  Position is relative to : - TVPaint's window in single-window mode, - the screen in multi-window mode.
     */
    virtual  tErrorCode  EventWindowResized( const cPixelRectangle&  iArea );

    /** @brief  The window has been hidden.
     */
    virtual  tErrorCode  EventWindowHidden();

    /** @brief  The window has been shown.
     */
    virtual  tErrorCode  EventWindowShown();

    /** @brief  The window has been collapsed.
     */
    virtual  tErrorCode  EventWindowCollapsed();

    /** @brief  The window has been uncollapsed.
     */
    virtual  tErrorCode  EventWindowUncollapsed();

    /** @brief  A key has been pressed.

        @param[in]  iCharacter  The character corresponding to the key.
        @param[in]  iTimeMS     A timer in milliseconds.
     */
    virtual  tErrorCode  EventWindowKeyDown( wchar_t  iCharacter, int  iTimeMS );

    /** @brief  A function key has been pressed.

        @param[in]  iKey     The index of the pressed function key (i.e. F1 -> 1, F12 -> 12)
        @param[in]  iTimeMS  A timer in milliseconds.
     */
    virtual  tErrorCode  EventWindowFunctionKeyDown( int  iKey, int  iTimeMS );

    /** @brief  The current project changed in some way (drawing, layer structure, stuff like that...)
     */
    virtual  tErrorCode  EventProjectChanged();

    /** @brief  The current project has been inactivated (most likely another project has been selected...)
     */
    virtual  tErrorCode  EventProjectInactivated();

    /** @brief  The current project has been activated (it is most likely another project than the last one...)
     */
    virtual  tErrorCode  EventProjectActivated();

    /** @brief  The keyset has been reset.
        You should set default values for the keys (which will generate a refresh).
     */
    virtual  tErrorCode  EventKeysetReset();

    /** @brief  The keyset must be loaded.
        You should display a filebrowser and load the selected file.
     */
    virtual  tErrorCode  EventKeysetLoad();

    /** @brief  The keyset must be saved.
        You should save the keys (to the same file as loaded from/previously saved to).
     */
    virtual  tErrorCode  EventKeysetSave();

    /** @brief  The keyset must be saved.
        You should display a filebrowser and save the keys to the selected file.
     */
    virtual  tErrorCode  EventKeysetSaveAs();

    /** @brief  The user moved the mouse.

        @note  This event only occurs when you ask for it with //TODO: ?!?!?!??!

        @param[in]  iPosition     The position of the click, relative to //TODO: ?!?!?!.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iFX           The X position in sub-pixel resolution.
        @param[in]  iFY           The Y position in sub-pixel resolution.
     */
    virtual  tErrorCode  EventCoordinates( const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY );

    /** @brief  Some time elapsed... more or less 20ms (or 50 times per second) but a lot of stuff has priority over this event so don't count too much on that.
        You can use iTimeMS to mesure the time elapsed between two events.

        @note  This event only occurs when you ask for it with //TODO: ?!?!?!??!

        @param[in]  iPosition     The position of the click, relative to //TODO: ?!?!?!.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iFX           The X position in sub-pixel resolution.
        @param[in]  iFY           The Y position in sub-pixel resolution.
     */
    virtual  tErrorCode  EventTicks( const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic );

    /** @brief  The plugin should redraw its HUD if it has any.

        You should only call TV2**HUD**() functions and 'getters' here...  //TODO: name of fcts ?!??!
     */
    virtual  tErrorCode  EventRedrawHud();

    /** @brief  This is called repeatedly after a call to TV2**Pick**Color(), until the mouse button is released.

        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iColor        The picked color.
     */
    virtual  tErrorCode  EventColorPicked( int  iMouseButton, int  iPressure, bool  iCtrlKey, PIPixel32  iColor );

    /** @brief  The user has changed room.

        @param[in]  iRoomName     The name of the new active room.
     */
    virtual  tErrorCode  EventRoomChanged( const std::string&  iRoomName );
///@}
};


//MARK: ========================================================================================================= cTV2ButtonEventHandlingMixIn


class  cTV2ButtonEventHandlingMixIn
{
public:
    SDK_NON_COPYABLE( cTV2ButtonEventHandlingMixIn );

public:
///@name  Construction/Destruction
///@{
    cTV2ButtonEventHandlingMixIn();
///@}

public:
///@name  Event Dispatcher
///@{
    virtual  tErrorCode  HandleButtonEvent( INTPTR  iEvent, INTPTR*  iArgs );
///@}

public:
///@name  Events (to override)
///@{
    /** @brief  The user pressed a mouse button over the given button.

        @param[in]  iButtonID     The ID of the button over which the event occurred.
        @param[in]  iPosition     The position of the click, relative to //TODO: ?!?!?!.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iFX           The X position in sub-pixel resolution.
        @param[in]  iFY           The Y position in sub-pixel resolution.
     */
    virtual  tErrorCode  EventButtonDown( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY );

    /** @brief  The user released a mouse button over the given button.
        This is usually where your plugin takes some action.

        @param[in]  iButtonID     The ID of the button over which the event occurred.
        @param[in]  iPosition     The position of the click, relative to //TODO: ?!?!?!.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iFX           The X position in sub-pixel resolution.
        @param[in]  iFY           The Y position in sub-pixel resolution.
     */
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY );

    /** @brief  The textfield button identified by iButtonID has changed value (most likely the user typed something in).

        @param[in]  iButtonID  The ID of the button that has changed.
     */
    virtual  tErrorCode  EventButtonText( int  iButtonID );

    /** @brief  The user interacted with the given minislider.

        @param[in]  iButtonID  The ID of the minislider.
        @param[in]  iDelta     How much the user moved the mouse (relative to the last event of this kind).
        @param[in]  iTimeMS    A timer in milliseconds.
        @param[in]  iCtrlKey   Is the control key also pressed ?
     */
    virtual  tErrorCode  EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey );

    /** @brief  The user dragged the given slider.

        @param[in]  iButtonID     The ID of the slider.
        @param[in]  iValueX       The new value of the slider in the X direction.
        @param[in]  iValueY       The new value of the slider in the Y direction.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  EventButtonSliderDragged( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey );

    /** @brief  The user 'released' the given slider after dragging it.

        @param[in]  iButtonID     The ID of the slider.
        @param[in]  iValueX       The new value of the slider in the X direction.
        @param[in]  iValueY       The new value of the slider in the Y direction.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  EventButtonSliderReleased( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey );

    /** @brief  The user changed the active tab of a group of tabs.

        @param[in]  iButtonID     The ID of the tabgroup.
        @param[in]  iCurrent      The new active tab index.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  EventButtonTabChanged( int  iButtonID, int  iCurrent, int  iTimeMS, bool  iCtrlKey );

    /** @brief  The user moved the mouse over the given button while a mouse button is pressed.

        @param[in]  iButtonID     The ID of the button over which the event occurred.
        @param[in]  iPosition     The position of the click, relative to //TODO: ?!?!?!.
        @param[in]  iMouseButton  The pressed button (1 = left button, 2 = right button).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iTimeMS       A timer in milliseconds.
        @param[in]  iCtrlKey      Is the control key also pressed ?
        @param[in]  iFX           The X position in sub-pixel resolution.
        @param[in]  iFY           The Y position in sub-pixel resolution.
     */
    virtual  tErrorCode  EventMouseMove( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY );
///@}
};


//MARK: ========================================================================================================= cTV2MetaEventHandlingMixIn


/** A handler for Meta Events.

    Meta Events is the legacy name for the user interaction in the project display window,
    it has nothing to do with meta-programming or the concept of Reflection (in programming, again...).

    @todo  One day we should brainstorm a new name for this...
 */
class  cTV2MetaEventHandlingMixIn
{
public:
    SDK_NON_COPYABLE( cTV2MetaEventHandlingMixIn );

public:
///@name  Construction/Destruction
///@{
    cTV2MetaEventHandlingMixIn();
///@}

public:
///@name  Event Dispatcher
///@{
    virtual  tErrorCode  HandleMetaEvent( INTPTR  iEvent, INTPTR*  iArgs );
///@}

public:
///@name  "Meta" Events (to override)  //TODO: //DOC: !!!!
///@{

    /** @brief  The MetaFunction has been closed (@see TV2InstallFunction()).
     */
    virtual  tErrorCode  MetaEventClosed();

    /** @brief  The MetaFunction FreeHandDot has been called and the user has drawn a point.

        @param[in]  iX            The X coordinate of the point in the current project (from the upper left corner).
        @param[in]  iY            The Y coordinate of the point in the current project (from the upper left corner).
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iPressure     The pressure (from 0 to 255) if a tablet has been used to make the button press, or else 255.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventPoint( double  iX, double  iY, int  iMouseButton, int  iPressure, bool  iCtrlKey );

    /** @brief  The MetaFunction FreeHandLine has been called and the user has drawn a curve.
                This is called over and over until a button release.

        @param[in]  iX            The X coordinate of the point in the current project (from the upper left corner).
        @param[in]  iY            The Y coordinate of the point in the current project (from the upper left corner).
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventFreehand( double  iX, double  iY, int  iMouseButton, int  iPressure, bool  iCtrlKey );

    /** @brief  The MetaFunction Line has been called and the user has drawn a line.

        @param[in]  iX1           The X coordinate of the first point in the current project (from the upper left corner).
        @param[in]  iY1           The Y coordinate of the first point in the current project (from the upper left corner).
        @param[in]  iX2           The X coordinate of the second point in the current project (from the upper left corner).
        @param[in]  iY2           The Y coordinate of the second point in the current project (from the upper left corner).
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iPressure     The pressure if a tablet has been used to make the button press, or else 255.
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventLine( double  iX1, double  iY1, double  iX2, double  iY2, int  iMouseButton, int  iPressure, bool  iCtrlKey );

    /** @brief  The MetaFunction Rectangle has been called and the user has drawn a rectangle.

        @param[in]  iX1           The X coordinate of the first point in the current project (from the upper left corner).
        @param[in]  iY1           The Y coordinate of the first point in the current project (from the upper left corner).
        @param[in]  iX2           The X coordinate of the second point in the current project (from the upper left corner).
        @param[in]  iY2           The Y coordinate of the second point in the current project (from the upper left corner).
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventRectangle( double  iX1, double  iY1, double  iX2, double  iY2, int  iMouseButton, bool  iCtrlKey );

    /** @brief  The MetaFunction Circle has been called and the user has drawn a circle.

        @param[in]  iX            The X coordinate of the center in the current project (from the upper left corner).
        @param[in]  iY            The Y coordinate of the center in the current project (from the upper left corner).
        @param[in]  iRadius       The radius.
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventCircle( double  iX, double  iY, double  iRadius, int  iMouseButton, bool  iCtrlKey );

    /** @brief  The MetaFunction Ellipse has been called and the user has drawn an ellipse.

        @param[in]  iX            The X coordinate of the center in the current project (from the upper left corner).
        @param[in]  iY            The Y coordinate of the center in the current project (from the upper left corner).
        @param[in]  iXRadius      The "radius" along the X axis.
        @param[in]  iYRadius      The "radius" along the Y axis.
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventEllipse( double  iX, double  iY, double  iXRadius, double  iYRadius, int  iMouseButton, bool  iCtrlKey );

    /** @brief  Something aftecting the preview has changed.
        You must redraw your preview.
        @todo //TODO:  called when and should do what ?!??!

        @param[in]  iArea         The area to redraw.
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
        @param[in]  iCtrlKey      Is the control key also pressed ?
     */
    virtual  tErrorCode  MetaEventPreview( const cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey );

    /** @brief  The user pressed a key.

        @param[in]  iCode         The keycode.  @todo //TODO: where is the list ?!??!
        @param[in]  iBuffer       The buffer containing the types characters in UTF8.
        @param[in]  iBufferCount  The length of the buffer.
        @param[in]  iQualifier    The qualifiers of the key (shift,ctrl,etc...).   @todo //TODO: where is the list ?!??!
        @param[in]  iMouseButton  The pressed button (1 = left mouse/main stylus button, 2 = right mouse/secondary stylus button.).
     */
    virtual  tErrorCode  MetaEventKeyboard( int  iCode, const char*  iBuffer, int  iBufferCount, int  iQualifier, int  iMouseButton );
///@}
};


} // namespace  nSDK

