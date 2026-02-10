/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-private.h
*  @ingroup  sdk_TVPA
*
*  @brief  The implementation of the plugin-side of the SDK.
*/

#ifndef __pi_private_HH
#define __pi_private_HH


//MARK: ========================================================================================================= enums


/** @brief  PRIVATE: calls back to TVPaint from the plugin. */
enum  eTVCallbacks
{
    CB_RESERVED =  128,

    CB_RESERVED1                  = (CB_RESERVED+1),
    CB_EXECUTE                    = (CB_RESERVED+2),
    CB_WORKON                     = (CB_RESERVED+3),
    CB_WORKTRACE                  = (CB_RESERVED+4),
    CB_WORKOFF                    = (CB_RESERVED+5),
    CB_WARNING                    = (CB_RESERVED+6),
    CB_DEMAND                     = (CB_RESERVED+7),
    CB_DEMANDINT                  = (CB_RESERVED+8),
    CB_DEMANDTEXT                 = (CB_RESERVED+9),
    CB_UPDATEDISPLAY              = (CB_RESERVED+10),
    CB_SENDCMD                    = (CB_RESERVED+11),
    CB_READLAYERDATA              = (CB_RESERVED+12),
    CB_WRITELAYERDATA             = (CB_RESERVED+13),
    CB_MAKEBRUSH                  = (CB_RESERVED+14),
    CB_PREVIEW_LINE               = (CB_RESERVED+15),
    CB_PREVIEW_HANDLE             = (CB_RESERVED+16),
    CB_DEMANDDOUBLE               = (CB_RESERVED+17),
    CB_PREVIEW_TEXT               = (CB_RESERVED+18),

    CB_OPENREQ                    = (CB_RESERVED+100),
    CB_CLOSEREQ                   = (CB_RESERVED+101),
    CB_SENDREQCMD                 = (CB_RESERVED+102),
    CB_READREQDATA                = (CB_RESERVED+103),
    CB_WRITEREQDATA               = (CB_RESERVED+104),
    CB_OPENREQEX                  = (CB_RESERVED+105),
    CB_OPENFILTERREQ              = (CB_RESERVED+106),

    CB_OPENMETA                   = (CB_RESERVED+200),
    CB_CLOSEMETA                  = (CB_RESERVED+201),

    CB_PACK_RLE                   = (CB_RESERVED+300),
    CB_UNPACK_RLE                 = (CB_RESERVED+301),

    CB_POPUP                      = (CB_RESERVED+400),

    CB_COLOR_BLEND                = (CB_RESERVED+500),
    CB_COLOR_BLEND_MODE           = (CB_RESERVED+501),

    CB_WRITE_USER_STRING          = (CB_RESERVED+600),
    CB_READ_USER_STRING           = (CB_RESERVED+601),
    CB_ERASE_USER_SECTION         = (CB_RESERVED+602),

    CB_WRITE_PROJECT_STRING       = (CB_RESERVED+605),
    CB_READ_PROJECT_STRING        = (CB_RESERVED+606),
    CB_ERASE_PROJECT_SECTION      = (CB_RESERVED+607),

    CB_OPEN_STRING_FILE           = (CB_RESERVED+610),
    CB_CLOSE_STRING_FILE          = (CB_RESERVED+611),

    CB_READ_STRING_FILE           = (CB_RESERVED+612),
    CB_WRITE_STRING_FILE          = (CB_RESERVED+613),
    CB_ERASE_STRING_FILE_SECTION  = (CB_RESERVED+614),

    CB_MAKE_IMAGE_DISPLAY         = (CB_RESERVED+650),

    CB_READ_PROJECT_IMAGE         = (CB_RESERVED+651),

    CB_READ_IMAGE                 = (CB_RESERVED+700),
    CB_ALLOC_PIBLOCK              = (CB_RESERVED+710),
    CB_FREE_PIBLOCK               = (CB_RESERVED+711),

    CB_ALLOC_FILEREQ              = (CB_RESERVED+800),
    CB_FREE_FILEREQ               = (CB_RESERVED+801),
    CB_OPEN_FILEREQ               = (CB_RESERVED+802),

    CB_CREATE_ALIAS               = (CB_RESERVED+820),

    CB_WRAP_BLOCK                 = (CB_RESERVED+830),

    CB_MULTITHREAD                = (CB_RESERVED+840),

    CB_ADD_DISPLAYDEVICE          = (CB_RESERVED+1000),
    CB_ENUM_DISPLAYDEVICE         = (CB_RESERVED+1001),
    CB_LOCK_DISPLAYDEVICE         = (CB_RESERVED+1002),
    CB_UNLOCK_DISPLAYDEVICE       = (CB_RESERVED+1003),

    CB_LOCAL_OPEN                 = (CB_RESERVED+1100),
    CB_LOCAL_CLOSE                = (CB_RESERVED+1101),
    CB_LOCAL_GET_STRING           = (CB_RESERVED+1102),
    CB_LOCAL_GET_ICON             = (CB_RESERVED+1103),

    CB_TEXT_TO_BLOCK              = (CB_RESERVED+1200),

    CB_ALLOC_KEYS                 = (CB_RESERVED+1300),

    CB_ADD_VIDEODEVICE            = (CB_RESERVED+1400),
    CB_ENUM_VIDEODEVICE           = (CB_RESERVED+1401),

    CB_BLOCK_CMD                  = (CB_RESERVED+1500),

    CB_OPEN_FONT                  = (CB_RESERVED+1510),

