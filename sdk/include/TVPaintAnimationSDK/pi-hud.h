/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-hud.h
*  @ingroup  sdk_TVPA
*
*  @brief  Drawing in the HUD.
*/

#ifndef __pi_hud_HH
#define __pi_hud_HH


#include "TVPaintAnimationSDK/pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIHud  Drawing in the HUD.
*/


//===============================================================================================================


/** @brief  The look of a handle drawn in the hud.

    @ingroup  sdk_PIHud
*/
typedef  enum  ePIHudHandle
{
    /** @brief  A square. */
    kPIHudHandle_Square   = 0,
    /** @brief  A cross (X). */
    kPIHudHandle_Cross    = 1,
    /** @brief  A plus (+). */
    kPIHudHandle_Plus     = 2,
    /** @brief  A Circle (O). */
    kPIHudHandle_Circle   = 3,
    /** @brief  An up arrow. */
    kPIHudHandle_Up       = 4,
    /** @brief  A down arrow. */
    kPIHudHandle_Down     = 5,
    /** @brief  A left arrow. */
    kPIHudHandle_Left     = 6,
    /** @brief  A right arrow. */
    kPIHudHandle_Right    = 7,
    /** @brief  A full screen reticule. */
    kPIHudHandle_Reticule = 8
} ePIHudHandle;


/** @brief  The alignment of the elements in the hud.

    @ingroup  sdk_PIHud
*/
typedef  enum  ePIHudAlign
{
    /** @brief  Centered. */
    bPIHudAlign_Center = 0,
    /** @brief  Top aligned. */
    bPIHudAlign_Top    = (1<<0),
    /** @brief  Bottom aligned. */
    bPIHudAlign_Bottom = (1<<1),
    /** @brief  Left aligned. */
    bPIHudAlign_Left   = (1<<2),
    /** @brief  Right aligned. */
    bPIHudAlign_Right  = (1<<3),
} ePIHudAlign;


//===============================================================================================================


/** @brief  Redraws the HUD.

    @param[in]  iPlugin  This plugin's reference.

    @ingroup  sdk_PIHud
*/
void  TVHudRedraw( PIPlugin*  iPlugin );


/** @brief  Draws a line in the HUD.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX1      The X coordinate of the first point.
    @param[in]  iY1      The Y coordinate of the first point.
    @param[in]  iX2      The X coordinate of the second point.
    @param[in]  iY2      The Y coordinate of the second point.
    @param[in]  iColor   The color of the line.

    @ingroup  sdk_PIHud
*/
void  TVHudLine( PIPlugin*  iPlugin, float  iX1, float  iY1, float  iX2, float  iY2, PIPixel  iColor );


/** @brief  Draws a spline in the HUD.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX1      The X coordinate of the first point.
    @param[in]  iY1      The Y coordinate of the first point.
    @param[in]  iX12     The X coordinate of the control point.
    @param[in]  iY12     The Y coordinate of the control point.
    @param[in]  iX2      The X coordinate of the second point.
    @param[in]  iY2      The Y coordinate of the second point.
    @param[in]  iColor   The color of the line.

    @ingroup  sdk_PIHud
*/
void  TVHudSpline( PIPlugin*  iPlugin,  float  iX1, float  iY1, float  iX12, float  iY12, float  iX2, float  iY2, PIPixel  iColor );


/** @brief  Draws text in the HUD.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iX         The X coordinate of the text.
    @param[in]  iY         The Y coordinate of the text.
    @param[in]  iText      The text to draw.
    @param[in]  iAlign     How the text should be aligned relative to the iX/iY point.
    @param[in]  iColor     The color of the text.
    @param[in]  iSelected  Should the text be displayed in some "selected" look.

    @ingroup  sdk_PIHud
*/
void  TVHudText( PIPlugin*  iPlugin,  float  iX, float  iY, const char*  iText, PIFlags/*ePIHudAlign*/  iAlign, PIPixel  iColor, int  iSelected );


