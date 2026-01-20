/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Action.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : An action button.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonAction


/** @brief  A button which, when clicked, executes some action. */
class  cTV2ButtonAction :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tAction = std::function< void( int  /*iID*/ ) >;

public:
    virtual  ~cTV2ButtonAction()  override;
    cTV2ButtonAction( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, tAction  iAction );

public: // event handling
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
//mLabel ?  here or in button ?
    tAction  mAction;
};


} // namespace  nSDK
