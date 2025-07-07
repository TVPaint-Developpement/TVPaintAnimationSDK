/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-private.c
*  @ingroup  sdk_TVPA
*
*  @brief  The implementation of the plugin-side of the SDK.
*/

#ifndef __TVPaintSDK_HH
#include "TVPaintAnimationSDK/TVPaintSDK.h"
#endif
#ifndef __pi_private_HH
#include "TVPaintAnimationSDK/pi-private.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN64)
#include <malloc.h>
#endif


//MARK: ========================================================================================================= pi-basics.h

// nothing

//MARK: ========================================================================================================= pi-block.h

PIBlock*
TVAllocPIBlock( PIFilter*  iPlugin, int  iWidth,  int  iHeight, PIFlags  iFlags )
{
    INTPTR  arg[] = { iWidth, iHeight, iFlags, 0,0,0,0 };

    return  (PIBlock*)iPlugin->CallBack( iPlugin, CB_ALLOC_PIBLOCK, arg );
}


void
TVFreePIBlock( PIFilter*  iPlugin, PIBlock*  iBlock )
{
    INTPTR  arg[] = { (INTPTR)iBlock, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_FREE_PIBLOCK, arg );
}


int
TVTextBlock( PIFilter*  iPlugin, PIBlock*  ioBlock, int  iX, int  iY, const char*  iText, PIPixel32  iAPen )
{
    INTPTR  arg[] = { (INTPTR)ioBlock, iX, iY, (INTPTR)iText, iAPen, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_TEXT_TO_BLOCK, arg );
}


int
TVWrapPIBlock( PIFilter*  iPlugin, const PIBlock*  iSrc, PIBlock*  ioDst,
               double  iX1, double  iY1, double  iX2, double  iY2,
               double  iX3, double  iY3, double  iX4, double  iY4 )
{
    INTPTR   arg[] = { (INTPTR)(iSrc), (INTPTR)(ioDst),
                       iX1*65536., iY1*65536., iX2*65536., iY2*65536.,
                       iX3*65536., iY3*65536., iX4*65536., iY4*65536.,
                       0,0,0,0
    };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRAP_BLOCK, arg );
}


int  TVWrapPIBlockMode( PIFilter*  iPlugin, const PIBlock*  iSrc, PIBlock*  ioDst,
                        double  iX1, double  iY1, double  iX2, double  iY2,
                        double  iX3, double  iY3, double  iX4, double  iY4,
                        int  iTile, int  iBlendMode )
{
    INTPTR  arg[] = { (INTPTR)iSrc, (INTPTR)ioDst,
                      iX1*65536., iY1*65536., iX2*65536., iY2*65536.,
                      iX3*65536., iY3*65536., iX4*65536., iY4*65536.,
                      iTile, iBlendMode, 0,0,0,0
    };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRAP_BLOCK, arg );
}


int
TVReadImage( PIFilter*  iPlugin, const char*  iFileName, PIPixel*  oDst, int  iDstWidth, int  iDstHeight )
{
    INTPTR  arg[] = { (INTPTR)iFileName, (INTPTR)oDst, iDstWidth, iDstHeight, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READ_IMAGE, arg );
}


int
TVBlockBlur( PIFilter*  iPlugin, PIBlock*  ioBlock, double  iBlurX, double  iBlurY, int  iMirrorBorder )
{
    INTPTR  arg[] = { PIBCMD_BLUR, (INTPTR)ioBlock, iBlurX*65536., iBlurY*65536., iMirrorBorder, 0,0,0,0 }; 

    return  (int)iPlugin->CallBack( iPlugin, CB_BLOCK_CMD, arg );
}


int
TVBlockStretch( PIFilter*  iPlugin, const PIBlock*  iSrc, PIBlock*  oDst )
{
    INTPTR  arg[] = { PIBCMD_STRETCH, (INTPTR)iSrc, (INTPTR)oDst, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_BLOCK_CMD, arg );
}


int
TVMakeBrush( PIFilter*  iPlugin, const PIPixel*  iBrush, int  iWidth, int  iHeight )
{
    INTPTR  arg[] = { (INTPTR)iBrush, iWidth, iHeight, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_MAKEBRUSH, arg );
}


int
TVMakeDisplayImage( PIFilter*  iPlugin, PIPixel*  oDst, int  iDstSize, int  iImageNumber, int  iBackground )
{
    INTPTR  arg[] = { (INTPTR)oDst, iDstSize, iImageNumber, iBackground, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_MAKE_IMAGE_DISPLAY, arg );
}


int
TVReadLayerData( PIFilter*  iPlugin, PIPixel*  oDst, int  iX, int  iY, int  iW, int  iH, int  iMod, ePIReadLayer  iLayer )
{
    INTPTR   arg[] = { (INTPTR)oDst, iX, iY, iW, iH, iMod, iLayer, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READLAYERDATA, arg );
}


int
TVWriteLayerData( PIFilter*  iPlugin, const PIPixel*  iSrc, int  iX, int  iY, int  iW, int  iH, int  iMod, ePIWriteLayer  iDisplay )
{
    INTPTR  arg[] = { (INTPTR)iSrc, iX, iY, iW, iH, iMod, iDisplay, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRITELAYERDATA, arg );
}


