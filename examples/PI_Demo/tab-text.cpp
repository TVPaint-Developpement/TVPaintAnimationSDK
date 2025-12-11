/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-text.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating texts.
*/


#include "TVPaintAnimationSDK/TVPaintSDK.hpp"


#include "TVPaintAnimationSDK/TVPaintSDK.h"


#include "demo.h"



//MARK: ========================================================================================================= Text Tab


constexpr  int  kTheButtonX = 450;
constexpr  int  kTheButtonY = 140;
constexpr  int  kTheButtonW = 150;
constexpr  int  kTheButtonH = 20;


enum  eIDs
{
    kID_First = 3000,

    kID_TopLabel = kID_First+1,
    kID_TheButtonLabel = kID_First+2,

    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cTabText


class  cTabText :
    public  nSDK::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabText()  override;
    cTabText( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Text"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

private:
    cDemoWindow*  mWindow;
    nSDK::cTV2RadioGroup  mLeftRight;
    nSDK::cTV2RadioGroup  mTopBottom;

    PIFlags  mLeftRightFlags  {0};
    PIFlags  mTopBottomFlags  {0};

    nSDK::cTV2ButtonLabel*       mTopLabel     {nullptr};

    nSDK::cTV2ButtonRadio*       mGUIILeft     {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIIRight    {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIOLeft     {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIORight    {nullptr};

    nSDK::cTV2ButtonRadio*       mGUIITop      {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIIBottom   {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIOTop      {nullptr};
    nSDK::cTV2ButtonRadio*       mGUIOBottom   {nullptr};

    nSDK::cTV2ButtonTextString*  mButton       {nullptr};
};


cTabText::cTabText( cDemoWindow*  iWindow ) :
    mWindow( iWindow ),
    mLeftRight( bPIButtonTextAlign_OutLeft, [this]( int  iValue )
                    {
                        mLeftRightFlags = iValue;
                        mButton->LabelAlign( mLeftRightFlags|mTopBottomFlags );
                    },
                  true ),
    mTopBottom( 0, [this]( int  iValue )
                    {
                        mTopBottomFlags = iValue;
                        mButton->LabelAlign( mLeftRightFlags|mTopBottomFlags );
                    },
                  true )
{
}


void
cTabText::Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 0;
    int  y = iArea.Y1() + 3;
    int  id = kID_First+10;

    mTopLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {x, y, 240, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "ePIButtonTextAlign"s );  y += 20;

    x = 100;
    y = iArea.Y1() + 25;
    mGUIILeft  = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_InLeft"s,   bPIButtonTextAlign_InLeft,   &mLeftRight );  y += 20;
    mGUIIRight = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_InRight"s,  bPIButtonTextAlign_InRight,  &mLeftRight );  y += 20;
    mGUIOLeft  = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_OutLeft"s,  bPIButtonTextAlign_OutLeft,  &mLeftRight );  y += 20;
    mGUIORight = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_OutRight"s, bPIButtonTextAlign_OutRight, &mLeftRight );  y += 20;

    x = 220;
    y = iArea.Y1() + 25;
    mGUIITop    = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_InTop"s,     bPIButtonTextAlign_InTop,     &mTopBottom );  y += 20;
    mGUIIBottom = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_InBottom"s,  bPIButtonTextAlign_InBottom,  &mTopBottom );  y += 20;
    mGUIOTop    = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_OutTop"s,    bPIButtonTextAlign_OutTop,    &mTopBottom );  y += 20;
    mGUIOBottom = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_OutBottom"s, bPIButtonTextAlign_OutBottom, &mTopBottom );  y += 20;

    mButton = new  nSDK::cTV2ButtonTextString( mWindow, kID_TheButton, {kTheButtonX, kTheButtonY, kTheButtonW, kTheButtonH, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, "Ga Bu Zo Meu"s );

    mWindow->WriteConsole( "<Nothing yet>"s );
}


void
cTabText::Hide()
{
    delete  mTopLabel;     mTopLabel    = nullptr;

    delete  mGUIILeft;     mGUIILeft    = nullptr;
    delete  mGUIIRight;    mGUIIRight   = nullptr;
    delete  mGUIOLeft;     mGUIOLeft    = nullptr;
    delete  mGUIORight;    mGUIORight   = nullptr;

    delete  mGUIITop;      mGUIITop     = nullptr;
    delete  mGUIIBottom;   mGUIIBottom  = nullptr;
    delete  mGUIOTop;      mGUIOTop     = nullptr;
    delete  mGUIOBottom;   mGUIOBottom  = nullptr;

    delete  mButton;       mButton      = nullptr;

    tSuperClass::Hide();
}


//---------------------------------------------------------------------------------------------------------------


nSDK::cTV2Tab*
NewTabText( cDemoWindow*  iWindow )
{
    return  new  cTabText( iWindow );
}

