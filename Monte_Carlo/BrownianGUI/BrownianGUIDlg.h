
// BrownianGUIDlg.h : header file
//

#pragma once

#include "BrownianGUISettingsDlg.h"

// CBrownianGUIDlg dialog
class CBrownianGUIDlg : public CDialog
{
// Construction
public:
	CBrownianGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BROWNIANGUI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit c_pStatus;

	BrownianGUISettingsDlg* brownianGUISettingsDlg;

	double dim_left;
	double dim_right;
	double dim_top;
	double dim_bottom;

	double max_vel_x;
	double max_vel_y;

	double cal_time;
	double unit_time;

	unsigned int RNG_type;

	Molecule* an_object;
	Physics* Rect_Estimation;

	bool AddTextToStatus(CString CStrText);
	bool AddTextToStatus(std_str CStrText);
	bool AddTextToStatus(const char* CStrText);
	void ClearStatusControl();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSettings();
};
