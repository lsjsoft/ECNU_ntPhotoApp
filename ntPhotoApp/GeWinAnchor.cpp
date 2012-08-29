#include "Stdafx.h"
#include "GeWinAnchor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL ntWinAnchor::OnSize(BOOL bRepaint) 
{
	if (!m_hWnd)
		return FALSE;

	RECT pRect;
	if (GetClientRect(m_hWnd, &pRect)) 
	{
		ntAnchorItem *item = m_pControls.next;
		while (item) 
		{
			// horizontal
			if ((item->uFlag & ANCHOR_LEFT) && (item->uFlag & ANCHOR_RIGHT))
			{
				item->pRect.right += pRect.right - m_pRect.right;
			}
			else if (item->uFlag & ANCHOR_LEFT) 
			{
				// left is default
			}
			else if (item->uFlag & ANCHOR_RIGHT)
			{
				item->pRect.right += pRect.right - m_pRect.right;
				item->pRect.left += pRect.right - m_pRect.right;
			}
			else 
			{
				// relative move
				LONG sx = ((pRect.right-pRect.left)/2)-((m_pRect.right-m_pRect.left)/2);
				item->pRect.right += sx;
				item->pRect.left += sx;
			}
			// vertical
			if ((item->uFlag & ANCHOR_TOP) && (item->uFlag & ANCHOR_BOTTOM)) 
			{
				item->pRect.bottom += pRect.bottom - m_pRect.bottom;
			}
			else if (item->uFlag & ANCHOR_TOP) 
			{
				// top is default
			}
			else if (item->uFlag & ANCHOR_BOTTOM) 
			{
				item->pRect.bottom += pRect.bottom - m_pRect.bottom;
				item->pRect.top += pRect.bottom - m_pRect.bottom;
			}
			else
			{
				// relative move
				LONG sy = ((pRect.bottom-pRect.top)/2)-((m_pRect.bottom-m_pRect.top)/2);
				item->pRect.bottom += sy;
				item->pRect.top += sy;
			}
			
			if (item->uFlag & DOCK_TOP) item->pRect.top = pRect.top;
			if (item->uFlag & DOCK_LEFT) item->pRect.left = pRect.left;
			if (item->uFlag & DOCK_RIGHT) item->pRect.right = pRect.right;
			if (item->uFlag & DOCK_BOTTOM) item->pRect.bottom = pRect.bottom;
			MoveWindow(item->hWnd, item->pRect.left, item->pRect.top, 
				item->pRect.right-item->pRect.left, item->pRect.bottom-item->pRect.top, bRepaint);
			item = item->next;
		}
		memcpy(&m_pRect, &pRect, sizeof(pRect));
		return TRUE;
	}
	return FALSE;
}


BOOL ntWinAnchor::Update(HWND hWnd) 
{
	ntAnchorItem *item = m_pControls.next;
	while (item)
	{
		if (item->hWnd==hWnd) 
		{
			// update position
			if (GetWindowRect(hWnd, &item->pRect))
			{
				ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[0]);
				ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[1]);
				return TRUE;
			}
		}
		item = item->next;
	}
	return FALSE;
}


void ntWinAnchor::UpdateAll() 
{
	ntAnchorItem *item = m_pControls.next;
	while (item) 
	{
		if (GetWindowRect(item->hWnd, &item->pRect)) 
		{
			ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[0]);
			ScreenToClient(m_hWnd, &((LPPOINT)&item->pRect)[1]);
		}
		item = item->next;
	}
}