/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-HSeparator.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : An horizontal separator.
 */


#include "TVPaintAnimationSDK/Button-HSeparator.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonHSeparator


cTV2ButtonHSeparator::~cTV2ButtonHSeparator()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonHSeparator::cTV2ButtonHSeparator( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea ) :
    cTV2Button( iWindow, iID, iArea )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVAddButtonReq( plugin, req, iArea.X(), iArea.Y(), iArea.W(), iArea.H(), iID, bPIButtonFlags_HSeparator, nullptr );
}


} // namespace  nSDK

