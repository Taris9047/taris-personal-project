#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include "wxcrafter.h"
#include "SettingsDialog.h"
#include "Physics.hpp"
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/log.h>

class MainDialog : public MainDialogBaseClass
{
public:
    MainDialog(wxWindow* parent);
    virtual ~MainDialog();

	double dim_left;
	double dim_right;
	double dim_top;
	double dim_bottom;
	
	double velocity_max_x;
	double velocity_max_y;

protected:
    virtual void OnInitMainDialog(wxInitDialogEvent& event);
    virtual void OnBtnExitClicked(wxCommandEvent& event);
    virtual void OnBtnRunClicked(wxCommandEvent& event);
    virtual void OnBtnSaveClicked(wxCommandEvent& event);
    virtual void OnBtnSettingsClicked(wxCommandEvent& event);
};
#endif // MAINDIALOG_H
