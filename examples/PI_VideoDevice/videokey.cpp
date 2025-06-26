/******************************************************************************/
/*                                                                            */
/*          Copyright (c) TVPaint Developpement. All Rights Reserved.         */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

// ===========================================================================
/** @file videokey.c

         @par   Library

      @author   Hervé ADAM
       $Date:   $
         $Id:   $
         @par   Copyright (c) 2001 TVPaint Developpement. All Rights Reserved.
*/
// ===========================================================================

#include "myvideodevice.h"


static int
GrabDsk(PIVideoBlock *vblock, void *UserData)
{
    OutputDisplay* display = (OutputDisplay*)UserData;

    int sh = vblock->Height * ((vblock->Field==1 || vblock->Field==2) ? 2 : 1);

    // copy to ButtonImage
    MyStretchVideoBlock(display, display->mButtonImage, vblock, 0, 0, 0, 0, 
                                display->mButtonImage->Width, display->mButtonImage->Height, vblock->Width/(double)display->mButtonImage->Width, sh/(double)display->mButtonImage->Height);

    // blend with GfxImage width ButtonImage
    TVColorBlend(display->PiFilter, display->mButtonImage->Data, 255, display->mGfxImage->Data, 255, display->mButtonImage->Data, display->mVideoImage->Width, display->mVideoImage->Height, 0, 0, 0);

    // refresh Button
    TVRefreshButtonImage(display->PiFilter, display->mReq, 1, 0, 0, display->mImage->Width, display->mImage->Height);
    return 1;
}


int
KeyOn(OutputDisplay* iDisplay, const char *iCmd)
{
    int devnum   = 0;
    int inputnum = 0;
    int modenum  = 0;

    sscanf(iCmd, "%d %d %d", &devnum, &inputnum, &modenum);

    if(iDisplay->mDskIsOn && iDisplay->mDSKDev==devnum && iDisplay->mDSKInput==inputnum && iDisplay->mDSKMode==modenum)
        return 1;

    KeyOff(iDisplay);

    iDisplay->mDSKDev   = devnum;
    iDisplay->mDSKInput = inputnum;
    iDisplay->mDSKMode  = modenum;


    iDisplay->mVDev = TVEnumVideoDevice(iDisplay->PiFilter, devnum);
    if(iDisplay->mVDev==0)return 0;

    
    iDisplay->mVideoImage = TVAllocPIBlock(iDisplay->PiFilter, iDisplay->mImage->Width, iDisplay->mImage->Height, 0);
    if(iDisplay->mVideoImage==NULL)
    {
        return 0;
    }

    iDisplay->mGfxImage = TVAllocPIBlock(iDisplay->PiFilter, iDisplay->mImage->Width, iDisplay->mImage->Height, 0);
    if(iDisplay->mGfxImage==NULL)
    {
        return 0;
    }


    iDisplay->mVDevID = iDisplay->mVDev->Start(iDisplay->mVDev, inputnum, modenum, GrabDsk, iDisplay);
    if(iDisplay->mVDevID)
    {
        iDisplay->mDskIsOn = 1;

        iDisplay->mImage = iDisplay->mGfxImage;
    }
    else
    {
        iDisplay->mVideoImage->free(iDisplay->mVideoImage);
        iDisplay->mVideoImage = 0;
        iDisplay->mGfxImage->free(iDisplay->mGfxImage);
        iDisplay->mGfxImage = 0;
    }

    return iDisplay->mDskIsOn;
}

//----------------------------------------------------------------------------------
void
KeyOff(OutputDisplay* iDisplay)
{
    if(iDisplay->mDskIsOn==0)return;

    iDisplay->mVDev->Stop(iDisplay->mVDev, iDisplay->mVDevID);

    iDisplay->mImage = iDisplay->mButtonImage;


    iDisplay->mGfxImage->free(iDisplay->mGfxImage);
    iDisplay->mGfxImage = 0;

    iDisplay->mVideoImage->free(iDisplay->mVideoImage);
    iDisplay->mVideoImage = 0;


    iDisplay->mDskIsOn = 0;

    iDisplay->mVDev = 0;
    iDisplay->mVDevID = 0;
}

//----------------------------------------------------------------------------------
