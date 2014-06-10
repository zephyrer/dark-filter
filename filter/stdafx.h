
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include<string>

#include<boost\algorithm\string.hpp>
#include<boost\bind.hpp>
#include<boost\function.hpp>
#include<boost\thread.hpp>
#include<boost\smart_ptr.hpp>
#include<boost\xpressive\xpressive_dynamic.hpp>
#include<boost\property_tree\ptree.hpp>
#include<boost\property_tree\xml_parser.hpp>
#include<boost\typeof\typeof.hpp>
#include<boost\foreach.hpp>
#include<boost\unordered_map.hpp>
#include<boost\serialization\singleton.hpp>

#include <Poco\Net\HTTPClientSession.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\StreamCopier.h>
#include <Poco\URI.h>
#include <Poco\Exception.h>

#include<lua.hpp>
#pragma comment(lib,"lua5.1.lib")

#include<dark\string.hpp>
#include<dark\dll\dll.hpp>

#define CNF_PATH "filter.xml"

#define DARK_DEFAULT_URL	L"http://cl.s1.lv/thread0806.php?fid=15&search=&page="
#define DARK_DEFAULT_REGEX	L".*護士.*"


#define DARK_DEFAULT_PROXY		L"0"
#define DARK_DEFAULT_ADDRESS	L"127.0.0.1"
#define DARK_DEFAULT_PORT		8087
#define DARK_DEFAULT_NAME		L""
#define DARK_DEFAULT_PASSWORD	L""


#define DARK_DEFAULT_TIMEOUT	10
#define DARK_DEFAULT_MAX_PAGE	100
#define DARK_DEFAULT_MAX_COUNT	5000

#define WM_THREAD_END WM_USER + 1102
#define WM_THREAD_MORE_END WM_USER + 1103