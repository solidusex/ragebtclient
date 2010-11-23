#pragma once


// CSettingDialog dialog

class CSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDialog();

// Dialog Data
	enum { IDD = IDD_GLOBAL_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
		UINT m_cache_size;
		UINT m_listen_port;
		UINT m_max_conn_per_task;
		UINT m_max_upload_conn_per_task;
		UINT m_request_queue_size;
		CString m_def_save_path;
public:
		afx_msg void OnBnClickedButtonBrowseFloder();
};
