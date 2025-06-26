/******************************************************************************/
/*                                                                            */
/*          Copyright (c) TVPaint Developpement. All Rights Reserved.         */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

// ===========================================================================
/** @file videoin.cpp

         @par   Library

      @author   Hervé ADAM
       $Date:   $
         $Id:   $
         @par   Copyright (c) 2001 TVPaint Developpement. All Rights Reserved.
*/
// ===========================================================================
#include <Windows.h>
#include <stdio.h>
#include <malloc.h>

//-----------------------------------------------------------------

#include  "TVPaintSDK.h"


//-----------------------------------------------------------------
typedef struct MyVideoDevice
{
    PIVideoDevice mVDev;
    PIFilter    *mPiFilter;
}MyVideoDevice;

//-----------------------------------------------------------------

class NoiseDevice
{
public:
    NoiseDevice(PIVideoInfo *iVInfo, int (*iGrabThread)(PIVideoBlock *iVBlock, void *iUserData), void *iUserData, int iInputNumber, int iModeNumber);
    ~NoiseDevice();
    void Thread();


private:
    PIVideoInfo mVideoInfo;
    PIUInt8 *mNoise;

    int (*mGrabThread)(PIVideoBlock *iVBlock, void *iUserData);
    void *mUserData;

    bool mThreadIsRunning;
    bool mNeedExit;

    PIVideoBlock mVideoBlock;

    int mInputNumber;
    int mModeNumber;
};

//-----------------------------------------------------------------

void NoiseDevice::Thread()
{
    int line=0;
    srand(GetTickCount());
    while(mNeedExit==false)
    {
        for(int f=0; f<2; f++)
        {
            DWORD t0 = GetTickCount();
            mVideoBlock.Field = (1-f)+1;

            if(mInputNumber==0)
            {
                int pos = rand()%(mVideoBlock.Width*mVideoBlock.Height);
                mVideoBlock.Data = &mNoise[pos * mVideoBlock.BytesPerPixel];//(rand()%(pos))&(~7)];
            }
            else
            {
                mVideoBlock.Data = &mNoise[(line+f)*mVideoBlock.Width*mVideoBlock.BytesPerPixel];
            }

            mVideoBlock.Field0 = mVideoBlock.Data;
            mVideoBlock.Field1 = mVideoBlock.Data;

            mGrabThread(&mVideoBlock, mUserData);

            DWORD t1 = GetTickCount();
            long t = long(t0 + 1000/(mVideoInfo.FrameRate*2)) - t1;
            if(t>0)Sleep(t);
        }
        line+=2;
        if(line>=mVideoBlock.Height)line=0;
    }
    mNeedExit=false;
    mThreadIsRunning=false;
}

//-----------------------------------------------------------------

static WINAPI ThreadFonc(void *pv)
{
    ((NoiseDevice *)pv)->Thread();
    return 0;
}

//-----------------------------------------------------------------
NoiseDevice::NoiseDevice(PIVideoInfo *iVInfo, int (*iGrabThread)(PIVideoBlock *iVBlock, void *iUserData), void *iUserData,
                         int iInputNumber, int iModeNumber):
    mNoise(NULL),
    mVideoInfo(*iVInfo),
    mGrabThread(iGrabThread),
    mUserData(iUserData),
    mThreadIsRunning(false),
    mNeedExit(false),
    mInputNumber(iInputNumber),
    mModeNumber(iModeNumber)
{
    int Pitch = mVideoInfo.Width  * mVideoInfo.BytesPerPixel;
    int size  = mVideoInfo.Height * Pitch;

    mNoise=(PIUInt8*)malloc(size + Pitch*2);
    if(mNoise==NULL)return;

    int h = (mVideoInfo.Height / 2);
    int v = rand()&0x7f + 128;

    for(int y=0; y<h; y++)
    {
        PIUInt8 *t = &mNoise[y * Pitch];
        for(int x=mVideoInfo.Width; x; x--)
        {
            if( (rand()&0x1f) == 0)
            {
                v = rand()%112 + 128;
            }
            if(mVideoInfo.PixelType==PIXTYPE_UYVY)
            {
                t[0] = 128 + (rand()&0xf) - 8;  // uv
                t[1] = v;                       // y
                t +=2;
            }
            else
            {
                t[0] = v;
                t[1] = v;
                t[2] = v;
                t[3] = 255;
                t+=4;
            }
            if(v>128)v-=16;
            v -= 16;
            if(v<16)v=16;
        }
    }
    memcpy(&mNoise[h*Pitch], mNoise, h*Pitch);

    memset(&mVideoBlock, 0, sizeof(mVideoBlock));
    mVideoBlock.Width           = mVideoInfo.Width;
    mVideoBlock.Height          = mVideoInfo.Height/2;
    mVideoBlock.BytesPerPixel   = mVideoInfo.BytesPerPixel;
    mVideoBlock.Pitch           = Pitch;
    mVideoBlock.PixelType       = mVideoInfo.PixelType;

    mVideoBlock.FieldMode       = iVInfo->FieldMode;
    mVideoBlock.AspectRatio     = iVInfo->AspectRatio;


    DWORD id;
    mThreadIsRunning=true;
    CreateThread(NULL,              // address of thread security attributes  
                    64*1024,        // initial thread stack size, in bytes 
                    (LPTHREAD_START_ROUTINE)ThreadFonc, // address of thread function 
                    (LPVOID)this,   // argument for new thread 
                    0,              // creation flags 
                    &id             // address of returned thread identifier 
                );
}

