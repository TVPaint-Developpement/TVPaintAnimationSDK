/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-block.h
*  @ingroup  sdk_TVPA
*
*  @brief  Rectangular blocks of pixels (an "image").
*/

#ifndef __pi_block_HH
#define __pi_block_HH


#include "pi-basics.h"
#include "pi-filter.h"
#include "pi-pixel.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIBlock  Stuff related to PIBlock.

    A PIBlock is what is usually called an image.
*/


//===============================================================================================================


/** @brief  The blending modes supported by the SDK.
    This is a subset of TVPaint's ones.

    @ingroup  sdk_PIBlock
  */
typedef  enum  ePIBlendingMode
{
    kPIBlendingMode_Color    = 0,
    kPIBlendingMode_Add      = 1,
    kPIBlendingMode_Sub      = 2,
    kPIBlendingMode_Behind   = 3,
    kPIBlendingMode_Multiply = 4,
    kPIBlendingMode_Screen   = 5,
    kPIBlendingMode_Replace  = 6,
    kPIBlendingMode_Colorize = 7,
    kPIBlendingMode_Hue      = 8,
    kPIBlendingMode_Erase    = 9,
    kPIBlendingMode_Shade    = 10,
    kPIBlendingMode_Light    = 11,

    bPIBlendingMode_Fast     = (1<<16)
} ePIBlendingMode;


/** @brief  The layer to read with TVReadLayerData().
    It can also read the image of the custom brush.

    @ingroup  sdk_PIBlock
  */
typedef  enum  ePIReadLayer
{
    /**  @brief  The custom brush. */
    kPIReadLayer_Brush   = -4,
    /**  @brief  The display, i.e. all layers composited together. */
    kPIReadLayer_Display = -3,
    /**  @brief  The undo buffer, that is, the image of the current layer before any modification has been applied. */
    kPIReadLayer_Undo    = -2,
    /**  @brief  The spare buffer. */
    kPIReadLayer_Spare   = -1,
    /**  @brief  The current layer. */
    kPIReadLayer_Current = 0,
} ePIReadLayer;


/** @brief  Should TVPaint refresh the display when calling TVWriteLayerData() ?
    Quiet mode is useful when you make several changes one after the other, but be sure to pass kPIWriteLayer_Update to the last one.

    @ingroup  sdk_PIBlock
  */
typedef  enum  ePIWriteLayer
{
    /** @brief  Don't refresh the display. */
    kPIWriteLayer_Quiet  = 0,
    /** @brief  Refresh the display. */
    kPIWriteLayer_Update = 1,
} ePIWriteLayer;


//===============================================================================================================


/** @brief  A rectangular block of pixels.
 
    @ingroup  sdk_PIBlock
 */
typedef  struct  PIBlock
{
    /** @brief  Width of the block, in pixels. */
    PIInt32  Width;
    /** @brief  Height of the block, in pixels. */
    PIInt32  Height;

    /** @brief  Offsets (from 0 to 3) which describe the order of the RGBA values in a pixel.
        This order is NOT ALWAYS the same for different versions of TVPaint, although it is for all PIBlocks in the same run.
        They are identical to PlaneOffsetRed, Green,... from the PIFilter structure.
        These fields are read only.
     */
    PIUInt8  r;
    /** @brief  See 'r' above. */
    PIUInt8  g;
    /** @brief  See 'r' above. */
    PIUInt8  b;
    /** @brief  See 'r' above. */
    PIUInt8  a;

    /** @brief  Pointer to pixels data.
        To prepare for future versions of TVPaint, use of this field is discouraged, use Img instead.
     */
    PIPixel*  Data;

    /** @brief  Not used anymore, MUST be 0. */
    PIInt32  x;
    /** @brief  Not used anymore, MUST be 0. */
    PIInt32  y;

    /** @brief  Allows a simpler access to pixels than Data.
        Access pixel at (x,y) with block->Img[y][x]. Note the reversed [y][x] !
        It is highly encouraged to used this method to access pixels for compatibility with future versions of TVPaint.
     */
    PIPixel**  Img;

    /** @brief  Method to free iBlock. Same as TVFreePIBlock(). */
    void  (*free) ( struct  PIBlock*  iBlock );
    /** @brief  Method to clear the whole iBlock with the specified iColor. */
    void  (*clear)( struct  PIBlock*  iBlock, PIPixel32  iColor );

    /** @brief  Private use for the host, never change it ! */
    PIUInt64  Reserved[1];    // reserved
} PIBlock;


