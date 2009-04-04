#pragma once
#include "afxcmn.h"


// CTaskSetting dialog


struct CFileInfo
{
		CString		file_name;
		CString		file_size;
		CString		finished_degree;
};

class CTaskSetting : public CDialog
{
public:
		DECLARE_DYNAMIC(CTaskSetting)

public:
		CTaskSetting(CWnd* pParent = NULL);   // standard constructor
		virtual ~CTaskSetting();

		// Dialog Data
		enum { IDD = IDD_TASK_SETTING };

protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
public:
		CString			m_save_location;
		CString			m_torrent_name;
		CString			m_torrent_publisher;
		CString			m_torrent_total_size;
		BOOL			m_is_stoped_task;

		CArray<CFileInfo>	m_files;
public:
		afx_msg void OnBnClickedButtonTaskSetBrowse();
private:
		CListCtrl m_file_list;
public:
		virtual BOOL OnInitDialog();
};
