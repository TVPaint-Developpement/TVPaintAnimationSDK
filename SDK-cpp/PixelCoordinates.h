/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  PixelCoordinates.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A set of pixels in the shape of an axis aligned rectangle, and support classes.
 */

#pragma once


//#include "tvpl/Core/Assertion.h"
#include "Types.h"


#if 0//TVP13:
#include <fmt/core.h>
#include <tinyxml2.h>
#endif//TVP13:


#include <algorithm>


namespace  nSDK {


class  cPixelPosition;
class  cPixelSize;
class  cPixelRectangle;


//MARK: ========================================================================================================= cPixelPosition


/** @brief  The position of a pixel in "pixel coordinates".
 *
 *  Pixel Coordinates means a point is referencing a whole pixel, not a "sizeless" point in R^2.
 *  It also means a rectangle with X1==X2 has a width of 1, not 0 !
 *
 *  @classtype  ADT, POD, Final.
*/
class  cPixelPosition  final
{
    SDK_DEFAULT_COPYABLE( cPixelPosition )

public:
///@name  Construction/Destruction
///@{
    /** @brief  The default constructor. */
    cPixelPosition() : mX( 0 ), mY( 0 )  {}

    /** @brief  The constructor.
     *
     *  @param[in]  iX  The X-coordinate.
     *  @param[in]  iY  The Y-coordinate.
     */
    cPixelPosition( tIndex32  iX, tIndex32  iY ) : mX( iX ), mY( iY )  {}
///@}

public:
///@name  Properties
///@{
    /** @brief  Returns the X component of the position.
     *
     *  @return The X component.
     */
    tIndex32  X() const  { return  mX; }

    /** @brief  Returns the Y component of the position.
     *
     *  @return  The Y component.
     */
    tIndex32  Y() const  { return  mY; }

    /** @brief  Sets the X component of the position.
     *
     *  @param[in]  iX  The value for the X component.
     */
    void  X( tIndex32  iX )  { mX = iX; }

    /** @brief  Sets the Y component of the position.
     *
     *  @param[in]  iY  The value for the Y component.
     */
    void  Y( tIndex32  iY )  { mY = iY; }

    /** @brief  Sets the X and the Y components of the position.
     *
     *  @param[in]  iX  The value for the X component.
     *  @param[in]  iY  The value for the Y component.
     */
    void  XY( tIndex32  iX, tIndex32  iY )  { mX = iX; mY = iY; }

    /** @brief  Sets the X and the Y components of the position.
     *
     *  @param[in]  iX  The value for the X component.
     *  @param[in]  iY  The value for the Y component.
     */
    void  Set( tIndex32  iX, tIndex32  iY )  { mX = iX;  mY = iY; }
///@}

public:
///@name  ADT
///@{
    /** @brief  Unary operator -.
     *
     *  @return  The negative of this
     */
    cPixelPosition  operator -() const  { return  { -mX, -mY }; }

    /** @brief  operator +=.
     *
     *  @param[in]  iRhs  The offset/size to add to the current position.
     */
    void  operator +=( const cPixelSize&  iRhs );

    /** @brief  operator -=.
     *
     *  @param[in]  iRhs  The offset/size to substract from the current position.
     */
    void  operator -=( const cPixelSize&  iRhs );
///@}

public:
///@name  Debugging helpers
///@{
    /** @brief  Prints some useful infos for debugging, only in debug mode.
     *
     *  @param[in]  iStr  Text to write before the position components.
     */
    void  DebugPrint( const char*  iStr=nullptr ) const;
///@}

public:
#if 0//TVP13:
///@name  Input/Output
///@{
    /** @brief  The XML setter.
     *
     *  @param[in,out]  ioState  The xml state we should write.
     *  @param[in]      iName    The name given to the value.
     */
    tErrorCode  SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const;

    /** @brief  The XML getter.
     *
     *  @param[in]  ioState  The xml state we should read.
     *  @param[in]  iName    The name given to the value.
     */
    tErrorCode  GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName );
///@}
#endif//TVP13:

public:
///@name  Data is public, this is a POD...
///@{
    tIndex32  mX;  ///< The X coordinate of the position.
    tIndex32  mY;  ///< The Y coordinate of the position.
///@}
};


//--------------------------------------------------------------------------------------------- Debugging helpers


