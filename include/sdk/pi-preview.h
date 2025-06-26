/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-preview.h
*  @ingroup  sdk_TVPA
*
*  @brief  Previewing some shapes...
*/

#ifndef __pi_preview_HH
#define __pi_preview_HH


#include "pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIPreview  Stuff related to previewing on top of the project view.
*/

//===============================================================================================================


/** @brief  Different flags for the TVPreview*() functions.

    @ingroup  sdk_PIPreview
 */
typedef  enum  ePIPreviewFlags
{
    bPIPreviewFlags_Restore      = 0,
    bPIPreviewFlags_Invert       = (1<<0),
    bPIPreviewFlags_NoFirst      = (1<<1),
    bPIPreviewFlags_Check        = (1<<2),
    bPIPreviewFlags_Cross        = (1<<3),
    bPIPreviewFlags_Selection    = (1<<4),

    bPIPreviewFlags_TextBaseLine = 0,
    bPIPreviewFlags_TextTop      = (1<<16),
    bPIPreviewFlags_TextBottom   = (1<<17),
    bPIPreviewFlags_TextLeft     = (1<<18),
    bPIPreviewFlags_TextRight    = (1<<19),
} ePIPreviewFlags;


//===============================================================================================================


/** @brief  Draws a preview of a line on top of the project view.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX1      X coordinate of the first point.
    @param[in]  iY1      Y coordinate of the first point.
    @param[in]  iX2      X coordinate of the second point.
    @param[in]  iY2      Y coordinate of the second point.
    @param[in]  iFlags   Flags to change the way the line is drawn.

    @ingroup  sdk_PIPreview
*/
void  TVPreviewLine( PIPlugin*  iPlugin, double  iX1, double  iY1, double  iX2, double  iY2, PIFlags  iFlags );


/** @brief  Draws a preview of a handle on top of the project view.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX       X coordinate of the center of the handle.
    @param[in]  iY       Y coordinate of the center of the handle.
    @param[in]  iSize    Size of the handle.
    @param[in]  iFlags   Flags to change the way the handle is drawn.

    @ingroup  sdk_PIPreview
*/
void  TVPreviewHandle( PIPlugin*  iPlugin, double  iX, double  iY, double  iSize, PIFlags  iFlags );


/** @brief  Draws a preview of text on top of the project view.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX       X coordinate of the reference point of the text (@see ePIPreviewFlags).
    @param[in]  iY       Y coordinate of the reference point of the text (@see ePIPreviewFlags).
    @param[in]  iText    The text string to preview.
    @param[in]  iFlags   Flags to change the way the text is drawn.

    @ingroup  sdk_PIPreview
*/
void  TVPreviewText( PIPlugin*  iPlugin, double  iX, double  iY, const char*  iText, PIFlags  iFlags );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIPreviewFlags instead. */
enum  TVDEPRECATED( "Use ePIPreviewFlags instead" )  PIPREVIEW_DEPRECATED
{
    PREVLINE_RESTORE        TVDEPRECATED( "Use bPIPreviewFlags_Restore instead"      ) = bPIPreviewFlags_Restore,
    PREVLINE_INVERT         TVDEPRECATED( "Use bPIPreviewFlags_Invert instead"       ) = bPIPreviewFlags_Invert,
    PREVLINE_NO_FIRST       TVDEPRECATED( "Use bPIPreviewFlags_NoFirst instead"      ) = bPIPreviewFlags_NoFirst,
    PREVLINE_CHECK          TVDEPRECATED( "Use bPIPreviewFlags_Check instead"        ) = bPIPreviewFlags_Check,
    PREVLINE_CROSS          TVDEPRECATED( "Use bPIPreviewFlags_Cross instead"        ) = bPIPreviewFlags_Cross,
    PREVLINE_SELECTION      TVDEPRECATED( "Use bPIPreviewFlags_Selection instead"    ) = bPIPreviewFlags_Selection,
    PREVTEXT_RESTORE        TVDEPRECATED( "Use bPIPreviewFlags_Restore instead"      ) = bPIPreviewFlags_Restore,
    PREVTEXT_INVERT         TVDEPRECATED( "Use bPIPreviewFlags_Invert instead"       ) = bPIPreviewFlags_Invert,
    PREVTEXT_HANDLE_BS      TVDEPRECATED( "Use bPIPreviewFlags_TextBaseLine instead" ) = bPIPreviewFlags_TextBaseLine,
    PREVTEXT_HANDLE_TOP     TVDEPRECATED( "Use bPIPreviewFlags_TextTop instead"      ) = bPIPreviewFlags_TextTop,
    PREVTEXT_HANDLE_BOTTOM  TVDEPRECATED( "Use bPIPreviewFlags_TextBottom instead"   ) = bPIPreviewFlags_TextBottom,
    PREVTEXT_HANDLE_LEFT    TVDEPRECATED( "Use bPIPreviewFlags_TextLeft instead"     ) = bPIPreviewFlags_TextLeft,
    PREVTEXT_HANDLE_RIGHT   TVDEPRECATED( "Use bPIPreviewFlags_TextRight instead"    ) = bPIPreviewFlags_TextRight,
};


#ifdef __cplusplus
}
#endif


#endif // __pi_preview_HH

