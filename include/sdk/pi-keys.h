/** @copyright (c) 2002-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-keys.h
*  @ingroup  sdk_TVPA
*
*  @brief  xxxx
*/

#ifndef __pi_keys_HH
#define __pi_keys_HH


#include "pi-basics.h"


#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================================================


/** @defgroup  sdk_PIKeys  Stuff related to the keyframes.

    @brief  The keyframes.
*/


//===============================================================================================================


/** @brief  The version of the PIKeys API.

    @ingroup  sdk_PIKeys
*/
#define  PIKEYS_VERSION  1


//===============================================================================================================


/** @brief  The different types of keys.

    @ingroup  sdk_PIKeys
*/
enum  ePIKeyType
{
    /** @brief  A 32bit integer. */
    kPIKeyType_Int     = 0,

    /** @brief  A double. */
    kPIKeyType_Double  = 1,

    /** @brief  A color (PIKeyColor).
        @note  Setting the values need a PIKeyColor, getting the value will get a PIPixel.
        @todo //CHECK: or PIPixel32 ?
     */
    kPIKeyType_Color   = 2,

    /** @brief  A color (PIPixel).
        @todo //CHECK: or PIPixel32 ? */
    kPIKeyType_UColor  = 3,

    /** @brief  A boolean value, represented by a 32bit integer. */
    kPIKeyType_Bool    = 4,

    /** @brief  A position in space (PIKeyXYZ). */
    kPIKeyType_XYZ     = 5,

    /** @brief  A direction in space (PIKeyHPB). */
    kPIKeyType_HPB     = 6
};


//===============================================================================================================


/** @brief  The value of a key of type KEY_TYPE_XYZ.

    @ingroup  sdk_PIKeys
*/
typedef  struct  PIKeyXYZ
{
    /** @brief  The X coordinate. */
    double  x;

    /** @brief  The Y coordinate. */
    double  y;

    /** @brief  The Z coordinate. */
    double  z;
} PIKeyXYZ;


/** @brief  The value of a key of type KEY_TYPE_HPB.

    @ingroup  sdk_PIKeys
*/
typedef  struct  PIKeyHPB
{
    /** @brief  The H (heading) angle. */
    double  h;

    /** @brief  The P (pitch) angle. */
    double  p;

    /** @brief  The B (bank) angle. */
    double  b;
} PIKeyHPB;


/** @brief  The value of a key of type KEY_TYPE_COLOR.

    @ingroup  sdk_PIKeys
*/
typedef  struct  PIKeyColor
{
    /** @brief  The red component of the color. */
    double  r;

    /** @brief  The green component of the color. */
    double  g;

    /** @brief  The blue component of the color. */
    double  b;

    /** @brief  The alpha component of the color. */
    double  a;
} PIKeyColor;


//===============================================================================================================


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
    - "global" : it must be one of kKeyID_* enumerators/macros, which are strictly negative values.
    - ID 0 is reserved for when TVPaint tells your plugin that several parameters have changed at once, it can't be used for defining a parameter.

    Only keys with the same global IDs will be copiable from one filter to another.

    @ingroup  sdk_PIKeys
