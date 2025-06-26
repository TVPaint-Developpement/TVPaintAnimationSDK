

#include  "lumarange.h"


// The 'luminosity/replace' mode.
static void
LumaRangeLuminosityReplace( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams )
{
    int  i;

    // some initializations
    int  size = oDst->Width * oDst->Height;
    PIPixel*  table = iParams->mColors->mColors;
    PIPixel*  src = iSrc->Data;
    PIPixel*  dst = oDst->Data;

    // get the offsets of the channels in the pixel
    int  ro = iSrc->r;
    int  go = iSrc->g;
    int  bo = iSrc->b;
    int  ao = iSrc->a;

    // loop over all the pixels, here we don't care about their x/y positions...
    for( i = 0; i < size; ++i )
    {
        // get the source alpha channel
        int  a = src->c[ao];

        if( a != 0 )
        {
            // if not fully transparent, get the other three channels
            int  r = src->c[ro];
            int  g = src->c[go];
            int  b = src->c[bo];

            // compute the index in the color range
            // those strange values are the usual 0.30, 0.59, 0.11 in fixed mode
            int  idx = ((r*19584)+(g*38464)+(b*7488)) >> 16;

            // replace the destination with the color from the color range
            dst->l = table[idx].l;
        }
        else
        {
            // if fully transparent, take the first value of the color range
            dst->l = table[0].l;
        }

        // and on to the next pixel..
        ++src;
        ++dst;
    }
}


// The 'alpha,red,green,blue/replace' mode.
static void
LumaRangeChannelReplace( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams )
{
    int  i;

    // some initializations
    int  size = oDst->Width * oDst->Height;
    PIPixel*  table = iParams->mColors->mColors;
    PIPixel*  src = iSrc->Data;
    PIPixel*  dst = oDst->Data;

    // get the offsets of the channels in the pixel
    int  ro = iSrc->r;
    int  go = iSrc->g;
    int  bo = iSrc->b;
    int  ao = iSrc->a;

    // channel to use as source
    int  channel = 0;
    switch( iParams->mChannel )
    {
    case kAlpha:  channel = ao; break;
    case kRed:    channel = ro; break;
    case kGreen:  channel = go; break;
    case kBlue:   channel = bo; break;
    case kLuminosity: ; break; // this is forbidden !
    }

    if( iParams->mChannel != kAlpha )
    {
        // not the alpha channel as source

        // loop over all the pixels, here we don't care about their x/y positions...
        for( i = 0; i < size; ++i )
        {
            if( src->c[ao] != 0 )
            {
                // if not fully transparent, get the channel's value as index
                int  idx = src->c[channel];

                // replace the destination with the color from the color range
                dst->l = table[idx].l;
            }
            else
            {
                // if fully transparent, take the first value of the color range
                dst->l = table[0].l;
            }

            // and on to the next pixel...
            ++src;
            ++dst;
        }
    }
    else
    {
        // the alpha channel as source, a little faster version...

        // loop over all the pixels, here we don't care about their x/y positions...
        for( i = 0; i < size; ++i )
        {
            int  idx = src->c[channel];

            // replace the destination with the color from the color range
            dst->l = table[idx].l;

            // and on to the next pixel...
            ++src;
            ++dst;
        }
    }
}


// The 'luminosity' mode.
static void
LumaRangeLuminosity( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams )
{
    int  i;

    // some initializations
    int  size = oDst->Width * oDst->Height;
    PIPixel*  table = iParams->mColors->mColors;
    PIPixel*  src = iSrc->Data;
    PIPixel*  dst = oDst->Data;

    // get the offsets of the channels in the pixel
    int  ro = iSrc->r;
    int  go = iSrc->g;
    int  bo = iSrc->b;
    int  ao = iSrc->a;

    // loop over all the pixels, here we don't care about their x/y positions...
    for( i = 0; i < size; ++i )
    {
        // get the source alpha channel
        int  a = src->c[ao];

        if( a != 0 )
        {
            // if not fully transparent...
            if( a == 255 )
            {
                // if fully opaque, the channel values are the color values
                // (see the SDK docs for the meaning of this sentence :)
                int  r = src->c[ro];
                int  g = src->c[go];
                int  b = src->c[bo];

                // compute the index in the color range
                int  idx = ((r*19584)+(g*38464)+(b*7488)) >> 16;

                // replace the destination with the color from the color range
                // (we don't have to blend anything because the pixel is opaque)
                dst->l = table[idx].l;
            }
            else
            {
                // if neither fully transparent nor fully opaque,
                // the channel values have to be computed from the color values and the alpha channel
                // (see the SDK docs for the meaning of this sentence :)
                int  r = (src->c[ro] * 255) / a;
                int  g = (src->c[go] * 255) / a;
                int  b = (src->c[bo] * 255) / a;

                // compute the index in the color range
                int  idx = ((r*19584)+(g*38464)+(b*7488)) >> 16;

                // blend the new color with the alpha channel of the source
                dst->c[ro] = (table[idx].c[ro] * a) / 255;
                dst->c[go] = (table[idx].c[go] * a) / 255;
                dst->c[bo] = (table[idx].c[bo] * a) / 255;
                dst->c[ao] = a;
            }
        }
        else
        {
            // if fully transparent, destination will also be fully transparent
            dst->l = 0x00000000;
        }

        // and on to the next pixel..
        ++src;
        ++dst;
    }
}


