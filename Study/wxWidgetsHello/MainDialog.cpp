#include "MainDialog.h"

MainDialog::MainDialog(wxWindow* parent)
    : MainDialogBaseClass(parent)
{
}

MainDialog::~MainDialog()
{
}

void MainDialog::OnIDEXITClicked(wxCommandEvent& event)
{
	exit(0);
}
void MainDialog::OnIDHELLOClicked(wxCommandEvent& event)
{
	m_staticTextHello->SetLabel("Hello, World!!");
	m_textCtrlHello->Remove(0, m_textCtrlHello->GetLastPosition());
	m_textCtrlHello->WriteText("Hello, World!!!");
}
