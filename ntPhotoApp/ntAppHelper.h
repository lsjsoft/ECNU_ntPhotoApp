#pragma once

class ntMainFrame;
class ntPhotoAppDoc;

inline ntMainFrame* ntGetMainFrame()
{
	return (ntMainFrame*)AfxGetMainWnd();
}

ntPhotoAppDoc* ntGetActiveDoc();

void ntUpdateActiveView();

#define WM_UPDATE_HISTOGRAM		WM_USER + 401


std::string ntGetShortName(const std::string& rkpathName);