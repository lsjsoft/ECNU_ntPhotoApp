#pragma once
#include "ntTexture.h"

class ntDibBitmap;
class ntDC;

/**
	屏幕绘制类：ntScreen
	shaojingliu 2012-07-28
*/
class ntScreen
{
public:
	ntScreen(void);
	~ntScreen(void);

	void setPresentHandle(HWND hWnd);

	void create(unsigned int uiCanvasWidth= 500, unsigned int uiCanvasHeight= 400);

	bool createEx(HWND hWnd);

	void beginRender();

	void endRender();

	void* getBackbackBuffer();

	const ntRecti& getCanvasRect() const { return m_rcCanvasRect; }

	unsigned int getPitch() const { return m_rcCanvasRect.w * sizeof(ntPixel32); }

	void resize(unsigned int uiCanvasWidth, unsigned int uiCanvasHeight);

	void drawLine(ntPointi* p0, ntPointi* p1, unsigned int dwColor);

	void drawRect(const ntRecti& a, unsigned int dwColor);

	void drawPixel(unsigned int x, unsigned int y, unsigned int color);

	void drawTexture( ntTexture32Ptr spTexture, int x, int y);

	/** 可以绘制包装的纹理
	@parma [in] spTexture 纹理
	@param [in] drawRect 绘制区域
	*/
	bool drawTextureWrap( ntTexture32Ptr spTexture, const ntRecti& aDrawRect);

	bool drawTextureEx( ntTexture32Ptr spTexture, const ntRecti& aDrawRect, ntBlendMode blendMode = BM_ALPHA);

	void drawTextureInternal(ntPixel32* pSource, int iCanvasX, int iCanvasY, 
		int iWidth, int iHeight, int iTextureX, int iTextureY, int iTextureWidth);

	unsigned char getBackgroundColor() const;

	void setBackgroundColor(unsigned char val);

protected:
	void _createDc(unsigned int uiCanvasWidth, unsigned int uiCanvasHeight);

	void setViewRect();

protected:
	ntDibBitmap* m_dibBitmap;
	ntDC* m_drawDc;
	HWND m_hPresentHandle;
	HDC m_hPresentDC;
	unsigned int m_uiMaxAddress;
	ntRecti m_rcCanvasRect;
	ntRecti m_rcViewRect;
	void* m_pBackbufferData;
	unsigned char m_ucBackgroundColor;
};
