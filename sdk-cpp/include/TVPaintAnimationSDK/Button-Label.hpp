/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Label.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A simple inert label.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"
#include "TVPaintAnimationSDK/Types.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonLabel


/** @brief  A simple label. */
class  cTV2ButtonLabel :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonLabel()  override;
    cTV2ButtonLabel( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::string&  iLabel );

public: // properties
    const std::string&  Label() const  { return  mLabel; }
    void  Label( const std::string&  iLabel );

private:
    //mLabel ?  here or in button ?
    std::string  mLabel;
};


} // namespace  nSDK
