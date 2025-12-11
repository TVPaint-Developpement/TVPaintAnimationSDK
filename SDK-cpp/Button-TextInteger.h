/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-TextInteger.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A textfield for editing an integer.
 */

#pragma once


#include "SDK-CPP.h"


#include "TVPaintSDK.h" //TODO: //ZAP: find a clean way to use bPIButtonTextAlign_OutLeft and friends


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonTextInteger


/** @brief  A text field to edit an integer.
    It can optionaly have a minislider, and bounds.
    Its value is an integer.
 */
class  cTV2ButtonTextInteger :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, int  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonTextInteger()  override;
    cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, tValueChange  iValueChange=nullptr, bool  iMiniSlider=false );
    cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int*  iValue, tValueChange  iValueChange=nullptr, bool  iMiniSlider=false );
    cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, int  iValueMin, int  iValueMax, tValueChange  iValueChange=nullptr, bool  iMiniSlider=false );
    cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int*  iValue, int  iValueMin, int  iValueMax, tValueChange  iValueChange=nullptr, bool  iMiniSlider=false );

private:
    void  Construct();

public: // properties
    int  Value() const  { return  *mValuePtr; }
    void  Value( int  iValue );
    std::string  Label() const  { return  mLabel; }
    void  Label( const std::string&  iLabel );
    PIFlags  LabelAlign() const  { return  mLabelAlign; }
    void  LabelAlign( PIFlags  iLabelAlign );

    // call when the value has been changed "outside" the button.
    void  RefreshGUI();

public: // event handling
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;
    virtual  tErrorCode  EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )  override;

private:
    std::string  mLabel;  //TODO: here or in button ?
    PIFlags  mLabelAlign  {bPIButtonTextAlign_OutLeft};
    int  mValueValue  {0};
    int*  mValuePtr  {&mValueValue};
    bool  mBounded  {false};
    int  mValueMin  {0};
    int  mValueMax  {100};
    tValueChange  mValueChange;
    bool  mMiniSlider;
};


} // namespace  nSDK
