/** @copyright (c) 2025 TVPaint Developpement. All Rights Reserved.
 *
 *  @file  Keys.cpp
 *  @ingroup  tvpl_SDK_CPP
 *
 *  @brief  TVPaint SDK for C++ : A keyset.
 * //TODO: //NAME: file+class
 */


#include "Keys.h"

#include "Plugin.h"

#include "pi-basics.h"
#include "pi-keys.h"


#include <map>


namespace  nSDK {


//MARK: ========================================================================================================= cPI2Keys


//-------------------------------------------------------------------------------------- Construction/Destruction


cPI2Keys::~cPI2Keys()
{
    mLegacyKeys->free( mLegacyKeys );
    mLegacyKeys = nullptr;
}


static
void
UpdateViewCB( struct PIKeys*  iKeys, int  iID )
{
    cPI2Keys*  keys = (cPI2Keys*)iKeys->pidata;
    if( keys )
        keys->CallUpdateViews( iID );
}


cPI2Keys::cPI2Keys( cTV2Plugin*  iPlugin, const std::string&  iName, const std::string&  iNameID )
{
    mLegacyKeys = TVAllocKeys( iPlugin->LegacyPlugin(), iName.c_str(), iNameID.c_str() );
    mLegacyKeys->pidata = this;
    mLegacyKeys->refresh = &UpdateViewCB;
}


//------------------------------------------------------------------------------------------------ Plugin section


void
cPI2Keys::SetUpdateViews( tUpdateViews  iUpdateViews )
{
    mUpdateViews = iUpdateViews;
}


void
cPI2Keys::CallUpdateViews( int  iID )
{
    if( mUpdateViews )
        mUpdateViews( this, iID );
}


/** @todo //TODO: ?!?!? */
//TODO:    void  (*posrefreshCB)( cPI2Keys*  iKeys ); // callback

//-------------------------------------------------------------------------------------------------- Host section


int
//TODO: template fct
//TODO: return error code
//TODO: int  iType => enum
//TODO: also for a lot of methods below
cPI2Keys::AddParameter( int  iID, const std::string&  iName, int  iType, const void*  iValue )
{
    return  mLegacyKeys->addparam( mLegacyKeys, iID, iName.c_str(), iType, iValue );
}


int
cPI2Keys::SetValue( int  iID, const void*  iValue )
{
    return  mLegacyKeys->setval( mLegacyKeys, iID, iValue );
}


int
cPI2Keys::GetValue( int  iID, void*  oValue )
{
    return  mLegacyKeys->getval( mLegacyKeys, iID, oValue );
}


int
cPI2Keys::GetValueAtPosition( int  iID, double  iTime, void*  oValue )
{
    return  mLegacyKeys->getvalpos( mLegacyKeys, iID, iTime, oValue );
}


int
cPI2Keys::Save( void*  iFile, const std::string&  iSection )
{
    return  mLegacyKeys->save( mLegacyKeys, iFile, iSection.c_str() );
}


int
cPI2Keys::Load( void*  iFile, const std::string&  iSection )
{
    return  mLegacyKeys->load( mLegacyKeys, iFile, iSection.c_str() );
}


void
cPI2Keys::Lock()
{
    mLegacyKeys->lock( mLegacyKeys );
}


void
cPI2Keys::Unlock()
{
    mLegacyKeys->unlock( mLegacyKeys );
}


void
cPI2Keys::RemoveParameter( int  iID )
{
    mLegacyKeys->removeparam( mLegacyKeys, iID );
}


int
cPI2Keys::CopyParameter( int  iIDSrc, struct PIKeys*  oKeysDst, int  iIDDst, double  iDeltaT )
{
    return  mLegacyKeys->copyparam( mLegacyKeys, iIDSrc, oKeysDst, iIDDst, iDeltaT );
}


void
cPI2Keys::RenameParameter( int  iID, const std::string&  iName )
{
    mLegacyKeys->renameparam( mLegacyKeys, iID, iName.c_str() );
}


} // namespace  nSDK

