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

#include "wxsitemfactory.h"
#include "wxsitem.h"

wxsItem* wxsItemFactory::Build(const wxString& Name,wxsItemResData* Data)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return NULL;
    return it->second->OnBuild(Data);
}

const wxsItemInfo* wxsItemFactory::GetInfo(const wxString& Name)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return NULL;
    return it->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetFirstInfo()
{
    m_Iter = ItemMap().begin();
    return (m_Iter==ItemMap().end()) ? NULL : m_Iter->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetNextInfo()
{
    if ( m_Iter==ItemMap().end() ) return NULL;
    ++m_Iter;
    return (m_Iter==ItemMap().end()) ? NULL : m_Iter->second->m_Info;
}

wxsItemFactory::wxsItemFactory(const wxsItemInfo* Info):
    m_Info(Info)
{
    if ( Info==NULL ) return;
    ItemMap()[Info->ClassName] = this;
}

wxsItemFactory::wxsItemFactory(const wxsItemInfo* Info,const wxString& ClassName):
    m_Info(Info)
{
    if ( Info==NULL ) return;
    ItemMap()[ClassName] = this;
}

wxsItemFactory::~wxsItemFactory()
{
    if ( !m_Info ) return;
    ItemMapT::iterator it = ItemMap().find(m_Info->ClassName);
    if ( it == ItemMap().end() ) return;
    if ( it->second!=this ) return;
    ItemMap().erase(it);
}

wxsItemFactory::ItemMapT& wxsItemFactory::ItemMap()
{
    static ItemMapT Map;
    return Map;
}

wxsItemFactory::ItemMapT::iterator wxsItemFactory::m_Iter(wxsItemFactory::ItemMap().end());
