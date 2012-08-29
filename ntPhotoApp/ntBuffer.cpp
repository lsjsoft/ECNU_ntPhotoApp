#include "StdAfx.h"
#include "ntBuffer.h"

ntBuffer::ntBuffer(void)
: m_pBuffer(0)
, m_uiLength(0)
{
}

ntBuffer::~ntBuffer(void)
{
	clear();
}

void* ntBuffer::allocBufferSize( unsigned int uiLength )
{
	clear();

	m_uiLength = uiLength;

	if (m_uiLength > 0)
	{
		m_pBuffer = malloc( uiLength );
	}

	return m_pBuffer;
}

void ntBuffer::clear()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = 0;
	}

	m_uiLength = 0;
}
