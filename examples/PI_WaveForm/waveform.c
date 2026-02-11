/** @copyright (c) 1995-25 TVPaint Developpement. All Rights Reserved.
*
*  @file  waveform.c
*
*  @brief  A vectorscope emulator.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "TVPaintAnimationSDK/TVPaintSDK.h"
#include "block.h"
#include "aadraw.h"

#ifdef WIN32
#include <malloc.h>
#endif

/****************************************************************/

static  void*  sgLocalFile = NULL;

static  PIPixel  sgPalette[256];

static  double  sgGamma[256];
static  double  sgGamma299[256];
static  double  sgGamma587[256];
static  double  sgGamma114[256];

static  INTPTR  sgReq = 0;

/****************************************************************/

#define TXT_NAME              GetLocalString( iPlugin, 100,   "SDK Waveform" )
#define TXT_WINDOW            GetLocalString( iPlugin, 10000, "SDK Waveform" )

#define TXT_MODE_COMPOSITE    GetLocalString( iPlugin, 10010, "Composite WF" )
#define TXT_MODE_VECTOR       GetLocalString( iPlugin, 10011, "VectorScope" )
#define TXT_MODE_COMPONENT    GetLocalString( iPlugin, 10012, "Component WF" )

#define TXT_LUMINOSITY        GetLocalString( iPlugin, 10015, "Luminosity" )

#define TXT_DISPLAY           GetLocalString( iPlugin, 10020, "Display" )
#define TXT_DISPLAY_YC        GetLocalString( iPlugin, 10025, "YC" )
#define TXT_DISPLAY_Y         GetLocalString( iPlugin, 10026, "Y" )
#define TXT_DISPLAY_C         GetLocalString( iPlugin, 10027, "C" )

#define TXT_SCALE             GetLocalString( iPlugin, 10030, "Scale" )
#define TXT_SCALE_100         GetLocalString( iPlugin, 10035, "100%" )
#define TXT_SCALE_75          GetLocalString( iPlugin, 10036, "75%" )

#define TXT_COMPONENT         GetLocalString( iPlugin, 10040, "Component" )
#define TXT_COMPONENT_RGB     GetLocalString( iPlugin, 10045, "RGB" )
#define TXT_COMPONENT_YUV     GetLocalString( iPlugin, 10046, "YUV" )

#define TXT_SCREEN_Y          GetLocalString( iPlugin, 10050, "Y" )
#define TXT_SCREEN_U          GetLocalString( iPlugin, 10051, "U" )
#define TXT_SCREEN_V          GetLocalString( iPlugin, 10052, "V" )
#define TXT_SCREEN_R          GetLocalString( iPlugin, 10053, "R" )
#define TXT_SCREEN_G          GetLocalString( iPlugin, 10054, "G" )
#define TXT_SCREEN_B          GetLocalString( iPlugin, 10055, "B" )
#define TXT_SCREEN_YL         GetLocalString( iPlugin, 10056, "Yl" )
#define TXT_SCREEN_CY         GetLocalString( iPlugin, 10057, "Cy" )
#define TXT_SCREEN_MG         GetLocalString( iPlugin, 10058, "Mg" )
#define TXT_SCREEN_I          GetLocalString( iPlugin, 10059, "I" )
#define TXT_SCREEN_Q          GetLocalString( iPlugin, 10060, "Q" )
#define TXT_SCREEN_MI         GetLocalString( iPlugin, 10061, "-I" )
#define TXT_SCREEN_MQ         GetLocalString( iPlugin, 10062, "-Q" )

#define TXT_APPLY             GetLocalString( iPlugin, 10065, "Apply" )
#define TXT_RETICLE           GetLocalString( iPlugin, 10066, "Reticle" )
#define TXT_TRANSPARENT       GetLocalString( iPlugin, 10067, "Transparent" )

#define TXT_ERROR01           GetLocalString( iPlugin, 10070, "Can't open requester !" )

#define TXT_HELP_MODE         GetLocalString( iPlugin, 10110, "Choose your type of Waveform monitor" )
#define TXT_HELP_LUMINOSITY   GetLocalString( iPlugin, 10115, "Choose the luminosity" )
#define TXT_HELP_DISPLAY      GetLocalString( iPlugin, 10120, "Choose the channel to display" )
#define TXT_HELP_SCALE        GetLocalString( iPlugin, 10130, "Choose the scale to display" )
#define TXT_HELP_COMPONENT    GetLocalString( iPlugin, 10140, "Choose the channels to display" )
#define TXT_HELP_VECTOR       GetLocalString( iPlugin, 10150, "This is your Waveform monitor !" )
#define TXT_HELP_RETICLE      GetLocalString( iPlugin, 10160, "Display Waveform reticle" )
#define TXT_HELP_TRANSPARENT  GetLocalString( iPlugin, 10161, "Apply Waveform with a transparent background" )
#define TXT_HELP_APPLY        GetLocalString( iPlugin, 10162, "Apply Filter" )

/****************************************************************/

typedef  enum
{
    kMode_Composite   = 0,
    kMode_VectorScope = 1,
    kMode_Components  = 2,
} eMode;

typedef  enum
{
    kDisplay_YC = 0,
    kDisplay_Y  = 1,
    kDisplay_C  = 2,
} eComposite;

typedef  enum
{
    kScale_100 = 0,
    kScale_75  = 1,
} eScale;

typedef  enum
{
    kComponents_RGB = 0,
    kComponents_YUV = 1,
} eComponents;


typedef  struct
{
    eMode  mode;
    struct
    {
        int  composite;
        int  vector;
        int  component;
    } luminosity;
    eComposite  composite;
    eScale  scale;
    eComponents  components;

    int  reticle;
    int  transparent;
} UserParam;

/****************************************************************/

enum
{
    kID_Mode              = 10,

    kID_LuminosityLabel   = 20,
    kID_Luminosity        = 21,
    kID_LuminositySlider  = 22,
    kID_OptionsLabel      = 30,
    kID_OptionsPopup      = 31,

    kID_ReticleLabel      = 40,
    kID_Reticle           = 41,
    kID_TransparentLabel  = 50,
    kID_Transparent       = 51,

    kID_Display           = 60,

    kID_Apply             = 70,
};

/****************************************************************/

enum // GUI elements positions
{
    kWindowW = 364,
    kWindowH = 358,

    kY1 = 5,
    kY2 = 33,
    kY3 = 53,

    kLabel1X = 3,
    kLabel1W = 85,
    kValue1X = 89,
    kValue1W = 50,

    kLabel2X = 190,
    kLabel2W = 85,
    kValue2X = 282,
    kValue2W = 62,

    kDisplayX = 9,
    kDisplayY = 81,
    kDisplayW = 342,
    kDisplayH = 256,

    kApplyX = 9,
    kApplyY = 348,
    kApplyW = 342,
    kApplyH = 0,
};

/****************************************************************/

enum // Misc values...
{
    kNumberReadLines = 1,
    kReticleY        = 128,
};

static  const double  kScaleY = 0.56;
static  const double  kPI = 3.141592654;

/****************************************************************/

static  const char*  GetLocalString( PIPlugin* iPlugin, int  iNumber, const char*  iDefault )
{
    if( sgLocalFile == NULL )
        return  iDefault;
    const char*  str = TVGetLocalString( iPlugin, sgLocalFile, iNumber );
    if( str == NULL  ||  strlen( str ) == 0 )
        return  iDefault;

    return  str;
}

/****************************************************************/

static  void  CalcRotation( double  iAngle, double*  ioX, double*  ioY, double  iCX, double  iCY )
{
    double  x = iCX + (*ioX-iCX) * cos( iAngle ) + (*ioY-iCY) * sin( iAngle );
    double  y = iCY + (*ioY-iCY) * cos( iAngle ) - (*ioX-iCX) * sin( iAngle );

    *ioX = x;
    *ioY = y;
}

