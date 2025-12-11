/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Radio.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A radio button.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


// maybe some day make these classes templates on an enumeration, but for now an int as value will have to do


class  cTV2RadioGroup;
class  cTV2ButtonRadio;


//MARK: ========================================================================================================= cTV2RadioGroup


//TODO: we need to add a way to change the icons to the round ones thru the LL-SDK
class  cTV2RadioGroup
{
public:
    using  tValueChange = std::function< void( int  /*iValue*/ ) >;

public:
    cTV2RadioGroup( int  iValue, tValueChange  iValueChange, bool  iAllowNoValue=false ) :
        mValue( iValue ),
        mValueChange( iValueChange ),
        mAllowNoValue( iAllowNoValue )
    {
    }

public:
    int  Value() const  { return  mValue; }
    void  Value( int  iValue );

    bool  AllowNoValue() const  { return  mAllowNoValue; }

private:
    friend  class  cTV2ButtonRadio;
    void  RegisterButton( cTV2ButtonRadio*  iButton );
    void  UnregisterButton( cTV2ButtonRadio*  iButton );

private:
    int  mValue                 {0};
    tValueChange  mValueChange  {nullptr};
    bool  mAllowNoValue         {false};
    std::vector< cTV2ButtonRadio* >  mButtons;
};


//MARK: ========================================================================================================= cTV2ButtonRadio


/** @brief  A radio button.
    It needs a cTV2RadioGroup.
    Its value is an integer.

 */
class  cTV2ButtonRadio :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonRadio()  override;
    cTV2ButtonRadio( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, cTV2RadioGroup*  iRadioGroup );

public: // properties
    int  Value() const  { return  mValue; }

    bool  Checked() const  { return  mChecked; }

public: // event handling
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
    friend  class  cTV2RadioGroup;
    void  Check();
    void  Uncheck();

private:
//mLabel ?  here or in button ?
    bool  mChecked  {false};
    int  mValue     {-1};
    cTV2RadioGroup*  mRadioGroup  {nullptr};
};


} // namespace  nSDK