//===============================================================================================================


/** @brief  Allocates a block of pixels.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iWidth   The width of the block.
    @param[in]  iHeight  The height of the block.
    @param[in]  iFlags   Don't use, always pass 0.

    @return  The newly allocated block. Returns NULL on failure.

    @see  TVFreePIBlock(), TVPutButtonImage()

    @ingroup  sdk_PIBlock
 */
PIBlock*  TVAllocPIBlock( PIPlugin*  iPlugin, int  iWidth,  int  iHeight, PIFlags  iFlags );


/** @brief  Frees a block of pixels allocated with TVAllocPIBlock().

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iBlock   The block to free.

    @see  TVAllocPIBlock()

    @ingroup  sdk_PIBlock
 */
void  TVFreePIBlock( PIPlugin*  iPlugin, PIBlock*  iBlock );


/** @brief  Draws a text inside a PIBlock with the TVPaint system font.

    @param[in]      iPlugin  This plugin's reference.
    @param[in,out]  ioBlock  The pixel block to draw into.
    @param[in]      iX       The X coordinate of the lower left (//CHECK:) corner of the text.
    @param[in]      iY       The Y coordinate of the lower left (//CHECK:) corner of the text.
    @param[in]      iText    The text.
    @param[in]      iAPen    The color for the text.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIBlock
 */
int  TVTextBlock( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX, int  iY, const char*  iText, PIPixel32  iAPen );


/** @brief  Perspective warp the iSrc pixel block into the ioDst pixel block.

    @param[in]      iPlugin  This plugin's reference.
    @param[in]      iSrc     The pixel block to draw into.
    @param[in,out]  ioDst    The pixel block to draw into.
    @param[in]      iX1      The X coordinate of the upper left corner.
    @param[in]      iY1      The Y coordinate of the upper left corner.
    @param[in]      iX2      The X coordinate of the upper right corner.
    @param[in]      iY2      The Y coordinate of the upper right corner.
    @param[in]      iX3      The X coordinate of the lower right corner.
    @param[in]      iY3      The Y coordinate of the lower right corner.
    @param[in]      iX4      The X coordinate of the lower left corner.
    @param[in]      iY4      The Y coordinate of the lower left corner.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
 
    @see  TVWrapPIBlockMode()

    @ingroup  sdk_PIBlock
 */
int  TVWrapPIBlock( PIPlugin*  iPlugin, const PIBlock*  iSrc, PIBlock*  ioDst, double  iX1, double  iY1, double  iX2, double  iY2, double  iX3, double  iY3, double  iX4, double  iY4 );


/** @brief  Perspective warp the iSrc pixel block into the ioDst pixel block.

    @param[in]      iPlugin     This plugin's reference.
    @param[in]      iSrc        The pixel block to draw into.
    @param[in,out]  ioDst       The pixel block to draw into.
    @param[in]      iX1         The X coordinate of the upper left corner.
    @param[in]      iY1         The Y coordinate of the upper left corner.
    @param[in]      iX2         The X coordinate of the upper right corner.
    @param[in]      iY2         The Y coordinate of the upper right corner.
    @param[in]      iX3         The X coordinate of the lower right corner.
    @param[in]      iY3         The Y coordinate of the lower right corner.
    @param[in]      iX4         The X coordinate of the lower left corner.
    @param[in]      iY4         The Y coordinate of the lower left corner.
    @param[in]      iTile       Same as TVPaint's Fill mode. It repeats iSrc horizontally and vertically across ioDst.
    @param[in]      iBlendMode  The blending mode (@ref ePIBlendingMode).
 
    @return  An error code: 0 = An error occured, 1 = Everything went well.
 
    @see  TVWrapPIBlock()

    @ingroup  sdk_PIBlock
 */
int  TVWrapPIBlockMode( PIPlugin*  iPlugin, const PIBlock*  iSrc, PIBlock*  ioDst, double  iX1, double  iY1, double  iX2, double  iY2, double  iX3, double  iY3, double  iX4, double  iY4, int  iTile, int  iBlendMode );


/** @brief  This function reads an image from a file and returns the image size.

    @param[in]   iPlugin     This plugin's reference.
    @param[in]   iFileName   The file's name/path.
    @param[out]  oDst        The pixel block to read into (or NULL if you just want the image size, to allocate the block with the exact size needed and call TVReadImage() again.)
    @param[in]   iDstWidth   The width of oDst.
    @param[in]   iDstHeight  The height of oDst.

    @return  The image size, as (Width<<16 | Height), 0 on failure.

    @ingroup  sdk_PIBlock
 */
