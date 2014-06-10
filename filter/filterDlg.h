
// filterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "plugin.h"

#define DARK_STATE_START		0x1
#define DARK_STATE_THREAD_RUN	0x2


// CfilterDlg 对话框
class CfilterDlg : public CDialogEx
{
// 构造
public:
	CfilterDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CfilterDlg();

// 对话框数据
	enum { IDD = IDD_FILTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	lua_State* lua_state_ptr_;

	int index_now_;
	int index_;

	int start;
	afx_msg void OnBnClickedButtonGo();
	void Stop();
	afx_msg void OnBnClickedButtonFind();

	void WorkThread(std::string url);
	bool HttpRequest(const std::string& url);
	void Format(std::string& text,const std::string& host);

	boost::mutex mutex_;
	boost::unordered_map<std::wstring/*url*/,std::wstring/*title*/> datas_;

	typedef boost::shared_ptr<boost::thread> thread_ptr_t;
	thread_ptr_t	thread_ptr_;

	CComboBox m_combox;

	afx_msg void OnBnClickedButtonClear();
	CListCtrl m_listctrl;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileAbobt();
	afx_msg void OnFileSetting();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	CListCtrl m_list_timeout;
	boost::unordered_map<std::wstring/*url*/,int /*count*/> time_out_urls_;

	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonImport();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int more;
	thread_ptr_t	thread_more_ptr_;
	void WorkThreadMore();
	afx_msg void OnBnClickedButtonMore();
	afx_msg void OnBnClickedButtonClearTimeout();

	void LoadDll();
	boost::unordered_map<std::wstring,DataFormat_T> functions_;

};
