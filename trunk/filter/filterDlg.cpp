
// filterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "filter.h"
#include "filterDlg.h"
#include "afxdialogex.h"
#include "DialogSetting.h"
#include "SingletonCnf.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


// CfilterDlg 对话框




CfilterDlg::CfilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfilterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	lua_state_ptr_	=	luaL_newstate();
	luaL_openlibs(lua_state_ptr_);
	
	more	=	start	=	0;
	
	index_	=	0;
	index_now_	=	-1;
}

CfilterDlg::~CfilterDlg()
{
	lua_close(lua_state_ptr_);
}

void CfilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	DDX_Control(pDX, IDC_LIST2, m_list_timeout);
}

BEGIN_MESSAGE_MAP(CfilterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GO, &CfilterDlg::OnBnClickedButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CfilterDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CfilterDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CfilterDlg::OnNMDblclkList1)
	ON_COMMAND(ID_FILE_ABOBT, &CfilterDlg::OnFileAbobt)
	ON_COMMAND(ID_FILE_SETTING, &CfilterDlg::OnFileSetting)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CfilterDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CfilterDlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CfilterDlg::OnBnClickedButtonMore)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TIMEOUT, &CfilterDlg::OnBnClickedButtonClearTimeout)
END_MESSAGE_MAP()


// CfilterDlg 消息处理程序

BOOL CfilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	if(luaL_loadfile(lua_state_ptr_,"function.lua"))
	{
		MessageBox(L"加載腳本 function.lua 失敗");
		PostMessage(WM_CLOSE);
	}
	else if(lua_pcall(lua_state_ptr_, 0, 0, 0))
	{
		MessageBox(L"執行腳本 function.lua 失敗");
		PostMessage(WM_CLOSE);
	}
	else
	{
		SetDlgItemText(IDC_EDIT1,SingletonCnf::get_const_instance().GetUrl().c_str());
		SetDlgItemText(IDC_EDIT2,SingletonCnf::get_const_instance().GetRegex().c_str());
		

		m_listctrl.InsertColumn(0,L"index",0,90);
		m_listctrl.InsertColumn(1,L"url",0,200);
		m_listctrl.InsertColumn(2,L"title",0,300);

		m_listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		m_list_timeout.InsertColumn(0,L"url",0,210);
		m_list_timeout.InsertColumn(1,L"number",0,80);

		m_list_timeout.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		LoadDll();
		
	}
     
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfilterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfilterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
bool CfilterDlg::HttpRequest(const std::string& url)
{
	try
	{
		const Poco::URI uri(url); 
		Poco::Net::HTTPClientSession session(uri.getHost(),uri.getPort());

		if(SingletonCnf::get_const_instance().GetProxy())
		{
			std::wstring address	=	SingletonCnf::get_const_instance().GetAddress();
			unsigned short port		=	SingletonCnf::get_const_instance().GetPort();
			session.setProxy(dark::StringConvert::Utf16ToAscii(address)
				,port
				);

			std::wstring name	=	SingletonCnf::get_const_instance().GetName();
			if(!name.empty())
			{
				session.setProxyUsername(dark::StringConvert::Utf16ToAscii(name));

				std::wstring password	=	SingletonCnf::get_const_instance().GetPassword();
				if(!password.empty())
				{
					session.setProxyPassword(dark::StringConvert::Utf16ToAscii(password));
				}
			}
		}
		std::string path	=	uri.getPathAndQuery();
		
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,path);	

		{
			//假裝是firefox瀏覽器 (沒有User-Agent信息 草榴不會返回數據)
			request.set("User-Agent","Mozilla/5.0 (Windows NT 6.1; WOW64; rv:29.0) Gecko/20100101 Firefox/29.0");
		}
		//設置超時時間 d 秒
		session.setTimeout(Poco::Timespan(SingletonCnf::get_const_instance().GetTimeOut(),0));
        
        session.sendRequest(request);
		
		//獲取響應
		Poco::Net::HTTPResponse response;
		std::istream& rs	=	session.receiveResponse(response);
	
		std::string str;
		Poco::StreamCopier::copyToString(rs,str);


		Format(str,uri.getHost());
		return true;
	}
	catch(const Poco::TimeoutException& )
	{
		std::wstring unicode_url	=	dark::StringConvert::AsciiToUtf16(url);
		if(time_out_urls_.find(unicode_url)	!=	time_out_urls_.end())
		{
			++time_out_urls_[unicode_url];
		}
		else
		{
			time_out_urls_[unicode_url]	=	0;
		}

		CString tmp;
		
		tmp.Format(L"%d",time_out_urls_[unicode_url]);
		bool find = false;
		for(int i=0;i<m_list_timeout.GetItemCount();++i)
		{
			if(unicode_url	==	m_list_timeout.GetItemText(i,0).GetBuffer())
			{
				m_list_timeout.SetItemText(i,1,tmp);
				find	=	true;
				break;
			}
		}
		if(!find)
		{
			m_list_timeout.InsertItem(0,unicode_url.c_str());
			m_list_timeout.SetItemText(0,1,tmp);
		}
	}
	catch(const Poco::Exception& )
	{
	}

	return false;
}

