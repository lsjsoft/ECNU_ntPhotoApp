#pragma once

struct ntTextureInfo
{
	ntTextureInfo()
	: m_uiWidth(0)
	, m_uiHeight(0)
	, m_uiVersion(0)
	, m_uiId(0)
	{
	}

	void clear()
	{
		m_uiHeight = 0;
		m_uiWidth = 0;
		m_uiVersion = 0;
	}

	unsigned int getVersion() const 
	{
		return m_uiVersion;
	}

	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	unsigned int m_uiVersion;
	unsigned int m_uiId;
};
