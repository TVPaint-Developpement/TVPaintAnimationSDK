/******************************************************************************/
/*                                                                            */
/*          Copyright (c) TVPaint Developpement. All Rights Reserved.         */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

// ===========================================================================
/** @file videodevice_output.c

         @par   Library

      @author   Sebastien Miglio & Hervé ADAM
       $Date:   $
         $Id:   $
         @par   Copyright (c) 2001 TVPaint Developpement. All Rights Reserved.
*/
// ===========================================================================

//----------------------------------------------------------
//
//      Video Device Output - Example For Aura 2
//
//      VideoDevice_output.c
//
//----------------------------------------------------------

#include <math.h>
#include "myvideodevice.h"

static OutputDisplay* sgDisplay;


static int ReqPort(PIFilter *PiFilter, INTPTR fonc ,INTPTR req,INTPTR *tag)
{
    switch(fonc)
    {
        case PICBREQ_CLOSE:
        {
            KeyOff(sgDisplay);
            sgDisplay->mReq = 0;
            sgDisplay->mButtonImage = 0;
        }
        break;
    }
    return 1;
}


//----------------------------------------------------------
// Open Video Device Display
//----------------------------------------------------------
static int OpenDisplay(PIDisplay *display, int Width, int Height)
{
    OutputDisplay   *vd=(OutputDisplay *)display;

    ////////////////////////////////////////
    // open your screen here
    ////////////////////////////////////////
    InitializeCriticalSection(&vd->mLock);

    Width  = vd->Display.Width;
    Height = vd->Display.Height;

    vd->mReq = TVOpenReqEx(vd->PiFilter, Width+8, Height+8, 0, 0, PIRF_CENTER_REQ|PIRF_NO_CLOSE_REQ, "MyDisplayDevice", ReqPort);

    TVAddButtonReq(vd->PiFilter, vd->mReq, 4, 4, Width, Height, 1, PIRBF_BUTTON_INVERT|PIRBF_BUTTON_QUIET, NULL);

    vd->mButtonImage = TVAllocPIBlock(vd->PiFilter, Width, Height, 0);
    TVPutButtonImage(vd->PiFilter, vd->mReq, 1, vd->mButtonImage, 2);

    vd->mImage = vd->mButtonImage;


    return 1;
}

//----------------------------------------------------------
// Close Video Device Display
//----------------------------------------------------------
static void CloseDisplay(PIDisplay *display)
{
    OutputDisplay   *vd=(OutputDisplay *)display;

    ////////////////////////////////////////
    // close your screen here
    ////////////////////////////////////////
    KeyOff(vd);

    if(vd->mReq)
    {
        TVCloseReq(vd->PiFilter, vd->mReq);
        vd->mReq = 0;
    }
    vd->mButtonImage = 0;
    vd->mImage = 0;
    DeleteCriticalSection(&vd->mLock);
}

//----------------------------------------------------------
//----------------------------------------------------------
static void PutFreeImage(OutputDisplay *vd, ImageList *il)
{
    EnterCriticalSection(&vd->mLock);
    il->mNext = vd->mImageFree;
    vd->mImageFree = il;
    LeaveCriticalSection(&vd->mLock);
}

static ImageList *GetFreeImage(OutputDisplay *vd)
{
    ImageList *i;

    EnterCriticalSection(&vd->mLock);
    i = vd->mImageFree;
    if(i!=NULL)
    {
        vd->mImageFree = i->mNext;
    }
    LeaveCriticalSection(&vd->mLock);
    return i;
}

static void PutReadyImage(OutputDisplay *vd, ImageList *il)
{
    EnterCriticalSection(&vd->mLock);
    if(vd->mImageReady==NULL)
    {
        vd->mImageReady = il;
        il->mNext=0;
    }
    else
    {
        ImageList *i = vd->mImageReady;
        while(i->mNext)
        {
            i = i->mNext;
        }
        i->mNext  = il;
        il->mNext = NULL;
    }
    LeaveCriticalSection(&vd->mLock);
}

static ImageList *GetReadyImage(OutputDisplay *vd)
{
    ImageList *i = NULL;

    EnterCriticalSection(&vd->mLock);
    if(vd->mImageReady!=NULL)
    {
        i = vd->mImageReady;
        vd->mImageReady = i->mNext;
    }
    LeaveCriticalSection(&vd->mLock);
    return i;
}


