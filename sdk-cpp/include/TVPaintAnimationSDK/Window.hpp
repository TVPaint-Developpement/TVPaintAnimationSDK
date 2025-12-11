/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Window.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A window.
 */

#pragma once


#include "TVPaintAnimationSDK/Types.hpp"

#include "TVPaintAnimationSDK/EventHandlingMixIn.hpp"
#include "TVPaintAnimationSDK/PixelCoordinates.hpp"


#include <map>
#include <string>


namespace  nSDK { class  cTV2Button; }


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Window


class  cTV2Window :
    public  cTV2WindowEventHandlingMixIn,
    public  cTV2ButtonEventHandlingMixIn
{
public:
    SDK_ABSTRACT_CLASS( cTV2Window );
    SDK_NON_COPYABLE( cTV2Window );

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Creation/Destruction
///@{
    //TODO: //BLURP: if multiwindow, XY relative to screen, if single window, XY relative to that window
    static  cTV2Window*  TV2OpenWindow( cTV2Plugin*  iPlugin, cTV2Window*  iWindow, const cPixelRectangle&  iArea, const std::string&  iTitle, PIFlags  iFlags );

    static  cTV2Window*  TV2OpenFilterWindow( cTV2Plugin*  iPlugin, cTV2Window*  iWindow, const cPixelSize&  iSize, PIFlags  iReqFlags, PIFlags  iMenuFlags );

    static  cTV2Window*  Lookup( INTPTR  iReq );

    void  TV2CloseWindow();
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Properties
///@{
    cTV2Plugin*  Plugin()  { return  mPlugin; }
    const cTV2Plugin*  Plugin() const  { return  mPlugin; }

    tErrorCode  TV2Title( const std::string&  iTitle );

    tErrorCode  Area( const cPixelRectangle&  iArea );

    tErrorCode  RegisterButton( cTV2Button*  iButton );
    tErrorCode  UnregisterButton( cTV2Button*  iButton );
    const std::map< int, cTV2Button* >&  Buttons() const  { return  mButtons; }
    cTV2Button*  LookupButton( int  iID );
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Window management
///@{
    tErrorCode  TV2ToFront();
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Event handling
///@{
    tErrorCode  HandleEvent( INTPTR  iEvent, INTPTR*  iArgs );

    using  tTicksHandler = std::function< void( const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic ) >;
    tErrorCode  RegisterTicksHandler( INTPTR  iID, tTicksHandler  iHandler );
    tErrorCode  UnregisterTicksHandler( INTPTR  iID );
    virtual  nSDK::tErrorCode  EventTicks( const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic )  override;

    using  tCoordinatesHandler = std::function< void( const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY ) >;
    tErrorCode  RegisterCoordinatesHandler( INTPTR  iID, tCoordinatesHandler  iHandler );
    tErrorCode  UnregisterCoordinatesHandler( INTPTR  iID );
    virtual  nSDK::tErrorCode  EventCoordinates( const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Buttons (maybe convert to classes too ?)
///@{
    tErrorCode  TV2ButtonTextFieldAdd( int  iID, const cPixelRectangle&  iArea, const std::string&  iText );

    tErrorCode  TV2ButtonMiniSliderAdd( int  iID, const cPixelPosition&  iPosition );

    tErrorCode  TV2ButtonHSeparatorAdd( int  iID, const cPixelRectangle&  iArea );

    tErrorCode  TV2ButtonCheckBoxAdd( int  iID, const cPixelPosition&  iPosition, const std::string&  iText, bool  iState );
    tErrorCode  TV2ButtonCheckBoxState( int  iID, bool  iState );

    tErrorCode  TV2ButtonActionAdd( int  iID, const cPixelRectangle&  iArea, const std::string&  iText );

    tErrorCode  TV2ButtonTabsAdd( int  iID, const cPixelRectangle&  iArea, const std::vector< std::string >&  iTabNames, int  iSelectedTabIndex );

    tErrorCode  TV2ButtonSliderAdd( int  iID, const cPixelRectangle&  iArea, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY );
    tErrorCode  TV2ButtonSliderChange( int  iID, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY );

//    int  TVAddButtonReq( PIFilter*  iPlugin, INTPTR  iReq, const cPixelRectangle&  iArea, int  iID, INTPTR  iFlags, const char*  iText );

    tErrorCode  TV2ButtonRemove( int  iID );

    tErrorCode  TV2ButtonName( int  iID, const std::string&  iText, PIFlags/*ePIButtonTextAlign*/  iFlags );
    tErrorCode  TV2ButtonHelpText( int  iID, const std::string&  iHelpText );
    tErrorCode  TV2ButtonUnit( int  iID, const std::string&  iUnit );
    tErrorCode  TV2ButtonGetString( int  iID, std::string*  oString );
    tErrorCode  TV2ButtonSetString( int  iID, const std::string&  iString );

///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Helpers
///@{
    std::string  TV2GetLocaleString( int  iIndex, const std::string&  iDefault ) const;
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Access to Legacy Stuff (temporary)
///@{
    INTPTR  LegacyReq()  { return  mLegacyReq; }
    void  LegacyReq( INTPTR  iLegacyReq );
///@}

//---------------------------------------------------------------------------------------------------------------
protected:
    cTV2Window( cTV2Plugin*  iPlugin );

//---------------------------------------------------------------------------------------------------------------
private:
    INTPTR  mLegacyReq    {0};
    cTV2Plugin*  mPlugin  {nullptr};
    std::map< int, cTV2Button* >  mButtons  {};
    std::map< INTPTR, tTicksHandler >  mTicksHandlers;
    std::map< INTPTR, tCoordinatesHandler >  mCoordinatesHandlers;
};


} // namespace  nSDK
