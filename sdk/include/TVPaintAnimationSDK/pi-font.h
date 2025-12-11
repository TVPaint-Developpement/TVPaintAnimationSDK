/** @copyright (c) 2002-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-font.h
*  @ingroup  sdk_TVPA
*
*  @brief  A vectorial font
*/

#ifndef __pi_font_HH
#define __pi_font_HH

#include "TVPaintAnimationSDK/pi-basics.h"

#include <wchar.h>


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIFont  A vectorial font.
*/


//===============================================================================================================


/** @brief  A vectorial font.

    @ingroup  sdk_PIFont
*/
typedef  struct  PIFont
{
    /** @brief  The version of this structure.
        It MUST be equal to sizeof(PIFont).
     */
    int  Version;

    /** @brief  Closes the font.
        @param[in]  iFont  The font to close.
     */
    void  (*Close)( struct PIFont*  iFont );

    /** @brief  Sets the matrix.

        @param[in]  iFont  The font.
        @param[in]  iIX  ???.
        @param[in]  iIY  ???.
        @param[in]  iJX  ???.
        @param[in]  iJY  ???.
     */
    void  (*SetMatrix)( struct PIFont*  iFont, double  iIX, double  iIY, double  iJX, double  iJY );

    /** @brief  Applies the matrix to a point.

        @param[in]      iFont   The font.
        @param[in]      iScale  The size of the font.
        @param[in,out]  ioX     The X coordinate of the point.
        @param[in,out]  ioY     The Y coordinate of the point.
     */
    void  (*CalcMatrix)( struct PIFont*  iFont, double  iScale, double*  ioX, double*  ioY );

    /** @brief  Decomposes a character into it's constituting strokes and calls functions with them.
        @param[in]   iFont       The font.
        @param[in]   iUserParam  This will just be passed along to the functions, it has no meaning to TVPaint.
        @param[in]   iBegin      The function to call at the begining of a stroke.
        @param[in]   iLine       The function to call to make the stroke longer.
        @param[in]   iEnd        The function to call at the end of a stroke.
        @param[in]   iCode       The unicode character code.
        @param[in]   iScale      The size of the font.
        @param[out]  oDX         How much to advance the "caret" to draw the next character.
        @param[out]  oDY         How much to advance the "caret" to draw the next character.
     */
    void  (*Draw)( struct PIFont*  iFont, void*  iUserParam,
                                          void  (*iBegin)( void*  iUserParam, double  iX, double  iY ),
                                          void  (*iLine) ( void*  iUserParam, double  iX, double  iY ),
                                          void  (*iEnd)  ( void*  iUserParam ),
                                          wchar_t  iCode, double  iScale, double*  oDX, double*  oDY );

    /** @brief  XXX.
        @param[in]   iFont   The font.
        @param[in]   iCode   The unicode character code.
        @param[in]   iScale  The size of the font.
        @param[out]  oJX     ???.
        @param[out]  oJY     ???.
        @param[out]  oDX     How much to advance the "caret" to draw the next character.
        @param[out]  oDY     How much to advance the "caret" to draw the next character.
        @param[out]  oW      The width of the character.
        @param[out]  oH      The height of the character.
     */
    void  (*Size)( struct PIFont*  iFont, wchar_t  iCode, double  iScale, double*  oJX, double*  oJY, double*  oDX, double*  oDY, double*  oW, double*  oH );

    /** @brief  Computes the kerning between two characters of the font.
        @param[in]   iFont   The font.
        @param[in]   iCode1  The unicode code of the first character.
        @param[in]   iCode2  The unicode code of the second character.
        @param[in]   iScale  The size of the font.
        @param[out]  oKernX  The kerning.
        @param[out]  oKernY  The kerning.
     */
    void  (*Kerning)( struct PIFont*  iFont, wchar_t  iCode1, wchar_t  iCode2, double  iScale, double*  oKernX, double*  oKernY );

    /** @brief  Returns the ascent of the font (the point of all characters the furthest away above the baseline).
        @param[in]   iFont     The font.
        @param[out]  oAscentX  The X coordinate of the ascent.
        @param[out]  oAscentY  The Y coordinate of the ascent.
     */
    void  (*Ascent)( struct PIFont*  iFont, double  iScale, double*  oAscentX, double*  oAscentY );

    /** @brief  Returns the descent of the font (the point of all characters the furthest away below the baseline)..
        @param[in]   iFont      The font.
        @param[out]  oDescentX  The X coordinate of the descent.
        @param[out]  oDescentY  The Y coordinate of the descent.
     */
    void  (*Descent)( struct PIFont*  iFont, double  iScale, double*  oDescentX, double*  oDescentY );
} PIFont;


/** @brief  Opens a font.
    You HAVE to close it when you're finished using it, with `yourfont->Close( font );`

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iName    The name of the font.
    @param[in]  iScript  The script, in an OS dependent way...
    @param[in]  iStyle   The style, in an OS dependent way...

    @return  The font, or NULL in case of error.

    @ingroup  sdk_PIFont
*/
PIFont*  TVOpenPIFont( PIPlugin*  iPlugin, const char*  iName, unsigned long  iScript, unsigned long  iStyle );


#ifdef __cplusplus
};
#endif


#endif // __pi_font_HH
