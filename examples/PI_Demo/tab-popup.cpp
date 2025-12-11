/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-popup.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating popupmenu parameters.
*/


#include "SDK-CPP.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>


//MARK: ========================================================================================================= Popup Tab


constexpr  int  kTheButtonX = 450;
constexpr  int  kTheButtonY = 140;


enum  eIDs
{
    kID_First = 4000,

    kID_TopLabel = kID_First+1,
    kID_ThebuttonLabel = kID_First+2,

    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cLine


class  cLine
{
    enum  eIDOffset
    {
        kIDOffset_On        = 0,
        kIDOffset_ID        = 1,
        kIDOffset_Label     = 2,
        kIDOffset_Separator = 3,
        kIDOffset_Quiet     = 5,
        kIDOffset_Check     = 6,
        kIDOffset_Ghost     = 7,
        kIDOffset_Multi     = 8,
    };

public:
    cLine( int  iIdx )
    {
        mID = iIdx + 1;

        char  tmp[512]; //TODO: use fmt
        snprintf( tmp, sizeof(tmp), "Entry %d", iIdx + 1 );
        mLabel = tmp;

    }
public:
    void  Show( nSDK::cTV2Window*  iWindow, int  iY, int  iStartID );
    void  Hide( nSDK::cTV2Window*  iWindow );

public: // that's OK for demo code, you should use accessor methods in production code...
    nSDK::cTV2Window*  mWindow  {nullptr};

    nSDK::cTV2ButtonCheckBox*     mGUIOn         {nullptr};  bool         mOn         {true};
    nSDK::cTV2ButtonTextInteger*  mGUIID         {nullptr};  int          mID         {0};
    nSDK::cTV2ButtonTextString*   mGUILabel      {nullptr};  std::string  mLabel      {""s};
    nSDK::cTV2ButtonCheckBox*     mGUISeparator  {nullptr};  bool         mSeparator  {false};
    nSDK::cTV2ButtonCheckBox*     mGUIQuiet      {nullptr};  bool         mQuiet      {false};
    nSDK::cTV2ButtonCheckBox*     mGUICheck      {nullptr};  bool         mCheck      {false};
    nSDK::cTV2ButtonCheckBox*     mGUIGhost      {nullptr};  bool         mGhost      {false};
    nSDK::cTV2ButtonCheckBox*     mGUIMulti      {nullptr};  bool         mMulti      {false};
};


void
cLine::Show( nSDK::cTV2Window*  iWindow, int  iY, int  iStartID )
{
    mWindow = iWindow;

    mGUIOn      = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_On, {9, iY, 0, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s,     &mOn );
    mGUIOn->HelpText( "Enable/disable this line"s );

    mGUIID = new  nSDK::cTV2ButtonTextInteger( mWindow, iStartID+kIDOffset_ID, {34, iY,  20, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mID );
    mGUIID->HelpText( "'ID' field"s );

    mGUILabel = new  nSDK::cTV2ButtonTextString( mWindow, iStartID+kIDOffset_Label, {60, iY, 135, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mLabel );
    mGUILabel->HelpText( "'Name' field"s );

    mGUISeparator  = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_Separator, {200, iY, 21, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mSeparator );
    mGUISeparator->HelpText( "'Flags' field : bPIPopMode_Separator"s );
    mGUIQuiet      = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_Quiet, {225, iY, 21, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mQuiet );
    mGUIQuiet->HelpText( "'Flags' field : bPIPopMode_Quiet"s );
    mGUICheck      = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_Check, {250, iY, 21, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mCheck );
    mGUICheck->HelpText( "'Flags' field : bPIPopMode_Check"s );
    mGUIGhost      = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_Ghost, {275, iY, 21, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mGhost );
    mGUIGhost->HelpText( "'Flags' field : bPIPopMode_Ghost"s );
    mGUIMulti      = new  nSDK::cTV2ButtonCheckBox( mWindow, iStartID+kIDOffset_Multi, {300, iY, 21, 0, nSDK::cPixelRectangle::eXYWH::kXYWH}, ""s, &mMulti );
    mGUIMulti->HelpText( "'Flags' field : bPIPopMode_Multi"s );
}


void
cLine::Hide( nSDK::cTV2Window*  iWindow )
{
    delete  mGUIOn;         mGUIOn = nullptr;
    delete  mGUIID;         mGUIID = nullptr;
    delete  mGUILabel;      mGUILabel = nullptr;
    delete  mGUISeparator;  mGUISeparator = nullptr;
    delete  mGUIQuiet;      mGUIQuiet = nullptr;
    delete  mGUICheck;      mGUICheck = nullptr;
    delete  mGUIGhost;      mGUIGhost = nullptr;
    delete  mGUIMulti;      mGUIMulti = nullptr;

    mWindow = nullptr;
}


//MARK: ========================================================================================================= cTabPopup


class  cTabPopup :
    public  nSDK::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabPopup()  override;
    cTabPopup( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Popup"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

private:
    void  DoPopup();

private:
    cDemoWindow*  mWindow;

    std::vector< cLine >        mLines;

    nSDK::cTV2ButtonLabel*     mButtonLabel  {nullptr};
    nSDK::cTV2ButtonPopup*     mButton       {nullptr};
};


cTabPopup::cTabPopup( cDemoWindow*  iWindow ) :
    mWindow( iWindow )
{
    for( int  i = 0; i < 10; ++i )
        mLines.push_back( cLine( i ) );
}


void
cTabPopup::Show( nSDK::cTV2Window*  iWindow, const nSDK::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    for( int  i = 0; i < 10; ++i )
        mLines[i].Show( mWindow, y + i*20, kID_First + 100 + i*10 );

    mButtonLabel = new  nSDK::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;

    mButton      = new  nSDK::cTV2ButtonPopup( mWindow, id++, {kTheButtonX, kTheButtonY, 150, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, "Ga Bu Zo Meu"s, [this]( int  iID ){ DoPopup(); } ); y += 20;
}


void
cTabPopup::Hide()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    for( int  i = 0; i < 10; ++i )
        mLines[i].Hide( mWindow );

    delete  mButtonLabel;  mButtonLabel = nullptr;
    delete  mButton;       mButton      = nullptr;

    TVRemoveButtonReq( plugin, req, kID_TheButton );

    tSuperClass::Hide();
}


void
cTabPopup::DoPopup()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
//    INTPTR  req = mWindow->LegacyReq();

    int  cnt = 0;
    PIPopup  pop[11];

    for( int  i = 0; i < 10; ++i )
    {
        int  f = 0;
        if( !mLines[i].mOn )
            continue;

        if( mLines[i].mSeparator )  f |= bPIPopMode_Separator;
        if( mLines[i].mQuiet )      f |= bPIPopMode_Quiet;
        if( mLines[i].mCheck )      f |= bPIPopMode_Check;
        if( mLines[i].mGhost )      f |= bPIPopMode_Ghost;
        if( mLines[i].mMulti )      f |= bPIPopMode_Multi;

        pop[cnt].Name  = mLines[i].mLabel.c_str();
        pop[cnt].ID    = mLines[i].mID;
        pop[cnt].Flags = f;

        ++cnt;
    }

    int  i = TVPopup( plugin, pop, cnt, 0 );

    char  tmp[256];
    snprintf( tmp, sizeof(tmp), "POPUP SELECTION : %d", i );
    mWindow->WriteConsole( tmp );
}


//---------------------------------------------------------------------------------------------------------------


nSDK::cTV2Tab*
NewTabPopup( cDemoWindow*  iWindow )
{
    return  new  cTabPopup( iWindow );
}

