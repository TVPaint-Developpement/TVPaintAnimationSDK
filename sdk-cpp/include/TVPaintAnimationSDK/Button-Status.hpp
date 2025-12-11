/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Status.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A stat button.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"
#include "TVPaintAnimationSDK/Types.hpp"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonStatus


/** @brief  A status button. It works like a checkbox, but its look is different.
    Its value is a boolean.
 */
class  cTV2ButtonStatus :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, bool  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonStatus()  override;
    cTV2ButtonStatus( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool  iValue, tValueChange  iValueChange=nullptr );
    cTV2ButtonStatus( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool*  iValue, tValueChange  iValueChange=nullptr );

public: // properties
    bool  Value() const  { return  *mValuePtr; }
    void  Value( bool  iValue );

public: // event handling
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
//mLabel ?  here or in button ?
    bool  mValueValue  {false};
    bool*  mValuePtr  {&mValueValue};
    tValueChange  mValueChange;
};


} // namespace  nSDK
