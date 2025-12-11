/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-filerequester.h
*  @ingroup  sdk_TVPA
*
*  @brief  The file requester utilities.
*/

#ifndef __pi_filerequester_HH
#define __pi_filerequester_HH


#include "TVPaintAnimationSDK/pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIFileRequester  Stuff related to the file requester.
*/


//===============================================================================================================


/** @brief  The informations about a file.

    @ingroup  sdk_PIFileRequester
*/
typedef  struct  PIFileInfo
{
    /** @brief  Maximum size in any direction of the thumbnail. Provided by TVPaint. */
    int  MaxIconSize;

    /** @brief  The thumbnail.
        You SHOULD fill this in in the Info() function of PIFileRequester if asked for by iFull.
        TVPaint will take ownership of it, you MUST NOT free it. @todo //CHECK:
     */
    PIBlock*  Icon;

    /** @brief  The textual infos of the file, like width, height, bit depth, embedded comment, etc... */
    char  TextInfo[2048];

    /** @brief  Reserved for TVPaint, don't modify it ! */
    PIUInt64  Reserved[16];
} PIFileInfo;


//===============================================================================================================


/** @brief  A file requester.

    @see  TVAllocFileReq(), TVOpenFileReq(), TVFreeFileReq()

    @ingroup  sdk_PIFileRequester
*/
typedef  struct  PIFileRequester
{
    /** @brief  Private field reserved for TVPaint.
        Set it to NULL when initializing the struct, after that don't modify it !
     */
    void*  Private;

    /** @brief  Title of the window.
        It should start with '<' for loading a file, like "<Load...".
        It should start with '>' for saving a file, like ">Save...".
     */
    const char*  Title;

    /** @brief  Pattern to filter the displayed files.
        Unfortunately, this is OS specific.
     */
    char  Pattern[4096];

    /** @brief  Path to the folder in which the file requester will open.
        You can use the forward slash '/' also on windows.
     */
    char  Path[8192];

    /** @brief  Show the icons of the files ?
        Not showing the icons is usually much faster.
        0 = show, 1 = don't show !!  @todo //CHECK: !!
     */
    int  Icon;
#ifdef _WIN64
    FARPROC  Info;
#else
    /** @brief  You can set this pointer to a function to generate the file info and thumbnail.
        You have to provide your own function only if you want to show icons for some 'proprietary' information,
        else the one provided by default by TVAllocFileReq() is just fine.

        @param[in]   iPlugin    This plugin's reference.
        @param[in]   iFull      If 0, generate the thumbnail in all cases (slow!), if 1, generate it only if one is stored in the file (fast).
        @param[in]   iName      Complete path of the file.
        @param[out]  oFileInfo  The PIFileInfo you have to fill in.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*Info)( PIPlugin*  iPlugin, int  iFull, char*  iName, PIFileInfo*  oFileInfo );
#endif

    /** @brief  Reserved data, fill with 0 then forget about it... */
    PIUInt64  Reserved[16];
} PIFileRequester;


//===============================================================================================================


/** @brief  Allocates a file requester.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iFileReq  A PIFileRequester structure describing the requester.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVFreeFileReq(), TVOpenFileReq()

    @ingroup  sdk_PIFileRequester
*/
int  TVAllocFileReq( PIPlugin*  iPlugin, PIFileRequester*  iFileReq );


/** @brief  Open a file requester.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iFileReq  A PIFileRequester structure, has to be allocated with TVAllocFileReq().

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  TVAllocFileReq(), TVFreeFileReq()

    @ingroup  sdk_PIFileRequester
*/
int  TVOpenFileReq( PIPlugin*  iPlugin, PIFileRequester*  iFileReq );


/** @brief  Frees a file requester.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iFileReq  A PIFileRequester structure, has to be allocated with TVAllocFileReq().

    @see  TVAllocFileReq(), TVOpenFileReq()

    @ingroup  sdk_PIFileRequester
*/
void  TVFreeFileReq( PIPlugin*  iPlugin, PIFileRequester*  iFileReq );


#ifdef __cplusplus
}
#endif


#endif // __pi_filerequester_HH
