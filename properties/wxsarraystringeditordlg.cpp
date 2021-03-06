/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "wxsarraystringeditordlg.h"

#include <wx/tokenzr.h>

BEGIN_EVENT_TABLE(wxsArrayStringEditorDlg,wxDialog)
	//(*EventTable(wxsArrayStringEditorDlg)
	EVT_BUTTON(wxID_OK,wxsArrayStringEditorDlg::OnOK)
	EVT_BUTTON(wxID_CANCEL,wxsArrayStringEditorDlg::OnCancel)
	//*)
END_EVENT_TABLE()

wxsArrayStringEditorDlg::wxsArrayStringEditorDlg(wxWindow* parent,wxArrayString& _Data,wxWindowID id):
    Data(_Data)
{
	//(*Initialize(wxsArrayStringEditorDlg)
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxButton* Button1;
	wxButton* Button2;

	Create(parent,id,_("Edit items"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Enter items (one item per line)"));
	Items = new wxTextCtrl(this,ID_TEXTCTRL1,_T(""),wxDefaultPosition,wxSize(350,200),wxTE_MULTILINE);
	if ( 0 ) Items->SetMaxLength(0);
	StaticBoxSizer1->Add(Items,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER,2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	Button1 = new wxButton(this,wxID_OK,_("OK"),wxDefaultPosition,wxDefaultSize,0);
	if (true) Button1->SetDefault();
	Button2 = new wxButton(this,wxID_CANCEL,_("Cancel"),wxDefaultPosition,wxDefaultSize,0);
	if (false) Button2->SetDefault();
	BoxSizer2->Add(Button1,1,wxLEFT|wxTOP|wxBOTTOM|wxALIGN_CENTER,5);
	BoxSizer2->Add(Button2,1,wxALL|wxALIGN_CENTER,5);
	BoxSizer1->Add(StaticBoxSizer1,1,wxALL|wxALIGN_CENTER,5);
	BoxSizer1->Add(BoxSizer2,0,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,5);
	this->SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)

	size_t Count = Data.Count();
	for ( size_t i = 0; i<Count; i++ )
	{
	    Items->AppendText(Data[i]);
	    Items->AppendText(_T('\n'));
	}
}

wxsArrayStringEditorDlg::~wxsArrayStringEditorDlg()
{
}


void wxsArrayStringEditorDlg::OnOK(wxCommandEvent& event)
{
    wxStringTokenizer Tknz(Items->GetValue(),_T("\n"),wxTOKEN_RET_EMPTY);
    Data.Clear();

    while ( Tknz.HasMoreTokens() )
    {
        wxString Line = Tknz.GetNextToken();
        Data.Add(Line);
    }

    EndModal(wxID_OK);
}
