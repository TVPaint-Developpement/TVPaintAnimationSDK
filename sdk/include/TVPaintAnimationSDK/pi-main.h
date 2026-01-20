/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-main.h
*  @ingroup  sdk_TVPA
*
*  @brief  The main functions.
*/

#ifndef __pi_main_HH
#define __pi_main_HH

#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIMain  The main functions.
*/


//===============================================================================================================


///@name  Main Functions.
///@{


/** @brief  Creates a new entry for the plugin in the plugin menu.
    Useful for plugins having multiple functions.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iAlias   Name of the new entry.
    @param[in]  iParams  Parameters to pass to the plugin when the user selects the entry.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIMain
*/
int  TVCreateFilterAlias( PIPlugin*  iPlugin, const char*  iAlias, const char*  iParams );


/** @brief  Executes a filter.
    This will prepare the host for applying a filter on the current layer.
    Then the host will call these functions before returning from this call:
    - PI_SequenceStart() once, with the total number of frames to be processed (depending on the selection the user has done in Host)
    - For each frame:
        - PI_Start() for each frame.
        - PI_Work() for each frame.
        - PI_Finish() for each frame.
    - PI_SequenceFinish() once.

    In PI_SequenceStart(), you allocate and/or initialize everything that is constant throughout all frames.
    In PI_Start(), you allocate and/or initialize data for processing this frame.
    In PI_Work(), you actually modify the image (and you DON'T in ANY of the other functions !).
    In PI_Finish(), you cleanup what you've done in PI_Start().
    In PI_SequenceFinish(), you cleanup what you've done in PI_SequenceStart().

    @param[in]  iPlugin  This plugin's reference.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIMain
*/
int  TVExecute( PIPlugin*  iPlugin );


/** @brief  Runs a function on several threads.
    You CAN'T call any Host function inside that function.
    The main use is to process an image inside PI_Work().

    It will split the [iBegin,iEnd] range equally among all cores of your computer, calling iFunction on a subrange on a separate core.

    @param[in]  iPlugin    This plugin's reference.
    @param[in]  iFunction  The function to call.
    @param[in]  iStart     The starting index to process.
    @param[in]  iEnd       The ending index to process.
    @param[in]  iParam     A pointer to a data block containing infos for the function, it isn't used in any way by TVRunMultiThread, only passed thru to iFunction.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIMain
*/
int  TVRunMultiThread( PIPlugin*  iPlugin, int  (*iFunction)( void*  iThread, int  iStart, int  iEnd, void*  iParam), int  iStart, int  iEnd, void*  iParam );


/** @brief  Sends a George command to the Host.

    You can call any George function, for example :
    @code
        tv_SetAPen 255 0 0
    @endcode
    to set the current pen color to red.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iCommand  The command string to be executed by the Host.
    @param[in]  oResult   A buffer for the result of the command, or NULL if you don't care about it.

    @return  An error code: 0 = An error occured, 1 = Everything went well.
 
    @note  The size of the oResult buffer being not specified, there is a small chance of a buffer overrun, so make it quite big !

    @ingroup  sdk_PIMain
*/
int  TVSendCmd( PIPlugin*  iPlugin, const char*  iCommand, char*  oResult );


/** @brief  Refreshes an area in the project display window.
    Should be called when you called TVWriteLayerData() with the CB_WRITE_QUIET mode,
    or if you modified pixels in PIFilter->Current.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iX1      Coordinate of the area to refresh.
    @param[in]  iY1      Coordinate of the area to refresh.
    @param[in]  iX2      Coordinate of the area to refresh.
    @param[in]  iY2      Coordinate of the area to refresh.

    @ingroup  sdk_PIMain
*/
void  TVUpdateDisplay( PIPlugin*  iPlugin, int  iX1, int  iY1, int  iX2, int  iY2 );


///@}


#ifdef __cplusplus
}
#endif


#endif // __pi_main_HH

