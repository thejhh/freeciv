## Process this file with automake to produce Makefile.in

## Must be set before adding anything
GUI_SUBDIRS =

desktopfiledir = $(prefix)/share/applications

if CLIENT_GUI_SDL
GUI_SUBDIRS += gui-sdl
endif
if CLIENT_GUI_GTK_2_0
GUI_SUBDIRS += gui-gtk-2.0
desktopfile_DATA = \
	freeciv.desktop
endif
if CLIENT_GUI_GTK_3_0
GUI_SUBDIRS += gui-gtk-3.0
desktopfile_DATA = \
	freeciv.desktop
endif
if CLIENT_GUI_XAW
GUI_SUBDIRS += gui-xaw
endif
if CLIENT_GUI_QT
GUI_SUBDIRS += gui-qt
endif
if CLIENT_GUI_STUB
GUI_SUBDIRS += gui-stub
endif

ALL_AUDIO_SDL_FILES=audio_sdl.c audio_sdl.h

if AUDIO_SDL
AUDIO_SDL_FILES=$(ALL_AUDIO_SDL_FILES)
endif

if MINGW32
CLIENTICON=../win32/clienticon.o
endif

EXTRA_DIST= $(ALL_AUDIO_SDL_FILES)

## This is usually false, so "include" is not recursed into 
## by 'make', but it can be enabled in configure, and it is
## necessary to have something like this so that the include 
## directory is included for 'make dist'.
if MAKE_CLIENT_INCLUDE
CLI_INCLUDE = include
else
CLI_INCLUDE = 
endif

SUBDIRS = $(GUI_SUBDIRS) $(CLI_INCLUDE) agents luascript

AM_CPPFLAGS = \
	-I$(top_srcdir)/utility -I$(srcdir)/include \
	-I$(top_srcdir)/common -I$(top_srcdir)/common/aicore \
	-I$(top_srcdir)/common/scriptcore \
	-I$(srcdir)/luascript -I$(srcdir)/agents \
	$(CLIENT_CFLAGS) $(SOUND_CFLAGS) \
	$(MAPIMG_WAND_CFLAGS) \
	$(LIBGGZ_INCLUDES) $(GGZMOD_INCLUDES) $(GGZ_GTK_INCLUDES) \
	$(LUA_CFLAGS) $(LUASQL_CFLAGS) $(TOLUA_CFLAGS) \
	-I$(top_srcdir)/dependencies/cvercmp

# Build client common functionality in to library
# instead linking them directly to civclient. This is necessary
# for static linking.
freeciv_client_src = $(AUDIO_SDL_FILES) \
	attribute.h	\
	attribute.c	\
	citydlg_common.c \
	citydlg_common.h \
	cityrepdata.c	\
	cityrepdata.h	\
	client_main.c	\
	client_main.h	\
	chatline_common.c \
	chatline_common.h \
	connectdlg_common.c \
	connectdlg_common.h \
	climisc.c	\
	climisc.h	\
	climap.c	\
	climap.h	\
	clinet.c	\
	clinet.h	\
	colors_common.c		\
	colors_common.h		\
	control.c	\
	control.h	\
	editor.c 		\
	editor.h 		\
	ggzclient.c	\
	ggzclient.h	\
	global_worklist.c	\
	global_worklist.h	\
	goto.c		\
	goto.h		\
	helpdata.c	\
	helpdata.h	\
	luaconsole_common.c	\
	luaconsole_common.h	\
	mapctrl_common.c \
	mapctrl_common.h \
	mapview_common.c \
	mapview_common.h \
	messagewin_common.c \
	messagewin_common.h \
	overview_common.c	\
	overview_common.h	\
	packhand.c	\
	packhand.h	\
	packhand_gen.h	\
	packhand_gen.c	\
	plrdlg_common.c \
	plrdlg_common.h \
	options.c	\
	options.h	\
	repodlgs_common.c \
	repodlgs_common.h \
	reqtree.c \
	reqtree.h \
	servers.c		\
	servers.h		\
	text.c	\
	text.h	\
	themes_common.c	\
	themes_common.h	\
	tilespec.c	\
	tilespec.h	\
	unitselect_common.c	\
	unitselect_common.h	\
	update_queue.c	\
	update_queue.h	\
	voteinfo.c	\
	voteinfo.h	\
	audio.c         \
	audio.h         \
	audio_none.c    \
	audio_none.h

# packhand_gen.c & packhand_gen.h are generated files, but as they are
# generated outside this directory (when building common) there's no point in
# setting them to BUILT_SOURCES.

