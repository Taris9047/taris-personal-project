// BrownianGUISettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BrownianGUI.h"
#include "BrownianGUISettingsDlg.h"
#include "afxdialogex.h"


// BrownianGUISettingsDlg dialog

IMPLEMENT_DYNAMIC(BrownianGUISettingsDlg, CDialog)

BrownianGUISettingsDlg::BrownianGUISettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BrownianGUISettingsDlg::IDD, pParent)
{

}

/*
BrownianGUISettingsDlg::BrownianGUISettingsDlg(
	double* dim_left, double* dim_right, double* dim_top, double* dim_bottom, \
	double* calc_time, double* unit_time, unsigned int* RNG_type)
	: m_pDim_left(dim_left)
	, m_pDim_right(dim_right)
	, m_pDim_top(dim_top)
	, m_pDim_bottom(dim_bottom)
	, m_pCalc_time(calc_time)
	, m_pUnit_time(unit_time)
	, m_pRNG_type(RNG_type)
{

}
*/

BrownianGUISettingsDlg::~BrownianGUISettingsDlg()
{
}

void BrownianGUISettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LEFT, m_CEditLeft);
	DDX_Control(pDX, IDC_EDIT_RIGHT, m_CEditRight);
	DDX_Control(pDX, IDC_EDIT_TOP, m_CEditTop);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, m_CEditBottom);
	DDX_Control(pDX, IDC_EDIT_CALTIME, m_CEditCalcTime);
	DDX_Control(pDX, IDC_EDIT_UNITTIME, m_CEditUnitTime);
	DDX_Control(pDX, IDC_COMBO_RNGTYPE, m_CComboBoxRNGtype);
}


BEGIN_MESSAGE_MAP(BrownianGUISettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &BrownianGUISettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &BrownianGUISettingsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// Methods
void BrownianGUISettingsDlg::InitSettingsDlg(
	double* dim_left, double* dim_right, double* dim_top, double* dim_bottom, \
	double* calc_time, double* unit_time, unsigned int* RNG_type)
{
	m_pDim_left = dim_left;
	m_pDim_right = dim_right;
	m_pDim_top = dim_top;
	m_pDim_bottom = dim_bottom;

	m_pCalc_time = calc_time;
	m_pUnit_time = unit_time;
	m_pRNG_type = RNG_type;

	SetCEditText(&m_CEditLeft, *m_pDim_left);
	SetCEditText(&m_CEditRight, *m_pDim_right);
}

bool BrownianGUISettingsDlg::SetCEditText(CEdit *CEdt, CString text)
{
	int nTextLength = CEdt->GetWindowTextLengthW();
	CEdt->SetSel(nTextLength, nTextLength);
	CEdt->ReplaceSel(text);
	
	return true;
}

bool BrownianGUISettingsDlg::SetCEditText(CEdit *CEdt, double value)
{
	int nTextLength = CEdt->GetWindowTextLengthW();
	CString text;
	text.Format(L"%f", value);

	CEdt->SetSel(nTextLength, nTextLength);
	CEdt->ReplaceSel(text);

	return true;
}


// BrownianGUISettingsDlg message handlers


void BrownianGUISettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here



	CDialog::OnOK();
}


void BrownianGUISettingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
