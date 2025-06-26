/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tab-preview.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab for demonstrating the preview of some action.
*/


#include "SDK2.h"


#include "TVPaintSDK.h"


#include "demo.h"


#include <stdlib.h>


//MARK: ======================================================================================================== Preview Tab


constexpr  int  kTheButtonX    = 350;
constexpr  int  kTheButtonY    =  70;
constexpr  int  kTheButtonSize = 128;


enum  eIDs
{
    kID_First = 11000,

    kID_TheButton = kID_First+900,
};


enum  ePreviewType
{
    kNone = -1,
    kFullScreen = 1,
    kIconic = 2
};


static void
Negative( PIBlock*  oDst, const PIBlock*  iSrc )
{
    assert( oDst->Width  == iSrc->Width  );
    assert( oDst->Height == iSrc->Height );

    // some initializations
    int  size = oDst->Width * oDst->Height;
    PIPixel*  src = iSrc->Data;
    PIPixel*  dst = oDst->Data;

    // loop over all the pixels, here we don't care about their x/y positions...
    for( int  i = 0; i < size; ++i )
    {
        // get the source alpha channel
        int  a = src->Alpha;

        if( a == 0 )
        {
            // if fully transparent, destination will also be fully transparent
            dst->l = 0x00000000;
        }
        else if( a == 255 )
        {
            // if fully opaque, the channel values are the color values
            // (see the SDK docs for the meaning of this sentence :)

            dst->Red   = 255-src->Red;
            dst->Green = 255-src->Green;
            dst->Blue  = 255-src->Blue;
            dst->Alpha = 255;
        }
        else
        {
            // if neither fully transparent nor fully opaque,
            // the channel values have to be computed from the color values and the alpha channel
            // (see the SDK docs for the meaning of this sentence :)

            int  r = (src->Red   * 255) / a;
            int  g = (src->Green * 255) / a;
            int  b = (src->Blue  * 255) / a;

            dst->Red   = ((255-r) * a) / 255;
            dst->Green = ((255-g) * a) / 255;
            dst->Blue  = ((255-b) * a) / 255;
            dst->Alpha = a;
        }

        // and on to the next pixel..
        ++src;
        ++dst;
    }
}


//MARK: ========================================================================================================= cTabPreview


class  cTabPreview :
    public  nSDK2::cTV2Tab
{
public:
    using  tSuperClass = cTV2Tab;

public:
//    virtual  ~cTabPreview()  override  { Hide(); }
    cTabPreview( cDemoWindow*  iWindow );

public: // properties
    virtual  std::string  Name() const  override  { return  "Preview"s; }
    virtual  int  FirstID() const  override  { return  kID_First; }
    virtual  int  LastID() const  override  { return  kID_First+999; }

public:
    virtual  void  Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )  override;
    virtual  void  Hide()  override;

private:
    void  DoPreview();

private:
    cDemoWindow*  mWindow;

    nSDK2::cTV2RadioGroup  mPreviewMode;

    nSDK2::cTV2ButtonLabel*    mGUITopLabel      {nullptr};

    nSDK2::cTV2ButtonRadio*    mGUIFullScreen    {nullptr};
    nSDK2::cTV2ButtonRadio*    mGUIIconic        {nullptr};

    nSDK2::cTV2ButtonLabel*    mGUIPreviewLabel  {nullptr};
    nSDK2::cTV2ButtonPreview*  mGUIPreview       {nullptr};
};


cTabPreview::cTabPreview( cDemoWindow*  iWindow ) :
    mWindow( iWindow ),
    mPreviewMode( 0, [this]( int  iValue ){ DoPreview(); }, true )
{
}


