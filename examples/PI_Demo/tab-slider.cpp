/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-slider.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating sliders.
*/


#include "TVPaintAnimationSDK/TVPaintSDK.hpp"


#include "demo.h"


//MARK: ======================================================================================================== Slider Tab


constexpr  int  kTheButtonX = 450;
constexpr  int  kTheButtonY = 140;


enum  eIDs
{
    kID_First = 5000,

    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cTabSlider


class  cTabSlider :
    public  nSDK::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabSlider()  override;
    cTabSlider( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Slider"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

private:
    cDemoWindow*  mWindow;

    nSDK::cTV2ButtonTextInteger*  mGUIWidth   {nullptr};  int  mWidth   {200};
    nSDK::cTV2ButtonTextInteger*  mGUIMaxX    {nullptr};  int  mMaxX    {200};
    nSDK::cTV2ButtonTextInteger*  mGUINumX    {nullptr};  int  mNumX    {10};
    nSDK::cTV2ButtonTextInteger*  mGUIPosX    {nullptr};  int  mPosX    {50};
    nSDK::cTV2ButtonTextInteger*  mGUIHeight  {nullptr};  int  mHeight  {20};
    nSDK::cTV2ButtonTextInteger*  mGUIMaxY    {nullptr};  int  mMaxY    {0};
    nSDK::cTV2ButtonTextInteger*  mGUINumY    {nullptr};  int  mNumY    {0};
    nSDK::cTV2ButtonTextInteger*  mGUIPosY    {nullptr};  int  mPosY    {0};

    nSDK::cTV2ButtonLabel*   mGUISliderLabel  {nullptr};
    nSDK::cTV2ButtonSlider*  mGUISlider       {nullptr};
};


cTabSlider::cTabSlider( cDemoWindow*  iWindow ) :
    mWindow( iWindow )
{
}


void
cTabSlider::Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 140;
    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    x = 120;
    y = starty + 25 + 20;
    mGUIWidth  = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Width", &mWidth, 20, 300, [this](int,int){ mGUISlider->Area( {kTheButtonX, kTheButtonY, mWidth, mHeight, nSDK::cPixelRectangle::eXYWH::kXYWH} ); } ); y += 20;

    y += 20;
    mGUIMaxX   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "MaxX",  &mMaxX, 0, 1000, [this](int,int){ mGUISlider->UpdateBounds( mMaxX, mNumX, mMaxY, mNumY ); } ); y += 20;
    mGUINumX   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "NumX",  &mNumX, 0,  200, [this](int,int){ mGUISlider->UpdateBounds( mMaxX, mNumX, mMaxY, mNumY ); } ); y += 20;
    mGUIPosX   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "PosX",  &mPosX, 0, 1000, [this](int,int){ mGUISlider->RefreshGUI(); } ); y += 20;

    x = 300;
    y = starty + 25 + 20;
    mGUIHeight = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Height", &mHeight, 20, 100, [this](int,int){ mGUISlider->Area( {kTheButtonX, kTheButtonY, mWidth, mHeight, nSDK::cPixelRectangle::eXYWH::kXYWH} ); } ); y += 20;

    y += 20;
    mGUIMaxY   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "MaxY",  &mMaxY, 0, 1000, [this](int,int){ mGUISlider->UpdateBounds( mMaxX, mNumX, mMaxY, mNumY ); } ); y += 20;
    mGUINumY   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "NumY",  &mNumY, 0,  200, [this](int,int){ mGUISlider->UpdateBounds( mMaxX, mNumX, mMaxY, mNumY ); } ); y += 20;
    mGUIPosY   = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 60, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "PosY",  &mPosY, 0, 1000, [this](int,int){ mGUISlider->RefreshGUI(); } ); y += 20;

    mGUISliderLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;

    mGUISlider = new  nSDK::cTV2ButtonSlider( mWindow, kID_TheButton, {kTheButtonX, kTheButtonY, mWidth, mHeight, nSDK::cPixelRectangle::eXYWH::kXYWH}, &mPosX, mMaxX, mNumX, &mPosY, mMaxY, mNumY,
                                              [this]( int, int  iValueX, int  iValueY )
                                              {
                                                mGUIPosX->Value( iValueX );
                                                mGUIPosY->Value( iValueY );
                                                mGUIPosX->RefreshGUI();
                                                mGUIPosY->RefreshGUI();
                                              } );
}


void
cTabSlider::Hide()
{
    delete  mGUIWidth;   mGUIWidth  = nullptr;
    delete  mGUIMaxX;    mGUIMaxX   = nullptr;
    delete  mGUINumX;    mGUINumX   = nullptr;
    delete  mGUIPosX;    mGUIPosX   = nullptr;
    delete  mGUIHeight;  mGUIHeight = nullptr;
    delete  mGUIMaxY;    mGUIMaxY   = nullptr;
    delete  mGUINumY;    mGUINumY   = nullptr;
    delete  mGUIPosY;    mGUIPosY   = nullptr;

    delete  mGUISliderLabel;  mGUISliderLabel = nullptr;
    delete  mGUISlider;       mGUISlider      = nullptr;

    tSuperClass::Hide();
}


//---------------------------------------------------------------------------------------------------------------


nSDK::cTV2Tab*
NewTabSlider( cDemoWindow*  iWindow )
{
    return  new  cTabSlider( iWindow );
}

