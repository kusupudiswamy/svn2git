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

#include "wxsitemeditor.h"
#include "wxsitemeditorcontent.h"
#include "wxsitemfactory.h"
//#include "wxsbaseproperties.h"
#include "wxsitemresdata.h"
#include "wxsitem.h"
#include "wxsparent.h"
#include "../wxsproject.h"

namespace
{
    const long wxsInsIntoId    = wxNewId();
    const long wxsInsBeforeId  = wxNewId();
    const long wxsInsAfterId   = wxNewId();
    const long wxsDelId        = wxNewId();
    const long wxsPreviewId    = wxNewId();
    const long wxsQuickPropsId = wxNewId();

    inline int ToolIconSize() { return Manager::Get()->GetConfigManager(_T("wxsmith"))->ReadInt(_T("/tooliconsize"),32L); }
    inline int PalIconSize()  { return Manager::Get()->GetConfigManager(_T("wxsmith"))->ReadInt(_T("/paletteiconsize"),16L); }
}

wxsItemEditor::wxsItemEditor(wxWindow* parent,wxsItemRes* Resource):
    wxsEditor(parent,wxEmptyString,Resource),
    m_Data(NULL),
    m_Content(NULL),
    m_WidgetsSet(NULL),
    m_VertSizer(NULL),
    m_HorizSizer(NULL),
    m_QPSizer(NULL),
    m_OpsSizer(NULL),
    m_QPArea(NULL),
    m_InsIntoBtn(NULL),
    m_InsBeforeBtn(NULL),
    m_InsAfterBtn(NULL),
    m_DelBtn(NULL),
    m_PreviewBtn(NULL),
    m_QuickPanelBtn(NULL),
    m_TopPreview(NULL),
    m_PreviewBackground(NULL),
    m_InsType(itBefore),
    m_InsTypeMask(itBefore),
    m_QuickPropsOpen(false)
{
    InitializeResourceData();
    InitializeVisualStuff();
    m_AllEditors.insert(this);
}

wxsItemEditor::~wxsItemEditor()
{
    delete m_Data;
	m_AllEditors.erase(this);
}

void wxsItemEditor::InitializeResourceData()
{
    wxString ProjectPath = GetResource()->GetProjectPath();

    m_Data = new wxsItemResData(
        ProjectPath + GetWxsFileName(),
        ProjectPath + GetSrcFileName(),
        ProjectPath + GetHdrFileName(),
        GetXrcFileName().empty() ? _T("") : ProjectPath + GetXrcFileName(),
        GetResource()->GetResourceName(),
        GetResource()->GetResourceType(),
        GetResource()->GetLanguage(),
        GetResource()->GetTreeItemId(),
        this,
        (wxsItemRes*)GetResource() );

    if ( !m_Data->IsOk() )
    {
        // TODO: Some communicate (couldn't load resource) ?
    }

    if ( GetItemRes()->GetEditMode() == wxsItemRes::File )
    {
        InitFilename(GetXrcFileName());
        SetTitle(m_Shortname);
    }
    else
    {
        InitFilename(GetWxsFileName());
        SetTitle(m_Shortname);
    }

}

