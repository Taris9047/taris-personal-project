#pragma once
#include "afxwin.h"


// BrownianGUISettingsDlg dialog

class BrownianGUISettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(BrownianGUISettingsDlg)

public:
	BrownianGUISettingsDlg(CWnd* pParent = NULL);   // standard constructor
	BrownianGUISettingsDlg(CWnd* pParent, Physics* Rect_Phy, \
		double* dim_left, double* dim_right, double* dim_top, double* dim_bottom, \
		double* calc_time, double* unit_time, unsigned int* RNG_type);

	virtual ~BrownianGUISettingsDlg();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	Physics* m_pPhysics;

	double* m_pDim_left;
	double* m_pDim_right;
	double* m_pDim_top;
	double* m_pDim_bottom;

	double* m_pCalc_time;
	double* m_pUnit_time;
	unsigned int* m_pRNG_type;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_CEditLeft;
	CEdit m_CEditRight;
	CEdit m_CEditTop;
	CEdit m_CEditBottom;
	CEdit m_CEditCalcTime;
	CEdit m_CEditUnitTime;
	CComboBox m_CComboBoxRNGtype;

	bool SetCEditText(CEdit *CEdt, CString text); // Write text to CEdit control
	bool SetCEditText(CEdit *CEdt, double value); // Write text to CEdit control with double to CString conversion.
	double GetSettingValue(CEdit* CEdt);
};
