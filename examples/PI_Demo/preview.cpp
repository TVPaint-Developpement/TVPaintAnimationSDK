/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  preview.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  A button for displaying previews.
*/

#include "preview.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MAX
#define MAX(a, b) ((a)>(b) ? (a) : (b))
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#endif


static PIBlock*
MakeIcon( PIPlugin* iFilter, PIBlock* iSrc, int iSize )
{
    // here we do simple nearest-neighbor downscaling, doing nice looking scaling is not the point of this example code...

    double  r = iSize / (double)MAX( iSrc->Width, iSrc->Height );

    PIBlock*  icon = TVAllocPIBlock( iFilter, (int)(iFilter->ImageWidth*r), (int)(iFilter->ImageHeight*r), 0 );

    if( !icon )
        return  NULL;

    int       y;
    double    ix = iSrc->Width /(double)icon->Width;
    double    iy = iSrc->Height/(double)icon->Height;
    double    sy = 0;
    PIPixel*  dst = icon->Data;

    for( y = 0; y < icon->Height; ++y )
    {
        int       x;
        PIPixel*  src = iSrc->Data + (int)sy*iSrc->Width;
        double    sx = 0;

        for( x = 0; x < icon->Width; ++x )
        {
            *dst++ = src[(int)sx];
            sx += ix;
        }
        sy += iy;
    }
    return  icon;
}


