// ntPhotoAppDoc.cpp : implementation of the ntPhotoAppDoc class
//

#include "stdafx.h"
#include "ntPhotoApp.h"

#include "ntPhotoAppDoc.h"
#include "ntTextureService.h"
#include "ntService.h"
#include "ntCommandMgr.h"
#include "ntCmdGray.h"
#include "ntCmdInvert.h"
#include "ntCommonPlugCmd.h"
#include "ntNaturalImage.h"
#include "ximage.h"
#include "ntCommonDlg.h"


// ntPhotoAppDoc

IMPLEMENT_DYNCREATE(ntPhotoAppDoc, CDocument)

BEGIN_MESSAGE_MAP(ntPhotoAppDoc, CDocument)
	ON_COMMAND(ID_MAKE_GRAY, &ntPhotoAppDoc::OnMakeGray)
	ON_COMMAND(ID_MAKE_INVERT, &ntPhotoAppDoc::OnMakeInvert)
END_MESSAGE_MAP()


// ntPhotoAppDoc construction/destruction

ntPhotoAppDoc::ntPhotoAppDoc()
{
	// TODO: add one-time construction code here
	m_pCmdMgr = ntNew ntCommandMgr;
}

ntPhotoAppDoc::~ntPhotoAppDoc()
{
	delete m_pCmdMgr;
}

BOOL ntPhotoAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// ntPhotoAppDoc serialization

void ntPhotoAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// ntPhotoAppDoc diagnostics

#ifdef _DEBUG
void ntPhotoAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void ntPhotoAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG


BOOL ntPhotoAppDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	ntTextureService* pSvr= ntGetService(ntTextureService);
	if ( lpszPathName && pSvr)
	{
		m_workTexture = pSvr->create( lpszPathName, true);
		if (m_workTexture)
		{
			return TRUE;
		}
	}

	return FALSE;
}

ntTexture32Ptr ntPhotoAppDoc::getWorkTexture()
{
	return m_workTexture;
}

void ntPhotoAppDoc::OnMakeGray()
{
	if (m_workTexture)
	{
		ntCommonPlugCmd::doCmd(m_pCmdMgr, m_workTexture, "Gray");
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}


void ntPhotoAppDoc::OnMakeInvert()
{
	if (m_workTexture)
	{
		ntCmdInvert* pInvertCmd= ntNew ntCmdInvert(m_pCmdMgr, m_workTexture);
		pInvertCmd->run();
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}

BOOL ntPhotoAppDoc::OnSaveDocument( LPCTSTR lpszPathName )
{
	if (!m_workTexture)
	{
		return FALSE;
	}

	ntNaturalImage img;
	if (! img.save( lpszPathName, m_workTexture->getBuffer(), 
		m_workTexture->getWidth(), m_workTexture->getHeight(),
		CXIMAGE_FORMAT_TGA))
	{
		return FALSE;
	}

	SetModifiedFlag(FALSE);

	return TRUE;
}

CString RemoveExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--)
	{
		if (name[i] == '.')
		{
			return name.Mid(0,i);
		}
	}
	return name;
}

BOOL ntPhotoAppDoc::DoSave( LPCTSTR pszPathName, BOOL bReplace/*= TRUE*/ )
{
	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty()) 
	{
		bSaveAs = TRUE;
	}

	if (bSaveAs)
	{
		newName = m_strPathName;

		newName = RemoveExtension(newName);

		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			newName = RemoveExtension(newName);
			int iBad = newName.FindOneOf(_T("#%;/\\"));
			if (iBad != -1)
			{
				newName = _T("UntitledImage");
			}
		}

		int theApp_nDocType = CXIMAGE_FORMAT_TGA;
		ntCommonDlg dlg;
		if (!dlg.PromptForFileName(newName, 
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &theApp_nDocType))
		{
			return FALSE;
		}
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			TRY 
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	EndWaitCursor();

	if (bReplace) 
	{
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName);
	}
	else
	{
		SetModifiedFlag(bModified);
	}
	return TRUE;
}

void ntPhotoAppDoc::AddTex( const std::string& name, ntTexture32Ptr tex )
{
	m_Texs.insert( std::make_pair(name, tex));
}