int  TVReadImage( PIPlugin*  iPlugin, const char*  iFileName, PIPixel*  oDst, int  iDstWidth, int  iDstHeight );


/** @brief  Blurs the ioBlock pixel block, using the gaussian blur algorithm.

    @param[in]      iPlugin        This plugin's reference.
    @param[in,out]  ioBlock        The pixel block to blur.
    @param[in]      iBlurX         The size of the blur in X.
    @param[in]      iBlurY         The size of the blur in Y.
    @param[in]      iMirrorBorder  The the boeders be mirrored, or should we handle the "outside" as clear ?
 
    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIBlock
 */
int  TVBlockBlur( PIPlugin*  iPlugin, PIBlock*  ioBlock, double  iBlurX, double  iBlurY, int  iMirrorBorder );


/** @brief  Stretches the iSrc pixel block into the oDst pixel block.

    @param[in]   iPlugin  This plugin's reference.
    @param[in]   iSrc     The pixel block to stretch.
    @param[out]  oDst     The resulting stretched pixel block.
 
    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIBlock
 */
int  TVBlockStretch( PIPlugin*  iPlugin, const PIBlock*  iSrc, PIBlock*  oDst );


/** @brief  This function create a CustomBrush from the parameters.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iBrush   The pixels for the new brush.
    @param[in]  iWidth   Width of the image.
    @param[in]  iHeight  Height of the image.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
*/
int  TVMakeBrush( PIPlugin*  iPlugin, const PIPixel*  iBrush, int  iWidth, int  iHeight );


/** @brief  Create a display picture from selected position in the timeline.

    @param[in]   iPlugin       This plugin's reference.
    @param[out]  oDst          The destination for the pixels.
    @param[in]   iDstSize      Size of the oDst array, in BYTES (not pixels !!!). Must be >= iPlugin->ImageWidth*iPlugin->ImageHeight*sizeof(PIPixel)
    @param[in]   iImageNumber  Image number in the timeline.
    @param[in]   iBackground   1 if you want the picture with the background, 0 otherwise.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
*/
int  TVMakeDisplayImage( PIPlugin*  iPlugin, PIPixel*  oDst, int  iDstSize, int  iImageNumber, int  iBackground );


/** @brief  Reads a rectangular area from the current image of a layer.
    It can also read from the custom brush.
    This function is clipped.

    @param[in]   iPlugin  This plugin's reference.
    @param[out]  oDst     The pixels to read into.
    @param[in]   iX       The top left corner of the area to read.
    @param[in]   iY       The top left corner of the area to read.
    @param[in]   iW       The size of the area to read.
    @param[in]   iH       The size of the area to read.
    @param[in]   iMod     "Modulo" = (oDst.width) – iW.
    @param[in]   iLayer   The layer to read (@see ePIReadLayer).

    @return  1 if something is read, 0 if nothing has been read (area clipped away or non-existent layer).

    @see  TVWriteLayerData()
*/
int  TVReadLayerData( PIPlugin*  iPlugin, PIPixel*  oDst, int  iX, int  iY, int  iW, int  iH, int  iMod, ePIReadLayer  iLayer );


/** @brief  Writes a rectangular area into the current layer.
    This function is clipped.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iSrc      The pixels to write from.
    @param[in]  iX        The top left corner of the area to write.
    @param[in]  iY        The top left corner of the area to write.
    @param[in]  iW        The size of the area to write.
    @param[in]  iH        The size of the area to write.
    @param[in]  iMod      "Modulo" = (iSrc.width) – iW.
    @param[in]  iDisplay  Refresh the display or not (@see ePIWriteLayer).

    @return  1 if something is written, 0 if nothing has been written (?!?!?!??! //CHECK: ).

    @see  TVReadLayerData()
*/
int  TVWriteLayerData( PIPlugin*  iPlugin, const PIPixel*  iSrc, int  iX, int  iY, int  iW, int  iH, int  iMod, ePIWriteLayer  iDisplay );


