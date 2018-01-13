
// BrownianGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BrownianGUI.h"
#include "BrownianGUIDlg.h"
#include "BrownianGUISettingsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBrownianGUIDlg dialog

CBrownianGUIDlg::CBrownianGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrownianGUIDlg::IDD, pParent)
	, c_pStatus( CEdit() )
	, dim_left(-1000)
	, dim_right(1000)
	, dim_top(1000)
	, dim_bottom(-1000)
	, max_vel_x(666)
	, max_vel_y(666)
	, cal_time(100.)
	, unit_time(1.)
	, RNG_type(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	an_object = new Molecule(0.1);
	Rect_Estimation = new Physics(an_object, cal_time, unit_time, true, RNG_type);

	Rect_Estimation->Seed_Rand();
}

void CBrownianGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REPORT, c_pStatus);
}

BEGIN_MESSAGE_MAP(CBrownianGUIDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDEXIT, &CBrownianGUIDlg::OnBnClickedExit)
	ON_BN_CLICKED(ID_RUN, &CBrownianGUIDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_SAVE, &CBrownianGUIDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SETTINGS, &CBrownianGUIDlg::OnBnClickedSettings)
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
	//c_pStatus.Clear();
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

bool CBrownianGUIDlg::AddTextToStatus(std_str CStrText)
{
	int nTextLength = c_pStatus.GetWindowTextLengthW();
	c_pStatus.SetSel(nTextLength, nTextLength);
	c_pStatus.ReplaceSel(CString(CStrText.c_str()));

	return TRUE;
}

bool CBrownianGUIDlg::AddTextToStatus(const char* CStrText)
{
	int nTextLength = c_pStatus.GetWindowTextLengthW();
	c_pStatus.SetSel(nTextLength, nTextLength);
	c_pStatus.ReplaceSel(CString(CStrText));

	return TRUE;
}

void CBrownianGUIDlg::ClearStatusControl()
{
	c_pStatus.SetSel(0, -1);
	c_pStatus.ReplaceSel(_T(""));
}



//
// Event Handlers
//
void CBrownianGUIDlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
}


void CBrownianGUIDlg::OnBnClickedRun()
{
	// TODO: Add your control notification handler code here
	AddTextToStatus("\n\n");

	ClearStatusControl();

	Rect_Estimation->Reset_Sim();

	//Rect_Estimation->set_dimension_rect(dim_left, dim_right, dim_top, dim_bottom);
	max_vel_x = (abs(dim_left) + abs(dim_right)) / 3.0;
	max_vel_y = (abs(dim_top) + abs(dim_bottom)) / 3.0;

	Rect_Estimation->brownian_rect(max_vel_x, max_vel_y);
	
	//AddTextToStatus("\n");
	//CString dimension_log(Rect_Estimation->show_dimension_rect("\r\n").c_str());
	//AddTextToStatus(dimension_log);
	AddTextToStatus(_T("** Dimension Info **\n"));
	AddTextToStatus(_T("Left: "));
	AddTextToStatus(Converters::numtostdstr(dim_left)+"\n");
	AddTextToStatus(_T("Right: "));
	AddTextToStatus(Converters::numtostdstr(dim_right) + "\n");
	AddTextToStatus(_T("Top: "));
	AddTextToStatus(Converters::numtostdstr(dim_top) + "\n");
	AddTextToStatus(_T("Bottom: "));
	AddTextToStatus(Converters::numtostdstr(dim_bottom) + "\n");

	AddTextToStatus(_T("\n"));
	AddTextToStatus(_T("** Simulation Info **\n"));
	AddTextToStatus(_T("Simulation Time: "));
	AddTextToStatus(Converters::numtostdstr(cal_time*unit_time) + " seconds\n");
	AddTextToStatus(_T("RNG Type: "));
	AddTextToStatus(Rect_Estimation->get_RNG_type() + "\n");
	AddTextToStatus(_T("\n"));
	
	//AddTextToStatus("\n");
	CString status_log(Rect_Estimation->extract_log_rect("\t", "\r\n").c_str());
	AddTextToStatus(status_log);
}

void CBrownianGUIDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CString log_filename(_T("trace.csv"));
	CT2CA pszConvertCStr(log_filename);
	std::string strStd(pszConvertCStr);
	Rect_Estimation->write_log_rect(strStd, ",", "\n");

	AddTextToStatus("\n");
	AddTextToStatus("Log saved to: ");
	AddTextToStatus(log_filename);
}


void CBrownianGUIDlg::OnBnClickedSettings()
{
	// TODO: Add your control notification handler code here
	brownianGUISettingsDlg = new BrownianGUISettingsDlg(NULL, this->Rect_Estimation, \
		&this->dim_left, &this->dim_right, &this->dim_top, &this->dim_bottom, \
		&this->cal_time, &this->unit_time, &this->RNG_type);

	brownianGUISettingsDlg->Create(IDD_SETTINGS_DIALOG, NULL);
	brownianGUISettingsDlg->ShowWindow(SW_SHOW);

}
