#include "StdAfx.h"
#include "ntScreen.h"
#include "ntDibBitmap.h"
#include "ntDC.h"

ntScreen::ntScreen(void)
: m_hPresentHandle(0)
, m_hPresentDC(0)
, m_uiMaxAddress(0)
, m_ucBackgroundColor(0xFF)
{
	m_dibBitmap = ntNew ntDibBitmap;
	m_drawDc = ntNew ntDC;
}

ntScreen::~ntScreen(void)
{
	delete m_drawDc;
	delete m_dibBitmap;
}

void ntScreen::create(unsigned int uiCanvasWidth, unsigned int uiCanvasHeight)
{
	_createDc(uiCanvasWidth, uiCanvasHeight);
}

void ntScreen::setPresentHandle( HWND hWnd )
{
	m_hPresentHandle = hWnd;
	m_hPresentDC= ::GetDC(m_hPresentHandle);
}

void ntScreen::beginRender()
{
	if (!m_hPresentDC)
	{
		return;
	}

	unsigned int* pSrc= static_cast<unsigned int*>( getBackbackBuffer() );
	unsigned int uiSize = m_dibBitmap->getHeight() 
		* m_dibBitmap->getWidth() * sizeof(unsigned int);

	memset(pSrc, m_ucBackgroundColor, uiSize);
}

void* ntScreen::getBackbackBuffer()
{
	return m_dibBitmap->getData();
}

void ntScreen::endRender()
{
	if (!m_hPresentDC)
	{
		return;
	}

// 	ntRecti tmp = m_rcCanvasRect;
// 	tmp.w --;
// 	tmp.h --;
// 	drawRect( tmp, 0xFF00FF00);

	int x = 0;//(int)( ( (int)m_rcViewRect.w - (int)m_rcCanvasRect.w ) * 0.5f);
	int y = 0;//(int)( ( (int)m_rcViewRect.h - (int)m_rcCanvasRect.h ) * 0.5f);

	::BitBlt(m_hPresentDC, x, y, 
		m_dibBitmap->getWidth(), 
		m_dibBitmap->getHeight(), 
		*m_drawDc, 0, 0, SRCCOPY);
}

void ntScreen::resize(unsigned int uiCx, unsigned int uiCy)
{
	create(uiCx, uiCy);
}

void ntScreen::_createDc(unsigned int uiCanvasWidth, unsigned int uiCanvasHeight)
{
	setViewRect();

	m_dibBitmap->create(m_hPresentDC, uiCanvasWidth, uiCanvasHeight);

	m_drawDc->create(m_hPresentDC);
	m_drawDc->select(m_dibBitmap->getHandle());

	m_rcCanvasRect.w = uiCanvasWidth;
	m_rcCanvasRect.h = uiCanvasHeight;

	m_uiMaxAddress = m_rcCanvasRect.w * m_rcCanvasRect.h;
	m_pBackbufferData = m_dibBitmap->getData();
}