// The 'alpha,red,green,blue' mode.
static void
LumaRangeChannel( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams )
{
    int  i;

    // some initializations
    int  size = oDst->Width * oDst->Height;
    PIPixel*  table = iParams->mColors->mColors;
    PIPixel*  src = iSrc->Data;
    PIPixel*  dst = oDst->Data;

    // get the offsets of the channels in the pixel
    int  ro = iSrc->r;
    int  go = iSrc->g;
    int  bo = iSrc->b;
    int  ao = iSrc->a;

    // channel to use as source
    int  channel = 0;
    switch( iParams->mChannel )
    {
    case kAlpha:  channel = ao; break;
    case kRed:    channel = ro; break;
    case kGreen:  channel = go; break;
    case kBlue:   channel = bo; break;
    case kLuminosity: ; break; // this is forbidden !
    }

    // loop over all the pixels, here we don't care about their x/y positions...
    for( i = 0; i < size; ++i )
    {
        // get the source alpha channel
        int  a = src->c[ao];

        if( a != 0 )
        {
            // if not fully transparent...
            if( a == 255 )
            {
                // if fully opaque, the channel values are the color values
                // (see the SDK docs for the meaning of this sentence :)

                // get the channel's value as index
                int  idx = src->c[channel];

                // replace the destination with the color from the color range
                // (we don't have to blend anything because the pixel is opaque)
                dst->l = table[idx].l;
            }
            else
            {
                // if neither fully transparent nor fully opaque,
                // the channel values have to be computed from the color values and the alpha channel
                // (see the SDK docs for the meaning of this sentence :)

                // get the channel's value as index
                int  idx = src->c[channel];

                // if this is not the alpha channel, make the necessary computations
                if( iParams->mChannel != kAlpha )
                {
                    idx = (idx * 255) / a;
                }

                // blend the new color with the alpha channel of the source
                dst->c[ro] = (table[idx].c[ro] * a) / 255;
                dst->c[go] = (table[idx].c[go] * a) / 255;
                dst->c[bo] = (table[idx].c[bo] * a) / 255;
                dst->c[ao] = a;
            }
        }
        else
        {
            // if fully transparent, destination will also be fully transparent
            dst->l = 0x00000000;
        }

        // and on to the next pixel..
        ++src;
        ++dst;
    }
}


// The main function of the effect, just dispatches to the right function.
void
LumaRange( const PIBlock* iSrc, PIBlock* oDst, const LumaRangeParams* iParams )
{
    // some sanity checks
    if( !iSrc  ||  !oDst )
        return;
    if( iSrc->Width != oDst->Width  ||  iSrc->Height != oDst->Height )
        return;

    if( iParams->mReplace )
    {
        switch( iParams->mChannel )
        {
        case kLuminosity:
            LumaRangeLuminosityReplace( iSrc, oDst, iParams );
            break;

        case kAlpha:
        case kRed:
        case kGreen:
        case kBlue:
            LumaRangeChannelReplace( iSrc, oDst, iParams );
            break;
        }
    }
    else
    {
        switch( iParams->mChannel )
        {
        case kLuminosity:
            LumaRangeLuminosity( iSrc, oDst, iParams );
            break;

        case kAlpha:
        case kRed:
        case kGreen:
        case kBlue:
            LumaRangeChannel( iSrc, oDst, iParams );
            break;
        }
    }
}

