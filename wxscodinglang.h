#ifndef WXSCODINGLANG_H
#define WXSCODINGLANG_H

/** \brief This enumeration contains all coding languages supported in wxSmith */
enum wxsCodingLang
{
    wxsCPP,
    wxsUnknownLanguage
};

/** \brief Namespace with usefull common functions for supported coding languages */
namespace wxsCodeMarks
{
    /** \brief Getting wxString name of  coding language */
    wxString Name(wxsCodingLang Id);

    /** \brief Getting id of coding language's name */
    wxsCodingLang Id(const wxString& Name);

    /** \brief Getting coding language from file's extension */
    wxsCodingLang IdFromExt(const wxString& Extension);

    /** \brief Helper function for generating block begin */
    wxString Beg(wxsCodingLang Lang,const wxString& BlockName);

    /** \brief Helper function for generating block begin */
    wxString Beg(wxsCodingLang Lang,const wxString& BlockName,const wxString& Param);

    /** \brief Helper functino for generating block end */
    wxString End(wxsCodingLang Lang);
}

#endif
