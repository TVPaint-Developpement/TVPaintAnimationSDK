/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-popup.h
*  @ingroup  sdk_TVPA
*
*  @brief  Popup menus.
*/

#ifndef __pi_popup_HH
#define __pi_popup_HH


#include "pi-basics.h"
#include "pi-pixel.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIPopup  Stuff related to popup menus.

    A popup menu.
*/


//===============================================================================================================


/** @brief  The different options for a popup entry.

    @ingroup  sdk_PIPopup
*/
typedef  enum  ePIPopMode
{
    /** Entry is a separator bar (nonselectable). */
    bPIPopMode_Separator = (1<<0),
    /** Entry uses the color specified in the APen field, else uses the Host's default color. */
    bPIPopMode_UseAPen   = (1<<1),
    /** Entry is quiet (non selectable). */
    bPIPopMode_Quiet     = (1<<2),
    /** Entry is checked. */
    bPIPopMode_Check     = (1<<3),
    /** Entry is grayed out (but still selectable if it's ID is \> 0 !) */
    bPIPopMode_Ghost     = (1<<4),
    /** Entry for a multi level popup. Uses "/" for adding a popup level. */
    bPIPopMode_Multi     = (1<<5)
} ePIPopMode;


//===============================================================================================================


/** @brief  One entry of a popup menu.

    An array of these in passed to the TVPopup() function.

    @ingroup  sdk_PIPopup
*/
typedef  struct  PIPopup
{
    /** @brief  Name of the popup menu entry. */
    const char*  Name;
    /** @brief  Identifier of the popup menu entry.\n ID must be \> 0 (for a non-selectable entry, use the bPIPopMode_Quiet flag). */
    PIInt32      ID;
    /** @brief  A set of ePIPopMode, OR'ed together. */
    PIFlags      Flags;
    /** @brief  Color of the popup entry's text (use TVMakePIPixel32() to make the color). */
    PIPixel32    APen;
    /** @brief  Reserved, don't modify, set to 0. */
    PIUInt64     Reserved[9];
} PIPopup;


//===============================================================================================================


/** @brief  Opens a popup menu at the current mouse/tablet position.

    @param[in]  iPlugin      This plugin's reference.
    @param[in]  iPopup       Array of popup entries.
    @param[in]  iPopupCount  Number of entries of iPopup.
    @param[in]  iDefault     Index of the entry which should be selected when the popup opens.

    @return  ID of the selected item (>= 0), or -1 if it fails or the user did not select anything.

    @see  PIPopup

    @ingroup  sdk_PIPopup
*/
int  TVPopup( PIPlugin*  iPlugin, PIPopup*  iPopup,  int  iPopupCount, int  iDefault );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIPopMode instead. */
enum  TVDEPRECATED( "Use ePIPopMode instead" ) PIPOPMODE_Flags_DEPRECATED
{
    /** @deprecated  Use bPIPopMode_Separator instead. */
    PIPOPMODE_SEPARATOR   TVDEPRECATED( "Use bPIPopMode_Separator instead" ) = bPIPopMode_Separator,
    /** @deprecated  Use bPIPopMode_UseAPen instead. */
    PIPOPMODE_USE_APEN    TVDEPRECATED( "Use bPIPopMode_UseAPen instead"   ) = bPIPopMode_UseAPen,
    /** @deprecated  Use bPIPopMode_Quiet instead. */
    PIPOPMODE_QUIET       TVDEPRECATED( "Use bPIPopMode_Quiet instead"     ) = bPIPopMode_Quiet,
    /** @deprecated  Use bPIPopMode_Check instead. */
    PIPOPMODE_CHECK       TVDEPRECATED( "Use bPIPopMode_Check instead"     ) = bPIPopMode_Check,
    /** @deprecated  Use bPIPopMode_Ghost instead. */
    PIPOPMODE_GHOST       TVDEPRECATED( "Use bPIPopMode_Ghost instead"     ) = bPIPopMode_Ghost,
    /** @deprecated  Use bPIPopMode_Multi instead. */
    PIPOPMODE_MULTI       TVDEPRECATED( "Use bPIPopMode_Multi instead"     ) = bPIPopMode_Multi
};


#ifdef __cplusplus
}
#endif


#endif // __pi_popup_HH