*/
typedef  struct  PIKeys
{
//---------------------------------------------------------------------------------------------------------------
///@name  Plugin section
///@{
    /** @brief  The key set's name. */
    char*  name;

    /** @brief  This callback is called by TVPaint whenever the keys change.

        You have to provide this function.
        You should update the GUI (and possible preview) of your plugin to reflect the new values of the keys.
        It is called after the user changes interactively the value, after a setval(), a load(), or any such action.

        @param[in]  iKeys  The set of keys.
        @param[in]  iID    The ID of the parameter which has changed (if = 0, several, or all, parameters have changed).
     */
    void  (*refresh)( struct PIKeys*  iKeys, int  iID );

    /** @brief  User Data. You can put any pointer in here, TVPaint will never access and/or change it.
        Usually, you put your plugin's address here.
     */ 
    void*  pidata;
///@}

//---------------------------------------------------------------------------------------------------------------
///@name  Host section
///@{
    /** @brief  Adds a new keyable parameter to the key set.

        @param[in]  iKeys   The set of keys.
        @param[in]  iID     Key ID (must be > 0, or one of the global key IDs kKeyID_*).
        @param[in]  iName   The name of the parameter.
        @param[in]  iType   The type of the parameter (one of ePIKeyType).
        @param[in]  iValue  The default value of the parameter.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
    */
    int  (*addparam)( struct PIKeys*  iKeys, int  iID, const char*  iName, int  iType, const void*  iValue );

    /** @brief  Changes the value of the specified parameter at the current position/time (set in TVPaint's timeline).
        This will call the refresh() function, so you don't have to update the user interface right now.

        @param[in]  iKeys   The set of keys.
        @param[in]  iID     Key ID (must be > 0, or one of the global key IDs kKeyID_*).
        @param[in]  iValue  The new value to set.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*setval)( struct PIKeys*  iKeys, int  iID, const void*  iValue );

    /** @brief  Gets the current value of a parameter. Usually called in PI_Start() / PI_Work() to get the current value of each parameter.

        @param[in]  iKeys   The set of keys.
        @param[in]  iID     Key ID (must be > 0, or one of the global key IDs kKeyID_*).
        @param[in]  oValue  The returned value.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*getval)( struct PIKeys*  iKeys, int  iID, void*  oValue );

    /** @brief  Gets the value of a parameter at a specific position/time.
        This is useful to do such things as motion blur or 'streaks'.

        @param[in]  iKeys   The set of keys.
        @param[in]  iID     Key ID (must be > 0, or one of the global key IDs kKeyID_*).
        @param[in]  iTime   The time at which to get the value (@todo //TODO: iTime is given in what units : seconds ? frames ? eyeblinks ?).
        @param[in]  oValue  The returned value.

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*getvalpos)( struct PIKeys*  iKeys, int  iID, double  iTime, void*  oValue );

    /** @brief  Saves the keys to the specified file in the specified section.
        If you want to save your keys in the user configuration file, set iFile to NULL and iSection to your plugin's name.
        You can also save the keys in an open 'StringFile'.

        @param[in]  iKeys     The set of keys.
        @param[in]  iFile     The 'StringFile' to save to, or NULL to save to the user configuration file.
        @param[in]  iSection  The name of the section to save to. Often, this is the plug-in name.

        @return  An error code: 0 = An error occured, 1 = Everything went well.

        @see  TVOpenStringFile()
     */
    int  (*save)( struct PIKeys*  iKeys, void*  iFile, const char*  iSection );

    /** @brief  Loads the keys from the specified file in the specified section.
        If you want to load your keys from the user configuration file, set iFile to NULL and iSection to your plugin's name.
        You can also load the keys from an open 'StringFile'.
        The parameters have to be created before loading the keys (with addparam()). 

        @param[in]  iKeys     The set of keys.
        @param[in]  iFile     The 'StringFile' to load from, or NULL to load from the user configuration file.
        @param[in]  iSection  The name of the section to load from. Often, this is the plug-in name.

        @return  An error code: 0 = An error occured, 1 = Everything went well.

        @see  TVOpenStringFile(), addparam()
     */
    int  (*load)( struct PIKeys*  iKeys, void*  iFile, const char*  iSection );

    /** @brief  Frees all memory associated with this keys, iKeys included.
        DON'T access it afterwards !

        @param[in]  iKeys  The set of keys.

        @see  TVAllocKeys()
     */
    void  (*free)( struct PIKeys*  iKeys );

    /** @brief  Locks the refresh of the GUI until a call to unlock.
        Use this when you make many changes to the keys at once to avoid a big slow-down.

        @param[in]  iKeys  The set of keys.

        @see  unlock()
     */
    void  (*lock)( struct PIKeys*  iKeys );

    /** @brief  Unlocks the refresh of the GUI.
        This will usually call the refresh function once with an ID of 0 to refresh all the parameters
        that have been modified since the call to lock().

        @param[in]  iKeys  The set of keys.

        @see  lock()
     */
    void  (*unlock)( struct PIKeys*  iKeys );

    /** @brief  Removes a keyable parameter from the key set.

        @param[in]  iKeys  The set of keys.
        @param[in]  iID    The ID of the parameter to remove.

        @see  addparam()
      */
    void  (*removeparam)( struct PIKeys*  iKeys, int  iID );

    /** @brief  Copies a keyable parameter.

        @todo //TODO: please explain.

        @param[in]   iKeysSrc  The set of keys to copy from.
        @param[in]   iIDSrc    The ID of the parameter to copy.
        @param[out]  oKeysDst  The set of keys to copy to.
        @param[in]   iIDDst    The ID of the parameter to copy to.
        @param[in]   iDeltaT   Some time offset ??!?!???!

        @return  An error code: 0 = An error occured, 1 = Everything went well.
     */
    int  (*copyparam)( struct PIKeys*  iKeysSrc, int  iIDSrc, struct PIKeys*  oKeysDst, int  iIDDst, double  iDeltaT );

    /** @brief  Renames a keyable parameter.

        @param[in]  iKeys  The set of keys.
        @param[in]  iID    The ID of the parameter to rename.
        @param[in]  iName  The new name.
     */
    void  (*renameparam)( struct PIKeys*  iKeys, int  iID, const char*  iName );
///@}

///@name  Plugin section
///@{
    /** @todo //TODO: ?!?!? */
    void  (*posrefreshCB)( struct PIKeys*  iKeys ); // callback
///@}
} PIKeys;