/****************************************************************/

static  void  ComputeGammaTable( double  iGamma, int  iBlack, int  iWhite, int  iScale )
{
    // it's somewhat oversimplified, isn't it ?

    double  scale = iScale ? kScaleY : 1.0;
    for( int  i = 0; i < 256; ++i )
    {
        sgGamma[i] = i * scale;
    }

    for( int  i = 0; i < 256; ++i )
    {
        sgGamma114[i] = sgGamma[i]*0.114;
        sgGamma299[i] = sgGamma[i]*0.299;
        sgGamma587[i] = sgGamma[i]*0.587;
    }
}

/****************************************************************/

static  void  FindColorPosition( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iR, int  iG, int  iB, int*  oR, int*  oB )
{
    int  mx = ioBlock->Width/2;
    int  my = ioBlock->Height/2;

    double  ratio = ioBlock->Height/256.;
    double  y = 0.299 * iR + 0.587 * iG + 0.114 * iB;

    double  b =  ((iB-y) * 0.342 + 0.49)*ratio + mx;
    double  r = -((iR-y) * 0.609 + 0.49)*ratio + my;

    *oR = (int)r;
    *oB = (int)b;
}

static  void  DrawColorReticle( PIPlugin*  iPlugin, PIBlock*  ioBlock, int  iX, int  iY )
{
    PIPixel  white = TVMakePIPixel( kReticleY, kReticleY, kReticleY, kReticleY );

    int  pr = ioBlock->Height / 64;

    double  x1 = iX - pr;
    double  y1 = iY - pr;
    double  x2 = iX + pr;
    double  y2 = iY - pr;
    double  x3 = iX + pr;
    double  y3 = iY + pr;
    double  x4 = iX - pr;
    double  y4 = iY + pr;

    int  mx = ioBlock->Width / 2;
    int  my = ioBlock->Height / 2;

    int  anglex = (int)(mx - iX);
    if( anglex == 0 )
        anglex = 1;

    int  angley = (int)(my - iY);
    if( angley == 0 )
        angley = 1;

//TODO: use atan2 ! (and remove the =1 patches above !)
    double  angle = atan( anglex / (double)angley );

    CalcRotation( angle, &x1, &y1, iX, iY );
    CalcRotation( angle, &x2, &y2, iX, iY );
    CalcRotation( angle, &x3, &y3, iX, iY );
    CalcRotation( angle, &x4, &y4, iX, iY );

    AADrawLinePIBlock( ioBlock, (int)x1, (int)y1, (int)x2, (int)y2, white.l );
    AADrawLinePIBlock( ioBlock, (int)x2, (int)y2, (int)x3, (int)y3, white.l );
    AADrawLinePIBlock( ioBlock, (int)x3, (int)y3, (int)x4, (int)y4, white.l );
    AADrawLinePIBlock( ioBlock, (int)x4, (int)y4, (int)x1, (int)y1, white.l );
}

/****************************************************************/

static  void  DrawReticleComposite( PIPlugin*  iPlugin, PIBlock*  ioBlock, eMode  iMode )
{
    PIPixel  white = TVMakePIPixel( kReticleY, kReticleY, kReticleY, kReticleY );

    // Top Line
    AADrawLinePIBlock(ioBlock, 0, 0, ioBlock->Width - 1, 0, white.l );

    // Bottom Line
    AADrawLinePIBlock(ioBlock, 0, ioBlock->Height - 1, ioBlock->Width - 1, ioBlock->Height - 1, white.l );

    int  value = 120;

    // Repere 20 - 20
    int  even = 0;
    for( int  a = 1; a < 19; ++a )
    {
        int  y1 = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height * a) / 18) ) );

        if( even )
        {
            if( value == 0 )
            {
                //Reference ("0 IRE")
                AADrawLinePIBlock( ioBlock, 51, y1, ioBlock->Width - 52, y1, white.l );
            }
            if( value == 100 )
            {
                //Reference ("100 IRE")
                for( int  b = 51; b < ioBlock->Width - 56; b += 10 )
                {
                    AADrawLinePIBlock( ioBlock, b, y1, b + 5, y1, white.l );
                }
            }

            if( a != 18 )
            {
                AADrawLinePIBlock( ioBlock, 28, y1, 50, y1, white.l );
                AADrawLinePIBlock( ioBlock, ioBlock->Width - 51, y1, ioBlock->Width - 1, y1, white.l );
                if( value != -40 )
                {
                    char  text[512];
                    snprintf(text, sizeof(text), "%d", value );
                    TVTextBlock( iPlugin, ioBlock, 3, y1 + 4, text, white.l );
                }
            }
            value = value - 20;
        }
        else
        {
            AADrawLinePIBlock( ioBlock, 0, y1, 35, y1, white.l );
            AADrawLinePIBlock( ioBlock, ioBlock->Width - 36, y1, ioBlock->Width - 1, y1, white.l );
        }
        even = !even;
    }
}

