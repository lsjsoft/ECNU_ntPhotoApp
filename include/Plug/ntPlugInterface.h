#pragma once

#ifdef _USRDLL
	#define PLUG_API extern "C" __declspec( dllexport )
#else

	#define PLUG_API __declspec( dllimport )
#endif

#pragma pack(push, 1)

struct ntPlugPix
{
	unsigned char r, g, b, a;
};

#pragma pack(pop)

struct ntPlugPixData
{
	ntPlugPixData()
	: m_pPixelData(0)
	, m_uiWidth(0)
	, m_uiHeight(0)
	, m_bRefrerence(true)
	{
	}

	~ntPlugPixData()
	{
		if (!m_bRefrerence)
		{
			delete []m_pPixelData;
		}
	}

	ntPlugPix* m_pPixelData;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	bool m_bRefrerence;

	bool copyFrom(const ntPlugPixData* src)
	{
		if (src && src->m_pPixelData && m_pPixelData)
		{
			if ( src->m_uiWidth == m_uiWidth && 
				src->m_uiHeight == m_uiHeight)
			{
				memcpy(m_pPixelData, src->m_pPixelData, 
					sizeof(ntPlugPix)*m_uiWidth*m_uiHeight);

				return true;
			}
		}

		return false;
	}

	ntPlugPixData* clone()
	{
		if (m_pPixelData && m_uiHeight && m_uiHeight)
		{
			ntPlugPixData* pNewPd = new ntPlugPixData();
			pNewPd->m_uiWidth = m_uiWidth;
			pNewPd->m_uiHeight = m_uiHeight;
			pNewPd->m_bRefrerence = false;
			pNewPd->m_pPixelData = new ntPlugPix[ m_uiHeight * m_uiWidth ];
			memcpy( pNewPd->m_pPixelData, m_pPixelData, sizeof(ntPlugPix)*m_uiWidth*m_uiHeight );
			return pNewPd;
		}

		return NULL;
	}
};

struct ntPlugInfo
{
	char guid[64];
	char title[64];
	char desc[128];
	unsigned int uiVersion;
};

typedef bool (*pntPlugGetInfo)(ntPlugInfo* pInfo);

typedef bool (*pntPlugAllocPixData)(ntPlugPixData** pData,
									unsigned int uiWidth,
									unsigned int uiHeight);

typedef void (*pntPlugPreview)();


struct ntPlugCallParam
{
	ntPlugCallParam()
	: hWnd(0)
	, pSource(0)
	, pOutput(0)
	, pAllFunc(0)
	, pPreviewFunc(0)
	{
	}

	HWND hWnd;
	ntPlugPixData* pSource;
	ntPlugPixData* pSourceBackup;
	ntPlugPixData** pOutput;
	pntPlugAllocPixData pAllFunc;
	pntPlugPreview pPreviewFunc;

	bool checkValud()
	{
		if ( hWnd && pSource && pAllFunc )
		{
			return true;
		}

		return false;
	}

	void restoreSource()
	{
		pSource->copyFrom( pSourceBackup );
	}

	void draw()
	{
		pPreviewFunc();
	}
};

__forceinline bool drawNature(ntPlugPix* adst, unsigned int dstW, unsigned int dstH,
				ntPlugPix* asrc, unsigned int srcW, unsigned int srcH,
				unsigned int drawX, unsigned int drawY)
{
	if ( drawX >= dstW || drawY >= dstH )
	{
		return false;
	}

	unsigned int drawW = drawX+srcW> dstW ? dstW-drawX: dstW;
	unsigned int drawH = drawY+srcH> dstH ? dstH-drawY: dstH;
	ntPlugPix* pSrc= asrc;
	ntPlugPix* pDest= adst + drawY*dstW + drawX;
	unsigned int copyLineSize= sizeof(ntPlugPix)*drawW;

	for(unsigned int y=0; y<drawH; ++y)
	{
		memcpy(pDest, pSrc, copyLineSize);
		pDest+= dstW;
		pSrc+= srcW;
	}

	return true;
}

