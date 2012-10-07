#pragma once
#include "ntOp.h"
#include "ntTexture.h"

class ntOpPen: public ntOp
{
public:
	ntOpPen(void);
	~ntOpPen(void);

public:
	static opIdType _GetOpId();

	virtual void onEnter();
	virtual void onLeave();

	virtual void onInit();

	virtual HCURSOR getCursor();

	virtual bool OnMouseLBDown(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseLBUp(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseMove(const ntPointi& p, ntCpsBv bv);

	void SetTrans(int iValue);
	void SetColor(COLORREF color);

protected:
	HCURSOR m_hPen;
	ntPointi m_lastPoint;
	bool m_bLeftPress;
	ntTexture32Ptr m_opTex;
	ntPixel32 m_color;
};
