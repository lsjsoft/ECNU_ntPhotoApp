#pragma once
#include "ntServiceBase.h"
#include "ntTexture.h"

/** 资产服务组件
	shaojingliu 2012-07-28
*/
class ntAssetService: public ntServiceBase
{
	ntImplService(ntServiceBase);
public:
	ntAssetService(void);
	~ntAssetService(void);

	virtual void start();

	/** 得到背景纹理 */
	ntTexture32Ptr getBackground();

protected:
	/** 背景纹理 */
	ntTexture32Ptr m_backgroundTexture;
};