void wxsItemEditor::InitializeVisualStuff()
{
    // Loading images if needed
    InitializeImages();

    // Creating content of editor window
    m_VertSizer = new wxBoxSizer(wxVERTICAL);
    m_WidgetsSet = new wxNotebook(this,-1);
    BuildPalette(m_WidgetsSet);
    m_HorizSizer = new wxBoxSizer(wxHORIZONTAL);
    m_VertSizer->Add(m_HorizSizer,1,wxEXPAND);
    m_VertSizer->Add(m_WidgetsSet,0,wxEXPAND);

    m_Content = new wxsItemEditorContent(this,m_Data,this);
    m_HorizSizer->Add(m_Content,1,wxEXPAND);

    m_QPArea = new wxScrolledWindow(this,-1,wxDefaultPosition,wxDefaultSize,wxVSCROLL|wxSUNKEN_BORDER|wxALWAYS_SHOW_SB);
    m_QPArea->SetScrollbars(0,5,0,0);
    m_HorizSizer->Add(m_QPArea,0,wxEXPAND);
    m_QPSizer = new wxBoxSizer(wxVERTICAL);
    m_QPArea->SetSizer(m_QPSizer);

    m_OpsSizer = new wxBoxSizer(wxVERTICAL);
    m_HorizSizer->Add(m_OpsSizer,0,wxEXPAND);

    m_OpsSizer->Add(m_InsIntoBtn   = new wxBitmapButton(this,wxsInsIntoId,m_InsIntoImg));
    m_OpsSizer->Add(m_InsBeforeBtn = new wxBitmapButton(this,wxsInsBeforeId,m_InsBeforeImg));
    m_OpsSizer->Add(m_InsAfterBtn  = new wxBitmapButton(this,wxsInsAfterId,m_InsAfterImg));
    m_OpsSizer->Add(1,5);
    m_OpsSizer->Add(m_DelBtn       = new wxBitmapButton(this,wxsDelId,m_DelImg));
    m_OpsSizer->Add(m_PreviewBtn   = new wxBitmapButton(this,wxsPreviewId,m_PreviewImg));
    m_OpsSizer->Add(1,5);
    m_OpsSizer->Add(m_QuickPanelBtn = new wxBitmapButton(this,wxsQuickPropsId,m_QuickPropsImgOpen));
    m_InsIntoBtn   ->SetToolTip(_("Insert new widgets into current selection"));
    m_InsBeforeBtn ->SetToolTip(_("Insert new widgets before current selection"));
    m_InsAfterBtn  ->SetToolTip(_("Insert new widgets after current selection"));
    m_DelBtn       ->SetToolTip(_("Delete current selection"));
    m_PreviewBtn   ->SetToolTip(_("Show preview"));
    m_QuickPanelBtn->SetToolTip(_("Open / Close Quick Properties panel"));
    SetSizer(m_VertSizer);

    SetInsertionTypeMask(0);
    ToggleQuickPropsPanel(false);       // TODO: Shouldn't store initial state of panel somewhere?

    RebuildPreview();
    UpdateSelection();
}

void wxsItemEditor::ReloadImages()
{
    m_ImagesLoaded = false;
    InitializeImages();
    for ( WindowSet::iterator i=m_AllEditors.begin(); i!=m_AllEditors.end(); ++i )
    {
        (*i)->RebuildIcons();
    }
}

void wxsItemEditor::RebuildPreview()
{
    // Checking if we've already initialized visual stuff
    if ( !m_Content ) return;

    m_Content->BeforePreviewChanged();

    Freeze();

    // If there's previous preview, deleting it
    if ( m_PreviewBackground )
    {
        m_Content->SetSizer(NULL);
        delete m_PreviewBackground;
        m_TopPreview = NULL;
        m_PreviewBackground = NULL;
    }

    // Generating preview
    m_PreviewBackground = new wxPanel(m_Content,-1,wxDefaultPosition,wxDefaultSize,wxRAISED_BORDER);
    wxObject* TopPreviewObject = m_Data->GetRootItem()->BuildPreview(m_PreviewBackground,0);
    m_TopPreview = wxDynamicCast(TopPreviewObject,wxWindow);
    if ( !m_TopPreview )
    {
        DBGLOG(_T("One of root items returned class not derived from wxWindow"));
        delete TopPreviewObject;
        delete m_PreviewBackground;
    }
    else
    {
        wxSizer* BackgroundSizer = new wxBoxSizer(wxHORIZONTAL);
        BackgroundSizer->Add(m_TopPreview,0,0,0);
        m_PreviewBackground->SetSizer(BackgroundSizer);
        BackgroundSizer->Fit(m_PreviewBackground);
        wxSizer* NewSizer = new wxGridSizer(1);
        NewSizer->Add(m_PreviewBackground,0,wxALL,10);
        m_Content->SetVirtualSizeHints(1,1);
        m_Content->SetSizer(NewSizer);
        NewSizer->SetVirtualSizeHints(m_Content);
        NewSizer->FitInside(m_Content);
        m_HorizSizer->Layout();
        m_VertSizer->Layout();
        m_PreviewBackground->Layout();
        Layout();
    }

    Thaw();
    Update();

    // Updating all informations in Content
    m_Content->AfterPreviewChanged();
}

void wxsItemEditor::UpdateSelection()
{
    // Checking if we've already initialized visual stuff
    if ( !m_Content ) return;

    // Updating drag point data
    m_Content->RefreshSelection();

    // Updating insertion type mask
    wxsItem* Item = m_Data->GetRootSelection();
    int itMask = 0;
    if ( Item )
    {
        if ( Item->GetParent() )
        {
            // When sizer is added into non-sizer parent, no other items can be added to
            // this parent
            if ( Item->GetType() != wxsTSizer ||
                 Item->GetParent()->GetType() == wxsTSizer )
            {
                itMask |= itBefore | itAfter;
            }
        }

        if ( Item->ConvertToParent() )
        {
            itMask |= itInto;
        }
    }

    SetInsertionTypeMask(itMask);
    RebuildQuickProps(Item);

    // TODO: Refresh set of available items inside palette
}

