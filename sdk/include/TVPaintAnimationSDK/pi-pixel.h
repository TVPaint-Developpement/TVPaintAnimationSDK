/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-pixel.h
*  @ingroup  sdk_TVPA
*
*  @brief  The 32bit RGBA pixel.
*/

#ifndef __pi_pixel_HH
#define __pi_pixel_HH


#include "TVPaintAnimationSDK/pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIPixel  Stuff related to pixels.

    A pixel is the smallest part of an image...
*/


//===============================================================================================================


/** @brief  A compact representation of a 32bit RGBA pixel.
    In the long run this will be replaced by PIPixel everywhere...

    @ingroup  sdk_PIPixel
*/
typedef  PIUInt32  PIPixel32;
#ifdef __cplusplus
static_assert( sizeof(PIPixel32) == 4, "Bad size" );
#endif


/** @brief  Makes a 32 bit premultiplied pixel in the correct RGBA order (PIPixel32).

    @param[in]  iR  The red component of the pixel (premultiplied, must be <= iA).
    @param[in]  iG  The green component of the pixel (premultiplied, must be <= iA).
    @param[in]  iB  The blue component of the pixel (premultiplied, must be <= iA).
    @param[in]  iA  The alpha component of the pixel.

    @return  The 32 bit pixel (PIPixel32).

    @see  ColorControl   //TODO: name !  //DOXYGEN: \@see doesn't work for \@page ?!?!?!

    @ingroup  sdk_PIPixel
*/
PIPixel32  TVMakePIPixel32( int  iR, int  iG, int  iB, int  iA );


//===============================================================================================================


#if defined(__APPLE__) && defined(__MACH__)
enum
{
    kOffset_Alpha = 0,
    kOffset_Red   = 1,
    kOffset_Green = 2,
    kOffset_Blue  = 3,
};
#endif
#ifdef LINUX
enum
{
    kOffset_Alpha = 0,
    kOffset_Red   = 1,
    kOffset_Green = 2,
    kOffset_Blue  = 3,
};
#endif
#ifdef _WIN64
enum
{
    kOffset_Blue  = 0,
    kOffset_Green = 1,
    kOffset_Red   = 2,
    kOffset_Alpha = 3,
};
#endif


//===============================================================================================================


/** @brief  A 32bit RGBA pixel.

    This is an UNION.
    It only takes 4 bytes in memory, and you can access it either as :
    - a PIPixel32 (PIPixel::l), when you process the pixel as a whole, like when you are moving them around,
    - a four byte array (PIPixel::c), when you need to loop over the channels (but beware of the order of the four channels !),
    - four separate components (PIPixel::Red, PIPixel::Green, PIPixel::Blue, PIPixel::Alpha).

    @see  PIBlock and PIFilter for component order.
    @see  ColorControl   //TODO: name !  //DOXYGEN: doesn't work for \@page ?!?!?!

    @note  The order of the different color channels is OS dependant !
           This dates back to the times when graphic boards weren't able to shuffle them in real time,
           and had to be kept for backward compatibility...

    @ingroup  sdk_PIPixel
*/
typedef  union  PIPixel
{
    PIPixel32  l;  ///< The "whole" representation.
    PIUInt8  c[4]; ///< The "array" representation
#if defined(__APPLE__) && defined(__MACH__)
    struct
    {
        PIUInt8  Alpha;  ///< The alpha channel.
        PIUInt8  Red;    ///< The red channel.
        PIUInt8  Green;  ///< The green channel.
        PIUInt8  Blue;   ///< The blue channel.
    };
#endif
#ifdef LINUX
    struct
    {
        PIUInt8  Alpha;
        PIUInt8  Red;
        PIUInt8  Green;
        PIUInt8  Blue;
    };
#endif
#ifdef _WIN64
    struct
    {
        PIUInt8  Blue;
        PIUInt8  Green;
        PIUInt8  Red;
        PIUInt8  Alpha;
    };
#endif
} PIPixel;

#ifdef __cplusplus
static_assert( sizeof(PIPixel) == 4, "Bad size" );
#endif


//===============================================================================================================


///@name  Pixel handling.
///@{


