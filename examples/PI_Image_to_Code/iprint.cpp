
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "TVPaintSDK.h"

#if defined(WIN32) || defined(WIN64)
#include <malloc.h>
#endif

/****************************************************************/

typedef struct/* Structure to save the parameters of the filter*/
{
    int  none;
} UserParam;

/****************************************************************/
extern PIBlock *GetImage(PIPlugin *PiFilter);

#define ID_ICON         1
#define ID_RLE          3
#define ID_ADD_C_CODE   6
#define ID_START        7

static INTPTR greq=0;
static PIFileRequester FileReq;

static int  compress=0;
static int  addcode=0;

int FAR PASCAL PI_Open(PIPlugin * PiFilter)
{
char temp[256];

    strcpy(PiFilter->PIName, "SDK Image Print");
    PiFilter->PIVersion=1;
    PiFilter->PIRevision=0;

    if(PiFilter->UserParametersPtr==NULL)
    {
        UserParam *Param;
        Param=(UserParam*)malloc(sizeof(UserParam));
        if(NULL==Param)
        {
            return 0;
        }

        memset(Param, 0, sizeof(UserParam));

        PiFilter->UserParametersPtr=(void *)Param;
        PiFilter->UserParametersSize=sizeof(UserParam);
    }

    memset(&FileReq, 0, sizeof(PIFileRequester));
    FileReq.Icon=0;
    FileReq.Title="Save source";
    strcpy(FileReq.Pattern, "*.c");

    TVReadUserString(PiFilter, PiFilter->PIName, "File", FileReq.Path, "", 4096);

    if(!TVAllocFileReq(PiFilter, &FileReq))
    {
        TVWarning(PiFilter, "PIAllocFileReq Fail");
        free(PiFilter->UserParametersPtr);
        PiFilter->UserParametersPtr=NULL;
        PiFilter->UserParametersSize=0;
        return 0;
    }
    TVReadUserString(PiFilter, PiFilter->PIName, "PATH", FileReq.Path, FileReq.Path, 4095);

    TVReadUserString(PiFilter,PiFilter->PIName, "OPEN", temp, "0", 255);
    if(atoi(temp)==1)
        PI_Parameters(PiFilter,NULL);

    return 1;
}

