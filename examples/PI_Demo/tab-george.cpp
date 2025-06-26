/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-george.cpp
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating George calls.
*/


#include "SDK2.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>


using  namespace  std::literals::string_literals;


//TODO: move to own file...
class  cHistory
{
public:
    static  constexpr int  kMaxHistory = 50;

public:
    int  Count() const  { return  mCount; }
    const std::string&  History( int  iIndex ) const  { return  mHistory[iIndex]; }

    // adds or move to front if already exists
    void  Add( const std::string&  iString )
    {
        for( int  i = 0; i < mCount; ++i )
        {
            if( iString == mHistory[i] )
            {
                // move to front
                for( int  j = i; j < mCount-1; ++j )
                {
                    mHistory[j] = mHistory[j+1];
                }
                mHistory[mCount-1] = iString;
                return;
            }
        }

        // do we need to make some space ?
        if( mCount == kMaxHistory )
        {
            for( int  i = 0; i < kMaxHistory-1; ++i )
            {
                mHistory[i] = mHistory[i+1];
            }
        }

        mHistory[mCount] = iString;
        ++mCount;
    }

private:
    //TODO: maybe switch to std::vector ?
    int  mCount  {0};
    std::string  mHistory[kMaxHistory];
};


//MARK: ========================================================================================================= George Tab


constexpr  int  kTheButtonX = 9;
constexpr  int  kTheButtonY = 85;
constexpr  int  kTheButtonW = 880;
constexpr  int  kTheButtonH = 165;


enum  eIDs
{
    kID_First = 9000,

    kID_TopLabel = kID_First+1,
    kID_TheButtonLabel = kID_First+2,
    
    kID_Result = kID_First+900,
};


//MARK: ========================================================================================================= cTabGeorge


class  cTabGeorge :
    public  nSDK2::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabGeorge()  override;
    cTabGeorge( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "George"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

public:
    void  Execute();
    void  DoHistory();

private:
    cDemoWindow*  mWindow;

    cHistory  mHistory;
    bool  mSetHistory  {false};

    nSDK2::cTV2ButtonLabel*  mTopLabel  {nullptr};

    nSDK2::cTV2ButtonTextString*  mGUICommand  {nullptr};  std::string  mCommand;
    nSDK2::cTV2ButtonAction*  mGUIExecute  {nullptr};
    nSDK2::cTV2ButtonPopup*  mGUIHistory  {nullptr};

    nSDK2::cTV2ButtonConsoleOut*  mGUIResult  {nullptr};
};


cTabGeorge::cTabGeorge( cDemoWindow*  iWindow ) :
    mWindow( iWindow )
{
}


void
cTabGeorge::Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 9;
    int  y = iArea.Y1();
    int  id = kID_First+10;

    mTopLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {x, y, 80, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Command :"s );  y += 20;

    x = 9;
    y = iArea.Y1() + 25;

    mGUICommand = new  nSDK2::cTV2ButtonTextString( mWindow, id++, {x, y, 740, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, ""s, &mCommand, [this](int,const std::string&){ Execute(); } );  x += 750;

    mGUIExecute = new  nSDK2::cTV2ButtonAction( mWindow, id++, {x, y, 60, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Execute"s, [this](int){ Execute(); } );  x += 70;

    mGUIHistory = new  nSDK2::cTV2ButtonPopup( mWindow, id++, {x, y, 60, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "History"s, [this](int){ DoHistory(); } );  y += 20;

    mGUIResult = new  nSDK2::cTV2ButtonConsoleOut( mWindow, kID_Result, {kTheButtonX, kTheButtonY, kTheButtonW, kTheButtonH, nSDK2::cPixelRectangle::eXYWH::kXYWH} );

    mGUIResult->DrawString( "<Nothing yet>"s );
}


void
cTabGeorge::Hide()
{
    delete  mTopLabel;    mTopLabel   = nullptr;

    delete  mGUICommand;  mGUICommand = nullptr;
    delete  mGUIExecute;  mGUIExecute = nullptr;
    delete  mGUIHistory;  mGUIHistory = nullptr;
    delete  mGUIResult;   mGUIResult  = nullptr;

    tSuperClass::Hide();
}


void
cTabGeorge::Execute()
{
    if( mSetHistory ) // skip if we used the history popup...
        return;

    if( mCommand == ""s )
    {
        mGUIResult->DrawString( "<<Empty command>>"s );
        return;
    }

    mHistory.Add( mCommand );

    std::string  tmp = "CMD: "s + mCommand;
    mGUIResult->DrawString( tmp );

    char  result[1024];
    strcpy( result, "<No result>" );
    if( TVSendCmd( mWindow->Plugin()->LegacyPlugin(), mCommand.c_str(), result ) )
    {
        tmp = "RES: "s + result;
        mGUIResult->DrawString( tmp );
    }
    else
    {
        mGUIResult->DrawString( "<<Command failed>>"s );
    }
}


void
cTabGeorge::DoHistory()
{
//TODO: use new C++ popup when they are done !!
    int  cnt = 0;
    PIPopup  pop[cHistory::kMaxHistory];

    for( int  i = 0; i < mHistory.Count(); ++i )
    {
        pop[cnt].Name  = mHistory.History( mHistory.Count() - i - 1 ).c_str();
        pop[cnt].ID    = i+1; // +1 to avoid the forbidden 0 !
        pop[cnt].Flags = 0;

        cnt++;
    }

    if( cnt == 0 )
    {
        pop[0].Name  = "<Empty>";
        pop[0].ID    = 1;
        pop[0].Flags = bPIPopMode_Quiet;
        TVPopup( mWindow->Plugin()->LegacyPlugin(), pop, 1, 0 );
        return;
    }

    int  i = TVPopup( mWindow->Plugin()->LegacyPlugin(), pop, cnt, 0 );
    --i; // adjust to compensate for the +1 in the popup creation

    if( i >= 0 )
    {
        mSetHistory = true;
        mGUICommand->Value( mHistory.History( mHistory.Count() - i - 1 ) );
        mSetHistory = false;
        mGUICommand->Activate();
//TODO:        TVChangeButtonReq( iFilter, iReq, ID_COMMAND, bPIButtonFlags_Text|bPIButtonFlags_TextActive, NULL );
    }
}


//---------------------------------------------------------------------------------------------------------------


nSDK2::cTV2Tab*
NewTabGeorge( cDemoWindow*  iWindow )
{
    return  new  cTabGeorge( iWindow );
}

