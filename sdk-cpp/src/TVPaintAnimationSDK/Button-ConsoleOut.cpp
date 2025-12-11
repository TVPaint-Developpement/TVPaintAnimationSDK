/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ConsoleOut.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : An output console.
 */


#include "TVPaintAnimationSDK/Button-ConsoleOut.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonConsoleOut


cTV2ButtonConsoleOut::~cTV2ButtonConsoleOut()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonConsoleOut::cTV2ButtonConsoleOut( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea ) :
    cTV2Button( iWindow, iID, iArea )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVAddButtonReq( plugin, req, iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, bPIButtonFlags_Invert, "" );
    TVPutButtonImage( plugin, req, iID, TVAllocPIBlock( plugin, iArea.W(), iArea.H(), 0 ), 0 );
    DrawString( "<Nothing yet>"s );
}


void
cTV2ButtonConsoleOut::DrawString( const std::string&  iString )
{
    constexpr int  kLineHeight = 14;

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    PIBlock*  block = TVGetButtonImage( plugin, req, mID, 0 );

    if( !block )
        return;

    memmove( block->Data, &block->Data[block->Width*kLineHeight], block->Width*(block->Height-kLineHeight)*sizeof(PIPixel) );
    memset( &block->Data[block->Width*(block->Height-kLineHeight)], 0, block->Width*kLineHeight*sizeof(PIPixel) );

    TVTextBlock( plugin, block, 5, block->Height-5, iString.c_str(), 0xFFFFFFFF );
    TVRefreshButtonImage( plugin, req, mID, 0, 0, block->Width, block->Height );
}


} // namespace  nSDK

