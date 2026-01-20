/** @copyright (c) 1995-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  pi-basics.h
*  @ingroup  sdk_TVPA
*
*  @brief  The basic types used by the SDK.
*/

#ifndef __pi_basics_HH
#define __pi_basics_HH


#ifdef __cplusplus
extern "C" {
#endif


// version  5.00 for TVPaint  5.0 (Aura 1.0)
// version  5.01 for TVPaint  5.1 (Aura 1.0c)
// version  6.00 for TVPaint  6.0 (Aura 2)
// version  6.05 for TVPaint  6.5 (Aura 2.5)
// version  6.06 for TVPaint  6.6 (Aura 2.5b)
// version  7.00 for TVPaint  7.0 (Mirage 1.0)
// version  7.01 for TVPaint  7.1 (Mirage 1.01)
// version  7.02 for TVPaint  7.2 (Mirage 1.50)
// version  7.05 for TVPaint  8.0 (Animation)
// version  8.00 for TVPaint  8.1 (Animation)
// version  9.00 for TVPaint  9.0 (Animation)
// the big black hole...
// version 12.00 for TVPaint 12.0 (Animation)

/** @brief  The version of the SDK API. */
#define  PI_VERSION   12
/** @brief  The revision of the SDK API. */
#define  PI_REVISION  0

/** @brief  The version of the Callback API (private). */
#define  CALLBACK_VERSION   1
/** @brief  The revision of the Callback API (private). */
#define  CALLBACK_REVISION  0


#ifdef __cplusplus // [[deprecated]] exists in all reasonably modern C++ versions (> C++14)
/** @brief  A macro to mark something (function, struct, enum, typedef, ...) as deprecated. */
#define  TVDEPRECATED( iiString )  [[deprecated( iiString )]]
#elif __STDC_VERSION__ >= 202311L // [[deprecated]] exists since C23 (plain C)
/** @brief  A macro to mark something (function, struct, enum, typedef, ...) as deprecated. */
#define  TVDEPRECATED( iiString )  [[deprecated( iiString )]]
#else // [[deprecated]] doesn't exists in older C versions
/** @brief  A macro to mark something (function, struct, enum, typedef, ...) as deprecated. */
#define  TVDEPRECATED( iiString )
#endif


// _WIN64 is *the only one* to use : https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros
#ifdef _WIN64

/** @brief  An unsigned 64bit integer. */
typedef  unsigned _int64  PIUInt64;
/** @brief  Integral Ptr. Used to cast ptr to arithmetic type. BUT... what's the point ? */
typedef  unsigned _int64  INTPTR;

#define  strncasecmp  strnicmp  ///< Windows junk.

#else

/** @brief  An unsigned 64bit integer. */
typedef  unsigned long long  PIUInt64;

/** @brief  Integral Ptr. Used to cast ptr to arithmetic type. BUT... what's the point ? */
typedef  unsigned long long  INTPTR;

#define  strnicmp  strncasecmp  ///< Windows junk.

#endif

// Obsolete Windows calling conventions - define as empty for modern code
#define  FAR     ///< Windows junk.
#define  PASCAL  ///< Windows junk.
#define  WINAPI  ///< Windows junk.


#ifdef __cplusplus
static_assert( sizeof(PIUInt64) == 8, "Bad size" );
static_assert( sizeof(INTPTR) == 8, "Bad size" );
#endif


TVDEPRECATED( "Use builtin 'char' instead." )
/** @deprecated  Use builtin 'char' instead. */
typedef  char  TVCHAR;


/** @brief  An unsigned 8bit integer. */
typedef  unsigned char  PIUInt8;
#ifdef __cplusplus
static_assert( sizeof(PIUInt8) == 1, "Bad size" );
#endif


/** @brief  A signed 32bit integer. */
typedef  int  PIInt32;
#ifdef __cplusplus
static_assert( sizeof(PIInt32) == 4, "Bad size" );
#endif
#ifndef INT32
TVDEPRECATED( "Use PIInt32 instead." )
/** @deprecated  Use 'PIInt32' instead. */
typedef  PIInt32  INT32;
#endif


/** @brief  An unsigned 32bit integer. */
typedef  unsigned int  PIUInt32;
#ifdef __cplusplus
static_assert( sizeof(PIUInt32) == 4, "Bad size" );
#endif


#ifndef ULONG32
TVDEPRECATED( "Use PIUInt32 instead." )
/** @deprecated  Use 'PIUInt32' instead. */
typedef  PIUInt32  ULONG32;
#endif


/** @brief  Flags (usually or'ed together from some enumerators). */
typedef  PIUInt64  PIFlags;


// Forward declarations.

/** @brief  Forward declaration of PIBlock. */
typedef  struct  PIBlock           PIBlock;
/** @brief  Forward declaration of PIDisplay. */
typedef  struct  PIDisplay         PIDisplay;
/** @brief  Forward declaration of PIFileInfo. */
typedef  struct  PIFileInfo        PIFileInfo;
/** @brief  Forward declaration of PIFileRequester. */
typedef  struct  PIFileRequester   PIFileRequester;
/** @brief  Forward declaration of PIFont. */
typedef  struct  PIFont            PIFont;
/** @brief  Forward declaration of PIImageSequence. */
typedef  struct  PIImageSequence   PIImageSequence;
/** @brief  Forward declaration of PIKeyColor. */
typedef  struct  PIKeyColor        PIKeyColor;
/** @brief  Forward declaration of PIKeyHPB. */
typedef  struct  PIKeyHPB          PIKeyHPB;
/** @brief  Forward declaration of PIKeyXYZ. */
typedef  struct  PIKeyXYZ          PIKeyXYZ;
/** @brief  Forward declaration of PIKeys. */
typedef  struct  PIKeys            PIKeys;
/** @brief  Forward declaration of PIPixel. */
typedef  union   PIPixel           PIPixel;
/** @brief  Forward declaration of PIPlugin. */
typedef  struct  PIPlugin          PIPlugin;
/** @brief  Forward declaration of PIPopup. */
typedef  struct  PIPopup           PIPopup;
/** @brief  Forward declaration of PIProfile. */
typedef  struct  PIProfile         PIProfile;
/** @brief  Forward declaration of PIVideoInfo. */
typedef  struct  PIVideoInfo       PIVideoInfo;
/** @brief  Forward declaration of PIVideoBlock. */
typedef  struct  PIVideoBlock      PIVideoBlock;
/** @brief  Forward declaration of PIVideoDevice. */
typedef  struct  PIVideoDevice     PIVideoDevice;


#ifdef __cplusplus
}
#endif


#endif // __pi_basics_HH
