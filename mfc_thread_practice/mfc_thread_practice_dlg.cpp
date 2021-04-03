
// mfc_thread_practice_dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "mfc_thread_practice.h"
#include "mfc_thread_practice_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT SampleThreadProc(LPVOID arg)
{
	Sleep(1000);

	mfc_thread_practice_dlg* _dlg = (mfc_thread_practice_dlg*)arg;
	_dlg->SetThreadStatus(TRUE);
	_dlg->ClearEditLog();

	while (_dlg->IsThreadStarted())
	{
		CString _log;
		_log.Format(_T("\r\n%s"), L"Hello, I am another thread");

		int _log_length = _dlg->GetEditLog()->GetWindowTextLength();
		_dlg->GetEditLog()->SetSel(_log_length, _log_length);
		_dlg->GetEditLog()->ReplaceSel(_log);

		Sleep(1000);
	}

	return 0;
}

mfc_thread_practice_dlg::mfc_thread_practice_dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_THREAD_PRACTICE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEdit* mfc_thread_practice_dlg::GetEditLog()
{
	return &_edit_log;
}

void mfc_thread_practice_dlg::ClearEditLog()
{
	_edit_log.Clear();
}

BOOL mfc_thread_practice_dlg::IsThreadStarted()
{
	return b_thread_started;
}

void mfc_thread_practice_dlg::SetThreadStatus(BOOL started)
{
	b_thread_started = started;
}

void mfc_thread_practice_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, _edit_log);
}

BEGIN_MESSAGE_MAP(mfc_thread_practice_dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStartClicked)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStopClicked)
END_MESSAGE_MAP()


// mfc_thread_practice_dlg message handlers

BOOL mfc_thread_practice_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void mfc_thread_practice_dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR mfc_thread_practice_dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void mfc_thread_practice_dlg::OnButtonStartClicked()
{	
	if (!IsThreadStarted())
	{
		_edit_log.SetWindowText(L"Start thread");
		AfxBeginThread(SampleThreadProc, this);
	}
}

void mfc_thread_practice_dlg::OnButtonStopClicked()
{
	SetThreadStatus(FALSE);
	_edit_log.SetWindowText(L"Stop thread");
}