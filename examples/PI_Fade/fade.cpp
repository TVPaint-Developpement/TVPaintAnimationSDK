/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  fade.cpp
 *
 *  @brief  The "fade" demo plugin.
 */


#include "fade.h"


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <algorithm>


// sizes of some GUI components
constexpr  int  kSize_RequesterW = 185;
constexpr  int  kSize_RequesterH =  83;
constexpr  int  kSize_PositionX  = 103;
constexpr  int  kSize_TextFieldW =  50;


// ID's of GUI components
enum  eID
{
    kID_Factor         = 10,
    kID_Factor_SLIDER  = 11,
    kID_Separator1     = 20,
    kID_Preview        = 30,
    kID_Separator2     = 40,
    kID_Apply          = 50
};


// ID's of the different keys
enum  eParameterID
{
    eParameterID_FadeFactor = 10    // only the fade factor is keyable
};


//MARK: ========================================================================================================= cFadeWindow


cFadeWindow::cFadeWindow( cFadePlugin*  iPlugin ) :
    tSuperClass( iPlugin ),
    mPlugin( iPlugin )
{
}


nSDK2::tErrorCode
cFadeWindow::EventWindowOpen( const nSDK2::cPixelRectangle&  iArea )
{
    char  tmp[512];

    // Build the requester.
    int  y = 5;

    TV2Title( TV2GetLocaleString( 10000, "Filter : Fade"s ) );

    // fade factor button
    snprintf( tmp, sizeof(tmp), "%.2f", mPlugin->FadeFactor()*100.0 );
    TV2ButtonTextFieldAdd( kID_Factor, {kSize_PositionX, y, kSize_TextFieldW, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, tmp );  // Text button
    TV2ButtonUnit( kID_Factor, "%" );                                                                                           // Text Unit
    TV2ButtonName( kID_Factor, TV2GetLocaleString( 10010, "Fade factor"s ), bPIButtonTextAlign_OutLeft );                       // Text + Align
    TV2ButtonHelpText( kID_Factor, TV2GetLocaleString( 20010, "Fade factor button"s ) );                                        // Help Popup
    TV2ButtonMiniSliderAdd( kID_Factor_SLIDER, {kSize_PositionX+kSize_TextFieldW+3, y} );                                       // Minislider
    y += 20;

    // sep01
    mSeparator1 = new  nSDK2::cTV2ButtonHSeparator( this, kID_Separator1, {0, y, kSize_RequesterW, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH} );
    y += 8;

    // preview button
    mPreview = new  nSDK2::cTV2ButtonCheckBox( this, kID_Preview, {kSize_RequesterW-20-9, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, TV2GetLocaleString( 10011, "Preview"s ), mPlugin->NeedPreview(), [this]( int, bool  iState ){ mPlugin->SetPreview( iState ); } );
    mPreview->HelpText( TV2GetLocaleString( 20011, "Preview button"s ) );
    y += 20;

    // sep02
    mSeparator2 = new  nSDK2::cTV2ButtonHSeparator( this, kID_Separator2, {0, y, kSize_RequesterW, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH} );
    y += 8;

    // apply button
    mApply = new  nSDK2::cTV2ButtonAction( this, kID_Apply, {9, y, kSize_RequesterW-19, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, TV2GetLocaleString( 10012, "Apply"s ), [this](int){ mPlugin->Apply(); } );
    mApply->HelpText( TV2GetLocaleString( 20012, "Apply button"s ) );
    y += 20;

    mPlugin->DoPreview();

    return  0;
}


nSDK2::tErrorCode
cFadeWindow::EventWindowClose( const nSDK2::cPixelRectangle&  iArea, bool  iFinal )
{
    // Save the requester state (opened or closed)
    mPlugin->TV2UserStringWrite( iFinal ? "yes"s : "no"s, Plugin()->Name(), "Open"s );

    // Save the keys
    if( mPlugin->Keys() )
        mPlugin->Keys()->Save( NULL, mPlugin->Name().c_str() );

    return  0;
}


nSDK2::tErrorCode
cFadeWindow::EventButtonUp( int  iButtonID, const nSDK2::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    switch( iButtonID )
    {
        case  kID_Factor_SLIDER:
            //USELESS: Plugin()->Keys()->SetValue( eParameterID_FadeFactor, &mFadeFactor );
            break;

        case  kID_Factor:
        {
            std::string  tmp;
            TV2ButtonGetString( kID_Factor, &tmp );
            // Update the fade factor
            mPlugin->FadeFactor( std::stod( tmp )/100.0 );
        }
        break;

        case  kID_Preview:
        {
            //ZAP:mPlugin->FlipPreview();
        }
        break;

        case  kID_Apply:
        {
            //ZAP:mPlugin->Apply();
        }
        break;
    }

    return  0;
}


nSDK2::tErrorCode
cFadeWindow::EventButtonText( int  iButtonID )
{
    switch( iButtonID )
    {
        case  kID_Factor:
        {
            std::string  tmp;
            TV2ButtonGetString( iButtonID, &tmp );
            for( int  i = 0; tmp[i] != 0; i++ )
            {
                if( !isdigit( tmp[i]) )
                {
                    tmp.erase( i, 1 );
                    TV2ButtonSetString( iButtonID, tmp );
                    //break;
                }
            }
        }
        break;
    }

    return  0;
}


nSDK2::tErrorCode
cFadeWindow::EventButtonMiniSlider( int  iButtonID, int  iDelta, int  iTimeMS, bool  iCtrlKey )
{
    switch( iButtonID )
    {
        case  kID_Factor_SLIDER:
        {
            mPlugin->FadeFactor( mPlugin->FadeFactor() + iDelta/100. );
        }
        break;
    }

    return  0;
}


//MARK: ========================================================================================================= cFadePlugin


nSDK2::tErrorCode
cFadePlugin::CreateKeys()
{
    mKeys = new  nSDK2::cPI2Keys( this, Name().c_str(), NameID().c_str() );  // Key allocation
    if( mKeys == nullptr )
        return  'aloc';

    mFadeFactor = 0.0;
    mKeys->AddParameter( eParameterID_FadeFactor, TV2GetLocaleString( 10010,  "Fade factor"s ), kPIKeyType_Double, &mFadeFactor );  // Key creation

    mKeys->Load( nullptr, NameID().c_str() );

    return  0;
}


// Refresh function for the keys
// Refresh is called after a SetValue(), Load(), moving a key interactively, ...
// When iId==0, then several (or all) of the keys have changed and must be refreshed on screen
void
cFadePlugin::UpdateKeyViews( nSDK2::cPI2Keys*  iKeys, int  iID )
{
    // check for the correct ID (all in our simple case)
    if( iID == 0  ||  iID == eParameterID_FadeFactor )
    {
        iKeys->GetValue( eParameterID_FadeFactor, &mFadeFactor );

        if( mWindow )
        {
            char  tmp[256];

            snprintf( tmp, sizeof(tmp), "%.2f", mFadeFactor*100.0 );
            mWindow->TV2ButtonSetString( kID_Factor, tmp );
        }
    }

    if( mWindow )
        DoPreview();
}


nSDK2::tErrorCode
cFadePlugin::CreateRequester()
{
    // requester already created
    if( mWindow != nullptr )
    {
        mWindow->TV2ToFront();
        return  0;
    }

    mWindow = nSDK2::cTV2Window::TV2OpenFilterWindow( this, new  cFadeWindow( this ), {kSize_RequesterW, kSize_RequesterH}, bPIRequesterFlags_Standard, 0 );

    if( mWindow == nullptr )
    {
        TV2Warning( TV2GetLocaleString( 30000, "Can't Open Requester !"s ) );
        return  'open';
    }

    return  0;
}


nSDK2::tErrorCode
cFadePlugin::ExecuteFilterWithParameters( const std::string&  iParameters )
{
    double  saveFadeFactor = mFadeFactor;

    // If this string exists, some parameters are send from an external
    // program (from George or a DDE command). In this case, DO NOT
    // open the user interface but run TV2Execute() (after the
    // interpretation of the parameters in the string).

    if( NeedPreview() )
        TV2MetaFunctionClose();

    double  val = 0.0;
    sscanf( iParameters.c_str(), "%lf", &val );

    mFadeFactor = std::clamp( val/100.0, 0.0, 1.0 );
    TV2Execute();

    mFadeFactor = saveFadeFactor;

    return  0;
}


int
cFadePlugin::ComputeFrame()
{
    // Read the current value for the fade factor
    mKeys->GetValue( eParameterID_FadeFactor, &mFadeFactor );
    
    // Apply the working function with the current parameters
    // iFilter->Undo is the source block (Read Only)
    // iFilter->Current is the destination block
    if( mFadeFactor != 0.0 )
    {
        // If the processing is a 'no-op' (just copy the pixels),
        // we don't call the Fade function because in TVPaint,
        // iFilter->Undo and iFilter->Current already contains the same pixel values
        // when a preview is needed or when the filter has to be applied.
        DoFade();
    }
    
    TV2UpdateDisplay( WorkArea() );
    
    return  1;
}



nSDK2::tErrorCode
cFadePlugin::MetaEventClosed()
{
    // We have to close our preview 'coz the user selected another tool.
    if( mNeedPreview )
    {
        mNeedPreview = false;
        DoPreviewButton();

        TV2UpdateDisplay( WorkArea() );
    }

    return  nSDK2::kErrorCode_NoError;
}


nSDK2::tErrorCode
cFadePlugin::MetaEventPreview( const nSDK2::cPixelRectangle&  iArea, int  iMouseButton, bool  iCtrlKey )
{
    // We have to redo the preview 'coz the user changed something that affect it.
    // (Like changing the background mode...)
    DoPreview();

    return  nSDK2::kErrorCode_NoError;
}


void
cFadePlugin::FadeFactor( double  iFadeFactor )
{
    mFadeFactor = std::clamp( iFadeFactor, 0.0, 1.0 );

    mKeys->SetValue( eParameterID_FadeFactor, &mFadeFactor ); // this will call UpdateKeyViews() where we'll be updating the GUI with the new value
}


void
cFadePlugin::NeedPreview( bool  iNeedPreview )
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
cFadePlugin::DoFade()
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
cFadePlugin::DoPreviewButton()
{
    mWindow->TV2ButtonCheckBoxState( kID_Preview, mNeedPreview );
}


void
cFadePlugin::DoPreview()
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
cFadePlugin::FlipPreview()//ZAP:
{
    NeedPreview( !NeedPreview() );
    
    if( NeedPreview() )
    {
        TV2MetaFunctionInstall( kPIMeta_Preview );
        DoPreview();
    }
    else
    {
        TV2MetaFunctionClose();
    }
    
    DoPreviewButton();
}


void
cFadePlugin::SetPreview( bool  iPreview )
{
    NeedPreview( iPreview );
    
    if( NeedPreview() )
    {
        TV2MetaFunctionInstall( kPIMeta_Preview );
        DoPreview();
    }
    else
    {
        TV2MetaFunctionClose();
    }
    
    DoPreviewButton();
}


void
cFadePlugin::Apply()
{
    if( NeedPreview() )
        TV2MetaFunctionClose();
    TV2Execute();
}


static  cFadePlugin  sgPlugin;


/**************************************************************************************/
/**************************************************************************************/

// The functions directly called by the Host through the plugin interface
//TODO: move to some special include file

extern "C" {


int FAR PASCAL
PI_Open( PIPlugin*  iPlugin )
{
    return  sgPlugin.PI2Open( iPlugin );
}


void FAR PASCAL
PI_About( PIPlugin*  iPlugin )
{
    sgPlugin.PI2About( iPlugin );
}


int FAR PASCAL
PI_Parameters( PIPlugin*  iPlugin, const char*  iArg )
{
    return  sgPlugin.PI2Parameters( iPlugin, iArg );
}


void FAR PASCAL
PI_Close( PIPlugin*  iPlugin )
{
    sgPlugin.PI2Close( iPlugin );
}


int FAR PASCAL
PI_Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs )
{
    return  sgPlugin.PI2Msg( iPlugin, iEvent, iReq, iArgs );
}


int FAR PASCAL
PI_SequenceStart( PIPlugin*  iPlugin, int  iCount )
{
    return  sgPlugin.PI2SequenceStart( iPlugin, iCount );
}


int FAR PASCAL
PI_Start( PIPlugin*  iPlugin, double  iPos, double  iSize )
{
    return  sgPlugin.PI2Start( iPlugin, iPos, iSize );
}


int FAR PASCAL
PI_Work( PIPlugin*  iPlugin )
{
    return  sgPlugin.PI2Work( iPlugin );
}


void FAR PASCAL
PI_Finish( PIPlugin*  iPlugin )
{
    sgPlugin.PI2Finish( iPlugin );
}


void FAR PASCAL
PI_SequenceFinish( PIPlugin*  iPlugin )
{
    sgPlugin.PI2SequenceFinish( iPlugin );
}


} // extern "C"

