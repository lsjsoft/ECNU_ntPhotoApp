#pragma once

#define ntGetCount(a) ( sizeof(a) / sizeof(a[0]) )

#define ntLoadService(ntName) { ntName* pNtObj= ntNew ntName(); ntService::getIns()->add(pNtObj); }

#define ntImplService(ntName) public: \
	static const char* _getServiceName() { return #ntName; } \
	virtual const char* getServiceName() { return #ntName; }

#define ntServiceShutdown() ntService::release()


#define ntLogFunction() { printf("%s\n", __FUNCTION__); }

template<class T> inline T ntAbs( T a )	
{ 
	return ( (a>=(T)0) ? a : -a );
}

template<class T> inline T ntMin( T a, T b )	
{ 
	return ( (a<=b) ? a : b );
}

template<class T> inline T ntMax( T a, T b )
{
	return ( (a>=b) ? a : b ); 
}

enum ntBlendMode
{
	BM_NONE = 0,	/** 不混合，覆盖 */
	BM_ALPHA,		/** 进行Alpha混合 */
	BM_ADD,			/** 进行加法运算 */
};

#define ntInvalidId (-1)

#define ntm24(c) ((c>>16)&0xFF)

#define ntm16(c) ((c>>8)&0xFF)

#define ntm8(c) (c&0xFF)

#define ntblend24(a, src, ia, dest) ( (a*ntm24(src)+ia*ntm24(dest))>>8 )

#define ntblend16(a, src, ia, dest) ( (a*ntm16(src)+ia*ntm16(dest))>>8 )

#define mtblend8(a, src, ia, dest) ( (a*ntm8(src)+ia*ntm8(dest))>>8 )

#define ntblend3(a, src, ia, dest) ( ntblend24(a,src,ia,dest)<<16 \
	| ntblend16(a,src,ia,dest)<<8 | mtblend8(a,src,ia,dest) )

#define ntMemset(obj) { memset(&obj, 0, sizeof(obj)); }

enum ntColorEnum
{
	ntColorWhite = 0xFFFFFFFF,
	ntColorBlack = 0xFF000000,
	ntColorRed   = 0xFFFF0000,
	ntColorGreen = 0xFF00FF00,
	ntColorBlue  = 0xFF0000FF,
};

inline void ntPrint(const char* szLogFormat, ...)
{
	va_list vl;   
	char buffer[2048];
	va_start(vl, szLogFormat);
	vsprintf_s(buffer, 2048, szLogFormat, vl);
	va_end(vl);
	printf(buffer);
	printf("\n");
}

#define ntLog ntPrint

#define ntMakeLower(str) std::transform(str.begin(), str.end(), str.begin(), std::tolower)

#define WM_PLUG_MENU_BASE_ID	(WM_USER+201)
#define WM_PLUG_MENU_MAX_ID		(WM_PLUG_MENU_BASE_ID+512)
#define IsPlugMenuId(Id)		(Id>=WM_PLUG_MENU_BASE_ID && Id<WM_PLUG_MENU_MAX_ID)