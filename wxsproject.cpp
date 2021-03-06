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

#include "wxsproject.h"
#include "wxsmith.h"
#include "wxsresource.h"
#include "wxsresourcefactory.h"
#include "wxsguifactory.h"
#include "wxsgui.h"
#include "wxsversionconverter.h"

#include <wx/string.h>
#include <messagemanager.h>

namespace
{
    const int CurrentVersion = 1;
    const char* CurrentVersionStr = "1";
}

wxsProject::wxsProject(cbProject* Project):
    m_Project(Project),
    m_GUI(NULL),
    m_UnknownConfig("unknown_config"),
    m_UnknownResources("unknown_resource"),
    m_WasModifiedDuringLoad(false)
{
    // Creating resource tree entery for this project
    m_TreeItem = wxsTree()->NewProjectItem(GetCBProject()->GetTitle(),this);

    // Building paths
    wxFileName PathBuilder(Project->GetFilename());
    m_ProjectPath = PathBuilder.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
}

wxsProject::~wxsProject()
{
    delete m_GUI;
    m_GUI = NULL;

    for ( size_t i=m_Resources.Count(); i-->0; )
    {
        delete m_Resources[i];
        m_Resources[i] = NULL;
    }
    m_Resources.Clear();

    wxsTree()->Delete(m_TreeItem);
    wxsTree()->Refresh();
}

void wxsProject::ReadConfiguration(TiXmlElement* element)
{
    TiXmlElement* SmithNode = element->FirstChildElement("wxsmith");
    if ( !SmithNode ) return;

    TiXmlDocument TempDoc;

    // Checking version
    if ( wxsVersionConverter::Get().DetectOldConfig(SmithNode,this) )
    {
        SmithNode = wxsVersionConverter::Get().ConvertFromOldConfig(SmithNode,&TempDoc,this);
        if ( !SmithNode )
        {
            // TODO: Some info about conversion failure
            //       and store full node for later save
            return;
        }
        else
        {
            m_WasModifiedDuringLoad = true;
        }
    }

    const char* VersionStr = SmithNode->Attribute("version");
    int Version = VersionStr ? atoi(VersionStr) : 1;

    if ( Version > CurrentVersion )
    {
        // TODO: Show some dialog box that resources were created by newer version,
        //       store all configuration for later save and return
        return;
    }

    if ( Version < CurrentVersion )
    {
        SmithNode = wxsVersionConverter::Get().Convert(SmithNode,&TempDoc,this);
        if ( !SmithNode )
        {
            // TODO: Show some dialog box that resources were created by newer version,
            //       store all configuration for later save and return
            return;
        }
        else
        {
            m_WasModifiedDuringLoad = true;
        }
    }

    // Iterating through elements
    for ( TiXmlElement* Node = SmithNode->FirstChildElement(); Node; Node=Node->NextSiblingElement() )
    {
        wxString NodeValue = cbC2U(Node->Value());
        if ( NodeValue == _T("gui") )
        {
            wxString GUIName = cbC2U(Node->Attribute("name"));
            wxsGUI* NewGUI = wxsGUIFactory::Build(GUIName,this);
            if ( !NewGUI )
            {
                m_UnknownConfig.InsertEndChild(*Node);
            }
            else
            {
                delete m_GUI;
                m_GUI = NewGUI;
                if ( NewGUI )
                {
                    NewGUI->ReadConfig(Node);
                }
            }
        }
        else if ( NodeValue == _T("resources") )
        {
            for ( TiXmlElement* ResNode = Node->FirstChildElement(); ResNode; ResNode = ResNode->NextSiblingElement() )
            {
                wxString Type = cbC2U(ResNode->Value());
                wxsResource* Res = wxsResourceFactory::Build(Type,this);

                if ( Res )
                {
                    // Storing unknown Xml Element
                    if ( !Res->ReadConfig(ResNode) )
                    {
                        m_UnknownResources.InsertEndChild(*ResNode);
                        delete Res;
                    }
                    else
                    {
                        m_Resources.Add(Res);
                        Res->BuildTreeEntry(GetResourceTypeTreeId(Type));
                    }
                }
                else
                {
                    m_UnknownResources.InsertEndChild(*ResNode);
                }
            }
        }
        else
        {
            m_UnknownConfig.InsertEndChild(*Node);
        }
    }
}

