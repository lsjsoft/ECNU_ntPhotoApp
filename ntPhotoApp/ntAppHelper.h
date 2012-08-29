#pragma once

class ntMainFrame;

inline ntMainFrame* ntGetMainFrame()
{
	return (ntMainFrame*)AfxGetMainWnd();
}

#define WM_UPDATE_HISTOGRAM		WM_USER + 401


std::string ntGetShortName(const std::string& rkpathName);