/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-misc.h
*  @ingroup  sdk_TVPA
*
*  @brief  Miscellaneous stuff that doesn't fit anywhere else.
*/

#ifndef __pi_misc_HH
#define __pi_misc_HH

#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIMisc  Miscellaneous stuff that doesn't fit anywhere else.
*/


//===============================================================================================================


///@name  Miscellaneous.
///@{


/** @brief  The options for TVGrabTicks().

    @ingroup  sdk_PIMisc
*/
typedef  enum  ePITicks
{
    /** Start receiving tick messages. */
    kPITicks_Off    = 0,
    /** Stop receiving tick messages. */
    kPITicks_On     = 1,
    /** Start receiving always tick messages (of for requesters with the bPIRequesterFlags_Lock flag. */
    kPITicks_NoWait = 666,
} ePITicks;


/** @brief  Enables/Disables the reception of the PICBREQ_TICKS message in PI_Msg() at each timer tick (about every 20 milliseconds).

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The window on which we want to get tick messages.
    @param[in]  iFlag    The flags, @see ePITicks.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  PI_Msg()

    @ingroup  sdk_PIMisc
*/
int  TVGrabTicks( PIPlugin*  iPlugin, INTPTR  iReq, ePITicks  iFlag );


/** @brief  Enables/Disables the reception of the PICBREQ_COORD message in PI_Msg() at each mouse move.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iReq     The window on which we want to grab the mouse move messages.
    @param[in]  iFlag    1 = enable, 0 = disable.

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @see  PI_Msg()

    @ingroup  sdk_PIMisc
*/
int  TVGrabCoords( PIPlugin*  iPlugin, INTPTR  iReq, int  iFlag );


/** @brief  Returns the image ID of the image in iProject, iLayer at position iImagePosition.

    Returns also the data ID although I don't know what use it can have in plugins...

    @param[in]   iPlugin      This plugin's reference.
    @param[in]   iProjectID   The ID of the project containing the image, or 0 to use the current project.
    @param[in]   iLayerID     The ID of the layer containing the image, or 0 to use the current layer.
    @param[in]   iImageIndex  The index of the image in the layer.
    @param[out]  oImageID     The returned image ID (buffer must be at least 21 bytes long).
    @param[out]  oDataID      The returned data ID (buffer must be at least 21 bytes long).

    @return  An error code: 0 = An error occured, 1 = Everything went well.

    @ingroup  sdk_PIMisc
*/
int  TVGetImageID( PIPlugin*  iPlugin, int  iProjectID, int  iLayerID, int  iImageIndex, char*  oImageID, char*  oDataID );


///@}


//=============================================================================================================== Deprecations


/** @deprecated  Use ePITicks instead. */
enum  TVDEPRECATED( "Use ePITicks instead" )  PITICKS_DEPRECATED
{
    PITICKS_FLAG_OFF      TVDEPRECATED( "Use kPITicks_Off instead"    ) = kPITicks_Off,
    PITICKS_FLAG_ON       TVDEPRECATED( "Use kPITicks_On instead"     ) = kPITicks_On,
    PITICKS_FLAG_NO_WAIT  TVDEPRECATED( "Use kPITicks_NoWait instead" ) = kPITicks_NoWait,
};


#ifdef __cplusplus
}
#endif


#endif // __pi_misc_HH

