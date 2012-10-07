#pragma once

enum ntControlPressState 
{
	NCPS_ALT  = 0x01,
	NCPS_CTL  = 0x02,
	NCPS_SHIFT= 0x04,
};

typedef ntControlPressState ntCPS;
typedef unsigned char ntCpsBv;

class ntMsgHandle
{
public:
	virtual bool OnMouseLBDown(const ntPointi& p, ntCpsBv bv) { return false; }
	virtual bool OnMouseLBUp(const ntPointi& p, ntCpsBv bv) { return false; }
	virtual bool OnMouseMove(const ntPointi& p, ntCpsBv bv) { return false; }
	virtual bool OnMouseRBDown(const ntPointi& p, ntCpsBv bv) { return false; }
	virtual bool OnMouseRUp(const ntPointi& p, ntCpsBv bv) { return false; }
	virtual bool OnKeyDown(char key, ntCpsBv bv) { return false; }
	virtual bool OnKeyUp(char key, ntCpsBv bv) { return false; }
};