inline
void
cPixelPosition::DebugPrint( const char*  iStr ) const
{
#ifdef DEBUG
    if( !iStr )  iStr = "";
//TODO:    LOGGING_DEBUG( "%s : % 5.3f % 5.3f", iStr, mX, mY );
#endif//DEBUG
}


//-------------------------------------------------------------------------------------------------- Input/Output


#if 0//TVP13:
inline
tErrorCode
cPixelPosition::SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const
{
    XXRequire( ioState  &&  iName );
    XXRequire( !ioState->Attribute( iName )  &&  !ioState->FirstChildElement( iName ) );

    ioState->SetAttribute( iName, fmt::format( "[{},{}]", mX, mY ).c_str() );

    return  kErrorCode_NoError;
}


inline
tErrorCode
cPixelPosition::GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName )
{
    XXRequire( iState  &&  iName );

    const char*  value = iState->Attribute( iName );
    XXRequire( strlen( value ) >= 5 );  // 5 is for [0,0]
    XXRequire( value[0] == '['  &&  value[strlen(value)-1] == ']' );
    ++value; // skip '['
    char*  end = nullptr;

    int  x = strtol( value, &end, 10 );
    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );
    XXRequire( *end == ',' );
    value = end+1; // skip ',', assumes no spaces as specified by format in SetXML()

    int  y = strtol( value, &end, 10 );
    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );

    mX = x;
    mY = y;

    return  kErrorCode_NoError;
}
#endif//TVP13:


//MARK: ========================================================================================================= cPixelSize


/** @brief  The size of a rectangle of pixels in "pixel coordinates".
 *
 *  Pixel Coordinates means a point is referencing a whole pixel, not a "sizeless" point in R^2.
 *  It also means a rectangle with X1==X2 has a width of 1, not 0 !
 *
 *  @classtype  ADT, POD, Final.
*/
class  cPixelSize  final
{
    SDK_DEFAULT_COPYABLE( cPixelSize )

public:
///@name  Construction/Destruction
///@{
    /** @brief  The default constructor. That's a single pixel. */
    cPixelSize() : mW( 0 ), mH( 0 )  {}

    /** @brief  The constructor.
     *
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     */
    cPixelSize( tCount32  iW, tCount32  iH ) : mW( iW ), mH( iH )  {}
///@}

public:
///@name  Properties
///@{
    /** @brief  Returns the width.
     *
     *  @return The width.
     */
    tCount32  W() const  { return  mW; }

    /** @brief  Returns the height.
     *
     *  @return The height.
     */
    tCount32  H() const  { return  mH; }

    /** @brief  Sets the width.
     *
     *  @param[in]  iW  The width.
     */
    void  W( tCount32  iW )  { mW = iW; }

    /** @brief  Sets the height.
     *
     *  @param[in]  iH  The height.
     */
    void  H( tCount32  iH )  { mH = iH; }

    /** @brief  Sets the width and the height.
     *
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     */
    void  WH( tCount32  iW, tCount32  iH )  { mW = iW; mH = iH; }

    /** @brief  Sets the width and the height.
     *
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     */
    void  Set( tCount32  iW, tCount32  iH )  { mW = iW;  mH = iH; }
///@}

public:
///@name  ADT
///@{
    /** @brief  Unary operator -.
     *
     *  @return  The negative of this.
     */
    cPixelSize  operator -() const  { return  { -mW, -mH }; }

    /** @brief  operator +=.
     *
     *  @param[in]  iRhs  The size to add to the current size.
     */
    void  operator +=( const cPixelSize&  iRhs )  { mW += iRhs.mW;  mH += iRhs.mH; }

    /** @brief  operator -=.
     *
     *  @param[in]  iRhs  The size to substract to the current size.
     */
    void  operator -=( const cPixelSize&  iRhs )  { mW -= iRhs.mW;  mH -= iRhs.mH; }

    /** @brief  operator *=.
     *
     *  @param[in]  iRhs  The scale to use for multiplying the size.
     */
    void  operator *=( int  iRhs )  { mW *= iRhs;  mH *= iRhs; }

    /** @brief  operator /=.
     *
     *  @param[in]  iRhs  The scale to use for dividing the size.
     */
    void  operator /=( int  iRhs )  { mW /= iRhs;  mH /= iRhs; }
///@}

public:
///@name  Debugging helpers
///@{
    /** @brief  Prints some useful infos for debugging, only in debug mode.
     *
     *  @param[in]  iStr  Text to write before the size.
     */
    void  DebugPrint( const char*  iStr=nullptr ) const;
///@}

public:
#if 0//TVP13:
///@name  Input/Output
///@{
    /** @brief  The XML setter.
     *
     *  @param[in,out]  ioState  The xml state we should write.
     *  @param[in]      iName  The name given to the value.
     */
    tErrorCode  SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const;

