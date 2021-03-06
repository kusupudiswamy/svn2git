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

#include "wxsnewwindowdlg.h"
#include "wxwidgetsres.h"
#include "wxsdialogres.h"
#include "wxsframeres.h"
#include "wxspanelres.h"
#include "../wxsmith.h"
#include "../wxsproject.h"

#include <messagemanager.h>
#include <projectmanager.h>


BEGIN_EVENT_TABLE(wxsNewWindowDlg,wxDialog)
    //(*EventTable(wxsNewWindowDlg)
    EVT_TEXT(ID_TEXTCTRL1,wxsNewWindowDlg::OnClassChanged)
    EVT_TEXT(ID_TEXTCTRL2,wxsNewWindowDlg::OnHeaderChanged)
    EVT_TEXT(ID_TEXTCTRL3,wxsNewWindowDlg::OnSourceChanged)
    EVT_CHECKBOX(ID_CHECKBOX1,wxsNewWindowDlg::OnUseXrcChange)
    EVT_TEXT(ID_TEXTCTRL4,wxsNewWindowDlg::OnXrcChanged)
    //*)
    EVT_BUTTON(wxID_OK,wxsNewWindowDlg::OnCreate)
    EVT_BUTTON(wxID_CANCEL,wxsNewWindowDlg::OnCancel)
END_EVENT_TABLE()

