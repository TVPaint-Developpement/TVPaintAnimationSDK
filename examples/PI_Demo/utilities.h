/** @copyright (c) 2005-24 TVPaint Developpement. All Rights Reserved.
*
*  @file  utilities.h
*  @ingroup  tvpa_demo_filter
*
*  @brief  Some utility functions for the demo plugin.
*/

#pragma  once


#include "TVPaintAnimationSDK/TVPaintSDK.h"


#include <string>


#define ARG2FLOAT(x)  (((int)iArgs[x])/65536.)


std::string  EventToString( const char*  iPrefix, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iArgs );

