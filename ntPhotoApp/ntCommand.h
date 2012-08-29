#pragma once

class ntCommandMgr;

class ntCommand
{
public:
	ntCommand(ntCommandMgr*);
	virtual ~ntCommand(void);

	bool run();

	virtual bool doCmd() { return false; }
	virtual void redo() {}
	virtual void undo() {}

	void setPrevCmd(ntCommand* pCmd) { m_pPrevCmd = pCmd; }

	void setNextCmd(ntCommand* pCmd) { m_pNextCmd = pCmd; }

	ntCommand* getPrevCmd() const { return m_pPrevCmd; }

	ntCommand* getNextCmd() const { return m_pNextCmd; }

protected:
	ntCommand* m_pNextCmd;
	ntCommand* m_pPrevCmd;
	ntCommandMgr* m_pMgr;
};