    /** @brief  The XML getter.
     *
     *  @param[in]  ioState  The xml state we should read.
     *  @param[in]  iName    The name given to the value.
     */
    tErrorCode  GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName );
///@}
#endif//TVP13:

public:
///@name  Data is public, this is a POD...
///@{
    tCount32  mW;  ///< The W of the size.
    tCount32  mH;  ///< The H of the size.
///@}
};


//--------------------------------------------------------------------------------------------- Debugging helpers


inline
void
cPixelSize::DebugPrint( const char*  iStr ) const
{
#ifdef DEBUG
    if( !iStr )  iStr = "";
//TODO:    LOGGING_DEBUG( "%s : % 5.3f % 5.3f", iStr, mW, mH );
#endif//DEBUG
}


//-------------------------------------------------------------------------------------------------- Input/Output


#if 0//TVP13:
inline
tErrorCode
cPixelSize::SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const
{
    XXRequire( ioState  &&  iName );
    XXRequire( !ioState->Attribute( iName )  &&  !ioState->FirstChildElement( iName ) );

    ioState->SetAttribute( iName, fmt::format( "[{},{}]", mW, mH ).c_str() );

    return  kErrorCode_NoError;
}


inline
tErrorCode
cPixelSize::GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName )
{
    XXRequire( iState  &&  iName );

    const char*  value = iState->Attribute( iName );
    XXRequire( strlen( value ) >= 5 );  // 5 is for [0,0]
    XXRequire( value[0] == '['  &&  value[strlen(value)-1] == ']' );
    ++value; // skip '['
    char*  end = nullptr;

    int  x = strtol( value, &end, 10 );
    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );
    XXRequire( *end == ',' );
    value = end+1; // skip ',', assumes no spaces as specified by format in SetXML()

    int  y = strtol( value, &end, 10 );
    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );

    mW = x;
    mH = y;

    return  kErrorCode_NoError;
}
#endif//TVP13:


//MARK: ========================================================================================================= cPixelRectangle


/** @brief  A rectangle in "pixel coordinates".
 *
 *  Pixel Coordinates means a point is referencing a whole pixel, not a "sizeless" point in R^2.
 *  It also means a rectangle with X1==X2 has a width of 1, not 0 !
 *
 *  There are two ways of accessing the properties :
 *  - Position + size
 *  - Two corners
 *  Avoid using both ways in the same context. It will work but will be so hard to understand...
 *
 *  //TODO: should we check and adjust for negative sizes in position+size setters ?!?!?!
 *
 *  //TODO: how to represent an empty rectangle, as W=0 (or H=0) does not mean empty ?  W < 0 ? add a bool ? in cPixelSize ?
 *  //----: right now this implies that Intersection() has to return a bool and take the result rectangle as a pointer, contrary to BoundingBox()...
 *
 *  @classtype  ADT, POD, Final.
 */
class  cPixelRectangle  final
{
    SDK_DEFAULT_COPYABLE( cPixelRectangle )

public:
///@name  Construction/Destruction
///@{
    /** @brief  The default destructor. */
    cPixelRectangle() = default;

    /** @brief  Type for the geometry, used only in constructors because of C++'s stupid ctor naming convention. */
    enum  class  eXYWH  { kXYWH };

    /** @brief  A constructor.
     *
     *  @param[in]  iX  The x-coordinate of the top-left corner.
     *  @param[in]  iY  The y-coordinate of the top-left corner.
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     */
    cPixelRectangle( tIndex32  iX, tIndex32  iY, tCount32  iW, tCount32  iH, eXYWH ) :
        mX( iX ),
        mY( iY ),
        mW( iW ),
        mH( iH )
    {
    }

    /** @brief  A constructor.
     *
     *  @param[in]  iXY  The top-left corner.
     *  @param[in]  iWH  The size.
     */
    cPixelRectangle( const cPixelPosition&  iXY, const cPixelSize&  iWH ) :
        mX( iXY.X() ),
        mY( iXY.Y() ),
        mW( iWH.W() ),
        mH( iWH.H() )
    {
    }