/** @brief  Makes a 32 bit premultiplied pixel in the correct RGBA order (PIPixel).

    @param[in]  iR  The red component of the pixel (premultiplied, must be <= iA).
    @param[in]  iG  The green component of the pixel (premultiplied, must be <= iA).
    @param[in]  iB  The blue component of the pixel (premultiplied, must be <= iA).
    @param[in]  iA  The alpha component of the pixel.

    @return  The 32 bit pixel (PIPixel).

    @see  ColorControl   //TODO: name !  //DOXYGEN: \@see doesn't work for \@page ?!?!?!

    @ingroup  sdk_PIPixel
*/
PIPixel  TVMakePIPixel( int  iR, int  iG, int  iB, int  iA );


/** @brief  Merges two pixel sources into one.
    This uses Alpha channel and density. The second source will be merged over the first.

    @param[in]   iPlugin    This plugin's reference.
    @param[in]   iSrc1      The first source of pixels.
    @param[in]   iDensity1  The density of the first source (0->255).
    @param[in]   iSrc2      The second source of pixels.
    @param[in]   iDensity2  The density of the second source (0->255).
    @param[out]  oDst       The destination for the blended pixels.
    @param[in]   iWidth     Width of the area to blend.
    @param[in]   iHeight    Height of the area to blend.
    @param[in]   iModSrc1   "Modulo" of the first source (= iSrc1->width - iWidth).
    @param[in]   iModSrc2   "Modulo" of the second source (= iSrc2->width - iWidth).
    @param[in]   iModDst    "Modulo" of the destination (= oDst->width - iWidth).

    @see  TVColorBlendMode()

    @ingroup  sdk_PIPixel
*/
void  TVColorBlend( PIPlugin*  iPlugin, const PIPixel*  iSrc1, int  iDensity1, const PIPixel*  iSrc2, int  iDensity2, PIPixel*  oDst, int  iWidth, int  iHeight, int  iModSrc1, int  iModSrc2,  int  iModDst );


/** @brief  Merges two pixel sources into one.
    This uses Alpha channel. The second source will be merged over the first using the blending mode.

    @param[in]   iPlugin     This plugin's reference.
    @param[in]   iSrc1       The first source of pixels.
    @param[in]   iSrc2       The second source of pixels.
    @param[out]  oDst        The destination for the blended pixels.
    @param[in]   iCount      The number of pixels to blend.
    @param[in]   iBlendMode  The blending mode (@see Blending_Mode_Flags).

    @see  TVColorBlend()

    @ingroup  sdk_PIPixel
*/
void  TVColorBlendMode( PIPlugin*  iPlugin, const PIPixel*  iSrc1, const PIPixel*  iSrc2, PIPixel*  oDst, int  iCount, int  iBlendMode );


/** @brief  This function packs an array of 32bit pixels, using RLE encoding.

    @param[in]   iPlugin    This plugin's reference.
    @param[in]   iSrc       The pixels to compress.
    @param[out]  oDst       The compressed array of bytes (must be 10% bigger than source array).
    @param[in]   iSrcCount  Number of 32bit pixels in source.

    @return  Return the size (in bytes) of the packed result.

    @see  TVUnpackRLE()

    @ingroup  sdk_PIPixel
*/
int  TVPackRLE( PIPlugin*  iPlugin, const PIPixel32*  iSrc,  char*  oDst, int  iSrcCount );


/** @brief  This function unpacks an array of 32bit pixels, using RLE encoding.

    @param[in]   iPlugin    This plugin's reference.
    @param[in]   iSrc       The bytes to uncompress.
    @param[out]  oDst       The uncompressed pixels.
    @param[in]   iDstCount  Number of 32bit pixels to decompress.

    @return  Return the number of bytes used in source.

    @see  TVPackRLE()

    @ingroup  sdk_PIPixel
*/
int  TVUnpackRLE( PIPlugin*  iPlugin, const char*  iSrc,  PIPixel32*  oDst, int  iDstCount );


/** @brief  Converts RGBA pixels to YUV pixels and a separate alpha channel.

    @param[in]   iPlugin  This plugin's reference.
    @param[in]   iRGBA    The RGBA pixels.
    @param[out]  oYUV     The returned YUV pixels (in UYVY format).
    @param[out]  oAlpha   The returned alpha channel.
    @param[in]   iCount   The number of pixels to convert.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIPixel
*/
int  TVRGBAToYUV( PIPlugin*  iPlugin, const PIPixel*  iRGBA, PIUInt8*  oYUV, PIUInt8*  oAlpha, int  iCount );


