// ntPhotoAppDoc.h : interface of the ntPhotoAppDoc class
//


#pragma once

#include "ntTexture.h"

class ntCommandMgr;

class ntPhotoAppDoc : public CDocument
{
protected: // create from serialization only
	ntPhotoAppDoc();
	DECLARE_DYNCREATE(ntPhotoAppDoc)

// Attributes
public:
	ntCommandMgr* getCmdMgr() { return m_pCmdMgr; }

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace= TRUE);
	ntTexture32Ptr getWorkTexture();

// Implementation
public:
	virtual ~ntPhotoAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	ntTexture32Ptr m_workTexture;

	ntCommandMgr* m_pCmdMgr;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMakeGray();
	afx_msg void OnMakeInvert();
};


