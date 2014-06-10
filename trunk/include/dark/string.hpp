//string.hpp	字符串相關算法
#ifndef _DARK_STRING_
#define _DARK_STRING_
#include<string>
#include<vector>

#include<boost\smart_ptr.hpp>
#include<boost\noncopyable.hpp>
#include<boost\foreach.hpp>
#include<boost\function.hpp>

namespace dark
{
	template<typename _Elem
		,typename _Traits = std::char_traits<_Elem>
		,typename _Ax =	std::allocator<_Elem> >
	class basic_tokenizer
	{
		typedef boost::function<int(_Elem _C)> delimiter_fun_t;
	public:
		basic_tokenizer(delimiter_fun_t _Per = basic_tokenizer::isspace
			)
		{
			per_	=	_Per;
		}
		~basic_tokenizer(void){}

		inline static bool isspace(_Elem _C)
		{
			return std::isspace(_C,std::locale("chs"));
		}
		inline static bool ischar(_Elem _C,_Elem _Compare)
		{
			return _C	==	_Compare;
		}
	protected:
		std::basic_string<_Elem,_Traits,_Ax> str_;

		delimiter_fun_t per_;
	
	public:
		static std::size_t find_delimiter(const std::basic_string<_Elem,_Traits,_Ax>& str
			,const std::size_t begin
			,const std::size_t end
			,delimiter_fun_t _Per
			)
		{
			for(std::size_t i = begin;i<end;++i)
			{
				if(i >= str.size())
				{
					break;
				}
				if(_Per(str[i]))
				{
					return i;
				}
			}

			return str.npos;
		}
		void operator()(const std::basic_string<_Elem,_Traits,_Ax>& str)
		{
			params_.clear();

			std::size_t begin	=	0;
			std::size_t find	=	0;

			std::basic_string<_Elem,_Traits,_Ax> note;
			while(true)
			{
				find = find_delimiter(str,begin,str.npos,per_);

				if(find	==	str.npos)
				{
					note	=	str.substr(begin);
					if(!note.empty())
					{
						params_.push_back(note);
					}
					break;
				}
				else
				{
					note	=	str.substr(begin,find - begin);
					if(!note.empty())
					{
						params_.push_back(note);
					}
				}
				begin	=	find + 1;
			}

		}
	public:
		std::vector<std::basic_string<_Elem,_Traits,_Ax>> params_;
	
	};

	typedef basic_tokenizer<char>		tokenizer_t;
	typedef basic_tokenizer<wchar_t>	wtokenizer_t;

	class StringTools
	{
	private:
		StringTools(){};

