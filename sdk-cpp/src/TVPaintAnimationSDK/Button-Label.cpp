/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Label.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A simple inert label.
 */


#include "TVPaintAnimationSDK/Button-Label.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonLabel


cTV2ButtonLabel::~cTV2ButtonLabel()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonLabel::cTV2ButtonLabel( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, mLabel.c_str() ) )
        return;
}


void
cTV2ButtonLabel::Label( const std::string&  iLabel )
{
    if( iLabel == mLabel )
        return;

    mLabel = iLabel;
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVChangeButtonReq( plugin, req, mID, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, mLabel.c_str() ) )
        return;
}


} // namespace  nSDK

