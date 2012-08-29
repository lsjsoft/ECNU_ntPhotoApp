#include "StdAfx.h"
#include "ntGdiObject.h"

ntGdiObject::ntGdiObject(void)
:m_hObject(0)
{
}

ntGdiObject::~ntGdiObject(void)
{
	reset();
}

void ntGdiObject::reset()
{
	if (m_hObject)
	{
		DeleteObject(m_hObject);
		m_hObject= 0;
	}
}

HGDIOBJ ntGdiObject::getHandle()
{
	return m_hObject;
}

