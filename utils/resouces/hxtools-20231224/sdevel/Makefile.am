# -*- Makefile -*-

AM_CPPFLAGS = ${regular_CPPFLAGS} ${libHX_CFLAGS}
AM_CFLAGS   = ${regular_CFLAGS}
AM_CXXFLAGS = ${regular_CXXFLAGS}

bin_PROGRAMS = \
	bin2c checkbrack gxxdm

dist_bin_SCRIPTS = \
	cwdiff \
	gh-trim-workflowruns \
	git-author-stat \
	git-blame-stats \
	git-forest \
	git-revert-stats \
	git-track \
	man2html \
	spec-beautifier

pkglibexec_PROGRAMS = \
	peicon

dist_pkglibexec_SCRIPTS = \
	diff2php \
	doxygen-kerneldoc-filter \
	paddrspacesize \
	proc_stat_signal_decode \
	sourcefuncsize

bin2c_LDADD = ${libHX_LIBS}
checkbrack_SOURCES = checkbrack.cpp
checkbrack_LDADD = ${libHX_LIBS}
gxxdm_SOURCES = gxxdm.cpp
gxxdm_LDADD = ${libHX_LIBS}
peicon_LDADD = ${libHX_LIBS}
EXTRA_DIST = .gitignore
