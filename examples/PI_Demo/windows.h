/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  tabs.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  The tab public interface.
*/

#ifndef __tabs_HH
#define __tabs_HH


typedef  struct
{
    void  (*Open)( PIPlugin*  iPlugin, INTPTR  iReq );
    void  (*Close)( PIPlugin*  iPlugin, INTPTR  iReq );
    int   (*Msg)( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs );
} DemoWindow;


extern  DemoWindow  gButtonWindow;
extern  DemoWindow  gTextWindow;
extern  DemoWindow  gPopupWindow;
extern  DemoWindow  gSliderWindow;
extern  DemoWindow  gMetaWindow;
extern  DemoWindow  gReqWindow;
extern  DemoWindow  gProjectWindow;
extern  DemoWindow  gGeorgeWindow;
extern  DemoWindow  gMiscWindow;
extern  DemoWindow  gPreviewWindow;


extern  int  gStartSubWindowY;


#endif // __tabs_HH