__forceinline bool drawNatureA8(ntPlugPix* adst, unsigned int dstW, unsigned int dstH,
				unsigned char* asrc, unsigned int srcW, unsigned int srcH,
				unsigned int drawX, unsigned int drawY, unsigned int color)
{
	if ( drawX >= dstW || drawY >= dstH )
	{
		return false;
	}

	unsigned int drawW = drawX+srcW> dstW ? dstW-drawX: srcW;
	unsigned int drawH = drawY+srcH> dstH ? dstH-drawY: srcH;

	for(unsigned int y=0; y<drawH; ++y)
	{
		unsigned char* pSrc=asrc + y * srcW;
		ntPlugPix* pDest= adst + (y+drawY)*dstW + drawX;

		for(unsigned int x=0; x<drawW; ++x)
		{
			pDest->a = *pSrc;
			pDest->r = pDest->g = pDest->b = color;
			++pDest;
			++pSrc;
		}
	}

	return true;
}


typedef bool (*pntPlugExcute)(ntPlugCallParam* pParam);

#define cntPlugGetInfo "plugGetInfo"

#define cntPlugExcute "plugExcute"

#define cntPlugImplInfo(guid, cap, desc, ver) \
static const ntPlugInfo g_PlugInfo= \
{ \
	guid, \
	cap, \
	desc, \
	ver, \
}; \
PLUG_API bool plugGetInfo(ntPlugInfo* pInfo) \
{ \
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
	if (!pInfo) \
	{ \
		return false; \
	} \
	memcpy(pInfo, &g_PlugInfo, sizeof(ntPlugInfo)); \
	return true; \
}


