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
* $Revision:  $
* $Id:  $
* $HeadURL:  $
*/

#include "wxsboolproperty.h"

// Helper macro for fetching variable
#define VALUE   wxsVARIABLE(Object,Offset,bool)

wxsBoolProperty::wxsBoolProperty(const wxString& PGName,const wxString& DataName,long _Offset,bool _Default):
    wxsProperty(PGName,DataName),
    Offset(_Offset),
    Default(_Default)
{}


void wxsBoolProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    wxPGId ID = Grid->AppendIn(Parent,wxBoolProperty(GetPGName(),wxPG_LABEL,VALUE));
    Grid->SetPropertyAttribute(ID,wxPG_BOOL_USE_CHECKBOX,1L,wxRECURSE);
    PGRegister(Object,Grid,ID);
}

bool wxsBoolProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    VALUE = Grid->GetPropertyValue(Id).GetBool();
    return true;
}

bool wxsBoolProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    Grid->SetPropertyValue(Id,VALUE);
    return true;
}

bool wxsBoolProperty::XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( !Element )
    {
        VALUE = Default;
        return false;
    }
    TiXmlText* Text = Element->FirstChild()->ToText();
    if ( !Text )
    {
        VALUE = Default;
        return false;
    }
    VALUE = atoi(Text->Value()) != 0;
    return true;
}

bool wxsBoolProperty::XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( VALUE != Default )
    {
        Element->InsertEndChild(TiXmlText(VALUE?"1":"0"));
        return true;
    }
    return false;
}

bool wxsBoolProperty::PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->GetBool(GetDataName(),VALUE,Default);
}

bool wxsBoolProperty::PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->PutBool(GetDataName(),VALUE,Default);
}
