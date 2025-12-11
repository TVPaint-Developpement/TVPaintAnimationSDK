/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  EventHandlingMixIn.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A mix-in for handling events.
 */


#include "TVPaintAnimationSDK/EventHandlingMixIn.hpp"

#include "TVPaintAnimationSDK/Keys.hpp"
#include "TVPaintAnimationSDK/Plugin.hpp"


namespace  nSDK {


//MARK: ========================================================================================================= cTV2WindowEventHandlingMixIn


//-------------------------------------------------------------------------------------- Construction/Destruction


cTV2WindowEventHandlingMixIn::cTV2WindowEventHandlingMixIn()
{
}


//---------------------------------------------------------------------------------------------- Event Dispatcher


tErrorCode
cTV2WindowEventHandlingMixIn::HandleWindowEvent( INTPTR  iEvent, INTPTR*  iArgs )
{
printf("cTV2WindowEventHandlingMixIn::HandleWindowEvent()\n");//DEBUG:
    switch( iEvent )
    {
        case  kPIEvents_WindowOpen:
            return  EventWindowOpen( {int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[3]), cPixelRectangle::eXYWH::kXYWH} );

        case  kPIEvents_WindowClose:
            return  EventWindowClose( {int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[3]), cPixelRectangle::eXYWH::kXYWH}, !!iArgs[4] );

        case  kPIEvents_WindowDragged:
            return  EventWindowDragged( {int(iArgs[0]), int(iArgs[1])}, !!iArgs[2] );

        case  kPIEvents_WindowResizing:
            return  EventWindowResizing( {int(iArgs[2]), int(iArgs[3]), int(iArgs[0]), int(iArgs[1]), cPixelRectangle::eXYWH::kXYWH} );

        case  kPIEvents_WindowResized:
            return  EventWindowResized( {int(iArgs[2]), int(iArgs[3]), int(iArgs[0]), int(iArgs[1]), cPixelRectangle::eXYWH::kXYWH} );

        case  kPIEvents_WindowHidden:
            return  EventWindowHidden();

        case  kPIEvents_WindowShown:
            return  EventWindowShown();

        case  kPIEvents_WindowCollapsed:
            return  EventWindowCollapsed();

        case  kPIEvents_WindowUncollapsed:
            return  EventWindowUncollapsed();

        case  kPIEvents_WindowKeyDown:
            return  EventWindowKeyDown( int(iArgs[0]), int(iArgs[1]) );

        case  kPIEvents_WindowFunctionKeyDown:
            return  EventWindowFunctionKeyDown( int(iArgs[0]), int(iArgs[1]) );

        case  kPIEvents_ProjectChanged:
            return  EventProjectChanged();

        case  kPIEvents_ProjectInactivated:
            return  EventProjectInactivated();

        case  kPIEvents_ProjectActivated:
            return  EventProjectActivated();

        case  kPIEvents_KeysetReset:
            return  EventKeysetReset();

        case  kPIEvents_KeysetLoad:
            return  EventKeysetLoad();

        case  kPIEvents_KeysetSave:
            return  EventKeysetSave();

        case  kPIEvents_KeysetSaveAs:
            return  EventKeysetSaveAs();

        case  kPIEvents_Coordinates:
            return  EventCoordinates( {int(iArgs[1])/65536, int(iArgs[2])/65536}, int(iArgs[0]), int(iArgs[5]), int(iArgs[6]), !!iArgs[7], int(iArgs[1])/65536., int(iArgs[2])/65536. );

        case  kPIEvents_Ticks:
            return  EventTicks( {int(iArgs[1])/65536, int(iArgs[2])/65536}, int(iArgs[0]), int(iArgs[5]), int(iArgs[6]), !!iArgs[7], int(iArgs[1])/65536., int(iArgs[2])/65536., !!iArgs[10] );

        case  kPIEvents_RedrawHud:
            return  EventRedrawHud();

        case  kPIEvents_ColorPicked:
            // we don't care about the position as it doesn't make any sense anymore...
            return  EventColorPicked( int(iArgs[2]), int(iArgs[3]), !!iArgs[4], PIPixel32(iArgs[5]) );

        case  kPIEvents_RoomChanged:
        {
            std::string  roomname( (const char*)(iArgs[0]) );
            return  EventRoomChanged( roomname );
        }

#if 0 // these two are deprecated...
            PICBREQ_VIDEO_CHANGE_PREV
            PICBREQ_VIDEO_CHANGE
#endif

    }
    return  kErrorCode_NoError;
}


