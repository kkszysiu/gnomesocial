SUBDIRS=theme

@INTLTOOL_DESKTOP_RULE@

desktopfiles_in_files=gnome-social-zone.desktop.in
desktopfiles_files=$(desktopfiles_in_files:.desktop.in=.desktop)

#settingsfiles_in_files=myzone.desktop.in
#settingsfiles_files=$(settingsfiles_in_files:.desktop.in=.desktop)
#settingsdir = $(datadir)/applications
#settings_DATA = $(settingsfiles_files)

%.desktop.in: %.desktop.in.in Makefile
	$(QUIET_GEN)sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@

# would prefer to just symlink this, but could make that pass distcheck
autostartdir = $(sysconfdir)/xdg/autostart
autostart_DATA = $(desktopfiles_files)

schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = gnome-social-zone.schemas

install-data-local:
if GCONF_SCHEMAS_INSTALL
	if test -z "$(DESTDIR)" ; then \
		GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) gconftool-2 --makefile-install-rule $(schema_DATA) 2>&1 > /dev/null; \
	fi
endif

EXTRA_DIST = gnome-social-zone.desktop.in.in gnome-social-zone.schemas.in
CLEANFILES = gnome-social-zone.desktop gnome-social-zone.desktop.in
