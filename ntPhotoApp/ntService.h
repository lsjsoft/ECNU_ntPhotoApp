#pragma once

#define ntGetService(T) ntService::getIns()->getService<T>()

class ntServiceBase;

class ntService
{
	enum { MAX_SERVICE_COUNT= 32, };

public:
	ntService(void);
	~ntService(void);

	static ntService* getIns();
	static void init();
	static void release();

	void update(float fDeltaTime);

	bool add(ntServiceBase* pBase);

	void start();

	void end();

	template<typename T>
	T* getService()
	{
		const char* szName= T::_getServiceName();
		if (szName)
		{
			ntSvrMap::iterator it = m_svrMap.find(szName);
			if (it != m_svrMap.end())
			{
				return static_cast<T*>(it->second);
			}
		}

		return 0;
	}

	void uninitialize();

	void destroy();

protected:
	typedef std::map< std::string, ntServiceBase* > ntSvrMap;
	ntSvrMap m_svrMap;

	typedef std::vector< ntServiceBase* > ntSvrPtrVector;
	ntSvrPtrVector m_svrVector;
};

