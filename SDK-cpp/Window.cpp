/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Window.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A window.
 */


#include "Window.h"

#include "Button.h"
#include "Keys.h"
#include "Plugin.h"


#include <map>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Window


static  std::map< INTPTR, cTV2Window* >  sgRequesters;
static  cTV2Window*  sgWindowPatch = nullptr;


static
int
EventHandler( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs )
{
printf("(cTV2Window)::EventHandler()\n");//DEBUG:
    if( sgWindowPatch )
        sgWindowPatch->LegacyReq( iReq );

    cTV2Window*  window = cTV2Window::Lookup( iReq );
    if( window == nullptr )
        return  0;

    tErrorCode  err = window->HandleEvent( iEvent, iArgs );
    if( err )
        return  0;

    return  1;
}


//------------------------------------------------------------------------------------------------------ Creation


//static
cTV2Window*
cTV2Window::TV2OpenWindow( cTV2Plugin*  iPlugin, cTV2Window*  iWindow, const cPixelRectangle&  iArea, const std::string&  iTitle, PIFlags  iFlags )
{
    sgWindowPatch = iWindow;

    /*INTPTR  req =*/ TVOpenReqEx( iPlugin->LegacyPlugin(), iArea.W(), iArea.H(), iArea.X(), iArea.Y(), iFlags, iTitle.c_str(), &EventHandler );

    sgWindowPatch = nullptr;
    return  iWindow;
}


//static
cTV2Window*
cTV2Window::TV2OpenFilterWindow( cTV2Plugin*  iPlugin, cTV2Window*  iWindow, const cPixelSize&  iSize, PIFlags  iReqFlags, PIFlags  iMenuFlags )
{
    sgWindowPatch = iWindow;

    /*INTPTR  req =*/ TVOpenFilterReqEx( iPlugin->LegacyPlugin(), iSize.W(), iSize.H(), &EventHandler, iPlugin->Keys()->LegacyKeys(), iReqFlags, iMenuFlags );

    sgWindowPatch = nullptr;
    return  iWindow;
}


//static
cTV2Window*
cTV2Window::Lookup( INTPTR  iReq )
{
    cTV2Window*  window = sgWindowPatch;
    if( window == nullptr )
    {
        try
        {
            window = sgRequesters.at( iReq );
        }
        catch( ... )
        {
            return  nullptr;
        }
    }
    return  window;
}


void
cTV2Window::TV2CloseWindow()
{
    if( mLegacyReq )
        TVCloseReq( mPlugin->LegacyPlugin(), mLegacyReq );
    mLegacyReq = 0;
}


//---------------------------------------------------------------------------------------------------- Properties


