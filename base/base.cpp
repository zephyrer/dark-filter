// base.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "base.h"


// 这是导出变量的一个示例
BASE_API int nbase=0;

// 这是导出函数的一个示例。
BASE_API int fnbase(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 base.h
Cbase::Cbase()
{
	return;
}

_declspec(dllexport)
std::wstring GetPluinName()
{
	static std::wstring name	=	DARK_PLUGIN_NAME;
	return name;
}
_declspec(dllexport)
void DataFormat(std::string& text,const std::string& host,boost::mutex& mutex,boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/>& datas)
{
	std::string::size_type begin	=	text.find("<tbody")
		,end	=	text.find("/tbody>");
	if(begin == std::string::npos
		//|| end == std::string::npos
		)
	{
		return;
	}
	std::string tbody	=	text.substr(begin , end + strlen("tbody>") - begin);
	
	std::string tr,a,href,title;
	std::string::size_type a_begin,a_end;
	end	=	0;
	while(true)
	{
		boost::this_thread::interruption_point();
		//匹配<tr>
		begin	=	tbody.find("<tr",end);
		if(begin	==	std::string::npos)
		{
			break;
		}
		end		=	tbody.find("/tr>",begin);
		if(end	==	std::string::npos)
		{
			break;
		}
		tr	=	tbody.substr(begin , end + strlen("/tr>") - begin);

		a_end	=	0;
		while(true)
		{
			boost::this_thread::interruption_point();
			//匹配<a>
			a_begin	=	tr.find("<a",a_end);
			if(a_begin	==	std::string::npos)
			{
				break;
			}
			a_end	=	tr.find("/a>",a_begin);
			if(a_end	==	std::string::npos)
			{
				break;
			}
			a	=	tr.substr(a_begin , a_end + strlen("/a>") - a_begin);
		
			try
			{
				boost::property_tree::ptree tree;
				std::stringstream in;
				in<<a;
				boost::property_tree::read_xml(in,tree);
				tree.get<std::string>("a.<xmlattr>.id");
				if(tree.get<std::string>("a.<xmlattr>.target")	==	"_blank")
				{
					href	=	tree.get<std::string>("a.<xmlattr>.href");
					title	=	tree.get<std::string>("a");
					if(!boost::starts_with(href,L"http"))
					{
						href	=	"http://" + host + "/" + href;
					}
					//增加數據
					boost::mutex::scoped_lock lock(mutex);
					datas[dark::StringConvert::AsciiToUtf16(href)]	
						=	dark::StringConvert::AsciiToUtf16(title)
							;
					break;
				}
			}
			catch(const boost::property_tree::xml_parser::xml_parser_error& )
			{
				
			}
			catch(const boost::property_tree::ptree_bad_path& )
			{
				
			}
			catch(const boost::property_tree::ptree_bad_data& )
			{
			
			}
		}
	}

}