static void CALLBACK TimeProc( UINT uID, UINT UMsg, ULONG dwUser, ULONG dw1, ULONG dw2 )
{
    OutputDisplay *vd = (OutputDisplay*)dwUser;

    if(vd->mReq==0)
    {
        return;
    }
    else
    {
        ImageList *image = GetReadyImage(vd);
        if(image)
        {
            memcpy(vd->mImage->Data, image->mImage->Data, vd->mImage->Width*vd->mImage->Height*sizeof(PIPixel));
            PutFreeImage(vd, image);

            if(vd->mDskIsOn == 0)
                TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, 0, 0, vd->mImage->Width, vd->mImage->Height);
        }
    }
}


static int StartTimer(OutputDisplay *vd)
{
    TIMECAPS tc;
    memset(  &tc,  0,    sizeof( TIMECAPS ) );
    timeGetDevCaps( &tc, sizeof( TIMECAPS ) );

    //vd->mTimerRes = max(1000 / 100 ,tc.wPeriodMin );
    vd->mTimerRes = max((int)(1000/29.997) ,tc.wPeriodMin );
    vd->mTimerRes = min(vd->mTimerRes  , tc.wPeriodMax );

    timeBeginPeriod( vd->mTimerRes );

    vd->mTimer = timeSetEvent( (int)(1000/29.997), 0, TimeProc, (LONG)vd, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
    if(!vd->mTimer)
    {
        timeEndPeriod( vd->mTimerRes );
        return 0;
    }
    return 1;
}

static void StopTimer(OutputDisplay *vd)
{
    if(vd->mTimer)
    {
        timeKillEvent(vd->mTimer);
        timeEndPeriod( vd->mTimerRes );
        vd->mTimer=0;
    }
}



static int OpenOutput(PIDisplay *iDisplay)
{
    OutputDisplay   *vd=(OutputDisplay *)iDisplay;
    int a;

    for(a=0;a<4;a++)
    {
        ImageList *il = (ImageList*)malloc(sizeof(ImageList));
        il->mImage = TVAllocPIBlock(vd->PiFilter, vd->mImage->Width, vd->mImage->Height, 0);
        PutFreeImage(vd, il);
    }
    return StartTimer(vd);
}

static int CloseOutput(PIDisplay *iDisplay)
{
    OutputDisplay   *vd=(OutputDisplay *)iDisplay;
    ImageList *il;

    while(vd->mImageReady)
    {
        Sleep(10);
    }
    StopTimer(vd);

    while(il=GetReadyImage(vd))
    {
        il->mImage->free(il->mImage);
        free(il);
    }
    while(il=GetFreeImage(vd))
    {
        il->mImage->free(il->mImage);
        free(il);
    }
    return 1;
}


//----------------------------------------------------------
// Function for writing a PIBlock
//----------------------------------------------------------
static int WritePIBlock(PIDisplay *Display, const PIBlock *piblock, 
                        int sx, int sy, int dx, int dy, int w, int h, unsigned long mode)
{
    OutputDisplay   *vd=(OutputDisplay *)Display;
    int x,y;
    PIPixel *dst;
    const PIPixel *src;

    if(vd->mReq==0)
        return 0;

    // clipping
    if(dx<0)
    {
        w = w + dx;
        sx = sx - dx;
        dx = 0;
    }

    if(dy<0)
    {
        h = h + dy;
        sy = sy-dy;
        dy = 0;
    }

    if(dx+w>vd->Display.Width)
    {
        if(dx>=vd->Display.Width)return 1;
        w=vd->Display.Width-dx;
    }

    if(dy+h>vd->Display.Height)
    {
        if(dy>=vd->Display.Height)return 1;
        h=vd->Display.Height-dy;
    }
    if(w<=0 || h<=0)return 1;

    /////////////////////////////////////
    // Write your pixels here
    /////////////////////////////////////

    dst = &vd->mImage->Data[dx + dy*vd->mImage->Width];
    src = &piblock->Data[sx + sy*piblock->Width];
    for(y=0; y<h; y++)
    {
        for(x=0;x<w; x++)
        {
            dst[x] = src[x];
        }
        dst += vd->mImage->Width;
        src += piblock->Width;
    }
    if(vd->mDskIsOn == 0)
        TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, dx, dy, w, h);

    return 1;
}