static  void  DrawReticleVectorScope( PIPlugin*  iPlugin, PIBlock*  ioBlock, eMode  iMode )
{
    PIPixel  white = TVMakePIPixel( kReticleY, kReticleY, kReticleY, kReticleY );

    int  r1 = ioBlock->Height / 2 - 1;
    int  r2 = ioBlock->Height / 2 - 2;
    int  r3 = ioBlock->Height / 2 - 5;

    int  mx = ioBlock->Width / 2;
    int  my = ioBlock->Height / 2;

    // axe U
    AADrawLinePIBlock( ioBlock, mx - 20, my, mx - r1, my, white.l );
    AADrawLinePIBlock( ioBlock, mx + 20, my, mx + r1, my, white.l );

    // axe V
    AADrawLinePIBlock( ioBlock, mx, my - r1, mx, my - 20, white.l );
    AADrawLinePIBlock( ioBlock, mx, my + 20, mx, my + r1, white.l );

    // Center
    AADrawLinePIBlock( ioBlock, mx - 8, my, mx + 8, my, white.l );
    AADrawLinePIBlock( ioBlock, mx, my - 8, mx, my + 8, white.l );

    // reticle
    int  p = 0;
    for( double  a = 0.; a < kPI/2; a += kPI/90 )
    {
        double  x = cos( a );
        double  y = sin( a );

        int  x1 = (int)(x*r1);
        int  y1 = (int)(y*r1);
        int  x2, y2;
        if( p == 0 )
        {
            x2 = (int)(x*r3);
            y2 = (int)(y*r3);
            p = 4;
        }
        else
        {
            x2 = (int)(x*r2);
            y2 = (int)(y*r2);
            p--;
        }

        AADrawLinePIBlock( ioBlock, mx + x1, my + y1, mx + x2, my + y2, white.l );
        AADrawLinePIBlock( ioBlock, mx - x1, my - y1, mx - x2, my - y2, white.l );
        AADrawLinePIBlock( ioBlock, mx + x1, my - y1, mx + x2, my - y2, white.l );
        AADrawLinePIBlock( ioBlock, mx - x1, my + y1, mx - x2, my + y2, white.l );
    }

    // Axe +Q
    double  x = cos( 33/180.*kPI );
    double  y = sin( -33/180.*kPI );

    int  q1 = ioBlock->Width / 8;

    int  x1 = (int)(x*q1);
    int  y1 = (int)(y*q1);

    int  x2 = (int)(x*r1);
    int  y2 = (int)(y*r1);

    AADrawLinePIBlock(ioBlock, mx + x1, my + y1, mx + x2, my + y2, white.l );
    TVTextBlock(iPlugin, ioBlock, mx + x2 - 5, my + y2 + 15 + 7, TXT_SCREEN_Q, white.l );

    // Axe -Q
    x = cos( (33/180.*kPI)+kPI );

    //TODO: NO +kPI here as above ?!?!
    y = sin( 33/180.*kPI );

    q1 = ioBlock->Width / 8;

    x1 = (int)(x*q1);
    y1 = (int)(y*q1);

    x2 = (int)(x*r1);
    y2 = (int)(y*r1);

    AADrawLinePIBlock(ioBlock, mx + x1, my + y1, mx + x2, my + y2, white.l );

    // Axe -I
    x = cos( 57/180.*kPI );

    y = sin( 57/180.*kPI );

    int  i1 = ioBlock->Width / 8;

    x1 = (int)(x*i1);
    y1 = (int)(y*i1);

    x2 = (int)(x*r1);
    y2 = (int)(y*r1);

    AADrawLinePIBlock(ioBlock, mx + x1, my + y1, mx + x2, my + y2, white.l );
    TVTextBlock(iPlugin, ioBlock, mx + x2 - 2, my + y2 - 20 + 7, TXT_SCREEN_MI, white.l );

    // Axe +I
    x = cos( (57/180.*kPI)-kPI );

    //TODO: NO -kPI here as above ?!?!
    y = sin( -57/180.*kPI );

    i1 = ioBlock->Width / 8;

    x1 = (int)(x*i1);
    y1 = (int)(y*i1);

    x2 = (int)(x*r1);
    y2 = (int)(y*r1);

    AADrawLinePIBlock(ioBlock, mx + x1, my + y1, mx + x2, my + y2, white.l );

    // Red
    FindColorPosition(iPlugin, ioBlock, 255, 0, 0, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 - 20, y1 + 4 + 7, TXT_SCREEN_R, white.l );

    // Magenta
    FindColorPosition(iPlugin, ioBlock, 255, 0, 255, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 + 14, y1 + 4 + 7, TXT_SCREEN_MG, white.l );

    // Yellow
    FindColorPosition(iPlugin, ioBlock, 255, 255, 0, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 - 24, y1 - 6 + 7, TXT_SCREEN_YL, white.l );

    // Blue
    FindColorPosition(iPlugin, ioBlock, 0, 0, 255, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 + 12, y1 + 4 + 7, TXT_SCREEN_B, white.l );

    // Cyan
    FindColorPosition(iPlugin, ioBlock, 0, 255, 255, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 + 10, y1 - 10 + 7, TXT_SCREEN_CY, white.l );

    // Green
    FindColorPosition(iPlugin, ioBlock, 0, 255, 0, &y1, &x1 );
    DrawColorReticle(iPlugin, ioBlock, x1, y1 );
    TVTextBlock(iPlugin, ioBlock, x1 - 21, y1 - 10 + 7, TXT_SCREEN_G, white.l );
}

static  void  DrawReticleComponentsRGB( PIPlugin*  iPlugin, PIBlock*  ioBlock, eMode  iMode )
{
    PIPixel  white = TVMakePIPixel( kReticleY, kReticleY, kReticleY, kReticleY );

    int  value = 120;
    int  stepw = ioBlock->Width / 6;

    // text RGB
    TVTextBlock( iPlugin, ioBlock, stepw * 1, ioBlock->Height - 16, TXT_SCREEN_R, white.l );
    TVTextBlock( iPlugin, ioBlock, stepw * 3, ioBlock->Height - 16, TXT_SCREEN_G, white.l );
    TVTextBlock( iPlugin, ioBlock, stepw * 5, ioBlock->Height - 16, TXT_SCREEN_B, white.l );

    // Separations RGB
    for( int  a = 0; a < ioBlock->Height; a += 10 )
    {
        AADrawLinePIBlock( ioBlock, stepw * 2, a, stepw * 2, a + 5, white.l );
        AADrawLinePIBlock( ioBlock, stepw * 4, a, stepw * 4, a + 5, white.l );
    }

    // Repere 20 - 20
    int  even = 0;
    for( int  a = 1; a < 19; ++a )
    {
        int  y1 = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height * a) / 18) ) );

        if( even )
        {
            if( value == 0 )
            {
                // Reference ("0 IRE")
                AADrawLinePIBlock(ioBlock, 51, y1, ioBlock->Width - 52, y1, white.l );
            }
            if( value == 100 )
            {
                // Reference ("100 IRE")
                for( int  b = 51; b < ioBlock->Width - 56; b += 10 )
                {
                    AADrawLinePIBlock(ioBlock, b, y1, b + 5, y1, white.l );
                }
            }
            if( value <= 100  &&  value >= 0  &&  a != 18 )
            {
                char  text[512];
                snprintf( text, sizeof(text), "%d", value );
                TVTextBlock( iPlugin, ioBlock, 3, y1 + 4, text, white.l );

                AADrawLinePIBlock( ioBlock, 28, y1, 50, y1, white.l );
                AADrawLinePIBlock( ioBlock, ioBlock->Width - 51, y1, ioBlock->Width - 1, y1, white.l );
            }
            value = value - 20;
        }
        else
        {
            if( value <= 80  &&  value >= 0 )
            {
                AADrawLinePIBlock( ioBlock, 0, y1, 35, y1, white.l );
                AADrawLinePIBlock( ioBlock, ioBlock->Width - 36, y1, ioBlock->Width - 1, y1, white.l );
            }
        }
        even = !even;
    }
}

static  void  DrawReticleComponentsYUV( PIPlugin*  iPlugin, PIBlock*  ioBlock, eMode  iMode )
{
    PIPixel  white = TVMakePIPixel( kReticleY, kReticleY, kReticleY, kReticleY );
    int  stepw = ioBlock->Width / 6;

    // text YUV
    TVTextBlock( iPlugin, ioBlock, stepw * 1, ioBlock->Height - 16, TXT_SCREEN_Y, white.l );
    TVTextBlock( iPlugin, ioBlock, stepw * 3, ioBlock->Height - 16, TXT_SCREEN_U, white.l );
    TVTextBlock( iPlugin, ioBlock, stepw * 5, ioBlock->Height - 16, TXT_SCREEN_V, white.l );

    // Separations YUV
    AADrawLinePIBlock( ioBlock, stepw * 2, 0, stepw * 2, ioBlock->Height - 1, white.l );
    for( int  a = 0; a < ioBlock->Height; a += 10 )
    {
        AADrawLinePIBlock( ioBlock, stepw * 4, a, stepw * 4, a + 5, white.l );
    }

    int  value = 120;

    // grad Y
    int  even = 0;
    for( int  a = 1; a < 19; ++a )
    {
        int  y1 = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height * a) / 18) ) );

        if( even )
        {
            if( value == 0 )
            {
                // Reference ("0 IRE Y")
                AADrawLinePIBlock( ioBlock, 51, y1, stepw * 2, y1, white.l );
            }
            if( value == 100 )
            {
                // Reference ("100 IRE")
                for( int  b = 51; b < stepw*2; b += 10 )
                {
                    AADrawLinePIBlock( ioBlock, b, y1, MIN( stepw * 2, b + 5 ), y1, white.l );
                }
            }

            if( value <= 100  &&  value >= 0  &&  a != 18 )
            {
                char  text[512];
                snprintf( text, sizeof(text), "%d", value );
                TVTextBlock( iPlugin, ioBlock, 3, y1 + 4, text, white.l );
                AADrawLinePIBlock( ioBlock, 28, y1, 50, y1, white.l );
            }
            value = value - 20;
        }
        else
        {
            if( value <= 80  &&  value >= 0 )
            {
                AADrawLinePIBlock( ioBlock, 0, y1, 35, y1, white.l );
            }
        }
        even = !even;
    }

    // grad UV
    even = 0;
    value = 70;
    for( int  a = 1; a < 19; ++a )
    {
        int  y1 = MAX( 0, MIN(ioBlock->Height - 1, (int)((ioBlock->Height * a) / 18) ) );

        if( even )
        {
            if( a != 18 )
            {
                AADrawLinePIBlock( ioBlock, ioBlock->Width - 51, y1, ioBlock->Width - 28, y1, white.l );
                if( value != -80 )
                {
                    char  text[512];
                    snprintf( text, sizeof(text), "%d", value );
                    TVTextBlock( iPlugin, ioBlock, ioBlock->Width - 20, y1 + 4, text, white.l );
                }
            }
            value = value - 20;
        }
        else if( a != 17  &&  a != 1 )
        {
            if( a == 9 )
            {
                // Reference ("0 IRE UV")
                AADrawLinePIBlock( ioBlock, stepw * 2 + 1, y1, ioBlock->Width - 37, y1, white.l );
            }
            AADrawLinePIBlock( ioBlock, ioBlock->Width - 36, y1, ioBlock->Width - 1, y1, white.l );
        }
        even = !even;
    }
}

