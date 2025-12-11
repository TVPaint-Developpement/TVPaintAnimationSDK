/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-requester.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating requesters.
*/


#include "SDK-CPP.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>
#include <stdio.h>


class  cTabWindow;


//MARK: ========================================================================================================= Window Tab


constexpr  int  kTheButtonW = 770;
constexpr  int  kTheButtonH = 160;


enum  eIDs
{
    kID_First = 7000,

    kID_Result = kID_First+900,
};


//MARK: ========================================================================================================= cTestWindow


class  cTestWindow :
    public  nSDK::cTV2Window
{
    using  tSuperClass = nSDK::cTV2Window;

public:
    virtual  ~cTestWindow()  override  {}
    cTestWindow( cDemoPlugin*  iPlugin, cTabWindow*  iTab ) : tSuperClass( iPlugin ), mTab( iTab )  {}

public:
///@name  Events
///@{
    virtual  nSDK::tErrorCode  EventWindowOpen( const nSDK::cPixelRectangle&  iArea )  override;
    virtual  nSDK::tErrorCode  EventWindowClose( const nSDK::cPixelRectangle&  iArea, bool  iFinal )  override;

    virtual  nSDK::tErrorCode  EventWindowDragged( const nSDK::cPixelPosition&  iPosition, bool  iDropped )  override;
    virtual  nSDK::tErrorCode  EventWindowResizing( const nSDK::cPixelRectangle&  iArea )  override;
    virtual  nSDK::tErrorCode  EventWindowResized( const nSDK::cPixelRectangle&  iArea )  override;
///@}

private:
    cTabWindow*  mTab  {nullptr};
};


//MARK: ========================================================================================================= cTabWindow


class  cTabWindow :
    public  nSDK::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabWindow()  override  { Hide(); }
    cTabWindow( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Window"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

public:
    void  WriteConsole( const std::string&  iString );
    nSDK::tErrorCode  TestWindowDragged( const nSDK::cPixelPosition&  iPosition, bool  iDropped );
    nSDK::tErrorCode  TestWindowResizing( const nSDK::cPixelRectangle&  iArea );
    nSDK::tErrorCode  TestWindowResized( const nSDK::cPixelRectangle&  iArea );
    PIFlags  Flags() const;
    void  UpdateWindow();

private:
    cDemoWindow*  mWindow;

    nSDK::cTV2ButtonLabel*     mGUITopLabel  {nullptr};

    nSDK::cTV2ButtonCheckBox*  mGUILock     {nullptr};  bool  mLock     {false};
    nSDK::cTV2ButtonCheckBox*  mGUINoDrag   {nullptr};  bool  mNoDrag   {false};
    nSDK::cTV2ButtonCheckBox*  mGUINoDepth  {nullptr};  bool  mNoDepth  {false};
    nSDK::cTV2ButtonCheckBox*  mGUINoClose  {nullptr};  bool  mNoClose  {false};
    nSDK::cTV2ButtonCheckBox*  mGUICenter   {nullptr};  bool  mCenter   {false};
    nSDK::cTV2ButtonCheckBox*  mGUIResize   {nullptr};  bool  mResize   {false};

    nSDK::cTV2ButtonTextInteger*  mGUIX  {nullptr};  int  mX  {50};
    nSDK::cTV2ButtonTextInteger*  mGUIY  {nullptr};  int  mY  {50};
    nSDK::cTV2ButtonTextInteger*  mGUIW  {nullptr};  int  mW  {100};
    nSDK::cTV2ButtonTextInteger*  mGUIH  {nullptr};  int  mH  {100};

    nSDK::cTV2Window*  mTheWindow  {nullptr};

    nSDK::cTV2ButtonLabel*       mGUIResultLabel  {nullptr};
    nSDK::cTV2ButtonConsoleOut*  mGUIResult       {nullptr};

    PIFlags  mOldFlags  {0xFFFFFFFF}; // different from all possible flags combinations
};


//MARK: ========================================================================================================= cTestWindow


