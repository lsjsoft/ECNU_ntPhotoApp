#pragma once

class ntGdiObject
{
public:
	ntGdiObject(void);
	~ntGdiObject(void);

	void reset();

	HGDIOBJ getHandle();

protected:
	HGDIOBJ	m_hObject;
};