void
cTabPreview::Show( nSDK2::cTV2Window*  iWindow, const nSDK2::cPixelRectangle&  iArea )
{
    tSuperClass::Show( iWindow, iArea );

    int  x = 140;
    int  starty = iArea.Y1() + 3;
    int  y = starty;
    int  id = kID_First+1;

    mGUITopLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {0, y, 360, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Preview"s ); y += 20;

    x = 100;
    y = starty + 25;
    mGUIFullScreen = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Full Screen"s, kFullScreen, &mPreviewMode );  y += 20;
    mGUIIconic     = new  nSDK2::cTV2ButtonRadio( mWindow, id++, {x, y, 100, 0, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "Iconic"s,      kIconic,     &mPreviewMode );  y += 20;


    mGUIPreviewLabel = new  nSDK2::cTV2ButtonLabel( mWindow, id++, {kTheButtonX, kTheButtonY-30, 150, 20, nSDK2::cPixelRectangle::eXYWH::kXYWH}, "What you get ..."s ); y += 20;
    mGUIPreview = new  nSDK2::cTV2ButtonPreview( mWindow, id++, {kTheButtonX, kTheButtonY, kTheButtonSize, kTheButtonSize, nSDK2::cPixelRectangle::eXYWH::kXYWH}, kTheButtonSize );

    DoPreview();
}


void
cTabPreview::Hide()
{
    delete  mGUITopLabel;      mGUITopLabel     = nullptr;

    delete  mGUIFullScreen;    mGUIFullScreen   = nullptr;
    delete  mGUIIconic;        mGUIIconic       = nullptr;

    delete  mGUIPreviewLabel;  mGUIPreviewLabel = nullptr;
    delete  mGUIPreview;       mGUIPreview      = nullptr;

    tSuperClass::Hide();
}


void
cTabPreview::DoPreview()
{
    switch( mPreviewMode.Value() )
    {
        case  kNone:
        {
            mGUIPreview->ClearPreview();
        }
        break;

        case  kFullScreen:
        {
            mGUIPreview->ClearPreview();

            auto  plugin = mWindow->Plugin()->LegacyPlugin();
//TODO: make easier to use fcts !!!!!!!!!!!
            TVSendCmd( plugin, "tv_lockmouse 2", nullptr );
            Negative( plugin->Current, plugin->Undo );
            TVUpdateDisplay( plugin, plugin->WorkArea_x1, plugin->WorkArea_y1, plugin->WorkArea_x2, plugin->WorkArea_y2 );
            memcpy( plugin->Current->Data, plugin->Undo->Data, plugin->Current->Width*plugin->Current->Height*sizeof(PIPixel32) );
            TVSendCmd( plugin, "tv_unlockmouse", nullptr );
        }
        break;

        case  kIconic:
        {
printf("mGUIPreview->StartPreview()\n");
            mGUIPreview->StartPreview();
printf("mGUIPreview->Negative()\n");
            Negative( mGUIPreview->Dst(), mGUIPreview->Src() );
printf("mGUIPreview->FinishPreview()\n");
            mGUIPreview->FinishPreview();
        }
        break;
    }
}


//---------------------------------------------------------------------------------------------------------------


nSDK2::cTV2Tab*
NewTabPreview( cDemoWindow*  iWindow )
{
    return  new  cTabPreview( iWindow );
}



#if 0
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Preview Subwindow

static struct
{
    int  mType;

    Preview*  mPreview;
} sgToggles;



static void
Open( PIFilter* iFilter, INTPTR iReq )
{
    int  x = 140;
    int  y = gStartSubWindowY;

    x = 140;
    y = gStartSubWindowY + 25;

    sgToggles.mPreview = PreviewNew( iFilter, iReq, ID_THE_BUTTON, THE_BUTTON_X, THE_BUTTON_Y, THE_BUTTON_SIZE );

    if( sgToggles.mType != kNone )
    {
        if( sgToggles.mType == kFullScreen )
            TVInstallFunction( iFilter, kPIMeta_Preview );
        DoPreview( iFilter, iReq );
    }
}


static void
Close( PIFilter* iFilter, INTPTR iReq )
{
    if( sgToggles.mType == kFullScreen )
    {
        TVCloseFunction( iFilter );
    }

    if( sgToggles.mPreview )
    {
        PreviewDelete( iFilter, sgToggles.mPreview );
        sgToggles.mPreview = NULL;
    }
}


static int
Msg( PIFilter* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    switch( iEvent )
    {
    case PICMETA_CLOSE:
        TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check, NULL );
        TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check, NULL );
        sgToggles.mType = kNone;
        return  1;

    case PICMETA_PREVIEW:
        DoPreview( iFilter, iReq );
        return  1;

    case PICBREQ_PROJECT_CHANGE:
        DoPreview( iFilter, iReq );
        return  1;

    case PICBREQ_BUTTON_UP:
    {
        switch( iArgs[0] )
        {
        case ID_FULLSCREEN:
            if( sgToggles.mType == kFullScreen )
            {
                TVCloseFunction( iFilter );
                DoPreview( iFilter, iReq );
            }
            else
            {
                TVInstallFunction( iFilter, kPIMeta_Preview );
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check|bPIButtonFlags_Select, NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check,                       NULL );
                sgToggles.mType = kFullScreen;
                DoPreview( iFilter, iReq );
            }
            return  1;

        case ID_ICONIC:
            if( sgToggles.mType == kIconic )
            {
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check, NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check, NULL );
                sgToggles.mType = kNone;
                DoPreview( iFilter, iReq );
            }
            else
            {
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check,                       NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check|bPIButtonFlags_Select, NULL );
                sgToggles.mType = kIconic;
                DoPreview( iFilter, iReq );
            }
            return  1;
        }
    }
    break;
    }

    return  0;
}