void wxsProject::WriteConfiguration(TiXmlElement* element)
{
    if ( !m_GUI && m_Resources.empty() )
    {
        // Ths project does not use wxSmith at all - we do not store anything
        return;
    }

    TiXmlElement* SmithElement = element->InsertEndChild(TiXmlElement("wxsmith"))->ToElement();
    SmithElement->SetAttribute("version",CurrentVersionStr);

    // saving GUI item
    if ( m_GUI )
    {
        TiXmlElement* GUIElement = SmithElement->InsertEndChild(TiXmlElement("gui"))->ToElement();
        GUIElement->SetAttribute("name",cbU2C(m_GUI->GetName()));
        m_GUI->WriteConfig(GUIElement);
    }

    // saving resources
    if ( !m_Resources.empty() )
    {
        TiXmlElement* ResElement = SmithElement->InsertEndChild(TiXmlElement("resources"))->ToElement();
        size_t Count = m_Resources.Count();
        for ( size_t i=0; i<Count; i++ )
        {
            const wxString& Name = m_Resources[i]->GetResourceName();
            const wxString& Type = m_Resources[i]->GetResourceType();
            TiXmlElement* Element = ResElement->InsertEndChild(TiXmlElement(cbU2C(Type)))->ToElement();
            // TODO: Check value returned from WriteConfig
            m_Resources[i]->WriteConfig(Element);
            Element->SetAttribute("name",cbU2C(Name));
        }

        // Saving all unknown resources
        for ( TiXmlNode* Node = m_UnknownResources.FirstChild(); Node; Node=Node->NextSibling() )
        {
            SmithElement->InsertEndChild(*Node);
        }
    }

    // Saving all unknown configuration nodes
    for ( TiXmlNode* Node = m_UnknownConfig.FirstChild(); Node; Node=Node->NextSibling() )
    {
        SmithElement->InsertEndChild(*Node);
    }

}

bool wxsProject::AddResource(wxsResource* NewResource)
{
    if ( NewResource == NULL )
    {
        return false;
    }

    const wxString& Type = NewResource->GetResourceType();
    const wxString& Name = NewResource->GetResourceName();

    if ( FindResource(Name) != NULL )
    {
        return false;
    }

    m_Resources.Add(NewResource);
    wxsResourceItemId Id = GetResourceTypeTreeId(Type);
    NewResource->BuildTreeEntry(Id);
    m_Project->SetModified(true);
    return true;
}

wxsResource* wxsProject::FindResource(const wxString& Name)
{
    for ( size_t i = m_Resources.Count(); i-->0; )
    {
        if ( m_Resources[i]->GetResourceName() == Name )
        {
            return m_Resources[i];
        }
    }

    return NULL;
}

void wxsProject::Configure()
{
    if ( !m_GUI )
    {
        m_GUI = wxsGUIFactory::SelectNew(_("wxSmith does not manage any GUI for this project.\nPlease select GUI you want to be managed in wxSmith."),this);
        if ( m_GUI )
        {
            NotifyChange();
        }
    }

    if ( m_GUI )
    {
        if ( !m_GUI->CheckIfApplicationManaged() )
        {
            // TODO: Prepare better communicate, consider chancing to cbAnnoyingDiaog
            if ( wxMessageBox(_("wxSmith does not manage this application's source.\n"
                                "Should I create proper bindings?"),_("wxSmith"),wxYES_NO) == wxNO ) return;
            if ( !m_GUI->CreateApplicationBinding() ) return;
        }
        cbConfigurationDialog Dlg(NULL,-1,_("Configuring wxSmith"));
        Dlg.AttachConfigurationPanel(m_GUI->BuildConfigurationPanel(&Dlg));
        Dlg.ShowModal();
    }
}

wxString wxsProject::GetProjectPath()
{
    return m_ProjectPath;
}

bool wxsProject::CanOpenEditor(const wxString& FileName)
{
    for ( size_t i=m_Resources.Count(); i-->0; )
    {
        if ( m_Resources[i]->OnCanHandleFile(FileName) )
        {
            return true;
        }
    }
    return false;
}

bool wxsProject::TryOpenEditor(const wxString& FileName)
{
    for ( size_t i=m_Resources.Count(); i-->0; )
    {
        if ( m_Resources[i]->OnCanHandleFile(FileName) )
        {
            m_Resources[i]->EditOpen();
            return true;
        }
    }
    return false;
}

void wxsProject::NotifyChange()
{
    return m_Project->SetModified(true);
}

wxsResourceItemId wxsProject::GetResourceTypeTreeId(const wxString& Name)
{
    if ( m_ResBrowserIds.find(Name) != m_ResBrowserIds.end() )
    {
        return m_ResBrowserIds[Name];
    }
    return m_ResBrowserIds[Name] = wxsTree()->AppendItem(m_TreeItem,Name);
}

void wxsProject::UpdateName()
{
    wxsResourceTree::Get()->SetItemText(m_TreeItem,GetCBProject()->GetTitle());
}