bool wxsItemEditor::Save()
{
    if ( !m_Data->Save() )
    {
        // TODO: Some message here please
    }
    UpdateModified();
	return true;
}

bool wxsItemEditor::GetModified() const
{
	return m_Data ? m_Data->GetModified() : false;
}

void wxsItemEditor::UpdateModified()
{
    if ( m_Data && m_Data->GetModified() )
    {
        SetTitle(_T("*") + GetShortName());
    }
    else
    {
        SetTitle(GetShortName());
    }
}

bool wxsItemEditor::CanUndo() const
{
	return m_Data ? m_Data->CanUndo() : false;
}

bool wxsItemEditor::CanRedo() const
{
	return m_Data ? m_Data->CanRedo() : false;
}

void wxsItemEditor::Undo()
{
    if ( m_Data ) m_Data->Undo();
}

void wxsItemEditor::Redo()
{
    if ( m_Data ) m_Data->Redo();
}

bool wxsItemEditor::HasSelection() const
{
    return m_Data ? m_Data->AnySelected() : false;
}

bool wxsItemEditor::CanPaste() const
{
    return m_Data ? m_Data->CanPaste() : false;
}

void wxsItemEditor::Cut()
{
    if ( m_Data ) m_Data->Cut();
}

void wxsItemEditor::Copy()
{
    if ( m_Data ) m_Data->Copy();
}

void wxsItemEditor::Paste()
{
    if ( m_Data ) m_Data->Paste();
}

void wxsItemEditor::InsertRequest(const wxString& Name)
{
    wxsItem* Reference = GetReferenceItem(m_InsType);
    if ( !Reference )
    {
        DBGLOG(_T("wxSmith: No item selected - couldn't create new item"));
        return;
    }

    wxsItem* New = wxsItemFactory::Build(Name,m_Data);
    if ( !New )
    {
        DBGLOG(_T("wxSmith: Culdn't generate item inside factory"));
        return;
    }

    m_Data->BeginChange();
    wxsParent* Parent = Reference->GetParent();
    int RefIndex = Parent ? Parent->GetChildIndex(Reference) : -1;

    switch ( m_InsType )
    {
        case itAfter:
            RefIndex++;
            // We don't break here - continuing on itBefore code

        case itBefore:
            if ( Parent )
            {
                if ( m_Data->InsertNew(New,Parent,RefIndex) )
                {
                    m_Data->SelectItem(New,true);
                }
            }
            else
            {
                delete New;
            }
            break;

        case itInto:
        {
            if ( m_Data->InsertNew(New,Reference->ConvertToParent(),-1) )
            {
                m_Data->SelectItem(New,true);
            }
        }

    }
    m_Data->EndChange();
}

void wxsItemEditor::InitializeImages()
{
    if ( m_ImagesLoaded ) return;
    wxString basePath = ConfigManager::GetDataFolder() + _T("/images/wxsmith/");

    static const wxString NormalNames[] =
    {
        _T("insertinto.png"),
        _T("insertafter.png"),
        _T("insertbefore.png"),
        _T("deletewidget.png"),
        _T("showpreview.png"),
        _T("quickpropsopen.png"),
        _T("quickpropsclose.png"),
        _T("selected.png")
    };

    static const wxString SmallNames[] =
    {
        _T("insertinto16.png"),
        _T("insertafter16.png"),
        _T("insertbefore16.png"),
        _T("deletewidget16.png"),
        _T("showpreview16.png"),
        _T("quickpropsopen16.png"),
        _T("quickpropsclose16.png"),
        _T("selected16.png")
    };

    const wxString* Array = ( ToolIconSize() == 16L ) ? SmallNames : NormalNames;

    m_InsIntoImg.LoadFile(basePath + Array[0]);
    m_InsAfterImg.LoadFile(basePath + Array[1]);
    m_InsBeforeImg.LoadFile(basePath + Array[2]);
    m_DelImg.LoadFile(basePath + Array[3]);
    m_PreviewImg.LoadFile(basePath + Array[4]);
    m_QuickPropsImgOpen.LoadFile(basePath + Array[5]);
    m_QuickPropsImgClose.LoadFile(basePath + Array[6]);
    m_SelectedImg.LoadFile(basePath + Array[7]);

    m_ImagesLoaded = true;
}

void wxsItemEditor::OnButton(wxCommandEvent& event)
{
    wxWindow* Btn = (wxWindow*)event.GetEventObject();
    if ( Btn )
    {
        InsertRequest(Btn->GetName());
    }
}

