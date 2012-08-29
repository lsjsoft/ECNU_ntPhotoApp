#pragma once

class ntScreen;

class ntCanvasCtl: public CStatic
{
public:
	ntCanvasCtl(void);
	~ntCanvasCtl(void);

	void initClt();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	ntScreen* getScreen() const { return m_pScreen; }

	void beginDraw();

	void endDraw();

	HWND operator()() { return GetSafeHwnd(); }

	virtual afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

protected:
	ntScreen* m_pScreen;
};
