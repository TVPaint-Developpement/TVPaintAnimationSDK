/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Preview.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A button to show a small preview.
 */


#include "TVPaintAnimationSDK/Button-Preview.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonPreview


cTV2ButtonPreview::~cTV2ButtonPreview()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );

    if( mSrc )
        TVFreePIBlock( plugin, mSrc );
    if( mDst )
        TVFreePIBlock( plugin, mDst );
    mSrc = mDst = nullptr;
}


cTV2ButtonPreview::cTV2ButtonPreview( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iSize ) :
    cTV2Button( iWindow, iID, iArea ),
    mSize( iSize )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddButtonReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, bPIButtonFlags_Invert, "" ) )
        return;
}


tErrorCode
cTV2ButtonPreview::ClearPreview()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVPutButtonImage( plugin, req, mID, nullptr, 0 );

    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonPreview::StartPreview()
{
printf(">> cTV2ButtonPreview::StartPreview()\n");
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    if( mSrc )
        TVFreePIBlock( plugin, mSrc );
    mSrc = MakeIcon();
    if( !mSrc )
        return  kErrorCode_LegacyError;

    if( mDst )
        TVFreePIBlock( plugin, mDst );
    mDst = TVAllocPIBlock( plugin, mSrc->Width, mSrc->Height, 0 );
    if( !mDst )
    {
        TVFreePIBlock( plugin, mSrc );
        return  kErrorCode_LegacyError;
    }

printf("<< cTV2ButtonPreview::StartPreview()\n");
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonPreview::FinishPreview()
{
printf(">> cTV2ButtonPreview::FinishPreview()\n");
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    // Doesn't work anymore since blending modes break this old paradigm...
//    if( MakeBGBlock( mDst ) )
//{printf("ERROR MakeBGBlock()\n");
//        return  kErrorCode_LegacyError;
//}
//    if( MakeFGBlock( mDst ) )
//{printf("ERROR MakeFGBlock()\n");
//        return  kErrorCode_LegacyError;
//}

    TVPutButtonImage( plugin, req, mID, mDst, 0 ); // tvpaint takes ownership of block
    mDst = nullptr;

printf("<< cTV2ButtonPreview::FinishPreview()\n");
    return  kErrorCode_NoError;
}


PIBlock*
cTV2ButtonPreview::MakeIcon() const
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    // here we do simple nearest-neighbor downscaling, doing nice looking scaling is not the point of this example code...

    PIBlock*  image = plugin->Current;

    double  r = mSize / double(std::max( image->Width, image->Height ));
    PIBlock*  icon = TVAllocPIBlock( plugin, int(image->Width*r), int(image->Height*r), 0 );
    if( !icon )
        return  nullptr;

    double    ix = image->Width  / double(icon->Width);
    double    iy = image->Height / double(icon->Height);
    double    sy = 0;
    PIPixel*  dst = icon->Data;

    for( int  y = 0; y < icon->Height; ++y )
    {
        PIPixel*  src = image->Data + int(sy)*image->Width;
        double    sx = 0;

        for( int  x = 0; x < icon->Width; ++x )
        {
            *dst++ = src[int(sx)];
            sx += ix;
        }
        sy += iy;
    }

    return  icon;
}


