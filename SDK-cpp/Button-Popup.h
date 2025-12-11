/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Popup.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A popup button.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonPopup


/** @brief  A button which opens a popup menu. */
class  cTV2ButtonPopup :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tAction = std::function< void( int  /*iID*/ ) >;

public:
    virtual  ~cTV2ButtonPopup()  override;
    cTV2ButtonPopup( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, tAction  iAction );

public: // event handling
    virtual  tErrorCode  EventButtonDown( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
//mLabel ?  here or in button ?
    tAction  mAction;
};


} // namespace  nSDK
