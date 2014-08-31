
// BrownianGUIDlg.h : header file
//

#pragma once


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

	double dim_left;
	double dim_right;
	double dim_top;
	double dim_bottom;

	double max_vel_x;
	double max_vel_y;

	bool AddTextToStatus(CString CStrText);
	bool AddTextToStatus(std_str CStrText);
	bool AddTextToStatus(const char* CStrText);
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedRun();
};
