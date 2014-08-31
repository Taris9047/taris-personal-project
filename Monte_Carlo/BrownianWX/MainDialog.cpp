#include "MainDialog.h"


MainDialog::MainDialog(wxWindow* parent)
    : MainDialogBaseClass(parent)
{
	dim_left = -1000.;
	dim_right = 1000.;
	dim_top = 1000.;
	dim_bottom = -1000;
	
	velocity_max_x = 500.;
	velocity_max_y = 500.;
	
	m_gaugeCalculation->SetRange(100);
	m_gaugeCalculation->SetValue(0);
}

MainDialog::~MainDialog()
{
}

void MainDialog::OnBtnExitClicked(wxCommandEvent& event)
{
	exit(0);
}

void MainDialog::OnBtnRunClicked(wxCommandEvent& event)
{
	m_gaugeCalculation->SetValue(0);
	
	double time_limit = 1000.;
	double time_scale = 1.;
	std::string brownian_report; 
		
	// Better set dimnesions and velocity here but now... just skip it.
	
	
	Molecule* current_molecule = new Molecule();
	Physics* current_rect_system = 
		new Physics(current_molecule, time_limit, time_scale);
	
	current_rect_system->\
		set_dimension_rect(
			dim_left, dim_right, dim_top, dim_bottom);
	current_rect_system->\
		brownian_rect(velocity_max_x, velocity_max_y);
	brownian_report = \
		current_rect_system->extract_log_rect("\t", "\n");
	
	m_textReport->WriteText("\n");
	m_textReport->WriteText(brownian_report.c_str());
}

void MainDialog::OnBtnSaveClicked(wxCommandEvent& event)
{
	wxFileDialog* saveFileDialog = new wxFileDialog(this, ("Save to File"), "", "", "csv files (*.csv)|*.csv", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	
	if (saveFileDialog->ShowModal() == wxID_CANCEL)
		return;
	
	m_textReport->SaveFile(saveFileDialog->GetPath());
		
}

void MainDialog::OnBtnSettingsClicked(wxCommandEvent& event)
{
	
}

void MainDialog::OnInitMainDialog(wxInitDialogEvent& event)
{
	m_textReport->Remove(0, m_textReport->GetLastPosition());
	m_textReport->WriteText("****** BrownianWX 0.0.1 ******");
}
