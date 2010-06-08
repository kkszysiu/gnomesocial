QUIET_GEN = $(Q:@=@echo '  GEN   '$@;)

AM_CFLAGS = \
	    $(SOCIALZONE_CFLAGS) \
	    -I$(top_srcdir) \
	    -DLOCALEDIR=\"$(localedir)\" \
	    -DTHEMEDIR=\"$(pkgdatadir)/theme\"

libexec_PROGRAMS = gnome-social-zone

gnome_social_zone_LDADD = \
			    $(top_builddir)/socialzone/libsocialzone.la \
			    $(SOCIALZONE_LIBS)

gnome_social_zone_SOURCES = \
  gnome-social-zone.c

servicedir = $(datadir)/dbus-1/services
service_in_files = org.moblin.UX.Shell.Panels.myzone.service.in
service_DATA = org.moblin.UX.Shell.Panels.myzone.service

EXTRA_DIST = $(service_in_files)

DISTCLEANFILES = org.moblin.UX.Shell.Panels.myzone.service

org.moblin.UX.Shell.Panels.myzone.service: org.moblin.UX.Shell.Panels.myzone.service.in
	$(QUIET_GEN)sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@
