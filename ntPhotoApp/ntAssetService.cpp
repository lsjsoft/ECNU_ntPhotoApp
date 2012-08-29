#include "StdAfx.h"
#include "ntAssetService.h"
#include "ntTextureService.h"
#include "ntService.h"

const std::string backgroundPathame= "./media/background.png";

ntAssetService::ntAssetService(void)
{
}

ntAssetService::~ntAssetService(void)
{
}

ntTexture32Ptr ntAssetService::getBackground()
{
	return m_backgroundTexture;
}

void ntAssetService::start()
{
	m_backgroundTexture = ntGetService(ntTextureService)->create(backgroundPathame);
}
