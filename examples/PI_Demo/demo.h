/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  demo.h
 *
 *  @brief  The demo plugin.
 */


#include "SDK2.h"


using  namespace  std::literals::string_literals;


//MARK: ========================================================================================================= forwards


class  cDemoWindow;
class  cDemoPlugin;


//MARK: ========================================================================================================= cDemoWindow


class  cDemoWindow :
    public  nSDK2::cTV2Window
{
    using  tSuperClass = nSDK2::cTV2Window;

public:
    virtual  ~cDemoWindow()  override  {}
    cDemoWindow( cDemoPlugin*  iPlugin );

public:
///@name  Events
///@{
    virtual  nSDK2::tErrorCode  EventWindowOpen( const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  nSDK2::tErrorCode  EventWindowClose( const nSDK2::cPixelRectangle&  iArea, bool  iFinal )  override;

    virtual  nSDK2::tErrorCode  EventButtonUp( int  iButtonID, const nSDK2::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )  override;
    virtual  nSDK2::tErrorCode  EventButtonText( int  iButtonID )  override;
    virtual  nSDK2::tErrorCode  EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )  override;
///@}

public:
    // meta tab
    void  DoMeta( PIFlags  iMetaKind, PIFlags  iMetaFlags );

    void  WriteConsole( const std::string&  iString );

private:
    cDemoPlugin*  mPlugin  {nullptr};
    
    nSDK2::cTV2ButtonTabs*  mTabs  {nullptr};

    nSDK2::cTV2ButtonHSeparator*  mSeparator1  {nullptr};
    nSDK2::cTV2ButtonHSeparator*  mSeparator2  {nullptr};

    nSDK2::cTV2ButtonConsoleOut*  mConsoleOut  {nullptr};
};


//MARK: ========================================================================================================= cDemoPlugin


class  cDemoPlugin :
    public  nSDK2::cTV2Plugin
{
    using  tSuperClass = nSDK2::cTV2Plugin;

public:
    cDemoPlugin()  {}

public: // infos
    virtual  std::string  LocaleFileName() const  override  { return  "sdk-demo.loc"s; }
    virtual  std::string  NameID() const  override  { return  "SDK-Demo"s; }
    virtual  std::string  Name() const  override  { return  TV2GetLocaleString( 100, "SDK Demo"s ); }
    virtual  int  Version() const  override  { return  2; }
    virtual  int  Revision() const  override  { return  0; }

public: // generic
    virtual  bool  NeedKeys() const  override  { return  true; }
    virtual  nSDK2::tErrorCode  CreateKeys()  override;
    virtual  void  UpdateKeyViews( nSDK2::cPI2Keys*  iKeys, int  iID )  override;
    virtual  nSDK2::tErrorCode  CreateRequester()  override;
    virtual  nSDK2::tErrorCode  ExecuteFilterWithParameters( const std::string&  iParameters )  override;
    virtual  int  ComputeFrame()  override;

public: // meta events handling
    nSDK2::tErrorCode  MetaEventClosed()  override;
    nSDK2::tErrorCode  MetaEventPreview( const nSDK2::cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey )  override;

public: // properties
    double  FadeFactor() const  { return  mFadeFactor; }
    void  FadeFactor( double  iFadeFactor );
    bool  NeedPreview() const  { return  mNeedPreview; }
    void  NeedPreview( bool  iNeedPreview );

public:
    // Apply the filter to the selected frames
    void  Apply();

    // Does a full screen preview
    void  DoPreview();

    // plugin specific
    void  DoFade();

public:
    double  mFadeFactor   {0.0};
    bool    mNeedPreview  {false};
};