BOOL wiz_3d_clip_2d_line(const ntRecti* vb_p, ntPointi* io_begin_p, ntPointi* io_end_p)
{
	#define WIZ_MATH_IS_ZERO_F(a)	(a == 0.0f)
	#define WIZ_MATH_LESS_ZERO_F(a)	(a < 0.0f )

	int ww= vb_p->width();
	int hh= vb_p->height();

	/// 垂直
	if (WIZ_MATH_IS_ZERO_F(io_end_p->x - io_begin_p->x))
	{
		if (WIZ_MATH_LESS_ZERO_F(io_begin_p->x) || (io_begin_p->x > ww))
		{
			return false;
		}

		if (io_begin_p->y < 0.0f)
		{
			io_begin_p->y = 0;
		}
		else if (io_begin_p->y > hh)
		{
			io_begin_p->y = hh;
		}

		if (io_end_p->y < 0.0f)
		{
			io_end_p->y = 0;
		}
		else if (io_end_p->y > hh)
		{
			io_end_p->y = hh;
		}

		return true;
	}

	/// 水平
	if (WIZ_MATH_IS_ZERO_F(io_end_p->y - io_begin_p->y))
	{
		if (WIZ_MATH_LESS_ZERO_F(io_begin_p->y) || (io_begin_p->y > ww))
		{
			return false;
		}

		if (io_begin_p->x < 0.0f)
		{
			io_begin_p->x = 0;
		}
		else if (io_begin_p->x > ww)
		{
			io_begin_p->x = ww;
		}

		if (io_end_p->x < 0.0f)
		{
			io_end_p->x = 0;
		}
		else if (io_end_p->x > ww)
		{
			io_end_p->x = ww;
		}

		return true;
	}

	/// 常规线
	float k = (io_end_p->x - io_begin_p->x) / (float)(io_end_p->y - io_begin_p->y);
	float o_k = 1.0f / k;

	if (io_begin_p->x < 0.0f)
	{
		io_begin_p->y = io_begin_p->y - (int)(io_begin_p->x * o_k);
		io_begin_p->x = 0;
	}
	else if (io_begin_p->x > ww)
	{
		io_begin_p->y = io_begin_p->y + (int)((ww - io_begin_p->x) * o_k);
		io_begin_p->x = ww;
	}

	if (io_begin_p->y < 0.0f)
	{
		io_begin_p->x = io_begin_p->x - (int)(io_begin_p->y * k);
		io_begin_p->y = 0;
	}
	else if (io_begin_p->y > hh)
	{
		io_begin_p->x = io_begin_p->x + (int)((hh - io_begin_p->y) * k);
		io_begin_p->y = hh;
	}

	if (WIZ_MATH_LESS_ZERO_F(io_begin_p->x) || (io_begin_p->x > ww))
	{
		return false;
	}

	if (io_end_p->x < 0.0f)
	{
		io_end_p->y = io_end_p->y - (int)(io_end_p->x * o_k);
		io_end_p->x = 0;
	}
	else if (io_end_p->x > ww)
	{
		io_end_p->y = io_end_p->y + (int)( (ww - io_end_p->x) * o_k );
		io_end_p->x = ww;
	}

	if (io_end_p->y < 0.0f)
	{
		io_end_p->x = io_end_p->x - (int)( io_end_p->y * k );
		io_end_p->y = 0;
	}
	else if (io_end_p->y > hh)
	{
		io_end_p->x = io_end_p->x + (int)( (hh - io_end_p->y) * k );
		io_end_p->y = hh;
	}

	if (WIZ_MATH_LESS_ZERO_F(io_end_p->x) || (io_end_p->x > ww))
	{
		return false;
	}

	return true;
}

void ntScreen::drawLine(ntPointi* _pSrc, ntPointi* _pDest, unsigned int dwColor)
{
	ntPointi a(*_pSrc), b(*_pDest);

	if (! wiz_3d_clip_2d_line(&m_rcCanvasRect, &a, &b))
	{
		return;
	}

	ntPointi* pSrc = &a;
	ntPointi* pDest = &b;

	int dx = pDest->x - pSrc->x;
	int dy = pDest->y - pSrc->y;

	if( dx == 0 && dy == 0 )
	{
		return;
	}

	unsigned int* pDestData= (unsigned int*)getBackbackBuffer();

	if( ntAbs(dx) >= ntAbs(dy) )
	{
		int x_add = (dx > 0) ? 1 : -1;
		for(int x = 0; x != dx; x += x_add )
		{
			int y = x* dy / dx;
			drawPixel(pSrc->x+x, pSrc->y+y, dwColor);
		}
	}
	else
	{
		int y_add = (dy > 0) ? 1 : -1;
		for(int y = 0; y != dy; y += y_add )
		{
			int x = y*dx/dy;
			drawPixel(pSrc->x+x, pSrc->y+y, dwColor);
		}
	}
}