void CfilterDlg::Format(std::string& text,const std::string& host)
{

	int index	=	m_combox.GetCurSel();
	if(index	!=	-1)
	{
		CString tmp;
		m_combox.GetLBText(index,tmp);

		BOOST_AUTO(find,functions_.find(tmp.GetBuffer()));
		if(find	!=	functions_.end())
		{
			find->second(text,host,mutex_,datas_);
			
			tmp.Format(L"all:%d",datas_.size());
			SetDlgItemText(IDC_STATIC_COUNT,tmp);
		}
	}
}
void CfilterDlg::WorkThread(std::string url)
{
	try
	{
		
		SetDlgItemInt(IDC_STATIC_NUMBER,index_);
		std::string new_url;
		while(true)
		{
			lua_getglobal(lua_state_ptr_,"url_format");  

			lua_pushlstring(lua_state_ptr_,url.c_str(),url.size());
			lua_pushinteger(lua_state_ptr_,index_);
			if(!lua_pcall(lua_state_ptr_,2,1,0))
			{
				new_url	=	lua_tostring(lua_state_ptr_,-1);	
				lua_pop(lua_state_ptr_,1);

				boost::trim(new_url);
				if(new_url.empty())
				{
					break;
				}

				SetDlgItemText(IDC_STATIC_URL,dark::StringConvert::AsciiToUtf16(new_url).c_str());
				SetDlgItemInt(IDC_STATIC_NUMBER,index_);
				HttpRequest(new_url);

				++index_;
				if(SingletonCnf::get_const_instance().GetMaxCount() <= datas_.size()
					|| SingletonCnf::get_const_instance().GetMaxPage() <= index_)
				{
					break;
				}

			}
			else
			{
				lua_pop(lua_state_ptr_,1);
				continue;
			}
			boost::this_thread::interruption_point();
		}
	}
	catch(boost::thread_interrupted&)
	{
	}

	PostMessage(WM_THREAD_END);
}
void CfilterDlg::Stop()
{
	if(thread_ptr_
		&& start & DARK_STATE_THREAD_RUN
		)
	{
		thread_ptr_->interrupt();
		thread_ptr_	=	thread_ptr_t();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_GO)->SetWindowText(L"start");
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		m_combox.EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_CLEAR_TIMEOUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MORE)->EnableWindow(TRUE);

		CString tmp;
		tmp.Format(L"all:%d",datas_.size());
		SetDlgItemText(IDC_STATIC_COUNT,tmp);

		start	=	0;

		if(index_now_	!=	-1)
		{
			index_	=	index_now_;
		}
		else
		{
			index_	=	0;
		}
	}
}
void CfilterDlg::OnBnClickedButtonGo()
{
	if(start & DARK_STATE_START)
	{
		index_now_	=	index_;
		Stop();
	}
	else
	{	
		CString tmp;
		GetDlgItemText(IDC_EDIT1,tmp);
		std::wstring unicode	=	tmp.GetBuffer();
		boost::trim(unicode);
		if(unicode.empty())
		{
			return;
		}

		int index	=	m_combox.GetCurSel();
		if(index	==	-1)
		{
			return;
		}
		m_combox.GetLBText(index,tmp);
		if(functions_.find(tmp.GetBuffer())	==	functions_.end())
		{
			return;
		}
		SingletonCnf::get_const_instance().SavePlugin(tmp.GetBuffer());



		index_now_	=	-1;
		std::string ascii	=	dark::StringConvert::Utf16ToAscii(unicode);

		thread_ptr_	=	boost::make_shared<boost::thread>(
			boost::bind(&CfilterDlg::WorkThread,this,ascii)
			);
		
		GetDlgItem(IDC_BUTTON_GO)->SetWindowText(L"stop");
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		m_combox.EnableWindow(FALSE);
		
		GetDlgItem(IDC_BUTTON_CLEAR_TIMEOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MORE)->EnableWindow(FALSE);

		SingletonCnf::get_const_instance().SaveUrl(unicode);
		start	=	DARK_STATE_START | DARK_STATE_THREAD_RUN;
	}
}

