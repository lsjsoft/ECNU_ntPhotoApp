#include "StdAfx.h"
#include "ntPlugExtendService.h"
#include "ntAppHelper.h"
#include "ntMainFrm.h"
#include "ntPhotoAppView.h"
#include "ntMenuHelper.h"

bool ntAllocPixData(ntPlugPixData** pData, unsigned int uiWidth, 
					unsigned int uiHeight)
{
	(*pData) = ntNew ntPlugPixData();
	unsigned int uiSize= sizeof(ntPlugPix)* uiWidth * uiHeight;
	(*pData)->m_pPixelData = (ntPlugPix*)malloc(uiSize);
	(*pData)->m_bRefrerence = false;
	(*pData)->m_uiWidth = uiWidth;
	(*pData)->m_uiHeight = uiHeight;
	return true;
}

ntPhotoAppView* g_pAppView = NULL;

void ntPreviewPlugData()
{
	if (g_pAppView)
	{
		g_pAppView->DoRefresh();
	}
}

void AxFixPath(std::string& rkPath)
{
	if (!rkPath.empty())
	{
		if ( *rkPath.rbegin() != '\\' )
		{
			rkPath += "\\";
		}
	}
}


unsigned int AxFindFileList(std::vector<std::string>& Output,
							const std::string& rkSearchPath,
							const std::string& rkFilter)
{
	unsigned int uiCount= 0;

	if (rkSearchPath.empty())
	{
		return uiCount;
	}

	std::string strSearchPath= rkSearchPath;
	AxFixPath(strSearchPath);

	WIN32_FIND_DATA FindData;
	memset(&FindData, 0, sizeof(FindData));

	const std::string strPattern= strSearchPath + rkFilter;
	HANDLE hFile = FindFirstFile(strPattern.c_str(), &FindData);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		do 
		{
			if (strcmp(FindData.cFileName, ".") 
				&& strcmp(FindData.cFileName, ".."))
			{
				const std::string Filename= strSearchPath
					+ FindData.cFileName;

				Output.push_back(Filename);

				++uiCount;
			}
		}while(FindNextFile(hFile, &FindData));
	}
	FindClose(hFile);

	return uiCount;
}

ntPlugExtendService::ntPlugExtendService(void)
{
}

ntPlugExtendService::~ntPlugExtendService(void)
{
}

void ntPlugExtendService::start()
{
	std::vector< std::string> DllList;
	AxFindFileList( DllList, "plugs\\", "*.dll");

	for each ( const std::string& rkFilename in DllList)
	{
		loadDll( rkFilename.c_str());
	}
}

void ntPlugExtendService::loadDll( const std::string& rkPathName )
{
	HMODULE hDll= LoadLibrary(rkPathName.c_str());
	if (!hDll)
	{
		return;
	}

 	pntPlugGetInfo pInfo= (pntPlugGetInfo)GetProcAddress(hDll, cntPlugGetInfo);
 	pntPlugExcute pExcute= (pntPlugExcute)GetProcAddress(hDll, cntPlugExcute);
 
 	if (pInfo == NULL || pExcute == NULL)
 	{
 		FreeLibrary(hDll);
 		return;
 	}
 
	ntPlugInfo plugInfos = {};
 	if (!pInfo(&plugInfos))
 	{
 		FreeLibrary(hDll);
 		return;
 	}
 
 	DllInfo DInfo;
 	DInfo.m_pExcute = pExcute;
 	DInfo.m_Handle = hDll;
 	m_FuncMaps.insert(std::make_pair(plugInfos.guid, DInfo));

	m_vPlugs.push_back(plugInfos);
}

void ntPlugExtendService::end()
{
	DllMap::iterator it = m_FuncMaps.begin();

	while( it!= m_FuncMaps.end())
	{
		HMODULE hDllHandle= it->second.m_Handle;
		BOOL bFreeSucc= FreeLibrary(hDllHandle);
		++it;
	}

	m_FuncMaps.clear();
}

bool ntPlugExtendService::callPlug(const char* szGuid, 
							ntPlugPixData* pSource,
							ntPlugPixData** pOutput)
{
	if (!szGuid)
	{
		return false;
	}

	pntPlugExcute p = findPlug(szGuid);
	if (! p)
	{
		return false;
	}

	ntPlugCallParam param;
	param.hWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	param.pAllFunc = ntAllocPixData;
	param.pPreviewFunc = ntPreviewPlugData;
	param.pSourceBackup = pSource->clone();
	param.pSource = pSource;
	param.pOutput = pOutput;

	g_pAppView= NULL;
	CFrameWnd* pChildFrame= ntGetMainFrame()->GetActiveFrame();
	if (pChildFrame)
	{
		g_pAppView= static_cast<ntPhotoAppView*>(
			pChildFrame->GetActiveView());
	}


	bool bSucc= p(&param);

	if (!bSucc)
	{
		param.pSource->copyFrom( param.pSourceBackup );
	}

	delete param.pSourceBackup;
	param.pSourceBackup = NULL;

	return bSucc;
}

pntPlugExcute ntPlugExtendService::findPlug(const std::string& rkGuid)
{
	DllMap::iterator it = m_FuncMaps.find( rkGuid );
	if (it == m_FuncMaps.end())
	{
		return NULL;
	}

	return it->second.m_pExcute;
}

void ntPlugExtendService::appendToMenu(CMenu* pRootMenu)
{
	unsigned int uiBase= WM_PLUG_MENU_BASE_ID;
	for each (const ntPlugInfo& info in m_vPlugs)
	{
		ntMenuHelper::AddSubMenuItem( pRootMenu, uiBase++, info.title);
	}	
}

const ntPlugInfo* ntPlugExtendService::getPlug( unsigned int uiId )
{
	uiId-= WM_PLUG_MENU_BASE_ID;

	if ( uiId >= m_vPlugs.size() )
	{
		NULL;
	}

	return &m_vPlugs[uiId];
}
