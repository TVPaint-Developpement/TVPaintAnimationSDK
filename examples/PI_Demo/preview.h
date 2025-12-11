/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  preview.h
*  @ingroup  tvpa_demo_filter
*
*  @brief  A button for displaying previews.
*/

#ifndef __preview_H
#define __preview_H


#ifndef __TVPaintSDK_HH
#include "TVPaintSDK.h"
#endif


typedef struct Preview
{
    INTPTR  mReq;
    int     mID;
    int     mX;
    int     mY;
    int     mSize;

    PIBlock*  mSrc;
    PIBlock*  mDst;

    // methods
    int  (*StartPreview)( struct Preview*  ioPreview, PIPlugin*  iFilter );
    int  (*FinishPreview)( struct Preview*  ioPreview, PIPlugin*  iFilter );
} Preview;


extern  Preview*  PreviewNew( PIPlugin*  iFilter, INTPTR  iReq, int  iID, int  iX, int  iY, int  iSize );
extern  void      PreviewDelete( PIPlugin*  iFilter, Preview*  iPreview );


#endif // __preview_H
