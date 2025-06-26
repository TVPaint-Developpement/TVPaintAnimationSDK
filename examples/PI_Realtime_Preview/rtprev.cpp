/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include  "TVPaintSDK.h"

#ifdef WIN32
#include <malloc.h>
#endif

#ifndef MAX
#define MAX(a, b)   ((a)>(b) ? (a) : (b))
#define MIN(a, b)   ((a)<(b) ? (a) : (b))
#endif


/****************************************************************/

// Variable for localisation
static void *LocalFile;

/****************************************************************/

// Structure to save the parameters of the filter
typedef struct
{
    char Arguments[4096];
} UserParam;

/********************w********************************************/

PIFilter    *GlobalPiFilter;

/****************************************************************/

//Button ID
#define    ID_PLAY 1
#define    ID_POS 2
#define    ID_NUM 3
#define    ID_DISPLAY 4

/****************************************************************/

//Variable initialization
static int    ShowPosition=0;
static int    ShowNumber=0;
static char    DeviceName[4096];

/****************************************************************/

// Identification of the window.
// Indicates if the window is open
// (=0 closed  !=0 already open)

static INTPTR greq=0;

/****************************************************************/

//Localisation
#define TXT_NAME            GetLocalString(PiFilter, 100,   "SDK Realtime preview")
#define TXT_WINDOW          GetLocalString(PiFilter, 10000, "SDK Realtime preview")

#define TXT_POS             GetLocalString(PiFilter, 10010,  "Show position")
#define TXT_NUM             GetLocalString(PiFilter, 10011,  "Show number")
#define TXT_DISPLAY         GetLocalString(PiFilter, 10012,  "Display with")

#define TXT_MENU_DEFAULT    GetLocalString(PiFilter, 10015,  "VGA monitor")

#define TXT_ERROR01         GetLocalString(PiFilter, 10020,  "Can't open requester !")

#define TXT_MSG01           GetLocalString(PiFilter, 10030,  "RTPreview in progress")

#define TXT_HELP_POS        GetLocalString(PiFilter, 10110,  "Display current position in the animation")
#define TXT_HELP_NUM        GetLocalString(PiFilter, 10111,  "Display current frame number")
#define TXT_HELP_DISPLAY    GetLocalString(PiFilter, 10112,  "Choose your output display device")

/****************************************************************/

//Localisation function
static char *GetLocalString(PIFilter * PiFilter, int num, char *def)
{
char *str;

    if(LocalFile==NULL)return def;
    str=TVGetLocalString(PiFilter, LocalFile, num);
    if(str==NULL || strlen(str)==0)return def;

    return str;
}

/****************************************************************/

int FAR PASCAL PI_MakePrevAnim(PIFilter *PiFilter, int FirstImage, int LastImage, double FrameRate, void *future);
static int  PrepareAnim(PIFilter *PiFilter);

/****************************************************************/
/****************************************************************/

//Function called FIRST for the initialization of the filter
int FAR PASCAL PI_Open(PIFilter * PiFilter)
{
    char rien[1024];

    //Open localisation file
    LocalFile=TVOpenLocalFile(PiFilter, "rtprev.loc", 0);

    //Init PI field
    strcpy(PiFilter->PIName,TXT_NAME);
    PiFilter->PIVersion=1;
    PiFilter->PIRevision=0;

    //Init user param
    if(!PiFilter->UserParametersPtr)
    {
        UserParam *Param;
        Param=(UserParam*)malloc(sizeof(UserParam));
        if(!Param)
        {
            return 0;
        }

        memset(Param,0,sizeof(UserParam));

        PiFilter->UserParametersPtr=(void *)Param;
        PiFilter->UserParametersSize=sizeof(UserParam);
    }

    //Load previous settings
    TVReadUserString(PiFilter, PiFilter->PIName, "Position", rien, "1", 255);
    ShowPosition=atoi(rien) ? 1 : 0;
    TVReadUserString(PiFilter, PiFilter->PIName, "Number", rien, "1", 255);
    ShowNumber=atoi(rien) ? 1 : 0;
    TVReadUserString(PiFilter, PiFilter->PIName, "Device", DeviceName, "", 4095);

    return 1;
}

/****************************************************************/
/****************************************************************/

