/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  gamma.c
*
*  @brief  A better gamma function then the default one (which in fact only handles the gamma==1.0 case).
*  Not used in this example but you can extend it as an exercise to include gamma, black and white controls.
*/

#include <math.h>

#define  CLAMP( iV, iA, iB )  ((iV) < (iA) ? (iA) : ((iV) > (iB) ? (iB) : (iV)))

static  int  ComputeGammaTable( double  iGamma, int  iBlack, int  iWhite, double*  oTable )
{
    iGamma = 1.0/iGamma;

    int  size = iWhite - iBlack;
    if( size <= 1 )
        return  0;

    oTable[0] = iBlack;

    for( int  y = 1; y < 256; ++y )
    {
        double  tt = log( y / 255. ) * iGamma;
        double  t = exp( tt ) * size + 0.5;
        oTable[y] = iBlack + CLAMP( t, 0, size );
    }

    return  1;
}
