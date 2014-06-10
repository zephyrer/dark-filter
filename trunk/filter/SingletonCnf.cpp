#include "StdAfx.h"
#include "SingletonCnf.h"


SingletonCnf::SingletonCnf(void)
{
	locale_	=	std::locale("chs");
	timeout_	=	DARK_DEFAULT_TIMEOUT;
	max_page_	=	DARK_DEFAULT_MAX_PAGE;
	max_count_	=	DARK_DEFAULT_MAX_COUNT;


	proxy_		=	DARK_DEFAULT_PROXY;
	address_	=	DARK_DEFAULT_ADDRESS;
	port_		=	DARK_DEFAULT_PORT;
	name_		=	DARK_DEFAULT_NAME;
	password_	=	DARK_DEFAULT_PASSWORD;
	
	boost::property_tree::wptree tree;
	if(LoadXml(tree))
	{
		
		std::wstring rs	=	tree.get(L"cnf.proxy.proxy",DARK_DEFAULT_PROXY);
		if(rs.empty()
			|| rs == L"false"
			|| rs == L"0"
			)
		{
			proxy_	=	false;
		}

		address_	=	tree.get(L"cnf.proxy.address",DARK_DEFAULT_ADDRESS);
		port_		=	tree.get(L"cnf.proxy.port",DARK_DEFAULT_PORT);
		name_		=	tree.get(L"cnf.proxy.name",DARK_DEFAULT_NAME);
		password_	=	tree.get(L"cnf.proxy.password",DARK_DEFAULT_PASSWORD);
	

		timeout_	=	tree.get(L"cnf.base.timeout",DARK_DEFAULT_TIMEOUT);
		max_page_	=	tree.get(L"cnf.base.max_page",DARK_DEFAULT_MAX_PAGE);
		max_count_	=	tree.get(L"cnf.base.max_count",DARK_DEFAULT_MAX_COUNT);
	}

	if(timeout_ <= 0)
	{
		timeout_	=	DARK_DEFAULT_TIMEOUT;
	}
	if(max_page_ <= 0)
	{
		max_page_	=	DARK_DEFAULT_TIMEOUT;
	}
	if(max_count_ <= 0)
	{
		max_count_	=	DARK_DEFAULT_TIMEOUT;
	}
}


SingletonCnf::~SingletonCnf(void)
{
}



bool SingletonCnf::LoadXml(boost::property_tree::wptree& tree)const
{
	std::wifstream in(CNF_PATH,std::ios::in);
	if(in.is_open())
	{
		in.imbue(locale_);	
		try
		{
			boost::property_tree::read_xml(in,tree);		
		}
		catch(const boost::property_tree::xml_parser::xml_parser_error& )
		{
			in.close();
			return false;
		}
		in.close();
		return true;
	}
	return false;
}

bool SingletonCnf::LoadXml(boost::property_tree::ptree& tree) const
{
	std::ifstream in(CNF_PATH,std::ios::in);
	if(in.is_open())
	{
		in.imbue(locale_);	
		try
		{
			boost::property_tree::read_xml(in,tree);		
		}
		catch(const boost::property_tree::xml_parser::xml_parser_error& )
		{
			in.close();
			return false;
		}
		in.close();
		return true;
	}
	return false;
}

bool SingletonCnf::SaveXml(boost::property_tree::ptree& tree) const
{
	std::ofstream out(CNF_PATH,std::ios::out);
	if(out.is_open())
	{
		out.imbue(locale_);
		boost::property_tree::write_xml(out,tree);
		out.close();
		return true;
	}
	return false;
}
bool SingletonCnf::SaveXml(boost::property_tree::wptree& tree) const
{
	std::wofstream out(CNF_PATH,std::ios::out);
	if(out.is_open())
	{
		out.imbue(locale_);
		boost::property_tree::write_xml(out,tree);
		out.close();
		return true;
	}
	return false;
}

void SingletonCnf::SavePlugin(const std::wstring& name)const
{
	boost::property_tree::wptree tree;
	LoadXml(tree);
	tree.put(L"cnf.base.plugin",name);

	SaveXml(tree);

	
}

std::wstring SingletonCnf::GetPlugin()const
{
	std::wstring rs;
	
	boost::property_tree::wptree tree;
	if(LoadXml(tree))
	{
		rs	=	tree.get(L"cnf.base.plugin",L"");
	}

	return rs;
}

std::wstring SingletonCnf::GetUrl()const
{
	std::wstring rs	=	DARK_DEFAULT_URL;
	
	boost::property_tree::wptree tree;
	if(LoadXml(tree))
	{
		rs	=	tree.get(L"cnf.base.url",DARK_DEFAULT_URL);
	}

	return rs;
}
std::wstring SingletonCnf::GetRegex()const
{
	std::wstring rs	=	DARK_DEFAULT_REGEX;
	
	boost::property_tree::wptree tree;
	if(LoadXml(tree))
	{
		rs	=	tree.get(L"cnf.base.regex",DARK_DEFAULT_REGEX);
	}

	return rs;
}
void SingletonCnf::SaveUrl(const std::wstring& url)const
{
	boost::property_tree::wptree tree;
	LoadXml(tree);
	tree.put(L"cnf.base.url",url);

	SaveXml(tree);
}
	
void SingletonCnf::SaveRegex(const std::wstring& regex)const
{
	boost::property_tree::wptree tree;
	LoadXml(tree);
	tree.put(L"cnf.base.regex",regex);

	SaveXml(tree);
}

void SingletonCnf::Save(bool proxy
		,const std::wstring& address
		,unsigned short port
		,const std::wstring& name
		,const std::wstring& password

		,int timeout
		,int max_page
		,int max_count
		)
{

	if(timeout <=0)
	{
		timeout	=	DARK_DEFAULT_TIMEOUT;
	}
	if(max_page <=0)
	{
		max_page	=	DARK_DEFAULT_MAX_PAGE;
	}
	if(max_count <=0)
	{
		max_count	=	DARK_DEFAULT_MAX_COUNT;
	}

	boost::property_tree::wptree tree;
	LoadXml(tree);
	tree.put(L"cnf.proxy.proxy",proxy);
	tree.put(L"cnf.proxy.address",address);
	tree.put(L"cnf.proxy.port",port);
	tree.put(L"cnf.proxy.name",name);
	tree.put(L"cnf.proxy.password",password);

	tree.put(L"cnf.base.timeout",timeout);
	tree.put(L"cnf.base.max_page",max_page);
	tree.put(L"cnf.base.max_count",max_count);

	SaveXml(tree);

	proxy_		=	proxy;
	address_	=	address;
	port_		=	port;
	name_		=	name;
	password_	=	password;

	timeout_	=	timeout;
	max_page_	=	max_page;
	max_count_	=	max_count;
}