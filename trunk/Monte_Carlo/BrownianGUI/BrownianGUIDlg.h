
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
	bool AddTextToStatus(CString CStrText);
};
