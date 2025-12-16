/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  fade.h
 *
 *  @brief  The "fade" demo plugin.
 */

#pragma once

#include "TVPaintAnimationSDK/TVPaintSDK.hpp"


using  namespace  std::literals::string_literals;


//MARK: ========================================================================================================= forwards


class  cFadeWindow;
class  cFadePlugin;


//MARK: ========================================================================================================= cFadeWindow


class  cFadeWindow :
    public  nSDK::cTV2Window
{
    using  tSuperClass = nSDK::cTV2Window;

public:
    virtual  ~cFadeWindow()  override  {}
    cFadeWindow( cFadePlugin*  iPlugin );

public:
///@name  Events
///@{
    virtual  nSDK::tErrorCode  EventWindowOpen( const nSDK::cPixelRectangle&  iArea )  override;
    virtual  nSDK::tErrorCode  EventWindowClose( const nSDK::cPixelRectangle&  iArea, bool  iFinal )  override;
    virtual  nSDK::tErrorCode  EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;
    virtual  nSDK::tErrorCode  EventButtonText( int  iButtonID )  override;
    virtual  nSDK::tErrorCode  EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )  override;
///@}

private:
    cFadePlugin*  mPlugin  {nullptr};

//TODO:    text field double...
    nSDK::cTV2ButtonHSeparator*  mSeparator2  {nullptr};
    nSDK::cTV2ButtonCheckBox*    mPreview     {nullptr};
    nSDK::cTV2ButtonHSeparator*  mSeparator1  {nullptr};
    nSDK::cTV2ButtonAction*      mApply       {nullptr};
};


//MARK: ========================================================================================================= cFadePlugin


class  cFadePlugin :
    public  nSDK::cTV2Plugin
{
    using  tSuperClass = nSDK::cTV2Plugin;

public:
    cFadePlugin()  {}

public: // infos
    virtual  std::string  LocaleFileName() const  override  { return  "sdk-fade.loc"s; }
    virtual  std::string  NameID() const  override  { return  "SDK-Fade"s; }
    virtual  std::string  Name() const  override  { return  TV2GetLocaleString( 100, "SDK Fade"s ); }
    virtual  int  Version() const  override  { return  2; }
    virtual  int  Revision() const  override  { return  0; }

public: // generic
    virtual  bool  NeedKeys() const  override  { return  true; }
    virtual  nSDK::tErrorCode  CreateKeys()  override;
    virtual  void  UpdateKeyViews( nSDK::cPI2Keys*  iKeys, int  iID )  override;
    virtual  nSDK::tErrorCode  CreateRequester()  override;
    virtual  nSDK::tErrorCode  ExecuteFilterWithParameters( const std::string&  iParameters )  override;
    virtual  int  ComputeFrame()  override;

public: // meta events handling
    nSDK::tErrorCode  MetaEventClosed()  override;
    nSDK::tErrorCode  MetaEventPreview( const nSDK::cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey )  override;

public: // properties
    double  FadeFactor() const  { return  mFadeFactor; }
    void  FadeFactor( double  iFadeFactor );
    bool  NeedPreview() const  { return  mNeedPreview; }
    void  NeedPreview( bool  iNeedPreview );

public:
    // flip the preview state and refresh everything needed
    void  FlipPreview();//ZAP:
    // set the preview state and refresh everything needed
    void  SetPreview( bool  iPreview );
    // Apply the filter to the selected frames
    void  Apply();

    // Puts the 'preview' button in the correct state
    void  DoPreviewButton();
    // Does a full screen preview
    void  DoPreview();

    // plugin specific
    void  DoFade();

public:
    double  mFadeFactor   {0.0};
    bool    mNeedPreview  {false};
};

