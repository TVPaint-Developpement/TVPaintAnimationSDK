/** @copyright (c) 2002-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-profile.h
*  @ingroup  sdk_TVPA
*
*  @brief  A profile.
*/

#ifndef __pi_profile_HH
#define __pi_profile_HH


#include "TVPaintAnimationSDK/pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIProfile  Stuff related to profiles.

    A profile is a curve mapping the segment [0,1] to [0,1].
*/


//===============================================================================================================


/** @brief  The list of possible shapes of a newly created profile.

    @ingroup  sdk_PIProfile
 */
typedef  enum  ePIProfileShape
{
    /** @brief  Horizontal at 0. */
    kPIProfileShape_00 = 0,
    /** @brief  Ascending ramp from 0 to 1. */
    kPIProfileShape_01 = 1,
    /** @brief  Descending ramp from 1 to 0. */
    kPIProfileShape_10 = 2,
    /** @brief  Horizontal at 1. */
    kPIProfileShape_11 = 3 
} ePIProfileShape;


//===============================================================================================================


/** @brief  A profile.
 
    A profile is a curve mapping the segment [0,1] to [0,1].

    @ingroup  sdk_PIProfile
 */
typedef  struct  PIProfile
{
//---------------------------------------------------------------------------------------------------------------
///@name  Plugin section
///@{
    /** @brief  The structures version number, MUST be sizeof(PIProfile).
     */
    int  Version;

    /** @brief  A callback called by TVPaint on the plugin every time the profile is modified by TVPaint's profile editor.

        You can set it to whatever you want, most often it will refresh some sort of preview of the effect of the plugin.
     
        @param[in]  iUserData  The UserData field from its PIProfile.
     */
    void  (*Refresh)( void*  iUserData );

    /** @brief  This is some pointer you get back in the Refresh() CB.
        TVPaint won't access the data pointed to by UserData, it can be anything you want.
        Most often set to the plugin's reference.
     */
    void*  UserData;
///@}

//---------------------------------------------------------------------------------------------------------------
///@name  Host section
///@{
    /** @brief  Will delete/free the PIProfile structure.

        @param[in]  iProfile  The profile to delete.
     */
    void  (*Delete)( struct PIProfile*  iProfile );

    /** @brief  This will open the profile editor panel in TVPaint.

        @param[in]  iProfile  The profile to edit.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*OpenEdit)( struct PIProfile*  iProfile );

    /** @brief  This will close the profile editor panel in TVPaint.

        @param[in]  iProfile  The profile to stop editing.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*CloseEdit)( struct PIProfile*  iProfile );

    /** @brief  This will load a profile from a file.

        @param[in]  iProfile  The profile to load.
        @param[in]  iFile     The string file to load from.  @todo //CHECK: does NULL work ?
        @param[in]  iSection  The section name to load from.

        @return  An error code: 0 = An error occured, 1 = Everything went well.

        @see  TVOpenStringFile() 
     */
    int  (*Load)( struct PIProfile*  iProfile, void*  iFile, const char*  iSection );

    /** @brief  This will save a profile to a file.

        @param[in]  iProfile  The profile to save.
        @param[in]  iFile     The string file to save to.  @todo //CHECK: does NULL work ?
        @param[in]  iSection  The section name to save to.

        @return  An error code: 0 = An error occured, 1 = Everything went well.

        @see  TVOpenStringFile() 
     */
    int  (*Save)( struct PIProfile*  iProfile, void*  iFile, const char*  iSection );

    /** @brief  This will evaluate the profile, i.e. sample it at the given number of points.

        @param[in]  iProfile  The profile to load.
        @param[in]  oDest     The sampled points.
        @param[in]  iCount    Number of samples of oDest.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*Evaluate)( struct PIProfile*  iProfile, float*  oDest, int  iCount );
///@}
} PIProfile;


//===============================================================================================================


/** @brief  Allocates a Profile.

    @param[in]  iPlugin   This plugin's reference.
    @param[in]  iName     The name of the new profile.
    @param[in]  iDefault  The default shape of the profile.
 
    @return  The newly allocated profile, or NULL is case of failure.

    @ingroup  sdk_PIProfile
 */
PIProfile*  TVAllocProfile( PIPlugin*  iPlugin, const char*  iName, ePIProfileShape  iDefault );


//=============================================================================================================== Deprecations


/** @deprecated  Use ePIProfileShape instead. */
enum  TVDEPRECATED( "Use ePIProfileShape instead" )  PIPROF_DEPRECATED
{
    /** @deprecated  Use kPIProfileShape_00 instead. */
    PIPROF_00  TVDEPRECATED( "Use kPIProfileShape_00 instead" ) = kPIProfileShape_00,
    /** @deprecated  Use kPIProfileShape_01 instead. */
    PIPROF_01  TVDEPRECATED( "Use kPIProfileShape_01 instead" ) = kPIProfileShape_01,
    /** @deprecated  Use kPIProfileShape_10 instead. */
    PIPROF_10  TVDEPRECATED( "Use kPIProfileShape_10 instead" ) = kPIProfileShape_10,
    /** @deprecated  Use kPIProfileShape_11 instead. */
    PIPROF_11  TVDEPRECATED( "Use kPIProfileShape_11 instead" ) = kPIProfileShape_11
};


#ifdef __cplusplus
}
#endif


#endif // __pi_profile_HH
