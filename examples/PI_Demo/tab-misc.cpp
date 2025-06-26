/** @copyright (c) 2005-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-misc.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating miscellaneous stuff.
*/


#include "SDK2.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>
#include <stdio.h>


//MARK: ========================================================================================================= Misc Tab


constexpr  int  kTheButtonW = 770;
constexpr  int  kTheButtonH = 160;


enum  eIDs
{
    kID_First = 10000,

    kID_Result = kID_First+900,
};


//MARK: ========================================================================================================= cTabMisc


class  cTabMisc :
    public  nSDK2::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabMisc()  override  { Hide(); }
    cTabMisc( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Misc"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

public:
    void  RegisterTicksHandler( bool  iRegister );
    void  RegisterCoordinatesHandler( bool  iRegister );

private:
    cDemoWindow*  mWindow;

    nSDK2::cTV2ButtonCheckBox*     mGUITicks             {nullptr};  bool  mTicks   {false};
    nSDK2::cTV2ButtonCheckBox*     mGUICoords            {nullptr};  bool  mCoords  {false};

    nSDK2::cTV2ButtonLabel*        mGUIColorSwatchLabel  {nullptr};
    nSDK2::cTV2ButtonColorSwatch*  mGUIColorSwatch       {nullptr};  PIPixel  mColorSwatch  {false};

    nSDK2::cTV2ButtonLabel*        mGUIColorRangeLabel   {nullptr};
    nSDK2::cTV2ButtonColorRange*   mGUIColorRange        {nullptr};  nSDK2::cTV2ColorRange  mColorRange;

    nSDK2::cTV2ButtonLabel*        mGUIResultLabel       {nullptr};
    nSDK2::cTV2ButtonConsoleOut*   mGUIResult            {nullptr};
};


cTabMisc::cTabMisc( cDemoWindow*  iWindow ) :
    mWindow( iWindow ),
    mColorRange( iWindow->Plugin() )
{
}