void CfilterDlg::OnBnClickedButtonFind()
{
	CString tmp;
	GetDlgItemText(IDC_EDIT2,tmp);
	std::wstring text	=	tmp.GetBuffer();

	SingletonCnf::get_const_instance().SaveRegex(text);
	boost::xpressive::wcregex reg	=	boost::xpressive::wcregex::compile(text);
	
	m_listctrl.DeleteAllItems();

	int index	=	0;
	boost::mutex::scoped_lock lock(mutex_);
	BOOST_FOREACH(auto& note,datas_)
	{
		if(boost::xpressive::regex_match(note.second.c_str(),reg))
		{
			tmp.Format(L"%d",index);
			m_listctrl.InsertItem(index,tmp);
			m_listctrl.SetItemText(index,1,note.first.c_str());
			m_listctrl.SetItemText(index,2,note.second.c_str());
			++index;
		}
	}
}


void CfilterDlg::OnBnClickedButtonClear()
{
	boost::mutex::scoped_lock lock(mutex_);
	datas_.clear();

	SetDlgItemText(IDC_STATIC_COUNT,L"all:0");
}


void CfilterDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CfilterDlg::OnCancel()
{
	/*if(thread_ptr_)
	{
		thread_ptr_->interrupt();
		thread_ptr_->join();
			
		thread_ptr_	=	thread_ptr_t();
	}

	if(thread_more_ptr_)
	{
		thread_more_ptr_->interrupt();
		thread_more_ptr_->join();
			
		thread_more_ptr_	=	thread_ptr_t();
	}*/

	CDialogEx::OnCancel();
}


void CfilterDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if(pNMItemActivate->iItem	!=	-1)
	{
		if(GetKeyState(VK_CONTROL) & 0x8000 
			&& OpenClipboard()
			)
		{
			if(EmptyClipboard())
			{
				CString wcs		=	m_listctrl.GetItemText(pNMItemActivate->iItem,2);
				int len			=	(wcs.GetLength() + 1) * sizeof(wchar_t);
				HANDLE hClip	=	GlobalAlloc(GMEM_MOVEABLE,len);
				wchar_t * pBuf	=	(wchar_t *)GlobalLock(hClip);
				wcscpy_s(pBuf,len/2,wcs.GetBuffer());
				GlobalUnlock(hClip);
				SetClipboardData(CF_UNICODETEXT,hClip);
				CloseClipboard();					
			}	
		}
		else
		{
			std::wstring url	=	m_listctrl.GetItemText(pNMItemActivate->iItem,1).GetBuffer();
			ShellExecute(NULL,NULL,url.c_str(),NULL,NULL,SW_SHOW);
		}
	}
	*pResult = 0;
}


void CfilterDlg::OnFileAbobt()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CfilterDlg::OnFileSetting()
{
	CDialogSetting dlg;
	dlg.DoModal();
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShellExecute(NULL,NULL,L"http://zuiwuchang.blogspot.com/",NULL,NULL,SW_SHOW);
	*pResult = 0;
}


