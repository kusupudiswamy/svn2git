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

#include "wxsnotebook.h"
#include "../../wxsadvqppchild.h"

//(*Headers(wxsNotebookParentQP)
#include <wx/checkbox.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)


// TODO: Add notebook images
namespace
{
    wxsRegisterItem<wxsNotebook> Reg(_T("Notebook"),wxsTContainer,_T("Standard"),61);

    /** \brief Extra parameters for notebook's children */
    class wxsNotebookExtra: public wxsPropertyContainer
    {
        public:

            wxsNotebookExtra():
                m_Label(_("Page name")),
                m_Selected(false)
            {}

            wxString m_Label;
            bool m_Selected;

        protected:

            virtual void OnEnumProperties(long Flags)
            {
                WXS_STRING(wxsNotebookExtra,m_Label,0,_("Page name"),_T("label"),_T(""),false,false);
                WXS_BOOL(wxsNotebookExtra,m_Selected,0,_("Page selected"),_T("selected"),false);
            }
    };

    /** \brief Inernal Quick properties panel */
    class wxsNotebookParentQP: public wxsAdvQPPChild
    {
        public:

            wxsNotebookParentQP(wxsAdvQPP* parent,wxsNotebookExtra* Extra,wxWindowID id = -1):
                wxsAdvQPPChild(parent,_("Notebook")),
                m_Extra(Extra)
            {
                //(*Initialize(wxsNotebookParentQP)
                Create(parent,id,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL,_T(""));
                FlexGridSizer1 = new wxFlexGridSizer(0,1,0,0);
                StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL,this,_("Label"));
                Label = new wxTextCtrl(this,ID_TEXTCTRL1,_T(""),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_TEXTCTRL1"));
                if ( 0 ) Label->SetMaxLength(0);
                StaticBoxSizer1->Add(Label,0,wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
                StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Selection"));
                Selected = new wxCheckBox(this,ID_CHECKBOX1,_("Selected"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_CHECKBOX1"));
                Selected->SetValue(false);
                StaticBoxSizer2->Add(Selected,1,wxBOTTOM|wxALIGN_CENTER|wxEXPAND,5);
                FlexGridSizer1->Add(StaticBoxSizer1,1,wxLEFT|wxRIGHT|wxALIGN_CENTER|wxEXPAND,5);
                FlexGridSizer1->Add(StaticBoxSizer2,1,wxLEFT|wxRIGHT|wxALIGN_CENTER|wxEXPAND,5);
                this->SetSizer(FlexGridSizer1);
                FlexGridSizer1->Fit(this);
                FlexGridSizer1->SetSizeHints(this);
                //*)
                ReadData();

                Label->Connect(-1,wxEVT_KILL_FOCUS,(wxObjectEventFunction)&wxsNotebookParentQP::OnLabelKillFocus,NULL,this);
            }

            virtual ~wxsNotebookParentQP() {}

        private:

            virtual void Update()
            {
                ReadData();
            }

            void ReadData()
            {
                if ( !GetPropertyContainer() || !m_Extra ) return;
                Label->SetValue(m_Extra->m_Label);
                Selected->SetValue(m_Extra->m_Selected);
            }

            void SaveData()
            {
                if ( !GetPropertyContainer() || !m_Extra ) return;
                m_Extra->m_Label = Label->GetValue();
                m_Extra->m_Selected = Selected->GetValue();
                NotifyChange();
            }

            //(*Identifiers(wxsNotebookParentQP)
            enum Identifiers
            {
                ID_TEXTCTRL1 = 0x1000,
                ID_CHECKBOX1
            };
            //*)

            //(*Handlers(wxsNotebookParentQP)
            void OnLabelText(wxCommandEvent& event);
            void OnLabelKillFocus(wxFocusEvent& event);
            void OnSelectionChange(wxCommandEvent& event);
            //*)

            //(*Declarations(wxsNotebookParentQP)
            wxFlexGridSizer* FlexGridSizer1;
            wxStaticBoxSizer* StaticBoxSizer1;
            wxTextCtrl* Label;
            wxStaticBoxSizer* StaticBoxSizer2;
            wxCheckBox* Selected;
            //*)

            wxsNotebookExtra* m_Extra;

            DECLARE_EVENT_TABLE()
    };

    BEGIN_EVENT_TABLE(wxsNotebookParentQP,wxPanel)
        //(*EventTable(wxsNotebookParentQP)
        EVT_TEXT_ENTER(ID_TEXTCTRL1,wxsNotebookParentQP::OnLabelText)
        EVT_CHECKBOX(ID_CHECKBOX1,wxsNotebookParentQP::OnSelectionChange)
        //*)
    END_EVENT_TABLE()

    void wxsNotebookParentQP::OnLabelText(wxCommandEvent& event)       { SaveData(); }
    void wxsNotebookParentQP::OnLabelKillFocus(wxFocusEvent& event)    { SaveData(); event.Skip(); }
    void wxsNotebookParentQP::OnSelectionChange(wxCommandEvent& event) { SaveData(); }

    WXS_ST_BEGIN(wxsNotebookStyles,_T(""))
        WXS_ST_CATEGORY("wxNotebook")
        WXS_ST(wxNB_DEFAULT)
        WXS_ST(wxNB_LEFT)
        WXS_ST(wxNB_RIGHT)
        WXS_ST(wxNB_TOP)
        WXS_ST(wxNB_BOTTOM)
        WXS_ST_MASK(wxNB_FIXEDWIDTH,wxsSFWin,0,true)
        WXS_ST_MASK(wxNB_MULTILINE,wxsSFWin,0,true)
        WXS_ST_MASK(wxNB_NOPAGETHEME,wxsSFWin,0,true)
        // NOTE (cyberkoa##): wxNB_FLAT is in HELP (WinCE only) file but not in wxMSW's XRC
        WXS_ST_MASK(wxNB_FLAT,wxsSFWinCE,0,true)
    WXS_ST_END()

    WXS_EV_BEGIN(wxsNotebookEvents)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGED,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,wxNotebookEvent,PageChanged)
        WXS_EVI(EVT_NOTEBOOK_PAGE_CHANGING,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,wxNotebookEvent,PageChanging)
    WXS_EV_END()

}


wxsNotebook::wxsNotebook(wxsItemResData* Data):
    wxsContainer(
        Data,
        &Reg.Info,
        wxsNotebookEvents,
        wxsNotebookStyles),
    m_CurrentSelection(0)
{
}

void wxsNotebook::OnEnumContainerProperties(long Flags)
{
}

bool wxsNotebook::OnCanAddChild(wxsItem* Item,bool ShowMessage)
{
    if ( Item->GetType() == wxsTSizer )
    {
        if ( ShowMessage )
        {
            wxMessageBox(_("Can not add sizer into Notebook.\nAdd panels first"));
        }
        return false;
    }

	return wxsContainer::OnCanAddChild(Item,ShowMessage);
}

wxsPropertyContainer* wxsNotebook::OnBuildExtra()
{
    return new wxsNotebookExtra();
}

wxString wxsNotebook::OnXmlGetExtraObjectClass()
{
    return _T("notebookpage");
}

void wxsNotebook::OnAddChildQPP(wxsItem* Child,wxsAdvQPP* QPP)
{
    wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(GetChildIndex(Child));
    if ( Extra )
    {
        QPP->Register(new wxsNotebookParentQP(QPP,Extra),_("Notobook"));
    }
}

wxObject* wxsNotebook::OnBuildPreview(wxWindow* Parent,long PreviewFlags)
{
    UpdateCurrentSelection();
	wxNotebook* Notebook = new wxNotebook(Parent,-1,Pos(Parent),Size(Parent),Style());

	if ( !GetChildCount() && !(PreviewFlags&pfExact) )
	{
	    // Adding additional empty notebook to prevent from having zero-sized notebook
	    Notebook->AddPage(
            new wxPanel(Notebook,-1,wxDefaultPosition,wxSize(50,50)),
            _("No pages"));
	}

	AddChildrenPreview(Notebook,PreviewFlags);

	for ( int i=0; i<GetChildCount(); i++ )
	{
	    wxsItem* Child = GetChild(i);
	    wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(i);

	    wxWindow* ChildPreview = wxDynamicCast(GetChild(i)->GetLastPreview(),wxWindow);
	    if ( !ChildPreview ) continue;

	    bool Selected = (Child == m_CurrentSelection);
	    if ( PreviewFlags & pfExact ) Selected = Extra->m_Selected;

	    Notebook->AddPage(ChildPreview,Extra->m_Label,Selected);
	}

	return Notebook;
}

void wxsNotebook::OnBuildCreatingCode(wxString& Code,const wxString& WindowParent,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP:
        {
            if ( GetParent() )
            {
                Code << GetVarName() << _T(" = new wxNotebook(");
            }
            else
            {
                Code << _T("Create(");
            }
            Code << WindowParent << _T(",")
                 << GetIdName() << _T(",")
                 << PosCode(WindowParent,wxsCPP) << _T(",")
                 << SizeCode(WindowParent,wxsCPP) << _T(",")
                 << StyleCode(wxsCPP) << _T(",")
                 << wxsCodeMarks::WxString(wxsCPP,GetIdName(),false) << _T(");\n");

            AddChildrenCode(Code,wxsCPP);

            for ( int i=0; i<GetChildCount(); i++ )
            {
                wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(i);
                Code << GetVarName() << _T("->AddPage(")
                     << GetChild(i)->GetVarName() << _T(",")
                     << wxsCodeMarks::WxString(wxsCPP,Extra->m_Label) << _T(",")
                     << (Extra->m_Selected ? _T("true") : _T("false")) << _T(");\n");
            }

            break;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsNotebook::OnBuildCreatingCode"),Language);
        }
    }
}

void wxsNotebook::OnEnumDeclFiles(wxArrayString& Decl,wxArrayString& Def,wxsCodingLang Language)
{
    switch ( Language )
    {
        case wxsCPP: Decl.Add(_T("<wx/notebook.h>")); break;
        default: wxsCodeMarks::Unknown(_T("wxsNotebook::OnEnumDeclFiles"),Language);
    }
}

bool wxsNotebook::OnMouseClick(wxWindow* Preview,int PosX,int PosY)
{
    UpdateCurrentSelection();
    wxNotebook* Notebook = (wxNotebook*)Preview;
    int Hit = Notebook->HitTest(wxPoint(PosX,PosY));
    if ( Hit != wxNOT_FOUND )
    {
        wxsItem* OldSel = m_CurrentSelection;
        m_CurrentSelection = GetChild(Hit);
        return OldSel != m_CurrentSelection;
    }
    return false;
}

bool wxsNotebook::OnIsChildPreviewVisible(wxsItem* Child)
{
    UpdateCurrentSelection();
    return Child == m_CurrentSelection;
}

bool wxsNotebook::OnEnsureChildPreviewVisible(wxsItem* Child)
{
    if ( IsChildPreviewVisible(Child) ) return false;
    m_CurrentSelection = Child;
    UpdateCurrentSelection();
    return true;
}

void wxsNotebook::UpdateCurrentSelection()
{
    wxsItem* NewCurrentSelection = NULL;
    for ( int i=0; i<GetChildCount(); i++ )
    {
        if ( m_CurrentSelection == GetChild(i) ) return;
        wxsNotebookExtra* Extra = (wxsNotebookExtra*)GetChildExtra(i);
        if ( (i==0) || Extra->m_Selected )
        {
            NewCurrentSelection = GetChild(i);
        }
    }
    m_CurrentSelection = NewCurrentSelection;
}