int
TVReadProjectImage( PIFilter*  iPlugin, PIBlock*  oDst, int  iProjectID, int  iLayerID, int  iImageNumber )
{
    INTPTR  arg[] = { (INTPTR)oDst, iProjectID, iLayerID, iImageNumber, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READ_PROJECT_IMAGE, arg );
}


//MARK: ========================================================================================================= pi-display.h


int
TVAddDisplayDevice( PIFilter*  iPlugin, PIDisplay*  iDevice, PIFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)iDevice, iFlags, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_ADD_DISPLAYDEVICE, arg );
}


const char*
TVEnumDisplayDevice( PIFilter*  iPlugin, int  iIndex, PIFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)iIndex, iFlags, 0,0,0,0 };

    return  (const char*)iPlugin->CallBack( iPlugin, CB_ENUM_DISPLAYDEVICE, arg );
}


PIDisplay*
TVLockDisplayDevice( PIFilter*  iPlugin, const char*  iName, PIFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)iName, iFlags, 0,0,0,0 };

    return  (PIDisplay*)iPlugin->CallBack( iPlugin, CB_LOCK_DISPLAYDEVICE, arg );
}


void
TVUnlockDisplayDevice( PIFilter*  iPlugin, PIDisplay*  iDevice )
{
    INTPTR  arg[] = { (INTPTR)iDevice, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_UNLOCK_DISPLAYDEVICE, arg );
}


//MARK: ========================================================================================================= pi-filerequester.h


int
TVAllocFileReq( PIFilter*  iPlugin, PIFileRequester*  iFileReq )
{
    INTPTR  arg[] = { (INTPTR)iFileReq, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_ALLOC_FILEREQ, arg );
}


int
TVOpenFileReq( PIFilter*  iPlugin, PIFileRequester*  iFileReq )
{
    INTPTR  arg[] = { (INTPTR)iFileReq, 0,0,0,0 };
    
    return  (int)iPlugin->CallBack( iPlugin, CB_OPEN_FILEREQ, arg );
}


void
TVFreeFileReq( PIFilter*  iPlugin, PIFileRequester*  iFileReq )
{
    INTPTR  arg[] = { (INTPTR)iFileReq, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_FREE_FILEREQ, arg );
}


//MARK: ========================================================================================================= pi-filter.h

// nothing

//MARK: ========================================================================================================= pi-font.h


PIFont*
TVOpenPIFont( PIFilter*  iPlugin, const char*  iName, unsigned long  iScript, unsigned long  iStyle )
{
    // second iName is here just for filling some space...
    INTPTR  arg[] = { (INTPTR)iName, (INTPTR)iName, (INTPTR)iScript, (INTPTR)iStyle, 0,0,0,0 };

    return  (PIFont*)iPlugin->CallBack( iPlugin, CB_OPEN_FONT, arg );
}


//MARK: ========================================================================================================= pi-hud.h


