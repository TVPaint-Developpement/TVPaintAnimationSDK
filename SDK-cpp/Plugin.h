/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Plugin.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A plugin.
 */

#pragma once

#include "TVPaintSDK.h" //TODO: //ZAP: !!!!!!!!!!!!!!!!!!!!!!


#include "Types.h"
#include "EventHandlingMixIn.h"
#include "PixelCoordinates.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Plugin


class  cTV2Plugin :
    public  cTV2MetaEventHandlingMixIn
{
public:
    SDK_ABSTRACT_CLASS( cTV2Plugin );
    SDK_NON_COPYABLE( cTV2Plugin );

public:
///@name  Construction/Destruction
///@{
    cTV2Plugin();
///@}

public:
///@name  Plugin Infos
///@{
    virtual  std::string  LocaleFileName() const = 0;
    virtual  std::string  NameID() const = 0;
    virtual  std::string  Name() const;
    virtual  int  Version() const;
    virtual  int  Revision() const;
    virtual  std::string  Copyright() const;
    virtual  tErrorCode  ShowAboutWindow() const;
///@}

public:
///@name  Plugin Guts
///@{
    virtual  bool  NeedKeys() const;
    virtual  tErrorCode  CreateKeys();
    virtual  void  UpdateKeyViews( cPI2Keys*  iKeys, int  iID );

    virtual  tErrorCode  CreateRequester();

    virtual  tErrorCode  ExecuteFilterWithParameters( const std::string&  iParameters );

    //TODO: tErrorCode...
    virtual  int  ComputeSequenceStart( int  iCount );
    virtual  int  ComputeFrameStart( double  iPos, double  iSize );
    virtual  int  ComputeFrame();
    virtual  void  ComputeFrameFinish();
    virtual  void  ComputeSequenceFinish();
///@}

public:
///@name  Host interface
///@{
    int   PI2Open( PIPlugin*  iPlugin );
    void  PI2About( PIPlugin*  iPlugin );
    int   PI2Parameters( PIPlugin*  iPlugin, const char*  iArg );
    int   PI2Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs );
    void  PI2Close( PIPlugin*  iPlugin );

    int  PI2SequenceStart( PIPlugin*  iPlugin, int  iCount );
    int  PI2Start( PIPlugin*  iPlugin, double  iPos, double  iSize );
    int  PI2Work( PIPlugin*  iPlugin );
    void  PI2Finish( PIPlugin*  iPlugin );
    void  PI2SequenceFinish( PIPlugin*  iPlugin );
///@}

public:
///@name  Main Functions
///@{
    tErrorCode  TV2Execute();  //TODO: //NAME:
    tErrorCode  TV2SendCmd( const std::string&  iCommand, std::string*  oResult );
    void  TV2UpdateDisplay( const cPixelRectangle&  iArea );
    void  TV2UpdateDisplay();
///@}

public:
///@name  Host functions/pi-requester.h/Localisation
//TODO: extract to new file pi-localisation.h !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///@{
    void  TV2OpenLocaleFile( const std::string&  iFileName );
    void  TV2CloseLocaleFile();
    std::string  TV2GetLocaleString( int  iIndex, const std::string&  iDefault ) const;
///@}

public:
///@name  Host functions/pi-requester.h/User Interface / Window
///@{
    INTPTR  TV2OpenFilterReqEx( int  iWidth, int  iHeight, tEventHandler  iEventHandler, cPI2Keys*  iKeys, INTPTR  iReqFlags, INTPTR  iMenuFlags );
///@}

public:
///@name  Host functions/pi-stringfile.h/String File
///@{
    //TODO: make specialized fcts for usual types (int,float,bool,...)
    tErrorCode  TV2UserStringRead( std::string*  oString, int  iMaxChar, const std::string&  iSection, const std::string&  iName, const std::string&  iDefault );
    tErrorCode  TV2UserStringWrite( const std::string&  iString, const std::string&  iSection, const std::string&  iName );
    tErrorCode  TV2UserStringEraseSection( const std::string&  iSection );
///@}

public:
///@name  User Interface / Requester
///@{
    void  TV2Warning( const std::string&  iWarningText );
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Meta/Hud
///@{
    tErrorCode  TV2MetaFunctionInstall( PIFlags  iFunction );
    tErrorCode  TV2MetaFunctionClose();
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Properties
///@{
    cPI2Keys*  Keys()  { return  mKeys; }
    cPixelRectangle  WorkArea();
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Access to Legacy Stuff (temporary)
///@{
    PIPlugin*  LegacyPlugin()  { return  mLegacyPlugin; }
    const PIPlugin*  LegacyPlugin() const  { return  mLegacyPlugin; }
///@}

protected:
///@name  Helpers
///@{
///@}

protected:
    PIPlugin*    mLegacyPlugin  {nullptr}; // the plugin, as known by TVPaint
    void*        mLocaleFile    {nullptr}; // file containing localized messages
    cPI2Keys*    mKeys          {nullptr};
    cTV2Window*  mWindow        {nullptr};
};


} // namespace  nSDK
