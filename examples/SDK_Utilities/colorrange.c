

#include "colorrange.h"

#include <stdio.h>
#include <stdlib.h>


static void
ColorRangeLoad( ColorRange* ioRange, PIFilter* iFilter )
{
    int  a;

    for( a = 0; a < 256; a += 32 )
    {
        char   tmp[128];
        char   val[1024];
        char*  t = val;
        int    b;

        for( b = 0; b < 32; ++b )
        {
            snprintf( val + b*8, sizeof(val) - b*8, "%02x%02x%02x%02x",
                      ioRange->mColors[a+b].Red,
                      ioRange->mColors[a+b].Green,
                      ioRange->mColors[a+b].Blue,
                      ioRange->mColors[a+b].Alpha );
        }

        snprintf( tmp, sizeof(tmp), "range%d", a );

        TVReadUserString( iFilter, iFilter->PIName, tmp, val, val, 1024 );

        for( b = 0; b < 32; ++b )
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
            ioRange->mColors[a+b].Red   = rr;
            ioRange->mColors[a+b].Green = gg;
            ioRange->mColors[a+b].Blue  = bb;
            ioRange->mColors[a+b].Alpha = aa;

            t += 8;
        }
    }
}


static void
ColorRangeSave( const ColorRange* iRange, PIFilter* iFilter )
{
    int  a;

    for( a = 0; a < 256; a += 32 )
    {
        char  tmp[128];
        char  val[1024];
        int   b;

        for( b = 0; b < 32; ++b )
        {
            snprintf( val + b*8, sizeof(val) - b*8, "%02x%02x%02x%02x",
                      iRange->mColors[a+b].Red,
                      iRange->mColors[a+b].Green,
                      iRange->mColors[a+b].Blue,
                      iRange->mColors[a+b].Alpha );
        }

        snprintf( tmp, sizeof(tmp), "range%d", a );
        TVWriteUserString( iFilter, iFilter->PIName, tmp, val );
    }
}



static void
ColorRangeGrab( ColorRange* oRange, PIFilter* iFilter )
{
    int  y;

    int  red   = iFilter->PlaneOffsetRed;
    int  green = iFilter->PlaneOffsetGreen;
    int  blue  = iFilter->PlaneOffsetBlue;
    int  alpha = iFilter->PlaneOffsetAlpha;

    PIPixel*  p = oRange->mColors;
    for( y = 0; y < 256; ++y )
    {
        char  cmd[256];
        char  result[256];
        int   r, g, b, a;

        snprintf( cmd, sizeof(cmd), "tv_rangegrab %d", y );
        TVSendCmd( iFilter, cmd, result );
        sscanf( result, "%d %d %d %d", &r, &g, &b, &a );
        p->c[red]   = (r*a)/255;
        p->c[green] = (g*a)/255;
        p->c[blue]  = (b*a)/255;
        p->c[alpha] = a;
        p++;
    }
}


static PIBlock*
ColorRangeMakeBlock( const ColorRange* iRange, PIFilter* iFilter, int w, int h )
{
    int  r = iFilter->PlaneOffsetRed;
    int  g = iFilter->PlaneOffsetGreen;
    int  b = iFilter->PlaneOffsetBlue;
    int  a = iFilter->PlaneOffsetAlpha;

    int     x;
    double  step;
    double  p = 0;

    PIBlock*  block = TVAllocPIBlock( iFilter, w, h, 0 );
    if( !block )
        return  NULL;

    step = 256.0/w;
    for( x = 0; x < w; x++ )
    {
        int       y;
        PIPixel*  tmp = block->Data + x;
        PIPixel   src = iRange->mColors[(int)p];
        int       alpha = 255-src.c[a];
        PIPixel   pix1, pix2;

        pix1.c[r] = src.c[r];
        pix1.c[g] = src.c[g];
        pix1.c[b] = src.c[b];
        pix1.c[a] = 255;

        pix2.c[r] = src.c[r] + alpha;
        pix2.c[g] = src.c[g] + alpha;
        pix2.c[b] = src.c[b] + alpha;
        pix2.c[a] = 255;

        for( y = 0; y < h; ++y )
        {
            *tmp = (((x>>2) + (y>>2)) & 1) ? pix1 : pix2;
            tmp += w;
        }
        p += step;
    }

    return  block;
}




ColorRange*
ColorRangeNew( PIFilter* iFilter )
{
    int  i;
    ColorRange*  colors = (ColorRange*)malloc( sizeof(ColorRange) );

    if( !colors )
        return  NULL;

    for( i = 0; i < 256; ++i )
    {
        colors->mColors[i].Red   = i;
        colors->mColors[i].Green = i;
        colors->mColors[i].Blue  = i;
        colors->mColors[i].Alpha = 255;
    }

    colors->Load      = ColorRangeLoad;
    colors->Save      = ColorRangeSave;
    colors->Grab      = ColorRangeGrab;
    colors->MakeBlock = ColorRangeMakeBlock;

    return  colors;
}


void
ColorRangeDelete( PIFilter* iFilter, ColorRange* iRange )
{
    if( iRange )
    {
        free( iRange );
    }
}
