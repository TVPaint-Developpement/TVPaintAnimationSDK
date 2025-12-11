/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Image.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button with an image as content.
 */


#include "TVPaintAnimationSDK/Button-Image.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonImage


cTV2ButtonImage::~cTV2ButtonImage()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonImage::cTV2ButtonImage( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea ) :
    cTV2Button( iWindow, iID, iArea )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;

    mImage = TVAllocPIBlock( plugin, mArea.W(), mArea.H(), 0 );
    TVPutButtonImage( plugin, req, iID, mImage, 0 );
}


void
cTV2ButtonImage::RefreshImage()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRefreshButtonImage( plugin, req, mID, 0, 0, Area().W(), Area().H() );
}


void
cTV2ButtonImage::RefreshImage( const cPixelRectangle&  iArea )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRefreshButtonImage( plugin, req, mID, iArea.X(), iArea.Y(), iArea.W(), iArea.H() );
}



} // namespace  nSDK

