# -*- Makefile -*-

AM_CPPFLAGS = ${regular_CPPFLAGS} ${libHX_CFLAGS} ${libmount_CFLAGS} \
              ${libpci_CFLAGS} ${libxcb_CFLAGS}
AM_CFLAGS   = ${regular_CFLAGS}

bin_PROGRAMS = \
	declone \
	sysinfo \
	tailhex \
	xcp

dist_bin_SCRIPTS = \
	fxterm \
	pegrep pesubst qpdecode \
	rot13 \
	qtar \
	wktimer xmlformat

dist_pkglibexec_SCRIPTS = \
	mailsplit \
	rezip

dist_noinst_SCRIPTS = settime.pl
declone_LDADD = ${libHX_LIBS}
sysinfo_LDADD = ${libHX_LIBS} ${libmount_LIBS} ${libpci_LIBS} ${libxcb_LIBS}
tailhex_LDADD = ${libHX_LIBS}
xcp_LDADD     = ${libHX_LIBS}
EXTRA_DIST = .gitignore proc_cpuinfo_ia64.txt proc_cpuinfo_ppc64.txt proc_cpuinfo_sparc64.txt proc_cpuinfo_x86_64.txt
