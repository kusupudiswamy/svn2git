SUBDIRS = properties wxwidgets

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/sdk \
		-I$(top_srcdir)/src/sdk/wxFlatNotebook/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include \
		-I$(top_srcdir)/src/sdk/propgrid/include

libdir = $(pkgdatadir)/plugins

lib_LTLIBRARIES = libwxsmith.la

libwxsmith_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libwxsmith_la_LIBADD = ../../../sdk/libcodeblocks.la \
				properties/libwxsmith_properties.la \
				wxwidgets/libwxsmith_wxwidgets.la

libwxsmith_la_SOURCES = \
./wxsadvqpp.cpp \
./wxsadvqppchild.cpp \
./wxscoder.cpp \
./wxscodinglang.cpp \
./wxseditor.cpp \
./wxsextresmanager.cpp \
./wxsgui.cpp \
./wxsguifactory.cpp \
./wxsmith.cpp \
./wxsmithmime.cpp \
./wxsproject.cpp \
./wxsresource.cpp \
./wxsresourcefactory.cpp \
./wxsresourcetree.cpp \
./wxsresourcetreeitemdata.cpp \
./wxsstoringsplitterwindow.cpp \
./wxsversionconverter.cpp \
./wxssettings.cpp

noinst_HEADERS = \
./wxsadvqpp.h \
./wxsadvqppchild.h \
./wxscoder.h \
./wxscodinglang.h \
./wxseditor.h \
./wxsextresmanager.h \
./wxsgui.h \
./wxsguifactory.h \
./wxsmith.h \
./wxsmithmime.h \
./wxsproject.h \
./wxsresource.h \
./wxsresourcefactory.h \
./wxsresourcetree.h \
./wxsresourcetreeitemdata.h \
./wxsstoringsplitterwindow.h \
./wxsversionconverter.h \
./wxssettings.h

EXTRA_DIST = \
*.cbp \
*.bat \
*.unix \
manifest.xml

pkgdata_DATA = wxsmith.zip
CLEANFILES = $(pkgdata_DATA)

wxsmith.zip:
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/wxsmith.zip manifest.xml > /dev/null
