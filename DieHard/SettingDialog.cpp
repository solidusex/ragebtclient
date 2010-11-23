// SettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DieHard.h"
#include "SettingDialog.h"
#include "FolderDialog.h"
#include "Util.h"
// CSettingDialog dialog

IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
	, m_cache_size(0)
	, m_listen_port(0)
	, m_max_conn_per_task(0)
	, m_max_upload_conn_per_task(0)
	, m_request_queue_size(0)
	, m_def_save_path(_T(""))
{

}

CSettingDialog::~CSettingDialog()
{

}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_EDIT_CACHE_SIZE, m_cache_size);
		DDV_MinMaxUInt(pDX, m_cache_size, 2, 64);
		DDX_Text(pDX, IDC_EDIT_LISTEN_PORT, m_listen_port);
		DDV_MinMaxUInt(pDX, m_listen_port, 0, 65535);
		DDX_Text(pDX, IDC_EDIT_MAX_CONN_PER_TASK, m_max_conn_per_task);
		DDV_MinMaxUInt(pDX, m_max_conn_per_task, 20, 100);
		DDX_Text(pDX, IDC_EDIT_MAX_UPLOAD_PER_TASK, m_max_upload_conn_per_task);
		DDV_MinMaxUInt(pDX, m_max_upload_conn_per_task, 10, 100);
		DDX_Text(pDX, IDC_EDIT_QUEUE_SIZE, m_request_queue_size);
		DDV_MinMaxUInt(pDX, m_request_queue_size, 5, 10);
		DDX_Text(pDX, IDC_EDIT_DEF_SAVE_PATH, m_def_save_path);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
		ON_BN_CLICKED(IDC_BUTTON_BROWSE_FLODER, &CSettingDialog::OnBnClickedButtonBrowseFloder)
END_MESSAGE_MAP()


// CSettingDialog message handlers

void CSettingDialog::OnBnClickedButtonBrowseFloder()
{
		// TODO: Add your control notification handler code here
		CFolderDialog dialog;
		
		if(dialog.DoModal() == IDOK)
		{
				CEdit edit;
				edit.Attach(this->GetDlgItem(IDC_EDIT_DEF_SAVE_PATH)->m_hWnd);
				
				CString str = dialog.GetPathName();

				using namespace FileSysSpace;

				Path path(StrConv(str));

				if(path.GetPathType() == PATH_FILE)
				{
						str += _T("\\");
				}
				
				edit.SetWindowText(str);
				edit.Detach();
		}

}
