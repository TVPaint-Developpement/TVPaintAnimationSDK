////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// A color range "object"


#ifndef __colorrange_H
#define __colorrange_H


#ifdef __cplusplus
extern "C" {
#endif


#include  "TVPaintSDK.h"


typedef struct ColorRange
{
    PIPixel  mColors[256];

    // methods
    void  (*Load)( struct ColorRange* ioRange, PIPlugin* iFilter );
    void  (*Save)( const struct ColorRange* iRange,  PIPlugin* iFilter );
    void  (*Grab)( struct ColorRange* oRange,  PIPlugin* iFilter);

    PIBlock*  (*MakeBlock)( const struct ColorRange* iRange, PIPlugin* iFilter, int iW, int iH );
} ColorRange;


extern  ColorRange*  ColorRangeNew( PIPlugin* iFilter );
extern  void         ColorRangeDelete( PIPlugin* iFilter, ColorRange* iRange );


#ifdef __cplusplus
}
#endif


#endif // __colorrange_H
