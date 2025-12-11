/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-TextString.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A textfield for editing an integer.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"


#include "TVPaintAnimationSDK/TVPaintSDK.h" //TODO: //ZAP: find a clean way to use bPIButtonTextAlign_OutLeft and friends


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonTextString


/** @brief  A text field to edit a string.
    Its value is a string.
 */
class  cTV2ButtonTextString :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, const std::string&  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonTextString()  override;
    cTV2ButtonTextString( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, const std::string&  iValue, tValueChange  iValueChange=nullptr );
    cTV2ButtonTextString( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, std::string*  iValue, tValueChange  iValueChange=nullptr );

public: // properties
    std::string  Value() const  { return  *mValuePtr; }
    void  Value( const  std::string&  iValue );
    std::string  Label() const  { return  mLabel; }
    void  Label( const std::string&  iLabel );
    PIFlags  LabelAlign() const  { return  mLabelAlign; }
    void  LabelAlign( PIFlags  iLabelAlign );

public: // event handling
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

public:
    void  Activate();

private:
    std::string  mLabel;  //TODO: here or in button ?
    PIFlags  mLabelAlign  {bPIButtonTextAlign_OutLeft};
    std::string  mValueValue  {0};
    std::string*  mValuePtr  {&mValueValue};
    tValueChange  mValueChange;
};


} // namespace  nSDK