//===============================================================================================================


// Default IDs for inter-filter copy/paste compatibility


///@name  Constants.
///@{

/** @brief  The ID of the first global parameter.

    @ingroup  sdk_PIKeys
*/
#define  kKeyID_Start  (-65536)

/** @brief  The (maximum) size of a group of parameters.

    @ingroup  sdk_PIKeys
*/
#define  kKeyID_MaxSize  256

///@}


/** @brief  ID offsets.

    @ingroup  sdk_PIKeys
*/
enum  eKeyIDOffset
{
    /** @brief  The offset of the global radius parameter. */
    kKeyIDOffset_Radius      = ( -4),

    /** @brief  The offset of the global heading angle parameter. */
    kKeyIDOffset_H           = ( -5),

    /** @brief  The offset of the global pitch angle parameter. */
    kKeyIDOffset_P           = ( -6),

    /** @brief  The offset of the global bank angle parameter. */
    kKeyIDOffset_B           = ( -7),

    /** @brief  The offset of the global color parameter. */
    kKeyIDOffset_Color       = ( -8),

    /** @brief  The offset of the global horizontal scale parameter. */
    kKeyIDOffset_Width       = ( -9),

    /** @brief  The offset of the global vertical scale parameter. */
    kKeyIDOffset_Height      = (-10),

    /** @brief  The offset of the global pressure parameter. */
    kKeyIDOffset_Pressure    = (-11),

    /** @brief  The offset of the global altitude parameter. */
    kKeyIDOffset_Altitude    = (-12),

    /** @brief  The offset of the global azimuth parameter. */
    kKeyIDOffset_Azimuth     = (-13),

    /** @brief  The offset of the global fingerwheel parameter. */
    kKeyIDOffset_FingerWheel = (-14),

    /** @brief  The offset of the global speed parameter. */
    kKeyIDOffset_Speed       = (-15),

    /** @brief  The offset of the global down parameter. */
    kKeyIDOffset_Down        = (-16),

    // Once upon a time, there was a kKeyIDOffset_Linear with value (-17) here... It is now deprecated.

    /** @brief  The offset of the global noise parameter. */
    kKeyIDOffset_Noise       = (-18),

    /** @brief  The offset of the global motion blur parameter. */
    kKeyIDOffset_MotionBlur  = (-19),

    /** @brief  The offset of the global opacity parameter. */
    kKeyIDOffset_Opacity     = (-20),

    /** @brief  The offset of the global position parameter. */
    kKeyIDOffset_XYZ         = (-21),

    /** @brief  The offset of the global rotations parameter. */
    kKeyIDOffset_HPB         = (-22),

    /** @brief  The offset of the global quaternion parameter. */
    kKeyIDOffset_Quaternion  = (-23),
};


/** @brief  Simple global IDs.

    @ingroup  sdk_PIKeys
*/
enum  eKeyID
{
    /** @brief  The radius of something (in pixels). type=double */
    kKeyID_Radius      = (kKeyID_Start + kKeyIDOffset_Radius),

