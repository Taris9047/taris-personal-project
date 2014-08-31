#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include "wxcrafter.h"

class SettingsDialog : public SettingsDialogBase
{
public:
	double* m_dim_left;
	double* m_dim_right;
	double* m_dim_top;
	double* m_dim_bottom;
	
	double* m_max_vel_x;
	double* m_max_vel_y;

    SettingsDialog(wxWindow* parent);
	SettingsDialog(wxWindow* parent, 
		double* left, double* right, double* top, double* bottom, 
		double* max_vel_x, double* max_vel_y);
    virtual ~SettingsDialog();
protected:
	virtual void SetTextFields();
	
	const char* dtostr(double input);
	double strtod(wxString str);

    virtual void OnBtnApplyClicked(wxCommandEvent& event);
    virtual void OnBtnCloseClicked(wxCommandEvent& event);
};
#endif // SETTINGSDIALOG_H
