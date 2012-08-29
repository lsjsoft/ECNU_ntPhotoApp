#pragma once
#include "ntServiceBase.h"
#include "ntTexture.h"

/** �ʲ��������
	shaojingliu 2012-07-28
*/
class ntAssetService: public ntServiceBase
{
	ntImplService(ntServiceBase);
public:
	ntAssetService(void);
	~ntAssetService(void);

	virtual void start();

	/** �õ��������� */
	ntTexture32Ptr getBackground();

protected:
	/** �������� */
	ntTexture32Ptr m_backgroundTexture;
};