    /** @brief Type for the geometry, used only in constructors because of C++'s stupid ctor naming convention. */
    enum  class  eXYXY  { kXYXY };

    /** @brief  A constructor.
     *
     *  @param[in]  iX1  The x-coordinate of one corner.
     *  @param[in]  iY1  The y-coordinate of one corner.
     *  @param[in]  iX2  The x-coordinate of the opposite corner.
     *  @param[in]  iY2  The y-coordinate of the opposite corner.
     */
    cPixelRectangle( tIndex32  iX1, tIndex32  iY1, tIndex32  iX2, tIndex32  iY2, eXYXY ) :
        mX( iX1 ),
        mY( iY1 ),
        mW( iX2 - iX1 + 1 ),
        mH( iY2 - iY1 + 1 )
    {
        // fix "inverted" rects
        if( iX1 > iX2 )
        {
            mX = iX2;
            mW = iX1 - iX2 + 1;
        }
        if( iY1 > iY2 )
        {
            mY = iY2;
            mH = iY1 - iY2 + 1;
        }
    }

    /** @brief  A constructor.
     *
     *  @param[in]  iXY1  One of the corners.
     *  @param[in]  iXY2  The opposite corner.
     */
    cPixelRectangle( const cPixelPosition&  iXY1, const cPixelPosition&  iXY2 ) :
        mX( iXY1.X() ),
        mY( iXY1.Y() ),
        mW( iXY2.X() - iXY1.X() + 1 ),
        mH( iXY2.Y() - iXY1.Y() + 1 )
    {
        // fix "inverted" rects
        if( iXY1.X() > iXY2.X() )
        {
            mX = iXY2.X();
            mW = iXY1.X() - iXY2.X() + 1;
        }
        if( iXY1.Y() > iXY2.Y() )
        {
            mY = iXY2.Y();
            mH = iXY1.Y() - iXY2.Y() + 1;
        }
    }
///@}

public:
///@name  'Makors'
///@{
    /** @brief  Makes a rectangle with coordinates of the top-left corner and the size.
     *
     *  @param[in]  iX  The x-coordinate of the top-left corner.
     *  @param[in]  iY  The y-coordinate of the top-left corner.
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     *
     *  @return  A rectangle with the given parameters.
     */
    static  cPixelRectangle  MakeXYWH( tIndex32  iX, tIndex32  iY, tCount32  iW, tCount32  iH )
    {
        return  cPixelRectangle( iX, iY, iW, iH, eXYWH::kXYWH );
    }

    /** @brief  Makes a rectangle with coordinates of the top-left corner and the size.
     *
     *  @param[in]  iXY  The top-left corner.
     *  @param[in]  iWH  The size.
     *
     *  @return  A rectangle with the given parameters.
     */
    static  cPixelRectangle  MakeXYWH( const cPixelPosition&  iXY, const cPixelSize&  iWH )
    {
        return  cPixelRectangle( iXY, iWH );
    }

    /** @brief  Makes a rectangle with coordinates of two opposite corners.
     *
     *  @param[in]  iX1  The x-coordinate of one corner.
     *  @param[in]  iY1  The y-coordinate of one corner.
     *  @param[in]  iX2  The x-coordinate of the opposite corner.
     *  @param[in]  iY2  The y-coordinate of the opposite corner.
     *
     *  @return  A rectangle with the given parameters.
     */
    static  cPixelRectangle  MakeXYXY( tIndex32  iX1, tIndex32  iY1, tIndex32  iX2, tIndex32  iY2 )
    {
        return  cPixelRectangle( iX1, iY1, iX2, iY2, eXYXY::kXYXY );
    }

    /** @brief  Makes a rectangle with coordinates of two opposite corners.
     *
     *  @param[in]  iXY1  One of the corners.
     *  @param[in]  iXY2  The opposite corner.
     *
     *  @return  A rectangle with the given parameters.
     */
    static  cPixelRectangle  MakeXYXY( const cPixelPosition&  iXY1, const cPixelPosition&  iXY2 )
    {
        return  cPixelRectangle( iXY1, iXY2 );
    }

///@}

public:
///@name  Position + size style properties.
///@{
    /** @brief  Returns the top-left corner.
     *
     *  @return  The top-left corner of this rectangle.
     */
    cPixelPosition  XY() const  { return  {mX, mY}; }

