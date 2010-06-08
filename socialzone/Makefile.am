lib_LTLIBRARIES = libsocialzone.la

socialzoneincludedir = $(pkgincludedir)/socialzone

socialzoneinclude_HEADERS = \
		   penge-people-tile.h \
		   penge-recent-file-tile.h \
		   penge-event-tile.h \
		   penge-events-pane.h \
		   penge-grid-view.h \
		   penge-utils.h \
		   penge-magic-texture.h \
		   penge-view-background.h \
		   penge-app-tile.h \
		   penge-task-tile.h \
		   penge-tasks-pane.h \
		   penge-welcome-tile.h \
		   penge-interesting-tile.h \
		   penge-block-container.h \
		   penge-everything-pane.h \
		   penge-clickable-label.h \
		   penge-count-tile.h \
		   penge-dynamic-box.h

libsocialzone_la_SOURCES = \
		      $(pengeinclude_HEADERS) \
		      penge-people-tile.c \
		      penge-recent-file-tile.c \
		      penge-event-tile.c \
		      penge-events-pane.c \
		      penge-grid-view.c \
		      penge-utils.c \
		      penge-magic-texture.c \
		      penge-view-background.c \
		      penge-app-tile.c \
		      penge-task-tile.c \
		      penge-tasks-pane.c \
		      penge-welcome-tile.c \
		      penge-interesting-tile.c \
		      penge-block-container.c \
		      penge-everything-pane.c \
		      penge-clickable-label.c \
		      penge-count-tile.c \
		      penge-dynamic-box.c


AM_CFLAGS = -Wall \
	    $(SOCIALZONE_CFLAGS) \
	    -I$(top_srcdir) \
	    -DTHEMEDIR=\"$(pkgdatadir)/theme\" \
	    -DLOCALEDIR=\"$(localedir)\"