void wxsItemEditor::SetInsertionTypeMask(int Mask)
{
    m_InsTypeMask = Mask;
    SetInsertionType(m_InsType);
}

void wxsItemEditor::SetInsertionType(int Type)
{
    Type &= m_InsTypeMask;

    if ( !Type )
    {
        Type = m_InsTypeMask;
    }

    if ( Type & itInto )
    {
        m_InsType = itInto;
    }
    else if ( Type & itAfter )
    {
        m_InsType = itAfter;
    }
    else if ( Type & itBefore )
    {
        m_InsType = itBefore;
    }
    else
    {
        m_InsType = 0;
    }

    RebuildInsTypeIcons();
}

void wxsItemEditor::RebuildInsTypeIcons()
{
    BuildInsTypeIcon(m_InsIntoBtn,m_InsIntoImg,itInto);
    BuildInsTypeIcon(m_InsBeforeBtn,m_InsBeforeImg,itBefore);
    BuildInsTypeIcon(m_InsAfterBtn,m_InsAfterImg,itAfter);
}

void wxsItemEditor::BuildInsTypeIcon(wxBitmapButton* Btn,const wxImage& Original,int ButtonType)
{
    bool Selected = (m_InsType & ButtonType) != 0;
    bool Enabled = (m_InsTypeMask & ButtonType) != 0;

    if ( !Enabled || !Selected )
    {
        Btn->SetLabel(Original);
    }
    else
    {
        wxBitmap Copy = Original;
        wxMemoryDC DC;
        DC.SelectObject(Copy);
        DC.DrawBitmap(m_SelectedImg,0,0);
        Btn->SetLabel(Copy);
    }

    Btn->Enable(Enabled);
    Btn->Refresh();
}

void wxsItemEditor::RebuildQuickPropsIcon()
{
    m_QuickPanelBtn->SetLabel( m_QuickPropsOpen ? m_QuickPropsImgClose : m_QuickPropsImgOpen );
}

void wxsItemEditor::RebuildIcons()
{
    RebuildInsTypeIcons();
    RebuildQuickPropsIcon();
    m_DelBtn->SetLabel(m_DelImg);
    m_PreviewBtn->SetLabel(m_PreviewImg);
    BuildPalette(m_WidgetsSet);
    Layout();
}

namespace
{
    int PrioritySort(const wxsItemInfo** it1,const wxsItemInfo** it2)
    {
        return (*it1)->Priority - (*it2)->Priority;
    }

    WX_DEFINE_ARRAY(const wxsItemInfo*,ItemsT);
    WX_DECLARE_STRING_HASH_MAP(ItemsT,MapT);
}

void wxsItemEditor::BuildPalette(wxNotebook* Palette)
{
    Palette->DeleteAllPages();

    // First we need to split all widgets into groups
    // it will be done using multimap (map of arrays)

    MapT Map;

    for ( const wxsItemInfo* Info = wxsItemFactory::GetFirstInfo(); Info; Info = wxsItemFactory::GetNextInfo() )
    {
        if ( !Info->Category.empty() )
        {
            Map[Info->Category].Add(Info);
        }
    }

    for ( MapT::iterator i = Map.begin(); i!=Map.end(); ++i )
    {
        wxScrolledWindow* CurrentPanel = new wxScrolledWindow(Palette,-1,wxDefaultPosition,wxDefaultSize,0/*wxALWAYS_SHOW_SB|wxHSCROLL*/);
        CurrentPanel->SetScrollRate(1,0);
        Palette->AddPage(CurrentPanel,i->first);
        wxSizer* RowSizer = new wxBoxSizer(wxHORIZONTAL);

        ItemsT& Items = i->second;
        Items.Sort(PrioritySort);

        for ( size_t j=Items.Count(); j-->0; )
        {
            const wxsItemInfo* Info = Items[j];
            const wxBitmap& Icon = ( PalIconSize() == 16L ) ? Info->Icon16 : Info->Icon32;

            if ( Icon.Ok() )
            {
                wxBitmapButton* Btn =
                    new wxBitmapButton(CurrentPanel,-1,Icon,
                        wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW,
                        wxDefaultValidator, Info->ClassName);
                RowSizer->Add(Btn,0,wxALIGN_CENTER);
                Btn->SetToolTip(Info->ClassName);
            }
            else
            {
                wxButton* Btn = new wxButton(CurrentPanel,-1,Info->ClassName,
                    wxDefaultPosition,wxDefaultSize,0,
                    wxDefaultValidator,Info->ClassName);
                RowSizer->Add(Btn,0,wxGROW);
                Btn->SetToolTip(Info->ClassName);
            }
        }
        CurrentPanel->SetSizer(RowSizer);
        RowSizer->SetVirtualSizeHints(CurrentPanel);
    }
}

