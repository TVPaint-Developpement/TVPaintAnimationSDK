/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ColorSwatch.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show/pick a color.
 */


#include "TVPaintAnimationSDK/Button-ColorSwatch.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonColorSwatch


cTV2ButtonColorSwatch::~cTV2ButtonColorSwatch()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonColorSwatch::cTV2ButtonColorSwatch( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, PIPixel  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;

    mImage = TVAllocPIBlock( plugin, mArea.W(), mArea.H(), 0 );
    mImage->clear( mImage, mValuePtr->l );
    TVPutButtonImage( plugin, req, iID, mImage, 0 );
}


cTV2ButtonColorSwatch::cTV2ButtonColorSwatch( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, PIPixel*  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValuePtr( iValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;

    mImage = TVAllocPIBlock( plugin, mArea.W(), mArea.H(), 0 );
    mImage->clear( mImage, mValuePtr->l );
    TVPutButtonImage( plugin, req, iID, mImage, 0 );
}


void
cTV2ButtonColorSwatch::Color( PIPixel  iColor )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    *mValuePtr = iColor;
    mImage->clear( mImage, mValuePtr->l );
    TVRefreshButtonImage( plugin, req, mID, 0, 0, Area().W(), Area().H() );
}


tErrorCode
cTV2ButtonColorSwatch::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    char  result[256];
    int   but, r, g, b, a;

    TVSendCmd( plugin, "tv_piccolor", result );
    sscanf( result, "%d %d %d %d %d", &but, &r, &g, &b, &a );

    if( but == 0 )
    {
        // why did we ignore alpha in the original code ?  anyway I'm ignoring it here too for compatibility...
        Color( TVMakePIPixel( r, g, b, 255 ) );
        mValueChange( mID, *mValuePtr );
    }

    return  kErrorCode_NoError;
}


} // namespace  nSDK