//-------------------------------------------------------------------------------------------------------- Events


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowOpen( const cPixelRectangle&  iArea )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowClose( const cPixelRectangle&  iArea, bool  iFinal )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowDragged( const cPixelPosition&  iPosition, bool  iDropped )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowResizing( const cPixelRectangle&  iArea )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowResized( const cPixelRectangle&  iArea )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowKeyDown( wchar_t  iCharacter, int  iTimeMS )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowFunctionKeyDown( int  iKey, int  iTimeMS )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowHidden()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowShown()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowCollapsed()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventWindowUncollapsed()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventProjectChanged()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventProjectInactivated()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventProjectActivated()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventKeysetReset()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventKeysetLoad()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventKeysetSave()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventKeysetSaveAs()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventCoordinates( const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventTicks( const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventRedrawHud()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventColorPicked( int  iMouseButton, int  iPressure, bool  iCtrlKey, PIPixel32  iColor )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2WindowEventHandlingMixIn::EventRoomChanged( const std::string&  iRoomName )
{
    return  kErrorCode_NoError;
}


//MARK: ========================================================================================================= cTV2ButtonEventHandlingMixIn


//-------------------------------------------------------------------------------------- Construction/Destruction


cTV2ButtonEventHandlingMixIn::cTV2ButtonEventHandlingMixIn()
{
}


//---------------------------------------------------------------------------------------------- Event Dispatcher


tErrorCode
cTV2ButtonEventHandlingMixIn::HandleButtonEvent( INTPTR  iEvent, INTPTR*  iArgs )
{
    switch( iEvent )
    {
        case  kPIEvents_ButtonDown:
            return  EventButtonDown( int(iArgs[0]), {int(iArgs[1]), int(iArgs[2])}, int(iArgs[3]), int(iArgs[4]), int(iArgs[5]), !!iArgs[6], int(iArgs[7])/65536., int(iArgs[8])/65536. );

        case  kPIEvents_ButtonUp:
            return  EventButtonUp( int(iArgs[0]), {int(iArgs[1]), int(iArgs[2])}, int(iArgs[3]), int(iArgs[4]), int(iArgs[5]), !!iArgs[6], int(iArgs[7])/65536., int(iArgs[8])/65536. );

        case  kPIEvents_ButtonText:
            return  EventButtonText( int(iArgs[0]) );

        case  kPIEvents_ButtonMiniSlider:
            return  EventButtonMiniSlider( int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), !!iArgs[3] );

        case  kPIEvents_ButtonSliderDragged:
            return  EventButtonSliderDragged( int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[5]), int(iArgs[3]), !!iArgs[4] );

        case  kPIEvents_ButtonSliderReleased:
            return  EventButtonSliderReleased( int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[5]), int(iArgs[3]), !!iArgs[4] );

        case  kPIEvents_ButtonTabChanged:
            return  EventButtonTabChanged( int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[3]) );

        case  kPIEvents_MouseMove:
            return  EventMouseMove( int(iArgs[0]), {int(iArgs[1]), int(iArgs[2])}, int(iArgs[3]), int(iArgs[4]), int(iArgs[5]), !!iArgs[6], int(iArgs[7])/65536., int(iArgs[8])/65536. );
    }
    return  kErrorCode_NoError;
}


//-------------------------------------------------------------------------------------------------------- Events


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonDown( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonUp( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonText( int  iButtonID )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonSliderDragged( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonSliderReleased( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventButtonTabChanged( int  iButtonID, int  iCurrent, int  iTimeMS, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonEventHandlingMixIn::EventMouseMove( int  iButtonID, const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    return  kErrorCode_NoError;
}


//MARK: ========================================================================================================= cTV2MetaEventHandlingMixIn


//-------------------------------------------------------------------------------------- Construction/Destruction


cTV2MetaEventHandlingMixIn::cTV2MetaEventHandlingMixIn()
{
}


//---------------------------------------------------------------------------------------------- Event Dispatcher


tErrorCode
cTV2MetaEventHandlingMixIn::HandleMetaEvent( INTPTR  iEvent, INTPTR*  iArgs )
{
    switch( iEvent )
    {
        case  kPIEvents_MetaClosed:
            return  MetaEventClosed();

        case  kPIEvents_MetaPoint:
            return  MetaEventPoint( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2]), int(iArgs[3]), !!iArgs[4] );

        case  kPIEvents_MetaFreehand:
            return  MetaEventFreehand( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2]), int(iArgs[3]), !!iArgs[4] );

        case  kPIEvents_MetaLine:
            return  MetaEventLine( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2])/65536., int(iArgs[3])/65536., int(iArgs[4]), int(iArgs[5]), !!iArgs[6] );

        case  kPIEvents_MetaRectangle:
            return  MetaEventRectangle( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2])/65536., int(iArgs[3])/65536., int(iArgs[4]), !!iArgs[5] );

        case  kPIEvents_MetaCircle:
            return  MetaEventCircle( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2])/65536., int(iArgs[3]), !!iArgs[4] );

        case  kPIEvents_MetaEllipse:
            return  MetaEventEllipse( int(iArgs[0])/65536., int(iArgs[1])/65536., int(iArgs[2])/65536., int(iArgs[3])/65536., int(iArgs[4]), !!iArgs[5] );

        case  kPIEvents_MetaPreview:
            return  MetaEventPreview( {int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[3]), cPixelRectangle::eXYWH::kXYWH}, int(iArgs[4]), !!iArgs[5] );

        case  kPIEvents_MetaKeyboard:
            return  MetaEventKeyboard( int(iArgs[0]), (const char*)(iArgs[1]), int(iArgs[2]), int(iArgs[3]), int(iArgs[4]) );
    }
    return  kErrorCode_NoError;
}


//-------------------------------------------------------------------------------------------------------- Events


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventClosed()
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventPoint( double  iX, double  iY, int  iMouseButton, int  iPressure, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventFreehand( double  iX, double  iY, int  iMouseButton, int  iPressure, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventLine( double  iX1, double  iY1, double  iX2, double  iY2, int  iMouseButton, int  iPressure, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventRectangle( double  iX1, double  iY1, double  iX2, double  iY2, int  iMouseButton, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventCircle( double  iX, double  iY, double  iRadius, int  iMouseButton, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventEllipse( double  iX, double  iY, double  iXRadius, double  iYRadius, int  iMouseButton, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventPreview( const cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey )
{
    return  kErrorCode_NoError;
}


tErrorCode
cTV2MetaEventHandlingMixIn::MetaEventKeyboard( int  iCode, const char*  iBuffer, int  iBufferCount, int  iQualifier, int  iMouseButton )
{
    return  kErrorCode_NoError;
}


} // namespace  nSDK