/****************************************************************/
static void SaveCodeBrush(PIPlugin *PiFilter, char *filename,  int compress,  int addcode)
{
PIBlock *image;
FILE    *file;
int x, t;
PIPixel *p;

    if(PiFilter->BrushWidth<=0  ||  PiFilter->BrushHeight<=0 )return;

    image=TVAllocPIBlock(PiFilter, PiFilter->BrushWidth, PiFilter->BrushHeight, 0);
    if(image==NULL)return;


    TVReadLayerData(PiFilter, image->Data, 0, 0, image->Width, image->Height, 0, kPIReadLayer_Brush);

    file=fopen(filename, "wb");
    if(file!=NULL)
    {
        fprintf(file, "#include \"TVPaintSDK.h\"\n\n");

        fprintf(file, "#define IMAGEWIDTH  %d\n", image->Width);
        fprintf(file, "#define IMAGEHEIGHT %d\n", image->Height);

        if(compress==0)
        {
            p=image->Data;
            t=image->Width*image->Height;
            fprintf(file, "static unsigned char imagedata[IMAGEWIDTH*IMAGEHEIGHT*4]={\n");
            fprintf(file, "   ");
            for(x=0;x<t-1;x++)
            {
                fprintf(file, " %d, %d, %d, %d,",p->c[image->r], p->c[image->g], p->c[image->b], p->c[image->a]);
                if((x%10)==0) fprintf(file, "\n   ");
                p++;
            }
            fprintf(file, " %d, %d, %d, %d\n};\n",p->c[image->r], p->c[image->g], p->c[image->b], p->c[image->a]);

            if(addcode)
            {
                fprintf(file, "PIBlock *GetImage(PIPlugin *PiFilter)\n");
                fprintf(file, "{\n");
                fprintf(file, "PIBlock *image;\n");
                fprintf(file, "int x;\n");
                fprintf(file, "    image=TVAllocPIBlock(PiFilter, IMAGEWIDTH, IMAGEHEIGHT, 0);\n");
                fprintf(file, "    if(image==NULL)return NULL;\n");
                fprintf(file, "    for(x=0;x<IMAGEWIDTH*IMAGEHEIGHT;x++)\n");
                fprintf(file, "    {\n");
                fprintf(file, "        image->Data[x].c[image->r]=imagedata[x*4+0];\n");
                fprintf(file, "        image->Data[x].c[image->g]=imagedata[x*4+1];\n");
                fprintf(file, "        image->Data[x].c[image->b]=imagedata[x*4+2];\n");
                fprintf(file, "        image->Data[x].c[image->a]=imagedata[x*4+3];\n");
                fprintf(file, "    }\n");
                fprintf(file, "    return image;\n");
                fprintf(file, "}\n");
                fprintf(file, "\n");
            }
        }
        else
        {
            char *buf;

            t=1;
            fprintf(file, "#define POSRED   %d\n", image->r);
            fprintf(file, "#define POSGREEN %d\n", image->g);
            fprintf(file, "#define POSBLUE  %d\n", image->b);
            fprintf(file, "#define POSALPHA %d\n\n", image->a);

            buf=(char *)malloc(image->Width*image->Height*6);
            if(buf)
            {
                t=TVPackRLE(PiFilter, &image->Data->l, buf, image->Width*image->Height);
                fprintf(file, "static unsigned char imagerle[%d]={\n   ", t);
                for(x=0;x<t-1;x++)
                {
                    fprintf(file, " %d,",buf[x]);
                    if((x%20)==0) fprintf(file, "\n   ");
                }
                fprintf(file, " %d\n};\n",buf[x]);
                free(buf);

                if(addcode)
                {
                    fprintf(file, "PIBlock *GetImage(PIPlugin *PiFilter)\n");
                    fprintf(file, "{\n");
                    fprintf(file, "    PIBlock *image;\n");
                    fprintf(file, "    int x;\n");
                    fprintf(file, "    int r, g, b, a;\n");
                    fprintf(file, "    image=TVAllocPIBlock(PiFilter, IMAGEWIDTH, IMAGEHEIGHT, 0);\n");
                    fprintf(file, "    if(image==NULL)return NULL;\n");
                    fprintf(file, "    TVUnpackRLE(PiFilter, imagerle, &image->Data->l, IMAGEWIDTH*IMAGEHEIGHT);\n");
                    fprintf(file, "    r=POSRED;\n");
                    fprintf(file, "    g=POSGREEN;\n");
                    fprintf(file, "    b=POSBLUE;\n");
                    fprintf(file, "    a=POSALPHA;\n");
                    fprintf(file, "    for(x=0;x<IMAGEWIDTH*IMAGEHEIGHT;x++)\n");
                    fprintf(file, "    {\n");
                    fprintf(file, "        PIPixel t;\n");
                    fprintf(file, "        t=image->Data[x];\n");
                    fprintf(file, "        image->Data[x].c[image->r]=t.c[r];\n");
                    fprintf(file, "        image->Data[x].c[image->g]=t.c[g];\n");
                    fprintf(file, "        image->Data[x].c[image->b]=t.c[b];\n");
                    fprintf(file, "        image->Data[x].c[image->a]=t.c[a];\n");
                    fprintf(file, "    }\n");
                    fprintf(file, "    return image;\n");
                    fprintf(file, "}\n");
                    
                    
                }
            }
        }
        fclose(file);
    }

    TVFreePIBlock(PiFilter, image);
    return;
}

