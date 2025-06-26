/*********************************************************************/
/*********************************************************************/
/*                      LUMARANGE                                    */
/*-------------------------------------------------------------------*/
/*              Version 2.0 (15 september 1999)                      */
/*********************************************************************/
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include  "TVPaintSDK.h"
#include "lumarange.h"

#ifdef WIN32
#include <malloc.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// TVPaint interface


// sizes of some GUI components

// 185 is the standard width of most requesters in TVPaint.
// you should try to respect it, as this makes life easier for the end user
// (for stacking several requesters, and so on...).
#define REQUESTER_W  185
#define REQUESTER_H  107


// ID's of GUI components
#define ID_CRANGE    10
#define ID_SOURCE    20
#define ID_SOURCE2   21
#define ID_REPLACE   30
#define ID_PREVIEW   40
#define ID_APPLY     50

#define ID_SEP01     1000
#define ID_SEP02     1001



/**************************************************************************************/
// all data needed by the plugin

static struct
{
    LumaRangeParams  mParams;    // Parameters

    INTPTR           mReq;       // Identification of the requester.  (=0 closed, !=0 requester ID)

    void*            mLocalFile; // file containing localized messages

    int              mDoPreview; // preview the effect ?
}
Data =
{
    {
        kLuminosity,
        0,
        NULL
    },
    0,
    NULL,
    0
};


/**************************************************************************************/
//  Localisation

// numbers (like 10010) are IDs in the localized file.
// strings are the default values to use when the ID is not found
// in the localized file (or the localized file doesn't exist).
#define TXT_NAME                   GetLocalString( iFilter, 100,    "SDK Luma Range" )

#define TXT_REQUESTER              GetLocalString( iFilter, 10000,  "Filter : Luma Range" )

#define TXT_CRANGE                 GetLocalString( iFilter, 10010,  "Range" )
#define TXT_SOURCE                 GetLocalString( iFilter, 10020,  "Source" )
#define TXT_REPLACE                GetLocalString( iFilter, 10030,  "Replace" )
#define TXT_PREVIEW                GetLocalString( iFilter, 10040,  "Preview" )
#define TXT_APPLY                  GetLocalString( iFilter, 10050,  "Apply" )

#define TXT_CHANNEL_LUMINOSITY     GetLocalString( iFilter, 10100,  "Luma" )
#define TXT_CHANNEL_ALPHA          GetLocalString( iFilter, 10110,  "Alpha" )
#define TXT_CHANNEL_RED            GetLocalString( iFilter, 10120,  "Red" )
#define TXT_CHANNEL_GREEN          GetLocalString( iFilter, 10130,  "Green" )
#define TXT_CHANNEL_BLUE           GetLocalString( iFilter, 10140,  "Blue" )

#define TXT_HELP_CRANGE            GetLocalString( iFilter, 20010,  "Grab the current color range" )
#define TXT_HELP_SOURCE            GetLocalString( iFilter, 20020,  "Sets the source of the filter" )
#define TXT_HELP_REPLACE           GetLocalString( iFilter, 20030,  "Replace with color range (no Alpha)" )
#define TXT_HELP_PREVIEW           GetLocalString( iFilter, 20040,  "Enable full screen preview" )
#define TXT_HELP_APPLY             GetLocalString( iFilter, 20050,  "Apply filter" )

#define TXT_ERROR01                GetLocalString( iFilter, 30000,  "Can't Open Requester !" )


