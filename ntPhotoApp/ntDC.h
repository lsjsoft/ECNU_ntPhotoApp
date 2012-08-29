#pragma once

class ntDC
{
public:
	ntDC(void);
	~ntDC(void);

	operator HDC()  { return m_hDC; }
	bool create(HDC dc);
	void reset();
	void select(HGDIOBJ hObj);

protected:
	HDC m_hDC;
};
