
// mfc_thread_practice_dlg.h : header file
//

#pragma once


// mfc_thread_practice_dlg dialog
class mfc_thread_practice_dlg : public CDialogEx
{
// Construction
public:
	mfc_thread_practice_dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_THREAD_PRACTICE_DIALOG };
#endif

	CEdit* GetEditLog();
	void ClearEditLog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStartClicked();
	afx_msg void OnButtonStopClicked();
	DECLARE_MESSAGE_MAP()

private:

	CEdit _edit_log;
	CWinThread* _log_thread;
};