    /** @brief  The heading angle (in degrees). type=double */
    kKeyID_H           = (kKeyID_Start + kKeyIDOffset_H),

    /** @brief  The pitch angle (in degrees). type=double */
    kKeyID_P           = (kKeyID_Start + kKeyIDOffset_P),

    /** @brief  The bank angle (in degrees). type=double */
    kKeyID_B           = (kKeyID_Start + kKeyIDOffset_B),

    /** @brief  The color of something. type=color
        @todo //CHECK: what exact type is this ???? */
    kKeyID_Color       = (kKeyID_Start + kKeyIDOffset_Color),

    /** @brief  The horizontal scale (range=[0-1]). type=double */
    kKeyID_Width       = (kKeyID_Start + kKeyIDOffset_Width),

    /** @brief  The vertical scale (range=[0-1]). type=double */
    kKeyID_Height      = (kKeyID_Start + kKeyIDOffset_Height),

    /** @brief  The pressure provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
    kKeyID_Pressure    = (kKeyID_Start + kKeyIDOffset_Pressure),

    /** @brief  The altitude provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
    kKeyID_Altitude    = (kKeyID_Start + kKeyIDOffset_Altitude),

    /** @brief  The azimuth provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
    kKeyID_Azimuth     = (kKeyID_Start + kKeyIDOffset_Azimuth),

    /** @brief  The fingerwheel provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
    kKeyID_FingerWheel = (kKeyID_Start + kKeyIDOffset_FingerWheel),

    /** @brief  x @todo //TODO: */
    kKeyID_Speed       = (kKeyID_Start + kKeyIDOffset_Speed),

    /** @brief  x @todo //WTF: Pointer device state. When kKeyID_Down == 1, the pointer is visible.  When kKeyID_Down == 0, the pointer is invisible. type=bool(represented by an integer) */
    kKeyID_Down        = (kKeyID_Start + kKeyIDOffset_Down),

    /** @brief  "Path noise value" (whatever that means...). type=double
        @todo //CHECK: what is this ???? */
    kKeyID_Noise       = (kKeyID_Start + kKeyIDOffset_Noise),

    /** @brief  The path motion blur value (range=[0-1]). type=double */
    kKeyID_MotionBlur  = (kKeyID_Start + kKeyIDOffset_MotionBlur),

    /** @brief  The opacity of something (range=[0-1]). type=double */
    kKeyID_Opacity     = (kKeyID_Start + kKeyIDOffset_Opacity),

    /** @brief  The position of something in project coordinates (in pixels). Z is rarely used. type=PIKeyXYZ */
    kKeyID_XYZ         = (kKeyID_Start + kKeyIDOffset_XYZ),

    /** @brief  The rotations of something (in degrees). type=PIKeyHPB */
    kKeyID_HPB         = (kKeyID_Start + kKeyIDOffset_HPB),

    /** @brief  The rotation of something (expressed as a quaternion). type=?!?!??! 
        @todo //CHECK: what type is this ???? */
    kKeyID_Quaternion  = (kKeyID_Start + kKeyIDOffset_Quaternion),
};


///@name  Indexed global IDs.
///@{

// In C, these have to be macros :(

/** @brief  The radius of something (in pixels). type=double */
#define  kKeyID_Radius_N( iiNum )       (kKeyID_Radius      - (iiNum)*kKeyID_MaxSize)

/** @brief  The heading angle (in degrees). type=double */
#define  kKeyID_H_N( iiNum )            (kKeyID_H           - (iiNum)*kKeyID_MaxSize)

/** @brief  The pitch angle (in degrees). type=double */
#define  kKeyID_P_N( iiNum )            (kKeyID_P           - (iiNum)*kKeyID_MaxSize)

/** @brief  The bank angle (in degrees). type=double */
#define  kKeyID_B_N( iiNum )            (kKeyID_B           - (iiNum)*kKeyID_MaxSize)

/** @brief  The color of something. type=color
    @todo //CHECK: what exact type is this ???? */
#define  kKeyID_Color_N( iiNum )        (kKeyID_Color       - (iiNum)*kKeyID_MaxSize)

/** @brief  The horizontal scale (range=[0-1]). type=double */
#define  kKeyID_Width_N( iiNum )        (kKeyID_Width       - (iiNum)*kKeyID_MaxSize)

