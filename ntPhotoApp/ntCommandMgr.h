#pragma once

class ntCommand;

class ntCommandMgr
{
public:
	ntCommandMgr(void);
	~ntCommandMgr(void);

	void redo();

	void undo();

	void pushCmd(ntCommand* pCmd);

	bool canRedo();

	bool canUndo();

protected:
	void destroyList(ntCommand* pCmd);

	void destroyAll();

protected:
	ntCommand* m_pRootCmd;
	ntCommand* m_pCurrentCmd;
};
