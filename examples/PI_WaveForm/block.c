/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  block.c
*
*  @brief  Utilities to work with PIBlock.
*/


#include "block.h"

#include "TVPaintAnimationSDK/TVPaintSDK.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __NT__
#define	strncasecmp strnicmp
#define	strcasecmp stricmp
#endif

#ifdef WIN32
#include <malloc.h>
#endif


/*---------------------------------------------------------------------------------------------------------------------- AllocBlockIcon() */

PIBlock*
AllocBlockIcon( PIPlugin*  iPlugin, int  iSize )
{
    double  ratio = iSize / (double)MAX( iPlugin->ImageWidth, iPlugin->ImageHeight );

    return  TVAllocPIBlock( iPlugin, (int)(iPlugin->ImageWidth*ratio), (int)(iPlugin->ImageHeight*ratio), 0 );
}

/*---------------------------------------------------------------------------------------------------------------------- FaitBlockIcon() */

PIBlock*
FaitBlockIcon( PIPlugin*  iPlugin, PIBlock*  iImage, int iSize )
{
    double  ratio = iSize / (double)MAX( iImage->Width, iImage->Height );
    PIBlock*  icon = TVAllocPIBlock( iPlugin, (int)(iPlugin->ImageWidth*ratio), (int)(iPlugin->ImageHeight*ratio), 0 );
    if( icon == NULL )
        return NULL;

    double  ix = iImage->Width/(double)icon->Width;
    double  iy = iImage->Height/(double)icon->Height;
    double  sy = 0;
    PIPixel*  pd = icon->Data;
    for( int  y = 0; y < icon->Height; ++y )
    {
        double  sx = 0;
        PIPixel*  ps = &iImage->Data[ (int)(sy)*iImage->Width ];
        for( int  x = 0; x < icon->Width; ++x )
        {
            *pd++ = ps[ (int)(sx) ];
            sx += ix;
        }
        sy += iy;
    }

    return  icon;
}

/*---------------------------------------------------------------------------------------------------------------------- MakeIcon() */

PIBlock*
MakeIcon( PIPlugin*  iPlugin, int  iSize )
{
    int  ww = iPlugin->WorkArea_x2 - iPlugin->WorkArea_x1 + 1;
    int  wh = iPlugin->WorkArea_y2 - iPlugin->WorkArea_y1 + 1;

    double  ratio = iSize / (double)MAX( ww, wh );

    int  iw = (int)(ww*ratio);
    int  ih = (int)(wh*ratio);

    PIBlock*  icon = TVAllocPIBlock( iPlugin, iw, ih, 0 );
    if( !icon )
        return  NULL;

    PIPixel*  pix = (PIPixel*)malloc( ww*sizeof(PIPixel) );
    if( !pix )
    {
        TVFreePIBlock( iPlugin, icon );
        TVWarning( iPlugin, "No memory" );
        return  NULL;
    }

    int  step = (ww<<16)/iw;

    for( int  y = 0; y < icon->Height; ++y )
    {
        int  yy = (int)(y/ratio);
        TVReadLayerData( iPlugin, pix, iPlugin->WorkArea_x1, yy+iPlugin->WorkArea_y1, ww, 1, 0, kPIReadLayer_Current );
        PIPixel*  data = &icon->Data[y*icon->Width];
        int  pos = 0;

        for( int  x = icon->Width; x; --x )
        {
            *data++ = pix[pos>>16];
            pos += step;
        }
    }

    char  result[512];
    TVSendCmd( iPlugin, "tv_BackGround", result );
    if( !strncasecmp( result, "color", 5 )  ||  !strncasecmp( result, "check", 5 ) )
    {
        int  red, green, blue;
        sscanf( &result[5], "%d %d %d", &red, &green, &blue );

        PIPixel*  data = icon->Data;
        unsigned char*  r = &data->c[(int)icon->r];
        unsigned char*  g = &data->c[(int)icon->g];
        unsigned char*  b = &data->c[(int)icon->b];
        unsigned char*  a = &data->c[(int)icon->a];
        for( int  y = 0; y < icon->Height; ++y )
        {
            for( int  x = icon->Width; x; --x )
            {
                if( *a == 0 )
                {
                    *r = red;
                    *g = green;
                    *b = blue;
                    *a = 255;
                }
                else if( *a != 255 )
                {
                    int  na = 255 - *a;
                    *r += red   * na / 255 ;
                    *g += green * na / 255;
                    *b += blue  * na / 255;
                    *a = 255;
                }
                r += 4;
                g += 4;
                b += 4;
                a += 4;
            }
        }
    }

    free( pix );
    return  icon;
}

