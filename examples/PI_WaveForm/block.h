/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  block.h
*
*  @brief  Utilities to work with PIBlock.
*/

#ifndef __block_HH
#define __block_HH


#include "TVPaintAnimationSDK/TVPaintSDK.h"


#ifdef __cplusplus
extern "C" {
#endif


PIBlock*  AllocBlockIcon( PIPlugin*  iPlugin, int  iSize );
PIBlock*  FaitBlockIcon( PIPlugin*  iPlugin, PIBlock*  iImage, int iSize );
PIBlock*  MakeIcon( PIPlugin*  iPlugin, int  iSize );
PIBlock*  MakeIconCurrent( PIPlugin*  iPlugin, int  iSize );
void  ComputeGradient( PIPixel*  oPixels, int  iCount, PIPixel  iColor1, PIPixel  iColor2 );
void  MakeBackground( PIPlugin*  iPlugin, PIBlock*  ioIcon );
void  PIBlockCopyRect( PIPlugin* iPlugin, const PIBlock *source, int xs, int ys, PIBlock *dest, int xd, int yd, int w, int h );
void  PIBlockCopyRectClip( PIPlugin*  iPlugin, const PIBlock*  iSrc, int  iSrcX, int  iSrcY, PIBlock*  ioDst, int  iDstX, int  iDstY, int  iW, int  iH );
void  PIBlockBlendRectClip( PIPlugin*  iPlugin, const PIBlock*  iSrc, int  iSrcX, int  iSrcY, PIBlock*  ioDst, int  iDstX, int  iDstY, int  iW, int  iH, int  iDensity );
void  PIBlockFill( PIPlugin*  iPlugin, PIBlock*  oBlock, PIPixel32  iColor );
void  DrawLineEx( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor, int  iFirst );
void  DrawLine( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor );
void  DrawRectangle( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel  iColor );
void  FillBlockAlpha( PIPlugin*  iPlugin, PIBlock*  ioBlock );
void  ColorizePixels( PIPixel*  ioSrc, PIPixel  iColor, int  iSize );
void  FillBlockWithGradient( PIPlugin*  iPlugin, PIBlock*  ioBlock, const PIBlock*  iGradient, int  iX, int  iY, int  iWidth, int  iHeight );
void  FillLineWithGradient( PIPixel*  ioBlock, const PIPixel*  iColor, int  iCount, float  iStart, float  iStep );


static  __inline  int  BlinnMult( int  iA, int  iB )
{
    // a * b / 255
    int  i = iA * iB +128;
    return  (i + (i>>8))>>8;
}


#ifndef MIN
#define  MIN( iA, iB )  (((iA) < (iB) ) ? (iA) : (iB))
#endif
#ifndef MAX
#define  MAX( iA, iB )  (((iA) > (iB) ) ? (iA) : (iB))
#endif
#ifndef CLAMP
#define  CLAMP( iV, iA, iB )  ((iV) < (iA) ? (iA) : ((iV) > (iB) ? (iB) : (iV)))
#endif

#ifdef __cplusplus
}
#endif


#endif //block_HH