void wxsItemEditor::OnInsInto(wxCommandEvent& event)
{
    SetInsertionType(itInto);
}

void wxsItemEditor::OnInsAfter(wxCommandEvent& event)
{
    SetInsertionType(itAfter);
}

void wxsItemEditor::OnInsBefore(wxCommandEvent& event)
{
    SetInsertionType(itBefore);
}

void wxsItemEditor::OnDelete(wxCommandEvent& event)
{
    if ( !m_Data ) return;
    m_Data->BeginChange();
    m_Data->DeleteSelected();
    m_Data->EndChange();
}

void wxsItemEditor::OnPreview(wxCommandEvent& event)
{
    if ( !m_Data ) return;
    if ( m_Data->IsPreview() )
    {
        m_Data->HidePreview();
    }
    else
    {
        m_Data->ShowPreview();
    }
}

void wxsItemEditor::OnQuickProps(wxCommandEvent& event)
{
    m_QuickPropsOpen = !m_QuickPropsOpen;
    RebuildQuickPropsIcon();
    ToggleQuickPropsPanel(m_QuickPropsOpen);
}

void wxsItemEditor::ToggleQuickPropsPanel(bool Open)
{
    m_HorizSizer->Show(m_QPArea,Open,true);
    Layout();
}

void wxsItemEditor::RebuildQuickProps(wxsItem* Selection)
{
    // Checking if we've already initialized visual stuff
    if ( !m_Content ) return;

    Freeze();

    int QPx, QPy;
    // TODO: Check if content of previous QPPanel shouldn't be stored into item
    m_QPArea->GetViewStart(&QPx,&QPy);
    m_QPArea->SetSizer(NULL);
    m_QPArea->DestroyChildren();
    m_QPSizer = new wxBoxSizer(wxVERTICAL);
    m_QPArea->SetSizer(m_QPSizer);

    if ( Selection )
    {
        wxWindow* QPPanel = Selection->BuildQuickPropertiesPanel(m_QPArea);
        if ( QPPanel )
        {
            m_QPSizer->Add(QPPanel,0,wxEXPAND);
        }
    }
    m_QPSizer->Layout();
    m_QPSizer->Fit(m_QPArea);
    Layout();
    m_QPArea->Scroll(QPx,QPy);
    Thaw();
}

wxsItem* wxsItemEditor::GetReferenceItem(int& InsertionType)
{
    wxsItem* Reference = m_Data->GetLastSelection();
    if ( !Reference )
    {
        // Fixing up reference item when there's nothing selected
        InsertionType = itInto;
        Reference = m_Data->GetRootItem();
        wxsParent* AsParent = Reference->ConvertToParent();
        if ( AsParent &&
             AsParent->GetChildCount() == 1 &&
             AsParent->GetChild(0)->GetType() == wxsTSizer )
        {
            Reference = AsParent->GetChild(0);
        }
    }
    return Reference;
}

wxImage wxsItemEditor::m_InsIntoImg;
wxImage wxsItemEditor::m_InsBeforeImg;
wxImage wxsItemEditor::m_InsAfterImg;
wxImage wxsItemEditor::m_DelImg;
wxImage wxsItemEditor::m_PreviewImg;
wxImage wxsItemEditor::m_QuickPropsImgOpen;
wxImage wxsItemEditor::m_QuickPropsImgClose;
wxImage wxsItemEditor::m_SelectedImg;
wxsItemEditor::WindowSet wxsItemEditor::m_AllEditors;
bool wxsItemEditor::m_ImagesLoaded = false;

BEGIN_EVENT_TABLE(wxsItemEditor,wxsEditor)
    EVT_BUTTON(wxsInsIntoId,wxsItemEditor::OnInsInto)
    EVT_BUTTON(wxsInsBeforeId,wxsItemEditor::OnInsBefore)
    EVT_BUTTON(wxsInsAfterId,wxsItemEditor::OnInsAfter)
    EVT_BUTTON(wxsDelId,wxsItemEditor::OnDelete)
    EVT_BUTTON(wxsPreviewId,wxsItemEditor::OnPreview)
    EVT_BUTTON(wxsQuickPropsId,wxsItemEditor::OnQuickProps)
    EVT_BUTTON(-1,wxsItemEditor::OnButton)
END_EVENT_TABLE()