/*---------------------------------------------------------------------------------------------------------------------- MakeIconCurrent() */
PIBlock*
MakeIconCurrent( PIPlugin*  iPlugin, int  iSize )
{
    int  ww = iPlugin->WorkArea_x2 - iPlugin->WorkArea_x1 + 1;
    int  wh = iPlugin->WorkArea_y2 - iPlugin->WorkArea_y1 + 1;

    double  ratio = iSize / (double)MAX( ww, wh );

    int  iw = (int)(ww*ratio);
    int  ih = (int)(wh*ratio);

    PIBlock*  icon = TVAllocPIBlock( iPlugin, iw, ih, 0 );
    if( !icon )
        return  NULL;

    PIPixel*  pix = (PIPixel*)malloc( ww*sizeof(PIPixel) );
    if( !pix )
    {
        TVFreePIBlock( iPlugin, icon );
        TVWarning( iPlugin, "No memory" );
        return  NULL;
    }

    int  step = (ww<<16)/iw;

    for( int  y = 0; y < icon->Height; ++y )
    {
        int  yy = (int)(y/ratio);
        TVReadLayerData( iPlugin, pix, iPlugin->WorkArea_x1, iPlugin->WorkArea_y1+yy, ww, 1, 0, kPIReadLayer_Current );
        PIPixel*  data = &icon->Data[ y*icon->Width ];
        int  pos = 0;

        for( int  x = icon->Width; x; --x )
        {
            *data++ = pix[pos>>16];
            pos += step;
        }
    }

    free( pix );
    return  icon;
}

/*---------------------------------------------------------------------------------------------------------------------- ComputeGradient() */

void
ComputeGradient( PIPixel*  oPixels, int  iCount, PIPixel  iColor1, PIPixel  iColor2 )
{
    double  i0 = iColor1.Red;
    double  i1 = iColor1.Green;
    double  i2 = iColor1.Blue;
    double  i3 = iColor1.Alpha;

    double  s0 = (iColor2.Red   - iColor1.Red  ) / (double)iCount;
    double  s1 = (iColor2.Green - iColor1.Green) / (double)iCount;
    double  s2 = (iColor2.Blue  - iColor1.Blue ) / (double)iCount;
    double  s3 = (iColor2.Alpha - iColor1.Alpha) / (double)iCount;

    for( int  i = iCount; i; --i )
    {
        oPixels->Red   = (PIUInt8)i0;
        oPixels->Green = (PIUInt8)i1;
        oPixels->Blue  = (PIUInt8)i2;
        oPixels->Alpha = (PIUInt8)i3;

        i0 += s0;
        i1 += s1;
        i2 += s2;
        i3 += s3;

        ++oPixels;
    }
}

/*---------------------------------------------------------------------------------------------------------------------- MakeBackground() */

