#ifndef WXSFONTPROPERTY_H
#define WXSFONTPROPERTY_H

#include "../../properties/wxsproperties.h"
#include "../../wxscodinglang.h"

/** \brief Structure holding font configuration */
struct wxsFontData
{
    long Size;
    long Style;
    long Weight;
    bool Underlined;
    long Family;
    wxArrayString Faces;
    wxString Encoding;
    wxString SysFont;
    double RelativeSize;

    bool IsDefault : 1;
    bool HasSize : 1;
    bool HasStyle : 1;
    bool HasWeight : 1;
    bool HasUnderlined : 1;
    bool HasFamily : 1;
    bool HasEncoding : 1;
    bool HasSysFont : 1;
    bool HasRelativeSize : 1;

    /** \brief Function building font from font data */
    wxFont BuildFont();

    /** \brief Function generating code which will build font object with specified name
     * \return Code building font or empty string if font is default
     */
    wxString BuildFontCode(const wxString& FontName,wxsCodingLang Language);

    /** \brief Ctor - initializes all values to default ones */
    wxsFontData():
        Size(12),
        Style(wxFONTSTYLE_NORMAL),
        Weight(wxFONTWEIGHT_NORMAL),
        Underlined(false),
        Family(wxFONTFAMILY_DEFAULT),
        IsDefault(true),
        HasSize(false),
        HasStyle(false),
        HasWeight(false),
        HasUnderlined(false),
        HasFamily(false),
        HasEncoding(false),
        HasSysFont(false),
        HasRelativeSize(false)
    {}

};

/** \brief Property for editing font properties
 */
class wxsFontProperty: public wxsCustomEditorProperty
{
	public:

        /** \brief Ctor
         *  \param PGName       name of property in Property Grid
         *  \param DataName     name of property in data stuctures
         *  \param Offset       offset of wxsFontData structure (returned from wxsOFFSET macro)
         */
		wxsFontProperty(
            const wxString& PGName,
            const wxString& DataName,
            long Offset);

		/** \brief Returning type name */
		virtual const wxString GetTypeName() { return _T("wxFont"); }

        /** \brief Showing editor for this property */
        virtual bool ShowEditor(wxsPropertyContainer* Object);

    protected:

        virtual bool XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element);
        virtual bool XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element);
        virtual bool PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream);
        virtual bool PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream);

	private:

        long Offset;
};

/** \addtogroup ext_properties_macros
 *  \{ */

/** \brief Macro automatically declaring font property
 *  \param ClassName name of class holding this property
 *  \param VarName name of wxsFontData variable inside class
 *  \param Flags flags of availability, see \link wxsPropertyContainer::Property
           wxsPropertyContainer::Property \endlink for details, use 0 to always
           use this property
 *  \param PGName name used in property grid
 *  \param DataName name used in Xml / Data Streams
 */
#define WXS_FONT(ClassName,VarName,Flags,PGName,DataName) \
    static wxsFontProperty PropertyFont##ClassName##VarName(PGName,DataName,wxsOFFSET(ClassName,VarName)); \
    Property(PropertyFont##ClassName##VarName,Flags);

/** \} */


#endif
