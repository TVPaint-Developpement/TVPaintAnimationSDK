/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Tabs.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A tab group.
 */


#include "TVPaintAnimationSDK/Button-Tabs.hpp"


#include "TVPaintAnimationSDK/Plugin.hpp"
#include "TVPaintAnimationSDK/Window.hpp"


#include <cstdlib>


using  namespace  std::literals::string_literals;


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Tab


cTV2Tab::~cTV2Tab()
{
}


cTV2Tab::cTV2Tab()
{
}


void
cTV2Tab::Show( cTV2Window*  iWindow, const cPixelRectangle&  iArea )
{
    mWindow = iWindow;
}


void
cTV2Tab::Hide()
{
    mWindow = nullptr;
}


//MARK: ========================================================================================================= cTV2ButtonTabs


cTV2ButtonTabs::~cTV2ButtonTabs()
{
    mTabs[mSelectedTabIndex]->Hide();

    for( auto&& tab: mTabs )
        delete  tab;

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();

    TVRemoveButtonReq( plugin, req, mID );
}


cTV2ButtonTabs::cTV2ButtonTabs( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::vector< cTV2Tab* >&  iTabs, int  iSelectedTabIndex ) :
    cTV2Button( iWindow, iID, iArea ),
    mTabs( iTabs ),
    mSelectedTabIndex( iSelectedTabIndex )
{
    constexpr  int  kMaxTabs = 20;

    PIPlugin*  plugin = mWindow->Plugin()->LegacyPlugin();
    INTPTR  req = mWindow->LegacyReq();
printf("cTV2ButtonTabs::cTV2ButtonTabs() -> plugin=%p, req=%d\n",plugin,int(req));

    int  tabcount = std::min( int(mTabs.size()), kMaxTabs );
    const char*  tabnames[kMaxTabs];
    for( int  i = 0; i < tabcount; ++i )
        tabnames[i] = strdup(mTabs[i]->Name().c_str());
    iSelectedTabIndex = std::min( iSelectedTabIndex, kMaxTabs );

//TODO: //CHECK: i *think* tvpaint will make a copy of tabnames...
    //TODO: 20 = hardcoded height !
    mArea.Y1( mArea.Y1() + 20 );
    if( !TVAddTabs( plugin, req, iArea.X(), iArea.Y(), iArea.W(), 20, iID, tabcount, mSelectedTabIndex, tabnames ) )
    {
        for( int  i = 0; i < tabcount; ++i )
            free( (void*)tabnames[i] );
        return;
    }
printf("mTabs->Show()\n");
    mTabs[mSelectedTabIndex]->Show( mWindow, mArea );
printf("<<cTV2ButtonTabs::cTV2ButtonTabs()\n");

    for( int  i = 0; i < tabcount; ++i )
        free( (void*)tabnames[i] );
}


tErrorCode
cTV2ButtonTabs::EventButtonTabChanged( int  iButtonID, int  iCurrent, int  iTimeMS, bool  iCtrlKey )
{
    assert( iButtonID == mID );

    mTabs[mSelectedTabIndex]->Hide();
    mSelectedTabIndex = iCurrent;
    mTabs[mSelectedTabIndex]->Show( mWindow, mArea );

    return  kErrorCode_NoError;
}


} // namespace  nSDK

