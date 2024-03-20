rel-20231224
============
* Add gh-trim-workflowruns script
* git-forest: port to Perl 5.38


rel-20231101
============
* qtar: add .zst suffix support
* sadmin: delete utmp_register
* dircolors: reinstate upstream color for BLK/CHR
* spec-beautifier: apply transformation logic to preamble as well
* spec-beautifier: eliminate BuildRoot/Package/Distribution/Vendor lines
* spec-beautifier: delete massaging of BuildRequire lists


rel-20230411
============
* FreeBSD/OpenBSD portability fixes
* checkbrack: remove -p option
* gpsh: avoid spurious "mkvmerge failed" report
* wktimer: -S option behavior split off to -X


rel-20221120
============
* A few utilities have been split off to a new project, consoleet-utils.
  See https://inai.de/projects/consoleet/ for details.
* Affected: colortable cp437table unitable palcomp vfontas


rel-20221119
============
* data: add vga.theme
* vfontas: add "copy" operation
* smm: new utility "palcomp"
* vfontas: implement aspect ratio correction for SFD output


rel-20211204
============
Enhancements:
* vfontas: add a -move operation
* kbd: offer "the UEFI font" (a file for setfont(8) or for further vfontas(1)
  processing)
* Documentation updates


rel-20210928
============
Enhancements:
* vfontas: add recognition for BDF glyphs as produced by fontforge from PCF
* kbd: map beta to eszett on cp437x
* vfontas: recognize "idem" lines in kbd unimaps
* vfontas: add -setbold, -setprop, -lgeu, -lgeuf commands


rel-20210803
============
Enhancements:
* vfontas: read support for BDF
* kbd: cp437x.uni learned a few Unicode codepoints for checkmarks
  (cp437x is a glyph mapping table for arbitrary CP437 fonts
  that declares some extra mappings to display more Unicode
  characters somehow, rather than not showing them with a blank/box character)


rel-20201116
============
* pegrep: cure uninitialized variable warnings
* gpsh: change tmp location to /var/tmp
  (avoid saving potentially large files to tmpfs)
* sysinfo: fix null deref when /etc/os-release is absent


rel-20200712
============
* profile: add QUILT_COLORS
* bsvplay: Pianoman MUS support
* pcmmix: new utility for arpeggio/polyphonic mixing of
  bsvplay/qplay outputs


rel-20200428
============
* vfontas: N2 vectorizer: cease smoothing of stub serifs
  (concerns the topleft corner of an 'E' in some fonts)
* vfontas: N2 vectorizer: avoid diagonalizing "pimples" in font outlines
  (concerns 1-pixel protrusions in the right vertical edge of the '4')


rel-20200331
============
* vfontas: add new N2 vectorizer
* vfontas: new -saven2 command to use N2

The N2 vectorizer is able to recognize logical 45-degree angles in
bitmap glyphs and will output the outlines as such. Unlike N1,
it analyzes glyph outlines rather than pixel patterns, and produces
higher quality output due to a larger working radius.

N2 can reproduce the "Nouveau IBM" font quite closely now (e.g. from
Fontraption's VGA9.F16 file). A few differences have just become apparent (one
can inspect in e.g. Inkscape by overlaying both fontfaces) that may be
addressed in a later release.

N2:

* Isthmuses tend to get washed out, noticable in glyphs like 'G'.
* Sumps in the 'K', 'M', 'N' glyphs could be improved.
  This is noticable in glyphs like 'G'.

Nouveau IBM:

* Kerning is not original: too closely spaced.
* Node points are not at pixel or half-pixel locations.
  'B' has a 30.78 diagonal for no apparent reason. This is bad for integral
  scaling. Other values can be observed in 'W'.
* Glyph 'C' is asymmetric.


rel-20200309
============
* vfontas: add N1 vectorizer
* vfontas: new -saven1 command to use N1
* vfontas: remove -xbrz



rel-20200124
============
* vfontas: add full vectorizer for SFD glyphs
* vfontas: improved ascent/descent values for vector output
* vfontas: hex format misreading was fixed


rel-20190428
============
* vfontas: new implementation. Replaces clt2bdf, clt2pbm, cltxscale, fnt2bdf.
* vfontas: xBRZ x6 support
* profile: use new GREP_COLORS variable


rel-20180301
====
