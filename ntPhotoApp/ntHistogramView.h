#pragma once
#include "ntHistogram.h"
#include "afxwin.h"
#include "ntCanvasCtl.h"
#include "GeWinAnchor.h"


class ntPhotoAppView;
// ntHistogramView dialog
class ntScreen;

class ntHistogramView : public CDialog
{
	DECLARE_DYNAMIC(ntHistogramView)

public:
	ntHistogramView(CWnd* pParent = NULL);   // standard constructor
	virtual ~ntHistogramView();

// Dialog Data
	enum { IDD = IDD_HISTOGRAM };

	ntHistogramPtr getHistogram() const { return m_spHistogram; }
	void setHistogram(ntHistogramPtr val) { m_spHistogram = val; }

	void updateHistogram();

	void makeInvalid();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP();


	ntHistogramPtr m_spHistogram;

	unsigned int m_uiTextureId;
	unsigned int m_uiTextureVersion;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	ntCanvasCtl m_histogramCanvas;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
