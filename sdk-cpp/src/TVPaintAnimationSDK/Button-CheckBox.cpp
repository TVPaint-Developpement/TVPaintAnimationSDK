/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-CheckBox.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A checkbox.
 */


#include "TVPaintAnimationSDK/Button-CheckBox.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonCheckBox


cTV2ButtonCheckBox::~cTV2ButtonCheckBox()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonCheckBox::cTV2ButtonCheckBox( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID,
                        bPIButtonFlags_Check | (*mValuePtr ? bPIButtonFlags_Select : 0),
                        iLabel.c_str() ) )
        return;
}


cTV2ButtonCheckBox::cTV2ButtonCheckBox( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel, bool*  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValuePtr( iValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID,
                        bPIButtonFlags_Check | (*mValuePtr ? bPIButtonFlags_Select : 0),
                        iLabel.c_str() ) )
        return;
}


void
cTV2ButtonCheckBox::Value( bool  iValue )
{
    if( iValue == *mValuePtr )
        //TODO: //CHECK: should we call mValueChange() anyway ?!?!?!
        return;

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    *mValuePtr = iValue;

    TVChangeButtonReq( plugin, req, mID, bPIButtonFlags_Check | (*mValuePtr ? bPIButtonFlags_Select : 0), nullptr );

    if( mValueChange )
        mValueChange( mID, *mValuePtr );
}


tErrorCode
cTV2ButtonCheckBox::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    Value( !Value() );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

