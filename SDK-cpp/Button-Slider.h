/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Slider.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A slider button.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonSlider


/** @brief  A slider button.
    Its value is an integer.
 */
class  cTV2ButtonSlider :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;
    using  tValueChange = std::function< void( int  /*iID*/, int  /*iValueX*/, int  /*iValueY*/ ) >;

public:
    virtual  ~cTV2ButtonSlider()  override;
    cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iValueX, int  iMaxX, int  iNumX, tValueChange  iValueChange=nullptr );
    cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int*  iValueX, int  iMaxX, int  iNumX, tValueChange  iValueChange=nullptr );

    //TODO: how to make one in the Y direction, with exactly the same arguments, in C++ ?!??!?!??!

    cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iValueX, int  iMaxX, int  iNumX, int  iValueY, int  iMaxY, int  iNumY, tValueChange  iValueChange=nullptr );
    cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int*  iValueX, int  iMaxX, int  iNumX, int*  iValueY, int  iMaxY, int  iNumY, tValueChange  iValueChange=nullptr );

public: // properties
    void  Area( const cPixelRectangle&  iArea );

    int  ValueX() const  { return  *mValueXPtr; }
    void  ValueX( int  iValueX );
    int  ValueY() const  { return  *mValueYPtr; }
    void  ValueY( int  iValueY );
    void  ValueXY( int  iValueX, int  iValueY );

    void  UpdateBounds( int  iMaxX, int  iNumX, int  iMaxY, int  iNumY );

    // call when the value has been changed "outside" the button.
    void  RefreshGUI();

public: // event handling
    virtual  tErrorCode  EventButtonSliderDragged( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )  override;
    virtual  tErrorCode  EventButtonSliderReleased( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )  override;

private:
//mLabel ?  here or in button ?
    int  mValueXValue  {0};
    int*  mValueXPtr  {&mValueXValue};
    int  mMaxX;
    int  mNumX;
    int  mValueYValue  {0};
    int*  mValueYPtr  {&mValueYValue};
    int  mMaxY;
    int  mNumY;
    tValueChange  mValueChange;
};


} // namespace  nSDK
