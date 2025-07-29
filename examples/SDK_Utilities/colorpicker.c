

#include "colorpicker.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static void
ColorPickerLoad( ColorPicker* ioPicker, PIFilter* iFilter, const char* iName )
{
    char  val[1024];
    int  r, g, b;

    strcpy( val, "255 255 255" );
    TVReadUserString( iFilter, iFilter->PIName, (char*)iName, val, val, 1024 );

    sscanf( val, "%d %d %d", &r, &g, &b );

    ioPicker->mColor.l = TVMakePIPixel32( r, g, b, 255 );
}


static void
ColorPickerSave( const ColorPicker* iPicker, PIFilter* iFilter, const char* iName )
{
    char  tmp[1024];

    snprintf( tmp, sizeof(tmp), "%d %d %d",
              (int)(iPicker->mColor.Red),
              (int)(iPicker->mColor.Green),
              (int)(iPicker->mColor.Blue)
        );
    TVWriteUserString( iFilter, iFilter->PIName, (char*)iName, tmp );
}



static int
ColorPickerGrab( ColorPicker* oPicker, PIFilter* iFilter )
{
    char  result[256];
    int   but, r, g, b, a;

    TVSendCmd( iFilter, "tv_piccolor", result );
    sscanf( result, "%d %d %d %d %d", &but, &r, &g, &b, &a );

    if( but == 0 )
    {
        oPicker->mColor.l = TVMakePIPixel32( r, g, b, 255 );

        return  1;
    }

    return  0;
}


static PIBlock*
ColorPickerMakeBlock( const ColorPicker* iPicker, PIFilter* iFilter, int iW, int iH )
{
    int  x;
    PIPixel*  p = NULL;

    PIBlock*  block = TVAllocPIBlock( iFilter, iW, iH, 0 );
    if( !block )
        return  NULL;

    p = block->Data;

    for( x = 0; x < iW*iH; x++ )
    {
        p[x].l = iPicker->mColor.l;
    }

    return  block;
}




ColorPicker*
ColorPickerNew( PIFilter* iFilter )
{
    ColorPicker*  color = (ColorPicker*)malloc( sizeof(ColorPicker) );

    if( !color )
        return  NULL;

    color->mColor.l = 0xFFFFFFFF; // full opaque white !

    color->Load      = ColorPickerLoad;
    color->Save      = ColorPickerSave;
    color->Grab      = ColorPickerGrab;
    color->MakeBlock = ColorPickerMakeBlock;

    return  color;
}


void
ColorPickerDelete( PIFilter* iFilter, ColorPicker* iPicker )
{
    if( iPicker )
    {
        free( iPicker );
    }
}
