/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  aadraw.c
*
*  @brief  Antialiased drawing.
*/

#include "aadraw.h"

#include "block.h"

#include <stdlib.h>
#include <math.h>

#ifdef WIN32
#include <malloc.h>
#endif


static  void  BlendPixel( PIBlock*  ioBlock, int  iX, int  iY, PIPixel32  iColor, int  iV )
{
    if( iV == 0  ||  iX < 0  ||  iY < 0  ||  iX >= ioBlock->Width  ||  iY >= ioBlock->Height )
        return;

    PIPixel*  p = &ioBlock->Data[iX + iY*ioBlock->Width];

    PIPixel color;
    color.l = iColor;
    color.c[0] = (color.c[0]*iV)>>8;
    color.c[1] = (color.c[1]*iV)>>8;
    color.c[2] = (color.c[2]*iV)>>8;
    color.c[3] = (color.c[3]*iV)>>8;

    int  a = color.Alpha;
    if( a == 255 )
    {
        *p = color;
    }
    else if( a )
    {
        a = 255 - a;
        p->c[0] = color.c[0] + BlinnMult( p->c[0], a );
        p->c[1] = color.c[1] + BlinnMult( p->c[1], a );
        p->c[2] = color.c[2] + BlinnMult( p->c[2], a );
        p->c[3] = color.c[3] + BlinnMult( p->c[3], a );
    }
}


void  AADrawLinePIBlock( PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel32  iColor )
{
    if( iX1 == iX2  &&  iY1 == iY2 )
    {
        BlendPixel( ioBlock, iX1, iY1, iColor, 256 );
        return;
    }

    int  dx = iX2 - iX1;
    int  dy = iY2 - iY1;

    int  deltax = abs( dx );
    int  deltay = abs( dy );

    int  xinc = (dx > 0) ? 1 : -1;
    int  yinc = (dy > 0) ? 1 : -1;

    int  x = iX1;
    int  y = iY1;

    BlendPixel( ioBlock, x, y, iColor, 256 );

    if( deltax >= deltay )
    {
        int  d = 0;
        int  ys;

        for( int  i = 1; i <= deltax; ++i )
        {
            x += xinc;
            d += deltay;
            if( d <= 0 )
            {
                ys = y-yinc;
            }
            else
            {
                if( d-deltax < 0 )
                {
                    if( d+d-deltax <= 0 )
                    {
                        ys = y+yinc;
                    }
                    else
                    {
                        ys = y;
                        y = y+yinc;
                        d = d-deltax;
                    }
                }
                else
                {
                    y = y+yinc;
                    ys = y+yinc;
                    d = d-deltax;
                }
            }
            int  ip = (d<<8)/deltax;
            if( ip < 0 )
                ip = -ip;

            BlendPixel( ioBlock, x, y, iColor, 256-ip );
            BlendPixel( ioBlock, x, ys, iColor, ip );
        }
    }
    else
    {
        int  d = 0;
        int  xs;

        for( int  i = 1; i <= deltay; ++i )
        {
            y = y+yinc;
            d = d+deltax;
            if( d <= 0 )
            {
                xs = x-xinc;
            }
            else
            {
                if( d-deltay < 0 )
                {
                    if( d+d-deltay <= 0 )
                    {
                        xs = x + xinc;
                    }
                    else
                    {
                        xs = x;
                        x = x+xinc;
                        d = d-deltay;
                    }
                }
                else
                {
                    x = x+xinc;
                    xs = x+xinc;
                    d = d-deltay;
                }
            }

            int  ip = (d<<8)/deltay;
            if( ip < 0 )
                ip = -ip;

            BlendPixel( ioBlock, x, y, iColor, 256-ip );
            BlendPixel( ioBlock, xs, y, iColor, ip );
        }
    }
}


void  AADrawArcPIBlock( PIBlock*  ioBlock, int  iCenterX, int  iCenterY, int  iRadius, double  iAngle1, double  iAngle2, PIPixel32  iColor )
{
    double  step = (iAngle2-iAngle1)/20.0;
    int  ox = (int)(cos( iAngle1 )*iRadius);
    int  oy = (int)(sin( iAngle1 )*iRadius);

    if( iAngle1 < iAngle2 )
    {
        for( double  a = iAngle1; a < iAngle2; a += step )
        {
            int  x = (int)(cos( a )*iRadius);
            int  y = (int)(sin( a )*iRadius);
            AADrawLinePIBlock( ioBlock, iCenterX-ox, iCenterY-oy, iCenterX-x, iCenterY-y, iColor );
            ox = x;
            oy = y;
        }
        int  x = (int)(cos( iAngle2 )*iRadius);
        int  y = (int)(sin( iAngle2 )*iRadius);
        AADrawLinePIBlock( ioBlock, iCenterX-ox, iCenterY-oy, iCenterX-x, iCenterY-y, iColor );
    }
    else
    {
        for( double  a = iAngle1; a > iAngle2; a += step )
        {
            int  x = (int)(cos( a )*iRadius);
            int  y = (int)(sin( a )*iRadius);
            AADrawLinePIBlock( ioBlock, iCenterX-ox, iCenterY-oy, iCenterX-x, iCenterY-y, iColor );
            ox = x;
            oy = y;
        }
        int  x = (int)(cos( iAngle2 )*iRadius);
        int  y = (int)(sin( iAngle2 )*iRadius);
        AADrawLinePIBlock( ioBlock, iCenterX-ox, iCenterY-oy, iCenterX-x, iCenterY-y, iColor );
    }
}