/** @brief  The vertical scale (range=[0-1]). type=double */
#define  kKeyID_Height_N( iiNum )       (kKeyID_Height      - (iiNum)*kKeyID_MaxSize)

/** @brief  The pressure provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
#define  kKeyID_Pressure_N( iiNum )     (kKeyID_Pressure    - (iiNum)*kKeyID_MaxSize)

/** @brief  The altitude provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
#define  kKeyID_Altitude_N( iiNum )     (kKeyID_Altitude    - (iiNum)*kKeyID_MaxSize)

/** @brief  The azimuth provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
#define  kKeyID_Azimuth_N( iiNum )      (kKeyID_Azimuth     - (iiNum)*kKeyID_MaxSize)

/** @brief  The fingerwheel provided by the stylus, usually set by the stroke recorder (range=[0-1]). type=double */
#define  kKeyID_FingerWheel_N( iiNum )  (kKeyID_FingerWheel - (iiNum)*kKeyID_MaxSize)

/** @brief  x @todo //TODO: */
#define  kKeyID_Speed_N( iiNum )        (kKeyID_Speed       - (iiNum)*kKeyID_MaxSize)

/** @brief  x @todo //WTF: Pointer device state. When kKeyID_Down == 1, the pointer is visible.  When kKeyID_Down == 0, the pointer is invisible. type=bool(represented by an integer) */
#define  kKeyID_Down_N( iiNum )         (kKeyID_Down        - (iiNum)*kKeyID_MaxSize)

/** @brief  "Path noise value" (whatever that means...). type=double
    @todo //CHECK: what is this ???? */
#define  kKeyID_Noise_N( iiNum )        (kKeyID_Noise       - (iiNum)*kKeyID_MaxSize)

/** @brief  The path motion blur value (range=[0-1]). type=double */
#define  kKeyID_MotionBlur_N( iiNum )   (kKeyID_MotionBlur  - (iiNum)*kKeyID_MaxSize)

/** @brief  The opacity of something (range=[0-1]). type=double */
#define  kKeyID_Opacity_N( iiNum )      (kKeyID_Opacity     - (iiNum)*kKeyID_MaxSize)

/** @brief  The position of something in project coordinates (in pixels). Z is rarely used. type=PIKeyXYZ */
#define  kKeyID_XYZ_N( iiNum )          (kKeyID_XYZ         - (iiNum)*kKeyID_MaxSize)

/** @brief  The rotations of something (in degrees). type=PIKeyHPB */
#define  kKeyID_HPB_N( iiNum )          (kKeyID_HPB         - (iiNum)*kKeyID_MaxSize)

/** @brief  The rotation of something (expressed as a quaternion). type=?!?!??! 
    @todo //CHECK: what type is this ???? */
#define  kKeyID_Quaternion_N( iiNum )   (kKeyID_Quaternion  - (iiNum)*kKeyID_MaxSize)

///@}


//===============================================================================================================


/** @brief  Allocates a PIKeys structure.

    @param[in]  iPlugin  This plugin's reference.
    @param[in]  iName    Name of the key structure. Usually, the Plugin's name.
    @param[in]  iNameID  Name for identification (load, save). Don't localise this parameter.

    @return  A new PIKeys structure, or NULL if failure.

    @see  TVCloseFunction()

    @ingroup  sdk_PIKeys
*/
PIKeys*  TVAllocKeys( PIPlugin*  iPlugin, const char*  iName, const char*  iNameID );


//===============================================================================================================  Deprecations


/** @deprecated  Use ePIKeyType instead. */
enum  TVDEPRECATED( "Use ePIKeyType instead" )  KEY_TYPE
{
    KEY_TYPE_INT     TVDEPRECATED( "Use kPIKeyType_Int instead"    ) = kPIKeyType_Int,
    KEY_TYPE_DOUBLE  TVDEPRECATED( "Use kPIKeyType_Double instead" ) = kPIKeyType_Double,
    KEY_TYPE_COLOR   TVDEPRECATED( "Use kPIKeyType_Color instead"  ) = kPIKeyType_Color,
    KEY_TYPE_UCOLOR  TVDEPRECATED( "Use kPIKeyType_UColor instead" ) = kPIKeyType_UColor,
    KEY_TYPE_BOOL    TVDEPRECATED( "Use kPIKeyType_Bool instead"   ) = kPIKeyType_Bool,
    KEY_TYPE_XYZ     TVDEPRECATED( "Use kPIKeyType_XYZ instead"    ) = kPIKeyType_XYZ,
    KEY_TYPE_HPB     TVDEPRECATED( "Use kPIKeyType_HPB instead"    ) = kPIKeyType_HPB
};