void
MakeBackground( PIPlugin*  iPlugin, PIBlock*  ioIcon )
{
    char  result[512];
    TVSendCmd( iPlugin, "tv_BackGround", result );
    if( !strncasecmp( result, "color", 5 )  ||  !strncasecmp( result, "check", 5 ) )
    {
        int  red, green, blue;
        sscanf( &result[5], "%d %d %d", &red, &green, &blue );

        PIPixel*  data = ioIcon->Data;
        PIUInt8*  r = &data->c[ (int)ioIcon->r ];
        PIUInt8*  g = &data->c[ (int)ioIcon->g ];
        PIUInt8*  b = &data->c[ (int)ioIcon->b ];
        PIUInt8*  a = &data->c[ (int)ioIcon->a ];
        for( int  y = 0; y < ioIcon->Height; ++y )
        {
            for( int  x = ioIcon->Width; x; --x )
            {
                if( *a == 0 )
                {
                    *r = red;
                    *g = green;
                    *b = blue;
                    *a = 255;
                }
                else if( *a != 255 )
                {
                    int  na = 255 - *a;
                    *r += red   * na / 255 ;
                    *g += green * na / 255;
                    *b += blue  * na / 255;
                    *a  = 255;
                }

                r += 4;
                g += 4;
                b += 4;
                a += 4;
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------------- PIBlockCopyRect() */
void
PIBlockCopyRect( PIPlugin*  iPlugin, const PIBlock*  iSrc, int  iSrcX, int  iSrcY, PIBlock*  ioDst, int  iDstX, int  iDstY, int  iW, int  iH )
{
    const PIPixel*  s = &iSrc->Data[ iSrcX + iSrcY*iSrc->Width ];
    PIPixel*  d = &ioDst->Data[ iDstX + iDstY*ioDst->Width ];

    for( int  y = 0; y < iH; ++y )
    {
        for( int  x = iW; x; --x )
        {
            *d++ = *s++;
        }
        s += iSrc->Width-iW;
        d += ioDst->Width-iW;
    }
}

/*---------------------------------------------------------------------------------------------------------------------- PIBlockCopyRect() */

void
PIBlockCopyRectClip( PIPlugin*  iPlugin, const PIBlock*  iSrc, int  iSrcX, int  iSrcY, PIBlock*  ioDst, int  iDstX, int  iDstY, int  iW, int  iH )
{
    if( iDstX + iW > ioDst->Width )
    {
        if( iDstX >= ioDst->Width )
            return;
        iW = ioDst->Width - iDstX;
    }
    if( iDstY + iH > ioDst->Height )
    {
        if( iDstY >= ioDst->Height )
            return;
        iH = ioDst->Height - iDstY;
    }

    if( iDstX < 0 )
    {
        iSrcX -= iDstX;
        iW += iDstX;
        iDstX = 0;
    }
    if( iDstY < 0 )
    {
        iSrcY -= iDstY;
        iH += iDstY;
        iDstY = 0;
    }

    if( iSrcX < 0 )
    {
        iDstX += -iSrcX;
        iW += iSrcX;
        iSrcX = 0;
    }
    if( iSrcY < 0 )
    {
        iDstY += -iSrcY;
        iH += iSrcY;
        iSrcY = 0;
    }

    if( iSrcX + iW > iSrc->Width )
    {
        if( iSrcX > iSrc->Width )
            return;
        iW = iSrc->Width-iSrcX;
    }
    if( iSrcY + iH > iSrc->Height )
    {
        if( iSrcY > iSrc->Height )
            return;
        iH = iSrc->Height-iSrcY;
    }

    if( iW <= 0  ||  iH <= 0 )
        return;

    PIBlockCopyRect( iPlugin, iSrc, iSrcX, iSrcY, ioDst, iDstX, iDstY, iW, iH );
}

/*---------------------------------------------------------------------------------------------------------------------- PIBlockBlendRectClip() */

void
PIBlockBlendRectClip( PIPlugin*  iPlugin, const PIBlock*  iSrc, int  iSrcX, int  iSrcY, PIBlock*  ioDst, int  iDstX, int  iDstY, int  iW, int  iH, int  iDensity )
{
    if( iDstX + iW > ioDst->Width )
    {
        if( iDstX >= ioDst->Width )
            return;
        iW = ioDst->Width - iDstX;
    }
    if( iDstY + iH > ioDst->Height )
    {
        if( iDstY >= ioDst->Height )
            return;
        iH = ioDst->Height - iDstY;
    }

    if( iDstX < 0 )
    {
        iSrcX -= iDstX;
        iW += iDstX;
        iDstX = 0;
    }
    if( iDstY < 0 )
    {
        iSrcY -= iDstY;
        iH += iDstY;
        iDstY = 0;
    }

    if( iSrcX < 0 )
    {
        iDstX += -iSrcX;
        iW += iSrcX;
        iSrcX = 0;
    }
    if( iSrcY < 0 )
    {
        iDstY += -iSrcY;
        iH += iSrcY;
        iSrcY = 0;
    }

    if( iSrcX + iW > iSrc->Width )
    {
        if( iSrcX > iSrc->Width )
            return;
        iW = iSrc->Width-iSrcX;
    }
    if( iSrcY + iH > iSrc->Height )
    {
        if( iSrcY > iSrc->Height )
            return;
        iH = iSrc->Height-iSrcY;
    }

    if( iW <= 0  ||  iH <= 0 )
        return;

    const PIPixel*  s = &iSrc->Data[ iSrcX + iSrcY*iSrc->Width];
    PIPixel*  d = &ioDst->Data[ iDstX + iDstY*ioDst->Width];

    TVColorBlend( iPlugin,
                  d, 255,
                  s, iDensity,
                  d, iW, iH, ioDst->Width-iW, iSrc->Width-iW, ioDst->Width-iW );
}

/*---------------------------------------------------------------------------------------------------------------------- PIBlockFill() */
void
PIBlockFill( PIPlugin*  iPlugin, PIBlock*  oBlock, PIPixel32  iColor )
{
    int  x = oBlock->Width * oBlock->Height;
    PIPixel*  p = oBlock->Data;

    while( x-- )
    {
        p->l = iColor;
        p++;
    }
}

/*----------------------------------------------------------------------------------------------------------------------  */

static  void
GoLineBlk( PIPlugin*  iPlugin, int  iCount, PIPixel*  oPoint, int  iD, int  iA, int  iB, int  iIncA, int  iIncB, PIPixel  iColor )
{
    while( iCount-- >= 0 )
    {
        *oPoint = iColor;
        if( iD >= 0 )
        {
            iD += iA;
            oPoint += iIncA;
        }
        else
        {
            iD += iB;
            oPoint += iIncB;
        }
    }
}

/*----------------------------------------------------------------------------------------------------------------------  */

static  void
GoLineBlkDensity( PIPlugin*  iPlugin, int  iCount, PIPixel*  oPoint, int  iD, int  iA, int  iB, int  iIncA, int  iIncB, PIPixel  iColor )
{
    if( iColor.l == 0 )
        return;

    int  pr = iPlugin->PlaneOffsetRed;
    int  pg = iPlugin->PlaneOffsetGreen;
    int  pb = iPlugin->PlaneOffsetBlue;
    int  pa = iPlugin->PlaneOffsetAlpha;

    int  vr = iColor.c[pr];
    int  vg = iColor.c[pg];
    int  vb = iColor.c[pb];
    int  va = iColor.c[pa];

    int  na = 255 - va;

    while( iCount-- >= 0 )
    {
        if( oPoint->c[pa] == 0 )
        {
            *oPoint = iColor;
        }
        else
        {
            oPoint->c[pr] = (oPoint->c[pr]*na/255) + vr;
            oPoint->c[pg] = (oPoint->c[pg]*na/255) + vg;
            oPoint->c[pb] = (oPoint->c[pb]*na/255) + vb;
            oPoint->c[pa] = (oPoint->c[pa]*na/255) + va;
        }
        if( iD >= 0 )
        {
            iD += iA;
            oPoint += iIncA;
        }
        else
        {
            iD += iB;
            oPoint += iIncB;
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------------- DrawLineEx() */

void
DrawLineEx( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor, int  iFirst )
{
    int  a = iX2 - iX1;
    int  b = iY2 - iY1;

    int  inc1x = 1;
    if( a <= 0 )
    {
        inc1x = -1;
        a = -a;
    }

    int  inc1y = 1;
    if( b <= 0 )
    {
        inc1y = -1;
        b = -b;
    }

    int  inc2x = inc1x;
    int  inc2y = 0;
    if( b > a )
    {
        inc2x = 0;
        inc2y = inc1y;
        int  c = a;
        a = b;
        b = c;
    }
    int  count = a;

    int  d = b+b-a;
    b = b+b;
    a = a+a;
    a = b-a;

    PIPixel*  point = &ioBlock->Data[ iX1 + iY1*ioBlock->Width];

    inc1x = inc1x + inc1y*ioBlock->Width;
    inc2x = inc2x + inc2y*ioBlock->Width;

    if( !iFirst )
    {
        if( d >= 0 )
        {
            d += a;
            point += inc1x;
        }
        else
        {
            d += b;
            point += inc2x;
        }
        count--;
    }
    if( iColor.c[ (int)ioBlock->a ] != 255 )
        GoLineBlkDensity( iPlugin, count, point, d, a, b, inc1x, inc2x, iColor );
    else
        GoLineBlk( iPlugin, count, point, d, a, b, inc1x, inc2x, iColor );
}

/*---------------------------------------------------------------------------------------------------------------------- DrawLine() */

void
DrawLine( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor )
{
    DrawLineEx( iPlugin, ioBlock, iX1, iY1, iX2, iY2, iColor, 1 );
}

/*---------------------------------------------------------------------------------------------------------------------- DrawRectangle() */

void
DrawRectangle( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor )
{
    if( iX1 > iX2 )
    {
        int  t = iX1;
        iX1 = iX2;
        iX2 = t;
    }
    if( iY1 > iY2 )
    {
        int  t = iY1;
        iY1 = iY2;
        iY2 = t;
    }

    if( iX2 < 0  ||  iY2 < 0  ||  iX1 >= ioBlock->Width  ||  iY1 >= ioBlock->Height )
        return;

    if( iX1 < 0 )  iX1 = 0;
    if( iY1 < 0 )  iY1 = 0;
    if( iX2 >= ioBlock->Width )  iX2 = ioBlock->Width-1;
    if( iY2 >= ioBlock->Height )  iY2 = ioBlock->Height-1;

    PIPixel*  p = &ioBlock->Data[ iX1 + iY1*ioBlock->Width ];

    int  w = iX2 - iX1 + 1;
    int  h = iY2 - iY1 + 1;

    int  s = ioBlock->Width - w;

    for( int  y = h; y; --y )
    {
        for( int  x = w; x; --x )
        {
            *p = iColor;
            ++p;
        }
        p += s;
    }
}

/*---------------------------------------------------------------------------------------------------------------------- FillBlockAlpha() */

void
FillBlockAlpha( PIPlugin*  iPlugin, PIBlock*  ioBlock )
{
    PIPixel*  p = ioBlock->Data;
    int  a = ioBlock->a;
    for( int  x = ioBlock->Width*ioBlock->Height; x; --x, ++p )
    {
        p->c[a] = 255;
    }
}

/*---------------------------------------------------------------------------------------------------------------------- ColorizePixels() */

void
ColorizePixels( PIPixel*  ioSrc, PIPixel  iColor, int  iSize )
{
    PIPixel	 map[256];

    for( int  i = 0; i < 256; ++i )
    {
        map[i].Red   = BlinnMult( iColor.Red,   i );
        map[i].Green = BlinnMult( iColor.Green, i );
        map[i].Blue  = BlinnMult( iColor.Blue,  i );
        map[i].Alpha = BlinnMult( iColor.Alpha, i );
    }

    for( int  x = 0; x < iSize; ++x )
    {
        ioSrc[x] = map[ ioSrc[x].Alpha ];
    }
}

/*---------------------------------------------------------------------------------------------------------------------- FillBlockWithGradient() */

void
FillBlockWithGradient( PIPlugin*  iPlugin, PIBlock*  ioBlock, const PIBlock*  iGradient, int  iX, int  iY, int  iWidth, int  iHeight )
{
    TVWrapPIBlockMode( iPlugin, iGradient, ioBlock,
                       iX, iY, iX+iWidth, iY, iX+iWidth,
                       iY + iHeight, iX, iY + iHeight,
                       0, PIWRAP_MODE_PRESERVE | PIWRAP_FAST );
}

//YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE:
//YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE:
//YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE://YOUAREHERE:
/*----------------------------------------------------------------------------------------------------------------------  */

void
FillLineWithGradient( PIPixel*  ioBlock, const PIPixel*  iColor, int  iCount, float  iStart, float  iStep )
{
    float  pos = iStart;
    for( int  i = 0; i < iCount; ++i, pos += iStep )
    {
        int  a = ioBlock[i].Alpha;
        int  p = (int)( pos );

        ioBlock[i].Red   = BlinnMult( iColor[p].Red,   a );
        ioBlock[i].Green = BlinnMult( iColor[p].Green, a );
        ioBlock[i].Blue	 = BlinnMult( iColor[p].Blue,  a );
        ioBlock[i].Alpha = BlinnMult( iColor[p].Alpha, a );
    }
}