    CB_PIXEL_CMD                  = (CB_RESERVED+1520),

    CB_OPEN_IMAGESEQUENCE         = (CB_RESERVED+1540),

    CB_ALLOC_PROFILE              = (CB_RESERVED+1550),

    CB_HUD_CMD                    = (CB_RESERVED+1560),

    CB_PICK_COLOR                 = (CB_RESERVED+1570),

    CB_GET_IMAGE_ID               = (CB_RESERVED+1580),
};



/** @brief  PRIVATE: calls back to TVPaint from the plugin for the CB_HUD_CMD. */
enum  eTVHudCommand
{
    PIHUDCMD_REDRAW = 0,
    PIHUDCMD_LINE   = 1,
    PIHUDCMD_SPLINE = 2,
    PIHUDCMD_TEXT   = 3,
    PIHUDCMD_HANDLE = 4,
    PIHUDCMD_IMAGE  = 5,
};


/** @brief  PRIVATE: calls back to TVPaint from the plugin for the CB_BLOCK_CMD. */
enum  eTVBlockCommand
{
    PIBCMD_STRETCH = 1,       // 1, src,dst
    PIBCMD_BLUR    = 2,       // 2, blck, w., h.
};



/** @brief  PRIVATE: calls back to TVPaint from the plugin for the CB_PIXEL_CMD. */
enum  eTVPixelCommand
{
    PIBCMD_RGBA_TO_YUV      = 1,       // iRGBA,oYUV,(oAlpha),width
    PIBCMD_YUV_TO_RGBA      = 2,       // iYUV,iALPHA,oRGBA, w,
    PIBCMD_RGBA_UNMULTIPLY  = 3,       // iRGBA, oRGBA, w
    PIBCMD_RGBA_PREMULTIPLY = 4,       // iRGBA, oRGBA, w
};


/** @brief  PRIVATE: calls back to TVPaint from the plugin for the CB_SENDREQCMD. */
enum  eTVRequesterCommand
{
    PIRT_INFO                 =  1,   /* * (32 tags) id  -> tag 2 3 4 5 -> x,y,w,h*/
    PIRT_NAME                 =  2,   /* * id, name*/
    PIRT_FRONT                =  3,   /* * id   ->to front*/
    PIRT_BACK                 =  4,   /* * id   ->to back*/
    PIRT_MOVE                 =  5,   /* * id,x,y,   ->to back*/
    PIRT_RESIZE               =  6,   /* * req id x y w h */
    PIRT_ADD_BUTTON           = 10,   /* * id,x,y,w,h,type*/
    PIRT_REMOVE_BUTTON        = 11,   /* * id*/
    PIRT_CHANGE_BUTTON        = 12,   /* * id,flag */
    PIRT_SET_INFO_TEXT        = 13,   /* * id, *text */
    PIRT_TEXT                 = 20,   /* * text,x,y,gmode,apen,bpen*/
    PIRT_RECT                 = 21,   /* * x2,y2,x2,y2,gmode,color*/
    PIRT_AADRAW               = 22,   /* * x2,y2,x2,y2,r1,r2,gmode,color,cx1,cy1,cx2,cy2*/
    PIRT_GET_BUTTON_STRING    = 24,   /* * req,id,string,maxchar*/
    PIRT_PUT_BUTTON_STRING    = 25,   /* * req,id,string*/
    PIRT_GRAB_COORD           = 26,   /* * req, on/off */
    PIRT_TEXT_CLIP            = 27,   /* * text,x,y,gmode,apen,bpen,clipx1,clipy1,clipx2,clipy2*/
    PIRT_GRAB_TICKS           = 28,   /* * req, on/off */
    PIRT_TEXT_LENGTH          = 29,   /* * text,size*/
    PIRT_PUT_BUTTON_UNIT      = 30,   /* * id, *text */
    PIRT_PUT_BUTTON_IMAGE     = 31,   /* * req, id, PIBlock, flags */
    PIRT_ADD_SLIDER           = 32,   /* * req x y w h id flags maxx numx posx maxy numy posy*/
    PIRT_CHANGE_SLIDER        = 33,   /* * req id maxx numx posx maxy numy posy */
    PIRT_CHANGE_BUTTON_NAME   = 34,   /* * id, name*/
    PIRT_CHANGE_BUTTON_FLAGS  = 35,   /* * id, flags*/
    PIRT_ADD_TABS             = 36,   /* * req x y w h id num current names */
    PIRT_CHANGE_TABS          = 37,   /* * req id pos */
    PIRT_GET_BUTTON_IMAGE     = 38,   /* * req, id */
    PIRT_REFRESH_BUTTON_IMAGE = 39,   /* * req, id, int x, int y, int w, int h */
    PIRT_COMPASS_OPEN         = 40,   /* * req, id, double val */
    PIRT_COMPASS_CHANGE       = 41,   /* * double val */
    PIRT_COMPASS_CLOSE        = 42,   /* * void */
    PIRT_ADD_VIDEO_BUTTON     = 43,   /* * iReq, iX, iY, iW, iH, iID, iVDev, iVMode  */
    PIRT_SAVE_REQ_STATE       = 44,   /* * iReq, iSection, iSubSection  */
    PIRT_LOAD_REQ_STATE       = 45,   /* * iReq, iSection, iSubSection  */
    PIRT_DISPLAY_REQ          = 46,   /* * iReq, Show/Hide  */
};


#endif // __pi_private_HH