/*

{F7047433-E9D3-DDC0-3831-7ECD03E8FC71}
{2FE74142-8DFD-42AE-37FF-1B3EBDF7032F}
{2B6998D8-81A5-9352-5E13-F6F827735824}
{2CA43756-EFE0-6525-AABF-47939DC53424}
{5AE039BA-23C2-82A0-0F5F-F73296B506D9}
{D3BBEDB1-7E6C-D187-F106-FACAB99BBD6F}
{D8C0FF6F-A36E-719B-F64D-A2CBD89CA0D1}
{CC76B790-486A-81F7-7778-5EDEE882ECC8}
{1003BD17-55A7-F5B5-929E-DD01CCA96BBF}
{36ACBEC0-CE9E-49AE-7418-4419C08AECAE}
{D6DC4EC2-A797-C1A3-DECB-754B9BF122F3}
{BCB84F2B-F2C1-C65D-E92A-646285B5B448}
{81806DA4-F16C-851C-C163-20A3CEDC78EE}
{0991B148-716B-1B12-7776-C54325F2F166}
{D43ABB4F-FC82-C46B-1131-F81BE7B5A773}
{E2A41238-B497-9DC4-1155-AF99FFE66CD3}
{E052B059-9EAC-D1E4-2FA1-C9D933396436}
{31FEA90B-B9F5-53AC-4A2F-37DB4938BC92}
{2C841FD7-52D0-6E9E-B6A8-848784F3B795}
{F8C5EDC4-69E1-9F90-26C7-6E4C2CB336AC}
{C53132C0-B7E9-5F87-D223-9D0D9024EC36}
{65BDA041-F0EE-5FA5-3F67-B1B10E99643A}
{C234046E-D712-AB35-3C37-47C0408946ED}
{A8800FF9-635C-2A04-3512-7316A7D350FB}
{52EA3918-AEC6-1256-EB79-D76C276FD89E}
{90415A2F-B1F8-415B-BC93-2A55B446D943}
{A7353BE6-98ED-07F9-C215-FE939EF75FE3}
{B96DBED1-6925-1565-3B97-FE46963C4BF5}
{A4B8E336-2629-25BC-3381-C6A30E80EE83}
{9F75479E-2EE2-3E38-3A26-981486405A82}
{56B7F8AA-4B85-73E3-4801-1677982DE605}
{072E8B36-D064-F505-253E-3996528AA9E6}
{9D7B8FFE-BEFD-668F-6EC6-1F04B4073363}
{EA2B2B28-5A3A-EE53-408D-6383FF52C5F8}
{522A1E86-D7DC-BDA4-017D-BDCCB75577C0}
{27CFC9AF-5143-4BF6-02BD-C5807293BAB2}
{CD3CB81B-69B2-4BEF-B67F-D09D21646788}
{E5B55214-99F7-9858-07FD-745D278D0257}
{A3ED34C3-1CE8-1340-3D2D-670B5A7824D1}
{154389F8-29D6-A81C-BAE4-BB4219CF1D44}
{1DA871C2-FE55-E5DC-02AD-8C34482820B8}
{E13B44DC-8B66-E463-D69F-C73B680A2959}
{D620950A-245B-1B31-B92B-AFBA1AD1F7F8}
{8789A225-82AE-C0B1-3720-8DC9D2007C9F}
{F292EDC4-0EEE-F2A5-0986-D3E60F6D0704}
{E0D7BB33-316C-E84A-D7D1-F695E46AC47B}
{039F67A6-FF12-6020-7D1E-AD324AB11ED4}
{EE8EC705-0707-C04C-5674-31225FB34A73}
{18F2949C-1A12-7981-2D16-9EC5FEFF234A}
{AF38BD6F-40ED-4FCD-5B06-809D74CCA750}
{005A944F-53A8-A53F-D0E2-2FDF7273C889}
{A2341CB4-F96E-28FF-1C1C-B1BFEC0C47F6}
{980B60EC-BE99-C515-D168-A75E4F08663F}
{0CE922E4-C58D-8CB3-CF98-024D8C16FC89}
{6387576E-2537-9D16-ACCD-83912086248C}
{5CE526BE-D94A-25D7-ECAB-99E11E08CA1E}
{A0FB0D14-9BA1-5E5C-FAB9-08CE7695F213}
{BEAA9928-34B8-AF84-0B1C-18B415C6B401}
{848AB38F-8CAE-7DB5-6296-5AFB3F25378D}
{62E6CB40-6635-E8D9-36F7-189AD04D47AA}
{715DF29F-D817-D736-72F7-249C66DB26F8}
{AF7B0B7D-DD13-CF7D-917A-D8F16107C2FE}
{208C9B20-E8D3-9972-180B-1E6B2868D6AC}
{B5D8A2B7-F75C-D517-8000-B71F9F71AF98}
{7512CDDD-9A8B-FEF0-F36C-8177D2F77B76}
{8E6234CF-CED7-3D57-86CB-CD55A21913BC}
{E3746AED-79EF-CB9C-DDF5-D328628314C1}
{6DC09B8B-8C83-087A-92D0-0EE545091256}
{CDFE7CFB-62CA-B6CB-5552-16E2183456B4}
{8CF26540-CF06-D6E6-B0E0-4032B7319279}
{FDA15F61-9491-8DC7-5D4D-B4B974F6CB29}
{2B64E8A1-8957-EBD2-3467-30DD24BB8E19}
{9817A956-8A50-7165-75C4-938116302EDE}
{5A4F5F3E-21AB-5B33-11A6-68448217A82F}
{1FB4B307-4F7F-BB4A-60C4-1282AF52D4EA}
{2740BD1A-55F4-9195-9781-FB2BAC673EFE}
{0366935E-57B9-1BF9-FC86-6E2035782C44}
{459B0564-27DB-7F43-3F47-7529952DA1B8}
{C2C759A7-C2AF-D02C-0E0C-0A7514093158}
{0E1824DF-C395-5F85-6A54-0564D2B32D95}
{B5B1228C-0BDF-9DD6-BD3E-1598EAC0D1FF}
{4F295819-3E93-BD21-E50A-869DABBF8E39}
{8C5960B0-1273-4B18-40A4-2E7A3CE0C0ED}
{A8DD7FA0-D492-4938-A0E3-5BBEC1421D4C}
{F89CD11F-2588-E376-2256-7B68404AA0C3}
{3797D6A9-60D8-7201-663B-8E14A4DCDB99}
{A6E18959-0F07-936B-B383-A98653883BF4}
{4535E07E-E9F7-DBB8-5EE3-B0BEFD131D92}
{58470FD5-4F03-77B8-52C6-99A65C0F193A}
{A124CD62-F026-A873-81B5-D203BA749E5D}
{AA165C2E-577B-B13C-99AB-8215155A315F}
{F043C877-0E03-2680-D309-31641EFCC748}
{22525609-76E9-6197-1BC6-FC7B926FD5EC}
{A3DF8873-581D-FAD0-DBDA-9716E119A399}

*/
