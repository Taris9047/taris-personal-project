//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef WXCRAFTER_BASE_CLASSES_H
#define WXCRAFTER_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/button.h>

class MainDialogBaseClass : public wxDialog
{
public:
    enum {
        wxID_HELLO = 1001,
    };
protected:
    wxStaticText* m_staticTextHello;
    wxTextCtrl* m_textCtrlHello;
    wxCheckBox* m_checkBoxHello;
    wxStaticLine* m_staticLine15;
    wxButton* m_buttonHello;
    wxButton* m_buttonEXIT;

protected:
    virtual void OnIDHELLOClicked(wxCommandEvent& event) { event.Skip(); }
    virtual void OnIDEXITClicked(wxCommandEvent& event) { event.Skip(); }

public:
    MainDialogBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WxHello"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,200), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~MainDialogBaseClass();
};

#endif
