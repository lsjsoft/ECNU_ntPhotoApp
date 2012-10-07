#pragma once

class ntOp;
#include "ntMsgHandle.h"
#include "ntOp.h"
#include "ntServiceBase.h"

class ntOpMgr: public ntMsgHandle
{
	ntImplService(ntOpMgr);

public:
	ntOpMgr(void);
	~ntOpMgr(void);

	bool AddOp(ntOp* pOp);

	bool Active(const opIdType& opId);

	virtual void start();
	virtual void end();

	virtual bool OnMouseLBDown(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseLBUp(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseMove(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseRBDown(const ntPointi& p, ntCpsBv bv);
	virtual bool OnMouseRUp(const ntPointi& p, ntCpsBv bv);
	virtual bool OnKeyDown(char key, ntCpsBv bv);
	virtual bool OnKeyUp(char key, ntCpsBv bv);

	template<typename T>
	bool Active()
	{
		return Active( T::_GetOpId());
	}


	HCURSOR getActiveCursor();

	template <typename T>
	T* GetOp()
	{
		return (T*)findOp(T::_GetOpId());
	}

	ntOp* getActiveOp()
	{
		return m_pActive;
	}

protected:
	ntOp* findOp(const opIdType& opId);

	void destroyAll();

	virtual void update(float fDeltaTime) {}

	typedef std::map<opIdType, ntOp*> ntOps;
	ntOps m_Ops;
	ntOp* m_pActive;

	HCURSOR m_hDefault;
};
