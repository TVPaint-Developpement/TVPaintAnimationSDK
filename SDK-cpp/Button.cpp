/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : The base class of all buttons.
 */


#include "Button.h"

#include "PixelCoordinates.h"
#include "Window.h"


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Button


cTV2Button::~cTV2Button()
{
    mWindow->UnregisterButton( this );

    mWindow = nullptr;
    mID = -1;
}


cTV2Button::cTV2Button( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea ) :
    cTV2ButtonEventHandlingMixIn(),
    mWindow( iWindow ),
    mID( iID ),
    mArea( iArea )
{
    mWindow->RegisterButton( this );
}


// properties


const std::string&
cTV2Button::HelpText() const
{
    return  mHelpText;
}


tErrorCode
cTV2Button::HelpText( const std::string&  iHelpText )
{
    mHelpText = iHelpText;
    return  mWindow->TV2ButtonHelpText( mID, mHelpText );
}


} // namespace  nSDK