static const char*
GetLocalString( PIPlugin* iFilter, int iNum, const char* iDefault )
{
    char*  str;

    if( Data.mLocalFile == NULL )
        return  iDefault;

    str = TVGetLocalString( iFilter, Data.mLocalFile, iNum );
    if( str == NULL  ||  strlen( str ) == 0 )
        return  iDefault;

    return  str;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Everything needed by the requester


// Puts the 'preview' button in the correct state
static void
DoPreviewButton( PIPlugin* iFilter )
{
    if( Data.mDoPreview )
    {
        TVChangeButtonReq( iFilter, Data.mReq, ID_PREVIEW, bPIButtonFlags_Check|bPIButtonFlags_Select, NULL );
    }
    else
    {
        TVChangeButtonReq( iFilter, Data.mReq, ID_PREVIEW, bPIButtonFlags_Check, NULL );
    }
}


// Does a full screen preview.
static void
DoPreview( PIPlugin* iFilter )
{
    if( !Data.mDoPreview )
        return;

    TVSendCmd( iFilter, "tv_lockmouse 2", NULL );
    LumaRange( iFilter->Undo, iFilter->Current, &Data.mParams );
    TVUpdateDisplay( iFilter, iFilter->WorkArea_x1, iFilter->WorkArea_y1, iFilter->WorkArea_x2, iFilter->WorkArea_y2 );
    memcpy( iFilter->Current->Data, iFilter->Undo->Data, iFilter->Current->Width*iFilter->Current->Height*sizeof(PIPixel32) );
    TVSendCmd( iFilter, "tv_unlockmouse", NULL );
}


// Puts the 'source' button in the correct state.
static void
DoSourceButton( PIPlugin* iFilter )
{
    const char*  txt = NULL;

    // Choose the right text
    switch( Data.mParams.mChannel )
    {
    case kLuminosity: txt = TXT_CHANNEL_LUMINOSITY; break;
    case kAlpha:      txt = TXT_CHANNEL_ALPHA;      break;
    case kRed:        txt = TXT_CHANNEL_RED;        break;
    case kGreen:      txt = TXT_CHANNEL_GREEN;      break;
    case kBlue:       txt = TXT_CHANNEL_BLUE;       break;
    }

    // Puts the text in the button
    // Texts in popup buttons should be left aligned
    TVChangeButtonName( iFilter, Data.mReq, ID_SOURCE, txt, bPIButtonTextAlign_InLeft );
}


// Shows the popup for the 'source' button.
static void
DoSourcePopup( PIPlugin* iFilter )
{
    // Define a PIPopup structure
    PIPopup  pop[]=
    {
        { TXT_CHANNEL_LUMINOSITY, kLuminosity },
        { TXT_CHANNEL_ALPHA,      kAlpha      },
        { TXT_CHANNEL_RED,        kRed        },
        { TXT_CHANNEL_GREEN,      kGreen      },
        { TXT_CHANNEL_BLUE,       kBlue       },
        { NULL }
    };

    // Shows the popup
    // The last parameter is the *index* of item selected by default, not his value, hence the '-1'
    int  channel = TVPopup( iFilter, pop, 5, Data.mParams.mChannel-1 );
    if( channel > 0 )
    {
        // If the user selected something, update everything.
        Data.mParams.mChannel = (eLumaRangeChannel)channel;
        DoSourceButton( iFilter );
        DoPreview( iFilter );
    }
}


// Puts the 'replace' button in the correct state.
static void
DoReplaceButton( PIPlugin* iFilter )
{
    if( Data.mParams.mReplace )
    {
        TVChangeButtonReq( iFilter, Data.mReq, ID_REPLACE, bPIButtonFlags_Check | bPIButtonFlags_Select, NULL );
    }
    else
    {
        TVChangeButtonReq( iFilter, Data.mReq, ID_REPLACE, bPIButtonFlags_Check, NULL );
    }
}


/**************************************************************************************/
// The function to process the events of my requester.
// In this simple example this isn't really necessary, we could have
// processed all messages in PI_Msg, but this way of doing things
// is easier to extend
static int
Requester_Msg( PIPlugin* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    switch( iEvent )
    {
        case kPIEvents_WindowOpen:
        {
            // Build the requester.
            int  y = 5;

            Data.mReq = iReq;

            TVSetReqTitle( iFilter, iReq, TXT_REQUESTER );

            // color range
            Data.mParams.mColors->Load( Data.mParams.mColors, iFilter );
            TVAddButtonReq( iFilter, iReq, 90, y, 85, 0, ID_CRANGE, bPIButtonFlags_Normal, NULL );
            TVChangeButtonName( iFilter, iReq, ID_CRANGE, TXT_CRANGE, bPIButtonTextAlign_OutLeft );
            TVPutButtonImage( iFilter, iReq, ID_CRANGE, Data.mParams.mColors->MakeBlock( Data.mParams.mColors, iFilter, 82, 13 ), 0 );
            y += 20;

            // source
            TVAddButtonReq( iFilter, iReq, 90, y, 85, 0, ID_SOURCE, bPIButtonFlags_Popup, NULL );
            DoSourceButton( iFilter );
            TVAddButtonReq( iFilter, iReq, 9, y, 80, 0, ID_SOURCE2, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, TXT_SOURCE );
            TVChangeButtonName( iFilter, iReq, ID_SOURCE2, TXT_SOURCE, bPIButtonTextAlign_InRight );
            y += 20;

            // replace
            TVAddButtonReq( iFilter, iReq, 90, y, 0, 0, ID_REPLACE, bPIButtonFlags_Check, TXT_REPLACE );
            DoReplaceButton( iFilter );
            TVSetButtonInfoText( iFilter, iReq, ID_REPLACE, TXT_HELP_REPLACE );
            y += 20;

            // sep01
            TVAddButtonReq( iFilter, iReq, 0, y, REQUESTER_W, 0, ID_SEP01, bPIButtonFlags_HSeparator, NULL );
            y += 8;

            // preview
            TVAddButtonReq( iFilter, iReq, 90, y, 0, 0, ID_PREVIEW, bPIButtonFlags_Check, TXT_PREVIEW );
            DoPreviewButton( iFilter );
            TVSetButtonInfoText( iFilter, iReq, ID_PREVIEW, TXT_HELP_PREVIEW );
            y += 20;

            // sep02
            TVAddButtonReq( iFilter, iReq, 0, y, REQUESTER_W, 0, ID_SEP02, bPIButtonFlags_HSeparator, NULL );
            y += 8;

            // apply
            TVAddButtonReq( iFilter, iReq, 9, y, REQUESTER_W-19, 0, ID_APPLY, bPIButtonFlags_Action, TXT_APPLY );
            TVSetButtonInfoText( iFilter, iReq, ID_APPLY, TXT_HELP_APPLY );
            y += 20;

            DoPreview( iFilter );
        }
        break;

        case kPIEvents_WindowClose:
        {
            // The requester has been closed.
            char  tmp[256];

            if( Data.mDoPreview )
                TVCloseFunction( iFilter );

            // In TVPaint, you shouldn't save the state of the preview toggle,
            // preview is always off when a filter is started for the first time.

            snprintf( tmp, sizeof(tmp), "%d", (int)iArgs[4] );
            TVWriteUserString( iFilter, iFilter->PIName, "Open", tmp );

            // Save the source channel.
            snprintf( tmp, sizeof(tmp), "%d", (int)(Data.mParams.mChannel) );
            TVWriteUserString( iFilter, iFilter->PIName, "Channel", tmp );

            // Save the 'replace' mode.
            snprintf( tmp, sizeof(tmp), "%d", (int)(Data.mParams.mReplace) );
            TVWriteUserString( iFilter, iFilter->PIName, "Replace", tmp );

            // Save the color range.
            Data.mParams.mColors->Save( Data.mParams.mColors, iFilter );

            Data.mReq = 0;
        }
        break;

        case kPIEvents_ButtonUp:
        {
            switch( iArgs[0] )
            {
                case ID_CRANGE:
                    // Grab the current color range.
                    Data.mParams.mColors->Grab( Data.mParams.mColors, iFilter );
                    TVPutButtonImage( iFilter, iReq, ID_CRANGE, Data.mParams.mColors->MakeBlock( Data.mParams.mColors, iFilter, 82, 13 ), 0 );
                    DoPreview( iFilter );
                    break;

                case ID_REPLACE:
                    // Toggle the 'replace' mode.
                    Data.mParams.mReplace = !Data.mParams.mReplace;
                    DoReplaceButton( iFilter );
                    DoPreview( iFilter );
                    break;

                case ID_PREVIEW:
                    // Toggle the preview.
                    if( !Data.mDoPreview )
                    {
                        TVInstallFunction( iFilter, kPIMeta_Preview );
                        Data.mDoPreview = 1;
                    }
                    else
                    {
                        TVCloseFunction( iFilter );
                    }
                    DoPreviewButton( iFilter );
                    DoPreview( iFilter );
                    break;

                case ID_APPLY:
                    // Apply the filter.
                    if( Data.mDoPreview )
                        TVCloseFunction( iFilter );
                    TVExecute( iFilter );
                    break;
            }
        }
        break;

        case kPIEvents_ButtonDown:
        {
            switch( iArgs[0] )
            {
                case ID_SOURCE:
                    // Show the popup.
                    DoSourcePopup( iFilter );
                break;
            }
        }
        break;

        case kPIEvents_ProjectChanged:
        {
            // Redo the preview.
            DoPreview( iFilter );
        }
        break;
    }

    return  1;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// The functions directly called by TVPaint through the plugin interface



/**************************************************************************************/
// "About" function.

void FAR PASCAL
PI_About( PIPlugin* iFilter )
{
    char text[256];

    snprintf( text, sizeof(text),
             "%s V%d,%d\nCopyright TVPaint Developpement\n(c) 2024!",
             iFilter->PIName, iFilter->PIVersion, iFilter->PIRevision );

    TVWarning( iFilter, text );
}




/**************************************************************************************/
// Function called at TVPaint startup, when the filter is loaded.
// Should do as little as possible to keep Aura's startup time small.

int FAR PASCAL
PI_Open( PIPlugin* iFilter )
{
    char  tmp[512];

    Data.mLocalFile = TVOpenLocalFile( iFilter, "lumrange.loc", 0 );//////NAME:

    Data.mParams.mColors = ColorRangeNew( iFilter );

    strcpy( iFilter->PIName, TXT_NAME );
    iFilter->PIVersion = 2;
    iFilter->PIRevision = 1;

    // Read the channel setting
    TVReadUserString( iFilter, iFilter->PIName, "Channel", tmp, "1", 255 );
    if( atoi( tmp ) >= kLuminosity  &&  atoi( tmp ) <= kBlue )
        Data.mParams.mChannel = (eLumaRangeChannel)(atoi( tmp ));

    // Read the 'replace' setting
    TVReadUserString( iFilter, iFilter->PIName, "Replace", tmp, "0", 255 );
    Data.mParams.mReplace = atoi( tmp );

    TVReadUserString( iFilter, iFilter->PIName, "Open", tmp, "0", 255 );
    if( atoi( tmp ) )
    {
        PI_Parameters( iFilter, NULL );
        DoPreview( iFilter );
    }

    return  1;
}


/**************************************************************************************/
// TVPaint shutdown: we make all the necessary cleanup

void FAR PASCAL
PI_Close( PIPlugin* iFilter )
{
    if( Data.mLocalFile )
    {
        TVCloseLocalFile( iFilter, Data.mLocalFile );
    }

    if( Data.mReq )
    {
        TVCloseReq( iFilter, Data.mReq );
    }

    if( Data.mParams.mColors )
    {
        ColorRangeDelete( iFilter, Data.mParams.mColors );
    }

}


/**************************************************************************************/
// we have something to do !

int FAR PASCAL
PI_Parameters( PIPlugin* iFilter, const char* iArg )
{
    if( iArg )
    {
        // Save everything of importance
        ColorRange       saveColors = *Data.mParams.mColors;
        LumaRangeParams  saveParams = Data.mParams;
        char*  tmp = strdup( iArg );

        if( tmp )
        {
            int  i;

            for( i = 0; i < strlen(tmp); ++i )
            {
                if( isalpha( tmp[i] ) )
                {
                    tmp[i] = tolower( tmp[i] );
                }
            }

            // Look for the word 'replace'
            if( strstr( tmp, "replace" ) )
            {
                Data.mParams.mReplace = 1;
            }

            // Look for a 'source' name.
            if( strstr( tmp, "luma" ) )
            {
                Data.mParams.mChannel = kLuminosity;
            }
            else if( strstr( tmp, "alpha" ) )
            {
                Data.mParams.mChannel = kAlpha;
            }
            else if( strstr( tmp, "red" ) )
            {
                Data.mParams.mChannel = kRed;
            }
            else if( strstr( tmp, "green" ) )
            {
                Data.mParams.mChannel = kGreen;
            }
            else if( strstr( tmp, "blue" ) )
            {
                Data.mParams.mChannel = kBlue;
            }

            free( tmp );
        }

        // Grab the current range
        Data.mParams.mColors->Grab( Data.mParams.mColors, iFilter );

        // Execute the filter
        TVExecute( iFilter );

        // Restore everything
        Data.mParams = saveParams;
        *Data.mParams.mColors = saveColors;
    }
    else
    {
        if( !Data.mReq )
        {
            INTPTR  req = TVOpenFilterReqEx( iFilter, REQUESTER_W, REQUESTER_H, Requester_Msg, NULL, 0, bPIFilterFlags_NoTopBar );
            if( req == 0 )
            {
                TVWarning( iFilter, TXT_ERROR01 );
                return  0;
            }
        }
        else
        {
            TVReqToFront( iFilter, Data.mReq );
        }
    }

    return  1;
}


/**************************************************************************************/
// something happenned that needs our attention.

int FAR PASCAL
PI_Msg( PIPlugin* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    switch( iEvent )
    {
        case kPIEvents_MetaClosed:
        {
            // We have to close our preview 'coz the user selected another tool.
            if( Data.mDoPreview )
            {
                Data.mDoPreview = 0;
                DoPreviewButton( iFilter );

                TVUpdateDisplay( iFilter, iFilter->WorkArea_x1, iFilter->WorkArea_y1, iFilter->WorkArea_x2, iFilter->WorkArea_y2 );
            }
        }
        break;

        case kPIEvents_MetaPreview:
        {
            // We have to redo the preview 'coz the user changed something that affect it.
            // (Like changing the background mode...)
            DoPreview( iFilter );
        }
        break;

    }
    return  1;
}


/**************************************************************************************/
// Start of the 'execution' of the filter for a new sequence.
// - iNumImages contains the total number of frames to be processed.
// Here you should allocate memory that is used for all frames,
// and precompute all the stuff that doesn't change from frame to frame.

int FAR PASCAL
PI_SequenceStart( PIPlugin* iFilter, int iNumImages )
{
    // In this simple example we don't have anything to allocate/precompute.

    // 1 means 'continue', 0 means 'error, abort' (like 'not enough memory')
    return  1;
}


// Here you should cleanup what you've done in PI_SequenceStart

void FAR PASCAL
PI_SequenceFinish( PIPlugin* iFilter )
{
    // nothing special to cleanup
}


/**************************************************************************************/
// This is called before each frame.
// Here you should allocate memory and precompute all the stuff you can.

int FAR PASCAL
PI_Start( PIPlugin* iFilter, double iPos, double iSize )
{
    // In this simple example we don't have anything to allocate/precompute.

    // 1 means 'continue', 0 means 'error, abort' (like 'not enough memory')
    return  1;
}


void FAR PASCAL
PI_Finish( PIPlugin* iFilter )
{
    // nothing special to cleanup
}


/**************************************************************************************/
// 'Execution' of the filter.
// This is the only function in which you have the right to change
// the iFilter->Current image.
// In all other functions you just have the right to read it.

int FAR PASCAL
PI_Work( PIPlugin* iFilter )
{
    LumaRange( iFilter->Undo, iFilter->Current, &Data.mParams );

    TVUpdateDisplay( iFilter, iFilter->WorkArea_x1, iFilter->WorkArea_y1, iFilter->WorkArea_x2, iFilter->WorkArea_y2 );

    return  1;
}