int FAR PASCAL PI_Msg(PIPlugin *PiFilter,INTPTR fonc ,INTPTR req,INTPTR *tag)
{
    char tmp[256];

    switch(fonc)
    {

        case kPIEvents_ButtonUp:
        {
            switch(tag[0])
            /* identity of the selected button*/
            {
                case ID_START:
                {
                    if(TVOpenFileReq(PiFilter, &FileReq))
                    {
                        SaveCodeBrush(PiFilter, FileReq.Path, compress, addcode);
                    }
                }
                break;
                case ID_ADD_C_CODE:
                {
                    addcode=1-addcode;
                    TVChangeButtonReq(PiFilter, greq, ID_ADD_C_CODE, bPIButtonFlags_Check|(addcode ? bPIButtonFlags_Select : 0), NULL);
                }
                break;
                case ID_RLE:
                {
                    compress=1-compress;
                    TVChangeButtonReq(PiFilter, greq, ID_RLE, bPIButtonFlags_Check|(compress ? bPIButtonFlags_Select : 0), NULL);
                }
                break;
            }
        }
        break;

        case kPIEvents_WindowOpen:
        {
            greq=req;

            TVReadUserString(PiFilter, PiFilter->PIName, "ADD_CODE", tmp, "1", 255);
            addcode=atoi(tmp) ? 1 : 0;
            TVReadUserString(PiFilter, PiFilter->PIName, "COMPRESS", tmp, "1", 255);
            compress=atoi(tmp) ? 1 : 0;

            TVAddButtonReq(PiFilter,req, 9, 5, 101, 60, ID_ICON, bPIButtonFlags_Invert|bPIButtonFlags_Quiet, NULL);
            TVPutButtonImage(PiFilter, req, ID_ICON, GetImage(PiFilter), 0);

            TVAddButtonReq(PiFilter,req, 91, 70, 0, 0, ID_RLE, bPIButtonFlags_Check|(compress ? bPIButtonFlags_Select : 0), "Compress");
            TVAddButtonReq(PiFilter,req, 91, 90, 0, 0, ID_ADD_C_CODE, bPIButtonFlags_Check|(addcode ? bPIButtonFlags_Select : 0), "Add C code");
            TVAddButtonReq(PiFilter,req, 9,110, 101, 0, ID_START, bPIButtonFlags_Action, "Save");
        }
        break;

        case kPIEvents_WindowClose:
        {

            snprintf( tmp, sizeof(tmp), "%d", (int)tag[0]);
            TVWriteUserString(PiFilter, PiFilter->PIName, "WIN_X", tmp);
            snprintf( tmp, sizeof(tmp), "%d", (int)tag[1]);
            TVWriteUserString(PiFilter, PiFilter->PIName, "WIN_Y", tmp);

            snprintf( tmp, sizeof(tmp), "%d", (int)tag[4]);
            TVWriteUserString(PiFilter, PiFilter->PIName, "OPEN", tmp);

            TVWriteUserString(PiFilter, PiFilter->PIName, "PATH", FileReq.Path);
            TVWriteUserString(PiFilter, PiFilter->PIName, "ADD_CODE", addcode ? "1" : "0");
            TVWriteUserString(PiFilter, PiFilter->PIName, "COMPRESS", compress ? "1" : "0");

            greq=0;
        }
        break;

    }
    return 1;
}

/****************************************************************/

void FAR PASCAL PI_About(PIPlugin *PiFilter)
    {
    char text[1024];

    snprintf( text, sizeof(text),"%s %d,%d",PiFilter->PIName,PiFilter->PIVersion,PiFilter->PIRevision);

    TVWarning(PiFilter, text);
}

/****************************************************************/

int FAR PASCAL PI_Parameters(PIPlugin *PiFilter, const char *arg)
{
    if(arg)
    {
        // none
    }
    else
    {
        INTPTR   req;
        if(greq==0)
        {
            char temp[256];
            int x=100, y=100;
            unsigned long f=bPIRequesterFlags_Standard;
            
            TVReadUserString(PiFilter, PiFilter->PIName, "WIN_X", temp, "none", 255);
            if(!strcmp(temp, "none"))
                f |= bPIRequesterFlags_Center;
            else
                x=atoi(temp);

            TVReadUserString(PiFilter, PiFilter->PIName, "WIN_Y", temp, "none", 255);
            if(!strcmp(temp, "none"))
                f |= bPIRequesterFlags_Center;
            else
                y=atoi(temp);

            req=TVOpenReq(PiFilter, 120, 132, x, y, f, "Image Print");
            if(req==0)
            {
                TVWarning(PiFilter,"Can't open requester !");
                return 0;
            }
        }
        else
        {
            TVReqToFront(PiFilter, greq);
        }
    }
    return 1;
    }

/****************************************************************/
void FAR PASCAL PI_Close(PIPlugin *PiFilter)
{
    if(greq != 0)
    {
        TVCloseReq(PiFilter, greq);
        greq=0;
    }

    TVFreeFileReq(PiFilter, &FileReq);

    if(PiFilter->UserParametersPtr != NULL)
    {
        free(PiFilter->UserParametersPtr);
        PiFilter->UserParametersPtr=NULL;
    }
    return;
}
