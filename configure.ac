AC_PREREQ(2.53)
AC_INIT(gnome-social-zone, 0.1.0)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AS_ALL_LINGUAS
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=gnome-social-zone
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

AC_SUBST(SOCIALZONE_VERSION, [0.1])

CFLAGS="$CFLAGS -Wall"

PKG_CHECK_MODULES(SOCIALZONE,
                  libjana-ecal
                  libsocialweb-client
                  gtk+-2.0 >= 2.14
                  clutter-gtk-0.10
                  mx-1.0 >= 0.9.0
                  gconf-2.0
                  gio-2.0
                  gio-unix-2.0)

AM_GCONF_SOURCE_2

AC_OUTPUT([
        Makefile
        socialzone/Makefile
        src/Makefile
        data/Makefile
        data/moblin-panel-myzone.schemas
        data/theme/Makefile
        po/Makefile.in
])
