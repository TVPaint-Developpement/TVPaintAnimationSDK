/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Keys.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A keyset.
 * //TODO: //NAME: file+class
 */

#pragma once


#include "TVPaintAnimationSDK/Types.hpp"


#include "TVPaintAnimationSDK/pi-basics.h"


#include <functional>
#include <string>


namespace  nSDK {


//MARK: ========================================================================================================= cPI2Keys


/** @brief  A set of keys.

 The set of keys your filter needs for animation.\n
 You need to fill in the 'refresh' field, possibly the 'posrefreshCB' field too.\n
 You can use the 'pidata' field for your private data (usually you put a pointer to your plugin in there).

 A set of keys, also called keyset, or even keys in short, represent all the animatable values of a filter.\n
 A parameter, sometimes called a 'line' because it appears as a line in TVPaint's window, represent one of these animatable values.\n
 A key (note the singular) represent the value of one parameter at a given time.

 Each parameter has an ID to access it (and a name for the user to find it in the GUI...).\n
 These IDs are of two kinds (the third is forbidden...) :
 - specific to your filter : it must be strictly positive.
 - "global" : it must be one of KEY_ID_* macros, which are strictly negative values.
 - ID 0 is reserved for when TVPaint tells your plugin that several parameters have changed at once, it can't be used for defining a parameter.

 Only keys with the same global IDs will be copiable from one filter to another.

 @ingroup  sdk2_Keys
 */
class  cPI2Keys
{
public:
    SDK_NON_COPYABLE( cPI2Keys );

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Construction/Destruction
///@{
    ~cPI2Keys();
    cPI2Keys( cTV2Plugin*  iPlugin, const std::string&  iName, const std::string&  iNameID );
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Plugin section
///@{

//ZAP: ?!?!?!
//    /** @brief  User Data. You can put any pointer in here, TVPaint will never access and/or change it.
//        Usually, you put your plugin's address here.
//     */
//    void*  pidata;

    /** @brief  This callback is called by TVPaint whenever the keys change.

     You have to provide this function.
     You should update the GUI (and possible preview) of your plugin to reflect the new values of the keys.
     It is called after the user changes interactively the value, after a setval(), a load(), or any such action.

     @param[in]  iKeys  The set of keys.
     @param[in]  iID    The ID of the parameter which has changed (if = 0, several, or all, parameters have changed).
     */
    using  tUpdateViews = std::function< void  ( cPI2Keys*  iKeys, int  iID ) >;
    void  SetUpdateViews( tUpdateViews  iUpdateViews );
    void  CallUpdateViews( int  iID ); //TODO: private utilities

    /** @todo //TODO: ?!?!? */
    void  (*posrefreshCB)( cPI2Keys*  iKeys ); // callback

///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Host section
///@{
    /** @brief  Adds a new keyable parameter to the key set.

     @param[in]  iID     Key ID (must be > 0, or one of the global key IDs KEY_ID_*).
     @param[in]  iName   The name of the parameter.
     @param[in]  iType   The type of the parameter (one of ePIKeyType).
     @param[in]  iValue  The default value of the parameter.

     @return  //TODO:
     */
    int  AddParameter( int  iID, const std::string&  iName, int  iType, const void*  iValue );

    /** @brief  Changes the value of the specified parameter at the current position (set in TVPaint's timeline).
     This will call the refresh() function, so you don't have to update the user interface right now.

     @param[in]  iID     Key ID (must be > 0, or one of the global key IDs KEY_ID_*).
     @param[in]  iValue  The new value to set.

     @return  //TODO:
     */
    int  SetValue( int  iID, const void*  iValue );

    /** @brief  Gets the current value of a parameter. Usually called in PI_Start() / PI_Work() to get the current value of each parameter.

     @param[in]  iID     Key ID (must be > 0, or one of the global key IDs KEY_ID_*).
     @param[in]  oValue  The returned value.

     @return  //TODO:
     */
    int  GetValue( int  iID, void*  oValue );

