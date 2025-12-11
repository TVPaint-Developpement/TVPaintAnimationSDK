/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Radio.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A radio button.
 */


#include "Button-Radio.h"


#include "Plugin.h"
#include "Window.h"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2RadioGroup


void
cTV2RadioGroup::Value( int  iValue )
{
    mValue = iValue;
    for( auto&&  button : mButtons )
    {
        if( button->Value() == mValue )
            button->Check();
        else if( button->Checked() )
            button->Uncheck();
    }
    if( mValueChange )
        mValueChange( mValue );
}


void
cTV2RadioGroup::RegisterButton( cTV2ButtonRadio*  iButton )
{
    //assert  !mButtons.contain(iButton)
    mButtons.push_back( iButton );
}


void
cTV2RadioGroup::UnregisterButton( cTV2ButtonRadio*  iButton )
{
    //assert  mButtons.contain(iButton)
    mButtons.erase( std::remove( mButtons.begin(), mButtons.end(), iButton ), mButtons.end() );
}


//MARK: ========================================================================================================= cTV2ButtonRadio


cTV2ButtonRadio::~cTV2ButtonRadio()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );

    mRadioGroup->UnregisterButton( this );
}


cTV2ButtonRadio::cTV2ButtonRadio( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, int  iValue, cTV2RadioGroup*  iRadioGroup ) :
    cTV2Button( iWindow, iID, iArea ),
    mValue( iValue ),
    mRadioGroup( iRadioGroup )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    mChecked = (mRadioGroup->Value() == mValue);
    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID,
                         bPIButtonFlags_Check | (mChecked ? bPIButtonFlags_Select : 0),
                         iLabel.c_str() ) )
        return;

    mRadioGroup->RegisterButton( this );
}


void
cTV2ButtonRadio::Check()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVChangeButtonReq( plugin, req, mID, bPIButtonFlags_Check | bPIButtonFlags_Select, nullptr );
    mChecked = true;
}


void
cTV2ButtonRadio::Uncheck()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVChangeButtonReq( plugin, req, mID, bPIButtonFlags_Check, nullptr );
    mChecked = false;
}


tErrorCode
cTV2ButtonRadio::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    if( mRadioGroup->AllowNoValue()  &&  mRadioGroup->Value() == mValue )
        mRadioGroup->Value( -1 );
    else
        mRadioGroup->Value( mValue );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