# These files are not generated to builddir, but to srcdir */
MAINTAINERCLEANFILES = $(srcdir)/packhand_gen.c $(srcdir)/packhand_gen.h

bin_PROGRAMS =
noinst_LTLIBRARIES =

clientlibs = \
 $(top_builddir)/common/scriptcore/libscriptcore.la \
 $(top_builddir)/client/agents/libagents.la \
 $(top_builddir)/client/luascript/libscripting_client.la \
 $(top_builddir)/dependencies/cvercmp/libcvercmp.la

if CLIENT_GUI_GTK_2_0
bin_PROGRAMS += freeciv-gtk2
noinst_LTLIBRARIES += libfcgui-gtk2.la
libfcgui_gtk2_la_SOURCES = $(freeciv_client_src)
libfcgui_gtk2_la_LIBADD = gui-gtk-2.0/libgui-gtk2.la $(clientlibs)
freeciv_gtk2_SOURCES = dummy.c
freeciv_gtk2_LDFLAGS = $(GUI_gtk2_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_gtk2_LDADD = \
 $(SOUND_LIBS) \
 libfcgui-gtk2.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_gtk2_LIBS)
endif

if CLIENT_GUI_GTK_3_0
bin_PROGRAMS += freeciv-gtk3
noinst_LTLIBRARIES += libfcgui-gtk3.la
libfcgui_gtk3_la_SOURCES = $(freeciv_client_src)
libfcgui_gtk3_la_LIBADD = gui-gtk-3.0/libgui-gtk3.la $(clientlibs)
freeciv_gtk3_SOURCES = dummy.c
freeciv_gtk3_LDFLAGS = $(GUI_gtk3_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_gtk3_LDADD = \
 $(SOUND_LIBS) \
 libfcgui-gtk3.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_gtk3_LIBS)
endif

if CLIENT_GUI_SDL
bin_PROGRAMS += freeciv-sdl
noinst_LTLIBRARIES += libfcgui-sdl.la
libfcgui_sdl_la_SOURCES = $(freeciv_client_src)
libfcgui_sdl_la_LIBADD = gui-sdl/libgui-sdl.la $(clientlibs)
freeciv_sdl_SOURCES = dummy.c
freeciv_sdl_LDFLAGS = $(GUI_sdl_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_sdl_LDADD = \
 $(GUI_sdl_LIBS) \
 libfcgui-sdl.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_sdl_LIBS)
endif

if CLIENT_GUI_XAW
bin_PROGRAMS += freeciv-xaw
noinst_LTLIBRARIES += libfcgui-xaw.la
libfcgui_xaw_la_SOURCES = $(freeciv_client_src)
libfcgui_xaw_la_LIBADD = gui-xaw/libgui-xaw.la $(clientlibs)
freeciv_xaw_SOURCES = dummy.c
freeciv_xaw_LDFLAGS = $(GUI_xaw_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_xaw_LDADD = \
 libfcgui-xaw.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_xaw_LIBS)
endif

if CLIENT_GUI_QT
bin_PROGRAMS += freeciv-qt
noinst_LTLIBRARIES += libfcgui-qt.la
libfcgui_qt_la_SOURCES = $(freeciv_client_src)
libfcgui_qt_la_LIBADD = gui-qt/libgui-qt.la $(clientlibs)
freeciv_qt_SOURCES = dummycxx.cpp
freeciv_qt_LDFLAGS = $(GUI_qt_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_qt_LDADD = \
 libfcgui-qt.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_qt_LIBS)
endif

if CLIENT_GUI_STUB
bin_PROGRAMS += freeciv-stub
noinst_LTLIBRARIES += libfcgui-stub.la
libfcgui_stub_la_SOURCES = $(freeciv_client_src)
libfcgui_stub_la_LIBADD = gui-stub/libgui-stub.la $(clientlibs)
freeciv_stub_SOURCES = dummy.c
freeciv_stub_LDFLAGS = $(GUI_stub_LDFLAGS) $(GGZMOD_LDFLAGS)
freeciv_stub_LDADD = \
 libfcgui-stub.la \
 $(top_builddir)/common/libfreeciv.la \
 $(INTLLIBS) $(CLIENT_LIBS) $(SOUND_LIBS) $(LIB_GGZMOD) $(CLIENTICON) \
 $(LUA_LIBS) $(TOLUA_LIBS) $(LUASQL_LIBS) \
 $(MAPIMG_WAND_LIBS) \
 $(GUI_stub_LIBS)
endif
