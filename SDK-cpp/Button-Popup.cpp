/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Popup.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A popup button.
 */


#include "Button-Popup.h"


#include "Plugin.h"
#include "Window.h"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonPopup


cTV2ButtonPopup::~cTV2ButtonPopup()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonPopup::cTV2ButtonPopup( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, tAction  iAction ) :
    cTV2Button( iWindow, iID, iArea ),
    mAction( iAction )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Popup, iLabel.c_str() ) )
        return;
}


tErrorCode
cTV2ButtonPopup::EventButtonDown( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    if( mAction )
        mAction( iButtonID );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

