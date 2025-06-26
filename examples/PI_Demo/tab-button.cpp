/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-button.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating button parameters.
*/


#include "SDK2.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>


//MARK: ======================================================================================================== Button Tab


constexpr  int  kTheButtonX = 450;
constexpr  int  kTheButtonY = 140;


enum  eIDs
{
    kID_First = 2000,

    kID_TheButton = kID_First+900,
};


//MARK: ========================================================================================================= cTabButton


class  cTabButton :
    public  nSDK2::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabButton()  override  { Hide(); }
    cTabButton( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Button"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

private:
    void  MakeButton();

private:
    cDemoWindow*  mWindow;

    nSDK2::cTV2ButtonLabel*     mGUITopLabel     {nullptr};

    nSDK2::cTV2ButtonCheckBox*  mGUINormal       {nullptr};  bool  mNormal      {true};
    nSDK2::cTV2ButtonCheckBox*  mGUIPopup        {nullptr};  bool  mPopup       {false};
    nSDK2::cTV2ButtonCheckBox*  mGUICheck        {nullptr};  bool  mCheck       {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIText         {nullptr};  bool  mText        {false};
    nSDK2::cTV2ButtonCheckBox*  mGUITextInt      {nullptr};  bool  mTextInt     {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIMinislider   {nullptr};  bool  mMinislider  {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIHSeparator   {nullptr};  bool  mHSeparator  {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIVSeparator   {nullptr};  bool  mVSeparator  {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIFrame        {nullptr};  bool  mFrame       {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIHide         {nullptr};  bool  mHide        {false};

    nSDK2::cTV2ButtonCheckBox*  mGUIImmediate    {nullptr};  bool  mImmediate   {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIRepeat       {nullptr};  bool  mRepeat      {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIRight        {nullptr};  bool  mRight       {false};

    nSDK2::cTV2ButtonCheckBox*  mGUIFlat         {nullptr};  bool  mFlat        {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIInvert       {nullptr};  bool  mInvert      {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIQuiet        {nullptr};  bool  mQuiet       {false};
    nSDK2::cTV2ButtonCheckBox*  mGUISelect       {nullptr};  bool  mSelect      {false};

    nSDK2::cTV2ButtonCheckBox*  mGUITextActive   {nullptr};  bool  mTextActive  {false};
    nSDK2::cTV2ButtonCheckBox*  mGUITextLock     {nullptr};  bool  mTextLock    {false};

    nSDK2::cTV2ButtonCheckBox*  mGUIAction       {nullptr};  bool  mAction      {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIDrag         {nullptr};  bool  mDrag        {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIDialog       {nullptr};  bool  mDialog      {false};
    nSDK2::cTV2ButtonCheckBox*  mGUIStat         {nullptr};  bool  mStat        {false};

    nSDK2::cTV2ButtonLabel*     mGUIButtonLabel  {nullptr};
};


cTabButton::cTabButton( cDemoWindow*  iWindow ) :
    mWindow( iWindow )
{
}


void
cTabButton::Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 140;
    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    mGUITopLabel    = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {0, y, 360, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "ePIButtonFlags"s ); y += 20;

    x = 100;
    y = starty + 25;
    mGUINormal      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Normal"s,     &mNormal,     [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIPopup       = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Popup"s,      &mPopup,      [this](int,bool){ MakeButton(); } ); y += 20;
    mGUICheck       = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Check"s,      &mCheck,      [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIText        = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Text"s,       &mText,       [this](int,bool){ MakeButton(); } ); y += 20;
    mGUITextInt     = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_TextInt"s,    &mTextInt,    [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIMinislider  = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Minislider"s, &mMinislider, [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIHSeparator  = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_HSeparator"s, &mHSeparator, [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIVSeparator  = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_VSeparator"s, &mVSeparator, [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIFrame       = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Frame"s,      &mFrame,      [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIHide        = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Hide"s,       &mHide,       [this](int,bool){ MakeButton(); } ); y += 20;

    x = 220;
    y = starty + 25;
    mGUIImmediate   = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Immediate"s,  &mImmediate,  [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIRepeat      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Repeat"s,     &mRepeat,     [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIRight       = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Right"s,      &mRight,      [this](int,bool){ MakeButton(); } ); y += 20;
    y += 20;

    mGUIFlat        = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Flat"s,       &mFlat,       [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIInvert      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Invert"s,     &mInvert,     [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIQuiet       = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Quiet"s,      &mQuiet,      [this](int,bool){ MakeButton(); } ); y += 20;
    mGUISelect      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Select"s,     &mSelect,     [this](int,bool){ MakeButton(); } ); y += 20;

    x = 340;
    y = starty + 25;
    mGUITextActive  = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_TextActive"s, &mTextActive, [this](int,bool){ MakeButton(); } ); y += 20;
    mGUITextLock    = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_TextLock"s,   &mTextLock,   [this](int,bool){ MakeButton(); } ); y += 20;
    y += 40;

    mGUIAction      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Action"s,     &mAction,     [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIDrag        = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Drag"s,       &mDrag,       [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIDialog      = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Dialog"s,     &mDialog,     [this](int,bool){ MakeButton(); } ); y += 20;
    mGUIStat        = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "*_Stat"s,       &mStat,       [this](int,bool){ MakeButton(); } ); y += 20;

    mGUIButtonLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;

    MakeButton();
}


void
cTabButton::Hide()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    delete  mGUITopLabel;     mGUITopLabel    = nullptr;

    delete  mGUINormal;       mGUINormal      = nullptr;
    delete  mGUIPopup;        mGUIPopup       = nullptr;
    delete  mGUICheck;        mGUICheck       = nullptr;
    delete  mGUIText;         mGUIText        = nullptr;
    delete  mGUITextInt;      mGUITextInt     = nullptr;
    delete  mGUIMinislider;   mGUIMinislider  = nullptr;
    delete  mGUIHSeparator;   mGUIHSeparator  = nullptr;
    delete  mGUIVSeparator;   mGUIVSeparator  = nullptr;
    delete  mGUIFrame;        mGUIFrame       = nullptr;
    delete  mGUIHide;         mGUIHide        = nullptr;

    delete  mGUIImmediate;    mGUIImmediate   = nullptr;
    delete  mGUIRepeat;       mGUIRepeat      = nullptr;
    delete  mGUIRight;        mGUIRight       = nullptr;

    delete  mGUIFlat;         mGUIFlat        = nullptr;
    delete  mGUIInvert;       mGUIInvert      = nullptr;
    delete  mGUIQuiet;        mGUIQuiet       = nullptr;
    delete  mGUISelect;       mGUISelect      = nullptr;

    delete  mGUITextActive;   mGUITextActive  = nullptr;
    delete  mGUITextLock;     mGUITextLock    = nullptr;

    delete  mGUIAction;       mGUIAction      = nullptr;
    delete  mGUIDrag;         mGUIDrag        = nullptr;
    delete  mGUIDialog;       mGUIDialog      = nullptr;
    delete  mGUIStat;         mGUIStat        = nullptr;

    delete  mGUIButtonLabel;  mGUIButtonLabel = nullptr;

    TVRemoveButtonReq( plugin, req, kID_TheButton );

    tSuperClass::Hide();
}


void
cTabButton::MakeButton()
{
    // it is somewhat ironic to have a C++ demo to show the options for the old/legacy SDK...

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    int  flags = 0;
    int  w = 150;
    int  h = 20;
    const char*  str = "Ga Bu Zo Meu";

    TVRemoveButtonReq( plugin, req, kID_TheButton );

    if( mNormal     )  { flags |= bPIButtonFlags_Normal; }
    if( mPopup      )  { flags |= bPIButtonFlags_Popup; }
    if( mCheck      )  { flags |= bPIButtonFlags_Check; w = h = 0; }
    if( mText       )  { flags |= bPIButtonFlags_Text; }
    if( mTextInt    )  { flags |= bPIButtonFlags_TextInteger; }
    if( mMinislider )  { flags |= bPIButtonFlags_Minislider; w = h = 0; }
    if( mHSeparator )  { flags |= bPIButtonFlags_HSeparator; }
    if( mVSeparator )  { flags |= bPIButtonFlags_VSeparator; }
    if( mFrame      )  { flags |= bPIButtonFlags_Frame; }
    if( mHide       )  { flags |= bPIButtonFlags_Hide; }

    if( mImmediate  )  { flags |= bPIButtonFlags_Immediate; }
    if( mRepeat     )  { flags |= bPIButtonFlags_Repeat; }
    if( mRight      )  { flags |= bPIButtonFlags_Right; }

    if( mFlat       )  { flags |= bPIButtonFlags_Flat; }
    if( mInvert     )  { flags |= bPIButtonFlags_Invert; }
    if( mQuiet      )  { flags |= bPIButtonFlags_Quiet; }
    if( mSelect     )  { flags |= bPIButtonFlags_Select; }

    if( mTextActive )  { flags |= bPIButtonFlags_TextActive; }
    if( mTextLock   )  { flags |= bPIButtonFlags_TextLock; }

    if( mAction     )  { flags |= bPIButtonFlags_Action; }
    if( mDrag       )  { flags |= bPIButtonFlags_Drag; }
    if( mDialog     )  { flags |= bPIButtonFlags_Dialog; }
    if( mStat       )  { flags |= bPIButtonFlags_Stat; }

    TVAddButtonReq( plugin, req, kTheButtonX, kTheButtonY, w, h, kID_TheButton, flags, str );
}


//---------------------------------------------------------------------------------------------------------------


nSDK2::cTV2Tab*
NewTabButton( cDemoWindow*  iWindow )
{
    return  new  cTabButton( iWindow );
}

