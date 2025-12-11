/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Image.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button with an image as content.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonImage


/** @brief  A button displaying an image. */
class  cTV2ButtonImage :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonImage()  override;
    cTV2ButtonImage( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea );

public: // properties
    PIBlock*  Image() const  { return  mImage; }

public:
    void  RefreshImage();
    void  RefreshImage( const cPixelRectangle&  iArea );

private:
//mLabel ?  here or in button ?
    PIBlock*  mImage  {nullptr};
};


} // namespace  nSDK