void
TVHudRedraw( PIFilter*  iPlugin )
{
    INTPTR  arg[] = { PIHUDCMD_REDRAW, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


void
TVHudLine( PIFilter*  iPlugin, float  iX1, float  iY1, float  iX2, float  iY2, PIPixel  iColor )
{
    //TODO: //FUCK: Why floats are passed as float, and double are passed as fixed-point ?!?!?!?!?!?!??!??!?!?!??!?!?!??!?!??!??!?!??!??!??!??!?!
    INTPTR  arg[16];

    arg[0] = PIHUDCMD_LINE;
    *(float*)&arg[1] = iX1;
    *(float*)&arg[2] = iY1;
    *(float*)&arg[3] = iX2;
    *(float*)&arg[4] = iY2;
    arg[5] = (INTPTR)iColor.l;
    arg[6] = 0;
    arg[7] = 0;

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


void
TVHudSpline(PIFilter*  iPlugin,  float  iX1, float  iY1, float  iX12, float  iY12, float  iX2, float  iY2, PIPixel  iColor )
{
    //TODO: //FUCK: Why floats are passed as float, and double are passed as fixed-point ?!?!?!?!?!?!??!??!?!?!??!?!?!??!?!??!??!?!??!??!??!??!?!
    INTPTR  arg[16];

    arg[0] = PIHUDCMD_SPLINE;
    *(float*)&arg[1] = iX1;
    *(float*)&arg[2] = iY1;
    *(float*)&arg[3] = iX12;
    *(float*)&arg[4] = iY12;
    *(float*)&arg[5] = iX2;
    *(float*)&arg[6] = iY2;
    arg[7] = (INTPTR)iColor.l;
    arg[8] = 0;
    arg[9] = 0;

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


void
TVHudText( PIFilter*  iPlugin,  float  iX, float  iY, const char*  iText, PIFlags/*ePIHudAlign*/  iAlign, PIPixel  iColor, int  iSelected )
{
    //TODO: //FUCK: Why floats are passed as float, and double are passed as fixed-point ?!?!?!?!?!?!??!??!?!?!??!?!?!??!?!??!??!?!??!??!??!??!?!
    INTPTR  arg[16];

    arg[0] = PIHUDCMD_TEXT;
    *(float*)&arg[1] = iX;
    *(float*)&arg[2] = iY;
    arg[3] = (INTPTR)iText;
    arg[4] = (INTPTR)iAlign;
    arg[5] = (INTPTR)iColor.l;
    arg[6] = iSelected;
    arg[7] = 0;
    arg[8] = 0;

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


void
TVHudHandle( PIFilter*  iPlugin,  float  iX, float  iY, ePIHudHandle  iHandle, PIPixel  iColor, int  iSelected )
{
    //TODO: //FUCK: Why floats are passed as float, and double are passed as fixed-point ?!?!?!?!?!?!??!??!?!?!??!?!?!??!?!??!??!?!??!??!??!??!?!
    INTPTR  arg[16];

    arg[0] = PIHUDCMD_HANDLE;
    *(float*)&arg[1] = iX;
    *(float*)&arg[2] = iY;
    arg[3] = (INTPTR)iHandle;
    arg[4] = (INTPTR)iColor.l;
    arg[5] = iSelected;
    arg[6] = 0;
    arg[7] = 0;
    arg[8] = 0;

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


void
TVHudImage( PIFilter*  iPlugin,  float  iX, float  iY, float  iX1, float  iY1, float  iX2, float  iY2, float  iX3, float  iY3, float  iX4, float  iY4, const PIBlock*  iBlock )
{
    //TODO: //FUCK: Why floats are passed as float, and double are passed as fixed-point ?!?!?!?!?!?!??!??!?!?!??!?!?!??!?!??!??!?!??!??!??!??!?!
    INTPTR  arg[16];

    arg[0] = PIHUDCMD_IMAGE;
    *(float*)&arg[1] = iX;
    *(float*)&arg[2] = iY;
    *(float*)&arg[3] = iX1;
    *(float*)&arg[4] = iY1;
    *(float*)&arg[5] = iX2;
    *(float*)&arg[6] = iY2;
    *(float*)&arg[7] = iX3;
    *(float*)&arg[8] = iY3;
    *(float*)&arg[9] = iX4;
    *(float*)&arg[10] = iY4;
    arg[11] = (INTPTR)iBlock;

    iPlugin->CallBack( iPlugin, CB_HUD_CMD, arg );
}


//MARK: ========================================================================================================= pi-imagesequence.h


PIImageSequence*
TVOpenImageSequence( PIFilter*  iPlugin, const char*  iFileName, ePIImageSequenceFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)(iFileName), (INTPTR)(iFlags), 0,0,0,0 };

    return  (PIImageSequence*)iPlugin->CallBack( iPlugin, CB_OPEN_IMAGESEQUENCE, arg );
}


//MARK: ========================================================================================================= pi-keys.h


PIKeys*
TVAllocKeys( PIFilter*  iPlugin, const char*  iName, const char*  iNameID )
{
    INTPTR  arg[] = { (INTPTR)iName, (INTPTR)iNameID, 0,0,0,0 };

    return  (PIKeys*)iPlugin->CallBack( iPlugin, CB_ALLOC_KEYS, arg );
}


//MARK: ========================================================================================================= pi-main.h


int
TVCreateFilterAlias( PIFilter*  iPlugin, const char*  iAlias, const char*  iParams )
{
    INTPTR  arg[] = { (INTPTR)iAlias, (INTPTR)iParams, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_CREATE_ALIAS, arg );
}


int
TVExecute( PIFilter*  iPlugin )
{
    INTPTR  arg[] = { 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_EXECUTE, arg );
}


int
TVRunMultiThread( PIFilter*  iPlugin, int  (*iFunction)( void*  iThread, int  iStart, int  iEnd, void*  iParam), int  iStart, int  iEnd, void*  iParam )
{
    if( iPlugin->Version > 5  ||  (iPlugin->Version == 5  &&  iPlugin->Revision >= 1) )
    {
        INTPTR  arg[] = { (INTPTR)iFunction, iStart, iEnd, (INTPTR)iParam, 0,0,0,0 };

        return  (int)iPlugin->CallBack( iPlugin, CB_MULTITHREAD, arg );
    }
    else
    {
        return  iFunction( NULL, iStart, iEnd, iParam );
    }
}


int
TVSendCmd( PIFilter*  iPlugin, const char*  iCommand, char*  oResult )
{
    INTPTR   arg[] = { (INTPTR)iCommand, (INTPTR)oResult, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDCMD, arg);
}


void
TVUpdateDisplay( PIFilter*  iPlugin, int  iX1, int  iY1, int  iX2, int  iY2 )
{
    INTPTR   arg[] = { iX1, iY1, iX2, iY2, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_UPDATEDISPLAY, arg );
}


//MARK: ========================================================================================================= pi-misc.h


int
TVGrabTicks( PIFilter*  iPlugin, INTPTR  iReq, ePITicks  iFlag )
{
    INTPTR   arg[] = { PIRT_GRAB_TICKS, iReq, iFlag, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVGrabCoords( PIFilter*  iPlugin, INTPTR  iReq, int  iFlag )
{
    INTPTR   arg[] = { PIRT_GRAB_COORD, iReq, iFlag, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVGetImageID( PIFilter*  iPlugin, int  iProjectID, int  iLayerID, int  iImageIndex, char*  oImageID, char*  oDataID )
{
    INTPTR  arg[] = { iProjectID, iLayerID, iImageIndex, (INTPTR)oImageID, (INTPTR)oDataID, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_GET_IMAGE_ID, arg );
}


//MARK: ========================================================================================================= pi-pixel.h


PIPixel32
TVMakePIPixel32( int  iR, int  iG, int  iB, int  iA )
{
    PIPixel  p = { .Red=iR, .Green=iG, .Blue=iB, .Alpha=iA };

    return  p.l;
}


PIPixel
TVMakePIPixel( int  iR, int  iG, int  iB, int  iA )
{
    PIPixel  p = { .Red=iR, .Green=iG, .Blue=iB, .Alpha=iA };

    return  p;
}


void
TVColorBlend( PIFilter*  iPlugin, const PIPixel*  iSrc1, int  iDensity1, const PIPixel*  iSrc2, int  iDensity2,
              PIPixel*  oDst, int  iWidth, int  iHeight, int  iModSrc1, int  iModSrc2,  int  iModDst )
{
    INTPTR   arg[] = { (INTPTR)iSrc1, iDensity1, (INTPTR)iSrc2, iDensity2, (INTPTR)oDst, iWidth, iHeight, iModSrc1, iModSrc2, iModDst, 0,0,0,0 };

    iPlugin->CallBack( iPlugin,CB_COLOR_BLEND, arg );
}


void
TVColorBlendMode( PIFilter*  iPlugin, const PIPixel*  iSrc1, const PIPixel*  iSrc2, PIPixel*  oDst, int  iCount, int  iBlendMode )
{
    INTPTR   arg[] = { (INTPTR)iSrc1, (INTPTR)iSrc2, (INTPTR)oDst, iCount, iBlendMode, 0,0,0,0 };

    iPlugin->CallBack( iPlugin,CB_COLOR_BLEND_MODE, arg );
}


int
TVPackRLE( PIFilter*  iPlugin, const PIPixel32*  iSrc,  char*  oDst, int  iSrcCount )
{
    INTPTR   arg[] = { (INTPTR)iSrc, (INTPTR)oDst, iSrcCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_PACK_RLE, arg );
}


int
TVUnpackRLE( PIFilter*  iPlugin, const char*  iSrc,  PIPixel32*  oDst, int  iDstCount )
{
    INTPTR   arg[] = { (INTPTR)iSrc, (INTPTR)oDst, iDstCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_UNPACK_RLE, arg );
}


int
TVRGBAToYUV( PIFilter*  iPlugin, const PIPixel*  iRGBA, PIUInt8*  oYUV, PIUInt8*  oAlpha, int  iCount )
{
    INTPTR  arg[] = { PIBCMD_RGBA_TO_YUV, (INTPTR)iRGBA, (INTPTR)oYUV, (INTPTR)oAlpha, iCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_PIXEL_CMD, arg );
}


int
TVYUVToRGBA( PIFilter*  iPlugin, const PIUInt8*  iYUV, const PIUInt8*  iAlpha, PIPixel*  oRGBA, int  iCount )
{
    INTPTR  arg[] = { PIBCMD_YUV_TO_RGBA, (INTPTR)iYUV, (INTPTR)iAlpha, (INTPTR)oRGBA, iCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_PIXEL_CMD, arg );
}


int
TVRGBAUnmultiply( PIFilter*  iPlugin, const PIPixel*  iRGBA, PIPixel*  oRGBA, int  iCount )
{
    INTPTR  arg[] = { PIBCMD_RGBA_UNMULTIPLY, (INTPTR)iRGBA, (INTPTR)oRGBA, iCount, 0,0,0,0 }; 

    return  (int)iPlugin->CallBack( iPlugin, CB_PIXEL_CMD, arg );
}


int
TVRGBAPremultiply( PIFilter*  iPlugin, const PIPixel*  iRGBA, PIPixel*  oRGBA, int  iCount )
{
    INTPTR  arg[] = { PIBCMD_RGBA_PREMULTIPLY, (INTPTR)iRGBA, (INTPTR)oRGBA, iCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_PIXEL_CMD, arg );
}


int
TVPickColor( PIFilter*  iPlugin, PIPixel*  oResult )
{
    INTPTR  arg[] = { (INTPTR)oResult, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_PICK_COLOR, arg );
}


//MARK: ========================================================================================================= pi-popup.h


int
TVPopup( PIFilter*  iPlugin, PIPopup*  iPopup,  int  iPopupCount, int  iDefault )
{
    INTPTR  arg[] = { (INTPTR)iPopup, iPopupCount, iDefault, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_POPUP, arg );
}


//MARK: ========================================================================================================= pi-preview.h


void
TVPreviewLine( PIFilter*  iPlugin, double  iX1, double  iY1, double  iX2, double  iY2, PIFlags  iFlags )
{
    INTPTR  arg[] = { iX1*65536., iY1*65536., iX2*65536., iY2*65536., iFlags, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_PREVIEW_LINE, arg );
}


void
TVPreviewHandle( PIFilter*  iPlugin, double  iX, double  iY, double  iSize, PIFlags  iFlags )
{
    INTPTR  arg[] = { iX*65536., iY*65536., iSize*65536., iFlags, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_PREVIEW_HANDLE, arg );
}


void
TVPreviewText( PIFilter*  iPlugin, double  iX, double  iY, const char*  iText, PIFlags  iFlags )
{
    INTPTR  arg[] = { iX*65536., iY*65536., (INTPTR)iText, iFlags, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_PREVIEW_TEXT, arg );
}


//MARK: ========================================================================================================= pi-profile.h


PIProfile*
TVAllocProfile( PIFilter*  iPlugin, const char*  iName, ePIProfileShape  iDefault )
{
    INTPTR  arg[] = { (INTPTR)(iName), iDefault, 0,0,0,0 };

    return  (PIProfile*)iPlugin->CallBack( iPlugin, CB_ALLOC_PROFILE, arg );
}


//MARK: ========================================================================================================= pi-requester.h


INTPTR
TVOpenReq( PIFilter*  iPlugin, int  iWidth, int  iHeight, int  iX, int  iY, INTPTR  iFlags, const char*  iTitle )
{
    INTPTR   arg[] = { iWidth, iHeight, iX, iY, iFlags, (INTPTR)iTitle, 0,0,0,0 };

    return  iPlugin->CallBack( iPlugin, CB_OPENREQ, arg );
}


INTPTR
TVOpenReqEx( PIFilter*  iPlugin, int  iWidth, int  iHeight, int  iX, int  iY, INTPTR  iFlags, const char*  iTitle, tEventHandler  iEventHandler )
{
    INTPTR   arg[] = { iWidth, iHeight, iX, iY, iFlags, (INTPTR)iTitle, (INTPTR)iEventHandler, 0,0,0,0 };

    return  iPlugin->CallBack( iPlugin, CB_OPENREQEX, arg );
}


INTPTR
TVOpenFilterReqEx( PIFilter*  iPlugin, int  iWidth, int  iHeight, tEventHandler  iEventHandler, PIKeys*  iKeys, INTPTR  iReqFlags, INTPTR  iMenuFlags )
{
    INTPTR   arg[] = { iWidth, iHeight, (INTPTR)iEventHandler, (INTPTR)iKeys, iReqFlags, iMenuFlags, 0,0,0,0 };

    return  iPlugin->CallBack( iPlugin, CB_OPENFILTERREQ, arg );
}


void
TVCloseReq( PIFilter*  iPlugin, INTPTR  iReq )
{
    INTPTR   arg[] = { iReq, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_CLOSEREQ, arg );
}


int
TVSetReqTitle( PIFilter*  iPlugin, INTPTR  iReq, const char*  iTitle )
{
    INTPTR   arg[] = { PIRT_NAME, iReq, (INTPTR)iTitle, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVMoveReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY )
{
    INTPTR   arg[] = { PIRT_MOVE, iReq, iX, iY, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVResizeReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH )
{
    INTPTR   arg[] = { PIRT_RESIZE, iReq, iX, iY, iW, iH, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVInfoReq( PIFilter*  iPlugin, INTPTR  iReq, int*  oX, int*  oY, int*  oW, int*  oH )
{
    INTPTR   arg[] = { PIRT_INFO, iReq, 0,0,0,0 };

    int  r = (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
    if( r == 0 )
        return  0;

    if( oX )  *oX = (int)arg[2];
    if( oY )  *oY = (int)arg[3];
    if( oW )  *oW = (int)arg[4];
    if( oH )  *oH = (int)arg[5];

    return  r;
}


int
TVReqToFront( PIFilter*  iPlugin, INTPTR  iReq )
{
    INTPTR   arg[] = { PIRT_FRONT, iReq, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVReqToBack( PIFilter*  iPlugin, INTPTR  iReq )
{
    INTPTR   arg[] = { PIRT_BACK, iReq, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVDisplayReq( PIFilter*  iPlugin, INTPTR  iReq, int  iShow )
{
    INTPTR  arg[] = { PIRT_DISPLAY_REQ, iReq, iShow, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


void
TVSaveReqState( PIFilter*  iPlugin, INTPTR  iReq, const char*  iSection, const char*  iSubSection )
{
    INTPTR  arg[] = { PIRT_SAVE_REQ_STATE, iReq, (INTPTR)iSection, (INTPTR)iSubSection, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


void
TVLoadReqState( PIFilter*  iPlugin, INTPTR  iReq, const char*  iSection, const char*  iSubSection )
{
    INTPTR  arg[] = { PIRT_LOAD_REQ_STATE, iReq, (INTPTR)iSection, (INTPTR)iSubSection, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


//-- User Interface / Buttons


int
TVAddButtonReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, INTPTR  iFlags, const char*  iText )
{
    INTPTR  arg[] = { PIRT_ADD_BUTTON, iReq, iX, iY, iW, iH, iID, iFlags, (INTPTR)iText, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVRemoveButtonReq( PIFilter*  iPlugin, INTPTR  iReq, int  iID )
{
    INTPTR   arg[] = { PIRT_REMOVE_BUTTON, iReq, iID, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVChangeButtonReq( PIFilter*  iPlugin, INTPTR  iReq, int  iID, INTPTR  iFlags, const char*  iText )
{
    INTPTR   arg[] = { PIRT_CHANGE_BUTTON, iReq, iID, iFlags, (INTPTR)iText, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVSetButtonInfoText( PIFilter*  iPlugin, INTPTR  iReq, int  iID, const char*  iInfoText )
{
    INTPTR   arg[] = { PIRT_SET_INFO_TEXT, iReq, iID, (INTPTR)iInfoText, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVChangeButtonFlags( PIFilter*  iPlugin, INTPTR  iReq, int  iID, INTPTR  iFlags )
{
    INTPTR   arg[] = { PIRT_CHANGE_BUTTON_FLAGS, iReq, iID, iFlags, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVChangeButtonName( PIFilter*  iPlugin, INTPTR  iReq, int  iID, const char*  iText, INTPTR  iFlags )
{
    INTPTR   arg[] = { PIRT_CHANGE_BUTTON_NAME, iReq, iID, (INTPTR)iText, iFlags, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVGetButtonString( PIFilter*  iPlugin, INTPTR  iReq,  int  iID, char*  oString, int  iMaxChars )
{
    INTPTR   arg[] = { PIRT_GET_BUTTON_STRING, iReq, iID, (INTPTR)oString, iMaxChars, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVPutButtonString( PIFilter*  iPlugin, INTPTR  iReq,  int  iID, const char*  iString )
{
    INTPTR   arg[] ={ PIRT_PUT_BUTTON_STRING, iReq, iID, (INTPTR)iString, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVPutButtonStringUnit( PIFilter*  iPlugin, INTPTR  iReq,  int  iID, const char*  iUnit )
{
    INTPTR   arg[] = { PIRT_PUT_BUTTON_UNIT, iReq, iID, (INTPTR)iUnit, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


PIBlock*
TVGetButtonImage( PIFilter*  iPlugin, INTPTR  iReq, int  iID, PIFlags  iFlags )
{
    INTPTR   arg[] = { PIRT_GET_BUTTON_IMAGE, iReq, iID, iFlags, 0,0,0,0 };

    return  (PIBlock*)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVPutButtonImage( PIFilter*  iPlugin, INTPTR  iReq, int  iID, PIBlock*  iImage, PIFlags  iFlags )
{
    INTPTR   arg[] = { PIRT_PUT_BUTTON_IMAGE, iReq, iID, (INTPTR)iImage, iFlags, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVRefreshButtonImage( PIFilter*  iPlugin, INTPTR  iReq, int  iID, int  iX, int  iY, int  iW, int  iH )
{
    INTPTR   arg[] = { PIRT_REFRESH_BUTTON_IMAGE, iReq, iID, iX, iY, iW, iH, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVAddSliderReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, PIFlags  iFlags,
                int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY )
{
    INTPTR  arg[] = { PIRT_ADD_SLIDER, iReq, iX, iY, iW, iH, iID, iFlags, iMaxX, iNumX, iPosX, iMaxY, iNumY, iPosY, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVChangeSliderReq( PIFilter*  iPlugin, INTPTR  iReq, int  iID, int  iMaxX, int  iNumX, int  iPosX, int  iMaxY, int  iNumY, int  iPosY )
{
    INTPTR  arg[] = { PIRT_CHANGE_SLIDER, iReq, iID, iMaxX, iNumX, iPosX, iMaxY, iNumY, iPosY, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVAddTabs( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, int  iTabCount,  int  iCurrentTab,  const char**  iTabNames )
{
    INTPTR  arg[] = { PIRT_ADD_TABS, iReq, iX, iY, iW, iH, iID, iTabCount, iCurrentTab, (INTPTR)iTabNames, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVChangeTabs( PIFilter*  iPlugin, INTPTR  iReq, int  iID, int  iCurrentTab )
{
    INTPTR  arg[] = { PIRT_CHANGE_TABS, iReq, iID, iCurrentTab, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


//-- User Interface / Drawing


int
TVTextLength( PIFilter*  iPlugin, INTPTR  iReq, const char*  iText, int  iCount )
{
    INTPTR  arg[] = { PIRT_TEXT_LENGTH, iReq, (INTPTR)iText, iCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


//-- User Interface / Requester


void
TVWarning( PIFilter*  iPlugin, const char*  iWarningText )
{
    INTPTR  arg[] = { (INTPTR)iWarningText, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_WARNING, arg );
}


int
TVDemand( PIFilter*  iPlugin, const char*  iMessage, const char*  iYes, const char*  iNo )
{
    INTPTR  arg[] = { (INTPTR)iMessage, (INTPTR)iYes, (INTPTR)iNo, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_DEMAND, arg );
}


int
TVDemandInt( PIFilter*  iPlugin, const char*  iMessage, int*  ioValue, int  iMin, int  iMax )
{
    INTPTR  arg[] = { (INTPTR)iMessage, (INTPTR)ioValue, iMin, iMax, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_DEMANDINT, arg );
}


int
TVDemandDouble( PIFilter*  iPlugin, const char*  iMessage, double*  ioValue, double  iMin, double  iMax )
{
    INTPTR  arg[] = { (INTPTR)iMessage, (INTPTR)ioValue, (INTPTR)&iMin, (INTPTR)&iMax, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_DEMANDDOUBLE, arg );
}


int
TVDemandText( PIFilter*  iPlugin, const char*  iMessage, char*  ioText, int  iTextCount )
{
    INTPTR  arg[] = { (INTPTR)iMessage, (INTPTR)ioText, iTextCount, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_DEMANDTEXT, arg );
}


//-- User Interface / Progression


void
TVWorkOn( PIFilter*  iPlugin, const char*  iText )
{
    INTPTR  arg[] = { (INTPTR)iText, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_WORKON, arg );
}


int TVWorkTrace( PIFilter*  iPlugin, int  iCurrent, int  iMax )
{
    INTPTR  arg[] = { iCurrent, iMax, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WORKTRACE, arg );
}


void
TVWorkOff( PIFilter*  iPlugin )
{
    INTPTR  arg[] = { 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_WORKOFF, arg );
}


//-- User Interface / Gadget


int
TVCompassOpen( PIFilter*  iPlugin, INTPTR  iReq, int  iID, double  iValue )
{
    INTPTR  arg[] = { PIRT_COMPASS_OPEN, iReq, iID, (long)(iValue*65536.), 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


void
TVCompassChange( PIFilter*  iPlugin, double  iValue )
{
    INTPTR  arg[] = { PIRT_COMPASS_CHANGE, (INTPTR)(iValue*65536.), 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


void
TVCompassClose( PIFilter*  iPlugin )
{
    INTPTR  arg[] = { PIRT_COMPASS_CLOSE, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


//-- Meta/Hud


int
TVInstallFunction( PIFilter*  iPlugin, PIFlags  iFunction )
{
    INTPTR  arg[] = { iFunction, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_OPENMETA, arg );
}


int
TVCloseFunction( PIFilter*  iPlugin )
{
    INTPTR  arg[] = { 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_CLOSEMETA, arg );
}


//-- Localisation


void*
TVOpenLocalFile( PIFilter*  iPlugin, const char*  iFileName, PIFlags  iReserved )
{
    INTPTR  arg[] = { (INTPTR)iFileName, iReserved, 0,0,0,0 };

    return  (void*)iPlugin->CallBack( iPlugin, CB_LOCAL_OPEN, arg );
}


char*
TVGetLocalString( PIFilter*  iPlugin, void*  iLocaleFile, int  iLine )
{
    INTPTR  arg[] = { (INTPTR)iLocaleFile, iLine, 0,0,0,0 }; 

    return  (char *)iPlugin->CallBack( iPlugin, CB_LOCAL_GET_STRING, arg );
}


void
TVCloseLocalFile( PIFilter*  iPlugin, void*  iLocaleFile )
{
    INTPTR  arg[] = { (INTPTR)iLocaleFile, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_LOCAL_CLOSE, arg );
}


//MARK: ========================================================================================================= pi-stringfile.h


//-- String Files/Configuration file


void*
TVOpenStringFile( PIFilter*  iPlugin, const char*  iFileName, PIFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)iFileName, (INTPTR)iFlags, 0,0,0,0 };

    return  (void*)iPlugin->CallBack( iPlugin, CB_OPEN_STRING_FILE, arg );
}


int
TVCloseStringFile( PIFilter*  iPlugin, void*  iFile, PIFlags  iFlags )
{
    INTPTR  arg[] = { (INTPTR)iFile, (INTPTR)iFlags, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_CLOSE_STRING_FILE, arg );
}


int
TVReadStringFile( PIFilter*  iPlugin, void*  iFile, const char*  iSection,
                  const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar )
{
    INTPTR  arg[] = { (INTPTR)iFile, (INTPTR)iSection, (INTPTR)iName, (INTPTR)oString, (INTPTR)iDefault, iMaxChar, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READ_STRING_FILE, arg );
}


int
TVWriteStringFile( PIFilter*  iPlugin, void*  iFile, const char*  iSection, const char*  iName, const char*  iString )
{
    INTPTR  arg[] = { (INTPTR)iFile, (INTPTR)iSection, (INTPTR)iName, (INTPTR)iString, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRITE_STRING_FILE, arg );
}


int
TVEraseStringFileSection( PIFilter*  iPlugin, void*  iFile, const char*  iSection )
{
    INTPTR  arg[] = { (INTPTR)iFile, (INTPTR)iSection, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_ERASE_STRING_FILE_SECTION, arg );
}


//-- String Files/Preferences file


int
TVReadUserString( PIFilter*  iPlugin, const char*  iSection, const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar )
{
    INTPTR  arg[] = { (INTPTR)iSection, (INTPTR)iName, (INTPTR)oString, (INTPTR)iDefault, iMaxChar, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READ_USER_STRING, arg );
}


int
TVWriteUserString( PIFilter*  iPlugin, const char*  iSection, const char*  iName, const char*  iString )
{
    INTPTR  arg[] = { (INTPTR)iSection, (INTPTR)iName, (INTPTR)iString, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRITE_USER_STRING, arg );
}


int
TVEraseUserSection( PIFilter*  iPlugin, const char*  iSection )
{
    INTPTR  arg[] = { (INTPTR)iSection, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_ERASE_USER_SECTION, arg );
}


//-- String Files/Project file


int
TVReadProjectString(PIFilter*  iPlugin, const char*  iSection, const char*  iName, char*  oString, const char*  iDefault, int  iMaxChar )
{
    INTPTR  arg[] = { (INTPTR)iSection, (INTPTR)iName, (INTPTR)oString, (INTPTR)iDefault, iMaxChar, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READ_PROJECT_STRING, arg );
}


int
TVWriteProjectString( PIFilter*  iPlugin, const char*  iSection, const char*  iName, const char*  iString )
{
    INTPTR  arg[] = { (INTPTR)iSection, (INTPTR)iName, (INTPTR)iString, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRITE_PROJECT_STRING, arg );
}


int
TVEraseProjectSection( PIFilter*  iPlugin, const char*  iSection )
{
    INTPTR  arg[] = { (INTPTR)iSection, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_ERASE_PROJECT_SECTION, arg );
}


//MARK: ========================================================================================================= pi-videodevice.h


PIVideoDevice*
TVEnumVideoDevice( PIFilter*  iPlugin, int  iIndex )
{
    INTPTR  arg[] = { iIndex, 0,0,0,0 };

    return  (PIVideoDevice *)iPlugin->CallBack( iPlugin, CB_ENUM_VIDEODEVICE, arg );
}


void
TVAddVideoDevice( PIFilter*  iPlugin, PIVideoDevice*  iVideoDevice )
{
    INTPTR  arg[] = { (INTPTR)iVideoDevice, 0,0,0,0 };

    iPlugin->CallBack( iPlugin, CB_ADD_VIDEODEVICE, arg );
}


int
TVAddVideoButton( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, int  iW, int  iH, int  iID, PIVideoDevice*  iVideoDevice, int  iVideoInput, int  iVideoMode )
{
    INTPTR  arg[] = { PIRT_ADD_VIDEO_BUTTON, iReq, iX, iY, iW, iH, iID, (INTPTR)iVideoDevice, iVideoInput, iVideoMode, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


//MARK: ========================================================================================================= DEPRECATED


INTPTR
TVOpenFilterReq( PIFilter*  iPlugin, int  iWidth, int  iHeight, tEventHandler  iEventHandler, PIKeys*  iKeys, INTPTR  iReqFlags )
{
    INTPTR  arg[] = { iWidth, iHeight, (INTPTR)iEventHandler, (INTPTR)iKeys, iReqFlags, 0,0,0,0 };

    return  iPlugin->CallBack( iPlugin, CB_OPENFILTERREQ, arg );
}


int
TVTextReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int iY, const char*  iText, INTPTR  iMode, PIPixel32  iAPen, PIPixel32  iBPen )
{
    INTPTR  arg[] = { PIRT_TEXT, iReq, (INTPTR)iText, iX, iY, iMode, iAPen, iBPen, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


int
TVTextReqClip( PIFilter*  iPlugin, INTPTR  iReq, int  iX, int  iY, const char*  iText, INTPTR  iMode, PIPixel32  iAPen, PIPixel32  iBPen,
               int  iClipX1, int  iClipY1, int  iClipX2, int  iClipY2 )
{
    INTPTR  arg[] = { PIRT_TEXT_CLIP, iReq, (INTPTR)iText, iX, iY, iMode, iAPen, iBPen, iClipX1, iClipY1, iClipX2, iClipY2, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_SENDREQCMD, arg );
}


void
TVAADrawReq( PIFilter*  iPlugin, INTPTR  iReq, double  iX1,double  iY1, double  iX2, double  iY2,
             int  iR1, int  iR2, INTPTR  iMode, PIPixel32  iAPen,
             int  iClipX1, int  iClipY1, int  iClipX2, int  iClipY2)
{
    INTPTR  arg[] = { PIRT_AADRAW, iReq, iX1*65536., iY1*65536., iX2*65536., iY2*65536.,
                      iR1, iR2, iMode, iAPen, iClipX1, iClipY1, iClipX2, iClipY2, 0,0,0,0 };

    iPlugin->CallBack( iPlugin,CB_SENDREQCMD, arg );
}


int
TVRectangleReq( PIFilter*  iPlugin, INTPTR  iReq, int  iX1, int  iY1, int  iX2, int  iY2, INTPTR  iMode, PIPixel32  iAPen )
{
    INTPTR  arg[] = { PIRT_RECT, iReq, iX1, iY1, iX2, iY2, iMode, iAPen, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin,CB_SENDREQCMD, arg );
}


int
TVReadReqData( PIFilter*  iPlugin, PIPixel*  oDst, int  iX, int  iY, int  iW, int  iH, int  iMod, INTPTR  iReq )
{
    INTPTR  arg[] = { (INTPTR)oDst, iX, iY, iW, iH, iMod, iReq, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_READREQDATA, arg );
}


int
TVWriteReqData( PIFilter*  iPlugin, const PIPixel*  iSrc, int  iX, int  iY, int  iW, int  iH, int  iMod, INTPTR  iReq )
{
    INTPTR  arg[] = { (INTPTR)iSrc, iX, iY, iW, iH, iMod, iReq, 0,0,0,0 };

    return  (int)iPlugin->CallBack( iPlugin, CB_WRITEREQDATA, arg );
}

