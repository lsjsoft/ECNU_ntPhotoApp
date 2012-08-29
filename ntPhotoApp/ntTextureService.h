#pragma once
#include "ntServiceBase.h"
#include "ntTexture.h"

class ntTextureService: public ntServiceBase
{
	ntImplService(ntTextureService);

public:
	ntTextureService(void);
	~ntTextureService(void);

	ntTexture32Ptr create( const std::string& rkPathname, bool bForceLoad= false);

protected:
	typedef std::map< std::string, ntTexture32Ptr > ntTexturePtrMap;
	ntTexturePtrMap m_TexMap;
};