static void TraceDeviceName(PIFilter *PiFilter)
{
    if(greq==0)return;

    if(strlen(DeviceName)==0)
        TVChangeButtonReq(PiFilter, greq, ID_DISPLAY, PIRBF_BUTTON_POPUP,TXT_MENU_DEFAULT);
    else
        TVChangeButtonReq(PiFilter, greq, ID_DISPLAY, PIRBF_BUTTON_POPUP,DeviceName);
}

static void GereDevicePop(PIFilter *PiFilter)
{
    int num;
    int a;
    PIPopup *pop;
    int start=0;
    int f;

    num=0;
    while(1)
    {
        const char *dev;
        dev=TVEnumDisplayDevice(PiFilter, num, 0);

        if(dev==NULL)break;
        num++;
    }

    pop=malloc((num+2)*sizeof(PIPopup));
    if(pop==NULL)return;
    memset(pop, 0, (num+2)*sizeof(PIPopup));

    pop[0].ID=0;
    pop[0].Name=TXT_MENU_DEFAULT;
    for(a=0;a<num;a++)
    {
        pop[a+1].ID=a+1;
        pop[a+1].Name=TVEnumDisplayDevice(PiFilter, a, 0);

        if(strcmp(DeviceName, pop[a+1].Name)==0)
        {
            pop[a+1].Flags=PIPOPMODE_CHECK;
            start=a+1;
        }
}

    f=TVPopup(PiFilter, pop, num+1, start);

    if(f==0)
        strcpy(DeviceName, "");
    else
    if(f>0)
        strcpy(DeviceName, pop[f].Name);

    TraceDeviceName(PiFilter);
}
/****************************************************************/
/****************************************************************/

// Function called by TVPaint to control the interaction filter-TVPaint
int FAR PASCAL PI_Msg(PIFilter *PiFilter,INTPTR fonc ,INTPTR req, INTPTR *tag)
{
    // Detection of an event
    switch(fonc)
    {
        case PICBREQ_DRAG:  //Window moved
            break;

        case PICBREQ_BUTTON_UP: // Button released
            
            // It was a selection of a normal button
            switch(tag[0])
            // Identity of the selected button
            
            {
                case ID_PLAY:
                    if(PrepareAnim(PiFilter))
                        PI_PlayPrevAnim(PiFilter, NULL);
                    break;

                case ID_NUM:
                    ShowNumber=1-ShowNumber;
                    TVChangeButtonReq(PiFilter, greq, ID_NUM, PIRBF_BUTTON_CHECK|(ShowNumber ? PIRBF_BUTTON_SELECT : 0), NULL);
                    break;

                case ID_POS:
                    ShowPosition=1-ShowPosition;
                    TVChangeButtonReq(PiFilter, greq, ID_POS, PIRBF_BUTTON_CHECK|(ShowPosition ? PIRBF_BUTTON_SELECT : 0), NULL);
                    break;
                case ID_DISPLAY:
                    GereDevicePop(PiFilter);
                    break;

            }
            break;

        case kPIEvents_WindowOpen:
            break;

        case PICBREQ_RESIZE:    // Window resized
            break;

        case PICBREQ_CLOSE: // Window closed
            
            if(req==greq)
            {
                char rien[256];
                greq=0;

                // Settings saved (loaded in the PI_OPEN)
                sprintf(rien, "%d", ((int*)tag)[0]);
                TVWriteUserString(PiFilter, PiFilter->PIName, "WIN_X", rien);
                sprintf(rien, "%d", ((int*)tag)[1]);
                TVWriteUserString(PiFilter, PiFilter->PIName, "WIN_Y", rien);

                TVWriteUserString(PiFilter, PiFilter->PIName, "Number", ShowNumber? "1" : "0");
                TVWriteUserString(PiFilter, PiFilter->PIName, "Position", ShowPosition? "1" : "0");
                TVWriteUserString(PiFilter, PiFilter->PIName, "Device", DeviceName);
            }
            break;

    }
    return 1;
}

/****************************************************************/

// Function called by the user of TVPaint for informations
// (It can do nothing)
void FAR PASCAL PI_About(PIFilter *PiFilter)
{
    char text[256];

    sprintf( text, 
           "%s V%d,%d \nCopyright TVPaint Developpement\n(c) 2024", 
           PiFilter->PIName, PiFilter->PIVersion, PiFilter->PIRevision );

    // Writes an alert message on the working screen with the
    // name, the version and revision of the filter.
    TVWarning(PiFilter, text);
}

/****************************************************************/

