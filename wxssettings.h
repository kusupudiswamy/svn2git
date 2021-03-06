#ifndef WXSSETTINGS_H
#define WXSSETTINGS_H

#include <configurationpanel.h>

//(*Headers(wxsSettings)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

class wxsSettings: public cbConfigurationPanel
{
	public:

		wxsSettings(wxWindow* parent,wxWindowID id = -1);
		virtual ~wxsSettings();

	private:

		//(*Identifiers(wxsSettings)
		enum Identifiers
		{
		    ID_STATICTEXT2 = 0x1000,
		    ID_COMBOBOX1,
		    ID_STATICTEXT3,
		    ID_BUTTON1,
		    ID_STATICTEXT4,
		    ID_BUTTON2,
		    ID_STATICTEXT5,
		    ID_RADIOBUTTON1,
		    ID_RADIOBUTTON2,
		    ID_STATICTEXT6,
		    ID_RADIOBUTTON3,
		    ID_RADIOBUTTON4,
		    ID_STATICTEXT7,
		    ID_SPINCTRL2,
		    ID_STATICTEXT8,
		    ID_CHECKBOX1,
		    ID_CHECKBOX2,
		    ID_CHECKBOX3,
		    ID_CHECKBOX4,
		    ID_STATICTEXT9,
		    ID_CHECKBOX5,
		    ID_STATICTEXT10,
		    ID_CHECKBOX6,
		    ID_STATICTEXT12,
		    ID_CHOICE1,
		    ID_STATICTEXT13,
		    ID_SPINCTRL3,
		    ID_CHECKBOX8
		};
		//*)

		//(*Handlers(wxsSettings)
		void OnDragTargetColClick(wxCommandEvent& event);
		void OnDragParentColClick(wxCommandEvent& event);
		//*)

		//(*Declarations(wxsSettings)
		wxFlexGridSizer* FlexGridSizer1;
		wxFlexGridSizer* FlexGridSizer6;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxFlexGridSizer* FlexGridSizer2;
		wxComboBox* DragAssistType;
		wxButton* DragTargetCol;
		wxButton* DragParentCol;
		wxFlexGridSizer* FlexGridSizer3;
		wxRadioButton* Icons16;
		wxRadioButton* Icons32;
		wxStaticText* StaticText6;
		wxFlexGridSizer* FlexGridSizer4;
		wxRadioButton* TIcons16;
		wxRadioButton* TIcons32;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxFlexGridSizer* FlexGridSizer5;
		wxStaticText* StaticText7;
		wxSpinCtrl* spinProportion;
		wxStaticText* StaticText8;
		wxBoxSizer* BoxSizer2;
		wxCheckBox* chkTop;
		wxCheckBox* chkBottom;
		wxCheckBox* chkLeft;
		wxCheckBox* chkRight;
		wxStaticText* StaticText9;
		wxCheckBox* chkExpand;
		wxStaticText* StaticText10;
		wxCheckBox* chkShaped;
		wxStaticText* StaticText12;
		wxChoice* choicePlacement;
		wxStaticText* StaticText13;
		wxSpinCtrl* spinBorder;
		wxCheckBox* chkBorderDU;
		//*)

        wxString GetTitle() const { return _("wxSmith settings"); }
        wxString GetBitmapBaseName() const { return _T("wxsmith"); }

        void OnApply();
        void OnCancel(){}

		DECLARE_EVENT_TABLE()
};

#endif
