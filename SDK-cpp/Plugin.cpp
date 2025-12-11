/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Plugin.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A plugin.
 */


#include "Plugin.h"


#include "Keys.h"
#include "Window.h"


namespace  nSDK {


//MARK: ========================================================================================================= Construction/Destruction


cTV2Plugin::~cTV2Plugin()
{
}


cTV2Plugin::cTV2Plugin() :
    cTV2MetaEventHandlingMixIn()
{
}


//MARK: ========================================================================================================= Plugin Infos


std::string
cTV2Plugin::Name() const
{
    return  NameID();
}


int
cTV2Plugin::Version() const
{
    return  1;
}


int
cTV2Plugin::Revision() const
{
    return  0;
}


std::string
cTV2Plugin::Copyright() const
{
    static  char  text[256];

    //TODO: make this more generic.... !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    snprintf( text, sizeof(text),
             "%s V%d,%d\nCopyright TVPaint Developpement\n(c) 2002-25",
             Name().c_str(), Version(), Revision() );

    return  text;
}


tErrorCode
cTV2Plugin::ShowAboutWindow() const
{
    return  kErrorCode_NotImplemented;
}


//MARK: ========================================================================================================= Plugin Guts


bool
cTV2Plugin::NeedKeys() const
{
    return  false;
}


tErrorCode
cTV2Plugin::CreateKeys()
{
    return  0;
}


void
cTV2Plugin::UpdateKeyViews( cPI2Keys*  iKeys, int  iID )
{
}


tErrorCode
cTV2Plugin::CreateRequester()
{
    return  0;
}


tErrorCode
cTV2Plugin::ExecuteFilterWithParameters( const std::string&  iArg )
{
    return  0;
}


int
cTV2Plugin::ComputeSequenceStart( int  iCount )
{
    return  1;
}


int
cTV2Plugin::ComputeFrameStart( double  iPos, double  iSize )
{
    return  1;
}


int
cTV2Plugin::ComputeFrame()
{
    return  1;
}


void
cTV2Plugin::ComputeFrameFinish()
{
}


void
cTV2Plugin::ComputeSequenceFinish()
{
}


//MARK: ========================================================================================================= Host interface


int
cTV2Plugin::PI2Open( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;

    // load the .loc file
    // we don't really cares if it fails here, since we do in GetLocalString()
    TV2OpenLocaleFile( LocaleFileName().c_str() );

    strncpy( mLegacyPlugin->PIName, Name().c_str(), 128 ); // 128 = magic, size of PIName char array
    mLegacyPlugin->PIName[127] = 0;
    mLegacyPlugin->PIVersion  = Version();
    mLegacyPlugin->PIRevision = Revision();

    // If this plugin was the one open at TVPaint shutdown, re-open it
    std::string  open;
    TV2UserStringRead( &open, 255, Name().c_str(), "Open", "no" );
    if( open == "yes" )
    {
        PI2Parameters( mLegacyPlugin, nullptr );
//TODO:        DoPreview( mLegacyPlugin );  //TODO: //CHECK: not "standard" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    return  1; // everything wen't well
}


void
cTV2Plugin::PI2About( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;

    if( ShowAboutWindow() == kErrorCode_NotImplemented )
        TVWarning( mLegacyPlugin, Copyright().c_str() ); //TODO: call TV2Warning() when done
}


int
cTV2Plugin::PI2Parameters( PIPlugin*  iPlugin, const char*  iParameters )
{
printf("PI2Parameters\n");//DEBUG:
    using  namespace  std::placeholders;  // for _1, _2, _3...

    mLegacyPlugin = iPlugin;

    // If the keys are not created, we do it.
    if( mKeys == nullptr  &&  NeedKeys() )
    {
        tErrorCode  err = CreateKeys();
        if( err )
            return  0;

        mKeys->SetUpdateViews( std::bind( &cTV2Plugin::UpdateKeyViews, this, _1, _2 ) );
    }

    // Open the requester.
    if( iParameters == nullptr )
    {
        tErrorCode  err = CreateRequester();
        if( err )
            return  0;

        return  1;
    }

    // "Normal" case, execute the filter
    tErrorCode  err = ExecuteFilterWithParameters( iParameters );
    if( err )
        return  0;

    return  1;
}


int
cTV2Plugin::PI2Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs )
{
    mLegacyPlugin = iPlugin;

    if( iEvent < 1000 )
    {
        cTV2Window*  window = cTV2Window::Lookup( iReq );
        if( window == nullptr )
            return  0;
        tErrorCode  err = window->HandleEvent( iEvent, iArgs );
        if( err )
            return  0;
    }
    else
    {
        tErrorCode  err = HandleMetaEvent( iEvent, iArgs );
        if( err )
            return  0;
    }

    return  1;
}


void
cTV2Plugin::PI2Close( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;

    //TODO:    TVWriteUserString( mLegacyPlugin, iName, "Open", xxx ? "yes" : "no" );

    TV2CloseLocaleFile();

    if( mWindow )
        TVCloseReq( mLegacyPlugin, mWindow->LegacyReq() ); //TODO: TV2*() ...

    delete  mKeys;  mKeys = nullptr;
}


int
cTV2Plugin::PI2SequenceStart( PIPlugin*  iPlugin, int  iCount )
{
    mLegacyPlugin = iPlugin;
    return  ComputeSequenceStart( iCount );
}