DemoWindow  gPreviewWindow =
{
    Open,
    Close,
    Msg
};
#endif//0


#if 0
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// Preview Subwindow


#define THE_BUTTON_X   350
#define THE_BUTTON_Y    70
#define THE_BUTTON_SIZE   128
/////#define THE_BUTTON_H   128

#define START  11000


#define ID_TOP_LABEL         (START+001)
#define ID_THE_BUTTON_LABEL  (START+002)

#define ID_FULLSCREEN        (START+100)
#define ID_ICONIC            (START+101)

#define ID_THE_BUTTON        (START+999)



enum PreviewType
{
    kNone,
    kFullScreen,
    kIconic
};


static struct
{
    int  mType;

    Preview*  mPreview;
} sgToggles;


static void
DoPreview( PIFilter* iFilter, INTPTR iReq )
{
    switch( sgToggles.mType )
    {
    case kNone:
    {
        TVPutButtonImage( iFilter, iReq, ID_THE_BUTTON, NULL, 0 );
    }
    break;

    case kFullScreen:
    {
        TVPutButtonImage( iFilter, iReq, ID_THE_BUTTON, NULL, 0 );

        TVSendCmd( iFilter, "tv_lockmouse 2", NULL );
        Negative( iFilter->Undo, iFilter->Current );
        TVUpdateDisplay( iFilter, iFilter->WorkArea_x1, iFilter->WorkArea_y1, iFilter->WorkArea_x2, iFilter->WorkArea_y2 );
        memcpy( iFilter->Current->Data, iFilter->Undo->Data, iFilter->Current->Width*iFilter->Current->Height*sizeof(PIPixel32) );
        TVSendCmd( iFilter, "tv_unlockmouse", NULL );
    }
    break;

    case kIconic:
    {
        sgToggles.mPreview->StartPreview( sgToggles.mPreview, iFilter );
        Negative( sgToggles.mPreview->mSrc, sgToggles.mPreview->mDst );
        sgToggles.mPreview->FinishPreview( sgToggles.mPreview, iFilter );
    }
    break;
    }
}


