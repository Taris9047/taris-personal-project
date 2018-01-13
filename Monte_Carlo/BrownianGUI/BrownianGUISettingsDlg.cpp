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

BrownianGUISettingsDlg::BrownianGUISettingsDlg(CWnd* pParent, Physics* Rect_Phy, \
	double* dim_left, double* dim_right, double* dim_top, double* dim_bottom, \
	double* calc_time, double* unit_time, unsigned int* RNG_type)
	: CDialog(BrownianGUISettingsDlg::IDD, pParent)
	, m_pPhysics(Rect_Phy)
	, m_pDim_left(dim_left)
	, m_pDim_right(dim_right)
	, m_pDim_top(dim_top)
	, m_pDim_bottom(dim_bottom)
	, m_pCalc_time(calc_time)
	, m_pUnit_time(unit_time)
	, m_pRNG_type(RNG_type)
{

}

BrownianGUISettingsDlg::~BrownianGUISettingsDlg()
{
}

// Initializing dialog
BOOL BrownianGUISettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetCEditText(&m_CEditLeft, *m_pDim_left);
	SetCEditText(&m_CEditRight, *m_pDim_right);
	SetCEditText(&m_CEditTop, *m_pDim_top);
	SetCEditText(&m_CEditBottom, *m_pDim_bottom);

	SetCEditText(&m_CEditCalcTime, *m_pCalc_time);
	SetCEditText(&m_CEditUnitTime, *m_pUnit_time);

	this->m_CComboBoxRNGtype.SetCurSel(*m_pRNG_type-1);

	return TRUE;
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
	*m_pDim_left = GetSettingValue(&m_CEditLeft);
	*m_pDim_right = GetSettingValue(&m_CEditRight);
	*m_pDim_top = GetSettingValue(&m_CEditTop);
	*m_pDim_bottom = GetSettingValue(&m_CEditBottom);

	*m_pCalc_time = GetSettingValue(&m_CEditCalcTime);
	*m_pUnit_time = GetSettingValue(&m_CEditUnitTime);

	*m_pRNG_type = m_CComboBoxRNGtype.GetCurSel()+1;

	m_pPhysics->set_dimension_rect(
		*m_pDim_left, *m_pDim_right, *m_pDim_top, *m_pDim_bottom);
	m_pPhysics->set_timing(
		*m_pCalc_time, *m_pUnit_time);
	m_pPhysics->set_parameters(true, *m_pRNG_type);

	//CDialog::OnOK();
	DestroyWindow();
}


void BrownianGUISettingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnCancel();
	DestroyWindow();
}


double BrownianGUISettingsDlg::GetSettingValue(CEdit* CEdt)
{
	CString data_str;
	CEdt->GetWindowText(data_str);
	return _tstof(data_str);
}