int
cTV2Plugin::PI2Start( PIPlugin*  iPlugin, double  iPos, double  iSize )
{
    mLegacyPlugin = iPlugin;
    return  ComputeFrameStart( iPos, iSize );
}


int
cTV2Plugin::PI2Work( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;
    return  ComputeFrame();
}


void
cTV2Plugin::PI2Finish( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;
    ComputeFrameFinish();
}


void
cTV2Plugin::PI2SequenceFinish( PIPlugin*  iPlugin )
{
    mLegacyPlugin = iPlugin;
    ComputeSequenceFinish();
}


//MARK:  ======================================================================================================== Main Functions


tErrorCode
cTV2Plugin::TV2Execute()
{
    if( !TVExecute( mLegacyPlugin ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Plugin::TV2SendCmd( const std::string&  iCommand, std::string*  oResult )
{
    char  tmp[512];
    if( !TVSendCmd( mLegacyPlugin, iCommand.c_str(), tmp ) )
        return  kErrorCode_LegacyError;
    if( oResult )
    {
        tmp[511] = 0;
        *oResult = tmp;
    }
    return  0;
}


void
cTV2Plugin::TV2UpdateDisplay( const cPixelRectangle&  iArea )
{
    TVUpdateDisplay( mLegacyPlugin, iArea.X1(), iArea.Y1(), iArea.X2(), iArea.Y2() );
}


void
cTV2Plugin::TV2UpdateDisplay()
{
    TVUpdateDisplay( mLegacyPlugin, 0, 0, mLegacyPlugin->ImageWidth, mLegacyPlugin->ImageHeight );
}


//MARK: ========================================================================================================= Host functions/pi-requester.h/Localisation


void
cTV2Plugin::TV2OpenLocaleFile( const std::string&  iFileName )
{
    mLocaleFile = TVOpenLocalFile( mLegacyPlugin, iFileName.c_str(), 0 );
}


void
cTV2Plugin::TV2CloseLocaleFile()
{
    if( mLocaleFile )
        TVCloseLocalFile( mLegacyPlugin, mLocaleFile );
    mLocaleFile = nullptr;
}


std::string
cTV2Plugin::TV2GetLocaleString( int  iIndex, const std::string&  iDefault ) const
{
    if( mLocaleFile == nullptr )
        return  iDefault;

    const char*  string = TVGetLocalString( mLegacyPlugin, mLocaleFile, iIndex );
    if( string == nullptr  ||  strlen( string ) == 0 )
        return  iDefault;

    return  string;
}


INTPTR
cTV2Plugin::TV2OpenFilterReqEx( int  iWidth, int  iHeight, tEventHandler  iEventHandler, cPI2Keys*  iKeys, INTPTR  iReqFlags, INTPTR  iMenuFlags )
{
    return  TVOpenFilterReqEx( mLegacyPlugin, iWidth, iHeight, iEventHandler, iKeys->LegacyKeys(), iReqFlags, iMenuFlags );
}


//MARK: ========================================================================================================= Host functions/pi-stringfile.h/String File


tErrorCode
cTV2Plugin::TV2UserStringRead( std::string*  oString, int  iMaxChar, const std::string&  iSection, const std::string&  iName, const std::string&  iDefault )
{
    char  tmp[8192];
    if( !TVReadUserString( mLegacyPlugin, iSection.c_str(), iName.c_str(), tmp, iDefault.c_str(), 8192 ) )
        return  kErrorCode_LegacyError;
    tmp[8191] = 0;
    *oString = tmp;
    return  0;
}


tErrorCode
cTV2Plugin::TV2UserStringWrite( const std::string&  iString, const std::string&  iSection, const std::string&  iName )
{
    if( !TVWriteUserString( mLegacyPlugin, iSection.c_str(), iName.c_str(), iString.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Plugin::TV2UserStringEraseSection( const std::string&  iSection )
{
    if( !TVEraseUserSection( mLegacyPlugin, iSection.c_str() ) )
        return  kErrorCode_LegacyError;
    return  0;
}


//MARK: ========================================================================================================= User Interface / Requester


void
cTV2Plugin::TV2Warning( const std::string&  iWarningText )
{
    TVWarning( mLegacyPlugin, iWarningText.c_str() );
}


//MARK: ========================================================================================================= Meta/Hud


tErrorCode
cTV2Plugin::TV2MetaFunctionInstall( PIFlags  iFunction )
{
    if( !TVInstallFunction( mLegacyPlugin, iFunction ) )
        return  kErrorCode_LegacyError;
    return  0;
}


tErrorCode
cTV2Plugin::TV2MetaFunctionClose()
{
    if( !TVCloseFunction( mLegacyPlugin ) )
        return  kErrorCode_LegacyError;
    return  0;
}


//MARK: ========================================================================================================= Properties


cPixelRectangle
cTV2Plugin::WorkArea()
{
    return  { mLegacyPlugin->WorkArea_x1, mLegacyPlugin->WorkArea_y1, mLegacyPlugin->WorkArea_x2, mLegacyPlugin->WorkArea_y2, cPixelRectangle::eXYXY::kXYXY };
}


//MARK: ========================================================================================================= Helpers


} // namespace  nSDK

