/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-TextString.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A textfield for editing an integer.
 */


#include "TVPaintAnimationSDK/Button-TextString.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonTextString


cTV2ButtonTextString::~cTV2ButtonTextString()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonTextString::cTV2ButtonTextString( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, const std::string&  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mValueChange( iValueChange )
{
    mWindow->TV2ButtonTextFieldAdd( iID, iArea, *mValuePtr );           // Text button
//    mWindow->TV2ButtonUnit( kID_Factor, "%" );                        // Text Unit
    mWindow->TV2ButtonName( iID, mLabel, mLabelAlign );  // Text + Align
}


cTV2ButtonTextString::cTV2ButtonTextString( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, std::string*  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValuePtr( iValue ),
    mValueChange( iValueChange )
{
    mWindow->TV2ButtonTextFieldAdd( iID, iArea, *mValuePtr );           // Text button
//    mWindow->TV2ButtonUnit( kID_Factor, "%" );                        // Text Unit
    mWindow->TV2ButtonName( iID, mLabel, mLabelAlign );  // Text + Align
}


void
cTV2ButtonTextString::Value( const std::string&  iValue )
{
    if( iValue == *mValuePtr )
        //TODO: //CHECK: should we call mValueChange() anyway ?!?!?!
        return;

    *mValuePtr = iValue;

    mWindow->TV2ButtonSetString( mID, *mValuePtr );

    if( mValueChange )
        mValueChange( mID, *mValuePtr );
}


void
cTV2ButtonTextString::Label( const  std::string&  iLabel )
{
    mLabel = iLabel;
    mWindow->TV2ButtonName( mID, mLabel, mLabelAlign );
}


void
cTV2ButtonTextString::LabelAlign( PIFlags  iLabelAlign )
{
    mLabelAlign = iLabelAlign;
    mWindow->TV2ButtonName( mID, mLabel, mLabelAlign );
}


tErrorCode
cTV2ButtonTextString::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    std::string  tmp;
    mWindow->TV2ButtonGetString( mID, &tmp );

    Value( tmp );

    return  kErrorCode_NoError;
}


void
cTV2ButtonTextString::Activate()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVChangeButtonReq( plugin, req, mID, bPIButtonFlags_Text|bPIButtonFlags_TextActive, NULL );
}


} // namespace  nSDK