wxsNewWindowDlg::wxsNewWindowDlg(wxWindow* parent,const wxString& ResType,wxsProject* Project):
    m_SourceNotTouched(true),
    m_HeaderNotTouched(true),
    m_XrcNotTouched(true),
    m_BlockText(false),
    m_Type(ResType),
    m_Project(Project)
{
    wxWindowID id = wxID_ANY;

    //(*Initialize(wxsNewWindowDlg)
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxStaticText* StaticText3;
    wxStaticText* StaticText4;

    Create(parent,id,_T(""),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE,_T(""));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL,this,_("Options"));
    FlexGridSizer1 = new wxFlexGridSizer(0,2,5,5);
    FlexGridSizer1->AddGrowableCol(1);
    StaticText1 = new wxStaticText(this,ID_STATICTEXT1,_("Class Name:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT1"));
    m_Class = new wxTextCtrl(this,ID_TEXTCTRL1,_T(""),wxDefaultPosition,wxSize(80,-1),0,wxDefaultValidator,_("ID_TEXTCTRL1"));
    if ( 0 ) m_Class->SetMaxLength(0);
    StaticText2 = new wxStaticText(this,ID_STATICTEXT2,_("Header file:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT2"));
    m_Header = new wxTextCtrl(this,ID_TEXTCTRL2,_T(""),wxDefaultPosition,wxSize(80,-1),0,wxDefaultValidator,_("ID_TEXTCTRL2"));
    if ( 0 ) m_Header->SetMaxLength(0);
    StaticText3 = new wxStaticText(this,ID_STATICTEXT3,_("Source file:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT3"));
    m_Source = new wxTextCtrl(this,ID_TEXTCTRL3,_T(""),wxDefaultPosition,wxSize(80,-1),0,wxDefaultValidator,_("ID_TEXTCTRL3"));
    if ( 0 ) m_Source->SetMaxLength(0);
    m_UseXrc = new wxCheckBox(this,ID_CHECKBOX1,_("Xrc File:"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_CHECKBOX1"));
    m_UseXrc->SetValue(false);
    m_Xrc = new wxTextCtrl(this,ID_TEXTCTRL4,_T(""),wxDefaultPosition,wxSize(80,-1),0,wxDefaultValidator,_("ID_TEXTCTRL4"));
    if ( 0 ) m_Xrc->SetMaxLength(0);
    FlexGridSizer1->Add(StaticText1,0,wxALIGN_CENTER,5);
    FlexGridSizer1->Add(m_Class,0,wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,5);
    FlexGridSizer1->Add(StaticText2,0,wxALIGN_CENTER,5);
    FlexGridSizer1->Add(m_Header,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    FlexGridSizer1->Add(StaticText3,0,wxALIGN_CENTER,5);
    FlexGridSizer1->Add(m_Source,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    FlexGridSizer1->Add(m_UseXrc,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    FlexGridSizer1->Add(m_Xrc,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    StaticText4 = new wxStaticText(this,ID_STATICTEXT4,_("Warning:\nWhen adding resource in Xrc mode,\nthis resource must be manually loaded.\nFor details see wxXmlResource::Load()\nand wxXmlResource::Get()."),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE,_("ID_STATICTEXT4"));
    StaticBoxSizer1->Add(FlexGridSizer1,0,wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    StaticBoxSizer1->Add(StaticText4,1,wxALL|wxALIGN_CENTER,5);
    Custom1 = CreateStdDialogButtonSizer(wxOK|wxCANCEL);
    BoxSizer1->Add(StaticBoxSizer1,0,wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxEXPAND,5);
    BoxSizer1->Add(Custom1,1,wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER|wxEXPAND,4);
    this->SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();
    //*)

    m_BlockText = true;
    wxString StrippedName = ResType.Mid(2);
    wxString ResName = wxString::Format(_("New%s"),StrippedName.c_str());
    m_Class->SetValue(ResName);
    m_Source->SetValue(ResName.Lower()+_T(".cpp"));
    m_Header->SetValue(ResName.Lower()+_T(".h"));
    m_Xrc->SetValue(ResName.Lower()+_T(".xrc"));
    m_Xrc->Disable();
    SetTitle(wxString::Format(_("New %s resource"),ResType.c_str()));
    m_BlockText = false;
}

wxsNewWindowDlg::~wxsNewWindowDlg()
{
}

void wxsNewWindowDlg::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void wxsNewWindowDlg::OnCreate(wxCommandEvent& event)
{
	bool CreateXrc = m_UseXrc->GetValue();
	wxString Class = m_Class->GetValue();
	wxString Src   = m_Source->GetValue();
	wxString Hdr   = m_Header->GetValue();
	wxString Xrc   = CreateXrc ? m_Xrc->GetValue() : _T("");

    cbProject* cbProj = m_Project->GetCBProject();

    // Need to do some checks
    // First - validating name
    // TODO: Do not use fixed language
    if ( !wxsCodeMarks::ValidateIdentifier(wxsCPP,Class) )
    {
        wxMessageBox(_("Invalid class name"));
        return;
    }

    // Second - checking if there's given resoure in current project
    if ( m_Project->FindResource(Class) )
    {
        wxMessageBox(wxString::Format(_("Resource '%s' already exists"),Class.c_str()));
        return;
    }

    // Third - checking if files already exist
    wxString ProjectPrefix = m_Project->GetProjectPath();
    bool GenHeader = true;
    if ( wxFileName::FileExists(ProjectPrefix+Hdr) )
    {
        switch ( wxMessageBox(wxString::Format(
            _("File '%s' already exists. Overwrite it ?"),Hdr.c_str()),
            _("File exists"),
            wxYES_NO|wxCANCEL|wxICON_ERROR) )
        {
            case wxCANCEL: return;
            case wxNO: GenHeader = false; break;
        }
    }

    bool GenSource = true;
    if ( wxFileName::FileExists(ProjectPrefix+Src) )
    {
        switch ( wxMessageBox(wxString::Format(
            _("File '%s' already exists. Overwrite it ?"),Src.c_str()),
            _("File exists"),wxYES_NO|wxCANCEL|wxICON_ERROR) )
        {
            case wxCANCEL: return;
            case wxNO: GenSource = false; break;
        }
    }

    bool GenXRC = CreateXrc;
    if ( wxFileName::FileExists(ProjectPrefix+Xrc) )
    {
        // We will add new resource to existing one creating multi-resource xrc
        GenXRC = false;
    }

    // Creating new resource
    wxString WxsFile = Class + _T(".wxs");
    wxsItemRes* NewResource = NULL;

    if ( m_Type == _T("wxDialog") )
    {
        NewResource = new wxsDialogRes(m_Project);
    }
    else if ( m_Type == _T("wxFrame") )
    {
        NewResource = new wxsFrameRes(m_Project);
    }
    else if ( m_Type == _T("wxPanel") )
    {
        NewResource = new wxsPanelRes(m_Project);
    }
    else
    {
        DBGLOG(_T("wxSmith: Internal error: unknown type when creating resource"));
        EndModal(wxID_CANCEL);
        return;
    }

    // Building new data
    if ( !NewResource->CreateNewResource(Class,Src,GenSource,Hdr,GenHeader,Xrc,GenXRC) )
    {
        delete NewResource;
        DBGLOG(_T("wxSmith: Couldn't generate new resource"));
        EndModal(wxID_CANCEL);
        return;
    }

    // Updating content of resource
    // This is done to allow XRC loader load proper data
    if ( !PrepareResource(NewResource) )
    {
        delete NewResource;
        EndModal(wxID_CANCEL);
        return;
    }

    if ( !m_Project->AddResource(NewResource) )
    {
        DBGLOG(_T("Couldn't add new resource to project"));
        delete NewResource;
        EndModal(wxID_CANCEL);
        return;
    }

    // Adding new files to project
    wxArrayInt Targets;
    Manager::Get()->GetProjectManager()->AddFileToProject(Hdr,cbProj,Targets);
    if (Targets.GetCount() != 0)
    {
        Manager::Get()->GetProjectManager()->AddFileToProject(Src,cbProj,Targets);
    }
    Manager::Get()->GetProjectManager()->RebuildTree();

    // TODO: Rebuild code for new resource
    // Opening editor for this resource
    NewResource->EditOpen();
    EndModal(wxID_OK);
}

void wxsNewWindowDlg::OnClassChanged(wxCommandEvent& event)
{
    if ( m_BlockText ) return;
    m_BlockText = true;
    if ( m_HeaderNotTouched ) m_Header->SetValue((m_Class->GetValue() + _T(".h")).MakeLower());
    if ( m_SourceNotTouched ) m_Source->SetValue((m_Class->GetValue() + _T(".cpp")).MakeLower());
    if ( m_XrcNotTouched ) m_Xrc->SetValue((m_Class->GetValue() + _T(".xrc")).MakeLower());
    m_BlockText = false;
}

void wxsNewWindowDlg::OnSourceChanged(wxCommandEvent& event)
{
    if ( m_BlockText ) return;
    m_BlockText = true;
    m_SourceNotTouched = false;
    m_BlockText = false;
}

void wxsNewWindowDlg::OnHeaderChanged(wxCommandEvent& event)
{
    if ( m_BlockText ) return;
    m_BlockText = true;
    wxFileName FN(m_Header->GetValue());
    FN.SetExt(_T("cpp"));
    if ( m_SourceNotTouched )
    {
        m_Source->SetValue(FN.GetFullPath());
    }
    FN.SetExt(_T("xrc"));
    if ( m_XrcNotTouched )
    {
        m_Xrc->SetValue(FN.GetFullPath());
    }
    m_HeaderNotTouched = false;
    m_BlockText = false;
}

void wxsNewWindowDlg::OnUseXrcChange(wxCommandEvent& event)
{
    m_Xrc->Enable(m_UseXrc->GetValue());
}

void wxsNewWindowDlg::OnXrcChanged(wxCommandEvent& event)
{
    if ( m_BlockText ) return;
    m_BlockText = true;
    m_XrcNotTouched = false;
    m_BlockText = false;
}
