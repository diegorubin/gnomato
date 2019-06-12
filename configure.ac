AC_INIT([Gnomato],[1.0.3],[rubin.diego@gmail.com])

AM_INIT_AUTOMAKE(gnomato,1.0.3)

AC_PROG_CC
AC_PROG_CXX

AC_PROG_INSTALL

AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES([Gnomato],[gtkmm-3.0 >= 3.0.0, sqlite3, libnotify >= 0.7.3,
                   python3 >= 3.5])

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=gnomato
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext domain])

AM_GLIB_GNU_GETTEXT

dnl Add gsettings rules
GLIB_GSETTINGS

GNOMATO_LOCALEDIR=[${datadir}/locale]
AC_SUBST(GNOMATO_LOCALEDIR)

AC_CONFIG_FILES([
Makefile
src/Makefile
data/Makefile
po/Makefile.in
])

AC_OUTPUT(gnomato.desktop)

