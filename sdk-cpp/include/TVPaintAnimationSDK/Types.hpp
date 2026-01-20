/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Types.hpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : The types.
 */

#pragma once


#include <cassert>
#include <cstddef>
#include <cstdint>


namespace  nSDK {


//MARK: ========================================================================================================= integers


#if defined( STL_HAS_NO_INT8_16_32_64_etc )
//using   int8  =   signed char;
//using  uint8  = unsigned char;
//using   int16 =   signed short int;
//using  uint16 = unsigned short int;
//using   int32 =   signed int;
//using  uint32 = unsigned int;
//using   int64 =   signed long long int;
//using  uint64 = unsigned long long int;
//#endif
#elif defined( TVP_PLATFORM_LINUX )
using   int8  =    int8_t;
using  uint8  =  u_int8_t;
using   int16 =   int16_t;
using  uint16 = u_int16_t;
using   int32 =   int32_t;
using  uint32 = u_int32_t;
using   int64 =   int64_t;
using  uint64 = u_int64_t;
#else
using   int8  =   int8_t;
using  uint8  =  uint8_t;
using   int16 =  int16_t;
using  uint16 = uint16_t;
using   int32 =  int32_t;
using  uint32 = uint32_t;
using   int64 =  int64_t;
using  uint64 = uint64_t;
#endif

using  float32 = float;
using  float64 = double;


//MARK: ========================================================================================================= indexing


/** The number of elements in some structure. */
using  tCount32 = int32;
using  tCount64 = int64;

/** The index of an element in some structure. */
using  tIndex32 = int32;
using  tIndex64 = int64;

/** The 'not-found' marker. */
constexpr  tIndex32  kNotFound = -1; // this is also OK for 64 bits indexes

/** The number of bytes of some structure. */
using  tByteCount32 = int32;
using  tByteCount64 = int64;


//MARK: ========================================================================================================= errors


/** @brief  An errorcode.

    To ease reading error codes while debugging, it is recommended to use four char codes, which can be generated with operator""_Error().
 */
using  tErrorCode = uint32;


/** @brief  Used to generate error codes.

    "1234"_Error (with 1234 being four chars), will return an errorcode with this code.
 */
constexpr  tErrorCode
operator""_Error( const char*  iStr, std::size_t  iSize ) noexcept
{
    assert( iSize == 4 );

    return  static_cast< tErrorCode >( iStr[3] ) |
            static_cast< tErrorCode >( iStr[2] ) << 8u |
            static_cast< tErrorCode >( iStr[1] ) << 16u |
            static_cast< tErrorCode >( iStr[0] ) << 24u;
}


/** @brief  The various error codes.

    A function/method can return error codes not in this enum, but they have to document them in this case.

    @note  Predefined error codes starts with a space, all codes starting with a space are RESERVED.
 */
enum  eErrorCode : tErrorCode
{
    kErrorCode_NoError = 0, ///< No error happened, all is well.

    kErrorCode_LegacyError    = " LER"_Error,  ///< Error in legacy code, unfortunately this is the most common error and there is no further information available about what went wrong...
    kErrorCode_Error          = " ERR"_Error,  ///< Unknown/unspecified error
    kErrorCode_BadArgument    = " BAr"_Error,  ///< Bad Argument
    kErrorCode_InternalError  = " Int"_Error,  ///< Internal Error
    kErrorCode_NotImplemented = " !im"_Error,  ///< Not implemented
    kErrorCode_NoMemoryLeft   = " !ml"_Error,  ///< No memory left, memory allocation failure
    kErrorCode_NotInitialized = " !IN"_Error,  ///< Something is not initialized
    kErrorCode_NotProcessed   = " !Pr"_Error,  ///< Event has not been processed, try to process it in another way
    kErrorCode_NotFound       = " !fd"_Error,  ///< Something has not been found
};


//MARK: ========================================================================================================= class "properties"


#define  SDK_ABSTRACT_CLASS( iiClass )                       \
    public:                                                   \
        virtual    ~iiClass() = 0;

#define  SDK_NON_COPYABLE( iiClass )                         \
    public:                                                   \
                      iiClass(       iiClass&  ) = delete;    \
                      iiClass( const iiClass&  ) = delete;    \
        iiClass&    operator=(       iiClass&  ) = delete;    \
        iiClass&    operator=( const iiClass&  ) = delete;    \
                      iiClass(       iiClass&& ) = delete;    \
                      iiClass( const iiClass&& ) = delete;    \
        iiClass&    operator=(       iiClass&& ) = delete;    \
        iiClass&    operator=( const iiClass&& ) = delete;

// only valid thing for `...` is `virtual` for now...
#define  SDK_DEFAULT_COPYABLE( iiClass, ... )                \
    public:                                                   \
        __VA_ARGS__  ~iiClass(                 ) = default;   \
                      iiClass( const iiClass&  ) = default;   \
        iiClass&    operator=( const iiClass&  ) = default;   \
                      iiClass(       iiClass&& ) = default;   \
        iiClass&    operator=(       iiClass&& ) = default;

// only valid thing for `...` is `virtual` for now...
#define  SDK_DEFAULT_COPYABLE_NON_MOVEABLE( iiClass, ... )   \
    public:                                                   \
        __VA_ARGS__  ~iiClass(                 ) = default;   \
                      iiClass( const iiClass&  ) = default;   \
        iiClass&    operator=( const iiClass&  ) = default;   \
/* some dark-magic in C++ makes this go wrong... */           \
/*                      iiClass(       iiClass&& ) = delete;*/    \
/*                      iiClass( const iiClass&& ) = delete;*/    \
/*        iiClass&    operator=(       iiClass&& ) = delete;*/    \
/*        iiClass&    operator=( const iiClass&& ) = delete;*/

// only valid thing for `...` is `virtual` for now...
#define  SDK_USER_DEFINED_COPYABLE( iiClass, ... )           \
    public:                                                   \
        __VA_ARGS__  ~iiClass(                 );             \
                      iiClass(       iiClass&  );             \
                      iiClass( const iiClass&  );             \
        iiClass&    operator=(       iiClass&  );             \
        iiClass&    operator=( const iiClass&  );             \
                      iiClass(       iiClass&& );             \
                      iiClass( const iiClass&& );             \
        iiClass&    operator=(       iiClass&& );             \
        iiClass&    operator=( const iiClass&& );


//MARK: ========================================================================================================= Types


class  cPixelPosition;
class  cPixelRectangle;
class  cPixelSize;

class  cPI2Keys;
class  cTV2Plugin;
class  cTV2Window;


} // namespace  nSDK