//----------------------------------------------------------
// Function for writing a single Pixel
//----------------------------------------------------------
static int WritePixel(PIDisplay *Display, int x, int y, PIPixel32 color, unsigned long mode)
{
    OutputDisplay   *vd=(OutputDisplay *)Display;

    if(vd->mReq==0)return 0;


    // clipping
    if(x<0 || y<0 || x>=vd->Display.Width || y>=vd->Display.Height)return 1;

    //////////////////////////
    // write a pixel(x,y,color) here 
    //////////////////////////
    vd->mImage->Data[x + y*vd->mImage->Width].l = color;
    if(vd->mDskIsOn == 0)
        TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, x, y, 1, 1);

    return 1;
}

//----------------------------------------------------------
// Function for drawing a filled rectangle
//----------------------------------------------------------
static int  RectFill(PIDisplay *Display, int dest_x, int dest_y, int w, int h, PIPixel32 color, unsigned long mode)
{
    OutputDisplayg*  vd=(OutputDisplay *)Display;
    int x1, y1;
    int x2, y2;
    PIPixel *dst;
    int y;

    if(vd->mReq==0)
        return 0;


    x1 = dest_x;
    y1 = dest_y;
    x2 = x1+w-1;
    y2 = y1+h-1;

    // clipping
    if(x1<0)x1=0;
    if(x2<0)return 1;

    if(y1<0)y1=0;
    if(y2<0)return 1;

    if(x1>=vd->Display.Width)return 1;
    if(x2>=vd->Display.Width)x2=vd->Display.Width-1;

    if(y1>=vd->Display.Height)return 1;
    if(y2>=vd->Display.Height)y2=vd->Display.Height-1;

    w=x2-x1+1;
    h=y2-y1+1;

    if(w<=0 || h<=0)return 1;

    //////////////////////////////////////////////////////////
    // draw a filled rectangle(x1,y1,w,h,color) here
    //////////////////////////////////////////////////////////
    dst = &vd->mImage->Data[x1 + y1*vd->mImage->Width];
    for(y=0; y<h; y++)
    {
        int x;
        for(x=0;x<w; x++)
        {
            dst[x].l = color;
        }
        dst += vd->mImage->Width;
    }
    if(vd->mDskIsOn == 0)
        TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, x1, y1, w, h);
    return 1;
}

static void* GetSourcePtr(const PIVideoBlock *iSource, int iSrcY)
{
    switch(iSource->Field)
    {
        default:
        case 0:
            return &iSource->Data[iSrcY * iSource->Pitch];
        case 1:
            return &iSource->Field1[(iSrcY/2) * iSource->Pitch];
        case 2:
            return &iSource->Field0[(iSrcY/2) * iSource->Pitch];
        case 3:
            if(iSource->FieldMode==2)
            {
                if(iSrcY&1)
                    return &iSource->Field0[(iSrcY/2) * iSource->Pitch];
                else
                    return &iSource->Field1[(iSrcY/2) * iSource->Pitch];
            }
            else
            {
                if(iSrcY&1)
                    return &iSource->Field1[(iSrcY/2) * iSource->Pitch];
                else
                    return &iSource->Field0[(iSrcY/2) * iSource->Pitch];
            }
    }
}


static PIUInt8* GetSourceAlphaPtr(const PIVideoBlock *iSource, int iSrcY)
{
    switch(iSource->Field)
    {
        default:
        case 0:
            if(iSource->Alpha0==0)return NULL;
            return &iSource->Alpha0[iSrcY * iSource->AlphaPitch];
        case 1:
            if(iSource->Alpha1==0)return NULL;
            return &iSource->Alpha1[(iSrcY/2) * iSource->AlphaPitch];
        case 2:
            if(iSource->Alpha0==0)return NULL;
            return &iSource->Alpha0[(iSrcY/2) * iSource->AlphaPitch];
        case 3:
            if(iSource->FieldMode==2)
            {
                if(iSrcY&1)
                {
                    if(iSource->Alpha1==0)return NULL;
                    return &iSource->Alpha0[(iSrcY/2) * iSource->AlphaPitch];
                }
                else
                {
                    if(iSource->Alpha0==0)return NULL;
                    return &iSource->Alpha1[(iSrcY/2) * iSource->AlphaPitch];
                }
            }
            else
            {
                if(iSrcY&1)
                {
                    if(iSource->Alpha1==0)return NULL;
                    return &iSource->Alpha1[(iSrcY/2) * iSource->AlphaPitch];
                }
                else
                {
                    if(iSource->Alpha0==0)return NULL;
                    return &iSource->Alpha0[(iSrcY/2) * iSource->AlphaPitch];
                }
            }
    }
}

