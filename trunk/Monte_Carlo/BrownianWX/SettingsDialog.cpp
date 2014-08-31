#include "SettingsDialog.h"
#include <sstream>

SettingsDialog::SettingsDialog(wxWindow* parent)
    : SettingsDialogBase(parent)
{
}

SettingsDialog::SettingsDialog(wxWindow* parent, 
	double* left, double* right, double* top, double* bottom, 
	double* max_vel_x, double* max_vel_y)
	: SettingsDialogBase(parent)
{
	m_dim_left = left;
	m_dim_right = right;
	m_dim_top = top;
	m_dim_bottom = bottom;
	m_max_vel_x = max_vel_x;
	m_max_vel_y = max_vel_y;
	
	SetTextFields();
}

const char* SettingsDialog::dtostr(double input)
{
	std::ostringstream o;
	
	if (!(o << input))
		throw "SettingsDialog::dtostr(double input)";
	
	return o.str().c_str();
}

double SettingsDialog::strtod(wxString str)
{
	return atof(str.c_str());
}

SettingsDialog::~SettingsDialog()
{
}


void SettingsDialog::SetTextFields()
{
	m_textCtrlLeft->WriteText(dtostr(*m_dim_left));
	m_textCtrlRight->WriteText(dtostr(*m_dim_right));
	m_textCtrlTop->WriteText(dtostr(*m_dim_top));
	m_textCtrlBottom->WriteText(dtostr(*m_dim_bottom));

	m_textCtrlVelXMax->WriteText(dtostr(*m_max_vel_x));
	m_textCtrlVelYMax->WriteText(dtostr(*m_max_vel_y));
}

void SettingsDialog::OnBtnApplyClicked(wxCommandEvent& event)
{
	*m_dim_left = strtod(m_textCtrlLeft->GetLineText(0));
	
}

void SettingsDialog::OnBtnCloseClicked(wxCommandEvent& event)
{
	this->Close(true);
}