/** @deprecated  Renamed as PIKeyXYZ. */
TVDEPRECATED( "Renamed as PIKeyXYZ" )
typedef  PIKeyXYZ  KeyXYZ;


/** @deprecated  Renamed as PIKeyHPB. */
TVDEPRECATED( "Renamed as PIKeyHPB" )
typedef  PIKeyHPB  KeyHPB;


/** @deprecated  Renamed as PIKeyColor. */
TVDEPRECATED( "Renamed as PIKeyColor" )
typedef  PIKeyColor  KeyColor;


/** @deprecated  Use kKeyID_Start instead. */
#define  KEY_ID_START  kKeyID_Start

/** @deprecated  Use kKeyID_MaxSize instead. */
#define  KEY_ID_RES  kKeyID_MaxSize


/** @deprecated  Use eKeyIDOffset instead. */
enum  TVDEPRECATED( "Use eKeyIDOffset instead" )  KEY_OFFSET_ENUM_DEPRECATED
{
    KEY_OFFSET_RADIUS    TVDEPRECATED( "Use kKeyIDOffset_Radius instead."      ) = kKeyIDOffset_Radius,
    KEY_OFFSET_H         TVDEPRECATED( "Use kKeyIDOffset_H instead."           ) = kKeyIDOffset_H,
    KEY_OFFSET_P         TVDEPRECATED( "Use kKeyIDOffset_P instead."           ) = kKeyIDOffset_P,
    KEY_OFFSET_B         TVDEPRECATED( "Use kKeyIDOffset_B instead."           ) = kKeyIDOffset_B,
    KEY_OFFSET_COLOR     TVDEPRECATED( "Use kKeyIDOffset_Color instead."       ) = kKeyIDOffset_Color,
    KEY_OFFSET_WIDTH     TVDEPRECATED( "Use kKeyIDOffset_Width instead."       ) = kKeyIDOffset_Width,
    KEY_OFFSET_HEIGHT    TVDEPRECATED( "Use kKeyIDOffset_Height instead."      ) = kKeyIDOffset_Height,
    KEY_OFFSET_PRESSURE  TVDEPRECATED( "Use kKeyIDOffset_Pressure instead."    ) = kKeyIDOffset_Pressure,
    KEY_OFFSET_ALTITUDE  TVDEPRECATED( "Use kKeyIDOffset_Altitude instead."    ) = kKeyIDOffset_Altitude,
    KEY_OFFSET_AZIMUTH   TVDEPRECATED( "Use kKeyIDOffset_Azimuth instead."     ) = kKeyIDOffset_Azimuth,
    KEY_OFFSET_FWHEEL    TVDEPRECATED( "Use kKeyIDOffset_FingerWheel instead." ) = kKeyIDOffset_FingerWheel,
    KEY_OFFSET_SPEED     TVDEPRECATED( "Use kKeyIDOffset_Speed instead."       ) = kKeyIDOffset_Speed,
    KEY_OFFSET_DOWN      TVDEPRECATED( "Use kKeyIDOffset_Down instead."        ) = kKeyIDOffset_Down,
    KEY_OFFSET_LINEAR    TVDEPRECATED( "No replacement."                       ) = (-17),
    KEY_OFFSET_NOISE     TVDEPRECATED( "Use kKeyIDOffset_Noise instead."       ) = kKeyIDOffset_Noise,
    KEY_OFFSET_MBLUR     TVDEPRECATED( "Use kKeyIDOffset_MotionBlur instead."  ) = kKeyIDOffset_MotionBlur,
    KEY_OFFSET_OPACITY   TVDEPRECATED( "Use kKeyIDOffset_Opacity instead."     ) = kKeyIDOffset_Opacity,
    KEY_OFFSET_XYZ       TVDEPRECATED( "Use kKeyIDOffset_XYZ instead."         ) = kKeyIDOffset_XYZ,
    KEY_OFFSET_HPB       TVDEPRECATED( "Use kKeyIDOffset_HPB instead."         ) = kKeyIDOffset_HPB,
    KEY_OFFSET_QUAT      TVDEPRECATED( "Use kKeyIDOffset_Quaternion instead."  ) = kKeyIDOffset_Quaternion,
};


