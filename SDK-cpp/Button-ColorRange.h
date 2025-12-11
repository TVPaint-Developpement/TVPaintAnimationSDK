/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ColorRange.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show/select a color range.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ColorRange


class  cTV2ColorRange
{
public:
    static  constexpr  int  kCount = 256;

public:
    cTV2ColorRange( cTV2Plugin*  iPlugin );

public:
    void  Load();
    void  Save() const;
    void  Grab();

    void  RenderToBlock( PIBlock*  ioBlock ) const;
    PIBlock*  CreateBlock( int  iW, int  iH ) const;

private:
    cTV2Plugin*   mPlugin  {nullptr};
    PIPixel  mColors[kCount];
};


//MARK: ========================================================================================================= cTV2ButtonColorRange


/** @brief  A button to show/select a color range. */
class  cTV2ButtonColorRange :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;


    //TODO: //CHECK: maybe a ptr/ref for iValue ?!
    using  tValueChange = std::function< void( int  /*iID*/, cTV2ColorRange  /*iValue*/ ) >;

public:
    virtual  ~cTV2ButtonColorRange()  override;
    cTV2ButtonColorRange( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, cTV2ColorRange  iValue, tValueChange  iValueChange=nullptr );
    cTV2ButtonColorRange( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, cTV2ColorRange*  iValue, tValueChange  iValueChange=nullptr );

public: // properties
    const cTV2ColorRange&  ColorRange() const  { return  *mValuePtr; }
    void  ColorRange( const cTV2ColorRange&  iColorRange );

public:
    virtual  tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;

private:
//mLabel ?  here or in button ?
    cTV2ColorRange  mValueValue;
    cTV2ColorRange*  mValuePtr  {&mValueValue};
    tValueChange  mValueChange;

    PIBlock*  mImage  {nullptr};
};


} // namespace  nSDK