static  void  DrawReticle( PIPlugin*  iPlugin, PIBlock*  ioBlock, eMode  iMode )
{
    UserParam* param = (UserParam*)iPlugin->UserParametersPtr;

    if( iMode == kMode_Composite )
        DrawReticleComposite( iPlugin, ioBlock, iMode );
    else if( iMode == kMode_VectorScope )
        DrawReticleVectorScope( iPlugin, ioBlock, iMode );
    else if(iMode == kMode_Components && param->components == kComponents_RGB )
        DrawReticleComponentsRGB( iPlugin, ioBlock, iMode );
    else if(iMode == kMode_Components && param->components == kComponents_YUV )
        DrawReticleComponentsYUV( iPlugin, ioBlock, iMode );
}

/****************************************************************/

static  void  AddLineToComposite( PIPlugin*  iPlugin, PIBlock*  ioBlock, const PIBlock*  iLine, int  iLineCount )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    int  pr = ioBlock->r;
    int  pg = ioBlock->g;
    int  pb = ioBlock->b;

    int  vw = ioBlock->Width;
    int  vh = ioBlock->Height;

    double  rx = iLine->Width / ((double)vw);

    int  offsety = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height * 14) / 18) ) );

    double  ratio = ioBlock->Height / 256.;

    for( int  yy = 0; yy < iLineCount; ++yy )
    {
        PIPixel*  ld = &iLine->Data[yy * iLine->Width];
        double  px = 0;
        if( param->composite == kDisplay_YC )
        {
            for( int  x = 0; x < vw; ++x )
            {
                const PIPixel*  s = &ld[(int)(px)];

                double  dy = sgGamma299[s->c[pr]] + sgGamma587[s->c[pg]] + sgGamma114[s->c[pb]];

                int  bmy = (int)((sgGamma[s->c[pb]] - dy) * 0.492);
                int  rmy = (int)((sgGamma[s->c[pr]] - dy) * 0.876);

                int  v = (int)( (sqrtf( (float)((bmy*bmy)+(rmy*rmy)) )*kScaleY + 0.49)*ratio);
                int  y = offsety - (int)((dy*kScaleY + 0.49)*ratio);

                // iLine sup
                PIPixel*  p = &ioBlock->Data[x + vw * MAX(0, y - v )];
                ((int*)p)[0] += 2;
                ((int*)p)[vw] -= 1;

                // iLine Y
                p = &ioBlock->Data[x + vw * MIN(vh - 2, MAX(0, y ))];
                ((int*)p)[0] += 1;
                ((int*)p)[vw] -= 1;

                // iLine inf
                p = &ioBlock->Data[x + vw * MIN(vh - 2, y + v )];
                ((int*)p)[0] += 1;
                ((int*)p)[vw] -= 2;

                px += rx;
            }
        }
        else if( param->composite == kDisplay_Y )
        {
            for( int  x = 0; x < vw; ++x )
            {
                const PIPixel* s = &ld[(int)(px)];

                int y =	(int)( (sgGamma299[s->c[pr]] + sgGamma587[s->c[pg]] + sgGamma114[s->c[pb]] + 0.4)*ratio );

                y = offsety-y;

                // iLine Y
                PIPixel*  p = &ioBlock->Data[x + vw * MIN(vh - 2, MAX(0, y ) )];
                ((int*)p)[0] += 10;
                ((int*)p)[vw] -= 10;

                px += rx;
            }
        }
        else if( param->composite == kDisplay_C )
        {
            for( int  x = 0; x < vw; ++x )
            {
                const PIPixel*  s = &ld[(int)(px)];

                double dy = sgGamma299[s->c[pr]] + sgGamma587[s->c[pg]] + sgGamma114[s->c[pb]];

                int  bmy = (int)((sgGamma[s->c[pb]] - dy) * 0.492 + 0.49);
                int  rmy = (int)((sgGamma[s->c[pr]] - dy) * 0.876 + 0.49);

                int  v = (int)((sqrt( (bmy*bmy)+(rmy*rmy) )*kScaleY)*ratio);

                // iLine sup
                PIPixel*  p = &ioBlock->Data[x + vw * MAX(0, offsety - v )];
                ((int*)p)[0] += 2;
                ((int*)p)[vw] -= 1;

                // iLine 0
                p = &ioBlock->Data[x + vw * MIN(vh - 2, offsety )];
                ((int*)p)[0] += 1;
                ((int*)p)[vw] -= 2;

                px += rx;
            }
        }
    }
}

static  void  FillComposite( PIPlugin*  iPlugin, PIBlock*  ioBlock, double  iRatio, const PIPixel*  iPalette )
{
    int  vh = ioBlock->Height;
    int  vw = ioBlock->Width;
    int  w = vw;

    vw = vw & (~1);

    int  ir = (int)(iRatio * 65536.);

    for( int  x = 0; x < vw; x += 2 )
    {
        PIPixel*  p = &ioBlock->Data[x];
        unsigned long  v = 0;
        unsigned long  vv = 0;
        for( int  y = 0; y < vh; ++y )
        {
            v += ((int*)p)[0];
            if( v )
            {
                int  t = MIN( 255, (int)(v*ir)>>16 );
                p[0] = iPalette[t];
            }
            else
            {
                p[0] = iPalette[0];
            }

            vv += ((int*)p)[1];
            if( vv )
            {
                int  t = MIN( 255, (int)(vv*ir)>>16 );
                p[1] = iPalette[t];
            }
            else
            {
                p[1].l = iPalette[0].l;
            }

            p += w;
        }
    }
}

