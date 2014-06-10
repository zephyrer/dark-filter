#pragma once


class SingletonCnf
	: public boost::serialization::singleton<SingletonCnf>
{
public:
	SingletonCnf(void);
	~SingletonCnf(void);

private:
	std::locale locale_;
	bool LoadXml(boost::property_tree::ptree& tree) const;
	bool LoadXml(boost::property_tree::wptree& tree)const;
	bool SaveXml(boost::property_tree::ptree& tree) const;
	bool SaveXml(boost::property_tree::wptree& tree) const;


public:
	std::wstring GetUrl()const;
	std::wstring GetRegex()const;

	void SaveUrl(const std::wstring& url)const;
	void SaveRegex(const std::wstring& regex)const;

	void SavePlugin(const std::wstring& name)const;
	std::wstring GetPlugin()const;

protected:
	bool proxy_;
	std::wstring address_;
	unsigned short port_;
	std::wstring name_;
	std::wstring password_;
public:
	inline bool GetProxy()const
	{
		return proxy_;
	}
	inline std::wstring GetAddress()const
	{
		return address_;
	}
	inline unsigned short GetPort()const
	{
		return port_;
	}
	inline std::wstring GetName()const
	{
		return name_;
	}
	inline std::wstring GetPassword()const
	{
		return password_;
	}

	inline int GetTimeOut()const
	{
		return timeout_;
	}
	inline int GetMaxPage()const
	{
		return max_page_;
	}
	inline int GetMaxCount()const
	{
		return max_count_;
	}

protected:
	int timeout_;
	int max_page_;
	int max_count_;

public:
	void Save(bool proxy
		,const std::wstring& address
		,unsigned short port
		,const std::wstring& name
		,const std::wstring& password

		,int timeout
		,int max_page
		,int max_count
		);
};