/** @deprecated  Use eKeyID instead. */
enum  TVDEPRECATED( "Use eKeyID instead" )  KEY_ID_ENUM_DEPRECATED
{
    KEY_ID_RADIUS    TVDEPRECATED( "Use kKeyID_Radius instead."      ) = kKeyID_Radius,
    KEY_ID_H         TVDEPRECATED( "Use kKeyID_H instead."           ) = kKeyID_H,
    KEY_ID_P         TVDEPRECATED( "Use kKeyID_P instead."           ) = kKeyID_P,
    KEY_ID_B         TVDEPRECATED( "Use kKeyID_B instead."           ) = kKeyID_B,
    KEY_ID_COLOR     TVDEPRECATED( "Use kKeyID_Color instead."       ) = kKeyID_Color,
    KEY_ID_WIDTH     TVDEPRECATED( "Use kKeyID_Width instead."       ) = kKeyID_Width,
    KEY_ID_HEIGHT    TVDEPRECATED( "Use kKeyID_Height instead."      ) = kKeyID_Height,
    KEY_ID_PRESSURE  TVDEPRECATED( "Use kKeyID_Pressure instead."    ) = kKeyID_Pressure,
    KEY_ID_ALTITUDE  TVDEPRECATED( "Use kKeyID_Altitude instead."    ) = kKeyID_Altitude,
    KEY_ID_AZIMUTH   TVDEPRECATED( "Use kKeyID_Azimuth instead."     ) = kKeyID_Azimuth,
    KEY_ID_FWHEEL    TVDEPRECATED( "Use kKeyID_FingerWheel instead." ) = kKeyID_FingerWheel,
    KEY_ID_SPEED     TVDEPRECATED( "Use kKeyID_Speed instead."       ) = kKeyID_Speed,
    KEY_ID_DOWN      TVDEPRECATED( "Use kKeyID_Down instead."        ) = kKeyID_Down,
    KEY_ID_LINEAR    TVDEPRECATED( "No replacement."                 ) = 17,
    KEY_ID_NOISE     TVDEPRECATED( "Use kKeyID_Noise instead."       ) = kKeyID_Noise,
    KEY_ID_MBLUR     TVDEPRECATED( "Use kKeyID_MotionBlur instead."  ) = kKeyID_MotionBlur,
    KEY_ID_OPACITY   TVDEPRECATED( "Use kKeyID_Opacity instead."     ) = kKeyID_Opacity,
    KEY_ID_XYZ       TVDEPRECATED( "Use kKeyID_XYZ instead."         ) = kKeyID_XYZ,
    KEY_ID_HPB       TVDEPRECATED( "Use kKeyID_HPB instead."         ) = kKeyID_HPB,
    KEY_ID_QUAT      TVDEPRECATED( "Use kKeyID_Quaternion instead."  ) = kKeyID_Quaternion,
};


///@name  Deprecated, use kKeyID_*_N instead.
///@{

