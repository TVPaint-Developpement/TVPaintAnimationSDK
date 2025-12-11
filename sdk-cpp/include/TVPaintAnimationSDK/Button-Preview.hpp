/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Preview.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show a small preview.
 */

#pragma once


#include "TVPaintAnimationSDK/Button.hpp"
#include "TVPaintAnimationSDK/Types.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonPreview


/** @brief  A button displaying an image. */
class  cTV2ButtonPreview :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonPreview()  override;
    cTV2ButtonPreview( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iSize );

public: // properties
    const PIBlock*  Src() const  { return  mSrc; }
    PIBlock*  Dst() const  { return  mDst; }

public:
    tErrorCode  ClearPreview();
    tErrorCode  StartPreview();
    tErrorCode  FinishPreview();

private:
    PIBlock*  MakeIcon() const;
    tErrorCode  MakeBGBlock( PIBlock*  ioImg ) const;
    tErrorCode  MakeFGBlock( PIBlock*  ioImg ) const;
private:
//mLabel ?  here or in button ?
    int  mSize;
    PIBlock*  mSrc  {nullptr};
    PIBlock*  mDst  {nullptr};
};


} // namespace  nSDK