void
cTabMisc::Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 140;
    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    x = 100;
    y = starty + 25;
    mGUITicks = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Grab Ticks"s,  &mTicks,
                                                [this]( int, bool  iValue )
                                                {
                                                    RegisterTicksHandler( iValue );
                                                } ); x += 110;
    mGUICoords = new  nSDK2::cTV2ButtonCheckBox( mWindow, id++, {x, y, 0, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Grab Coords"s, &mCoords,
                                                [this]( int, bool  iValue )
                                                {
                                                    RegisterCoordinatesHandler( iValue );
                                                } ); x += 110;

    mGUIColorSwatchLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {x, y, 90, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Color Picker :"s );  x += 90;
    mGUIColorSwatch = new  nSDK2::cTV2ButtonColorSwatch( mWindow, id++, {x, y, 90, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, &mColorSwatch,
                                                        [this]( int  iID, PIPixel  iValue )
                                                        {
                                                            //TODO: use fmt ?
                                                            char  tmp[512];
                                                            snprintf( tmp, sizeof(tmp), "Color Picked : id=%d, color=%d %d %d %d", iID, iValue.Red, iValue.Green, iValue.Blue, iValue.Alpha );
                                                            mGUIResult->DrawString( tmp );
                                                        } ); x += 110;

    mGUIColorRangeLabel  = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {x, y, 90, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Color Range :"s );  x += 90;
    mGUIColorRange = new  nSDK2::cTV2ButtonColorRange( mWindow, id++, {x, y, 90, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, &mColorRange,
                                                        [this]( int  iID, const nSDK2::cTV2ColorRange&  iValue )
                                                        {
                                                            //TODO: use fmt ?
                                                            char  tmp[512];
                                                            snprintf( tmp, sizeof(tmp), "Color Range Grabbed : id=%d", iID );
                                                            mGUIResult->DrawString( tmp );
                                                        } ); x += 110;

    x = 9;
    y += 20;

    mGUIResultLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {x, y, 150, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;
    mGUIResult      = new  nSDK2::cTV2ButtonConsoleOut( mWindow, kID_Result, {x, y, kTheButtonW, kTheButtonH, nSDK2::cPixelRectangle::eXYWH::kXYWH} );
    mGUIResult->DrawString( "<Nothing yet>"s );

    if( mTicks  ||  mCoords )
    {
        mGUIResult->DrawString( "<<Autoreactivation of ticks and/or coords>>"s );
        if( mTicks )
            RegisterTicksHandler( true );
        if( mCoords )
            RegisterCoordinatesHandler( true );
    }
}


void
cTabMisc::Hide()
{
    if( mTicks  ||  mCoords )
    {
        mGUIResult->DrawString( "<<Autodeactivation of ticks and/or coords>>"s );
        if( mTicks )
            RegisterTicksHandler( false );
        if( mCoords )
            RegisterCoordinatesHandler( false );
    }

    delete  mGUITicks;             mGUITicks       = nullptr;
    delete  mGUICoords;            mGUICoords      = nullptr;

    delete  mGUIColorSwatchLabel;  mGUIColorSwatchLabel = nullptr;
    delete  mGUIColorSwatch;       mGUIColorSwatch = nullptr;

    delete  mGUIColorRangeLabel;   mGUIColorRangeLabel = nullptr;
    delete  mGUIColorRange;        mGUIColorRange  = nullptr;

    delete  mGUIResultLabel;       mGUIResultLabel = nullptr;
    delete  mGUIResult;            mGUIResult      = nullptr;

    tSuperClass::Hide();
}


//---------------------------------------------------------------------------------------------------------------


void
cTabMisc::RegisterTicksHandler( bool  iRegister )
{
    using  namespace  nSDK2; // only for _Error !!

    if( iRegister )
    {
        mWindow->RegisterTicksHandler( "Misc"_Error, 
                                      [this]( const nSDK2::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY, bool  iIconic )
                                      {
                                            //TODO: use fmt ?
                                            char  tmp[512];
                                            snprintf( tmp, sizeof(tmp), "Tick( x=%d, y=%d, buttons=%d, pressure=%d, time=%d, ctrl=%d, reqx=%7.2lf, reqy=%7.2lf, iconic=%s )", iPosition.X(), iPosition.Y(), iMouseButton, iPressure, iTimeMS, iCtrlKey, iFX, iFY, iIconic?"true":"false" );
                                            mGUIResult->DrawString( tmp );
                                      } );
    }
    else
    {
        mWindow->UnregisterTicksHandler( "Misc"_Error ); 
    }
}


void
cTabMisc::RegisterCoordinatesHandler( bool  iRegister )
{
    using  namespace  nSDK2; // only for _Error !!

    if( iRegister )
    {
        mWindow->RegisterCoordinatesHandler( "Misc"_Error, 
                                      [this]( const nSDK2::cPixelPosition&  iPosition, int  iMouseButton, int  iPressure, int  iTimeMS, bool  iCtrlKey, double  iFX,  double  iFY )
                                      {
                                            //TODO: use fmt ?
                                            char  tmp[512];
                                            snprintf( tmp, sizeof(tmp), "Coords( x=%d, y=%d, buttons=%d, pressure=%d, time=%d, ctrl=%d, reqx=%7.2lf, reqy=%7.2lf )", iPosition.X(), iPosition.Y(), iMouseButton, iPressure, iTimeMS, iCtrlKey, iFX, iFY );
                                            mGUIResult->DrawString( tmp );
                                      } );
    }
    else
    {
        mWindow->UnregisterCoordinatesHandler( "Misc"_Error ); 
    }
}


//---------------------------------------------------------------------------------------------------------------


nSDK2::cTV2Tab*
NewTabMisc( cDemoWindow*  iWindow )
{
    return  new  cTabMisc( iWindow );
}