static void
Open( PIFilter* iFilter, INTPTR iReq )
{
    int  x = 140;
    int  y = gStartSubWindowY;

    static int  first = 1;
    if( first )
    {
        first = 0;

        sgToggles.mType = kNone;
        sgToggles.mPreview = NULL;
    }

    TVAddButtonReq( iFilter, iReq, 0, y, 200, 20, ID_TOP_LABEL, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, "Preview" ); y += 20;

    x = 140;
    y = gStartSubWindowY + 25;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_FULLSCREEN, bPIButtonFlags_Check|((sgToggles.mType==kFullScreen)? bPIButtonFlags_Select:0), "Full Screen Preview" ); y += 20;
    TVAddButtonReq( iFilter, iReq, x, y, 0, 0, ID_ICONIC,     bPIButtonFlags_Check|((sgToggles.mType==kIconic)?     bPIButtonFlags_Select:0), "Iconic Preview"      ); y += 20;

    TVAddButtonReq( iFilter, iReq, THE_BUTTON_X, THE_BUTTON_Y-30, 150, 20, ID_THE_BUTTON_LABEL, bPIButtonFlags_Flat|bPIButtonFlags_Quiet, "What you get ..." ); y += 20;

    if( sgToggles.mPreview )
    {
        PreviewDelete( iFilter, sgToggles.mPreview );
    }
    sgToggles.mPreview = PreviewNew( iFilter, iReq, ID_THE_BUTTON, THE_BUTTON_X, THE_BUTTON_Y, THE_BUTTON_SIZE );

    if( sgToggles.mType != kNone )
    {
        if( sgToggles.mType == kFullScreen )
            TVInstallFunction( iFilter, kPIMeta_Preview );
        DoPreview( iFilter, iReq );
    }
}


static void
Close( PIFilter* iFilter, INTPTR iReq )
{
    TVRemoveButtonReq( iFilter, iReq, ID_TOP_LABEL );

    TVRemoveButtonReq( iFilter, iReq, ID_FULLSCREEN );
    TVRemoveButtonReq( iFilter, iReq, ID_ICONIC );

    TVRemoveButtonReq( iFilter, iReq, ID_THE_BUTTON_LABEL );
    TVRemoveButtonReq( iFilter, iReq, ID_THE_BUTTON );

    if( sgToggles.mType == kFullScreen )
    {
        TVCloseFunction( iFilter );
    }

    if( sgToggles.mPreview )
    {
        PreviewDelete( iFilter, sgToggles.mPreview );
        sgToggles.mPreview = NULL;
    }
}


static int
Msg( PIFilter* iFilter, INTPTR iEvent, INTPTR iReq, INTPTR* iArgs )
{
    switch( iEvent )
    {
    case PICMETA_CLOSE:
        TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check, NULL );
        TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check, NULL );
        sgToggles.mType = kNone;
        return  1;

    case PICMETA_PREVIEW:
        DoPreview( iFilter, iReq );
        return  1;

    case PICBREQ_PROJECT_CHANGE:
        DoPreview( iFilter, iReq );
        return  1;

    case PICBREQ_BUTTON_UP:
    {
        switch( iArgs[0] )
        {
        case ID_FULLSCREEN:
            if( sgToggles.mType == kFullScreen )
            {
                TVCloseFunction( iFilter );
                DoPreview( iFilter, iReq );
            }
            else
            {
                TVInstallFunction( iFilter, kPIMeta_Preview );
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check|bPIButtonFlags_Select, NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check,                       NULL );
                sgToggles.mType = kFullScreen;
                DoPreview( iFilter, iReq );
            }
            return  1;

        case ID_ICONIC:
            if( sgToggles.mType == kIconic )
            {
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check, NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check, NULL );
                sgToggles.mType = kNone;
                DoPreview( iFilter, iReq );
            }
            else
            {
                TVChangeButtonReq( iFilter, iReq, ID_FULLSCREEN, bPIButtonFlags_Check,                       NULL );
                TVChangeButtonReq( iFilter, iReq, ID_ICONIC,     bPIButtonFlags_Check|bPIButtonFlags_Select, NULL );
                sgToggles.mType = kIconic;
                DoPreview( iFilter, iReq );
            }
            return  1;
        }
    }
    break;
    }

    return  0;
}




DemoWindow  gPreviewWindow =
{
    Open,
    Close,
    Msg
};
#endif//0


