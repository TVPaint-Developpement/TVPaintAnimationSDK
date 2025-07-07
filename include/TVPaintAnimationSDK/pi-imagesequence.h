/** @copyright (c) 2002-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-imagesequence.h
*  @ingroup  sdk_TVPA
*
*  @brief  An image sequence.
*/

#ifndef __pi_imagesequence_HH
#define __pi_imagesequence_HH


#include "pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIImageSequence  Stuff related to PIImageSequence.

    An image sequence.
*/


//===============================================================================================================


/** @brief  PIImageSequence flags.

    @ingroup  sdk_PIImageSequence
*/
typedef  enum  ePIImageSequenceFlags
{
    /** Scan to see if the file is part of an image sequence. */
    kPIImageSequenceFlags_Open   = 0,
    /** Don't scan to see if the file is part of an image sequence, assume its only one image. */
    kPIImageSequenceFlags_NoScan = 1,
} ePIImageSequenceFlags;


//===============================================================================================================


/** @brief  Used to read an image sequence.

    @ingroup  sdk_PIImageSequence
*/
typedef  struct  PIImageSequence
{
    /** PIImageSequence version number.
        This is equal to sizeof(struct PIImageSequence) for some reason...
     */
    int  Version;
    /** Full file name with path and extension. */
    const char*  FileName;
    /** Width of the sequence in pixels. */
    int  Width;
    /** Height of the sequence in pixels. */
    int  Height;
    /** Number of frames in the sequence. */
    int  ImageCount;
    /** Frame rate. If there is no frame rate (or it is not known), FrameRate==0. */
    double  FrameRate;

    /** Closes the image sequence. */
    void (*Close)    ( struct PIImageSequence*  iSequence );
    /** Reads an image from the sequence. */
    int  (*ReadImage)( struct PIImageSequence*  iSequence, int  iPosition, PIBlock*  oDest );
} PIImageSequence;


//===============================================================================================================


/** @brief  Opens an Image Sequence.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iFileName  The full pathname of the sequence to open.
    @param[in]  iFlags     Flags (ePIImageSequenceFlags).

    @return  The new Image Sequence, or NULL if failure.

    @ingroup  sdk_PIImageSequence
*/
PIImageSequence*  TVOpenImageSequence( PIPlugin*  iPlugin, const char*  iFileName, ePIImageSequenceFlags  iFlags );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIImageSequenceFlags instead. */
enum  TVDEPRECATED( "Use ePIImageSequenceFlags instead" )  ISEQ_FLAG_DEPRECATED
{
    ISEQ_FLAG_OPEN     TVDEPRECATED( "Use kPIImageSequenceFlags_Open instead"   ) = kPIImageSequenceFlags_Open,
    ISEQ_FLAG_NO_SCAN  TVDEPRECATED( "Use kPIImageSequenceFlags_NoScan instead" ) = kPIImageSequenceFlags_NoScan,
};


#ifdef __cplusplus
};
#endif


#endif // __pi_imagesequence_HH
