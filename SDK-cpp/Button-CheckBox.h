/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-CheckBox.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A checkbox.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonCheckBox


/** @brief  A checkbox button.
    Its value is a boolean value.
 */
class  cTV2ButtonCheckBox :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, bool  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonCheckBox()  override;
    cTV2ButtonCheckBox( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool  iValue, tValueChange  iValueChange=nullptr );
    cTV2ButtonCheckBox( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool*  iValue, tValueChange  iValueChange=nullptr );

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