static  void  AddLineToVectorScope( PIPlugin*  iPlugin, PIBlock*  ioBlock, const PIBlock*  iLine, int  iLineCount )
{
    UserParam*  param = (UserParam *)iPlugin->UserParametersPtr;

    int  w = ioBlock->Width;
    int  h = ioBlock->Height;
    double  ratio = ioBlock->Height / 256.;

    int  pr = ioBlock->r;
    int  pg = ioBlock->g;
    int  pb = ioBlock->b;
    int  pa = ioBlock->a;

    int  mx = ioBlock->Width / 2;
    int  my = ioBlock->Height / 2;

    PIPixel*  p = &ioBlock->Data[mx + my * w];
    PIPixel  last;
    last.l = 0;

    for( int  yy = 0; yy < iLineCount; ++yy )
    {
        const PIPixel*  s = &iLine->Data[yy * iLine->Width];

        if( param->scale == kScale_100 )
        {
            for( int  x = iLine->Width / 2; x; x--, s += 2 )
            {
                if( last.l == s->l )
                {
                    p->l += 1;
                }
                else
                {
                    double  r = sgGamma[s->c[pr]];
                    double  g = sgGamma[s->c[pg]];
                    double  b = sgGamma[s->c[pb]];

                    double  y = 0.299 * r + 0.587 * g + 0.114 * b;

                    int  bmy = (int)( ((b-y) * 0.342 + 0.49 )*ratio + mx);
                    int  rmy = (int)(-((r-y) * 0.609 + 0.49 )*ratio + my);

                    p = &ioBlock->Data[bmy + rmy * w];
                    p->l += 1;
                    last.l = s->l;
                }
            }
        }
        else if( param->scale == kScale_75 )
        {
            for( int  x = iLine->Width / 2; x; x--, s += 2 )
            {
                if( last.l == s->l )
                {
                    p->l += 1;
                }
                else
                {
                    double  r = sgGamma[s->c[pr]];
                    double  g = sgGamma[s->c[pg]];
                    double  b = sgGamma[s->c[pb]];

                    double  y = 0.299 * r + 0.587 * g + 0.114 * b;

                    int  bmy = (int)( (((b-y) * (0.342*1.3333) + 0.49 ))*ratio + mx);
                    int  rmy = (int)(-(((r-y) * (0.609*1.3333) + 0.49 ))*ratio + my);

                    if( bmy >= 0  &&  bmy < w  &&  rmy >= 0  &&  rmy < h )
                    {
                        p = &ioBlock->Data[bmy + rmy * w];
                        p->l += 1;
                        last.l = s->l;
                    }
                }
            }
        }
    }
}

static  void  FillVectorScope( PIPlugin*  iPlugin, PIBlock*  ioBlock, double  iRatio, const PIPixel*  iPalette )
{
    PIPixel*  p = ioBlock->Data;
    for( int  x = ioBlock->Width * ioBlock->Height; x; --x )
    {
        PIPixel32  v = p->l;
        if( v )
        {
            v = MIN( 255, (int)(v * iRatio) );
            p->l = iPalette[v].l;
        }
        else
        {
            p->l = iPalette[0].l;
        }

        ++p;
    }
}

static  void  AddLineToComponent( PIPlugin*  iPlugin, PIBlock*  ioBlock, const PIBlock*  iLine, int  iLineCount )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    int  pr = ioBlock->r;
    int  pg = ioBlock->g;
    int  pb = ioBlock->b;

    int  w  = ioBlock->Width;
    int  vw = ioBlock->Width / 3 - 2;

    double  rx = iLine->Width / ((double)vw);

    double  ratio = ioBlock->Height / 256.;

    int  offsety  = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height * 14) / 18) ) );
    int  offsetmy = MAX( 0, MIN( ioBlock->Height - 1, (int)((ioBlock->Height *  9) / 18) ) );

    for( int  yy = 0; yy < iLineCount; ++yy )
    {
        PIPixel*  ld = &iLine->Data[yy * iLine->Width];
        double  px = 0;

        if( param->components == kComponents_RGB )
        {
            for( int  x = 3; x < vw+3; ++x )
            {
                PIPixel*  s = &ld[(int)(px)];

                int  r = offsety - (int)(sgGamma[s->c[pr]] * ratio);
                int  g = offsety - (int)(sgGamma[s->c[pg]] * ratio);
                int  b = offsety - (int)(sgGamma[s->c[pb]] * ratio);


                // iLine R
                PIPixel*  p = &ioBlock->Data[x + w * r];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                // iLine G
                p = &ioBlock->Data[x + vw + w * g];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                // iLine B
                p = &ioBlock->Data[x + vw + vw + w * b];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                px += rx;
            }
        }
        else if( param->components == kComponents_YUV )
        {
            double  mb = 0.492*kScaleY*ratio;
            double  mr = 0.877*kScaleY*ratio;

            for( int  x = 3; x < vw+3; ++x )
            {
                PIPixel*  s = &ld[(int)(px)];

                double  dy = sgGamma299[s->c[pr]] + sgGamma587[s->c[pg]] + sgGamma114[s->c[pb]];

                int  bmy = offsetmy - (int)((sgGamma[s->c[pb]] - dy) * mb + 0.49);
                int  rmy = offsetmy - (int)((sgGamma[s->c[pr]] - dy) * mr + 0.49);

                int  y = offsety - (int)(dy*kScaleY*ratio + 0.49);

                // iLine Y
                PIPixel*  p = &ioBlock->Data[x + w * y];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                // iLine BMY
                p = &ioBlock->Data[x + vw + w * bmy];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                // iLine RMY
                p = &ioBlock->Data[x + vw + vw + w * rmy];
                ((int*)p)[0] += 10;
                ((int*)p)[w] -= 10;

                px += rx;
            }
        }
    }
}

static  void  FillComponent( PIPlugin*  iPlugin, PIBlock*  ioBlock, double  iRatio, const PIPixel*  iPalette )
{
    // this is the same as FillComposite()...
    FillComposite( iPlugin, ioBlock, iRatio, iPalette );
}

/****************************************************************/

static  void  Draw( PIPlugin*  iPlugin, PIBlock*  ioBlock )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;


    PIBlock*  line = TVAllocPIBlock( iPlugin, iPlugin->ImageWidth, kNumberReadLines, 0 );
    if( line == NULL )
      return;

    ioBlock->clear( ioBlock, 0 );

    if( param->mode == kMode_Composite )
    {
        ComputeGammaTable( 1.0, 0, 255, param->composite == kDisplay_Y );

        for( int  y = 0; y < iPlugin->ImageHeight; y += kNumberReadLines )
        {
            int  nl = MIN( kNumberReadLines, iPlugin->ImageHeight-y );
            TVReadLayerData( iPlugin, line->Data, 0, y, line->Width, nl, 0, kPIReadLayer_Display );
            AddLineToComposite( iPlugin, ioBlock, line, nl );
        }
        FillComposite( iPlugin, ioBlock, (255 * param->luminosity.composite / 100.) / iPlugin->ImageHeight, sgPalette );
    }
    else if( param->mode == kMode_VectorScope )
    {
        ComputeGammaTable( 1.0, 0, 255, 0/*FALSE*/ );

        for( int  y = 0; y < iPlugin->ImageHeight; y += kNumberReadLines )
        {
            int  nl = MIN( kNumberReadLines, iPlugin->ImageHeight-y );
            TVReadLayerData( iPlugin, line->Data, 0, y, line->Width, nl, 0, kPIReadLayer_Display );
            AddLineToVectorScope( iPlugin, ioBlock, line, nl );
        }
        FillVectorScope( iPlugin, ioBlock, (255 * param->luminosity.vector / 100.) / iPlugin->ImageHeight, sgPalette );
    }
    else if( param->mode == kMode_Components )
    {
        ComputeGammaTable( 1.0, 0, 255, param->components == kComponents_RGB );

        for( int  y = 0; y < iPlugin->ImageHeight; y += kNumberReadLines )
        {
            int  nl = MIN( kNumberReadLines, iPlugin->ImageHeight-y );
            TVReadLayerData( iPlugin, line->Data, 0, y, line->Width, nl, 0, kPIReadLayer_Display );
            AddLineToComponent( iPlugin, ioBlock, line, nl );
        }
        FillComponent( iPlugin, ioBlock, (255 * param->luminosity.component / 100.) / iPlugin->ImageHeight, sgPalette );
    }

    if( param->reticle )
        DrawReticle( iPlugin, ioBlock, param->mode );

    FillBlockAlpha( iPlugin, ioBlock );

    TVFreePIBlock( iPlugin, line );
}