	public:
		static std::wstring GetFileNameByPath(const std::wstring& path)
		{
			std::wstring::size_type find1	=	path.find_last_of(L'\\')		
				,find2	=	path.find_last_of(L'/');
			if(find1	!=	std::wstring::npos
				&& find2	==	std::wstring::npos
				)
			{
				return path.substr(find1 + 1);
			}
			else if(find1	==	std::wstring::npos
				&& find2	!=	std::wstring::npos
				)
			{
				return path.substr(find2 + 1);
			}
			else if(find1	!=	std::wstring::npos
				&& find2	!=	std::wstring::npos
				)
			{
				find1	=	find1 > find2 ? find1 : find2;
				return path.substr(find1 + 1);
			}

			return path;
		}
	};
	class StringConvert
		: boost::noncopyable
	{
	private:
		StringConvert(){};

	protected:
		static char get_wchar_6bity(const wchar_t wc,const unsigned int index)
		{
			char c	=	0;
			switch(index)
			{
			case 0:
				c	=	wc & 0x3F;
				break;
			case 1:
				c	=	(wc & 0xFC0) >> 0x6;
				break;
			case 2:
				c	=	(wc >> 0xC);
				break;
			}
			return c;
		}
		static void loop_fill_utf8(const wchar_t wc,int begin,std::string& utf8)
		{
			char c;
			while(begin>=0)
			{
				c	=	get_wchar_6bity(wc,begin--);
				c	|=	0x80;	//10000000
				utf8.push_back(c);
			}
		}
	public:
		static void toUTF8(const std::wstring& unicode,std::string& utf8)
		{
			utf8.clear();
			char c;
			BOOST_FOREACH(wchar_t wc,unicode)
			{
				if(wc <= 0x0000007F)
				{
					//[0x00000000 ,0x0000007F]	單字節ASCII 編碼
					utf8.push_back((char)wc);
				}
				else if(wc >= 0x00000080
					&& wc <= 0x000007FF
					)
				{
					//[0x00000080,0x000007FF]	雙字節	拉丁文 希臘字母 ...
					c	=	get_wchar_6bity(wc,1);
					c	|=	0xC0;	//11000000
					utf8.push_back(c);

					loop_fill_utf8(wc,0,utf8);
				}
				else if(wc >= 0x00000800
					&& wc <= 0x0000FFFF
					)
				{
					//[0x00000800,0x0000FFFF]	基於多文種的常用字符 漢字 倭文 等方塊字 ...
					c	=	get_wchar_6bity(wc,2);
					c	|=	0xE0;	//11100000
					utf8.push_back(c);

					loop_fill_utf8(wc,1,utf8);
				}
				else if(wc >= 00010000
					&& wc <= 0x0010FFFF		//2003/11  RFC 3629 重新規範 utf8只取 [0x000000,0x0010FFFF] 範圍的值
					)
				{
					//[00010000,0x001FFFFF]	q其他不常用字符 ...
					c	=	get_wchar_6bity(wc,3);
					c	|=	0xE0;	//11100000
					utf8.push_back(c);

					loop_fill_utf8(wc,2,utf8);
				}

			}
		}

	protected:
		static wchar_t read_utf8_wchar(std::string::size_type* index_ptr,const std::string& utf8)
		{
			wchar_t wc,tmp;
			char c	=	utf8[*index_ptr];

			if(!(c & 0x80))
			{
				//0xxxxxxx	單字節
				++(*index_ptr);
				return c;
			}
			else if((c & 0xE0) == 0xC0)
			{
				//110xxxxx	2字節	
				wc	=	c;
				wc	<<=	0x6;
				wc	|=	utf8[*index_ptr + 1] & 0x3F;
				(*index_ptr)	+=	2;
			}
			else if((c & 0xF0) == 0xE0)
			{
				//1110xxxx	3字節
				wc	=	c;
				wc	<<=	0xC;
				
				tmp	=	utf8[*index_ptr + 1] & 0x3F;
				tmp	<<=	0x6;
				wc	|=	tmp;

				tmp	=	utf8[*index_ptr + 2] & 0x3F;
				wc	|=	tmp;

				(*index_ptr)	+=	3;
			}
			else if((c & 0xF8) == 0xF0)
			{
				//11110xxx	4字節
				wc	=	utf8[*index_ptr + 1] & 0x3F;;
				wc	<<=	0x6;
				
				tmp	=	utf8[*index_ptr + 2] & 0x3F;
				tmp	<<=	0x6;
				wc	|=	tmp;

				tmp	=	utf8[*index_ptr + 3] & 0x3F;
				wc	|=	tmp;

				(*index_ptr)	+=	4;
			}
			return wc;
		}
	public:
		static void toUNICODE(const std::string& utf8,std::wstring& unicode)
		{
			unicode.clear();
			wchar_t wc;
			for(std::string::size_type i = 0;i<utf8.size();)
			{
				wc	=	read_utf8_wchar(&i,utf8);
				unicode.push_back(wc);
			}
		}

#ifdef WIN32
#ifdef _WINDOWS_
		static std::string Utf16ToAscii(const std::wstring& utf16)
		{
			int count	=	WideCharToMultiByte(CP_ACP,0,utf16.c_str(),utf16.size() + 1,0,0,0,0);
			boost::scoped_array<char>	ptr	(new char[count]);

			WideCharToMultiByte(CP_ACP,0,utf16.c_str(),utf16.size() + 1,ptr.get(),count,0,0);

			return std::string(ptr.get());
		}
		static std::wstring AsciiToUtf16(const std::string& ascii)
		{
			int count	=	MultiByteToWideChar(CP_ACP,0,ascii.c_str(),ascii.size() + 1,0,0);
			boost::scoped_array<wchar_t>	ptr	(new wchar_t[count]);

			MultiByteToWideChar(CP_ACP,0,ascii.c_str(),ascii.size() + 1,ptr.get(),count);

			return std::wstring(ptr.get());
		}
#endif	//_WINDOWS_
#endif	//WIN32
	};
};

#endif	//_DARK_STRING_