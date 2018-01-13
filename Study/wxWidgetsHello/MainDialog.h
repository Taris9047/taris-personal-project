#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include "wxcrafter.h"

class MainDialog : public MainDialogBaseClass
{
public:
    MainDialog(wxWindow* parent);
    virtual ~MainDialog();
protected:
    virtual void OnIDEXITClicked(wxCommandEvent& event);
    virtual void OnIDHELLOClicked(wxCommandEvent& event);
};
#endif // MAINDIALOG_H
