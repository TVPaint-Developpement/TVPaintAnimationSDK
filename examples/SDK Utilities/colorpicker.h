////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// A color picker "object"


#ifndef __colorpicker_H
#define __colorpicker_H


#ifdef __cplusplus
extern "C" {
#endif


#include  "TVPaintSDK.h"


typedef struct ColorPicker
{
    PIPixel  mColor;

    // methods
    void  (*Load)( struct ColorPicker* ioPicker, PIFilter* iFilter, const char* iName );
    void  (*Save)( const struct ColorPicker* iPicker,  PIFilter* iFilter, const char* iName );
    int   (*Grab)( struct ColorPicker* oPicker,  PIFilter* iFilter);

    PIBlock*  (*MakeBlock)( const struct ColorPicker* iPicker, PIFilter* iFilter, int iW, int iH );
} ColorPicker;


extern  ColorPicker*  ColorPickerNew( PIFilter* iFilter );
extern  void          ColorPickerDelete( PIFilter* iFilter, ColorPicker* iPicker );


#ifdef __cplusplus
}
#endif


#endif // __colorpicker_H