nSDK::tErrorCode
cTestWindow::EventWindowOpen( const nSDK::cPixelRectangle&  iArea )
{
    TV2Title( TV2GetLocaleString( 10001, "Demo"s ) );

    new  nSDK::cTV2ButtonAction( this, 1000, {5, 5, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Close"s, [this](int){ TV2CloseWindow(); } );

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cTestWindow::EventWindowClose( const nSDK::cPixelRectangle&  iArea, bool  iFinal )
{
    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cTestWindow::EventWindowDragged( const nSDK::cPixelPosition&  iPosition, bool  iDropped )
{
    return  mTab->TestWindowDragged( iPosition, iDropped );
}


nSDK::tErrorCode
cTestWindow::EventWindowResizing( const nSDK::cPixelRectangle&  iArea )
{
    return  mTab->TestWindowResizing( iArea );
}


nSDK::tErrorCode
cTestWindow::EventWindowResized( const nSDK::cPixelRectangle&  iArea )
{
    return  mTab->TestWindowResized( iArea );
}


//MARK: ========================================================================================================= cTabWindow


cTabWindow::cTabWindow( cDemoWindow*  iWindow ) :
    mWindow( iWindow )
{
}


void
cTabWindow::Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 140;
    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    mGUITopLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {0, y, 360, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "ePIRequesterFlags"s ); y += 20;

    x = 100;
    y = starty + 25;

    mGUILock    = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Lock"s,    &mLock,    [this](int,bool){ UpdateWindow(); } ); y += 20;
    mGUINoDrag  = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_NoDrag"s,  &mNoDrag,  [this](int,bool){ UpdateWindow(); } ); y += 20;
    mGUINoDepth = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_NoDepth"s, &mNoDepth, [this](int,bool){ UpdateWindow(); } ); y += 20;
    mGUINoClose = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_NoClose"s, &mNoClose, [this](int,bool){ UpdateWindow(); } ); y += 20;
    mGUICenter  = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Center"s,  &mCenter,  [this](int,bool){ UpdateWindow(); } ); y += 20;
    mGUIResize  = new  nSDK::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "*_Resize"s,  &mResize,  [this](int,bool){ UpdateWindow(); } ); y += 20;

    x = 200;
    y = starty + 25;

    mGUIX = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "X"s, &mX, 20, 800 );  y += 20;
    mGUIY = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Y"s, &mY, 20, 800 );  y += 20;
    mGUIW = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "W"s, &mW, 50, 400 );  y += 20;
    mGUIH = new  nSDK::cTV2ButtonTextInteger( mWindow, id++, {x, y, 40, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, "H"s, &mH, 30, 400 );  y += 20;

    x = 9;
    y += 20;

    mGUIResultLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {x, y, 150, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;
    mGUIResult      = new  nSDK::cTV2ButtonConsoleOut( mWindow, kID_Result, {x, y, kTheButtonW, kTheButtonH, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    mGUIResult->DrawString( "<Nothing yet>"s );

    cDemoPlugin*  demoplugin = dynamic_cast< cDemoPlugin* >( mWindow->Plugin() );

    mTheWindow = new  cTestWindow( demoplugin, this );
    mTheWindow = mTheWindow->TV2OpenWindow( demoplugin, mTheWindow, {mX, mY, mW, mH, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Demo"s, Flags() );

}


void
cTabWindow::Hide()
{
    delete  mGUITopLabel;  mGUITopLabel = nullptr;

    delete  mGUILock;     mGUILock     = nullptr;
    delete  mGUINoDrag;   mGUINoDrag   = nullptr;
    delete  mGUINoDepth;  mGUINoDepth  = nullptr;
    delete  mGUINoClose;  mGUINoClose  = nullptr;
    delete  mGUICenter;   mGUICenter   = nullptr;
    delete  mGUIResize;   mGUIResize   = nullptr;

    delete  mGUIX;  mGUIX = nullptr;
    delete  mGUIY;  mGUIY = nullptr;
    delete  mGUIW;  mGUIW = nullptr;
    delete  mGUIH;  mGUIH = nullptr;

    delete  mGUIResultLabel;  mGUIResultLabel = nullptr;
    delete  mGUIResult;       mGUIResult      = nullptr;

    delete  mTheWindow;  mTheWindow = nullptr;

    tSuperClass::Hide();
}


void
cTabWindow::WriteConsole( const std::string&  iString )
{
    mGUIResult->DrawString( iString );
}


nSDK::tErrorCode
cTabWindow::TestWindowDragged( const nSDK::cPixelPosition&  iPosition, bool  iDropped )
{
    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "EventWindowDragged( iPosition={%d,%d}, iDropped=%s )", iPosition.X(), iPosition.Y(), iDropped?"true":"false" );
    WriteConsole( tmp );

    mGUIX->Value( iPosition.X() );
    mGUIY->Value( iPosition.Y() );
    if( mX != iPosition.X()  ||  mY != iPosition.Y() )
    {
        snprintf( tmp, sizeof(tmp), ">>> Limited to : x=%d, y=%d", mX, mY );
        WriteConsole( tmp );
        mTheWindow->Area( {mX, mY, mW, mH, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    }

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cTabWindow::TestWindowResizing( const nSDK::cPixelRectangle&  iArea )
{
    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "TestWindowResizing( iArea={%d,%d,%d,%d} )", iArea.X(), iArea.Y(), iArea.W(), iArea.H() );
    WriteConsole( tmp );

    mGUIW->Value( iArea.W() );
    mGUIH->Value( iArea.H() );

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cTabWindow::TestWindowResized( const nSDK::cPixelRectangle&  iArea )
{
    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "TestWindowResized( iArea={%d,%d,%d,%d} )", iArea.X(), iArea.Y(), iArea.W(), iArea.H() );
    WriteConsole( tmp );

    mGUIW->Value( iArea.W() );
    mGUIH->Value( iArea.H() );
    if( mW != iArea.W()  ||  mH != iArea.H() )
    {
        snprintf( tmp, sizeof(tmp), ">>> Limited to : w=%d, h=%d", mW, mH );
        WriteConsole( tmp );
        mTheWindow->Area( {mX, mY, mW, mH, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    }

    return  nSDK::kErrorCode_NoError;
}



PIFlags
cTabWindow::Flags() const
{
    PIFlags  flags = 0;

    if( mLock    )  flags |= bPIRequesterFlags_Modal;
    if( mNoDrag  )  flags |= bPIRequesterFlags_NoDrag;
    if( mNoDepth )  flags |= bPIRequesterFlags_NoDepth; // a deprecated warning here is normal....
    if( mNoClose )  flags |= bPIRequesterFlags_NoClose;
    if( mCenter  )  flags |= bPIRequesterFlags_Center;
    if( mResize  )  flags |= bPIRequesterFlags_Resize;

    return  flags;
}


void
cTabWindow::UpdateWindow()
{
    PIFlags  flags = Flags();

    if( flags == mOldFlags )
    {
        if( mTheWindow )
            mTheWindow->Area( {mX, mY, mW, mH, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    }
    else
    {
        cDemoPlugin*  demoplugin = dynamic_cast< cDemoPlugin* >( mWindow->Plugin() );

        //TODO: maybe make an UpdateFlags() method in window ?
        mTheWindow->TV2CloseWindow();
        mTheWindow = mTheWindow->TV2OpenWindow( demoplugin, mTheWindow, {mX, mY, mW, mH, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Demo"s, flags );

        mOldFlags = flags;
    }
}


//---------------------------------------------------------------------------------------------------------------


nSDK::cTV2Tab*
NewTabWindow( cDemoWindow*  iWindow )
{
    return  new  cTabWindow( iWindow );
}


#if 0
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Req Subwindow


#define THE_BUTTON_X   350
#define THE_BUTTON_Y    70
#define THE_BUTTON_W   500
#define THE_BUTTON_H   180

#define START                   7000


#define ID_TOP_LABEL            (START+001)
#define ID_THE_BUTTON_LABEL     (START+002)

#define ID_LOCK_REQ             (START+100)
#define ID_NO_DRAG_REQ          (START+101)
#define ID_NO_DEPTH_REQ         (START+102)
#define ID_NO_CLOSE_REQ         (START+103)
#define ID_CENTER_REQ           (START+104)
#define ID_RESIZE_REQ           (START+105)

#define ID_X                    (START+200)
#define ID_Y                    (START+201)
#define ID_W                    (START+202)
#define ID_H                    (START+203)

#define ID_THE_BUTTON           (START+999)

#define ID_CLOSE                (START+500)


static struct
{
    INTPTR  mReq;

    INTPTR  mBigReq;

    int  mX;
    int  mY;
    int  mW;
    int  mH;

    int  mLockReq;
    int  mNoDragReq;
    int  mNoDepthReq;
    int  mNoCloseReq;
    int  mCenterReq;
    int  mResizeReq;

    int  mOldFlags;
} sgToggles;


static int ReqMsg( PIFilter* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs );


static void
MakeButton( PIFilter* iFilter, INTPTR iReq )
{
    int  f = 0;

    if( sgToggles.mLockReq    )  f |= bPIRequesterFlags_Modal;
    if( sgToggles.mNoDragReq  )  f |= bPIRequesterFlags_NoDrag;
    if( sgToggles.mNoDepthReq )  f |= bPIRequesterFlags_NoDepth;
    if( sgToggles.mNoCloseReq )  f |= bPIRequesterFlags_NoClose;
    if( sgToggles.mCenterReq  )  f |= bPIRequesterFlags_Center;
    if( sgToggles.mResizeReq  )  f |= bPIRequesterFlags_Resize;

    if( f == sgToggles.mOldFlags )
    {
        if( sgToggles.mReq )
        {
            TVResizeReq( iFilter, sgToggles.mReq, sgToggles.mX, sgToggles.mY, sgToggles.mW, sgToggles.mH );
        }
    }
    else
    {
        if( sgToggles.mReq )
        {
            TVCloseReq( iFilter, sgToggles.mReq );
        }

        sgToggles.mReq = TVOpenReqEx( iFilter,
                                      sgToggles.mW, sgToggles.mH, sgToggles.mX, sgToggles.mY,
                                      f, "DEMO !!!", ReqMsg );
        sgToggles.mOldFlags = f;
    }
}


static int
ReqMsg( PIFilter* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    char  tmp[256];

    switch( iEvent )
    {
    case PICBREQ_BUTTON_UP:
        switch( iArgs[0] )
        {
        case ID_CLOSE:
            TVCloseReq( iFilter, iReq );
            sgToggles.mOldFlags = 1111111111; // != from all possible flag combinations
            sgToggles.mReq = 0;
            break;
        }
        break;

    case kPIEvents_WindowOpen://xywh
        sgToggles.mX = (int)iArgs[0];
        sgToggles.mY = (int)iArgs[1];
        sgToggles.mW = (int)iArgs[2];
        sgToggles.mH = (int)iArgs[3];
        snprintf( tmp, sizeof(tmp), "kPIEvents_WindowOpen( x=%d, y=%d, w=%d, h=%d )", sgToggles.mX, sgToggles.mY, sgToggles.mW, sgToggles.mH );
        mWindow->WriteConsole( tmp );

        TVAddButtonReq( iFilter, iReq, 5, 5, 40, 0, ID_CLOSE, bPIButtonFlags_Action, "Close" );
        break;

    case PICBREQ_CLOSE://xywhf
        sgToggles.mX = (int)iArgs[0];
        sgToggles.mY = (int)iArgs[1];
        sgToggles.mW = (int)iArgs[2];
        sgToggles.mH = (int)iArgs[3];
        snprintf( tmp, sizeof(tmp), "PICBREQ_CLOSE( x=%d, y=%d, w=%d, h=%d )", sgToggles.mX, sgToggles.mY, sgToggles.mW, sgToggles.mH );
        mWindow->WriteConsole( tmp );
        break;

    case PICBREQ_DRAG://xy
        sgToggles.mX = (int)iArgs[0];
        sgToggles.mY = (int)iArgs[1];
        snprintf( tmp, sizeof(tmp), "PICBREQ_DRAG( x=%d, y=%d )", sgToggles.mX, sgToggles.mY );
        mWindow->WriteConsole( tmp );
        if( sgToggles.mX < 20  ||  sgToggles.mX > 800  ||  sgToggles.mY < 20  ||  sgToggles.mY > 800 )
        {
            if( sgToggles.mX <  20 ) sgToggles.mX =  20;
            if( sgToggles.mX > 800 ) sgToggles.mX = 800;
            if( sgToggles.mY <  20 ) sgToggles.mY =  20;
            if( sgToggles.mY > 800 ) sgToggles.mY = 800;
            snprintf( tmp, sizeof(tmp), ">>> Limited to : x=%d, y=%d", sgToggles.mX, sgToggles.mY );
            mWindow->WriteConsole( tmp );
            TVResizeReq( iFilter, iReq, sgToggles.mX, sgToggles.mY, sgToggles.mW, sgToggles.mH );
        }

        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mX );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_X, tmp );
        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mY );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_Y, tmp );
        break;

    case PICBREQ_RESIZE://whxy
        sgToggles.mW = (int)iArgs[0];
        sgToggles.mH = (int)iArgs[1];
        sgToggles.mX = (int)iArgs[2];
        sgToggles.mY = (int)iArgs[3];
        snprintf( tmp, sizeof(tmp), "PICBREQ_RESIZE( w=%d, h=%d, x=%d, y=%d )", sgToggles.mW, sgToggles.mH, sgToggles.mX, sgToggles.mY );
        mWindow->WriteConsole( tmp );
        if( sgToggles.mW < 50  ||  sgToggles.mW > 400  ||  sgToggles.mH < 30  ||  sgToggles.mH > 400 )
        {
            if( sgToggles.mW <  50 ) sgToggles.mW = 50;
            if( sgToggles.mW > 400 ) sgToggles.mW = 400;
            if( sgToggles.mH <  30 ) sgToggles.mH = 30;
            if( sgToggles.mH > 400 ) sgToggles.mH = 400;
            snprintf( tmp, sizeof(tmp), ">>> Limited to : w=%d, h=%d", sgToggles.mW, sgToggles.mH );
            mWindow->WriteConsole( tmp );
        }

        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mX );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_X, tmp );
        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mY );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_Y, tmp );
        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mW );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_W, tmp );
        snprintf( tmp, sizeof(tmp), "%d", sgToggles.mH );
        TVPutButtonString( iFilter, sgToggles.mBigReq, ID_H, tmp );

        TVResizeReq( iFilter, iReq, sgToggles.mX, sgToggles.mY, sgToggles.mW, sgToggles.mH );
        break;
    }

    return  1;
}


static void
AddTextField( PIFilter* iFilter, INTPTR iReq, int iX, int iY, int iID, const char* iLabel, int iVal )
{
    char  tmp[256];

    TVAddButtonReq( iFilter, iReq, iX-5,  iY,  1, 0, iID+150, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, iLabel );
    TVChangeButtonName( iFilter, iReq, iID+150, iLabel, bPIButtonTextAlign_OutLeft );

    snprintf( tmp, sizeof(tmp), "%d", iVal );
    TVAddButtonReq( iFilter, iReq, iX,    iY, 40, 0, iID,     bPIButtonFlags_TextInteger, tmp );

    TVAddButtonReq( iFilter, iReq, iX+45, iY,  0, 0, iID+100, bPIButtonFlags_Minislider, NULL );
}


static void
DoTextField( PIFilter* iFilter, INTPTR iReq, int iID, int* ioVal, int iMin, int iMax )
{
    char  tmp[256];

    TVGetButtonString( iFilter, iReq, iID, tmp, 255 );
    sscanf( tmp, "%d", ioVal );
    if( *ioVal < iMin )
    {
        *ioVal = iMin;
        snprintf( tmp, sizeof(tmp), "%d", *ioVal );
        TVPutButtonString( iFilter, iReq, iID, tmp );
    }
    if( *ioVal > iMax )
    {
        *ioVal = iMax;
        snprintf( tmp, sizeof(tmp), "%d", *ioVal );
        TVPutButtonString( iFilter, iReq, iID, tmp );
    }

    MakeButton( iFilter, iReq );
}


static void
Open( PIFilter* iFilter, INTPTR iReq )
{
    int  x = 140;
    int  y = gStartSubWindowY;

    static int  first = 1;
    if( first )
    {
        first = 0;
        sgToggles.mReq = 0;

        sgToggles.mX = 50;
        sgToggles.mY = 50;
        sgToggles.mW = 100;
        sgToggles.mH = 100;

        sgToggles.mLockReq    = 0;
        sgToggles.mNoDragReq  = 0;
        sgToggles.mNoDepthReq = 0;
        sgToggles.mNoCloseReq = 0;
        sgToggles.mCenterReq  = 0;
        sgToggles.mResizeReq  = 0;

        sgToggles.mOldFlags = 1111111111; // != from all possible flag combinations
    }
    sgToggles.mBigReq = iReq;

    TVAddButtonReq( iFilter, iReq, 0, y, 200, 20, ID_TOP_LABEL, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, "ePIRequesterFlags" ); y += 20;

    x = 140;
    y = gStartSubWindowY + 25;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_LOCK_REQ,     bPIButtonFlags_Check|(sgToggles.mLockReq?     bPIButtonFlags_Select:0), "*_Lock"    ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_NO_DRAG_REQ,  bPIButtonFlags_Check|(sgToggles.mNoDragReq?   bPIButtonFlags_Select:0), "*_NoDrag"  ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_NO_DEPTH_REQ, bPIButtonFlags_Check|(sgToggles.mNoDepthReq?  bPIButtonFlags_Select:0), "*_NoDepth (Deprecated)" ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_NO_CLOSE_REQ, bPIButtonFlags_Check|(sgToggles.mNoCloseReq?  bPIButtonFlags_Select:0), "*_NoClose" ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_CENTER_REQ,   bPIButtonFlags_Check|(sgToggles.mCenterReq?   bPIButtonFlags_Select:0), "*_Center"  ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_RESIZE_REQ,   bPIButtonFlags_Check|(sgToggles.mResizeReq?   bPIButtonFlags_Select:0), "*_Resize"  ); y += 20;

    x = 230;
    y = gStartSubWindowY + 25;
    AddTextField( iFilter, iReq, x, y, ID_X, "X",      sgToggles.mX );  y += 20;
    AddTextField( iFilter, iReq, x, y, ID_Y, "Y",      sgToggles.mY );  y += 20;
    AddTextField( iFilter, iReq, x, y, ID_W, "Width",  sgToggles.mW );  y += 20;
    AddTextField( iFilter, iReq, x, y, ID_H, "Height", sgToggles.mH );  y += 20;

    TVAddButtonReq( iFilter, iReq, THE_BUTTON_X, THE_BUTTON_Y-30, 150, 20, ID_THE_BUTTON_LABEL, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, "What you get ..." ); y += 20;
    TVAddButtonReq( iFilter, iReq, THE_BUTTON_X, THE_BUTTON_Y, THE_BUTTON_W, THE_BUTTON_H, ID_THE_BUTTON, bPIButtonFlags_Invert, "" );
    TVPutButtonImage( iFilter, iReq, ID_THE_BUTTON, TVAllocPIBlock( iFilter, THE_BUTTON_W, THE_BUTTON_H, 0 ), 0 );
    mWindow->WriteConsole( "<Nothing yet>"s );

    MakeButton( iFilter, iReq );
}


static void
Close( PIFilter* iFilter, INTPTR iReq )
{
    TVRemoveButtonReq( iFilter, iReq, ID_TOP_LABEL );

    TVRemoveButtonReq( iFilter, iReq, ID_LOCK_REQ );
    TVRemoveButtonReq( iFilter, iReq, ID_NO_DRAG_REQ );
    TVRemoveButtonReq( iFilter, iReq, ID_NO_DEPTH_REQ );
    TVRemoveButtonReq( iFilter, iReq, ID_NO_CLOSE_REQ );
    TVRemoveButtonReq( iFilter, iReq, ID_CENTER_REQ );
    TVRemoveButtonReq( iFilter, iReq, ID_RESIZE_REQ );

    RemoveTextField( iFilter, iReq, ID_X );
    RemoveTextField( iFilter, iReq, ID_Y );
    RemoveTextField( iFilter, iReq, ID_W );
    RemoveTextField( iFilter, iReq, ID_H );

    TVRemoveButtonReq( iFilter, iReq, ID_THE_BUTTON_LABEL );
    TVRemoveButtonReq( iFilter, iReq, ID_THE_BUTTON );

    if( sgToggles.mReq )
    {
        TVCloseReq( iFilter, sgToggles.mReq );
        sgToggles.mOldFlags = 1111111111; // != from all possible flag combinations
        sgToggles.mReq = 0;
    }
}


static void
Click( PIFilter* iFilter, INTPTR iReq, int iWhich, int* ioBool )
{
    *ioBool = !*ioBool;

    TVChangeButtonReq( iFilter, iReq, iWhich, bPIButtonFlags_Check|(*ioBool?bPIButtonFlags_Select:0), NULL );
    MakeButton( iFilter, iReq );
}


static int
Msg( PIFilter* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    int  id = iArgs ? (int)iArgs[0] : 0;

    switch( iEvent )
    {
        case PICBREQ_BUTTON_UP:
        {
            switch( iArgs[0] )
            {
            case ID_LOCK_REQ:
                Click( iFilter, iReq, id, &sgToggles.mLockReq );
                return  1;
            case ID_NO_DRAG_REQ:
                Click( iFilter, iReq, id, &sgToggles.mNoDragReq );
                return  1;
            case ID_NO_DEPTH_REQ:
                Click( iFilter, iReq, id, &sgToggles.mNoDepthReq );
                return  1;
            case ID_NO_CLOSE_REQ:
                Click( iFilter, iReq, id, &sgToggles.mNoCloseReq );
                return  1;
            case ID_CENTER_REQ:
                Click( iFilter, iReq, id, &sgToggles.mCenterReq );
                return  1;
            case ID_RESIZE_REQ:
                Click( iFilter, iReq, id, &sgToggles.mResizeReq );
                return  1;

            case ID_X:
                DoTextField( iFilter, iReq, id, &sgToggles.mX, 20, 800 );
                return  1;
            case ID_Y:
                DoTextField( iFilter, iReq, id, &sgToggles.mY, 20, 800 );
                return  1;
            case ID_W:
                DoTextField( iFilter, iReq, id, &sgToggles.mW, 100, 400 );
                return  1;
            case ID_H:
                DoTextField( iFilter, iReq, id, &sgToggles.mH, 100, 400 );
                return  1;
            }
        }
        break;

        case PICBREQ_MINISLIDER:
        {
            int  delta = (int)iArgs[1];

            switch( iArgs[0] )
            {
            case ID_X+100:
                DoMiniSlider( iFilter, iReq, id, delta, &sgToggles.mX, 20, 800 );
                return  1;
            case ID_Y+100:
                DoMiniSlider( iFilter, iReq, id, delta, &sgToggles.mY, 20, 800 );
                return  1;
            case ID_W+100:
                DoMiniSlider( iFilter, iReq, id, delta, &sgToggles.mW, 50, 400 );
                return  1;
            case ID_H+100:
                DoMiniSlider( iFilter, iReq, id, delta, &sgToggles.mH, 30, 400 );
                return  1;
            }
        }
        break;
    }

    return  0;
}




DemoWindow  gReqWindow =
{
    Open,
    Close,
    Msg
};
#endif//0

