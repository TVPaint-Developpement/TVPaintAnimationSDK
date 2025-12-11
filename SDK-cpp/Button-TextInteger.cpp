/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-TextInteger.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A textfield for editing an integer.
 */


#include "Button-TextInteger.h"


#include "Plugin.h"
#include "Window.h"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonTextInteger


cTV2ButtonTextInteger::~cTV2ButtonTextInteger()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonTextInteger::cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, tValueChange  iValueChange, bool  iMiniSlider ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mValueChange( iValueChange ),
    mMiniSlider( iMiniSlider )
{
    Construct();
}


cTV2ButtonTextInteger::cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int*  iValue, tValueChange  iValueChange, bool  iMiniSlider ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValuePtr( iValue ),
    mValueChange( iValueChange ),
    mMiniSlider( iMiniSlider )
{
    Construct();
}


cTV2ButtonTextInteger::cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, int  iValueMin, int  iValueMax, tValueChange  iValueChange, bool  iMiniSlider ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mBounded( true ),
    mValueMin( iValueMin ),
    mValueMax( iValueMax ),
    mValueChange( iValueChange ),
    mMiniSlider( iMiniSlider )
{
    Construct();
}


cTV2ButtonTextInteger::cTV2ButtonTextInteger( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int*  iValue, int  iValueMin, int  iValueMax, tValueChange  iValueChange, bool  iMiniSlider ) :
    cTV2Button( iWindow, iID, iArea ),
    mLabel( iLabel ),
    mValuePtr( iValue ),
    mBounded( true ),
    mValueMin( iValueMin ),
    mValueMax( iValueMax ),
    mValueChange( iValueChange ),
    mMiniSlider( iMiniSlider )
{
    Construct();
}


void
cTV2ButtonTextInteger::Construct()
{
//    PIFilter*  plugin = mWindow->Plugin()->LegacyPlugin();
//    INTPTR  req = mWindow->LegacyReq();

    cPixelRectangle  textArea( mArea );
    cPixelRectangle  miniArea( mArea );
    if( mMiniSlider )
    {
        textArea.X2( textArea.X2() - 20 );
        miniArea.X1( miniArea.X2() - 20 );
    }
    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "%d", *mValuePtr );
    mWindow->TV2ButtonTextFieldAdd( mID, textArea, tmp );               // Text button
//    mWindow->TV2ButtonUnit( kID_Factor, "%" );                        // Text Unit
    mWindow->TV2ButtonName( mID, mLabel, bPIButtonTextAlign_OutLeft );  // Text + Align

    if( mMiniSlider )
    {
//        mWindow->TV2ButtonMiniSliderAdd( iID?!?!?!?!, miniArea );
    }
}


void
cTV2ButtonTextInteger::Value( int  iValue )
{
printf("cTV2ButtonTextInteger::Value( %d, (%d,%d,%d) )\n",iValue,*mValuePtr,mValueMin,mValueMax);
    if( iValue == *mValuePtr )
        //TODO: //CHECK: should we call mValueChange() anyway ?!?!?!
        return;

    if( mBounded  &&  iValue < mValueMin )
        iValue = mValueMin;
    if( mBounded  &&  iValue > mValueMax )
        iValue = mValueMax;
//    PIFilter*  plugin = mWindow->Plugin()->LegacyPlugin();
//    INTPTR  req = mWindow->LegacyReq();

    *mValuePtr = iValue;
printf("--                        --( %d )\n",*mValuePtr);

    RefreshGUI();

    if( mValueChange )
        mValueChange( mID, *mValuePtr );
}


void
cTV2ButtonTextInteger::Label( const  std::string&  iLabel )
{
    mLabel = iLabel;
    mWindow->TV2ButtonName( mID, mLabel, mLabelAlign );
}


void
cTV2ButtonTextInteger::LabelAlign( PIFlags  iLabelAlign )
{
    mLabelAlign = iLabelAlign;
    mWindow->TV2ButtonName( mID, mLabel, mLabelAlign );
}


void
cTV2ButtonTextInteger::RefreshGUI()
{
    //TODO: use fmt !
    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "%d", *mValuePtr );
    mWindow->TV2ButtonSetString( mID, tmp );
}


tErrorCode
cTV2ButtonTextInteger::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    std::string  tmp;
    mWindow->TV2ButtonGetString( mID, &tmp );

    Value( std::stoi( tmp ) );

    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonTextInteger::EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )
{
    Value( *mValuePtr + iDelta * (iCtrlKey ? 10 : 1) );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