static int
MakeBGBlock( PIPlugin* iFilter, PIBlock* ioImg )
{
    char  result[512];

    PIPixel*  line = (PIPixel*)malloc( iFilter->ImageWidth*sizeof(PIPixel) );
    if( !line )
        return  0;

    int  y;

    double  steph = iFilter->ImageWidth /(double)ioImg->Width;
    double  stepv = iFilter->ImageHeight/(double)ioImg->Height;

    int  pr = ioImg->r;
    int  pg = ioImg->g;
    int  pb = ioImg->b;
    int  pa = ioImg->a;

    for( y = 0; y < ioImg->Height; ++y )
    {
        int  x;

        double    pos = 0.0;
        PIPixel*  data = &ioImg->Data[y*ioImg->Width];

        if( TVReadLayerData( iFilter, line, 0, (int)(y*stepv), iFilter->ImageWidth, 1, 0, kPIReadLayer_Current/*kPIReadLayer_Back*/ ) == 0 )
            break;

        for( x = ioImg->Width; x; --x )
        {
            PIPixel  p = line[(int)pos];

            if( data->c[pa] != 255 )
            {
                if( data->c[pa] == 0 )
                {
                    *data = p;
                }
                else
                {
                    int a = 255-data->c[pa];
                    data->c[pr] += (p.c[pr]*a)/255;
                    data->c[pg] += (p.c[pg]*a)/255;
                    data->c[pb] += (p.c[pb]*a)/255;
                    data->c[pa] += (p.c[pa]*a)/255;
                }

            }
            data++;
            pos += steph;
        }
    }

    free( line );

    TVSendCmd( iFilter, "tv_BackGround", result );
    if( !strncasecmp( result, "color", 5 )  ||  !strncasecmp( result, "check", 5 ) )
    {
        int  red1, green1, blue1;
        int  red2, green2, blue2;
        int  na;

        int  ch = !strncasecmp( result, "check", 5 );

        PIPixel*  data = ioImg->Data;
        PIUInt8*  r = &data->Red;
        PIUInt8*  g = &data->Green;
        PIUInt8*  b = &data->Blue;
        PIUInt8*  a = &data->Alpha;

        sscanf( &result[5], "%d %d %d %d %d %d", &red1, &green1, &blue1, &red2, &green2, &blue2 );

        for( y = 0; y < ioImg->Height; ++y )
        {
            int  x;
            for( x = ioImg->Width; x; --x )
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
                        na = 255 - *a;
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
                        na = 255 - *a;
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
    return  1;
}


static int
MakeFGBlock( PIPlugin* iFilter, PIBlock* ioImg )
{
    PIPixel*  line = (PIPixel*)malloc( iFilter->ImageWidth*sizeof(PIPixel) );
    if( !line )
        return  0;

    int  y;

    double  steph = iFilter->ImageWidth /(double)ioImg->Width;
    double  stepv = iFilter->ImageHeight/(double)ioImg->Height;

    int  pr = ioImg->r;
    int  pg = ioImg->g;
    int  pb = ioImg->b;
    int  pa = ioImg->a;

    for( y = 0; y < ioImg->Height; ++y )
    {
        int  x;

        PIPixel*  data = &ioImg->Data[y*ioImg->Width];
        double    pos = 0.0;

        if( TVReadLayerData( iFilter, line, 0, (int)(y*stepv), iFilter->ImageWidth, 1, 0, kPIReadLayer_Current/*kPIReadLayer_Front*/ ) == 0 )
            break;

        for( x = ioImg->Width; x; --x )
        {
            PIPixel  p = line[(int)pos];

            if( p.c[pa] != 0 )
            {
                if( p.c[pa] == 255 )
                {
                    *data = p;
                }
                else
                {
                    int  a = 255-p.c[pa];
                    data->c[pr] = p.c[pr] + (data->c[pr]*a)/255;
                    data->c[pg] = p.c[pg] + (data->c[pg]*a)/255;
                    data->c[pb] = p.c[pb] + (data->c[pb]*a)/255;
                    data->c[pa] = p.c[pa] + (data->c[pa]*a)/255;
                }
            }
            data++;
            pos += steph;
        }
    }

    free( line );
    return  1;
}


static int
StartPreview( Preview* ioPreview, PIPlugin* iFilter )
{
    if( ioPreview->mSrc )
        TVFreePIBlock( iFilter, ioPreview->mSrc );

    ioPreview->mSrc = MakeIcon( iFilter, iFilter->Current, ioPreview->mSize );

    if( !ioPreview->mSrc )
        return  0;

    if( ioPreview->mDst )
        TVFreePIBlock( iFilter, ioPreview->mDst );

    ioPreview->mDst = TVAllocPIBlock( iFilter, ioPreview->mSrc->Width, ioPreview->mSrc->Height, 0 );   /* Creates the PIBlock (Destination) for storing theresult */

    if( !ioPreview->mDst )
    {
        TVFreePIBlock( iFilter, ioPreview->mSrc );
        return  0;
    }

    return  1;
}


static int
FinishPreview( Preview* ioPreview, PIPlugin* iFilter )
{
    if( !MakeBGBlock( iFilter, ioPreview->mDst ) )
        return  0;
    if( !MakeFGBlock( iFilter, ioPreview->mDst ) )
        return  0;

    TVPutButtonImage( iFilter, ioPreview->mReq, ioPreview->mID, ioPreview->mDst, 0 );
    ioPreview->mDst = 0;

    return  1;
}


////////////////////////////////////////////////////////////////////////////////////////


Preview*
PreviewNew( PIPlugin* iFilter, INTPTR iReq, int iID, int iX, int iY, int iSize )
{
    Preview*  preview = (Preview*)malloc( sizeof(Preview) );

    preview->mReq  = iReq;
    preview->mID   = iID;
    preview->mX    = iX;
    preview->mY    = iY;
    preview->mSize = iSize;

    preview->mSrc = NULL;
    preview->mDst = NULL;

    preview->StartPreview  = StartPreview;
    preview->FinishPreview = FinishPreview;

    TVAddButtonReq( iFilter, iReq, iX, iY, iSize, iSize, iID, bPIButtonFlags_Invert|bPIButtonFlags_Quiet, NULL );

    return  preview;
}


void
PreviewDelete( PIPlugin* iFilter, Preview* iPreview )
{
    TVRemoveButtonReq( iFilter, iPreview->mReq, iPreview->mID );

    if( iPreview->mSrc )
        TVFreePIBlock( iFilter, iPreview->mSrc );

    if( iPreview->mDst )
        TVFreePIBlock( iFilter, iPreview->mDst );

    free( iPreview );
}
