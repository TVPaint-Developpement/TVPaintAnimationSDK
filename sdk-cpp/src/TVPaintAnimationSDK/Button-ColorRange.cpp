/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-ColorRange.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show/select a color range.
 */


#include "TVPaintAnimationSDK/Button-ColorRange.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ColorRange


cTV2ColorRange::cTV2ColorRange( cTV2Plugin*  iPlugin ) :
    mPlugin( iPlugin )
{
    assert( mPlugin );

    for( int  i = 0; i < kCount; ++i )
    {
        mColors[i].Red   = i;
        mColors[i].Green = i;
        mColors[i].Blue  = i;
        mColors[i].Alpha = 255;
    }
}


void
cTV2ColorRange::Load()
{
    for( int  a = 0; a < kCount; a += 32 )
    {
        char   tmp[128];
        char   val[1024];
        char*  t = val;

        for( int  b = 0; b < 32; ++b )
        {
            snprintf( val + b*8, sizeof(val) - b*8, "%02x%02x%02x%02x",
                      mColors[a+b].Red,
                      mColors[a+b].Green,
                      mColors[a+b].Blue,
                      mColors[a+b].Alpha );
        }

        snprintf( tmp, sizeof(tmp), "range%d", a );

        TVReadUserString( mPlugin->LegacyPlugin(), mPlugin->NameID().c_str(), tmp, val, val, 1024 );

        for( int  b = 0; b < 32; ++b )
        {
            int  rr, gg, bb, aa;
            tmp[0]  = t[0];
            tmp[1]  = t[1];
            tmp[2]  = ' ';
            tmp[3]  = t[2];
            tmp[4]  = t[3];
            tmp[5]  = ' ';
            tmp[6]  = t[4];
            tmp[7]  = t[5];
            tmp[8]  = ' ';
            tmp[9]  = t[6];
            tmp[10] = t[7];
            tmp[11] ='\0';

            sscanf( tmp, "%x %x %x %x", &rr, &gg, &bb, &aa );
            mColors[a+b].Red   = rr;
            mColors[a+b].Green = gg;
            mColors[a+b].Blue  = bb;
            mColors[a+b].Alpha = aa;

            t += 8;
        }
    }
}


void
cTV2ColorRange::Save() const
{
    for( int  a = 0; a < kCount; a += 32 )
    {
        char  tmp[128];
        char  val[1024];

        for( int  b = 0; b < 32; ++b )
        {
            snprintf( val + b*8, sizeof(val) - b*8, "%02x%02x%02x%02x",
                      mColors[a+b].Red,
                      mColors[a+b].Green,
                      mColors[a+b].Blue,
                      mColors[a+b].Alpha );
        }

        snprintf( tmp, sizeof(tmp), "range%d", a );
        TVWriteUserString( mPlugin->LegacyPlugin(), mPlugin->NameID().c_str(), tmp, val );
    }
}


void
cTV2ColorRange::Grab()
{
    PIPixel*  p = mColors;
    for( int  x = 0; x < kCount; ++x )
    {
        char  cmd[256];
        char  result[256];

        snprintf( cmd, sizeof(cmd), "tv_rangegrab %d", x );
        TVSendCmd( mPlugin->LegacyPlugin(), cmd, result );
        int  r, g, b, a;
        sscanf( result, "%d %d %d %d", &r, &g, &b, &a );
        p->Red   = (r*a)/255;
        p->Green = (g*a)/255;
        p->Blue  = (b*a)/255;
        p->Alpha = a;
        ++p;
    }
}


void
cTV2ColorRange::RenderToBlock( PIBlock*  ioBlock ) const
{
    double  p = 0;

    double  step = 256.0/ioBlock->Width;
    for( int  x = 0; x < ioBlock->Width; x++ )
    {
        PIPixel*  tmp = ioBlock->Data + x;
        PIPixel   src = mColors[int(p)];
        int       alpha = 255 - src.Alpha;
        PIPixel   pix1, pix2;

        pix1.Red   = src.Red;
        pix1.Green = src.Green;
        pix1.Blue  = src.Blue;
        pix1.Alpha = 255;

        pix2.Red   = src.Red   + alpha;
        pix2.Green = src.Green + alpha;
        pix2.Blue  = src.Blue  + alpha;
        pix2.Alpha = 255;

        for( int  y = 0; y < ioBlock->Height; ++y )
        {
            *tmp = (((x>>2) + (y>>2)) & 1) ? pix1 : pix2;
            tmp += ioBlock->Width;
        }
        p += step;
    }
}


PIBlock*
cTV2ColorRange::CreateBlock( int  iW, int  iH ) const
{
    PIBlock*  block = TVAllocPIBlock( mPlugin->LegacyPlugin(), iW, iH, 0 );
    if( !block )
        return  nullptr;

    RenderToBlock( block );

    return  block;
}


//MARK: ========================================================================================================= cTV2ButtonColorRange


cTV2ButtonColorRange::~cTV2ButtonColorRange()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonColorRange::cTV2ButtonColorRange( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, cTV2ColorRange  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueValue( iValue ),
    mValuePtr( &mValueValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;

    mImage = TVAllocPIBlock( plugin, mArea.W()-2, mArea.H()-2, 0 );
    mValuePtr->RenderToBlock( mImage );
    TVPutButtonImage( plugin, req, iID, mImage, 0 );
}


cTV2ButtonColorRange::cTV2ButtonColorRange( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, cTV2ColorRange*  iValue, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueValue( *iValue ),
    mValuePtr( iValue ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;

    mImage = TVAllocPIBlock( plugin, mArea.W()-2, mArea.H()-2, 0 );
    mValuePtr->RenderToBlock( mImage );
    TVPutButtonImage( plugin, req, iID, mImage, 0 );
}


void
cTV2ButtonColorRange::ColorRange( const cTV2ColorRange&  iColorRange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    *mValuePtr = iColorRange;
    mValuePtr->RenderToBlock( mImage );
    TVRefreshButtonImage( plugin, req, mID, 0, 0, Area().W(), Area().H() );
}


tErrorCode
cTV2ButtonColorRange::EventButtonUp( int  iButtonID, const nSDK::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
{
    assert( iButtonID == mID );

    mValuePtr->Grab();

    ColorRange( *mValuePtr );
    mValueChange( mID, *mValuePtr );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

