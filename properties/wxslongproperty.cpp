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

#include "wxslongproperty.h"
#include <globals.h>

// Helper macro for fetching variable
#define VALUE   wxsVARIABLE(Object,Offset,long)

wxsLongProperty::wxsLongProperty(const wxString& PGName, const wxString& DataName,long _Offset,long _Default):
    wxsProperty(PGName,DataName),
    Offset(_Offset),
    Default(_Default)
{}


void wxsLongProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    PGRegister(Object,Grid,Grid->AppendIn(Parent,wxIntProperty(GetPGName(),wxPG_LABEL,VALUE)));
}

bool wxsLongProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    VALUE = Grid->GetPropertyValue(Id).GetLong();
    return true;
}

bool wxsLongProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    Grid->SetPropertyValue(Id,VALUE);
    return true;
}

bool wxsLongProperty::XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( !Element )
    {
        VALUE = Default;
        return false;
    }
    const char* Text = Element->GetText();
    if ( !Text )
    {
        VALUE = Default;
        return false;
    }
    VALUE = atol(Text);
    return true;
}

bool wxsLongProperty::XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( VALUE != Default )
    {
        Element->InsertEndChild(TiXmlText(cbU2C(wxString::Format(_T("%d"),VALUE))));
        return true;
    }
    return false;
}

bool wxsLongProperty::PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->GetLong(GetDataName(),VALUE,Default);
}

bool wxsLongProperty::PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->PutLong(GetDataName(),VALUE,Default);
}
