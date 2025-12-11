/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : The base class of all buttons.
 */

#pragma once


#include "EventHandlingMixIn.h"
#include "PixelCoordinates.h"
#include "Types.h"


//#include <functional>
//#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Button


class  cTV2Button :
    public  cTV2ButtonEventHandlingMixIn
{
public:
    virtual  ~cTV2Button();
    cTV2Button( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea );

public: //properties
    cTV2Window*  Window()  { return  mWindow; }

    int  ID() const  { return  mID; }

    const cPixelRectangle&  Area() const  { return  mArea; }
    // there is no way to change the area of a button...

    const std::string&  HelpText() const;
    tErrorCode  HelpText( const std::string&  iHelpText );

protected:
    cTV2Window*  mWindow;
    int  mID;
    cPixelRectangle  mArea;
    std::string  mHelpText;
};


} // namespace  nSDK
