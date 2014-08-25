
// BrownianGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BrownianGUI.h"
#include "BrownianGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBrownianGUIDlg dialog



CBrownianGUIDlg::CBrownianGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrownianGUIDlg::IDD, pParent)
	, c_pStatus(CEdit())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrownianGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REPORT, c_pStatus);
}

BEGIN_MESSAGE_MAP(CBrownianGUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CBrownianGUIDlg message handlers

BOOL CBrownianGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	c_pStatus.Clear();
	int nTextLength = c_pStatus.GetWindowTextLengthW();
	c_pStatus.SetSel(nTextLength, nTextLength);
	CString strIntro = _T("******  Brownian calculation ver. 0.0.1  ******\n\n");
	c_pStatus.ReplaceSel(strIntro);
	AddTextToStatus(_T("Currently, only rectangular dimension is supported."));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBrownianGUIDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBrownianGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





bool CBrownianGUIDlg::AddTextToStatus(CString CStrText)
{
	int nTextLength = c_pStatus.GetWindowTextLengthW();
	c_pStatus.SetSel(nTextLength, nTextLength);
	c_pStatus.ReplaceSel(CStrText);

	return TRUE;
}
