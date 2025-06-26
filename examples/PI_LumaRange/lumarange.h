////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// The "lumarange" effect



#ifndef __lumarange_H
#define __lumarange_H


#include  "TVPaintSDK.h"
#include  "colorrange.h"


// The channel used as source for the effect.
typedef enum
{
    kLuminosity = 1,
    kAlpha,
    kRed,
    kGreen,
    kBlue
} eLumaRangeChannel;


// Parameters for the effect.
typedef struct
{
    eLumaRangeChannel  mChannel; // source channel
    int                mReplace; // replace mode
    ColorRange*        mColors;  // color range
} LumaRangeParams;


extern void  LumaRange( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams );


#endif