    /** @brief  Returns the X coordinate of the top-left corner.
     *
     *  @return  The x (left) coordinate of this rectangle.
     */
    tIndex32  X() const  { return  mX; }

    /** @brief  Returns the Y coordinate of the top-left corner.
     *
     *  @return  The y (top) coordinate of this rectangle.
     */
    tIndex32  Y() const  { return  mY; }

    /** @brief  Returns the size.
     *
     *  @return  The size of this rectangle.
     */
    cPixelSize  WH() const  { return  {mW, mH}; }

    /** @brief  Returns the width of the rectangle.
     *
     *  @return  The width of this rectangle.
     */
    tCount32  W() const  { return  mW; }

    /** @brief  Returns the height of the rectangle.
     *
     *  @return  The height of this rectangle.
     */
    tCount32  H() const  { return  mH; }

    /** @brief  Sets the top-left corner.
     *
     *  @param[in]  iXY  The top-left corner of the rectangle.
     */
    void  XY( const cPixelPosition&  iXY )  { mX = iXY.X(); mY = iXY.Y(); }

    /** @brief  Sets the X coordinate of the top-left corner.
     *
     *  @param[in]  iX  The left coordinate of the rectangle.
     */
    void  X( tIndex32  iX )  { mX = iX; }

    /** @brief  Sets the Y coordinate of the top-left corner.
     *
     *  @param[in]  iY  The left coordinate of the rectangle.
     */
    void  Y( tIndex32  iY )  { mY = iY; }

    /** @brief  Sets the size.
     *
     *  @param[in]  iWH  The size of the rectangle.
     */
    void  WH( const cPixelSize&  iWH )  { mW = iWH.W(); mH = iWH.H(); }

    /** @brief  Sets the width of the rectangle.
     *
     *  @param[in]  iW  The width.
     */
    void  W( tCount32  iW )  { mW = std::max( 0, iW ); }

    /** @brief  Sets the height of the rectangle.
     *
     *  @param[in]  iH  The height.
     */
    void  H( tCount32  iH )  { mH = std::max( 0, iH ); }

    /** @brief  Sets the whole rectangle.
     *
     *  @param[in]  iXY  The top-left corner of the rectangle.
     *  @param[in]  iWH  The size of the rectangle.
     */
    void  XYWH( const cPixelPosition&  iXY, const cPixelSize&  iWH )  { mX = iXY.X(); mY = iXY.Y(); mW = iWH.W(); mH = iWH.H(); }

    /** @brief  Sets the whole rectangle.
     *
     *  @param[in]  iX  The x-coordinate of the top-left corner.
     *  @param[in]  iY  The y-coordinate of the top-left corner.
     *  @param[in]  iW  The width.
     *  @param[in]  iH  The height.
     */
    void  Set( tIndex32  iX, tIndex32  iY, tCount32  iW, tCount32  iH, eXYWH )
    {
        mX = iX;
        mY = iY;
        mW = iW;
        mH = iH;
    }
///@}

public:
///@name  Two corners style properties.
///@{
    /** @brief  Returns the top-left corner.
     *
     *  @return  The top-left corner of this rectangle.
     */
    cPixelPosition  XY1() const  { return  {mX, mY}; }

    /** @brief  Returns the X coordinate of the top-left corner.
     *
     *  @return  The left coordinate of the rectangle.
     */
    tIndex32  X1() const  { return  mX; }

    /** @brief  Returns the Y coordinate of the top-left corner.
     *
     *  @return  The top coordinate of the rectangle.
     */
    tIndex32  Y1() const  { return  mY; }

    /** @brief  Returns the bottom-right corner.
     *
     *  @return  The bottom-right corner of this rectangle.
     */
    cPixelPosition  XY2() const  { return  {mX + mW - 1, mY + mH - 1}; }

    /** @brief  Returns the X coordinate of the bottom-right corner.
     *
     *  @return  The right coordinate of the rectangle.
     */
    tIndex32  X2() const  { return  mX + mW - 1; }

    /** @brief  Returns the Y coordinate of the bottom-right corner.
     *
     *  @return  The bottom coordinate of the rectangle.
     */
    tIndex32  Y2() const  { return  mY + mH - 1; }

    /** @brief  Sets the top-left corner.
     *
     *  @param[in]  iXY  The top-left corner of the rectangle.
     */
    void  XY1( const cPixelPosition&  iXY )  { X1( iXY.X() ); Y1( iXY.Y() ); }

