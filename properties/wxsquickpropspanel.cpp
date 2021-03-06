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

#include "wxsquickpropspanel.h"
#include "wxspropertycontainer.h"

wxsQuickPropsPanel::wxsQuickPropsPanel(wxsPropertyContainer* PC):
    wxPanel(),
    Container(PC)
{
}

wxsQuickPropsPanel::wxsQuickPropsPanel(
    wxWindow* parent,
    wxsPropertyContainer* PC,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name):
        wxPanel(parent,id,pos,size,style,name),
        Container(PC)
{
}

wxsQuickPropsPanel::~wxsQuickPropsPanel()
{
    // Notifying container that quick properties object does no longer exist
    if ( Container )
    {
        Container->CurrentQP = NULL;
        Container = NULL;
    }
}

void wxsQuickPropsPanel::NotifyChange()
{
    if ( Container )
    {
        Container->NotifyPropertyChangeFromQuickProps();
    }
}
