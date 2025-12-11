/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  aadraw.h
*
*  @brief  Antialiased drawing.
*/

#ifndef __aadraw_HH
#define __aadraw_HH

#include "TVPaintAnimationSDK/TVPaintSDK.h"

#ifdef __cplusplus
extern "C" {
#endif

void  AADrawLinePIBlock( PIBlock*  ioBlock, int  iX1, int  iY1, int  iX2, int  iY2, PIPixel32  iColor );
void  AADrawArcPIBlock( PIBlock*  ioBlock, int  iCenterX, int  iCenterY, int  iRadius, double  iAngle1, double  iAngle2, PIPixel32  iColor );

#ifdef __cplusplus
}
#endif

#endif //__aadraw_HH