LRESULT CfilterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if(WM_THREAD_END	==	message
		&& start
		)
	{
		start	=	DARK_STATE_START;
		Stop();
	}
	else if(WM_THREAD_MORE_END	==	message
		&& more
		)
	{
		more	=	DARK_STATE_START;
		OnBnClickedButtonMore();
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CfilterDlg::OnBnClickedButtonExport()
{
	wchar_t filter[]	=	L"XML File(*.xml)|*.xml||";
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY,filter); 
	dlg.m_ofn.lpstrDefExt	=	L"xml";
	if(dlg.DoModal()	==	IDOK)
	{
		CString path	=	dlg.GetPathName();

		CString tmp;
		
		boost::property_tree::wptree tree;
		tree.put(L"cnf.progress",GetDlgItemInt(IDC_STATIC_NUMBER));
		GetDlgItemText(IDC_EDIT1,tmp);
		tree.put(L"cnf.base",tmp.GetBuffer());
	
			boost::property_tree::wptree urls;
			BOOST_FOREACH(auto& data,datas_)
			{
				boost::property_tree::wptree note;
				note.add(L"url",data.first);
				note.add(L"title",data.second);

				urls.add_child(L"note",note);
			}
			tree.add_child(L"cnf.urls",urls);

			boost::property_tree::wptree timeouts;
			BOOST_FOREACH(auto& data,time_out_urls_)
			{
				boost::property_tree::wptree note;
				note.add(L"url",data.first);
				note.add(L"count",data.second);

				timeouts.add_child(L"note",note);
			}
			tree.add_child(L"cnf.timeouts",timeouts);

			std::wofstream out(path.GetBuffer(),std::ios::out | std::ios::trunc);
			if(out.is_open())
			{
				out.imbue(std::locale("chs"));
				boost::property_tree::write_xml(out,tree);
				out.close();
			
				
				MessageBox(L"export success");
			}
			else
			{
				MessageBox(L"export error");
			}
	}

}


void CfilterDlg::OnBnClickedButtonImport()
{
	wchar_t filter[]	=	L"XML File(*.xml)|*.xml||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,filter); 
	if(dlg.DoModal()	==	IDOK)
	{
		CString path	=	dlg.GetPathName();

		std::wifstream in(path.GetBuffer(),std::ios::in);
		if(in.is_open())
		{
			in.imbue(std::locale("chs"));	
			try
			{
				boost::property_tree::wptree tree;
				boost::property_tree::read_xml(in,tree);		

				index_	=	tree.get<int>(L"cnf.progress");
				SetDlgItemInt(IDC_STATIC_NUMBER
					,index_
					);

				SetDlgItemText(IDC_EDIT1
					,tree.get<std::wstring>(L"cnf.base").c_str()
					);

				BOOST_AUTO(&child,tree.get_child(L"cnf.urls"));
				typedef BOOST_TYPEOF(*child.begin()) ptree_t;
				std::wstring url,title;
				BOOST_FOREACH(ptree_t& var,child)
				{
					if(var.first	==	L"note")
					{
						url		=	var.second.get<std::wstring>(L"url");
						title	=	var.second.get<std::wstring>(L"title");
						boost::trim(url);
						boost::trim(title);
						if(!url.empty())
						{
							datas_[url]	=	title;	
						}
					}
					
				}
				CString tmp;
				tmp.Format(L"all:%d",datas_.size());
				SetDlgItemText(IDC_STATIC_COUNT,tmp);

				BOOST_AUTO(&child_timeout,tree.get_child(L"cnf.timeouts"));
				int count;
				bool time_out	=	false;
				BOOST_FOREACH(ptree_t& var,child_timeout)
				{
					if(var.first	==	L"note")
					{
						url		=	var.second.get<std::wstring>(L"url");
						count	=	var.second.get<int>(L"count");
						boost::trim(url);
						
						if(!url.empty())
						{
							time_out_urls_[url]	=	count;	
							time_out	=	true;
						}
					}
					
				}
				if(time_out)
				{
					int i	=	0;
					
					m_list_timeout.DeleteAllItems();
					BOOST_FOREACH(auto& note,time_out_urls_)
					{
						m_list_timeout.InsertItem(i,note.first.c_str());
						tmp.Format(L"%d",note.second);
						m_list_timeout.SetItemText(i,1,tmp);
					}
					
				}

				MessageBox(L"import success");
			}
			catch(const boost::property_tree::xml_parser::xml_parser_error& )
			{
				in.close();
				MessageBox(L"import error");
			}
			catch(const boost::property_tree::ptree_bad_path& )
			{
				in.close();
				MessageBox(L"import error");
			}
			catch(const boost::property_tree::ptree_bad_data& )
			{
				in.close();
				MessageBox(L"import error");
			}
			in.close();
		}
	
	}
}