// Function which initializes the settings of the parameters.
// The parameters can be transmitted from an external program
// in the string LPSTR.
int FAR PASCAL PI_Parameters(PIFilter *PiFilter, const char *arg)
{
    // If this string exists, some parameters are send from an external
    // program (from George or a DDE command). In this case, DO NOT
    // open the user interface but you must run TVExecute() (after
    // the interpretation of the string).
    if(arg)
    {
        if(PrepareAnim(PiFilter))
            PI_PlayPrevAnim(PiFilter, NULL);
    }
    else
    {
        // If the window is not open, we do it.
        if(greq==0)
        {
            char rien[256];
            int x=100, y=100;
            int w=210, h=67;
            unsigned long f=0;
            
            
            //Previous position loaded
            TVReadUserString(PiFilter, PiFilter->PIName, "WIN_X", rien, "none", 255);
            if(!strcmp(rien, "none"))
                f|=PIRF_CENTER_REQ;
            else
                x=atoi(rien);

            TVReadUserString(PiFilter, PiFilter->PIName, "WIN_Y", rien, "none", 255);
            if(!strcmp(rien, "none"))
                f|=PIRF_CENTER_REQ;
            else
                y=atoi(rien);

            //Window open
            greq=TVOpenReq(PiFilter, w, h, x, y, f|PIRF_STANDARD_REQ,TXT_WINDOW);
            if(!greq)
            {
                TVWarning(PiFilter,TXT_ERROR01);
                return 0;
            }

            //Buttons creation
            TVAddButtonReq(PiFilter, greq, w-10-110, 5, 110, 0, ID_DISPLAY, PIRBF_BUTTON_POPUP, NULL);
            TVTextReq(PiFilter,greq, w-10-180, 10, TXT_DISPLAY,PIGMODE_TRANS, TVMakePIPixel32(0,0,0,255),0);
            TVSetButtonInfoText(PiFilter,greq,ID_DISPLAY,TXT_HELP_DISPLAY);

            TVAddButtonReq(PiFilter, greq, w-10-110, 25, 0, 0, ID_NUM,PIRBF_BUTTON_CHECK,NULL);
            TVChangeButtonName(PiFilter, greq, ID_NUM, TXT_NUM, BUTTON_TEXT_OLEFT);
            TVSetButtonInfoText(PiFilter,greq,ID_NUM,TXT_HELP_NUM);

            TVAddButtonReq(PiFilter, greq, w-10-110, 45, 0, 0, ID_POS,PIRBF_BUTTON_CHECK,NULL);
            TVChangeButtonName(PiFilter, greq, ID_POS, TXT_POS, BUTTON_TEXT_OLEFT);
            TVSetButtonInfoText(PiFilter,greq,ID_POS,TXT_HELP_POS);

            TVChangeButtonReq(PiFilter, greq, ID_NUM, PIRBF_BUTTON_CHECK|(ShowNumber ? PIRBF_BUTTON_SELECT : 0), NULL);
            TVChangeButtonReq(PiFilter, greq, ID_POS, PIRBF_BUTTON_CHECK|(ShowPosition ? PIRBF_BUTTON_SELECT : 0), NULL);

            TraceDeviceName(PiFilter);
        }
        else
        {
            TVReqToFront(PiFilter, greq);
        }
    }
    return 1;
}

/****************************************************************/
/****************************************************************/

// We completely closed the filter and exit.
void FAR PASCAL PI_Close(PIFilter *PiFilter)
{
    if(greq)
    {
        TVCloseReq(PiFilter, greq);
    }
    if(PiFilter->UserParametersPtr)
    {
        free(PiFilter->UserParametersPtr);
        PiFilter->UserParametersPtr=0;
    }
}
/****************************************************************/
/****************************************************************/

static int PrepareAnim(PIFilter *PiFilter)
{
    int     fr,f,l;
    char    rien[256];

    PI_RemovePrevAnim(PiFilter,NULL);
    TVSendCmd(PiFilter,"TV_FRAMERATE",rien);
    fr=atoi(rien);
    TVSendCmd(PiFilter,"TV_LAYERSELECTINFO",rien);
    sscanf(rien,"%d %d",&f,&l);
    if (l<=1)
    {
        TVSendCmd(PiFilter,"TV_FIRSTIMAGE",rien);
        f=atoi(rien);
        TVSendCmd(PiFilter,"TV_LASTIMAGE",rien);
        l=atoi(rien);
    }
    else
    {
        l=f+l-1;
    }
    return PI_MakePrevAnim(PiFilter,f,l,fr,NULL);
}
/****************************************************************/

