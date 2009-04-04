// TaskSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DieHard.h"
#include "TaskSetting.h"
#include "FolderDialog.h"
#include "Util.h"

// CTaskSetting dialog

IMPLEMENT_DYNAMIC(CTaskSetting, CDialog)

CTaskSetting::CTaskSetting( CWnd* pParent /*=NULL*/)	
		: CDialog(CTaskSetting::IDD, pParent)
		, m_save_location(_T(""))
		, m_torrent_name(_T("None"))
		, m_torrent_publisher(_T("None"))
		, m_torrent_total_size(_T("None"))
		, m_is_stoped_task(FALSE)
{
		

}

CTaskSetting::~CTaskSetting()
{

}


void CTaskSetting::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_EDIT_TASK_SAVE_PATH, m_save_location);
		DDX_Text(pDX, IDC_STATIC_TASK_SET_PUBLISHER, m_torrent_name);
		DDX_Text(pDX, IDC_STATIC_TASK_SET_TORRENT_NAME, m_torrent_publisher);
		DDX_Text(pDX, IDC_STATIC_TASK_SET_TOTAL_SIZE, m_torrent_total_size);
		DDX_Control(pDX, IDC_LIST_TASK_SET_FILELIST, m_file_list);
}


BEGIN_MESSAGE_MAP(CTaskSetting, CDialog)
		ON_BN_CLICKED(IDC_BUTTON_TASK_SET_BROWSE, &CTaskSetting::OnBnClickedButtonTaskSetBrowse)
END_MESSAGE_MAP()


// CTaskSetting message handlers

void CTaskSetting::OnBnClickedButtonTaskSetBrowse()
{
		// TODO: Add your control notification handler code here


		CFolderDialog dialog;
		
		if(dialog.DoModal() == IDOK)
		{
				CEdit edit;
				edit.Attach(this->GetDlgItem(IDC_EDIT_TASK_SAVE_PATH)->m_hWnd);
				
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

BOOL CTaskSetting::OnInitDialog()
{
		CDialog::OnInitDialog();

		// TODO:  Add extra initialization here
		
		m_file_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

		m_file_list.InsertColumn(0, _T("File Name"));
		m_file_list.InsertColumn(1, _T("Size[Mb]"));
		m_file_list.InsertColumn(2, _T("%"));

		m_file_list.SetColumnWidth(0, 150);
		m_file_list.SetColumnWidth(1, 100);
		m_file_list.SetColumnWidth(2, 50);

		//m_file_list.SetExtendedStyle(LVS_EX_GRIDLINES);

		for(UINT i = 0; i < m_files.GetSize(); ++i)
		{
				m_file_list.InsertItem(i, m_files[i].file_name, 0);
				m_file_list.SetItemText(i, 1, m_files[i].file_size);
				m_file_list.SetItemText(i, 2, m_files[i].finished_degree);
		}
		
		if(!m_is_stoped_task)
		{
				CButton browse;

				browse.Attach(this->GetDlgItem(IDC_BUTTON_TASK_SET_BROWSE)->m_hWnd);

				browse.EnableWindow(FALSE);

				browse.Detach();
		}

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}
