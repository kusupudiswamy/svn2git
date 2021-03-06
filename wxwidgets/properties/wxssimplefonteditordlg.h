#ifndef WXSSIMPLEFONTEDITORDLG_H
#define WXSSIMPLEFONTEDITORDLG_H

#include "wxsfontproperty.h"

//(*Headers(wxsSimpleFontEditorDlg)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class wxsSimpleFontEditorDlg: public wxDialog
{
	public:

		wxsSimpleFontEditorDlg(wxWindow* parent,wxsFontData& Data,wxWindowID id = -1);
		virtual ~wxsSimpleFontEditorDlg();

		//(*Identifiers(wxsSimpleFontEditorDlg)
		enum Identifiers
		{
		    ID_STATICTEXT1 = 0x1000,
		    ID_STATICLINE2,
		    ID_BUTTON1,
		    ID_BUTTON3,
		    ID_STATICLINE1,
		    ID_BUTTON2,
		    ID_TEXTCTRL1,
		    ID_STATICTEXT2,
		    ID_STATICTEXT3
		};
		//*)

	private:

		//(*Handlers(wxsSimpleFontEditorDlg)
		void OnOK(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		//*)

		//(*Declarations(wxsSimpleFontEditorDlg)
		wxFlexGridSizer* FlexGridSizer1;
		wxBoxSizer* BoxSizer4;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxFlexGridSizer* FlexGridSizer2;
		wxStaticText* FontDescription;
		wxStaticLine* StaticLine2;
		wxBoxSizer* BoxSizer2;
		wxButton* Button1;
		wxButton* Button3;
		wxStaticLine* StaticLine1;
		wxButton* Button2;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxTextCtrl* TestArea;
		wxBoxSizer* BoxSizer3;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxBoxSizer* BoxSizer1;
		wxStdDialogButtonSizer* StdDialogButtonSizer1;
		//*)

		wxsFontData& m_Data;
		wxsFontData  m_WorkingCopy;

		void UpdateFontDescription();

		DECLARE_EVENT_TABLE()
};

#endif