/** @brief  Reads an image from your current project.

    @param[in]   iPlugin       This plugin's reference.
    @param[out]  oDst          The pixel block to read into.
    @param[in]   iProjectID    The ID of the project to read from, or 0 to read from the current project.
    @param[in]   iLayerID      The ID of the layer to read from, or 0 to read from the current layer, or -1 to read the whole display (the whole stack of layers composed together, including the backgroud).
    @param[in]   iImageNumber  The number of the image to read. (//CHECK: relative to what ?!???!?!)

    @return  1 if something is read, 0 on failure.

    @note  oDst must have the same size than the project to read from.
    You can get this size from iPlugin->ImageWidth and iPlugin->ImageHeight.
*/
int  TVReadProjectImage( PIPlugin*  iPlugin, PIBlock*  oDst, int  iProjectID, int  iLayerID, int  iImageNumber );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIBlendingMode instead. */
enum  TVDEPRECATED( "Use ePIBlendingMode instead" )  PIWRAP_MODE_DEPRECATED
{
    PIWRAP_MODE_BLEND     TVDEPRECATED( "Use kPIBlendingMode_Color instead"    ) = kPIBlendingMode_Color,
    PIWRAP_MODE_ADD       TVDEPRECATED( "Use kPIBlendingMode_Add instead"      ) = kPIBlendingMode_Add,
    PIWRAP_MODE_SUB       TVDEPRECATED( "Use kPIBlendingMode_Sub instead"      ) = kPIBlendingMode_Sub,
    PIWRAP_MODE_PRESERVE  TVDEPRECATED( "Use kPIBlendingMode_Behind instead"   ) = kPIBlendingMode_Behind,
    PIWRAP_MODE_MULTIPLY  TVDEPRECATED( "Use kPIBlendingMode_Multiply instead" ) = kPIBlendingMode_Multiply,
    PIWRAP_MODE_SCREEN    TVDEPRECATED( "Use kPIBlendingMode_Screen instead"   ) = kPIBlendingMode_Screen,
    PIWRAP_MODE_REPLACE   TVDEPRECATED( "Use kPIBlendingMode_Replace instead"  ) = kPIBlendingMode_Replace,
    PIWRAP_MODE_COLORIZE  TVDEPRECATED( "Use kPIBlendingMode_Colorize instead" ) = kPIBlendingMode_Colorize,
    PIWRAP_MODE_HUE       TVDEPRECATED( "Use kPIBlendingMode_Hue instead"      ) = kPIBlendingMode_Hue,
    PIWRAP_MODE_ERASE     TVDEPRECATED( "Use kPIBlendingMode_Erase instead"    ) = kPIBlendingMode_Erase,
    PIWRAP_MODE_SHADE     TVDEPRECATED( "Use kPIBlendingMode_Shade instead"    ) = kPIBlendingMode_Shade,
    PIWRAP_MODE_LIGHT     TVDEPRECATED( "Use kPIBlendingMode_Light instead"    ) = kPIBlendingMode_Light,

    PIWRAP_FAST           TVDEPRECATED( "Use bPIBlendingMode_Fast instead"     ) = bPIBlendingMode_Fast
};


/** @deprecated  Use ePIReadLayer instead. */
enum  TVDEPRECATED( "Use ePIReadLayer instead" )  CB_READ_DEPRECATED
{
    CB_READ_BRUSH    TVDEPRECATED( "Use kPIReadLayer_Brush instead"   ) = kPIReadLayer_Brush,
    CB_READ_DISPLAY  TVDEPRECATED( "Use kPIReadLayer_Display instead" ) = kPIReadLayer_Display,
    CB_READ_UNDO     TVDEPRECATED( "Use kPIReadLayer_Undo instead"    ) = kPIReadLayer_Undo,
    CB_READ_SPARE    TVDEPRECATED( "Use kPIReadLayer_Spare instead"   ) = kPIReadLayer_Spare,
    CB_READ_CURRENT  TVDEPRECATED( "Use kPIReadLayer_Current instead" ) = kPIReadLayer_Current,
    CB_READ_FRONT    TVDEPRECATED( "Doesn't exists anymore"           ) = kPIReadLayer_Current,
    CB_READ_BACK     TVDEPRECATED( "Doesn't exists anymore"           ) = kPIReadLayer_Current,
};


/** @deprecated  Use ePIWriteLayer instead. */
enum  TVDEPRECATED( "Use ePIWriteLayer instead" )  CB_WRITE_DEPRECATED
{
    CB_WRITE_QUIET   TVDEPRECATED( "Use kPIWriteLayer_Quiet instead"  ) = kPIWriteLayer_Quiet,
    CB_WRITE_UPDATE  TVDEPRECATED( "Use kPIWriteLayer_Update instead" ) = kPIWriteLayer_Update,
};


#ifdef __cplusplus
}
#endif


#endif // __pi_block_HH