    /** @brief  Sets the X coordinate of the top-left corner.
     *
     *  @param[in]  iX1  The left coordinate of the rectangle.
     */
    void  X1( tIndex32  iX1 )  { int  x2 = X2(); mX = iX1; X2( x2 ); }

    /** @brief  Sets the Y coordinate of the top-left corner.
     *
     *  @param[in]  iY1  The top coordinate of the rectangle.
     */
    void  Y1( tIndex32  iY1 )  { int  y2 = Y2(); mY = iY1; Y2( y2 ); }

    /** @brief  Sets the bottom-right corner.
     *
     *  @param[in]  iXY  The bottom-right corner of the rectangle.
     */
    void  XY2( const cPixelPosition&  iXY )  { X2( iXY.X() ); Y2( iXY.Y() ); }

    /** @brief  Sets the X coordinate of the bottom-right corner.
     *
     *  @param[in]  iX2  The right coordinate of the rectangle.
     */
    void  X2( tIndex32  iX2 )  { W( iX2 - mX + 1 ); }

    /** @brief  Sets the Y coordinate of the bottom-right corner.
     *
     *  @param[in]  iY2  The right coordinate of the rectangle.
     */
    void  Y2( tIndex32  iY2 )  { H( iY2 - mY + 1 ); }

    /** @brief  Sets the whole rectangle.
     *
     *  @param[in]  iXY1  One corner of the rectangle.
     *  @param[in]  iXY2  The opposite corner of the rectangle.
     */
    void  XYXY( const cPixelPosition&  iXY1, const cPixelPosition&  iXY2 )  { X1( iXY1.X() ); Y1( iXY1.Y() ); X2( iXY2.X() ); Y2( iXY2.Y() ); }

    /** @brief  Sets the whole rectangle.
     *
     *  @param[in]  iX1  The x-coordinate of one corner.
     *  @param[in]  iY1  The y-coordinate of one corner.
     *  @param[in]  iX2  The x-coordinate of the opposite corner.
     *  @param[in]  iY2  The y-coordinate of the opposite corner.
     */
    void  Set( tIndex32  iX1, tIndex32  iY1, tIndex32  iX2, tIndex32  iY2, eXYXY )
    {
        mX = iX1;
        mY = iY1;
        mW = iX2 - iX1 + 1;
        mH = iY2 - iY1 + 1;

        // fix "inverted" rects
        if( iX1 > iX2 )
        {
            mX = iX2;
            mW = iX1 - iX2 + 1;
        }
        if( iY1 > iY2 )
        {
            mY = iY2;
            mH = iY1 - iY2 + 1;
        }
    }
///@}

public:
///@name  Verbose access to properties.
///@{
    /** @brief  Returns the top-left corner.
     *
     *  @return  The top-left corner of this rectangle.
     */
    cPixelPosition  TopLeft() const  { return  {X1(), Y1()}; }

    /** @brief  Returns the top-right corner.
     *
     *  @return  The top-right corner of this rectangle.
     */
    cPixelPosition  TopRight() const  { return  {X2(), Y1()}; }

    /** @brief  Returns the bottom-left corner.
     *
     *  @return  The bottom-left corner of this rectangle.
     */
    cPixelPosition  BottomLeft() const  { return  {X1(), Y2()}; }

    /** @brief  Returns the bottom-right corner.
     *
     *  @return  The bottom-right corner of this rectangle.
     */
    cPixelPosition  BottomRight() const  { return  {X2(), Y2()}; }

    /** @brief  Returns the size.
     *
     *  @return  The size of this rectangle.
     */
    cPixelSize  Size() const  { return  {mW, mH}; }
///@}

public:
///@name  Utilities.
///@{
    /** @brief  Gets the intersection, if it exists, between two rectangles.
     *
     *  @param[out]  oDst   The intersection rectangle (computed only if the two rectangles intersect).
     *  @param[in]   iSrc1  One of the rectangles.
     *  @param[in]   iSrc2  The other rectangle.
     *
     *  @return  True if the two rectangles intersect.
     */
    static  bool  Intersection( cPixelRectangle*  oDst, const cPixelRectangle&  iSrc1, const cPixelRectangle&  iSrc2 )
    {
        //XXRequire( oDst );

        //TODO: //CHECK: this is FALSE !!!!!!!!!!!!
        if( iSrc1.W() == 0  ||  iSrc2.W() == 0  ||  iSrc1.H() == 0  ||  iSrc2.H() == 0 ) // vide
            return  false;
        if( iSrc1.X2() < iSrc2.X1()  ||  iSrc1.Y2() < iSrc2.Y1()  ||  iSrc1.X1() > iSrc2.X2()  ||  iSrc1.Y1() > iSrc2.Y2() )
            return  false;
        tIndex32  x1 = std::max( iSrc1.X1(), iSrc2.X1() );
        tIndex32  y1 = std::max( iSrc1.Y1(), iSrc2.Y1() );
        tIndex32  x2 = std::min( iSrc1.X2(), iSrc2.X2() );
        tIndex32  y2 = std::min( iSrc1.Y2(), iSrc2.Y2() );
        *oDst = cPixelRectangle::MakeXYXY( x1, y1, x2, y2 );
        return  true;
    }

