/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-meta.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating meta/hud access.
*/


#include "TVPaintAnimationSDK/TVPaintSDK.hpp"


#include "TVPaintAnimationSDK/TVPaintSDK.h"


#include "demo.h"



//MARK: ========================================================================================================= Project Tab


constexpr  int  kTheButtonX = 350;
constexpr  int  kTheButtonY = 70;
constexpr  int  kTheButtonW = 500;
constexpr  int  kTheButtonH = 180;


enum  eIDs
{
    kID_First = 8000,

    kID_TopLabel = kID_First+1,
    kID_TheButtonLabel = kID_First+2,

    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cTabProject


class  cTabProject :
    public  nSDK::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabProject()  override;
    cTabProject( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Project"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public: // events
//    virtual  tErrorCode  EventProjectChanged()  override;

public:
    virtual  void  Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

public:
    void  UpdatePreview();

private:
    cDemoWindow*  mWindow;
    nSDK::cTV2RadioGroup  mReadLayer;

    nSDK::cTV2ButtonLabel*  mTopLabel  {nullptr};

    nSDK::cTV2ButtonRadio*  mGUIBrush    {nullptr};
    nSDK::cTV2ButtonRadio*  mGUIDisplay  {nullptr};
    nSDK::cTV2ButtonRadio*  mGUIUndo     {nullptr};
    nSDK::cTV2ButtonRadio*  mGUISpare    {nullptr};
    nSDK::cTV2ButtonRadio*  mGUICurrent  {nullptr};

    nSDK::cTV2ButtonTextInteger*  mGUIX  {nullptr};  int  mX  {0};
    nSDK::cTV2ButtonTextInteger*  mGUIY  {nullptr};  int  mY  {0};
    nSDK::cTV2ButtonTextInteger*  mGUIW  {nullptr};  int  mW  {10};
    nSDK::cTV2ButtonTextInteger*  mGUIH  {nullptr};  int  mH  {10};

    nSDK::cTV2ButtonLabel*  mButtonLabel  {nullptr};
    nSDK::cTV2ButtonImage*  mButton       {nullptr};
};


cTabProject::cTabProject( cDemoWindow*  iWindow ) :
    mWindow( iWindow ),
    mReadLayer( kPIReadLayer_Current, [this]( int  iValue )
                    {
                        UpdatePreview();
                    },
                  true )
{
}


//---------------------------------------------------------------------------------------------------------------


//TODO: !!!!!!!!
//tErrorCode
//cTabProject::EventProjectChanged()
//{
//    UpdatePreview();
//}


//---------------------------------------------------------------------------------------------------------------


void
cTabProject::Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 0;
    int  y = iArea.Y1() + 3;
    int  id = kID_First+10;

    mTopLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {x, y, 340, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "ePIReadLayer"s );  y += 20;

    x = 80;
    y = iArea.Y1() + 25;
    mGUIBrush   = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Brush"s,   kPIReadLayer_Brush,   &mReadLayer );  y += 20;
    mGUIDisplay = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Display"s, kPIReadLayer_Display, &mReadLayer );  y += 20;
    mGUIUndo    = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Undo"s,    kPIReadLayer_Undo,    &mReadLayer );  y += 20;
    mGUISpare   = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Spare"s,   kPIReadLayer_Spare,   &mReadLayer );  y += 20;
    mGUICurrent = new  nSDK::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Current"s, kPIReadLayer_Current, &mReadLayer );  y += 20;

    x = 230;
    y = iArea.Y1() + 25;
    mGUIX = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mX,  0, mWindow->Plugin()->WorkArea().W(), [this]( int, int ){ UpdatePreview(); }, true );
    mGUIY = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mY,  0, mWindow->Plugin()->WorkArea().W(), [this]( int, int ){ UpdatePreview(); }, true );
    mGUIW = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mW, 10, kTheButtonW, [this]( int, int ){ UpdatePreview(); }, true );
    mGUIH = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mH, 10, kTheButtonH, [this]( int, int ){ UpdatePreview(); }, true );


    mButtonLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;

    mButton      = new  nSDK::cTV2ButtonImage( mWindow, kID_TheButton, {kTheButtonX, kTheButtonY, kTheButtonW, kTheButtonH, nSDK::cPixelRectangle::eXYWH::kXYWH} );

    UpdatePreview();

    mWindow->WriteConsole( "<Nothing yet>"s );
}


void
cTabProject::Hide()
{
    delete  mTopLabel;  mTopLabel = nullptr;

    delete  mGUIBrush;    mGUIBrush   = nullptr;
    delete  mGUIDisplay;  mGUIDisplay = nullptr;
    delete  mGUIUndo;     mGUIUndo    = nullptr;
    delete  mGUISpare;    mGUISpare   = nullptr;
    delete  mGUICurrent;  mGUICurrent = nullptr;

    delete  mGUIX;  mGUIX = nullptr;
    delete  mGUIY;  mGUIY = nullptr;
    delete  mGUIW;  mGUIW = nullptr;
    delete  mGUIH;  mGUIH = nullptr;

    delete  mButtonLabel;  mButtonLabel = nullptr;
    delete  mButton;       mButton      = nullptr;

    tSuperClass::Hide();
}


//---------------------------------------------------------------------------------------------------------------


void
cTabProject::UpdatePreview()
{
    ePIReadLayer  readlayer = ePIReadLayer(mReadLayer.Value());

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    PIBlock*  block = mButton->Image();

    if( !block )
    {
        mWindow->WriteConsole( "No block !!"s );
        return;
    }

    // we don't need to check validity 'coz m{W,H} are bound by the block size...
    int  x = (block->Width-mW)/2;
    int  y = (block->Height-mH)/2;

    // clear the block;
    memset( block->Data, 0, block->Width*block->Height*sizeof(PIPixel32) );

    int  ret = TVReadLayerData( plugin, block->Data + x + y*block->Width,
                                mX, mY, mW, mH,
                                block->Width - mW,
                                readlayer );
    if( !ret )
        mWindow->WriteConsole( "Nothing read !!"s );

    mButton->RefreshImage();
}


//---------------------------------------------------------------------------------------------------------------


nSDK::cTV2Tab*
NewTabProject( cDemoWindow*  iWindow )
{
    return  new  cTabProject( iWindow );
}