static  void  DrawVectorScope( PIPlugin*  iPlugin )
{
    PIBlock*  scope = TVAllocPIBlock( iPlugin, kDisplayW, kDisplayH, 0 );
    if( scope == NULL )
        return;

    Draw( iPlugin, scope );

    TVPutButtonImage( iPlugin, sgReq, kID_Display, scope, 0 );
}

// Render pour TVExecute()
static  void  ExecuteVectorScope( PIPlugin*  iPlugin, PIBlock*  ioDst, const PIBlock*  iSrc )
{
    PIBlock*  scope = TVAllocPIBlock( iPlugin, iPlugin->ImageWidth, iPlugin->ImageHeight, 0 );
    if( scope == NULL )
        return;

    Draw( iPlugin, scope );

    PIBlockBlendRectClip( iPlugin, scope, 0, 0, ioDst, 0, 0, ioDst->Width, ioDst->Height, 255 );

    TVFreePIBlock( iPlugin, scope );
}

/****************************************************************/

static  void  DoPalette( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    PIPixel  p1 = TVMakePIPixel( 0, 0, 0, param->transparent ? 0 : 255 );
    PIPixel  p2 = TVMakePIPixel( 0, 255, 0, 255 );
    PIPixel  p3 = TVMakePIPixel( 255, 255, 0, 255 );

    ComputeGradient( &sgPalette[0]  , 128, p1, p2 );
    ComputeGradient( &sgPalette[128], 128, p2, p3 );
}

/****************************************************************/

static  void  DrawModePopUp( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    if( param->mode == kMode_Composite )
        TVChangeButtonReq( iPlugin, sgReq, kID_Mode, bPIButtonFlags_Popup, TXT_MODE_COMPOSITE );
    else if( param->mode == kMode_VectorScope )
        TVChangeButtonReq( iPlugin, sgReq, kID_Mode, bPIButtonFlags_Popup, TXT_MODE_VECTOR );
    else if(param->mode == kMode_Components )
        TVChangeButtonReq( iPlugin, sgReq, kID_Mode, bPIButtonFlags_Popup, TXT_MODE_COMPONENT );
}

static  void  HandleModePopUp( PIPlugin*  iPlugin )
{
    PIPopup  pop[4];
    int  pos = 0;

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    memset( pop, 0, sizeof(pop) );

    pop[pos].Name = TXT_MODE_COMPOSITE;
    pop[pos].ID = kMode_Composite;
    pop[pos].Flags = (param->mode==kMode_Composite) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_MODE_VECTOR;
    pop[pos].ID = kMode_VectorScope;
    pop[pos].Flags = (param->mode==kMode_VectorScope) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_MODE_COMPONENT;
    pop[pos].ID = kMode_Components;
    pop[pos].Flags = (param->mode==kMode_Components) ? bPIPopMode_Check : 0;
    pos++;

    int  f = TVPopup( iPlugin, pop, pos, 0 );

    if( f >= 0 )
        param->mode = (eMode)f;
}

/****************************************************************/

static  void  DrawCompositePopUp( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    if( param->composite == kDisplay_YC )
        TVChangeButtonReq(iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_DISPLAY_YC );
    else if( param->composite == kDisplay_Y )
        TVChangeButtonReq(iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_DISPLAY_Y );
    else if( param->composite == kDisplay_C )
        TVChangeButtonReq(iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_DISPLAY_C );
}

static  void  HandleCompositePopUp( PIPlugin*  iPlugin )
{
    PIPopup  pop[4];
    int  pos = 0;

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    memset( pop, 0, sizeof(pop) );

    pop[pos].Name = TXT_DISPLAY_YC;
    pop[pos].ID = kDisplay_YC;
    pop[pos].Flags = (param->composite==kDisplay_YC) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_DISPLAY_Y;
    pop[pos].ID = kDisplay_Y;
    pop[pos].Flags = (param->composite==kDisplay_Y) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_DISPLAY_C;
    pop[pos].ID = kDisplay_C;
    pop[pos].Flags = (param->composite==kDisplay_C) ? bPIPopMode_Check : 0;
    pos++;

    int  f = TVPopup( iPlugin, pop, pos, 0 );

    if( f >= 0 )
        param->composite = (eComposite)f;
}

/****************************************************************/

static  void  DrawScalePopUp( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    if( param->scale == kScale_100 )
        TVChangeButtonReq( iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_SCALE_100 );
    else if( param->scale == kScale_75 )
        TVChangeButtonReq( iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_SCALE_75 );
}

static  void  HandleScalePopUp( PIPlugin*  iPlugin )
{
    PIPopup  pop[3];
    int  pos = 0;

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    memset( pop, 0, sizeof(pop) );

    pop[pos].Name = TXT_SCALE_100;
    pop[pos].ID = kScale_100;
    pop[pos].Flags = (param->scale==kScale_100) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_SCALE_75;
    pop[pos].ID = kScale_75;
    pop[pos].Flags = (param->scale==kScale_75) ? bPIPopMode_Check : 0;
    pos++;

    int  f = TVPopup( iPlugin, pop, pos, 0 );

    if( f >= 0 )
        param->scale = (eScale)f;
}

/****************************************************************/

static  void  DrawComponentPopUp( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    if( param->components == kComponents_RGB )
        TVChangeButtonReq( iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_COMPONENT_RGB );
    else if( param->components == kComponents_YUV )
        TVChangeButtonReq( iPlugin, sgReq, kID_OptionsPopup, bPIButtonFlags_Popup, TXT_COMPONENT_YUV );
}

static  void  HandleComponentPopUp( PIPlugin*  iPlugin )
{
    PIPopup  pop[3];
    int  pos = 0;

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    memset( pop, 0, sizeof(pop) );

    pop[pos].Name = TXT_COMPONENT_RGB;
    pop[pos].ID = kComponents_RGB;
    pop[pos].Flags = (param->components==kComponents_RGB) ? bPIPopMode_Check : 0;
    pos++;

    pop[pos].Name = TXT_COMPONENT_YUV;
    pop[pos].ID = kComponents_YUV;
    pop[pos].Flags = (param->components==kComponents_YUV) ? bPIPopMode_Check : 0;
    pos++;

    int  f = TVPopup( iPlugin, pop, pos, 0 );

    if( f >= 0 )
        param->components = (eComponents)f;
}

/****************************************************************/