    /** @brief  Gets the axis aligned bounding box of two rectangles.
     *
     *  @param[in]  iSrc1  The first rectangle.
     *  @param[in]  iSrc2  The second rectangle.
     *
     *  @return  The bounding box.
     */
    static  cPixelRectangle  BoundingBox( const cPixelRectangle&  iSrc1, const cPixelRectangle&  iSrc2 )
    {
        if( iSrc1.W() == 0  ||  iSrc1.H() == 0 )
            return  iSrc2;
        if( iSrc2.W() == 0  ||  iSrc2.H() == 0 )
            return  iSrc1;

        tIndex32  x1 = std::min( iSrc1.X1(), iSrc2.X1() );
        tIndex32  y1 = std::min( iSrc1.Y1(), iSrc2.Y1() );
        tIndex32  x2 = std::max( iSrc1.X2(), iSrc2.X2() );
        tIndex32  y2 = std::max( iSrc1.Y2(), iSrc2.Y2() );
        return  cPixelRectangle::MakeXYXY( x1, y1, x2, y2 );
    }

    /** @brief  Checks if a point is contained inside a rectangle.
     *
     *  @param[in]  iPoint  The point to test.
     *
     *  @return  True if the point is inside the rectangle.
     */
    bool  Contains( const cPixelPosition&  iPoint ) const
    {
        return  iPoint.X() >= X1() && iPoint.Y() >= Y1() && iPoint.X() <= X2() && iPoint.Y() <= Y2();
    }

    /** @brief  operator ==
     *
     *  @param[in]  iLHS  The first rectangle.
     *  @param[in]  iRHS  The second rectangle.
     *
     *  @return  True if the two rectangles are equals.
     */
    friend
    bool
    operator==( const cPixelRectangle&  iLHS, const cPixelRectangle&  iRHS )
    {
        return  iLHS.mX == iRHS.mX  &&  iLHS.mY == iRHS.mY  &&  iLHS.mW == iRHS.mW  &&  iLHS.mH == iRHS.mH;
    }

    /** @brief  operator !=
     *
     *  @param[in]  iLHS  The first rectangle.
     *  @param[in]  iRHS  The second rectangle.
     *
     *  @return  True if the two rectangles are different.
     */
    friend
    bool
    operator!=( const cPixelRectangle&  iLHS, const cPixelRectangle&  iRHS )
    {
        return  iLHS.mX != iRHS.mX  ||  iLHS.mY != iRHS.mY  ||  iLHS.mW != iRHS.mW  ||  iLHS.mH != iRHS.mH;
    }
///@}

public:
///@name  Debugging helpers
///@{
    /** @brief  Prints some useful infos for debugging, only in debug mode.
     *
     *  @param[in]  iStr  Text to write before the rectangle components.
     */
    void  DebugPrint( const char*  iStr=nullptr ) const;
///@}

public:
#if 0//TVP13:
//TODO: XML I/Os !!!!
///@name  Input/Output
///@{
    /** @brief  The XML setter.
     *
     *  @param[in,out]  ioState  The xml state we should write.
     *  @param[in]      iName    The name given to the value.
     */
    tErrorCode  SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const;

    /** @brief  The XML getter.
     *
     *  @param[in]  ioState  The xml state we should read.
     *  @param[in]  iName    The name given to the value.
     */
    tErrorCode  GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName );
///@}
#endif//TVP13:

public:
///@name  Data is public, this is a POD...
///@{
    //TODO: //CHECK: use PixPos and PixSize ?
    tIndex32  mX {0};
    tIndex32  mY {0};
    tCount32  mW {0};
    tCount32  mH {0};
