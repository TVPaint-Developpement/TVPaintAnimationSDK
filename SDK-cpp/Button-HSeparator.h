/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-HSeparator.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : An horizontal separator.
 */

#pragma once


#include "Button.h"
#include "Types.h"


//#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonHSeparator


/** @brief  A horizontal separator. */
class  cTV2ButtonHSeparator :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonHSeparator()  override;
    cTV2ButtonHSeparator( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea );
};


} // namespace  nSDK