/** @brief  Draws a handle in the HUD.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iX         The X coordinate of the handle.
    @param[in]  iY         The Y coordinate of the handle.
    @param[in]  iHandle    The shape of the handle.
    @param[in]  iColor     The color of the handle.
    @param[in]  iSelected  Should the handle be displayed in some "selected" look.

    @ingroup  sdk_PIHud
*/
void  TVHudHandle( PIPlugin*  iPlugin,  float  iX, float  iY, ePIHudHandle  iHandle, PIPixel  iColor, int  iSelected );


/** @brief  Draws an image in the HUD.

    @note  I know, this is not very explicit...

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX       The X coordinate of some point...
    @param[in]  iY       The Y coordinate of some point...
    @param[in]  iX1      The X coordinate of one corner.
    @param[in]  iY1      The Y coordinate of one corner.
    @param[in]  iX2      The X coordinate of one corner.
    @param[in]  iY2      The Y coordinate of one corner.
    @param[in]  iX3      The X coordinate of one corner.
    @param[in]  iY3      The Y coordinate of one corner.
    @param[in]  iX4      The X coordinate of one corner.
    @param[in]  iY4      The Y coordinate of one corner.
    @param[in]  iBlock   The image to draw.

    @ingroup  sdk_PIHud
*/
void    TVHudImage( PIPlugin*  iPlugin,  float  iX, float  iY, float  iX1, float  iY1, float  iX2, float  iY2, float  iX3, float  iY3, float  iX4, float  iY4, const PIBlock*  iBlock );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIHudHandle instead. */
enum  TVDEPRECATED( "Use ePIHudHandle instead" )  HUD_HANDLE_DEPRECATED
{
    HUD_HANDLE_SQUARE    TVDEPRECATED( "Use kPIHudHandle_Square instead"   ) = kPIHudHandle_Square,
    HUD_HANDLE_CROSS     TVDEPRECATED( "Use kPIHudHandle_Cross instead"    ) = kPIHudHandle_Cross,
    HUD_HANDLE_PLUS      TVDEPRECATED( "Use kPIHudHandle_Plus instead"     ) = kPIHudHandle_Plus,
    HUD_HANDLE_CIRCLE    TVDEPRECATED( "Use kPIHudHandle_Circle instead"   ) = kPIHudHandle_Circle,
    HUD_HANDLE_UP        TVDEPRECATED( "Use kPIHudHandle_Up instead"       ) = kPIHudHandle_Up,
    HUD_HANDLE_DOWN      TVDEPRECATED( "Use kPIHudHandle_Down instead"     ) = kPIHudHandle_Down,
    HUD_HANDLE_LEFT      TVDEPRECATED( "Use kPIHudHandle_Left instead"     ) = kPIHudHandle_Left,
    HUD_HANDLE_RIGHT     TVDEPRECATED( "Use kPIHudHandle_Right instead"    ) = kPIHudHandle_Right,
    HUD_HANDLE_RETICULE  TVDEPRECATED( "Use kPIHudHandle_Reticule instead" ) = kPIHudHandle_Reticule,
};


/** @deprecated  Use ePIHudAlign instead. */
enum  TVDEPRECATED( "Use ePIHudAlign instead" )  HUD_TEXT_DEPRECATED
{
    HUD_TEXT_CENTER  TVDEPRECATED( "Use bPIHudAlign_Center instead" ) = bPIHudAlign_Center,
    HUD_TEXT_TOP     TVDEPRECATED( "Use bPIHudAlign_Top instead"    ) = bPIHudAlign_Top,
    HUD_TEXT_BOTTOM  TVDEPRECATED( "Use bPIHudAlign_Bottom instead" ) = bPIHudAlign_Bottom,
    HUD_TEXT_LEFT    TVDEPRECATED( "Use bPIHudAlign_Left instead"   ) = bPIHudAlign_Left,
    HUD_TEXT_RIGHT   TVDEPRECATED( "Use bPIHudAlign_Right instead"  ) = bPIHudAlign_Right,
};


#ifdef __cplusplus
}
#endif


#endif // __pi_hud_HH

