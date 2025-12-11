/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ConsoleOut.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : An output console.
 */

#pragma once


#include "Button.h"
#include "Types.h"


//#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonConsoleOut


/** @brief  A button which displays some console like text with scrolling. */
class  cTV2ButtonConsoleOut :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonConsoleOut()  override;
    cTV2ButtonConsoleOut( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea );

public:
    void  DrawString( const std::string&  iString );

private:
};


} // namespace  nSDK
