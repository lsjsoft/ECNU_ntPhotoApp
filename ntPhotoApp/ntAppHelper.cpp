#include "StdAfx.h"
#include "ntAppHelper.h"
#include "ntPhotoAppView.h"

std::string ntGetShortName(const std::string& rkpathName)
{
	size_t p = rkpathName.rfind('\\');
	if ( p == std::string::npos)
	{
		return rkpathName;
	}

	return rkpathName.substr(p+1);
}

void ntUpdateActiveView()
{
	CMDIChildWnd * pChild =
		((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if ( !pChild )
	{
		return;
	}

	ntPhotoAppView * pView = (ntPhotoAppView*)pChild->GetActiveView();
	if ( ! pView)
	{
		return;
	}

	pView->DoRefresh();
}


ntPhotoAppDoc* ntGetActiveDoc()
{
	CMDIChildWnd * pChild =
		((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

	if ( !pChild )
	{
		return NULL;
	}

	CDocument * pDoc = pChild->GetActiveDocument();
	if ( ! pDoc)
	{
		return NULL;
	}

	return (ntPhotoAppDoc*)(pDoc);
}
