/**************************************************************************************/
/**************************************************************************************/
/*                                                                                    */
/*                                    IMAGE FLIPPER                                   */
/*                                                                                    */
/**************************************************************************************/
/**************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include  "TVPaintSDK.h"

#ifdef WIN32
#include <malloc.h>
#endif

// All functions not exported should be static.
// All global variables should be static.


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// The heart of the filter.
// This function does the real work, everything else is just sugar
// to make it look nice in TVPaint.

// This function flips a block of pixels.

// It is always a good practice to separe the actual computation from
// the GUI, so it's easy to reuse the computation somewhere else
// (like in another filter who needs it as a part of it's work).

typedef enum
{
    kFlipX,
    kFlipY
} FlipDirection;

typedef struct
{
    FlipDirection  mDirection;
} FlipParams;


static void
Flip( const PIBlock* iSrc, PIBlock* oDst, FlipParams iParams )
{
    // some sanity checks
    if( !iSrc  ||  !oDst )
        return;
    if( iSrc->Width != oDst->Width  ||  iSrc->Height != oDst->Height )
        return;

    // no optimization for clarity !

    // here we just move pixels around, so we don't care about
    // the RGBA content of the pixels.
    switch( iParams.mDirection )
    {
        case kFlipX:
        {
            int  x, y;
            for( y = 0; y < oDst->Height; ++y )
            {
                PIPixel*  src = iSrc->Data + y*iSrc->Width;
                PIPixel*  dst = oDst->Data + y*oDst->Width + oDst->Width - 1;
                for( x = 0; x < oDst->Width; ++x )
                {
                    *dst = *src;
                    ++src;
                    --dst;
                }
            }
        }
        break;

        case kFlipY:
        {
            int  x, y;
            for( x = 0; x < oDst->Width; ++x )
            {
                PIPixel*  src = iSrc->Data + x;
                PIPixel*  dst = oDst->Data + x + oDst->Width*(oDst->Height-1);
                for( y = 0; y < oDst->Height; ++y )
                {
                    *dst = *src;
                    src += iSrc->Width;
                    dst -= oDst->Width;
                }
            }
        }
        break;
    }
}





////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// TVPaint interface


// sizes of some GUI components

// 185 is the standard width of most requesters in TVPaint.
// you should try to respect it, as this makes life easier for the end user
// (for stacking several requesters, and so on...).
#define REQUESTER_W  185
#define REQUESTER_H  30


// ID's of GUI components
#define ID_FLIPX     10
#define ID_FLIPY     11



/**************************************************************************************/
// all data needed by the plugin

static struct
{
    FlipParams  mParams;    // Parameters

    INTPTR      mReq;       // Identification of the requester.  (=0 closed, !=0 requester ID)

    void*       mLocalFile; // file containing localized messages
}
Data =
{
    { kFlipX },
    0,
    NULL
};



/**************************************************************************************/
//  Localisation

// numbers (like 10011) are IDs in the localized file.
// strings are the default values to use when the ID is not found
// in the localized file (or the localized file doesn't exist).
#define TXT_NAME        GetLocalString( iFilter, 100,    "SDK Flip" )

#define TXT_REQUESTER   GetLocalString( iFilter, 10000,  "Filter : Flip Image" )

#define TXT_FLIPX       GetLocalString( iFilter, 10010,  "FlipX" )
#define TXT_FLIPY       GetLocalString( iFilter, 10011,  "FlipY" )

#define TXT_HELP_FLIPX  GetLocalString( iFilter, 20010,  "Flips the image in X" )
#define TXT_HELP_FLIPY  GetLocalString( iFilter, 20011,  "Flips the image in Y" )

#define TXT_ERROR01     GetLocalString( iFilter, 30000,  "Can't Open Requester !" )


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

// The functions directly called by ATVPaintura through the plugin interface



/**************************************************************************************/
// "About" function.


void FAR PASCAL
PI_About( PIPlugin* iFilter )
{
    char  text[256];

    snprintf( text, sizeof(text), "%s %d,%d", iFilter->PIName, iFilter->PIVersion, iFilter->PIRevision );

    // Just open a warning popup with the filter name and version.
    // You can open a much nicer requester if you want.
    TVWarning( iFilter, text );
}


/**************************************************************************************/
// Function called at TVPaint startup, when the filter is loaded.
// Should do as little as possible to keep Aura's startup time small.

int FAR PASCAL
PI_Open( PIPlugin* iFilter )
{
    char  tmp[256];

    // Load the .loc file.
    // We don't really cares if it fails here, since we do care in GetLocalString()
    Data.mLocalFile = TVOpenLocalFile( iFilter, "sdk-flip.loc", 0 );

    strcpy( iFilter->PIName, TXT_NAME );
    iFilter->PIVersion = 1;
    iFilter->PIRevision = 1;

    // If this plugin was the one open at TVPaint shutdown, re-open it
    TVReadUserString( iFilter, iFilter->PIName, "Open", tmp, "0", 255 );
    if( atoi( tmp ) )
    {
        PI_Parameters( iFilter, NULL ); // NULL as iArg means "open the requester"
    }

    return  1; // OK
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
}


/**************************************************************************************/
// we have something to do !