tErrorCode
cTV2ButtonPreview::MakeBGBlock( PIBlock*  ioImg ) const
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    PIPixel*  line = (PIPixel*)malloc( plugin->ImageWidth*sizeof(PIPixel) );
    if( !line )
        return  kErrorCode_LegacyError;

    double  steph = plugin->ImageWidth /double(ioImg->Width);
    double  stepv = plugin->ImageHeight/double(ioImg->Height);

    for( int  y = 0; y < ioImg->Height; ++y )
    {
        double    pos = 0.0;
        PIPixel*  data = &ioImg->Data[y*ioImg->Width];

        if( TVReadLayerData( plugin, line, 0, int(y*stepv), plugin->ImageWidth, 1, 0, kPIReadLayer_Current/*kPIReadLayer_Back*/ ) == 0 )
            break;

        for( int  x = ioImg->Width; x; --x )
        {
            PIPixel  p = line[int(pos)];

            if( data->Alpha != 255 )
            {
                if( data->Alpha == 0 )
                {
                    *data = p;
                }
                else
                {
                    int a = 255-data->Alpha;
                    data->Red   += (p.Red   * a)/255;
                    data->Green += (p.Green * a)/255;
                    data->Blue  += (p.Blue  * a)/255;
                    data->Alpha += (p.Alpha * a)/255;
                }

            }
            data++;
            pos += steph;
        }
    }

    free( line );

    char  result[512];
    TVSendCmd( plugin, "tv_BackGround", result );
    if( !strncasecmp( result, "color", 5 )  ||  !strncasecmp( result, "check", 5 ) )
    {
        int  ch = !strncasecmp( result, "check", 5 );

        PIPixel*  data = ioImg->Data;
        PIUInt8*  r = &data->Red;
        PIUInt8*  g = &data->Green;
        PIUInt8*  b = &data->Blue;
        PIUInt8*  a = &data->Alpha;

        int  red1, green1, blue1;
        int  red2, green2, blue2;
        sscanf( &result[5], "%d %d %d %d %d %d", &red1, &green1, &blue1, &red2, &green2, &blue2 );

        for( int  y = 0; y < ioImg->Height; ++y )
        {
            for( int  x = ioImg->Width; x; --x )
            {
                if( ch  &&  (((x>>4)+(y>>4))&1) )
                {
                    if( *a == 0 )
                    {
                        *r = red2;
                        *g = green2;
                        *b = blue2;
                        *a = 255;
                    }
                    else if( *a != 255 )
                    {
                        int  na = 255 - *a;
                        *r += red2  *na/255;
                        *g += green2*na/255;
                        *b += blue2 *na/255;
                        *a = 255;
                    }

                }
                else
                {
                    if( *a == 0 )
                    {
                        *r = red1;
                        *g = green1;
                        *b = blue1;
                        *a = 255;
                    }
                    else if( *a != 255 )
                    {
                        int  na = 255 - *a;
                        *r += red1  *na/255 ;
                        *g += green1*na/255;
                        *b += blue1 *na/255;
                        *a = 255;
                    }
                }
                r += 4;
                g += 4;
                b += 4;
                a += 4;
            }
        }
    }
    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonPreview::MakeFGBlock( PIBlock*  ioImg ) const
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();

    PIPixel*  line = (PIPixel*)malloc( plugin->ImageWidth*sizeof(PIPixel) );
    if( !line )
        return  kErrorCode_LegacyError;

    double  steph = plugin->ImageWidth /double(ioImg->Width);
    double  stepv = plugin->ImageHeight/double(ioImg->Height);

    for( int  y = 0; y < ioImg->Height; ++y )
    {
        PIPixel*  data = &ioImg->Data[y*ioImg->Width];
        double    pos = 0.0;

        if( TVReadLayerData( plugin, line, 0, int(y*stepv), plugin->ImageWidth, 1, 0, kPIReadLayer_Current/*kPIReadLayer_Front*/ ) == 0 )
            break;

        for( int  x = ioImg->Width; x; --x )
        {
            PIPixel  p = line[int(pos)];

            if( p.Alpha != 0 )
            {
                if( p.Alpha == 255 )
                {
                    *data = p;
                }
                else
                {
                    int  a = 255-p.Alpha;
                    data->Red   = p.Red   + (data->Red   * a)/255;
                    data->Green = p.Green + (data->Green * a)/255;
                    data->Blue  = p.Blue  + (data->Blue  * a)/255;
                    data->Alpha = p.Alpha + (data->Alpha * a)/255;
                }
            }
            data++;
            pos += steph;
        }
    }

    free( line );
    return  kErrorCode_NoError;
}



} // namespace  nSDK