/** @deprecated  Use kKeyID_Radius_N instead. */
#define  KEY_ID_RADIUS_N( iiNum )    TVDEPRECATED( "Use kKeyID_Radius_N instead."      )  kKeyID_Radius_N( iiNum )
/** @deprecated  Use kKeyID_H_N instead. */
#define  KEY_ID_H_N( iiNum )         TVDEPRECATED( "Use kKeyID_H_N instead."           )  kKeyID_H_N( iiNum )
/** @deprecated  Use kKeyID_P_N instead. */
#define  KEY_ID_P_N( iiNum )         TVDEPRECATED( "Use kKeyID_P_N instead."           )  kKeyID_P_N( iiNum )
/** @deprecated  Use kKeyID_B_N instead. */
#define  KEY_ID_B_N( iiNum )         TVDEPRECATED( "Use kKeyID_B_N instead."           )  kKeyID_B_N( iiNum )
/** @deprecated  Use kKeyID_Color_N instead. */
#define  KEY_ID_COLOR_N( iiNum )     TVDEPRECATED( "Use kKeyID_Color_N instead."       )  kKeyID_Color_N( iiNum )
/** @deprecated  Use kKeyID_Width_N instead. */
#define  KEY_ID_WIDTH_N( iiNum )     TVDEPRECATED( "Use kKeyID_Width_N instead."       )  kKeyID_Width_N( iiNum )
/** @deprecated  Use kKeyID_Height_N instead. */
#define  KEY_ID_HEIGHT_N( iiNum )    TVDEPRECATED( "Use kKeyID_Height_N instead."      )  kKeyID_Height_N( iiNum )
/** @deprecated  Use kKeyID_Pressure_N instead. */
#define  KEY_ID_PRESSURE_N( iiNum )  TVDEPRECATED( "Use kKeyID_Pressure_N instead."    )  kKeyID_Pressure_N( iiNum )
/** @deprecated  Use kKeyID_Altitude_N instead. */
#define  KEY_ID_ALTITUDE_N( iiNum )  TVDEPRECATED( "Use kKeyID_Altitude_N instead."    )  kKeyID_Altitude_N( iiNum )
/** @deprecated  Use kKeyID_Azimuth_N instead. */
#define  KEY_ID_AZIMUTH_N( iiNum )   TVDEPRECATED( "Use kKeyID_Azimuth_N instead."     )  kKeyID_Azimuth_N( iiNum )
/** @deprecated  Use kKeyID_FingerWheel_N instead. */
#define  KEY_ID_FWHEEL_N( iiNum )    TVDEPRECATED( "Use kKeyID_FingerWheel_N instead." )  kKeyID_FingerWheel_N( iiNum )
/** @deprecated  Use kKeyID_Speed_N instead. */
#define  KEY_ID_SPEED_N( iiNum )     TVDEPRECATED( "Use kKeyID_Speed_N instead."       )  kKeyID_Speed_N( iiNum )
/** @deprecated  Use kKeyID_Down_N instead. */
#define  KEY_ID_DOWN_N( iiNum )      TVDEPRECATED( "Use kKeyID_Down_N instead."        )  kKeyID_Down_N( iiNum )
/** @deprecated  No replacement. */
#define  KEY_ID_LINEAR_N( iiNum )    TVDEPRECATED( "No replacement."                   )  (kKeyID_Start-17+(iiNum)*kKeyID_MaxSize)
/** @deprecated  Use kKeyID_Noise_N instead. */
#define  KEY_ID_NOISE_N( iiNum )     TVDEPRECATED( "Use kKeyID_Noise_N instead."       )  kKeyID_Noise_N( iiNum )
/** @deprecated  Use kKeyID_MotionBlur_N instead. */
#define  KEY_ID_MBLUR_N( iiNum )     TVDEPRECATED( "Use kKeyID_MotionBlur_N instead."  )  kKeyID_MotionBlur_N( iiNum )
/** @deprecated  Use kKeyID_Opacity_N instead. */
#define  KEY_ID_OPACITY_N( iiNum )   TVDEPRECATED( "Use kKeyID_Opacity_N instead."     )  kKeyID_Opacity_N( iiNum )
/** @deprecated  Use kKeyID_XYZ_N instead. */
#define  KEY_ID_XYZ_N( iiNum )       TVDEPRECATED( "Use kKeyID_XYZ_N instead."         )  kKeyID_XYZ_N( iiNum )
/** @deprecated  Use kKeyID_HPB_N instead. */
#define  KEY_ID_HPB_N( iiNum )       TVDEPRECATED( "Use kKeyID_HPB_N instead."         )  kKeyID_HPB_N( iiNum )
/** @deprecated  Use kKeyID_Quaternion_N instead. */
#define  KEY_ID_QUAT_N( iiNum )      TVDEPRECATED( "Use kKeyID_Quaternion_N instead."  )  kKeyID_Quaternion_N( iiNum )

///@}


#ifdef __cplusplus
};
#endif


#endif // __pi_keys_HH
