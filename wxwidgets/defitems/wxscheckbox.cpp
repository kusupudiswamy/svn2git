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

#include "wxscheckbox.h"

namespace
{
    wxsRegisterItem<wxsCheckBox> Reg(_T("CheckBox"),wxsTWidget,_T("Standard"),60);

    WXS_ST_BEGIN(wxsCheckBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxCheckBox")
        WXS_ST(wxCHK_2STATE)
        WXS_ST(wxALIGN_RIGHT)
        WXS_ST_MASK(wxCHK_3STATE,wxsSFAll,wxsSFMGL |wxsSFGTK12 | wxsSFOS2,true)
        WXS_ST_MASK(wxCHK_ALLOW_3RD_STATE_FOR_USER,wxsSFAll,wxsSFMGL |wxsSFGTK12 | wxsSFOS2,true)
    WXS_ST_END()

    WXS_EV_BEGIN(wxsCheckBoxEvents)
        WXS_EVI(EVT_CHECKBOX,wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEvent,Click)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsCheckBox::wxsCheckBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsCheckBoxEvents,
        wxsCheckBoxStyles),
    Label(_("Label")),
    IsChecked(false)
{}


void wxsCheckBox::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxCheckBox(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,Label) << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            if ( IsChecked ) Code << GetVarName() << _T("->SetValue(")
                             << (IsChecked?_T("true"):_T("false")) << _T(");\n");
            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsCheckBox::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsCheckBox::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxCheckBox* Preview = new wxCheckBox(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    if ( IsChecked ) Preview->SetValue(IsChecked);
    return SetupWindow(Preview,Flags);
}

void wxsCheckBox::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxsCheckBox,Label,0,_("Label"),_T("label"),_T(""),true,false)
    WXS_BOOL  (wxsCheckBox,IsChecked,0,_("Checked"),_T("checked"),false)
}

void wxsCheckBox::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/checkbox.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsCheckBox::OnEnumDeclFiles"),Language);
    }
}
