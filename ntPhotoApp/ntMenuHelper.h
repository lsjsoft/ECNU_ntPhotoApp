#pragma once

namespace ntMenuHelper
{
	struct MenuItemData 
	{
		UINT cmdID;
		const char *string;
	};

	// appends item to menu
	void AddSubMenuItem(CMenu *pMenu, UINT cmdID, const char *string);

	void AddSubMenuItems(CMenu* pMenu, MenuItemData* data);

	// inserts item in menu
	void InsertSubMenuItem(CMenu *pMenu, UINT itemIndex, UINT cmdID, 
		const char *string);

	// removes item from menu
	void RemoveSubMenuItem(CMenu *pMenu, UINT cmdID);

	CMenu *AddSubMenu(CMenu *pMenu, UINT mainMenuItem, UINT itemIndex, 
		const char *string, MenuItemData *data = NULL);

	// removes item from menu
	void RemoveSubMenu(CFrameWnd *pWnd, UINT mainMenuItem, UINT itemIndex);

	// emty submenu of all items
	void ClearSubMenu(CFrameWnd *pWnd, UINT mainMenuItem, UINT itemIndex);

};