static int first, last;
static double framerate;
int FAR PASCAL PI_MakePrevAnim(PIFilter *PiFilter, int FirstImage, int LastImage, double FrameRate, void *future)
{
    first=MIN(FirstImage, LastImage);
    last=MAX(FirstImage, LastImage);
    if(FrameRate)
        framerate=FrameRate;
    else
        framerate=100.;
    return 1;
}

#ifdef _WIN64
static void delay(int n) /* in milliseconds */
{
    if(n<=0)return;
    Sleep(n);
}

static double currenttime(void)
{
    return clock()/(double)CLOCKS_PER_SEC;
}


#else
#include <sys/time.h>
#include <unistd.h>

static double currenttime(void)
{
    struct timeval currenttime;
    gettimeofday(&currenttime, NULL);
return currenttime.tv_sec+currenttime.tv_usec/1000000.;
}

static void delay(int n) /* in milliseconds */
{
    usleep( n*1000 );
}

#endif

static int GetStop(PIFilter *PiFilter)
{
    char result[256];

    TVSendCmd(PiFilter,"tv_nop Buttons Stat", result);
    if(strcmp(result, "0 0")==0)
        return 0;
    else
        return 1;
}


static int PlayPrevAnimInDevice(PIFilter *PiFilter, char *displayname)
{
    PIDisplay   *display;
    PIBlock *block;
    int a;
    int stop=0;
    double t0=0, t1=0;
    int LastImage=first-1;

    if(displayname==NULL)
    {
        return 0;
    }

    display=TVLockDisplayDevice(PiFilter, displayname, 0);
    if(display==NULL)
    {
        return 0;
    }

    if(display->OpenDisplay(display, PiFilter->ImageWidth, PiFilter->ImageHeight)==0)
    {
        TVUnlockDisplayDevice(PiFilter, display);
        return 0;
    }

    display->Clear(display, 0);

    block=TVAllocPIBlock(PiFilter, PiFilter->ImageWidth, PiFilter->ImageHeight, 0);
    if(block==NULL)
    {
        display->CloseDisplay(display);
        TVUnlockDisplayDevice(PiFilter, display);
        return 0;
    }

    if(first==last)
    {
        TVMakeDisplayImage(PiFilter, block->Data,
            block->Width*block->Height*sizeof(PIPixel), first,  1);
        display->WritePIBlock(display, block, 0, 0,
            (display->Width-block->Width)/2, (display->Height-block->Height)/2, block->Width, block->Height, 0);
    }
    else
    {
        t0=currenttime();
        a=0;

        TVWorkOn(PiFilter, TXT_MSG01);
        while(stop==0 && GetStop(PiFilter)==0)
        {
            int pos;
                        
            while(a==LastImage && stop==0 && GetStop(PiFilter)==0)
            {
                t1=currenttime();

                a=(int)((t1-t0)*framerate)+first;
                if(a>last)
                {
                    a=first;
                    t0=t1;
                    break;
                }
                if(a==LastImage)
                {
                    double nouvelle=t0+(LastImage+1)*1./framerate;
                    stop=TVWorkTrace(PiFilter, 0, 0);
                    t1=currenttime();
                    delay((int)((nouvelle-t1)*1000.));
                }
            }

            stop=TVWorkTrace(PiFilter, a-first, last-first);
            if(stop)break;

            LastImage=a;
            TVMakeDisplayImage(PiFilter, block->Data, 
                block->Width*block->Height*sizeof(PIPixel), a,  1);
            
            if(display->WritePIBlock(display, block, 0, 0,
                    (display->Width-block->Width)/2, (display->Height-block->Height)/2, block->Width, block->Height, 0)==0)
            {
                stop=1;
                break;
            }
            if(ShowPosition)
            {
                pos=((a-first+1)*block->Width)/(last-first+1);
                display->RectFill(display, 
                    (display->Width-block->Width)/2, (display->Height-block->Height)/2+block->Height,
                     pos, 9, TVMakePIPixel32(255,255,255,255), 0);
                display->RectFill(display, 
                    (display->Width-block->Width)/2+pos+1, (display->Height-block->Height)/2+block->Height,
                     block->Width-pos-1, 9, TVMakePIPixel32(0,0,0,255), 0);

            }
        }
        TVWorkOff(PiFilter);
    }
    TVFreePIBlock(PiFilter, block);
    display->CloseDisplay(display);
    TVUnlockDisplayDevice(PiFilter, display);
    return 1;
}

