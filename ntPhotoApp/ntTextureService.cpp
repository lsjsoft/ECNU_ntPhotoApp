#include "StdAfx.h"
#include "ntTextureService.h"
#include "ntNaturalImage.h"

ntTextureService::ntTextureService(void)
{
}

ntTextureService::~ntTextureService(void)
{
	m_TexMap.clear();
}

ntTexture32Ptr ntTextureService::create( const std::string& rkPathname, bool bForceLoad )
{
	if (!bForceLoad)
	{
		ntTexturePtrMap::iterator it = m_TexMap.find( rkPathname);
		if ( it != m_TexMap.end())
		{
			return it->second;
		}
	}

	ntNaturalImage naImage;
	if (! naImage.load( rkPathname))
	{
		return NULL;
	}

	ntTexture32Ptr spTexture32Ptr= ntNew ntTexture32();
	spTexture32Ptr->createImmediate( naImage.getWidth(), 
		naImage.getHeight(), naImage.getBuffer());
	m_TexMap[rkPathname] = spTexture32Ptr;

	return spTexture32Ptr;
}