    /** @brief  Gets the value of a parameter at a specific position (time).
     This is useful to do such things as motion blur or 'streaks'.

     @param[in]  iID     Key ID (must be > 0, or one of the global key IDs KEY_ID_*).
     @param[in]  iTime   The time at which to get the value (@todo //TODO: iTime is given in what units : seconds ? frames ? eyeblinks ?).
     @param[in]  oValue  The returned value.

     @return  //TODO:
     */
    int  GetValueAtPosition( int  iID, double  iTime, void*  oValue );

    /** @brief  Saves the keys to the specified file in the specified section.
     If you want to save your keys in the user configuration file, set iFile to NULL and iSection to your plugin's name.
     You can also save the keys in an open 'StringFile'.

     @param[in]  iFile     The 'StringFile' to save to, or NULL to save to the user configuration file.
     @param[in]  iSection  The name of the section to save to. Often, this is the plug-in name.

     @return  //TODO:

     @see  TVOpenStringFile()
     */
    int  Save( void*  iFile, const std::string&  iSection );

    /** @brief  Loads the keys from the specified file in the specified section.
     If you want to load your keys from the user configuration file, set iFile to NULL and iSection to your plugin's name.
     You can also load the keys from an open 'StringFile'.
     The parameters have to be created before loading the keys (with addparam()).

     @param[in]  iFile     The 'StringFile' to load from, or NULL to load from the user configuration file.
     @param[in]  iSection  The name of the section to load from. Often, this is the plug-in name.

     @return  //TODO:

     @see  TVOpenStringFile(), addparam()
     */
    int  Load( void*  iFile, const std::string&  iSection );

    /** @brief  Frees all memory associated with this keys, iKeys included.
     DON'T access it afterwards !

     //TODO: => dtor !
     @see  TVAllocKeys()
     */
    void  Free();

    /** @brief  Locks the refresh of the GUI until a call to unlock.
     Use this when you make many changes to the keys at once to avoid a big slow-down.

     @see  unlock()
     */
    void  Lock();

    /** @brief  Unlocks the refresh of the GUI.
     This will usually call the refresh function once with an ID of 0 to refresh all the parameters
     that have been modified since the call to lock().

     @see  lock()
     */
    void  Unlock();

    /** @brief  Removes a keyable parameter from the key set.

     @param[in]  iID    The ID of the parameter to remove.

     @see  addparam()
     */
    void  RemoveParameter( int  iID );

    /** @brief  Copies a keyable parameter.

     @todo //TODO: please explain.

     @param[in]   iIDSrc    The ID of the parameter to copy.
     @param[out]  oKeysDst  The set of keys to copy to.
     @param[in]   iIDDst    The ID of the parameter to copy to.
     @param[in]   iDeltaT   Some time offset ??!?!???!

     @return  //TODO:
     */
    int  CopyParameter( int  iIDSrc, struct PIKeys*  oKeysDst, int  iIDDst, double  iDeltaT );

    /** @brief  Renames a keyable parameter.

     @param[in]  iID    The ID of the parameter to rename.
     @param[in]  iName  The new name.
     */
    void  RenameParameter( int  iID, const std::string&  iName );
///@}

//---------------------------------------------------------------------------------------------------------------
public:
///@name  Access to Legacy Stuff (temporary)
///@{
    PIKeys*  LegacyKeys()  { return  mLegacyKeys; }
///@}

//---------------------------------------------------------------------------------------------------------------
private:
///@name  Member data
///@{
    /** @brief  The sdk1 key set. */
    PIKeys*  mLegacyKeys  {nullptr};
    tUpdateViews  mUpdateViews  {nullptr};
///@}
};


//===============================================================================================================


/** @brief  Allocates a PIKeys structure.

 @param[in]  iPlugin  This plugin's reference.
 @param[in]  iName    Name of the key structure. Usually, the Plugin's name.
 @param[in]  iNameID  Name for identification (load, save). Don't localise this parameter.

 @return  A new PIKeys structure, or NULL if failure.

 @see  TVCloseFunction()

 @ingroup  sdk_PIKeys
 */
cPI2Keys*  TV2AllocKeys( PIPlugin*  iPlugin, const std::string&  iName, const std::string&  iNameID );


} // namespace  nSDK
