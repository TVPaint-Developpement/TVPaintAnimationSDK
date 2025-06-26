//////////////////////////////////////////////////////////////////////////////
//
//      Video Device Common Part - Example For Aura 2
//
//      VideoDevice_common.c
//
//////////////////////////////////////////////////////////////////////////////

#include "myvideodevice.h"

static void *LocalFile;
static INTPTR greq;

//----------------------------------------------------------
// Internal Function - No Comment
//----------------------------------------------------------
static char *GetLocalString(PIFilter * PiFilter, int num, char *def)
{
char *str;

    if(LocalFile==NULL)return def;
    str=TVGetLocalString(PiFilter, LocalFile, num);
    if(str==NULL || strlen(str)==0)return def;

    return str;
}

//----------------------------------------------------------
// Initializes the plugin itself. 
// It is the function called FIRST by the host to let the filter to initialize its parameters. 
// Plugin must set PiFilter->PIName, PiFilter->PIVersion, PiFilter->PIRevision. 
//----------------------------------------------------------
int FAR PASCAL PI_Open(PIFilter * PiFilter)
{
char temp[512];
int open=0;
UserParam *Param;

    ////////////////////////////////////////
    // Open the Localisation file (ASCII File)
    ////////////////////////////////////////
    LocalFile=TVOpenLocalFile(PiFilter, "videodevicename.loc", 0);

    strcpy(PiFilter->PIName,TXT_NAME);
    PiFilter->PIVersion=1;
    PiFilter->PIRevision=0;

    Param=malloc(sizeof(UserParam));
    if(!Param)
    {
        return 0;
    }

    memset(Param,0,sizeof(UserParam));

    PiFilter->UserParametersPtr=(void *)Param;
    PiFilter->UserParametersSize=sizeof(UserParam);

    ////////////////////////////////////////
    // Open the Video Device
    ////////////////////////////////////////
    if(Param->mDisplay==NULL)
    {
        Param->mDisplay = OpenVideoDevice(PiFilter);
        Param->mVideoIn = InitNoiseVideoDevice(PiFilter);
    }

    ////////////////////////////////////////
    // ReOpen the filter if it was open during last TVPaint shut Down
    ////////////////////////////////////////
    TVReadUserString(PiFilter,PiFilter->PIName,"Open",temp,"0",255);
    open=atoi(temp);
    if(open)
        PI_Parameters(PiFilter,NULL);


    return 1;
}

//----------------------------------------------------------
// Displays miscellaneous informations (Plugin author's, version, revision, ...). 
// In the Plugin window's or in a new window that the filter opens, 
// or in a warning window (with TVWarning() function, etc). 
// It can be called (or not) when selecting a special About button. 
//----------------------------------------------------------
void FAR PASCAL PI_About(PIFilter *PiFilter)
{
    ////////////////////////////////////////
    // About Information Here !!!
    // example :
    //
    //  char temp[512];
    //      sprintf(temp,"Version number %d",PiFilter->PIVersion);
    //      TVWarning(PiFilter,temp);
    ////////////////////////////////////////
}

//----------------------------------------------------------
// Initializes the settings of the parameters. 
// It handles the string args which can be sent by an external program OR 
// it opens the user interface TVOpenReq() or TVOpenFilterReqEx(). 
//----------------------------------------------------------
int FAR PASCAL PI_Parameters(PIFilter *PiFilter, char *arg)
{
    UserParam *param=(UserParam *)PiFilter->UserParametersPtr;

    if(arg)
    {
        ////////////////////////////////////////
        // External Call (from George for ex.)
        // Requester and GUI is not opened
        ////////////////////////////////////////
        return 1;
    }
    else
    {
        char temp[256];
        INTPTR   req;
        long     f=0;
        if(greq==0)
        {
            ////////////////////////////////////////
            // If it's the first time, the requester is center
            // Else, it used last TVPaint session coordinates (save in PI_Msg with PICBREQ_CLOSE)
            ////////////////////////////////////////
            TVReadUserString(PiFilter, PiFilter->PIName, "Sitx", temp, "", 255);
            if(strlen(temp)==0)
            {
                f=PIRF_CENTER_REQ;
            } 
            param->sitx=atoi(temp);
            TVReadUserString(PiFilter, PiFilter->PIName, "Sity", temp, "", 255);
            param->sity=atoi(temp);

            ////////////////////////////////////////
            // Create and Open Requester
            ////////////////////////////////////////
            req=TVOpenReq(PiFilter, WINDOW_W, WINDOW_H, param->sitx, param->sity, PIRF_STANDARD_REQ, TXT_NAME);
            if(req==0)
            {
                TVWarning(PiFilter,"Can't Open Requester");
                return 0;
            }
            greq=req;

            ////////////////////////////////////////
            // Create Button
            ////////////////////////////////////////
            
        }
        else
        {
            ////////////////////////////////////////
            // Requester Already Open, Put it on Top of requesters
            ////////////////////////////////////////
            TVReqToFront(PiFilter, greq);
        }
    }
    return 1;
}

//----------------------------------------------------------
// Handles messages: receives all PICMETA messages, and PICBREQ messages for 
// windows without a specific message processing function.
// This function (and window specific message processing functions) is the 'heart' 
// of the plugin.
//----------------------------------------------------------
int FAR PASCAL PI_Msg(PIFilter *PiFilter,INTPTR fonc ,INTPTR req,INTPTR *tag)
{
    UserParam *param=(UserParam *)PiFilter->UserParametersPtr;

    switch(fonc)
    {
        char temp[512];

        ////////////////////////////////////////
        // Filter requester is opened. 
        ////////////////////////////////////////
        case kPIEvents_WindowOpen:
        {
        }
        break;
        
        ////////////////////////////////////////
        // Filter requester is closed
        ////////////////////////////////////////
        case PICBREQ_CLOSE:
        {
            ////////////////////////////////////////
            // Save requester Coordinates
            ////////////////////////////////////////
            param->sitx=(int)(tag[0]);
            param->sity=(int)(tag[1]);
            
            param->open=(int)(tag[4]);

            greq=0;
            sprintf(temp,"%d",param->sitx);
            TVWriteUserString(PiFilter, PiFilter->PIName, "Sitx", temp);
            
            sprintf(temp,"%d",param->sity);
            TVWriteUserString(PiFilter, PiFilter->PIName, "Sity", temp);
                
            sprintf(temp,"%d",param->open);
            TVWriteUserString(PiFilter, PiFilter->PIName, "Open", temp);
        }
        break;
    }
    return 1;
}

//----------------------------------------------------------
// Close the Filter completly. Called when tvpaint shut down
//----------------------------------------------------------
void FAR PASCAL PI_Close(PIFilter *PiFilter)
{
    ////////////////////////////////////////
    // Close all Structures, Requesters, Etc ...
    ////////////////////////////////////////
    if(greq!=0)                         //Close Requester
    {
        TVCloseReq(PiFilter, greq);
    }
    if(PiFilter->UserParametersPtr)     //Free User Parameters
    {
        free(PiFilter->UserParametersPtr);
        PiFilter->UserParametersPtr=0;
    }
}
