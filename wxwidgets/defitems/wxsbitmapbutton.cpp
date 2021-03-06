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

#include "wxsbitmapbutton.h"

namespace
{
    wxsRegisterItem<wxsBitmapButton> Reg(_T("BitmapButton"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsBitmapButtonStyles,_T("wxBU_AUTODRAW"))
        WXS_ST_CATEGORY("wxBitmapButton")
        WXS_ST_MASK(wxBU_LEFT,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_TOP,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_RIGHT,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_BOTTOM,wxsSFWin,0,true)
        WXS_ST_MASK(wxBU_AUTODRAW,wxsSFWin,0,true)
        // cyberkoa: "The help mentions that wxBU_EXACTFIX is not used but the XRC code yes
        //  WXS_ST(wxBU_EXACTFIX)
    WXS_ST_END()


    WXS_EV_BEGIN(wxsBitmapButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
        WXS_EV_DEFAULTS()
    WXS_EV_END()
}

wxsBitmapButton::wxsBitmapButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsBitmapButtonEvents,
        wxsBitmapButtonStyles)
{}

void wxsBitmapButton::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            wxString BmpCode = BitmapLabel.IsEmpty() ? _T("wxNullBitmap") : BitmapLabel.BuildCode(true,_T(""),wxsCPP,wxART_BUTTON);

            if ( GetParent() )
            {
                Code<< GetVarName() << _T(" = new wxBitmapButton(");
            }
            else
            {
                Code<< _T("Create(");
            }
            Code<< WindowParent << _T(",")
                << GetIdName() << _T(",")
                << BmpCode << _T(",")
                << PosCode(WindowParent,wxsCPP) << _T(",")
                << SizeCode(WindowParent,wxsCPP) << _T(",")
                << StyleCode(wxsCPP) << _T(",")
                << _T("wxDefaultValidator") << _T(",")
                << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            if ( !BitmapDisabled.IsEmpty() )
            {
                BmpCode = BitmapDisabled.BuildCode(true,_T(""),wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapDisabled(") << BmpCode << _T(");\n");
            }
            if ( !BitmapSelected.IsEmpty() )
            {
                BmpCode = BitmapSelected.BuildCode(true,_T(""),wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapSelected(") << BmpCode << _T(");\n");
            }
            if ( !BitmapSelected.IsEmpty() )
            {
                BmpCode = BitmapFocus.BuildCode(true,_T(""),wxsCPP,wxART_OTHER);
                Code << GetVarName() << _T("->SetBitmapFocus(") << BmpCode << _T(");\n");
            }

            if ( IsDefault )
            {
                Code << GetVarName() << _T("->SetDefault();\n");
            }
            SetupWindowCode(Code,Language);
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBitmapButton::OnBuildCreatingCode"),Language);
        }
    }
}


wxObject* wxsBitmapButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxBitmapButton* Preview = new wxBitmapButton(Parent,GetId(),BitmapLabel.GetPreview(wxDefaultSize),Pos(Parent),Size(Parent),Style());

    if ( !BitmapDisabled.IsEmpty() )
    {
        Preview->SetBitmapDisabled(BitmapDisabled.GetPreview(wxDefaultSize));
    }

    if ( !BitmapSelected.IsEmpty() )
    {
        Preview->SetBitmapSelected(BitmapSelected.GetPreview(wxDefaultSize));
    }

    if ( !BitmapFocus.IsEmpty() )
    {
        Preview->SetBitmapFocus(BitmapFocus.GetPreview(wxDefaultSize));
    }

    if ( IsDefault )
    {
        Preview->SetDefault();
    }
    return SetupWindow(Preview,Flags);
}


void wxsBitmapButton::OnEnumWidgetProperties(long Flags)
{
    WXS_BITMAP(wxsBitmapButton,BitmapLabel,0,_("Bitmap"),_T("bitmap"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapDisabled,0,_("Disabled bmp."),_T("disabled"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapSelected,0,_("Pressed bmp."),_T("selected"),_T("wxART_OTHER"))
    WXS_BITMAP(wxsBitmapButton,BitmapFocus,0,_("Focused bmp."),_T("focus"),_T("wxART_OTHER"))
    WXS_BOOL  (wxsBitmapButton,IsDefault,0,_("Is default"),_T("default"),false)
}

void wxsBitmapButton::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            Decl.Add(_T("<wx/bmpbuttn.h>"));
            Def.Add(_T("<wx/bitmap.h>"));
            Def.Add(_T("<wx/image.h>"));
            Def.Add(_T("<wx/artprov.h>"));
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBitmapButtonn::OnEnumDeclFiles"),Language);
        }
    }
}