///@}
};


//--------------------------------------------------------------------------------------------- Debugging helpers


inline
void
cPixelRectangle::DebugPrint( const char*  iStr ) const
{
#ifdef DEBUG
    if( !iStr )  iStr = "";
//TODO:    LOGGING_DEBUG( "%s : % 5.3f % 5.3f % 5.3f % 5.3f", iStr, mX, mY, mW, mH );
#endif//DEBUG
}


//-------------------------------------------------------------------------------------------------- Input/Output


#if 0//TVP13:
inline
tErrorCode
cPixelRectangle::SetXML( ::tinyxml2::XMLElement*  ioState, const char*  iName ) const
{
    XXRequire( ioState  &&  iName );
    XXRequire( !ioState->Attribute( iName )  &&  !ioState->FirstChildElement( iName ) );

    XXTodo();
//    ioState->SetAttribute( iName, fmt::format( "[{},{}]", mX, mY ).c_str() );

    return  kErrorCode_NoError;
}


inline
tErrorCode
cPixelRectangle::GetXML( const ::tinyxml2::XMLElement*  iState, const char*  iName )
{
    XXRequire( iState  &&  iName );

    XXTodo();
//    const char*  value = iState->Attribute( iName );
//    XXRequire( strlen( value ) >= 5 );  // 5 is for [0,0]
//    XXRequire( value[0] == '['  &&  value[strlen(value)-1] == ']' );
//    ++value; // skip '['
//    char*  end = nullptr;
//
//    int  x = strtol( value, &end, 10 );
//    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );
//    XXRequire( *end == ',' );
//    value = end+1; // skip ',', assumes no spaces as specified by format in SetXML()
//
//    int  y = strtol( value, &end, 10 );
//    if( end == value )  return  XXErrorCode( "!val"_Error, "Invalid value" );
//
//    mX = x;
//    mY = y;

    return  kErrorCode_NoError;
}
#endif//TVP13:


//MARK: =========================================================================================================


inline
bool
operator ==( const cPixelPosition&  iLhs, const cPixelPosition&  iRhs )
{
    return  iLhs.mX == iRhs.mX  &&  iLhs.mY == iRhs.mY;
}


inline
bool
operator !=( const cPixelPosition&  iLhs, const cPixelPosition&  iRhs )
{
    return  iLhs.mX != iRhs.mX  ||  iLhs.mY != iRhs.mY;
}


inline
bool
operator ==( const cPixelSize&  iLhs, const cPixelSize&  iRhs )
{
    return  iLhs.mW == iRhs.mW  &&  iLhs.mH == iRhs.mH;
}


inline
bool
operator !=( const cPixelSize&  iLhs, const cPixelSize&  iRhs )
{
    return  iLhs.mW != iRhs.mW  ||  iLhs.mH != iRhs.mH;
}


inline
cPixelPosition
operator +( const cPixelPosition&  iLhs, const cPixelSize&  iRhs )
{
    return  { iLhs.mX + iRhs.mW, iLhs.mY + iRhs.mH };
}


inline
cPixelSize
operator +( const cPixelSize&  iLhs, const cPixelSize&  iRhs )
{
    return  { iLhs.mW + iRhs.mW, iLhs.mH + iRhs.mH };
}


inline
cPixelPosition
operator -( const cPixelPosition&  iLhs, const cPixelSize&  iRhs )
{
    return  { iLhs.mX - iRhs.mW, iLhs.mY - iRhs.mH };
}


inline
cPixelSize
operator -( const cPixelSize&  iLhs, const cPixelSize&  iRhs )
{
    return  { iLhs.mW - iRhs.mH, iLhs.mW - iRhs.mH };
}


inline
cPixelSize
operator *( const cPixelSize&  iLhs, int  iRhs )
{
    return  { iLhs.mW * iRhs, iLhs.mH * iRhs };
}


inline
cPixelSize
operator /( const cPixelSize&  iLhs, int  iRhs )
{
    return  { iLhs.mW / iRhs, iLhs.mH / iRhs };
}


inline
void
cPixelPosition::operator +=( const cPixelSize&  iRhs )
{
    mX += iRhs.mW;
    mY += iRhs.mH;
}


inline
void
cPixelPosition::operator -=( const cPixelSize&  iRhs )
{
    mX -= iRhs.mW;
    mY -= iRhs.mH;
}


} // namepace  nSDK

