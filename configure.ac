AC_PREREQ(2.53)
AC_INIT(moblin-panel-myzone, 0.1.21)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AS_ALL_LINGUAS
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=moblin-panel-myzone
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],
                   ["$GETTEXT_PACKAGE"],
                   [Gettext domain name])
AM_GLIB_GNU_GETTEXT

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_ISC_POSIX
AC_HEADER_STDC
AM_PROG_CC_C_O
AC_PROG_LN_S

AC_SUBST(PENGE_VERSION, [0.1])

CFLAGS="$CFLAGS -Wall"

PKG_CHECK_MODULES(MPL, moblin-panel >= 0.50.0)

MOBLIN_PANELS_DIR=`$PKG_CONFIG --variable=moblin_panel_panels_dir moblin-panel`
AC_SUBST(MOBLIN_PANELS_DIR)

PKG_CHECK_MODULES(PENGE,
                  libjana-ecal
                  libsocialweb-client
                  gtk+-2.0 >= 2.14
                  mx-1.0 >= 0.9.0
                  gconf-2.0
                  gio-2.0
                  gio-unix-2.0)

PKG_CHECK_MODULES(MAILME,
                  gobject-2.0
                  glib-2.0
                  telepathy-glib >= 0.10)

AC_ARG_ENABLE([cache],
              [AC_HELP_STRING([--enable-cache],
                              [Enable Nbtk image cache generation])],
              [],
              [enable_cache=no])

AS_IF([test "x$enable_cache" = "xyes"],
      [
      AC_CHECK_PROG([NBTK_CREATE_IMAGE_CACHE],
                [nbtk-create-image-cache],
                [$(which nbtk-create-image-cache)])
])

AM_CONDITIONAL([ENABLE_CACHE],   [test "x$enable_cache" = "xyes"])

AC_ARG_ENABLE([moblin-desktop-file],
              [AC_HELP_STRING([--disable-moblin-desktop-file],
                              [Disable installation of moblin desktop file])],
              [enable_mdesktop=$enableval],
              [enable_mdesktop=yes])


AM_CONDITIONAL([ENABLE_MOBLIN_DESKTOP_FILE], [test "x$enable_mdesktop" = "xyes"])

AC_ARG_ENABLE([capplet],
              [AC_HELP_STRING([--disable-capplet],
                              [Disable the building of the capplet])],
              [enable_cappled=$enableval],
              [enable_capplet=yes])


AM_CONDITIONAL([ENABLE_CAPPLET], [test "x$enable_capplet" = "xyes"])

AS_IF([test "x$enable_capplet" = "xyes"],
      [
      PKG_CHECK_MODULES(CAPPLET,
                        gconf-2.0
                        gtk+-2.0 >= 2.14
                        mx-gtk-1.0
                        gio-unix-2.0
                        libgnome-control-center-extension)

      EXTENSIONSDIR=$($PKG_CONFIG libgnome-control-center-extension --variable=extensiondir)
      AC_SUBST(EXTENSIONSDIR)
])

AM_GCONF_SOURCE_2

AC_OUTPUT([
        Makefile
        penge/Makefile
        mailme/Makefile
        src/Makefile
        data/Makefile
        data/moblin-panel-myzone.schemas
        data/theme/Makefile
        capplet/Makefile
        po/Makefile.in
        penge.pc
])
