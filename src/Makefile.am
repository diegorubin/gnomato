
AUTOMAKE_OPTIONS = subdir-objects 

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CPPFLAGS = ${Gnomato_CFLAGS} -DGNOMATO_LOCALEDIR=\"${GNOMATO_LOCALEDIR}\"

bin_PROGRAMS = gnomato 

gnomato_CPPFLAGS = \
	-I$(includedir)					\
	-DGNOMATO_DATADIR=\""$(pkgdatadir)"\"		\
	$(AM_CPPFLAGS)

gnomato_SOURCES = main.cc \
									win_main.cc \
									dialog_preferences.cc \
									dialog_task.cc \
									database.cc \
									task.cc \
									task_list.cc \
									config.cc \
									dbus_client.cc \
									python_executor.cc \
									wunderlist.cc \
									utils.cc

gnomato_LDADD = ${Gnomato_LIBS}

