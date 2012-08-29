// GeWinAnchor.h: interface for the CGeWinAnchor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEWINANCHOR_H__51E6B4CB_A82C_49D5_A535_90DA15F4B5D0__INCLUDED_)
#define AFX_GEWINANCHOR_H__51E6B4CB_A82C_49D5_A535_90DA15F4B5D0__INCLUDED_

#define ANCHOR_TOP			0x0001
#define ANCHOR_LEFT 		0x0002
#define ANCHOR_RIGHT		0x0004
#define ANCHOR_BOTTOM		0x0008
#define ANCHOR_TOPLEFT		0x0003
#define ANCHOR_TOPRIGHT 	0x0005
#define ANCHOR_BOTTOMLEFT	0x000a
#define ANCHOR_BOTTOMRIGHT	0x000c
#define ANCHOR_ALL			0x000f

#define DOCK_TOP			0x0100
#define DOCK_LEFT			0x0200
#define DOCK_RIGHT			0x0400
#define DOCK_BOTTOM 		0x0800
#define DOCK_FILL			0x0f00

#define WM_UPDATE_FORM	WM_USER+ 201

struct ntAnchorItem 
{
	HWND hWnd;	  // hWnd of element
	UINT uFlag;   // anchor
	RECT pRect;   // window rect
	struct ntAnchorItem *next; // next item
	
	ntAnchorItem() 
	{
		this->hWnd	= NULL;
		this->next	= NULL;
		this->uFlag = 0;
		ZeroMemory(&pRect, sizeof(pRect));
	}
	
	ntAnchorItem(HWND hWnd, UINT uFlag, LPRECT lpRect)
	{
		this->hWnd	= hWnd;
		this->next	= NULL;
		this->uFlag = uFlag;
		memcpy(&pRect, lpRect, sizeof(pRect));
	}
};

class ntWinAnchor
{
public:
	// constructor
	ntWinAnchor()
	{
		m_hWnd = NULL;
		ZeroMemory(&m_pRect, sizeof(m_pRect));
	}
	
	// initialize
	BOOL Init(HWND hDlgWindow) 
	{
		if (IsWindow(hDlgWindow))
		{
			m_hWnd = hDlgWindow;
			if (GetClientRect(m_hWnd, &m_pRect)) 
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	
	BOOL Add(HWND hWnd, UINT uFlag) 
	{
		// go to end of list
		ntAnchorItem *item = &m_pControls;
		while (item->next) 
		{
			item = item->next;
		}
		
		// insert new item to the end
		RECT pRect;
		if (GetWindowRect(hWnd, &pRect)) 
		{
			ScreenToClient(m_hWnd, &((LPPOINT)&pRect)[0]);
			ScreenToClient(m_hWnd, &((LPPOINT)&pRect)[1]);
			item->next = ntNew ntAnchorItem(hWnd, uFlag, &pRect);
		}
		return item->next ? TRUE : FALSE;
	}
	
	BOOL Add(UINT uID, UINT uFlag)
	{
		return Add(GetDlgItem(m_hWnd, uID), uFlag);
	}
	
	BOOL Remove(UINT uID) 
	{
		return Remove(GetDlgItem(m_hWnd, uID));
	}
	
	BOOL Remove(HWND hWnd) 
	{
		ntAnchorItem *item = &m_pControls;
		while (item->next) {
			if (item->next->hWnd==hWnd)
			{
				// remove it
				ntAnchorItem *s = item->next->next;
				delete item->next;
				item->next = s;
				return TRUE;
			}
			item = item->next;
		}
		return FALSE;
	}
	
	BOOL Update(UINT uID) 
	{
		return Update(GetDlgItem(m_hWnd, uID));
	}
	
	BOOL Update(HWND hWnd) ;
	
	void UpdateAll() ;
	
	void RemoveAll()
	{
		RemoveIt(m_pControls.next);
	}
	
	~ntWinAnchor() 
	{
		RemoveAll();
	}
	
	BOOL OnSize(BOOL bRepaint=TRUE);
	
protected:
	void RemoveIt(ntAnchorItem* item) 
	{
		if (item) 
		{
			RemoveIt(item->next);
			delete item;
		}
	}
	
protected:
	HWND m_hWnd;
	RECT m_pRect;
	ntAnchorItem m_pControls;
};

#endif // !defined(AFX_GEWINANCHOR_H__51E6B4CB_A82C_49D5_A535_90DA15F4B5D0__INCLUDED_)