/** @brief  Converts YUV pixels and a separate alpha channel to RGBA pixels.

    @param[in]   iPlugin  This plugin's reference.
    @param[in]   iYUV     The YUV pixels (in UYVY format).
    @param[in]   iAlpha   The alpha channel.
    @param[out]  oRGBA    The returned RGBA pixels.
    @param[in]   iCount   The number of pixels to convert.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIPixel
*/
int  TVYUVToRGBA( PIPlugin*  iPlugin, const PIUInt8*  iYUV, const PIUInt8*  iAlpha, PIPixel*  oRGBA, int  iCount );


/** @brief  Unmultiplies an array of RGBA pixels.

    @param[in]   iPlugin  This plugin's reference.
    @param[in]   iRGBA    The pixels to convert.
    @param[out]  oRGBA    The converted pixels.
    @param[in]   iCount   The number of pixels to convert.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
 
    @see  ColorControl

    @ingroup  sdk_PIPixel
*/
int  TVRGBAUnmultiply( PIPlugin*  iPlugin, const PIPixel*  iRGBA, PIPixel*  oRGBA, int  iCount );


/** @brief  Premultiplies an array of RGBA pixels.

    @param[in]   iPlugin  This plugin's reference.
    @param[in]   iRGBA    The pixels to convert.
    @param[out]  oRGBA    The converted pixels.
    @param[in]   iCount   The number of pixels to convert.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
 
    @see  ColorControl

    @ingroup  sdk_PIPixel
*/
int  TVRGBAPremultiply( PIPlugin*  iPlugin, const PIPixel*  iRGBA, PIPixel*  oRGBA, int  iCount );


/** @brief  Allows the user to pick a color on the screeen.

    @param[in]   iPlugin  This plugin's reference.
    @param[out]  oResult  The pixel to receive the picked color.

    @return  An error code: 0 = An error occured OR the user cancelled the picking, 1 = Everything went well.

    @ingroup  sdk_PIPixel
 */
int  TVPickColor( PIPlugin*  iPlugin, PIPixel*  oResult );


///@}


//===============================================================================================================


/** @page ColorControl  Color Control

    @todo cleanup doxygen formatting, pre-/un-multiplied, and frenglish...

This chapter is VERY IMPORTANT (VERY VERY IMPORTANT!), because TVPaint uses specific rules to handle colors.

EVERY color in TVPaint is coded with RGB values which are never (NEVER!) greater than the Alpha value.

A 100% opaque white is coded        R=255 G=255 B=255 A=255\n
A 100% opaque 50% gray is coded     R=128 G=128 B=128 A=255\n
A 50% transparent white is coded    R=128 G=128 B=128 A=128\n
A 10% transparent white is coded    R= 25 G= 25 B= 25 A= 25\n
Opaque black is                     R=  0 G=  0 B=  0 A=255\n
Fully transparent is                R=  0 G=  0 B=  0 A=  0
 
In a filter, before you work with colors, you (usually) have to convert these colors in classic RGB.
In the same way, after the work, you ABSOLUTELY MUST convert these colors so as to have RGB values not greater than the Alpha value.

Real Red   = Red in layer * 255 / Alpha \n
Real Green = Green in layer * 255 / Alpha\n
Real Blue  = Blue in layer * 255 / Alpha\n

Red in layer   = Real Red * Alpha / 255\n
Green in layer = Real Green * Alpha / 255\n
Blue in layer  = Real Blue * Alpha / 255\n

If Alpha == 0 RGB must be 0. The RGB values must NEVER be greater than the Alpha value.
If they are, the working image is altered and the reactions of TVPaint are OUT OF CONTROL.
Developers are supposed to know what they do.

@note George commands return non-premultiplied colors. It means RGB Channels can be greater than Alpha Channel.

@ingroup  sdk_PIPixel
*/


//=============================================================================================================== Deprecations


TVDEPRECATED( "Use 'TVMakePIPixel32' instead." )
/** @deprecated  Use 'TVMakePIPixel32' instead. */
inline  PIPixel32  TVMakeRGBAPixel( PIPlugin*  iPlugin, int  iR, int  iG, int  iB, int  iA )
{
    return  TVMakePIPixel32( iR, iG, iB, iA );
}


#ifdef __cplusplus
}
#endif


#endif // __pi_pixel_HH