static  int  BuildGUI( PIPlugin*  iPlugin )
{
    char  temp[512];

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    INTPTR  req = TVOpenFilterReqEx( iPlugin, kWindowW, kWindowH, NULL, NULL, bPIRequesterFlags_Standard, bPIFilterFlags_NoTopBar );
    if( req == 0 )
    {
        TVWarning( iPlugin, TXT_ERROR01 );
        return  0;
    }
    sgReq = req;
    TVSetReqTitle( iPlugin, sgReq, TXT_WINDOW );

// First row : Display mode
    TVAddButtonReq( iPlugin, sgReq, kApplyX, kY1, kApplyW+4, 0, kID_Mode, bPIButtonFlags_Popup, "" ); // full width of first column
    TVSetButtonInfoText( iPlugin, sgReq, kID_Mode, TXT_HELP_MODE );
    DrawModePopUp( iPlugin );

// Second row : Luminosity & Options Popup
    TVAddButtonReq( iPlugin, sgReq, kLabel1X, kY2, kLabel1W, 0, kID_LuminosityLabel, bPIButtonFlags_Flat, TXT_LUMINOSITY );
    TVChangeButtonName( iPlugin, sgReq, kID_LuminosityLabel, TXT_LUMINOSITY, bPIButtonTextAlign_InRight );
    TVSetButtonInfoText( iPlugin, sgReq, kID_LuminosityLabel, TXT_HELP_LUMINOSITY );

    TVAddButtonReq( iPlugin, sgReq, kValue1X, kY2, kValue1W-22, 0, kID_Luminosity, bPIButtonFlags_Text, "" ); // 22 = minislider width... //TODO:
    TVSetButtonInfoText( iPlugin, sgReq, kID_Luminosity, TXT_HELP_LUMINOSITY );

    TVAddButtonReq( iPlugin, sgReq, kValue1X+kValue1W-22, kY2, 0, 0, kID_LuminositySlider, bPIButtonFlags_Minislider, NULL ); // 22 = minislider width... //TODO:

    TVAddButtonReq( iPlugin, sgReq, kLabel2X, kY2, kLabel2W, 0, kID_OptionsLabel, bPIButtonFlags_Flat, "" );
    TVAddButtonReq( iPlugin, sgReq, kValue2X, kY2, kValue2W, 0, kID_OptionsPopup, bPIButtonFlags_Popup, "" );

// Third row : Reticle & Transparency
    TVAddButtonReq( iPlugin, sgReq, kLabel1X, kY3, kLabel1W, 0, kID_ReticleLabel, bPIButtonFlags_Flat, TXT_RETICLE );
    TVChangeButtonName( iPlugin, sgReq, kID_ReticleLabel, TXT_RETICLE, bPIButtonTextAlign_InRight );
    TVSetButtonInfoText( iPlugin, sgReq, kID_ReticleLabel, TXT_HELP_RETICLE );
    TVAddButtonReq( iPlugin, sgReq, kValue1X, kY3, kValue1W, 0, kID_Reticle, bPIButtonFlags_Check, "" );
    TVSetButtonInfoText( iPlugin, sgReq, kID_Reticle, TXT_HELP_RETICLE );

    TVAddButtonReq( iPlugin, sgReq, kLabel2X, kY3, kLabel2W, 0, kID_TransparentLabel, bPIButtonFlags_Flat, TXT_TRANSPARENT );
    TVChangeButtonName( iPlugin, sgReq, kID_TransparentLabel, TXT_TRANSPARENT, bPIButtonTextAlign_InRight );
    TVSetButtonInfoText( iPlugin, sgReq, kID_TransparentLabel, TXT_HELP_TRANSPARENT );
    TVAddButtonReq( iPlugin, sgReq, kValue2X, kY3, kValue2W, 0, kID_Transparent, bPIButtonFlags_Check, "" );
    TVSetButtonInfoText( iPlugin, sgReq, kID_Transparent, TXT_HELP_TRANSPARENT );

// Fourth row : Scope display
    TVAddButtonReq( iPlugin, sgReq, kDisplayX, kDisplayY, kDisplayW+4, kDisplayH+4, kID_Display, bPIButtonFlags_Invert | bPIButtonFlags_Quiet, NULL );
    TVSetButtonInfoText( iPlugin, sgReq, kID_Display, TXT_HELP_VECTOR );

// Fifth row : Apply button
    TVAddButtonReq( iPlugin, sgReq, kApplyX, kApplyY, kApplyW+4, kApplyH, kID_Apply, bPIButtonFlags_Action, TXT_APPLY ); // +4 to match the display geometry above
    TVSetButtonInfoText( iPlugin, sgReq, kID_Apply, TXT_HELP_APPLY );

    return  1;
}


static  void  UpdateGUI( PIPlugin*  iPlugin )
{
    char  temp[512];

    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    DrawModePopUp( iPlugin );

// Second row : Luminosity & Options Popup
    if( param->mode == kMode_Composite )
    {
        snprintf( temp, sizeof(temp), "%d", param->luminosity.composite );
        TVPutButtonString( iPlugin, sgReq, kID_Luminosity, temp );
        TVChangeButtonName( iPlugin, sgReq, kID_OptionsLabel, TXT_DISPLAY, bPIButtonTextAlign_InRight );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsLabel, TXT_HELP_DISPLAY );
        DrawCompositePopUp( iPlugin );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsPopup, TXT_HELP_DISPLAY );
    }
    else if( param->mode == kMode_VectorScope )
    {
        snprintf( temp, sizeof(temp), "%d", param->luminosity.vector );
        TVPutButtonString( iPlugin, sgReq, kID_Luminosity, temp );
        TVChangeButtonName( iPlugin, sgReq, kID_OptionsLabel, TXT_SCALE, bPIButtonTextAlign_InRight );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsLabel, TXT_HELP_SCALE );
        DrawScalePopUp( iPlugin );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsPopup, TXT_HELP_SCALE );
    }
    else if( param->mode == kMode_Components )
    {
        snprintf( temp, sizeof(temp), "%d", param->luminosity.component );
        TVPutButtonString( iPlugin, sgReq, kID_Luminosity, temp );
        TVChangeButtonName( iPlugin, sgReq, kID_OptionsLabel, TXT_COMPONENT, bPIButtonTextAlign_InRight );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsLabel, TXT_HELP_COMPONENT );
        DrawComponentPopUp( iPlugin );
        TVSetButtonInfoText( iPlugin, sgReq, kID_OptionsPopup, TXT_HELP_COMPONENT );
    }

// Third row : Reticle & Transparency
    TVChangeButtonReq( iPlugin, sgReq, kID_Reticle, bPIButtonFlags_Check | (param->reticle ? bPIButtonFlags_Select : 0), NULL );
    TVChangeButtonReq( iPlugin, sgReq, kID_Transparent, bPIButtonFlags_Check | (param->transparent ? bPIButtonFlags_Select : 0), NULL );

// Fourth row : Scope display
    // this is slow to update, so we update it only when strictly needed

// Fifth row : Apply button
    // nothing to update...
}


static  void  LoadConfig( PIPlugin*  iPlugin )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    char  temp[256];

    TVReadUserString( iPlugin, iPlugin->PIName, "Mode", temp, "0", 255 );
    param->mode = (eMode)atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "LuminosityComposite", temp, "50", 255 );
    param->luminosity.composite = atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "LuminosityVector", temp, "100", 255 );
    param->luminosity.vector = atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "LuminosityComponent", temp, "100", 255 );
    param->luminosity.component = atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "Composite", temp, "0", 255 );
    param->composite = (eComposite)atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "Scale", temp, "0", 255 );
    param->scale = (eScale)atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "Components", temp, "0", 255 );
    param->components = (eComponents)atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "Reticle", temp, "1", 255 );
    param->reticle = atoi( temp );

    TVReadUserString( iPlugin, iPlugin->PIName, "Transparent", temp, "1", 255 );
    param->transparent = atoi( temp );
}


static  void  SaveConfig( PIPlugin*  iPlugin, int  iOpen )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;
    char  temp[512];

    snprintf( temp, sizeof(temp), "%d", iOpen );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Open", temp );

    snprintf( temp, sizeof(temp), "%d", (int)param->mode );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Mode", temp );

    snprintf( temp, sizeof(temp), "%d", param->luminosity.composite );
    TVWriteUserString( iPlugin, iPlugin->PIName, "LuminosityComposite", temp );

    snprintf( temp, sizeof(temp), "%d", param->luminosity.vector );
    TVWriteUserString( iPlugin, iPlugin->PIName, "LuminosityVector", temp );

    snprintf( temp, sizeof(temp), "%d", param->luminosity.component );
    TVWriteUserString( iPlugin, iPlugin->PIName, "LuminosityComponent", temp );

    snprintf( temp, sizeof(temp), "%d", (int)param->composite );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Composite", temp );

    snprintf( temp, sizeof(temp), "%d", (int)param->scale );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Scale", temp );

    snprintf( temp, sizeof(temp), "%d", (int)param->components );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Components", temp );

    snprintf( temp, sizeof(temp), "%d", param->reticle );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Reticle", temp );

    snprintf( temp, sizeof(temp), "%d", param->transparent );
    TVWriteUserString( iPlugin, iPlugin->PIName, "Transparent", temp );
}