tErrorCode
cTV2Window::Area( const cPixelRectangle&  iArea )
{
    if( !TVResizeReq( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H() ) )
        return  kErrorCode_LegacyError;
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::TV2Title( const std::string&  iTitle )
{
    if( !TVSetReqTitle( Plugin()->LegacyPlugin(), LegacyReq(), iTitle.c_str() ) )
        return  kErrorCode_LegacyError;
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::RegisterButton( cTV2Button*  iButton )
{
    if( mButtons.contains( iButton->ID() ) )
       return  "dpID"_Error;  // DuPlicate ID

    mButtons.insert_or_assign( iButton->ID(), iButton );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::UnregisterButton( cTV2Button*  iButton )
{
    if( !mButtons.contains( iButton->ID() ) )
       return  "nsID"_Error;  // No Such ID

    mButtons.erase( iButton->ID() );
    return  kErrorCode_NoError;
}


cTV2Button*
cTV2Window::LookupButton( int  iID )
{
    if( !mButtons.contains( iID ) )
        return  nullptr;

    return  mButtons[iID];
}

//--------------------------------------------------------------------------------------------- Window management


tErrorCode
cTV2Window::TV2ToFront()
{
    if( !TVReqToFront( Plugin()->LegacyPlugin(), LegacyReq() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


//------------------------------------------------------------------------------------------------ Event handling

tErrorCode
cTV2Window::HandleEvent( INTPTR  iEvent, INTPTR*  iArgs )
{
printf("cTV2Window::HandleEvent()\n");//DEBUG:

    // all button events
    switch( iEvent )
    {
        case  kPIEvents_ButtonDown:
        case  kPIEvents_ButtonUp:
        case  kPIEvents_ButtonText:
        case  kPIEvents_ButtonMiniSlider:
        case  kPIEvents_ButtonSliderDragged:
        case  kPIEvents_ButtonSliderReleased:
        case  kPIEvents_ButtonTabChanged:
        case  kPIEvents_MouseMove:
        {
            cTV2Button*  button = LookupButton( int(iArgs[0]) );
            if( button )
                return  button->HandleButtonEvent( iEvent, iArgs );
            else
                return  HandleButtonEvent( iEvent, iArgs );
        }
        default:
            // do nothing
            break;
    }

    // all window/misc events
    switch( iEvent )
    {
        case  kPIEvents_WindowOpen:
        case  kPIEvents_WindowClose:
        case  kPIEvents_WindowDragged:
        case  kPIEvents_WindowResizing:
        case  kPIEvents_WindowResized:
        case  kPIEvents_WindowKeyDown:
        case  kPIEvents_WindowFunctionKeyDown:
        case  kPIEvents_WindowHidden:
        case  kPIEvents_WindowShown:
        case  kPIEvents_WindowCollapsed:
        case  kPIEvents_WindowUncollapsed:
        case  kPIEvents_ProjectChanged:
        case  kPIEvents_ProjectInactivated:
        case  kPIEvents_ProjectActivated:
        case  kPIEvents_KeysetReset:
        case  kPIEvents_KeysetLoad:
        case  kPIEvents_KeysetSave:
        case  kPIEvents_KeysetSaveAs:
        case  kPIEvents_Coordinates:
        case  kPIEvents_Ticks:
        case  kPIEvents_RedrawHud:
        case  kPIEvents_ColorPicked:
        case  kPIEvents_RoomChanged:
        {
            return  HandleWindowEvent( iEvent, iArgs );
        }
        default:
            // do nothing
            break;
    }

    return  kErrorCode_NotProcessed;
}


tErrorCode
cTV2Window::RegisterTicksHandler( INTPTR  iID, tTicksHandler  iHandler )
{
    mTicksHandlers.insert_or_assign( iID, iHandler );
    TVGrabTicks( Plugin()->LegacyPlugin(), LegacyReq(), kPITicks_On );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::UnregisterTicksHandler( INTPTR  iID )
{
    mTicksHandlers.erase( iID );
    if( mTicksHandlers.size() == 0 )
        TVGrabTicks( Plugin()->LegacyPlugin(), LegacyReq(), kPITicks_Off );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::EventTicks( const cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic )
{
    for( auto  handler: mTicksHandlers )
        handler.second( iPosition, iMouseButton, iPressure, iTimeMS, iCtrlKey, iFX,  iFY, iIconic );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::RegisterCoordinatesHandler( INTPTR  iID, tCoordinatesHandler  iHandler )
{
    mCoordinatesHandlers.insert_or_assign( iID, iHandler );
    TVGrabCoords( Plugin()->LegacyPlugin(), LegacyReq(), 1 );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::UnregisterCoordinatesHandler( INTPTR  iID )
{
    mCoordinatesHandlers.erase( iID );
    if( mCoordinatesHandlers.size() == 0 )
        TVGrabCoords( Plugin()->LegacyPlugin(), LegacyReq(), 0 );
    return  kErrorCode_NoError;
}


tErrorCode
cTV2Window::EventCoordinates( const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    for( auto  handler: mCoordinatesHandlers )
        handler.second( iPosition, iMouseButton, iPressure, iTimeMS, iCtrlKey, iFX,  iFY );
    return  kErrorCode_NoError;
}


//------------------------------------------------------------------------------------------------------- Buttons (maybe convert to classes too ?)


tErrorCode
cTV2Window::TV2ButtonTextFieldAdd( int  iID, const cPixelRectangle&  iArea, const std::string&  iText )
{
    if( !TVAddButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, bPIButtonFlags_Text, iText.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonMiniSliderAdd( int  iID, const cPixelPosition&  iPosition )
{
    if( !TVAddButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iPosition.X(), iPosition.Y(), 0, 0, iID, bPIButtonFlags_Minislider, nullptr ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonHSeparatorAdd( int  iID, const cPixelRectangle&  iArea )
{
    if( !TVAddButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, bPIButtonFlags_HSeparator, nullptr ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonCheckBoxAdd( int  iID, const cPixelPosition&  iPosition, const std::string&  iText, bool  iState )
{
    if( !TVAddButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iPosition.X(), iPosition.Y(), 0, 0, iID,
                         bPIButtonFlags_Check | (iState ? bPIButtonFlags_Select : 0),
                         iText.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonCheckBoxState( int  iID, bool  iState )
{
    if( !TVChangeButtonReq( mPlugin->LegacyPlugin(), mLegacyReq, iID,
                            bPIButtonFlags_Check | (iState ? bPIButtonFlags_Select : 0),
                            nullptr ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonActionAdd( int  iID, const cPixelRectangle&  iArea, const std::string&  iText )
{
    if( !TVAddButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, bPIButtonFlags_Action, iText.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonTabsAdd( int  iID, const nSDK::cPixelRectangle&  iArea, const std::vector< std::string >&  iTabNames, int  iSelectedTabIndex )
{
    int  tabcount = std::min( int(iTabNames.size()), 100 );
    const char*  tabnames[100];
    for( int  i = 0; i < tabcount; ++i )
        tabnames[i] = iTabNames[i].c_str();

//TODO: //CHECK: i *think* tvpaint will make a copy of tabnames...
    if( !TVAddTabs( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, tabcount, iSelectedTabIndex, tabnames ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonSliderAdd( int  iID, const cPixelRectangle&  iArea, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY )
{
    if( !TVAddSliderReq( Plugin()->LegacyPlugin(), LegacyReq(), iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, 0, iMaxX, iNumX, iPosX, iMaxY, iNumY, iPosY ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonSliderChange( int  iID, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY )
{
    if( !TVChangeSliderReq( Plugin()->LegacyPlugin(), LegacyReq(), iID, iMaxX, iNumX, iPosX, iMaxY, iNumY, iPosY ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonRemove( int  iID )
{
    if( !TVRemoveButtonReq( Plugin()->LegacyPlugin(), LegacyReq(), iID ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonName( int  iID, const std::string&  iText, PIFlags/*ePIButtonTextAlign*/  iAlign )
{
    if( !TVChangeButtonName( Plugin()->LegacyPlugin(), LegacyReq(), iID, iText.c_str(), iAlign ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonHelpText( int  iID, const std::string&  iHelpText )
{
    if( !TVSetButtonInfoText( Plugin()->LegacyPlugin(), LegacyReq(), iID, iHelpText.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonUnit( int  iID, const std::string&  iUnit )
{
    if( !TVPutButtonStringUnit( Plugin()->LegacyPlugin(), LegacyReq(), iID, iUnit.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonGetString( int  iID, std::string*  oString )
{
    char  tmp[512];
    if( !TVGetButtonString( Plugin()->LegacyPlugin(), LegacyReq(), iID, tmp, 512 ) )
        return  kErrorCode_LegacyError;
    tmp[511] = 0;
    *oString = tmp;
    return  0;
}


tErrorCode
cTV2Window::TV2ButtonSetString( int  iID, const std::string&  iString )
{
    if( !TVPutButtonString( Plugin()->LegacyPlugin(), LegacyReq(), iID, iString.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


//------------------------------------------------------------------------------------------------------- Helpers


std::string
cTV2Window::TV2GetLocaleString( int  iIndex, const std::string&  iDefault ) const
{
    return  Plugin()->TV2GetLocaleString( iIndex, iDefault );
}


//---------------------------------------------------------------------------- Access to Legacy Stuff (temporary)


void
cTV2Window::LegacyReq( INTPTR  iLegacyReq )
{
    mLegacyReq = iLegacyReq;
    sgRequesters.insert_or_assign( mLegacyReq, this );
}


//----------------------------------------------------------------------------------------------------- ctor/dtor


cTV2Window::cTV2Window( cTV2Plugin*  iPlugin ) :
    cTV2WindowEventHandlingMixIn(),
    cTV2ButtonEventHandlingMixIn(),
    mPlugin( iPlugin )
{
}


cTV2Window::~cTV2Window()
{
    sgRequesters.erase( mLegacyReq );
    mLegacyReq = 0;
    mPlugin = nullptr;
}


} // namespace  nSDK

