/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ColorSwatch.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show/pick a color.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"
#include "TVPaintAnimationSDK/Types.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


#include <functional>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonColorSwatch


/** @brief  A button to show/pick a color. */
class  cTV2ButtonColorSwatch :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, PIPixel  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonColorSwatch()  override;
    cTV2ButtonColorSwatch( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, PIPixel  iValue, tValueChange  iValueChange=nullptr );
    cTV2ButtonColorSwatch( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, PIPixel*  iValue, tValueChange  iValueChange=nullptr );

public: // properties
    PIPixel  Color() const  { return  *mValuePtr; }
    void  Color( PIPixel  iColor );

public:
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
//mLabel ?  here or in button ?
    PIPixel  mValueValue  {0};
    PIPixel*  mValuePtr  {&mValueValue};
    tValueChange  mValueChange;

    PIBlock*  mImage  {nullptr};
};


} // namespace  nSDK
