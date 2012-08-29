#pragma once
#include "ntTextureInfo.h"
#include "ntBuffer.h"

template<typename T>
class ntTexture: public ntCountedObject
{
public:
	ntTexture(void)
	{
		static unsigned int g_id = 0;
		m_textureInfo.m_uiId = ++g_id;
	}

	~ntTexture(void)
	{
	}

	bool createEmpty(unsigned int uiWidth, unsigned int uiHeight)
	{
		setBufferValid();
		m_textureInfo.m_uiWidth = uiWidth;
		m_textureInfo.m_uiHeight = uiHeight;
		m_spBuffer->allocBuffer<T>(uiWidth * uiHeight);
		return true;
	}

	bool operator == ( unsigned int id)
	{
		return equalId(id);
	}

	bool equalId( unsigned int id) const
	{
		return m_textureInfo.m_uiId == id;
	}

	bool equalVersion( unsigned int uiVersion) const
	{
		return m_textureInfo.m_uiVersion == uiVersion;
	}

	bool equalx( unsigned int id, unsigned int uiVersion) const
	{
		return equalId(id) && equalVersion(uiVersion);
	}

	unsigned int getId() const 
	{
		return m_textureInfo.m_uiId;
	}

	void createImmediate(unsigned int uiWidth, unsigned int uiHeight, ntBufferPtr buf)
	{
		clear();

		if (buf)
		{
			m_textureInfo.m_uiHeight = uiHeight;
			m_textureInfo.m_uiWidth = uiWidth;
			m_spBuffer = buf;
		}
	}

	T* getBuffer()
	{
		if (m_spBuffer)
		{
			return m_spBuffer->getBuffer<T>();
		}

		return NULL;
	}

	T* getPixel(unsigned int uiX, unsigned int uiY)
	{
		T* pBuffer = getBuffer();
		return &pBuffer[ uiY * getWidth() + uiX ];
	}

	unsigned int getWidth() const 
	{
		return m_textureInfo.m_uiWidth;
	}

	unsigned int getHeight() const 
	{
		return m_textureInfo.m_uiHeight;
	}

	unsigned int getSize() const 
	{
		return m_textureInfo.m_uiWidth * m_textureInfo.m_uiHeight;
	}

	void clear()
	{
		m_textureInfo.clear();
		m_spBuffer = 0;
	}


protected:
	void setBufferValid()
	{
		if (!m_spBuffer)
		{
			m_spBuffer = ntNew ntBuffer();
		}
	}

protected:
	ntTextureInfo m_textureInfo;
	ntBufferPtr m_spBuffer;
};

//////////////////////////////////////////////////////////////////////////

class ntTexture32;
typedef ntCountedPtr<ntTexture32> ntTexture32Ptr;

class ntTexture32: public ntTexture<ntPixel32>
{
public:
	ntTexture32Ptr clone()
	{
		ntTexture32Ptr pNewTex= ntNew ntTexture32();
		pNewTex->m_spBuffer = m_spBuffer->clone();
		pNewTex->m_textureInfo = m_textureInfo;
		return pNewTex;
	}

	~ntTexture32()
	{

	}

	void accept(ntPixel32* src, unsigned int uiWidth, unsigned int uiHeight)
	{
		m_spBuffer->clear();
		m_spBuffer->allocBuffer<ntPixel32>(uiWidth*uiHeight);
		void* pTarget= m_spBuffer->getBufferPtr();
		memcpy(pTarget, src, sizeof(ntPixel32)*uiWidth*uiHeight);
		m_textureInfo.m_uiWidth = uiWidth;
		m_textureInfo.m_uiHeight = uiHeight;
		m_textureInfo.m_uiVersion++;
	}

	bool copyFrom(ntTexture32Ptr src)
	{
	 	if (src == 0)
	 	{
	 		return false;
	 	}
	 
	 	unsigned int uiDestWidth = getWidth();
	 	unsigned int uiDestHeight= getHeight();
	 	unsigned int uiSrcWidth = src->getWidth();
	 	unsigned int uiSrcHeight= src->getHeight();
	 
	 	if (uiDestHeight == uiSrcHeight && uiDestWidth == uiSrcWidth &&
	 		uiDestHeight > 0 && uiDestWidth > 0 )
	 	{
	 		memcpy(getBuffer(), src->getBuffer(), 
	 			uiSrcWidth* uiSrcHeight*sizeof(ntPixel32) );
	 
			setModify();

	 		return true;
	 	}
	 
	 	return false;
	}

	unsigned int getVersion()
	{
		return m_textureInfo.m_uiVersion;
	}

	unsigned int setModify()
	{
		return m_textureInfo.m_uiVersion++;
	}
};