void ntScreen::drawTexture( ntTexture32Ptr spTexture, int ax, int ay)
{
	if (! spTexture)
	{
		return;
	}

	ntPixel32* pSourceBuffer = spTexture->getBuffer();
	ntPixel32* pTargetBuffer= static_cast<ntPixel32*>( getBackbackBuffer() );

	unsigned int uiTexWidth= spTexture->getWidth();
	unsigned int uiTexHeight= spTexture->getHeight();

	for( unsigned int y = 0; y < (unsigned int)m_rcCanvasRect.h && y < uiTexHeight; ++y)
	{
		for( unsigned int x = 0; x < (unsigned int)m_rcCanvasRect.w && x < uiTexWidth; ++x)
		{
			pTargetBuffer[y*m_rcCanvasRect.w+x]= pSourceBuffer[y*uiTexWidth+x];
		}
	}
}

void ntScreen::setViewRect()
{
	RECT rect;
	::GetClientRect(m_hPresentHandle, &rect);
	int uiViewHeight = rect.bottom - rect.top;
	int uiViewWidth = rect.right - rect.left;
	m_rcViewRect.w = uiViewWidth;
	m_rcViewRect.h = uiViewHeight;
}

void ntScreen::drawRect( const ntRecti& a, unsigned int dwColor )
{
	ntPointi leftTop(a.x, a.y);
	ntPointi rightTop(a.x+a.w, a.y);
	ntPointi leftBottom(a.x, a.y+a.h);
	ntPointi rightBottom(a.x+a.w, a.y+a.h);
	drawLine(&leftTop, &leftBottom, dwColor);
	drawLine(&leftTop, &rightTop, dwColor);
	drawLine(&leftBottom, &rightBottom, dwColor);
	drawLine(&rightTop, &rightBottom, dwColor);
}

void ntScreen::drawPixel( unsigned int x, unsigned int y, unsigned int color )
{
	unsigned int p= y * m_rcCanvasRect.w + x;
	if (p < m_uiMaxAddress)
	{
		unsigned int uiDest = ((unsigned int*)m_pBackbufferData)[p];
		unsigned char ucAlpha = (color>>24)&0xFF;
		unsigned char ucInvAlpha = 0xFF-ucAlpha;
		((unsigned int*)m_pBackbufferData)[p]= ntblend3(
			ucAlpha, color, ucInvAlpha, uiDest) | 0xFF000000;
	}
}


bool ntScreen::drawTextureWrap(ntTexture32Ptr spTexture, const ntRecti& aDrawRect)
{
	if (! spTexture)
	{
		return false;
	}

	ntRecti newRect;
	if ( ! m_rcCanvasRect.getIntersection(aDrawRect, newRect) )
	{
		return false;
	}

	/** 多余则切， 不够则补 */
	unsigned int uiTextureWidth= spTexture->getWidth();
	unsigned int uiTextureHeight= spTexture->getHeight();

	/** 纹理源数据 */
	ntPixel32* pSourceBuffer = spTexture->getBuffer();

	/** 目标数据 */
	ntPixel32* pTargetBuffer= static_cast<ntPixel32*>( getBackbackBuffer() );

	/** 由顶向下进行绘制 */
	for(unsigned int y=0; y <(unsigned int)newRect.h; ++y)
	{
		/** 考虑到重复绘制，所以要对y值进行取余 */
		unsigned int newTextureY= y % uiTextureHeight;

		/** 定位源纹理线首 */
		ntPixel32* pSrcLineHead = pSourceBuffer + newTextureY * uiTextureWidth;

		/** 定位目标画布线首 */
		ntPixel32* pDestLineHead = pTargetBuffer 
			+ ( newRect.y + y) * m_rcCanvasRect.w + newRect.x;

		/** 开始横向扫描 */
		unsigned int x=0;
		do
		{
			/** 画布总是递增的 */
			ntPixel32* pDest = pDestLineHead + x;

			/** 计算绘制的宽度 */
			unsigned int uiDrawWidth= uiTextureWidth;

			/** 进行裁切 */
			if ( x + uiTextureWidth >= (unsigned int)newRect.w )
			{
				uiDrawWidth= newRect.w - x;
			}

			memcpy(pDest, pSrcLineHead, sizeof(ntPixel32)*uiDrawWidth);

			/** 步进 */
			x+= uiDrawWidth;

		}while( x < (unsigned int)newRect.w);
	}

	return true;
}