_inline void swap_yuyv(PIUInt8 *dest, const PIUInt8 *src, int size)
{
    unsigned long *d=(unsigned long*)dest;
    const unsigned long *s=(const unsigned long*)src;

    size/=2;
    while(size--)
    {
        unsigned int a = *s++;
        *d++ =((a&0xff00ff00)>>8) | ((a&0x00ff00ff)<<8);
    }
}



int MyStretchVideoBlock(OutputDisplay *vd, PIBlock *oDest, const PIVideoBlock *iSource,
                        double iSource_X, double iSource_Y,
                        int iDest_X, int iDest_Y,
                        int iDest_Width, int iDest_Height,
                        double iRatio_X, double iRatio_Y)
{
    int x, y;
    int sh=iSource->Height * ((iSource->Field==1 || iSource->Field==2) ? 2 : 1);

    if(vd->mReq==0)return 0;

    // -------------------------- Clipping -------------------------------
    if(iDest_X < 0)
    {
        iSource_X   += iRatio_X * (-iDest_X);
        iDest_Width += iDest_X;
        iDest_X = 0;
    }
    if(iDest_Y < 0)
    {
        iSource_Y    += iRatio_Y * (-iDest_Y);
        iDest_Height += iDest_Y;
        iDest_Y = 0;
    }
    if(iDest_X+iDest_Width > oDest->Width)
    {
        iDest_Width = oDest->Width - iDest_X;
    }
    if(iDest_Y+iDest_Height > oDest->Height)
    {
        iDest_Height = oDest->Height - iDest_Y;
    }
    if(iDest_Width<=0 || iDest_Height<=0)return 0;

    //--------------------------------------------------------
    if(iSource->PixelType==PIXTYPE_BGRA32)
    {
        const unsigned long step_fix_x  = (unsigned long )(iRatio_X  * 65536.);
        const unsigned long fix_x_const = (unsigned long )(iSource_X * 65536.);

        for(y=0; y<iDest_Height; y++)
        {
            PIPixel* dest = &oDest->Data[iDest_X + (iDest_Y+y)*oDest->Width];
            unsigned long fix_x  = fix_x_const;

            int src_y = MIN( (int)iSource_Y, sh-1);
            const PIPixel *src = (PIPixel*)GetSourcePtr(iSource, src_y);

            if(step_fix_x>=0x9998 && step_fix_x<=0x10002)
            {
                memcpy(dest, &src[fix_x>>16], iDest_Width*4);
            }
            else
            {
                for(x=0; x<iDest_Width; x++)
                {
                    dest[x] = src[fix_x>>16];
                    fix_x  += step_fix_x;
                }
            }
            iSource_Y += iRatio_Y;
        }
    }
    else
    //--------------------------------------------------------
    if(iSource->PixelType==PIXTYPE_UYVY)
    {
        const unsigned long step_fix_x = (unsigned long )(iRatio_X  * 65536.);
        int sx = (int)floor(iSource_X) & ~1;
        int sw = (((int)floor(iSource_X + iDest_Width * iRatio_X) - sx + 1)+1) & ~1;

        PIPixel* temp=(PIPixel*)alloca((sw+8) * sizeof(PIPixel));
        const unsigned long fix_x_const   = (unsigned long )((iSource_X-sx) * 65536.);

        for(y=0; y<iDest_Height; y++)
        {
            PIPixel* dest = &oDest->Data[iDest_X + (iDest_Y+y)*oDest->Width];
            unsigned long fix_x      = fix_x_const;

            int src_y = MIN( (int)(iSource_Y), sh-1);
            const PIUInt8 *src = (PIUInt8*)GetSourcePtr(iSource, src_y);
            const PIUInt8 *alpha = GetSourceAlphaPtr(iSource, src_y);

            src += sx*2;
            if(alpha)alpha += sx;


            TVYUVToRGBA(vd->PiFilter, src, alpha, temp, sw);
            if(alpha)
                TVRGBAPremultiply(vd->PiFilter, temp, temp, sw);

            if(step_fix_x>=0x9998 && step_fix_x<=0x10002)
            {
                memcpy(dest, &temp[fix_x>>16], iDest_Width*4);
            }
            else
            {
                for(x=iDest_Width; x; x--)
                {
                    *dest++ = temp[fix_x>>16];
                    fix_x += step_fix_x;
                }
            }
            iSource_Y += iRatio_Y;
        }
    }
    //--------------------------------------------------------
    else
    if(iSource->PixelType==PIXTYPE_YUY2)
    {
        const unsigned long step_fix_x = (unsigned long )(iRatio_X  * 65536.);

        int sx=(int)floor(iSource_X) & ~1;
        int sw=(((int)floor(iSource_X + iDest_Width * iRatio_X) - sx + 1)+1) & ~1;

        PIUInt8* temp1=(PIUInt8*)alloca((sw+8) * 2);
        PIPixel* temp2=(PIPixel*)alloca((sw+8) * sizeof(PIPixel));
        const unsigned long fix_x_const   = (unsigned long )((iSource_X-sx) * 65536.);

        for(y=0; y<iDest_Height; y++)
        {
            PIPixel* dest = &oDest->Data[iDest_X + (iDest_Y+y)*oDest->Width];
            unsigned long fix_x      = fix_x_const;

            int src_y = MIN( (int)(iSource_Y), sh-1);
            const PIUInt8 *src = (PIUInt8*)GetSourcePtr(iSource, src_y);
            const PIUInt8 *alpha = GetSourceAlphaPtr(iSource, src_y);

            src += sx*2;
            if(alpha)alpha += sx;

            swap_yuyv(temp1, src, sw);

            TVYUVToRGBA(vd->PiFilter, temp1, alpha, temp2, sw);
            if(alpha)
                TVRGBAPremultiply(vd->PiFilter, temp2, temp2, sw);


            if(step_fix_x>=0x9998 && step_fix_x<=0x10002)
            {
                memcpy(dest, &temp2[fix_x>>16], iDest_Width*4);
            }
            else
            {
                for(x=iDest_Width; x; x--)
                {
                    *dest++ = temp2[fix_x>>16];
                    fix_x  += step_fix_x;
                }
            }
            iSource_Y += iRatio_Y;
        }
    }
    return 1;
}


