/******************************************************************************/
/*                                                                            */
/*          Copyright (c) TVPaint Developpement. All Rights Reserved.         */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

// ===========================================================================
/** @file videodevice.h

         @par Library

    @author Sebastien Miglio & Hervé ADAM
       $Date:   $
         $Id:   $
         @par   Copyright (c) 2001 TVPaint Developpement. All Rights Reserved.
*/
// ===========================================================================
//////////////////////////////////////////////////////////////////////////////
//
//      Video Device Header - Example For Aura 2
//
//      PI_VideoDevice.h
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __myvideodevice_HH
#define __myvideodevice_HH


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

#include  "TVPaintSDK.h"


//----------------------------------------------------------
//  LOCALISATION
//----------------------------------------------------------

#define TXT_NAME    GetLocalString(PiFilter, 100,  "VideoDeviceName")
#define TXT_WINDOW  GetLocalString(PiFilter, 10000,  "VideoDevicePanel")

//----------------------------------------------------------
typedef struct ImageList
{
    struct ImageList *mNext;
    PIBlock *mImage;
}ImageList;

typedef struct
{
    PIDisplay  Display;
    PIFilter*  PiFilter;

    ///////////////////////////////////////
    // put your datas here
    ///////////////////////////////////////
    INTPTR mReq;
    PIBlock* mButtonImage;  // ref

    PIBlock* mImage;        // ref -> mButtonImage or mGfxImage

    // Dsk
    int mDskIsOn;
    int mDSKDev;
    int mDSKInput;
    int mDSKMode;
    PIVideoDevice* mVDev;
    PIVideoDeviceID   mVDevID;
    PIBlock*     mVideoImage;
    PIBlock*     mGfxImage;


    // Streaming
    ImageList* mImageFree;
    ImageList* mImageReady;

    CRITICAL_SECTION mLock;
    MMRESULT  mTimer;
    UINT      mTimerRes;

} OutputDisplay;

typedef struct
{
    int open;
    int sitx;
    int sity;

    PIDisplay* mDisplay;
    PIVideoDevice* mVideoIn;
} UserParam;

#define WINDOW_W 200
#define WINDOW_H 100



//----------------------------------------------------------
// videodevice_ouput.c
//----------------------------------------------------------
void CloseVideoDevice(PIFilter *PiFilter, PIDisplay *Display);
PIDisplay *OpenVideoDevice(PIFilter *PiFilter);

int MyStretchVideoBlock(OutputDisplay *vd, PIBlock *oDest, const PIVideoBlock *iSource, double iSource_X, double iSource_Y, int iDest_X, int iDest_Y, int iDest_Width, int iDest_Height, double iRatio_X, double iRatio_Y);


//----------------------------------------------------------
// videokey.c
//----------------------------------------------------------

int KeyOn(OutputDisplay* iDisplay, const char *iCmd);
void KeyOff(OutputDisplay* iDisplay);


#endif //myvideodevice_HH
