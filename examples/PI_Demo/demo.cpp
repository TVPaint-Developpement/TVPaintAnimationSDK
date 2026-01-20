/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  main.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  A demo filter.
*/


#include  "demo.h"

#include  "TVPaintAnimationSDK/TVPaintSDK.h"

#include "utilities.h"

#include <cstring>

extern  nSDK::cTV2Tab*  NewTabButton( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabText( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabPopup( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabSlider( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabMeta( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabWindow( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabProject( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabGeorge( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabMisc( cDemoWindow*  iWindow );
extern  nSDK::cTV2Tab*  NewTabPreview( cDemoWindow*  iWindow );


// sizes of some GUI components
constexpr int  kSize_RequesterW = 900;
constexpr int  kSize_RequesterH = 380;
constexpr int  kSize_SubWindowH = 225;
constexpr int  kSize_MessagesW  = (kSize_RequesterW-19);
constexpr int  kSize_MessagesH  = 100;


// ID's of GUI components
enum  eID
{
    kID_Tabs       = 10,
    kID_Messages   = 100,
    kID_Separator1 = 901,
    kID_Separator2 = 902,
};


// ID's of the different keys
enum  eParameterID
{
    //TODO: //NAME: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    eParameterID_FadeFactor = 10    // only the fade factor is keyable
};


//MARK: ========================================================================================================= cDemoWindow


cDemoWindow::cDemoWindow( cDemoPlugin*  iPlugin ) :
    tSuperClass( iPlugin ),
    mPlugin( iPlugin )
{
}


nSDK::tErrorCode
cDemoWindow::EventWindowOpen( const nSDK::cPixelRectangle&  iArea )
{
printf("cDemoWindow::EventWindowOpen()\n");//DEBUG:
    int  y = 5;

    TV2Title( TV2GetLocaleString( 10000, "Filter : Demo"s ) );

    std::vector< nSDK::cTV2Tab* >  tabs;
    tabs.push_back( NewTabButton( this ) );
    tabs.push_back( NewTabText( this ) );
    tabs.push_back( NewTabPopup( this ) );
    tabs.push_back( NewTabSlider( this ) );
    tabs.push_back( NewTabMeta( this ) );
    tabs.push_back( NewTabWindow( this ) );
    tabs.push_back( NewTabProject( this ) );
    tabs.push_back( NewTabGeorge( this ) );
    tabs.push_back( NewTabMisc( this ) );
    tabs.push_back( NewTabPreview( this ) );

printf("  new  nSDK::cTV2ButtonTabs()\n");//DEBUG:
    mTabs = new  nSDK::cTV2ButtonTabs( this, kID_Tabs, {9, y, kSize_RequesterW-20-9, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, tabs, 0 );
//    TV2ButtonTabsAdd( kID_Tabs, {9, y, kSize_RequesterW-20-9, 20, nSDK::cPixelRectangle::eXYWH::kXYWH}, tabNames, 0 );
    y += 21;
printf("  new  nSDK::cTV2ButtonHSeparator(1)\n");//DEBUG:
    mSeparator1 = new  nSDK::cTV2ButtonHSeparator( this, kID_Separator1, {0, y, kSize_RequesterW, 0, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    y += 8;

    y += kSize_SubWindowH;

    y += 2;
printf("  new  nSDK::cTV2ButtonHSeparator(2)\n");//DEBUG:
    mSeparator2 = new  nSDK::cTV2ButtonHSeparator( this, kID_Separator2, {0, y, kSize_RequesterW, 0, nSDK::cPixelRectangle::eXYWH::kXYWH} );
    y += 8;

printf("  new  nSDK::cTV2ButtonConsoleOut()\n");//DEBUG:
    mConsoleOut = new  nSDK::cTV2ButtonConsoleOut( this, kID_Messages, {9, y, kSize_MessagesW, kSize_MessagesH, nSDK::cPixelRectangle::eXYWH::kXYWH} );
printf("  mConsoleOut->DrawString()\n");//DEBUG:
    mConsoleOut->DrawString( "<Nothing yet>"s );
    y += 52;

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoWindow::EventWindowClose( const nSDK::cPixelRectangle&  iArea, bool  iFinal )
{
//TODO:    CloseWindowEx( iPlugin, iReq );

    delete  mTabs;        mTabs = nullptr;
    delete  mSeparator1;  mSeparator1 = nullptr;
    delete  mSeparator2;  mSeparator2 = nullptr;
    delete  mConsoleOut;  mConsoleOut = nullptr;

    // Save the requester state (open/closed)
    mPlugin->TV2UserStringWrite( iFinal ? "yes"s : "no"s, Plugin()->Name(), "Open"s );

    // Save the keys
    if( mPlugin->Keys() )
        mPlugin->Keys()->Save( NULL, mPlugin->Name().c_str() );

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoWindow::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoWindow::EventButtonText( int  iButtonID )
{
    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoWindow::EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )
{
    return  nSDK::kErrorCode_NoError;
}


void
cDemoWindow::DoMeta( PIFlags  iMetaKind, PIFlags  iMetaFlags )
{
    PIPlugin*  plugin = Plugin()->LegacyPlugin();
//    INTPTR  req = LegacyReq();

    if( iMetaKind == -1 )
    {
        // deselection
        TVCloseFunction( plugin );

        WriteConsole( "CLOSE META"s );
    }
    else
    {
        TVInstallFunction( plugin, iMetaKind | iMetaFlags );

        char  tmp[256];
        snprintf( tmp, sizeof(tmp), "INSTALL META : %llud", iMetaKind );
        WriteConsole( tmp );
    }
}


void
cDemoWindow::WriteConsole( const std::string&  iString )
{
    mConsoleOut->DrawString( iString );
}


//MARK: ========================================================================================================= cDemoPlugin


nSDK::tErrorCode
cDemoPlugin::CreateKeys()
{
    mKeys = new  nSDK::cPI2Keys( this, Name().c_str(), NameID().c_str() );  // Key allocation
    if( mKeys == nullptr )
        return  'aloc';

    mFadeFactor = 0.0;
    mKeys->AddParameter( eParameterID_FadeFactor, TV2GetLocaleString( 10010, "Fade factor"s ), kPIKeyType_Double, &mFadeFactor );  // Key creation

    mKeys->Load( nullptr, NameID().c_str() );

    return  nSDK::kErrorCode_NoError;
}


// Refresh function for the keys
// Refresh is called after a SetValue(), Load(), moving a key interactively, ...
// When iId==0, then several (or all) of the keys have changed and must be refreshed on screen
void
cDemoPlugin::UpdateKeyViews( nSDK::cPI2Keys*  iKeys, int  iID )
{
    // check for the correct ID (all in our simple case)
    if( iID == 0  ||  iID == eParameterID_FadeFactor )
    {
        iKeys->GetValue( eParameterID_FadeFactor, &mFadeFactor );

        if( mWindow )
        {
            char  tmp[256];

            snprintf( tmp, sizeof(tmp), "%.2f", mFadeFactor*100.0 );
//TODO:            mWindow->TV2ButtonSetString( kID_Factor, tmp );
        }
    }

    if( mWindow )
        DoPreview();
}


nSDK::tErrorCode
cDemoPlugin::CreateRequester()
{
printf("cDemoPlugin::CreateRequester()\n");//DEBUG:
    // requester already created
    if( mWindow != nullptr )
    {
        mWindow->TV2ToFront();
        return  0;
    }

    mWindow = nSDK::cTV2Window::TV2OpenFilterWindow( this, new  cDemoWindow( this ), {kSize_RequesterW, kSize_RequesterH}, bPIRequesterFlags_Standard, 0 );

    if( mWindow == nullptr )
    {
        TV2Warning( TV2GetLocaleString( 30000, "Can't Open Requester !"s ) );
        return  'open';
    }

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoPlugin::ExecuteFilterWithParameters( const std::string&  iParameters )
{
//    double  saveFadeFactor = mFadeFactor;
//
//    // If this string exists, some parameters are send from an external
//    // program (from George or a DDE command). In this case, DO NOT
//    // open the user interface but run TV2Execute() (after the
//    // interpretation of the parameters in the string).
//
//    if( NeedPreview() )
//        TV2MetaFunctionClose();
//
//    double  val = 0.0;
//    sscanf( iParameters.c_str(), "%lf", &val );
//
//    mFadeFactor = std::clamp( val/100.0, 0.0, 1.0 );
//    TV2Execute();
//
//    mFadeFactor = saveFadeFactor;

    return  nSDK::kErrorCode_NoError;
}


int
cDemoPlugin::ComputeFrame()
{
//    // Read the current value for the fade factor
//    mKeys->GetValue( eParameterID_FadeFactor, &mFadeFactor );
//
//    // Apply the working function with the current parameters
//    // iFilter->Undo is the source block (Read Only)
//    // iFilter->Current is the destination block
//    if( mFadeFactor != 0.0 )
//    {
//        // If the processing is a 'no-op' (just copy the pixels),
//        // we don't call the Fade function because in TVPaint,
//        // iFilter->Undo and iFilter->Current already contains the same pixel values
//        // when a preview is needed or when the filter has to be applied.
//        DoFade();
//    }
//
//    TV2UpdateDisplay( WorkArea() );

    return  1;
}



nSDK::tErrorCode
cDemoPlugin::MetaEventClosed()
{
    // We have to close our preview 'coz the user selected another tool.
    if( mNeedPreview )
    {
        mNeedPreview = false;

        TV2UpdateDisplay( WorkArea() );
    }

    return  nSDK::kErrorCode_NoError;
}


nSDK::tErrorCode
cDemoPlugin::MetaEventPreview( const nSDK::cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey )
{
    // We have to redo the preview 'coz the user changed something that affect it.
    // (Like changing the background mode...)
    DoPreview();

    return  nSDK::kErrorCode_NoError;
}


void
cDemoPlugin::FadeFactor( double  iFadeFactor )
{
    mFadeFactor = std::clamp( iFadeFactor, 0.0, 1.0 );

    mKeys->SetValue( eParameterID_FadeFactor, &mFadeFactor ); // this will call UpdateKeyViews() where we'll be updating the GUI with the new value
}


void
cDemoPlugin::NeedPreview( bool  iNeedPreview )
{
    mNeedPreview = iNeedPreview;
}


// The heart of the filter.
// This function does the real work, everything else is just sugar
// to make it look nice in TVPaint.

// This function fades a block of pixels.
// mFactor = 0 : don't change anything
// mFactor = 1 : fully transparent
void
cDemoPlugin::DoFade()
{
    const PIBlock*  src = mLegacyPlugin->Undo;
    PIBlock*  dst = mLegacyPlugin->Current;

    // some sanity checks
    if( !src  ||  !dst )
        return;
    if( src->Width != dst->Width  ||  src->Height != dst->Height )
        return;

    // if there is no processing to do, just copy the pixels !
    if( mFadeFactor == 0.0 )
    {
        // et voila !
        memcpy( dst->Data, src->Data, dst->Height*dst->Width*sizeof(PIPixel) );
    }

    // fully transparent, optimize !
    else if( mFadeFactor == 1.0 )
    {
        PIPixel*  pdst = dst->Data;

        for( int  y = 0; y < dst->Height; ++y )
        {
            for( int  x = 0; x < dst->Width; ++x )
            {
                // this is a common trick in TVPaint Pixel processing.
                // if a pixel is fully tranparent, all of R, G, B and A are 0,
                // so we just assign 0x00000000 to pixel.l instead of
                // assigning all four channels separately
                pdst->l = 0x00000000;
                ++pdst;
            }
        }
        // in fact, in this precise case where the whole block has to be
        // cleared to fully transparent, we could even do :
        // memset( dst, 0, oDst->Height*oDst->Width*sizeof(PIPixel) );
    }

    // usual case...
    else
    {
        PIPixel*  psrc = src->Data;
        PIPixel*  pdst = dst->Data;

        int  factor = (int)((1.0-mFadeFactor)*256);

        for( int  y = 0; y < dst->Height; ++y )
        {
            for( int  x = 0; x < dst->Width; ++x )
            {
                // we do the same computation for all of R, G, B, A,
                // so we don't care which is at index 0, 1, 2 or 3
                // in most cases, however, we must care about that,
                // and use the r,g,b,a fields of the PIBlock structure
                // to index into PIBlock->c[]
                pdst->c[0] = (psrc->c[0]*factor)>>8;
                pdst->c[1] = (psrc->c[1]*factor)>>8;
                pdst->c[2] = (psrc->c[2]*factor)>>8;
                pdst->c[3] = (psrc->c[3]*factor)>>8;
                ++pdst;
                ++psrc;
            }
        }
    }
}


void
cDemoPlugin::DoPreview()
{
    // If we don't want a preview, return !
    if( !mNeedPreview )
        return;

    // Send a George command to show a 'clock' mouse
    TV2SendCmd( "tv_lockmouse 2", nullptr );

    // Do the computation, like usual
    if( mFadeFactor != 0.0 )
    {
        // If the processing is a 'no-op' (just copy the pixels),
        // we don't call the Fade function because in TVPaint,
        // iFilter->Undo and iFilter->Current always contains the same pixel values
        // when a preview is needed or when the filter has to be applied.
        DoFade();
    }

    // Show the result on-screen
    TV2UpdateDisplay( WorkArea() );

    // Copy the data back from the 'undo' buffer in the 'current' buffer
    // This won't affect the displayed image
    memcpy( mLegacyPlugin->Current->Data, mLegacyPlugin->Undo->Data, mLegacyPlugin->Current->Width*mLegacyPlugin->Current->Height*sizeof(PIPixel32) );

    // Show again the normal mouse
    TV2SendCmd( "tv_unlockmouse", nullptr );
}


void
cDemoPlugin::Apply()
{
    if( NeedPreview() )
        TV2MetaFunctionClose();
    TV2Execute();
}


static  cDemoPlugin  sgPlugin;


/**************************************************************************************/
/**************************************************************************************/

// The functions directly called by the Host through the plugin interface
//TODO: move to some special include file

extern "C" {


int FAR PASCAL
PI_Open( PIPlugin*  iPlugin )
{
    printf("PI_Open()\n");
    return  sgPlugin.PI2Open( iPlugin );
}


void FAR PASCAL
PI_About( PIPlugin*  iPlugin )
{
    printf("PI_About()\n");
    sgPlugin.PI2About( iPlugin );
}


int FAR PASCAL
PI_Parameters( PIPlugin*  iPlugin, const char*  iArg )
{
    printf("PI_Parameters()\n");
    return  sgPlugin.PI2Parameters( iPlugin, iArg );
}


void FAR PASCAL
PI_Close( PIPlugin*  iPlugin )
{
    printf("PI_Close()\n");
    sgPlugin.PI2Close( iPlugin );
}


int FAR PASCAL
PI_Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs )
{
    //    printf("PI_Msg()\n");
    return  sgPlugin.PI2Msg( iPlugin, iEvent, iReq, iArgs );
}


int FAR PASCAL
PI_SequenceStart( PIPlugin*  iPlugin, int  iCount )
{
    printf("PI_SequenceStart()\n");
    return  sgPlugin.PI2SequenceStart( iPlugin, iCount );
}


int FAR PASCAL
PI_Start( PIPlugin*  iPlugin, double  iPos, double  iSize )
{
    printf("PI_Start()\n");
    return  sgPlugin.PI2Start( iPlugin, iPos, iSize );
}


int FAR PASCAL
PI_Work( PIPlugin*  iPlugin )
{
    printf("PI_Work()\n");
    return  sgPlugin.PI2Work( iPlugin );
}


void FAR PASCAL
PI_Finish( PIPlugin*  iPlugin )
{
    printf("PI_Finish()\n");
    sgPlugin.PI2Finish( iPlugin );
}


void FAR PASCAL
PI_SequenceFinish( PIPlugin*  iPlugin )
{
    printf("PI_SequenceFinish()\n");
    sgPlugin.PI2SequenceFinish( iPlugin );
}


} // extern "C"