static int StretchVideoBlock(struct PIDisplay *iDisplay, const PIVideoBlock *iSource,
                             double iSource_X, double iSource_Y,
                             int iDest_X, int iDest_Y,
                             int iDest_Width, int iDest_Height,
                             double iRatio_X, double iRatio_Y)
{
    OutputDisplay *vd=(OutputDisplay *)iDisplay;

    MyStretchVideoBlock(vd, vd->mImage, iSource, iSource_X, iSource_Y, iDest_X, iDest_Y, iDest_Width, iDest_Height, iRatio_X, iRatio_Y);
    if(vd->mDskIsOn == 0)
        TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, iDest_X, iDest_Y, iDest_Width, iDest_Height);
    return 1;
}


static int  WriteVideoBlock(struct PIDisplay *display, const PIVideoBlock *source, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
    return StretchVideoBlock(display, source, (double)source_x, (double)source_y, dest_x, dest_y, width, height, 1.0, 1.0);
}

//----------------------------------------------------------
// Function for clearing the display
//----------------------------------------------------------
static int  Clear(PIDisplay *Display, PIPixel32 color)
{
    OutputDisplay   *vd=(OutputDisplay *)Display;

    if(vd->mReq==0)return 0;

    //////////////////////////////////////////////////////////
    // Clear your screen here
    //////////////////////////////////////////////////////////
    {
        int size = vd->mImage->Width * vd->mImage->Height;
        PIPixel *dst = vd->mImage->Data;
        int x;
        for(x=0; x<size; x++)
        {
            dst[x].l = color;
        }
        if(vd->mDskIsOn == 0)
            TVRefreshButtonImage(vd->PiFilter, vd->mReq, 1, 0, 0, vd->mImage->Width, vd->mImage->Height);
    }
    return 1;
}
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
static  double  OutputVideoBlock(PIDisplay *iDisplay, const PIVideoBlock *iSource, int iDest_Width, int iDest_Height)
{
    OutputDisplay   *vd=(OutputDisplay *)iDisplay;
    int sw=iSource->Width;
    int sh=iSource->Height * ((iSource->Field==1 || iSource->Field==2) ? 2 : 1);

    ImageList *image = GetFreeImage(vd);
    while(image==NULL)
    {
        Sleep(5);
        image = GetFreeImage(vd);
    }

    MyStretchVideoBlock(vd, image->mImage, iSource, 0, 0, (vd->mImage->Width - iDest_Width)/2, (vd->mImage->Height - iDest_Height)/2,
                                iDest_Width, iDest_Height, sw/(double)iDest_Width, sh/(double)iDest_Height);

    PutReadyImage(vd, image);

    return 1.0/29.997;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
static int  Cmd(PIDisplay *iDisplay, const char *iCmd, char *iResult)
{
    OutputDisplay   *vd=(OutputDisplay *)iDisplay;

    if(strnicmp(iCmd,"Disable", 7)==0)
    {
        while(*iCmd && !isspace(*iCmd))iCmd++;
        while(isspace(*iCmd))iCmd++;
        if(strnicmp(iCmd,"KeyOn", 5)==0)
        {
            //Gfx Off, Key On
            KeyOn(vd, &iCmd[6]);
        }
        else
        {
            //Gfx Off, Key Off
            KeyOff(vd);
        }
        //oResult[0]=0;
        return 1;
    }
    else
    if(strnicmp(iCmd,"Enable", 6)==0)
    {
        while(*iCmd && !isspace(*iCmd))iCmd++;
        while(isspace(*iCmd))iCmd++;
        if(strnicmp(iCmd,"KeyOn", 5)==0)
        {
            //Gfx On, Key On
            KeyOn(vd, &iCmd[6]);

        }
        else
        {
            //Gfx On, Key Off
            KeyOff(vd);
        }
        return 1;
    }
    return 0;
}

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
static int BestVideoMode(PIDisplay *iDisplay, PIVideoInfo *oInfo)
{
    memset(oInfo, 0, sizeof(*oInfo));
    strcpy(oInfo->Name, iDisplay->DeviceName);
    oInfo->Width   = iDisplay->Width;
    oInfo->Height  = iDisplay->Height;
    oInfo->BytesPerPixel = 4;
    oInfo->PixelType = PIXTYPE_BGRA32;
    oInfo->AspectRatio = iDisplay->AspectRatio;
    oInfo->FrameRate = 29.997;
    oInfo->FieldMode = 0;
    return 1;
}


//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------

PIDisplay *OpenVideoDevice(PIFilter *PiFilter)
{
    OutputDisplay   *vd;

    ///////////////////////////////////////////////
    // open your Video device here 
    ///////////////////////////////////////////////


    vd=(OutputDisplay *)calloc(1, sizeof(OutputDisplay));
    if(vd==NULL)return NULL;

    vd->Display.OpenDisplay=OpenDisplay;                //Function for opening a display.
    vd->Display.CloseDisplay=CloseDisplay;              //Function for closing a display.
    vd->Display.WritePIBlock=WritePIBlock;              //Function for writing a PIBlock.
    vd->Display.WritePixel=WritePixel;                  //Function for writing a single pixel.
    vd->Display.Clear=Clear;                            //Function for clearing the display.
    vd->Display.RectFill=RectFill;                      //Function for drawing a filled rectange.
    vd->Display.WriteVideoBlock = WriteVideoBlock;      //Function for writing a PIVideoBlock
    vd->Display.StretchVideoBlock = StretchVideoBlock;  //Function for stretching a PIVideoBlock

    vd->Display.OpenOutput  = OpenOutput;
    vd->Display.CloseOutput = CloseOutput;
    vd->Display.OutputVideoBlock = OutputVideoBlock;

    vd->Display.Cmd = Cmd;
    vd->Display.BestVideoMode = BestVideoMode;

    vd->Display.DeviceName="My Video Device";       //Pointer on a string containing the Name of the Display Device
    vd->Display.Width=720;                          //This field contains the Width of the Display Device in pixels.
    vd->Display.Height=486;                         //This field contains the Height of the Display Device in pixels.
    vd->Display.MaxWidth=720;                       //This field contains the maximum Width of the display in pixels.
    vd->Display.MaxHeight=576;                      //This field contains the maximum Height of the display in pixels.
    vd->Display.MinWidth=720;                       //This field contains the minimum Width  of the display in pixels.
    vd->Display.MinHeight=480;                      //This field contains the minimum Height of the display in pixels.
    vd->Display.AspectRatio=1.;                     //This field contains the aspectratio of the pixels of the Display Device.
    vd->Display.Version=PIDISPLAY_VERSION;          //This field contains PIDisplay structure version number.


    vd->PiFilter = PiFilter;

    TVAddDisplayDevice(PiFilter, &vd->Display, 0);

    sgDisplay = vd;

    return &vd->Display;
}
//--------------------------------------------------------------------------
void CloseVideoDevice(PIFilter *PiFilter, PIDisplay *Display)
{
    OutputDisplay   *vd=(OutputDisplay *)Display;

    ///////////////////////////////////////////
    // close your device here
    ///////////////////////////////////////////

    free(vd);
}