BOOL CfilterDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message	==	WM_KEYDOWN
		&& pMsg->wParam	==	VK_RETURN
		&& pMsg->hwnd	==	GetDlgItem(IDC_EDIT2)->m_hWnd)
	{
		OnBnClickedButtonFind();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CfilterDlg::OnBnClickedButtonMore()
{
	if(more)
	{
		if(more & DARK_STATE_THREAD_RUN
			&& thread_more_ptr_
			)
		{
			thread_more_ptr_->interrupt();
			thread_more_ptr_	=	thread_ptr_t();
		}
		else
		{
			GetDlgItem(IDC_BUTTON_MORE)->SetWindowText(L"noce more");
			GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			m_combox.EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_CLEAR_TIMEOUT)->EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		
			GetDlgItem(IDC_BUTTON_GO)->EnableWindow(TRUE);

			CString tmp;
			tmp.Format(L"all:%d",datas_.size());
			SetDlgItemText(IDC_STATIC_COUNT,tmp);

			more	=	0;
		}
	}
	else
	{
		CString tmp;
		GetDlgItemText(IDC_EDIT1,tmp);
		std::wstring unicode	=	tmp.GetBuffer();
		boost::trim(unicode);
		if(unicode.empty())
		{
			return;
		}
		std::string ascii	=	dark::StringConvert::Utf16ToAscii(unicode);

		thread_more_ptr_	=	boost::make_shared<boost::thread>(
			boost::bind(&CfilterDlg::WorkThreadMore,this)
			);
		
		GetDlgItem(IDC_BUTTON_MORE)->SetWindowText(L"stop");
		GetDlgItem(IDC_BUTTON_CLEAR_TIMEOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		m_combox.EnableWindow(FALSE);
		
		GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_GO)->EnableWindow(FALSE);

		SingletonCnf::get_const_instance().SaveUrl(unicode);
		more	=	DARK_STATE_START | DARK_STATE_THREAD_RUN;
	}
}

void CfilterDlg::WorkThreadMore()
{
	std::vector<std::wstring> urls;
	try
	{
		BOOST_FOREACH(auto& note,time_out_urls_)
		{
			if(HttpRequest(dark::StringConvert::Utf16ToAscii(note.first)))
			{
				urls.push_back(note.first);

				BOOST_AUTO(find,time_out_urls_.find(note.first));
				if(find	!=	time_out_urls_.end())
				{
					for(int i=0;i<m_list_timeout.GetItemCount();++i)
					{
						if(note.first	==	m_list_timeout.GetItemText(i,0).GetBuffer())
						{
							m_list_timeout.DeleteItem(i);					
							break;
						}
					}			
			
				}
		
			}
			boost::this_thread::interruption_point();
		}
	}
	catch(boost::thread_interrupted&)
	{
	}

	BOOST_FOREACH(std::wstring& url,urls)
	{
		time_out_urls_.erase(time_out_urls_.find(url));
	}

	PostMessage(WM_THREAD_MORE_END);
}

void CfilterDlg::OnBnClickedButtonClearTimeout()
{
	time_out_urls_.clear();
	m_list_timeout.DeleteAllItems();
}


void CfilterDlg::LoadDll()
{
	dark::dll::SetResourceHandle();

	WIN32_FIND_DATA find_data;
	HANDLE hFind	=	FindFirstFile(L"plugin/*.dll",&find_data);
	if(INVALID_HANDLE_VALUE	!=	hFind)
	{
		CString path;
		do
		{
			path	=	L"plugin/";
			path	+=	find_data.cFileName;

			HINSTANCE hinstance	=	LoadLibraryW(path);
			if(!hinstance)
			{
				continue;
			}

			GetPluinName_T GetPluinName	=	(GetPluinName_T)GetProcAddress(hinstance,"GetPluinName");
			DataFormat_T DataFormat	=	(DataFormat_T)GetProcAddress(hinstance,"DataFormat");
			if(!GetPluinName
				|| !DataFormat
				)
			{
				continue;
			}
			
			functions_[GetPluinName()]	=	DataFormat;

		}while(FindNextFile(hFind,&find_data));
		FindClose(hFind);
	}

	int i	=	0;
	int sel	=	0;
	std::wstring name	=	SingletonCnf::get_const_instance().GetPlugin();
	BOOST_FOREACH(auto& note,functions_)
	{
		m_combox.InsertString(i,note.first.c_str());
		if(name	==	note.first)
		{
			sel	=	i;
		}
		++i;
	}

	if(i)
	{
		m_combox.SetCurSel(sel);
	}
}