//-----------------------------------------------------------------
NoiseDevice::~NoiseDevice()
{
    while(mThreadIsRunning)
    {
        mNeedExit=true;
        Sleep(10);
    }
    if(mNoise)free(mNoise);
}

//-----------------------------------------------------------------

static const char* TEnumInputs(PIVideoDevice *iVDev, int InputNum)
{
    switch(InputNum)
    {
        case 0: return "Random";
        case 1: return "Scroll";
        default: return NULL;
    }
}


static int TEnumModes(PIVideoDevice *iVDev, int InputNum, int iModeNum, PIVideoInfo *iInfo)
{
    MyVideoDevice *MyVDev=(MyVideoDevice*)iVDev;

    memset(iInfo, 0, sizeof(PIVideoInfo));

    switch(iModeNum)
    {
        case 0:
            strcpy(iInfo->Name, "YUV NTSC 720x486");
            iInfo->Width=720;
            iInfo->Height=486;
            iInfo->AspectRatio = 1/0.900;
            iInfo->FrameRate = 29.970;
            iInfo->BytesPerPixel = 2;
            iInfo->PixelType=PIXTYPE_UYVY;
            iInfo->FieldMode = 1;
            break;
        case 1:
            strcpy(iInfo->Name, "YUV PAL 720x576");
            iInfo->Width=720;
            iInfo->Height=576;
            iInfo->AspectRatio = 1/1.067;
            iInfo->FrameRate = 25;
            iInfo->BytesPerPixel=2;
            iInfo->PixelType=PIXTYPE_UYVY;
            iInfo->FieldMode = 2;
            break;
        case 2:
            strcpy(iInfo->Name, "RGBA NTSC 720x486");
            iInfo->Width=720;
            iInfo->Height=486;
            iInfo->AspectRatio = 1/0.900;
            iInfo->FrameRate = 29.970;
            iInfo->BytesPerPixel=4;
            iInfo->PixelType=PIXTYPE_BGRA32;
            iInfo->FieldMode = 1;
            break;
        case 3:
            strcpy(iInfo->Name, "RGBA PAL 720x576");
            iInfo->Width=720;
            iInfo->Height=576;
            iInfo->AspectRatio = 1/1.067;
            iInfo->FrameRate = 25;
            iInfo->BytesPerPixel=4;
            iInfo->PixelType=PIXTYPE_BGRA32;
            iInfo->FieldMode = 2;
            break;
        default:
            return 0;
    }
    return 1;
}

//-----------------------------------------------------------------
static PIVideoDeviceID TStart(PIVideoDevice *VDev, int iInputNumber, int iModeNumber, int (*iGrabThread)(PIVideoBlock *iVBlock, void *iUserData), void *iUserData)
{
    if(iInputNumber>7)return 0;

    PIVideoInfo VInfo;
    TEnumModes(VDev, iInputNumber, iModeNumber, &VInfo);

    NoiseDevice *tdev=new NoiseDevice(&VInfo, iGrabThread, iUserData, iInputNumber, iModeNumber);
    return tdev;
}

//-----------------------------------------------------------------
static void TStop(PIVideoDevice *iVDev, PIVideoDeviceID VDevID)
{
    NoiseDevice *tdev=(NoiseDevice *)VDevID;

    delete tdev;
}

//-----------------------------------------------------------------
static void TConfigure(PIVideoDevice *iVDev)
{
    MyVideoDevice *MyVDev=(MyVideoDevice*)iVDev;

    PI_Parameters(MyVDev->mPiFilter, NULL);
}

//-----------------------------------------------------------------

extern "C"
PIVideoDevice *InitNoiseVideoDevice(PIFilter *iPifilter)
{
    //----------------------------------------------------
    MyVideoDevice *VDev0=(MyVideoDevice*)malloc(sizeof(MyVideoDevice));
    if(VDev0==NULL)return NULL;
    memset(VDev0, 0, sizeof(MyVideoDevice));

    VDev0->mVDev.Version    = PIVIDEODEVICE_VERSION;
    VDev0->mVDev.DeviceName = "Video Noise";
    VDev0->mVDev.EnumModes  = TEnumModes;
    VDev0->mVDev.EnumInputs = TEnumInputs;
    VDev0->mVDev.Start      = TStart;
    VDev0->mVDev.Stop       = TStop;
    VDev0->mVDev.Configure  = TConfigure;
    VDev0->mPiFilter        = iPifilter;
    TVAddVideoDevice(iPifilter, (PIVideoDevice*)VDev0);

    //----------------------------------------------------
    return (PIVideoDevice*)VDev0; 
}
//-----------------------------------------------------------------
