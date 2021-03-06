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

#include "wxsradiobox.h"

namespace
{
    wxsRegisterItem<wxsRadioBox> Reg(_T("RadioBox"),wxsTWidget,_T("Standard"),60);

    WXS_ST_BEGIN(wxsRadioBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxRadioBox")
        WXS_ST(wxRA_SPECIFY_ROWS)
        WXS_ST(wxRA_SPECIFY_COLS)
    // NOTE (cyberkoa##): wxRA_HORIZONTAL & wxRA_VERTICAL is not in HELP file but in wxMSW's XRC
        WXS_ST_MASK(wxRA_HORIZONTAL,wxsSFAll,0,false)
        WXS_ST_MASK(wxRA_VERTICAL,wxsSFAll,0,false)
        WXS_ST_MASK(wxRA_USE_CHECKBOX,wxsSFPALMOS,0,true)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsRadioBoxEvents)
        WXS_EVI(EVT_RADIOBOX,wxEVT_COMMAND_RADIOBOX_SELECTED,wxCommandEvent,Select)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsRadioBox::wxsRadioBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsRadioBoxEvents,
        wxsRadioBoxStyles),
    Label(_("Label")),
    DefaultSelection(-1),
    Dimension(1)
{}


void wxsRadioBox::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            // wxRadioBox does not have Append Function , therefore , have to build a wxString[]
            // to pass in to the ctor
            if ( ArrayChoices.GetCount() > 0 )
            {
                Code<< _T("wxString wxRadioBoxChoices_") << GetVarName()
                    << _T("[") << wxString::Format(_T("%d"),ArrayChoices.GetCount()) << _T("] = \n{\n");
                for ( size_t i = 0; i < ArrayChoices.GetCount(); ++i )
                {
                    Code << _T("\t") << wxsCodeMarks::WxString(wxsCPP,ArrayChoices[i]);
                    if ( i != ArrayChoices.GetCount()-1 ) Code << _T(",");
                    Code << _T("\n");
                }
                Code << _T("};\n");
            }

            if ( Dimension < 1 ) Dimension = 1;
            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxRadioBox(");
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
                << wxString::Format(_T("%d"),ArrayChoices.GetCount()) << _T(",");

            if ( ArrayChoices.GetCount()>0 )
            {
                Code<< _T("wxRadioBoxChoices_") << GetVarName();
            }
            else
            {
                Code << _T("NULL");
            }
            Code<< _T(",")
                << wxString::Format(_T("%d"),Dimension) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            if ( DefaultSelection >= 0 && DefaultSelection < (int)ArrayChoices.GetCount() )
            {
                Code << GetVarName() << _T("->SetSelection(")
                     << wxString::Format(_T("%d"),DefaultSelection) << _T(");\n");
            }

            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsRadioBox::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsRadioBox::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxRadioBox* Preview = new wxRadioBox(Parent,GetId(),Label,Pos(Parent),Size(Parent),ArrayChoices, Dimension, Style());
    if ( DefaultSelection >= 0 && DefaultSelection < (int)ArrayChoices.GetCount() )
    {
        Preview->SetSelection(DefaultSelection);
    }
    return SetupWindow(Preview,Flags);
}

void wxsRadioBox::OnEnumWidgetProperties(long Flags)
{
    WXS_STRING(wxsRadioBox,Label,0,_("Label"),_T("label"),_T(""),true,false)
    WXS_ARRAYSTRING(wxsRadioBox,ArrayChoices,0,_("Choices"),_T("content"),_T("item"))
    WXS_LONG(wxsRadioBox,DefaultSelection,0,_("Default"),_T("default"),0)
    WXS_LONG(wxsRadioBox,Dimension,0,_("Dimension"),_T("dimension"),1)

}

void wxsRadioBox::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/radiobox.h>")); return;
        default: wxsCodeMarks::Unknown(_T("wxsRadioBox::OnEnumDeclFiles"),Language);
    }
}