int FAR PASCAL
PI_Parameters( PIPlugin* iFilter, const char* iArg )
{
    if( iArg )
    {
        FlipParams  saveParams = Data.mParams;

        // If this string exists, some parameters are send from an external
        // program (from George or a DDE command). In this case, DO NOT
        // open the user interface but run TVExecute() (after the
        // interpretation of the parameters in the string).

        int  i;

        // Here we scan all the string of parameters.
        for( i = 0; iArg[i]; i++ )
        {
            switch( tolower( iArg[i] ) )
            {
                case 'x':
                    // found an 'x', do a flip
                    Data.mParams.mDirection = kFlipX;
                    TVExecute( iFilter );
                    break;
                case 'y':
                    // found an 'y', do a flip
                    Data.mParams.mDirection = kFlipY;
                    TVExecute( iFilter );
                    break;

                // Just ignore all other characters
                // We could also show a warning
            }
        }

        Data.mParams = saveParams;
    }
    else
    {
        // If the requester is not open, we open it.
        if( Data.mReq == 0 )
        {
            // We use a variable to contains the vertical position of the buttons.
            // Each time we create a button, we add its size to this variable.
            // This makes it very easy to add/remove/displace buttons in a requester.
            int  y = 5;

            // Create the requester, without a "menu bar" 'coz we don't need it
            // in this simple example.
            // Also we give 'NULL' as the 'Message Function' for this requester,
            // so all his messages will be sent to PI_Msg.
            // This is an acceptable practice when there are just a few buttons.
            INTPTR  req = TVOpenFilterReqEx( iFilter, REQUESTER_W, REQUESTER_H, NULL, NULL, bPIRequesterFlags_Standard, bPIFilterFlags_NoTopBar );
            if( req == 0 )
            {
                TVWarning( iFilter, TXT_ERROR01 );
                return  0;
            }
            Data.mReq = req;

            // This is a very simple requester, so we create it's content right here instead
            // of waiting for the kPIEvents_WindowOpen message...
            // Not recommended for more complex requesters. (see the other examples)

            // Sets the title of the requester.
            TVSetReqTitle( iFilter, Data.mReq, TXT_REQUESTER );

            // Creates a button in the requester (0 as height means use standard value).
            // The ID of the button is ID_FLIPX.
            // The type of the button is PIRBF_BUTTON_NORMAL.
            // The string "Flip X" is written in the middle of the button.
            TVAddButtonReq( iFilter, Data.mReq, 9, y, REQUESTER_W-19, 0, ID_FLIPX, bPIButtonFlags_Normal|bPIButtonFlags_Action, TXT_FLIPX );

            // Put a help messages on it.
            TVSetButtonInfoText( iFilter, Data.mReq, ID_FLIPX, TXT_HELP_FLIPX );                                                     // Help Popup

            // On to the next button !
            y += 20;

            // And another button.
            TVAddButtonReq( iFilter, Data.mReq, 9, y, REQUESTER_W-19, 0, ID_FLIPY, bPIButtonFlags_Normal|bPIButtonFlags_Action, TXT_FLIPY );
            TVSetButtonInfoText( iFilter, Data.mReq, ID_FLIPY, TXT_HELP_FLIPY );                                                     // Help Popup
            y += 20;
        }
        else
        {
            // If it is already open, we just put it on front of all other requesters.
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
    // what did happen ?
    switch( iEvent )
    {
        // The user just 'clicked' on a normal button
        case kPIEvents_ButtonUp:
        {
            switch( iArgs[0] )   // iArgs[0] is the ID of the selected button
            {
                case ID_FLIPX:   // "Flip X" button selected
                    // Update the data with the new parameter(s)
                    // Here we just set the direction of the flip
                    Data.mParams.mDirection = kFlipX;

                    // This call tells TVPaint to call the following functions in our plugin :
                    // PI_SequenceStart, PI_Start, PI_Work, PI_Finish and PI_SequenceFinish
                    // in the right order.
                    TVExecute( iFilter );
                    break;

                case ID_FLIPY:   // "Flip Y" button selected
                    Data.mParams.mDirection = kFlipY;
                    TVExecute( iFilter );
                    break;
            }
        }
        break;

        // The requester was just closed.
        case kPIEvents_WindowClose:
        {
            char  tmp[256];

            // requester doesn't exists anymore
            Data.mReq = 0;

            // Save the requester state (opened or closed)
            // iArgs[4] contains a flag which tells us if the requester
            // has been closed by the user (flag=0) or by Aura's shutdown (flag=1).
            // If it was by Aura's shutdown, that means this requester was the
            // last one open, so we should reopen this one the next time TVPaint
            // is started.  Else we won't open it next time.
            snprintf( tmp, sizeof(tmp), "%d", (int)(iArgs[4]) );

            // Save it in Aura's init file.
            TVWriteUserString( iFilter, iFilter->PIName, "Open", tmp );
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
    Flip( iFilter->Undo, iFilter->Current, Data.mParams );

    // Update the display.
    // In case of very slow filters, you should update the part of
    // the display that has already been computed from time to time.
    // Here we just update the whole display at once at the end.
    TVUpdateDisplay( iFilter, iFilter->WorkArea_x1, iFilter->WorkArea_y1, iFilter->WorkArea_x2, iFilter->WorkArea_y2 );

    // 1 means 'continue', 0 means 'error, abort' (like 'not enough memory')
    return  1;
}


