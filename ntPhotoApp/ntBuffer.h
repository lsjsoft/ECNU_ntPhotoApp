#pragma once

class ntBuffer;
typedef ntCountedPtr<ntBuffer> ntBufferPtr;

class ntBuffer: public ntCountedObject
{
public:
	ntBuffer(void);
	~ntBuffer(void);

	unsigned int getLength() const 
	{
		return m_uiLength;
	}

	void* getBufferPtr() const 
	{
		return m_pBuffer;
	}

	void* allocBufferSize(unsigned int uiLength);

	template<typename T>
	T* allocBuffer(unsigned int uiNum)
	{
		return (T*)allocBufferSize( sizeof(T) * uiNum );
	}

	template<typename T>
	T* getBuffer()
	{
		return (T*)(m_pBuffer);
	}

	void clear();

	ntBufferPtr clone()
	{
		ntBufferPtr pNewBuffer= ntNew ntBuffer();
		pNewBuffer->allocBufferSize( m_uiLength );
		memcpy(pNewBuffer->m_pBuffer, m_pBuffer, m_uiLength );
		return pNewBuffer;
	}

protected:
	void* m_pBuffer;
	unsigned int m_uiLength;
};

