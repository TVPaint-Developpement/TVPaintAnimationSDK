/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Slider.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A slider button.
 */


#include "Button-Slider.h"


#include "Plugin.h"
#include "Window.h"


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2ButtonSlider


cTV2ButtonSlider::~cTV2ButtonSlider()
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonSlider::cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iValueX, int  iMaxX, int  iNumX, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueXValue( iValueX ),
    mValueXPtr( &mValueXValue ),
    mMaxX( iMaxX ),
    mNumX( iNumX ),
    mValueYValue( 0 ),
    mValueYPtr( &mValueYValue ),
    mMaxY( 0 ),
    mNumY( 0 ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddSliderReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, 0, mMaxX, mNumX, *mValueXPtr, 0,0,0 ) )
        return;
}


cTV2ButtonSlider::cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int*  iValueX, int  iMaxX, int  iNumX, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueXPtr( iValueX ),
    mMaxX( iMaxX ),
    mNumX( iNumX ),
    mValueYPtr( &mValueYValue ),
    mMaxY( 0 ),
    mNumY( 0 ),
    mValueChange( iValueChange )
{
    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    if( !TVAddSliderReq( plugin, req, mArea.X(), mArea.Y(), mArea.W(), mArea.H(), iID, 0, mMaxX, mNumX, *mValueXPtr, 0,0,0 ) )
        return;
}


cTV2ButtonSlider::cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int  iValueX, int  iMaxX, int  iNumX, int  iValueY, int  iMaxY, int  iNumY, tValueChange  iValueChange ) :
   cTV2Button( iWindow, iID, iArea ),
   mValueXValue( iValueX ),
   mValueXPtr( &mValueXValue ),
   mMaxX( iMaxX ),
   mNumX( iNumX ),
   mValueYValue( iValueY ),
   mValueYPtr( &mValueYValue ),
   mMaxY( iMaxY ),
   mNumY( iNumY ),
   mValueChange( iValueChange )
{
    mWindow->TV2ButtonSliderAdd( mID, mArea, mMaxX, mNumX, *mValueXPtr, mMaxY, mNumY, *mValueYPtr );
}



cTV2ButtonSlider::cTV2ButtonSlider( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, int*  iValueX, int  iMaxX, int  iNumX, int*  iValueY, int  iMaxY, int  iNumY, tValueChange  iValueChange ) :
    cTV2Button( iWindow, iID, iArea ),
    mValueXPtr( iValueX ),
    mMaxX( iMaxX ),
    mNumX( iNumX ),
    mValueYPtr( iValueY ),
    mMaxY( iMaxY ),
    mNumY( iNumY ),
    mValueChange( iValueChange )
{
    mWindow->TV2ButtonSliderAdd( mID, mArea, mMaxX, mNumX, *mValueXPtr, mMaxY, mNumY, *mValueYPtr );
}


void
cTV2ButtonSlider::Area( const cPixelRectangle&  iArea )
{
    mArea = iArea;
    mWindow->TV2ButtonRemove( mID );
    mWindow->TV2ButtonSliderAdd( mID, mArea, mMaxX, mNumX, *mValueXPtr, mMaxY, mNumY, *mValueYPtr );
}


void
cTV2ButtonSlider::ValueX( int  iValueX )
{
    ValueXY( iValueX, *mValueYPtr );
}


void
cTV2ButtonSlider::ValueY( int  iValueY )
{
    ValueXY( *mValueXPtr, iValueY );
}


void
cTV2ButtonSlider::ValueXY( int  iValueX, int  iValueY )
{
    if( iValueX == *mValueXPtr  &&  iValueY == *mValueYPtr )
        //TODO: //CHECK: should we call mValueChange() anyway ?!?!?!
        return;

    *mValueXPtr = iValueX;
    *mValueYPtr = iValueY;

    RefreshGUI();

    if( mValueChange )
        mValueChange( mID, *mValueXPtr, *mValueYPtr );
}


void
cTV2ButtonSlider::UpdateBounds( int  iMaxX, int  iNumX, int  iMaxY, int  iNumY )
{
    mMaxX = iMaxX;
    mNumX = iNumX;
    mMaxY = iMaxY;
    mNumY = iNumY;

    mWindow->TV2ButtonSliderChange( mID, iMaxX, iNumX, *mValueXPtr, iMaxY, iNumY, *mValueYPtr );
}


void
cTV2ButtonSlider::RefreshGUI()
{
    mWindow->TV2ButtonSliderChange( mID, mMaxX, mNumX, *mValueXPtr, mMaxY, mNumY, *mValueYPtr );
}


tErrorCode
cTV2ButtonSlider::EventButtonSliderDragged( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )
{
    assert( iButtonID == mID );

    ValueXY( iValueX, iValueY );

    return  kErrorCode_NoError;
}


tErrorCode
cTV2ButtonSlider::EventButtonSliderReleased( int  iButtonID, int  iValueX, int  iValueY, int  iMouseButton, int  iTimeMS, bool  iCtrlKey )
{
    assert( iButtonID == mID );

    ValueXY( iValueX, iValueY );

    return  kErrorCode_NoError;
}




} // namespace  nSDK

