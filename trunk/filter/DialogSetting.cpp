// DialogSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "filter.h"
#include "DialogSetting.h"
#include "afxdialogex.h"
#include "SingletonCnf.h"

// CDialogSetting 对话框

IMPLEMENT_DYNAMIC(CDialogSetting, CDialogEx)

CDialogSetting::CDialogSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSetting::IDD, pParent)
{

}

CDialogSetting::~CDialogSetting()
{
}

void CDialogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogSetting 消息处理程序


BOOL CDialogSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(SingletonCnf::get_const_instance().GetProxy())
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	}

	SetDlgItemText(IDC_EDIT_ADDRESS,SingletonCnf::get_const_instance().GetAddress().c_str());
	SetDlgItemInt(IDC_EDIT_PORT,SingletonCnf::get_const_instance().GetPort());
	SetDlgItemText(IDC_EDIT_NAME,SingletonCnf::get_const_instance().GetName().c_str());
	SetDlgItemText(IDC_EDIT_PASSWORD,SingletonCnf::get_const_instance().GetPassword().c_str());

	SetDlgItemInt(IDC_EDIT_TIMEOUT,SingletonCnf::get_const_instance().GetTimeOut());
	SetDlgItemInt(IDC_EDIT_MAX_PAGE,SingletonCnf::get_const_instance().GetMaxPage());
	SetDlgItemInt(IDC_EDIT_MAX_COUNT,SingletonCnf::get_const_instance().GetMaxCount());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogSetting::OnBnClickedOk()
{
	CString tmp;
	bool proxy	=	((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == 1;
	GetDlgItemText(IDC_EDIT_ADDRESS,tmp);
	std::wstring address = tmp.Trim().GetBuffer();
	GetDlgItemText(IDC_EDIT_NAME,tmp);
	std::wstring name = tmp.Trim().GetBuffer();
	GetDlgItemText(IDC_EDIT_PASSWORD,tmp);
	std::wstring password = tmp.Trim().GetBuffer();

	SingletonCnf::get_mutable_instance().Save(proxy
		,address
		,GetDlgItemInt(IDC_EDIT_PORT)
		,name
		,password

		,GetDlgItemInt(IDC_EDIT_TIMEOUT)
		,GetDlgItemInt(IDC_EDIT_MAX_PAGE)
		,GetDlgItemInt(IDC_EDIT_MAX_COUNT)
		);
		
	CDialogEx::OnOK();
}