void ntScreen::drawTextureInternal(ntPixel32* pSource, int iCanvasX, 
								   int iCanvasY, int iWidth, int iHeight, 
								   int iTextureX, int iTextureY, int iTextureWidth )
{
	ntPixel32* pTargetBuffer= static_cast<ntPixel32*>( getBackbackBuffer() );
	
	for(int y = 0; y<iHeight; ++y)
	{
		ntPixel32* pDestLine= pTargetBuffer + (y+iCanvasY)*m_rcCanvasRect.w + iCanvasX;
		ntPixel32* pSrcLine = pSource + (y+iTextureY)*iTextureWidth + iTextureX;
		memcpy(pDestLine, pSrcLine, sizeof(ntPixel32)*iWidth);
	}
}

bool ntScreen::drawTextureEx( ntTexture32Ptr spTexture, const ntRecti& aDrawRect, ntBlendMode blendMode)
{
	if (! spTexture)
	{
		return false;
	}

	ntRecti newRect;
	if ( ! m_rcCanvasRect.getIntersection(aDrawRect, newRect) )
	{
		return false;
	}

	unsigned int uiTextureWidth= spTexture->getWidth();
	unsigned int uiTextureHeight= spTexture->getHeight();

	if ((unsigned int)newRect.w > uiTextureWidth )
	{
		newRect.w = uiTextureWidth;
	}

	if ((unsigned int)newRect.h > uiTextureHeight )
	{
		newRect.h = uiTextureHeight;
	}

	/** 纹理源数据 */
	ntPixel32* pSourceBuffer = spTexture->getBuffer();

	/** 目标数据 */
	ntPixel32* pTargetBuffer= static_cast<ntPixel32*>( getBackbackBuffer() );
	

	if ( blendMode == BM_NONE )
	{
		/** 由顶向下进行绘制 */
		for(unsigned int y=0; y <(unsigned int)newRect.h; ++y)
		{
			/** 定位源纹理线首 */
			ntPixel32* pSrcLineHead = pSourceBuffer + uiTextureWidth * y;

			/** 定位目标画布线首 */
			ntPixel32* pDestLineHead = pTargetBuffer 
				+ ( newRect.y + y ) * m_rcCanvasRect.w + newRect.x;

			memcpy(pDestLineHead, pSrcLineHead, newRect.w*sizeof(ntPixel32));
		}
	}
	else if (blendMode == BM_ALPHA)
	{
		/** 由顶向下进行绘制 */
		for(unsigned int y=0; y <(unsigned int)newRect.h; ++y)
		{
			/** 定位源纹理线首 */
			ntPixel32* pSrcLineHead = pSourceBuffer + uiTextureWidth * y;

			/** 定位目标画布线首 */
			ntPixel32* pDestLineHead = pTargetBuffer 
				+ ( newRect.y + y ) * m_rcCanvasRect.w + newRect.x;

			for(unsigned int x=0; x< (unsigned int)newRect.w; ++x)
			{
				(pDestLineHead++)->blend(pSrcLineHead++);
			}
		}
	}

	return true;
}

bool ntScreen::createEx( HWND hWnd )
{
	RECT rect;
	::GetClientRect(hWnd, &rect);
	int h = rect.bottom - rect.top;
	int w = rect.right - rect.left;

	if ( w > 0 && h > 0 )
	{
		setPresentHandle(hWnd);

		create(w, h);

		return true;
	}

	return false;
}

unsigned char ntScreen::getBackgroundColor() const
{
	return m_ucBackgroundColor;
}

void ntScreen::setBackgroundColor( unsigned char val )
{
	m_ucBackgroundColor = val;
}

