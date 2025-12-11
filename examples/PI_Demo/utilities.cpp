/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  utilities.c
*  @ingroup  tvpa_demo_filter
*
*  @brief  Some utility functions for the demo plugin.
*/

#include "utilities.h"


#include <stdio.h> // use fmt::fmt() instead of snprintf()...


std::string
EventToString( const char*  iPrefix, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs )
{
    char  tmp[256];

    switch( iEvent )
    {
        case kPIEvents_WindowOpen:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowOpen<0x%08llx>( x=%d, y=%d, w=%d, h=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_WindowClose:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowClose<0x%08llx>( x=%d, y=%d, w=%d, h=%d, shutdown=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3], (int)iArgs[4] );
            break;

        case kPIEvents_WindowDragged:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowDragged<0x%08llx>( x=%d, y=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1] );
            break;

        case kPIEvents_WindowResizing:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowResizing<0x%08llx>( w=%d, h=%d, x=%d, y=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_WindowResized:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowResized<0x%08llx>( w=%d, h=%d, x=%d, y=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_WindowHidden:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowHidden<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_WindowShown:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowShown<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_WindowCollapsed:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowCollapsed<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_WindowUncollapsed:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowUncollapsed<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_WindowKeyDown:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowKeyDown<0x%08llx>( ascii=%d )", iPrefix, iReq, (int)iArgs[0] );
            break;

        case kPIEvents_WindowFunctionKeyDown:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_WindowFunctionKeyDown<0x%08llx>( num=%d )", iPrefix, iReq, (int)iArgs[0] );
            break;

        case kPIEvents_ProjectChanged:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ProjectChanged<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_ProjectInactivated:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ProjectInactivated<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_ProjectActivated:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ProjectActivated<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_KeysetReset:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_KeysetReset<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_KeysetLoad:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_KeysetLoad<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_KeysetSave:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_KeysetSave<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_KeysetSaveAs:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_KeysetSaveAs<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_Coordinates:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_Coordinates<0x%08llx>( id=%d, x=%7.2f, y=%7.2f, scrx=%7.2f, scry=%7.2f, pressure=%d, time=%d, ctrl=%d, reqx=%d, reqy=%d )", iPrefix, iReq, (int)iArgs[0], ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), ARG2FLOAT(4), (int)iArgs[5], (int)iArgs[6], (int)iArgs[7], (int)iArgs[8], (int)iArgs[9] );
            break;

        case kPIEvents_Ticks:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_Ticks<0x%08llx>( id=%d, x=%7.2f, y=%7.2f, scrx=%7.2f, scry=%7.2f, pressure=%d, time=%d, ctrl=%d, reqx=%d, reqy=%d, iconic=%d )", iPrefix, iReq, (int)iArgs[0], ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), ARG2FLOAT(4), (int)iArgs[5], (int)iArgs[6], (int)iArgs[7], (int)iArgs[8], (int)iArgs[9], (int)iArgs[10] );
            break;

        case kPIEvents_RedrawHud:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_RedrawHud<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_ColorPicked:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ColorPicked<0x%08llx>( x=%d, y=%d, pressure=%d, ctrl=%d, color=0x%08x )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3], (int)iArgs[4] );
            break;

        case kPIEvents_RoomChanged:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_RoomChanged<0x%08llx>( name=%s )", iPrefix, iReq, (char*)iArgs[0] );
            break;



        case kPIEvents_ButtonDown:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonDown<0x%08llx>( id=%d, x=%d, y=%d, mouse=%d, pressure=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3], (int)iArgs[4], (int)iArgs[5], (int)iArgs[6] );
            break;

        case kPIEvents_ButtonUp:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonUp<0x%08llx>( id=%d, x=%d, y=%d, mouse=%d, pressure=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3], (int)iArgs[4], (int)iArgs[5], (int)iArgs[6] );
            break;

        case kPIEvents_ButtonText:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonText<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_ButtonMiniSlider:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonMiniSlider<0x%08llx>( id=%d, delta=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_ButtonSliderDragged:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonSliderDragged<0x%08llx>( id=%d, posx=%7.2f, posy=%7.2f, button=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], ARG2FLOAT(1), ARG2FLOAT(2), (int)iArgs[3], (int)iArgs[4], (int)iArgs[5] );
            break;

        case kPIEvents_ButtonSliderReleased:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonSliderReleased<0x%08llx>( id=%d, posx=%7.2f, posy=%7.2f, button=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], ARG2FLOAT(1), ARG2FLOAT(2), (int)iArgs[3], (int)iArgs[4], (int)iArgs[5] );
            break;

        case kPIEvents_ButtonTabChanged:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_ButtonTabChanged<0x%08llx>( id=%d, tab=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_MouseMove:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MouseMove<0x%08llx>( id=%d, x=%d, y=%d, mouse=%d, pressure=%d, time=%d, ctrl=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3], (int)iArgs[4], (int)iArgs[5], (int)iArgs[6] );
            break;



        case kPIEvents_MetaClosed:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaClosed<0x%08llx>()", iPrefix, iReq );
            break;

        case kPIEvents_MetaPoint:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaPoint<0x%08llx>( fx=%7.2lf, fy=%7.2lf, button=%d, pressure=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_MetaFreehand:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaFreehand<0x%08llx>( fx=%7.2lf, fy=%7.2lf, button=%d, pressure=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_MetaLine:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaLine<0x%08llx>( ofx=%7.2lf, ofy=%7.2lf, fx=%7.2lf, fy=%7.2lf, button=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), (int)iArgs[4] );
            break;

        case kPIEvents_MetaRectangle:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaRectangle<0x%08llx>( ofx=%7.2lf, ofy=%7.2lf, fx=%7.2lf, fy=%7.2lf, button=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), (int)iArgs[4] );
            break;

        case kPIEvents_MetaCircle:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaCircle<0x%08llx>( ofx=%7.2lf, ofy=%7.2lf, fr=%7.2lf, button=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), (int)iArgs[3] );
            break;

        case kPIEvents_MetaEllipse:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaEllipse<0x%08llx>( ofx=%7.2lf, ofy=%7.2lf, fa=%7.2lf, fb=%7.2lf, button=%d )", iPrefix, iReq, ARG2FLOAT(0), ARG2FLOAT(1), ARG2FLOAT(2), ARG2FLOAT(3), (int)iArgs[4] );
            break;

        case kPIEvents_MetaPreview:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaPreview<0x%08llx>( x=%d, y=%d, w=%d, h=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[1], (int)iArgs[2], (int)iArgs[3] );
            break;

        case kPIEvents_MetaKeyboard:
            snprintf( tmp, sizeof(tmp), "%s: kPIEvents_MetaKeyboard<0x%08llx>( code=%d, qualifiers=%d, mousebutton=%d )", iPrefix, iReq, (int)iArgs[0], (int)iArgs[3], (int)iArgs[4] );
            break;



// These two are deprecated, a warning about deprecation is normal...
        case PICBREQ_VIDEO_CHANGE_PREV:
            snprintf( tmp, sizeof(tmp), "%s: PICBREQ_VIDEO_CHANGE_PREV<0x%08llx>  DEPRECATED", iPrefix, iReq );
            break;

        case PICBREQ_VIDEO_CHANGE:
            snprintf( tmp, sizeof(tmp), "%s: PICBREQ_VIDEO_CHANGE<0x%08llx>  DEPRECATED", iPrefix, iReq );
            break;



        default:
            snprintf( tmp, sizeof(tmp), "%s: UNKNOWN MESSAGE <%d> !!", iPrefix, (int)iEvent );
            break;
    }

    return  tmp;
}

