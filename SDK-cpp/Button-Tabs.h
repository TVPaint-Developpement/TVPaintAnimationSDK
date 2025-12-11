/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Button-Tabs.h
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A tab group.
 */

#pragma once


#include "Button.h"
#include "Types.h"
#include "Window.h"


#include <functional>
#include <string>
#include <vector>

using  namespace  std::literals::string_literals;//ZAP:


namespace  nSDK {


//MARK: ========================================================================================================= cTV2Tab


class  cTV2Tab
{
public:
//    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2Tab();
    cTV2Tab();

public: // properties
    virtual  std::string  Name() const = 0;

    virtual  int  FirstID() const = 0;
    virtual  int  LastID() const = 0;

public:
    // you MUST override and call this FIRST
    virtual  void  Show( cTV2Window*  iWindow, const cPixelRectangle&  iArea );
    // you MUST override and call this LAST
    virtual  void  Hide();

protected:
    cTV2Window*  mWindow  {nullptr};
};


//MARK: ========================================================================================================= cTV2ButtonTabs


/** @brief  A tab bar. It needs a vector of cTV2Tab. */
class  cTV2ButtonTabs :
    public  cTV2Button
{
public:
    using  tSuperClass = cTV2Button;

public:
    virtual  ~cTV2ButtonTabs()  override;
    cTV2ButtonTabs( cTV2Window*  iWindow, int  iID, const cPixelRectangle&  iArea, const std::vector< cTV2Tab* >&  iTabs, int  iSelectedTabIndex );

public: // event handling
    virtual  tErrorCode  EventButtonTabChanged( int  iButtonID, int  iCurrent, int  iTimeMS, bool  iCtrlKey )  override;

private:
//    cPixelRectangle  mArea;
    std::vector< cTV2Tab* >  mTabs;
    int  mSelectedTabIndex  {-1};
};


} // namespace  nSDK