/****************************************************************/

int  STDCALL  PI_Open( PIPlugin*  iPlugin )
{
    char  temp[512];
    int  open;

    sgLocalFile = TVOpenLocalFile( iPlugin, "waveform.loc", 0 );

    strcpy( iPlugin->PIName, TXT_NAME );
    iPlugin->PIVersion = 2;
    iPlugin->PIRevision = 0;

    if( !iPlugin->UserParametersPtr )
    {
        UserParam*  param = (UserParam*)malloc(sizeof(UserParam));
        if( !param )
            return  0;

        memset( param, 0, sizeof(UserParam) );

        iPlugin->UserParametersPtr = (void *)param;
        iPlugin->UserParametersSize = sizeof(UserParam);
    }
    TVReadUserString( iPlugin, iPlugin->PIName, "Open", temp, "0", 255 );
    open = atoi(temp);
    if( open )
        PI_Parameters( iPlugin, NULL );

    return  1;
}

/****************************************************************/

void  STDCALL  PI_About( PIPlugin*  iPlugin )
{
    char  text[256];

    snprintf( text, sizeof(text),
              "%s V%d,%d\nCopyright 2025 TVPaint Développement",
              iPlugin->PIName, iPlugin->PIVersion, iPlugin->PIRevision );

    TVWarning( iPlugin, text );
}

/****************************************************************/

int  STDCALL  PI_Parameters( PIPlugin*  iPlugin, const char*  iArg )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;

    if( iArg )
        return  1; // we don't handle this case...

    if( sgReq != 0 )
    {
        TVReqToFront( iPlugin, sgReq );
        return  1;
    }

    LoadConfig( iPlugin );

    if( !BuildGUI( iPlugin ) )
        return  0;

    UpdateGUI( iPlugin );

    DoPalette( iPlugin );
    DrawVectorScope( iPlugin );

    return  1;
}

/****************************************************************/

int  STDCALL  PI_Msg( PIPlugin*  iPlugin, INTPTR  iEvent, INTPTR  iReq, INTPTR*  iTag )
{
    UserParam*  param = (UserParam*)iPlugin->UserParametersPtr;
    char  temp[512];

    switch( iEvent )
    {
        case kPIEvents_ButtonText:
            switch( iTag[0] )
            {
                case kID_Luminosity:
                    TVGetButtonString( iPlugin, iReq, iTag[0], temp, 511 );
                    for( int  i = 0; temp[i] != 0; ++i )
                    {
                        if( !isdigit( temp[i] )  &&  temp[i] != '.' )
                        {
                            memmove( &temp[i], &temp[i+1], strlen( &temp[i] ) );
                            TVPutButtonString( iPlugin, iReq, iTag[0], temp );
                            break;
                        }
                    }
                    break;
            }
            break;

        case kPIEvents_ButtonMiniSlider:
            switch( iTag[0] )
            {
                case kID_LuminositySlider:
                    if( param->mode == kMode_Composite )
                    {
                        param->luminosity.composite += *((int*)&iTag[1]); /* delta of move */
                        param->luminosity.composite = CLAMP( param->luminosity.composite, 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.composite );
                        TVPutButtonString( iPlugin, iReq, kID_Luminosity, temp );
                    }
                    else if( param->mode == kMode_VectorScope )
                    {
                        param->luminosity.vector += *((int*)&iTag[1]); /* delta of move */
                        param->luminosity.vector = CLAMP( param->luminosity.vector, 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.vector );
                        TVPutButtonString( iPlugin, iReq, kID_Luminosity, temp );
                    }
                    else if( param->mode == kMode_Components )
                    {
                        param->luminosity.component += *((int*)&iTag[1]); /* delta of move */
                        param->luminosity.component = CLAMP( param->luminosity.component, 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.component );
                        TVPutButtonString( iPlugin, iReq, kID_Luminosity, temp );
                    }
                    break;
            }
            break;

        case kPIEvents_ButtonUp:
            switch( iTag[0] )
            {
                case kID_Apply:
                    TVExecute( iPlugin );
                    break;

                case kID_Reticle:
                    param->reticle = !param->reticle;
                    TVChangeButtonReq( iPlugin, iReq, kID_Reticle, bPIButtonFlags_Check | (param->reticle ? bPIButtonFlags_Select : 0), NULL );
                    break;

                case kID_Transparent:
                    param->transparent = !param->transparent;
                    TVChangeButtonReq( iPlugin, iReq, kID_Transparent, bPIButtonFlags_Check | (param->transparent ? bPIButtonFlags_Select : 0), NULL );
                    DoPalette( iPlugin );
                    break;

                case kID_Display:
                    DrawVectorScope( iPlugin );
                    break;

                case kID_LuminositySlider:
                case kID_Luminosity:
                    TVGetButtonString( iPlugin, iReq, kID_Luminosity, temp, 511 );
                    if( param->mode == kMode_Composite )
                    {
                        param->luminosity.composite = CLAMP( atoi(temp), 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.composite );
                    }
                    else if( param->mode == kMode_VectorScope )
                    {
                        param->luminosity.vector = CLAMP( atoi(temp), 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.vector );
                    }
                    else if( param->mode == kMode_Components )
                    {
                        param->luminosity.component = CLAMP( atoi(temp), 0, 1000 );
                        snprintf( temp, sizeof(temp), "%d", param->luminosity.component );
                    }
                    TVPutButtonString( iPlugin, iReq, iTag[0], temp );
                    break;

                case kID_Mode:
                    HandleModePopUp( iPlugin );
                    DrawModePopUp( iPlugin );
                    UpdateGUI( iPlugin );
                    break;

                case kID_OptionsPopup:
                    if( param->mode == kMode_Composite )
                    {
                        HandleCompositePopUp( iPlugin );
                        DrawCompositePopUp( iPlugin );
                    }
                    else if( param->mode == kMode_VectorScope )
                    {
                        HandleScalePopUp( iPlugin );
                        DrawScalePopUp( iPlugin );
                    }
                    else if( param->mode == kMode_Components )
                    {
                        HandleComponentPopUp( iPlugin );
                        DrawComponentPopUp( iPlugin );
                    }
                    break;
            }
            DrawVectorScope( iPlugin );
            break;

        case kPIEvents_ProjectChanged:
            DrawVectorScope( iPlugin );
            break;

        case kPIEvents_WindowClose:
            SaveConfig( iPlugin, (int)iTag[4] );

            sgReq = 0;
            break;
    }
    return  1;
}

/****************************************************************/

int  STDCALL  PI_SequenceStart( PIPlugin*  iPlugin,  int iNum )
{
    return  1;
}

int  STDCALL  PI_Start( PIPlugin*  iPlugin, double  iPos, double  iSize )
{
    return  1;
}

int  STDCALL  PI_Work( PIPlugin*  iPlugin )
{
    ExecuteVectorScope( iPlugin, iPlugin->Current, iPlugin->Undo );

    return  1;
}

void  STDCALL  PI_Finish( PIPlugin*  iPlugin )
{
}

void  STDCALL  PI_SequenceFinish( PIPlugin*  iPlugin )
{
}

/****************************************************************/

void  STDCALL  PI_Close( PIPlugin*  iPlugin )
{
    if( sgReq != 0 )
        TVCloseReq( iPlugin, sgReq );

    if( iPlugin->UserParametersPtr )
    {
        free( iPlugin->UserParametersPtr );
        iPlugin->UserParametersPtr = NULL;
    }
}