int FAR PASCAL PI_PlayPrevAnim(PIFilter *PiFilter, void *future)
{
    PIBlock *block;
    int a;
    INTPTR req;
    int stop=0;
    double t0=0, t1=0;
    char rien[256];
    int LastImage=first-1;

    if(PlayPrevAnimInDevice(PiFilter, DeviceName))return 1;


    block=TVAllocPIBlock(PiFilter, PiFilter->ImageWidth, PiFilter->ImageHeight, 0);
    if(block==NULL)return 0;

    if(first==last)
    {
        req=TVOpenReq(PiFilter, PiFilter->ImageWidth, PiFilter->ImageHeight, 0, 0, PIRF_CENTER_REQ|PIRF_NO_SYSBUTTON_REQ, NULL);
        TVMakeDisplayImage(PiFilter, block->Data,
            block->Width*block->Height*sizeof(PIPixel), first,  1);
        TVWriteReqData(PiFilter, block->Data, 0, 0, block->Width, block->Height, 0, req);

        while(1)
        {
            stop=TVWorkTrace(PiFilter, 0, 0);
            if(stop)break;
            if(GetStop(PiFilter))break;
            delay(20);
        }
        TVCloseReq(PiFilter, req);

    }
    else
    {
        if(ShowPosition)
            req=TVOpenReq(PiFilter, PiFilter->ImageWidth, PiFilter->ImageHeight+10, 0, 0, PIRF_CENTER_REQ|PIRF_NO_SYSBUTTON_REQ, NULL);
        else
            req=TVOpenReq(PiFilter, PiFilter->ImageWidth, PiFilter->ImageHeight, 0, 0, PIRF_CENTER_REQ|PIRF_NO_SYSBUTTON_REQ, NULL);
        if(req)
        {
            t0=currenttime();
            a=0;
            while(stop==0 && GetStop(PiFilter)==0)
            {
                int pos;
                            
                stop=TVWorkTrace(PiFilter, 0, 0);
                if(stop)break;
                while(a==LastImage && stop==0 && GetStop(PiFilter)==0)
                {

                    t1=currenttime();

                    a=(int)((t1-t0)*framerate)+first;
                    if(a>last)
                    {
                        a=first;
                        t0=t1;
                        break;
                    }
                    if(a==LastImage)
                    {
                        double nouvelle=t0+(LastImage+1)*1./framerate;
                        stop=TVWorkTrace(PiFilter, 0, 0);
                        t1=currenttime();
                        delay((int)((nouvelle-t1)*1000.));
                    }

                }
                LastImage=a;
                TVMakeDisplayImage(PiFilter, block->Data, 
                    block->Width*block->Height*sizeof(PIPixel), a,  1);
                
                TVWriteReqData(PiFilter, block->Data, 0, 0, block->Width, block->Height, 0, req);
        
                if(ShowPosition)
                {
                    pos=((a-first+1)*block->Width)/(last-first+1);
                    TVRectangleReq(PiFilter, req, 0, block->Height, pos, block->Height+9, PIGMODE_COLOR, TVMakePIPixel32(255,255,255,255));
                    TVRectangleReq(PiFilter, req, pos+1, block->Height, block->Width-1, block->Height+9, PIGMODE_COLOR, TVMakePIPixel32(0,0,0,255));
                }
        
                if(ShowNumber)
                {
                    PIPixel32  red = TVMakePIPixel32( 0, 0, 0, 255 );
                    sprintf(rien, "%d", a);
                    TVTextReq( PiFilter, req, 10, block->Height-16, rien, PIGMODE_TRANS, red, 0 );
                    TVTextReq( PiFilter, req, 12, block->Height-16, rien, PIGMODE_TRANS, red, 0 );
                    TVTextReq( PiFilter, req, 12, block->Height-14, rien, PIGMODE_TRANS, red, 0 );
                    TVTextReq( PiFilter, req, 10, block->Height-14, rien, PIGMODE_TRANS, red, 0 );
                    TVTextReq( PiFilter, req, 11, block->Height-15, rien, PIGMODE_TRANS, TVMakePIPixel32( 255, 255, 255, 255 ), 0 );
                }
        
            }
            TVCloseReq(PiFilter, req);
        }
    }
    TVFreePIBlock(PiFilter, block);
    return 1;
}

int FAR PASCAL PI_RemovePrevAnim(PIFilter *PiFilter, void *future)
{
    return 1;
}
/****************************************************************/
