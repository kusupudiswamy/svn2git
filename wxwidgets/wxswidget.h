#ifndef WXSWIDGET_H
#define WXSWIDGET_H

#include "wxsitem.h"
#include "wxsbaseproperties.h"
#include "wxsstyle.h"

/** \brief Class used as a base class for standard widgets without any children
 *
 * Few steps to create new widgets:
 *  \li Create widget info structure
 *  \li Create new class deriving from wxsWidget
 *  \li Create set of styles using macros defined in wxsstyle.h
 *  \li Create set of events using macros defined in wxsevent.h
 *  \li Override EnumWidgetProperties function
 *  \li Override BuildCreatingCode function (it's declared in wxsItem)
 *  \li Override BuildPreview function (also declared in wxsItem)
 *
 */
class wxsWidget: public wxsItem
{
    public:

        /** \brief Ctor
         *  \param Resource resource containing this item
         *  \param PropertiesFlags flags filtering sed base properties
         *         (see wxsBaseProperties for details)
         *  \param Info pointer to static widget info
         *  \param EventArray pointer to static set of events
         *  \param StyleSet set of used styles, if NULL, this widget won't
         *         provide styles by default
         *  \param DefaultStyle default style used on read errors, string can
         *         contain one or more style names separated with '|' character
         */
        wxsWidget(
            wxsItemRes* Resource,
            const wxsItemInfo* Info,
            long PropertiesFlags,
            const wxsEventDesc* EventArray = NULL,
            const wxsStyle* StyleSet=NULL,
            const wxString& DefaultStyle=wxEmptyString);

    protected:

        /** \brief Function enumerating properties for this widget only
         *
         * This function should enumerate all extra properties
         * required by item (extra means not enumerated in base properties
         * not id or variable name).
         * These properties will be placed at the beginning, right after
         * there will be Variable name and identifier and at the end, all
         * required base properties.
         */
        virtual void OnEnumWidgetProperties(long Flags) = 0;

        /** \brief Function which adding new items to QPP
         *
         * This function may be used to add special quick properties for
         * this item.
         *
         * All QPPChild panels will be added before additional panels
         * added by widget.
         */
        virtual void OnAddWidgetQPP(wxsAdvQPP* QPP) { }

        /** \brief Easy access to position */
        inline wxPoint Pos(wxWindow* Parent)
        {
            // TODO: Implement properly when base properties done
            //return BaseProps.Position.GetPosition(Parent);
            return wxDefaultPosition;
        }

        /** \brief Easy access to size */
        inline wxSize Size(wxWindow* Parent)
        {
            // TODO: Implement properly when base properties done
            //return BaseProps.Size.GetSize(Parent);
            return wxDefaultSize;
        }

        /** \brief Easy access to style (can be used directly in wxWidgets */
        inline long Style()
        {
            // TODO: Implement properly when base properties done
            //return wxsStyleProperty::GetWxStyle(StyleBits,StyleSet,false);
            return 0;
        }

        /** \brief Function setting up standard widget properties after
         *         the window is created.
         *
         * Included properties:
         *  - Enabled
         *  - Focused
         *  - Hidden (skipped when not exact preview)
         *  - FG - Foreground colour
         *  - BG - Background colour
         *  - Font
         *  - ToolTip
         *  - HelpText
         *  - Extra style
         *
         * \param Preview window for which properties must be applied
         * \param IsExact value of IsExact argument passed to BuildPreview
         * \return window after settig up properties (value of Preview is returned)
         */
        wxWindow* SetupWindow(wxWindow* Preview,bool IsExact);

        /** \brief Easy acces to position code */
        inline wxString PosCode(const wxString& Parent,wxsCodingLang Language)
        {
            // TODO: Implement properly when base properties done
            //return BaseProps.Position.GetPositionCode(Parent,Language);
            return _T("wxDefaultPosition");
        }

        /** \brief Easy acces to size code */
        inline wxString SizeCode(const wxString& Parent,wxsCodingLang Language)
        {
            // TODO: Implement properly when base properties done
            //return BaseProps.Size.GetSizeCode(Parent,Language);
            return _T("wxDefaultSize");
        }

        /** \brief Easy access to style code */
        inline wxString StyleCode(wxsCodingLang Language)
        {
            // TODO: Implement properly when base properties done
            //return wxsStyleProperty::GetString(StyleBits,StyleSet,false,Language);
            return _T("0");
        }

        /** \brief Function adding code setting up properties after window
         *         is created.
         *
         * Behaviour of this function is simillar to SetupWindow but this
         * one creates source code doing it instead of setting up real window
         */
        void SetupWindowCode(wxString& Code,wxsCodingLang Language);

    private:

        /** \brief Function enumerating properties with default ones
         *
         * Function enumerating item properties. The implementation
         * does call EnumContainerProperties() and adds all default properties.
         */
        virtual void OnEnumItemProperties(long Flags);

        /** \brief Function Adding QPPChild panels for base properties of this
         *         container.
         *
         * This function calls internally AddContainerQPP to add any additional
         * QPPChild panels.
         */
        virtual void OnAddItemQPP(wxsAdvQPP* QPP);

        const wxsStyle* m_StyleSet;
        wxString m_DefaultStyle;
        long m_StyleBits;
        long m_ExStyleBits;

};

#endif
