/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-meta.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating meta/hud access.
*/


#include "SDK2.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>


//MARK: ========================================================================================================= Meta Tab


constexpr  int  kTheButtonX = 350;
constexpr  int  kTheButtonY = 70;
constexpr  int  kTheButtonW = 500;
constexpr  int  kTheButtonH = 180;


enum  eIDs
{
    kID_First = 6000,

    kID_TopLabel = kID_First+1,
    kID_TheButtonLabel = kID_First+2,
    
    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cTabMeta


class  cTabMeta :
    public  nSDK2::cTV2Tab,
    public  nSDK2::cTV2MetaEventHandlingMixIn
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabMeta()  override;
    cTabMeta( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Meta"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

public:
    virtual  nSDK2::tErrorCode  HandleMetaEvent( INTPTR  iEvent, INTPTR*  iArgs )  override;

private:
    cDemoWindow*  mWindow;
    nSDK2::cTV2RadioGroup  mMetaFunction;

    nSDK2::cTV2ButtonLabel*  mTopLabel  {nullptr};

    nSDK2::cTV2ButtonRadio*     mGUIDot              {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUIDraw             {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUILine             {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUIRectangle        {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUICircle           {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUIEllipse          {nullptr};
    nSDK2::cTV2ButtonRadio*     mGUISpline           {nullptr};

    nSDK2::cTV2ButtonCheckBox*  mGUIPreview          {nullptr};  bool  mPreview          {false};

    nSDK2::cTV2ButtonCheckBox*  mGUIPreviewBrush     {nullptr};  bool  mPreviewBrush     {false};
    nSDK2::cTV2ButtonCheckBox*  mGUINoInterpolation  {nullptr};  bool  mNoInterpolation  {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIRefreshPreview   {nullptr};  bool  mRefreshPreview   {false};

    nSDK2::cTV2ButtonLabel*  mButtonLabel  {nullptr};
    nSDK2::cTV2ButtonConsoleOut*  mButton       {nullptr};
};


cTabMeta::cTabMeta( cDemoWindow*  iWindow ) :
    mWindow( iWindow ),
    mMetaFunction( 4, [this]( int  iValue )
                    { 
                        PIFlags  flags = 0;
                        flags |= mPreviewBrush    ? bPIMeta_PreviewBrush    : 0;
                        flags |= mNoInterpolation ? bPIMeta_NoInterpolation : 0;
                        flags |= mRefreshPreview  ? bPIMeta_RefreshPreview  : 0;

                        mWindow->DoMeta( iValue, flags );
                    },
                  true )
{
}


void
cTabMeta::Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );
    
    //    PIFilter*  plugin = mWindow->Plugin()->LegacyPlugin();
    //    INTPTR  req = mWindow->LegacyReq();
    
    int  x = 0;
    int  y = iArea.Y1() + 3;
    int  id = kID_First+10;
    
    mTopLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {x, y, 340, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "ePIMetaFlags"s );  y += 20;
    
    x = 80;
    y = iArea.Y1() + 25;
    mGUIDot       = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Dot"s,       kPIMeta_Dot,       &mMetaFunction );  y += 20;
    mGUIDraw      = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Draw"s,      kPIMeta_Draw,      &mMetaFunction );  y += 20;
    mGUILine      = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Line"s,      kPIMeta_Line,      &mMetaFunction );  y += 20;
    mGUIRectangle = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Rectangle"s, kPIMeta_Rectangle, &mMetaFunction );  y += 20;
    mGUICircle    = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Circle"s,    kPIMeta_Circle,    &mMetaFunction );  y += 20;
    mGUIEllipse   = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Ellipse"s,   kPIMeta_Ellipse,   &mMetaFunction );  y += 20;
    mGUISpline    = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Spline"s,    kPIMeta_Spline,    &mMetaFunction );  y += 20;
    
    y += 10;
    mGUIPreview   = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Preview"s,   &mPreview );    y += 20;
    
    x = 280;
    y = iArea.Y1() + 25;
    mGUIPreviewBrush    = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_PreviewBrush"s,    &mPreviewBrush );     y += 20;
    mGUINoInterpolation = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_NoInterpolation"s, &mNoInterpolation );  y += 20;
    mGUIRefreshPreview  = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_RefreshPreview"s,  &mRefreshPreview );   y += 20;
    
    mButtonLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;
    mButton      = new  nSDK2::cTV2ButtonConsoleOut( mWindow, kID_TheButton, {kTheButtonX, kTheButtonY, kTheButtonW, kTheButtonH, nSDK2::cPixelRectangle::eXYWH::kXYWH} );

    mButton->DrawString( "<Nothing yet>"s );
}


void
cTabMeta::Hide()
{
    delete  mTopLabel;            mTopLabel           = nullptr;

    delete  mGUIDot;              mGUIDot             = nullptr;
    delete  mGUIDraw;             mGUIDraw            = nullptr;
    delete  mGUILine;             mGUILine            = nullptr;
    delete  mGUIRectangle;        mGUIRectangle       = nullptr;
    delete  mGUICircle;           mGUICircle          = nullptr;
    delete  mGUIEllipse;          mGUIEllipse         = nullptr;
    delete  mGUISpline;           mGUISpline          = nullptr;

    delete  mGUIPreview;          mGUIPreview         = nullptr;

    delete  mGUIPreviewBrush;     mGUIPreviewBrush    = nullptr;
    delete  mGUINoInterpolation;  mGUINoInterpolation = nullptr;
    delete  mGUIRefreshPreview;   mGUIRefreshPreview  = nullptr;

    delete  mButtonLabel;         mButtonLabel        = nullptr;
    delete  mButton;              mButton             = nullptr;

    tSuperClass::Hide();
}


#ifndef  ARG2FLOAT
#define  ARG2FLOAT(x)  (((int)iArgs[x])/65536.)
#endif


//TODO: some way to get this called !!!!!
nSDK2::tErrorCode
cTabMeta::HandleMetaEvent( INTPTR  iEvent, INTPTR*  iArgs )
{
    char  tmp[256];

    switch( iEvent )
    {
        case  kPIEvents_MetaClosed:
            snprintf( tmp, sizeof(tmp), "PICMETA_CLOSE" );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaPoint:
            snprintf( tmp, sizeof(tmp), "PICMETA_POINT( fx=%7.2lf, fy=%7.2lf, button=%d, pressure=%d)", ARG2FLOAT(0), ARG2FLOAT(1), int(iArgs[2]), int(iArgs[3]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaFreehand:
            snprintf( tmp, sizeof(tmp), "PICMETA_DRAW( fx=%7.2lf, fy=%7.2lf, button=%d, pressure=%d)", ARG2FLOAT(0), ARG2FLOAT(1), int(iArgs[2]), int(iArgs[3]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaLine:
            snprintf( tmp, sizeof(tmp), "PICMETA_LINE( ofx=%7.2lf, ofy=%7.2lf, fx=%7.2lf, fy=%7.2lf, button=%d)", ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), int(iArgs[4]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaRectangle:
            snprintf( tmp, sizeof(tmp), "PICMETA_RECT( ofx=%7.2lf, ofy=%7.2lf, fx=%7.2lf, fy=%7.2lf, button=%d)", ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), int(iArgs[4]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaCircle:
            snprintf( tmp, sizeof(tmp), "PICMETA_CIRCLE( ofx=%7.2lf, ofy=%7.2lf, fr=%7.2lf, button=%d)", ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), int(iArgs[3]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaEllipse:
            snprintf( tmp, sizeof(tmp), "PICMETA_ELLIPSE( ofx=%7.2lf, ofy=%7.2lf, fa=%7.2lf, fb=%7.2lf, button=%d)", ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), int(iArgs[4]) );
            mButton->DrawString( tmp );
            break;

        case  kPIEvents_MetaPreview:
            snprintf( tmp, sizeof(tmp), "PICMETA_PREVIEW( x=%d, y=%d, w=%d, h=%d)", int(iArgs[0]), int(iArgs[1]), int(iArgs[2]), int(iArgs[3]) );
            mButton->DrawString( tmp );
            break;
    }

    return  nSDK2::kErrorCode_NoError;
}


//---------------------------------------------------------------------------------------------------------------


nSDK2::cTV2Tab*
NewTabMeta( cDemoWindow*  iWindow )
{
    return  new  cTabMeta( iWindow );
}

