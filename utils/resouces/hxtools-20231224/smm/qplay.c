// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2002-2010 Jan Engelhardt
/*
 *	qplay.c - QBasic music command interpreter
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libHX.h>
#include "pcspkr.h"

enum {
	MAX_OCTAVES = 9,
};

static void parse_str(const char *);

static struct HXmap *keymap, *varmap;
static double notemap[MAX_OCTAVES*12], glob_mode = 7.0 / 8;
static int glob_spd = 120, glob_len = 4, glob_octave = 4;
static struct pcspkr pcsp = {
	.sample_rate = 48000,
	.prop_square = 1,
	.prop_sine   = 1,
	.volume      = 0.1,
};

/**
 * parse_arg_ag - parse a note, which is 'a' <= x <= 'g'.
 */
static unsigned int parse_arg_ag(const char *origptr)
{
	unsigned long note_len = glob_len, note_dur;
	const struct HXmap_node *fq_node;
	double af = 1, len_plus = 0;
	const char *ptr = origptr;
	char lookup[4] = {};

	lookup[0] = *ptr++;
	if (*ptr == '#' || *ptr == '+' || *ptr == '-') /* c# */
		lookup[1] = *ptr++;

	if (HX_isdigit(*ptr)) /* c2, or c#2 */
		note_len = strtoul(ptr, (char **)&ptr, 10);

	if ((lookup[0] == 'c' && lookup[1] == '-') ||
	    (lookup[0] == 'e' && lookup[1] == '+') ||
	    (lookup[0] == 'f' && lookup[1] == '-'))
		/* Allow B+/C- and E+/F- but warn the user */
		fprintf(stderr, "** Illegal note \"%s\" in C dur, coercing it\n", lookup);

	/* convert to ticks */
	note_len = pcsp.sample_rate * 240 / (note_len * glob_spd);
	fprintf(stderr, "%s", lookup);

	while (*ptr == '.') {
		/* find lengthy dots */
		len_plus += af /= 2;
		++ptr;
	}

	note_len += len_plus * note_len;
	note_dur  = note_len * glob_mode;

	if ((fq_node = HXmap_find(keymap, lookup)) == NULL) {
		fprintf(stderr, "Irk. fq_node == NULL\n");
	} else {
		pcspkr_output(&pcsp,
			notemap[glob_octave * 12 + (long)fq_node->data],
			note_dur,
			note_len - note_dur
		);
	}
	return ptr - origptr;
}

/**
 * parse_arg_l - set default note length
 */
static unsigned int parse_arg_l(const char *origptr)
{
	const char *ptr = origptr;
	unsigned long len;

	++ptr;
	if ((len = strtoul(ptr, (char **)&ptr, 0)) > 0) {
		glob_len = len;
		return ptr - origptr;
	}

	fprintf(stderr, "** Invalid command ignored: L0\n");
	return 1;
}

/**
 * parse_arg_m - set mode and playing style
 * @origptr:	data
 *
 * ML: legato
 * MN: normal
 * MS: staccato
 * MB (background) and MF (foreground) are not supported and ignored.
 */
static unsigned int parse_arg_m(const char *origptr)
{
	const char *ptr = origptr;

	switch (*++ptr) {
		case 'l':
			glob_mode = 1;
			break;
		case 'n':
			glob_mode = 7.0 / 8;
			break;
		case 's':
			glob_mode = 3.0 / 4;
			break;
		case 'b':
		case 'f':
			fprintf(stderr, "** Useless command ignored: MB/MF\n");
			break;
		default:
			fprintf(stderr, "** Invalid command ignored: M%c\n", *ptr);
			break;
	}
	return ++ptr - origptr;
}

/**
 * parse_arg_n - play a note value
 * (The values 0..85 are related to MIDI notes.)
 */
static unsigned int parse_arg_n(const char *origptr)
{
	unsigned long note_spc, note_dur;
	const char *ptr = origptr;
	int n;

	++ptr;
	if ((n = strtoul(ptr, (char **)&ptr, 10)) <= 0) {
		fprintf(stderr, "** Useless command ignored: N0\n");
		return ptr - origptr;
	}

	if (n > MAX_OCTAVES * 12 + 1) {
		fprintf(stderr, "** Invalid command ignored: N%d\n", n);
		return ptr - origptr;
	}

	note_spc = 240000.0 / (glob_spd * glob_len) * 48000;
	note_dur = note_spc * glob_mode;
	pcspkr_output(&pcsp, notemap[n-1], note_dur, note_spc - note_dur);
	return ptr - origptr;
}

/**
 * parse_arg_o - set octave
 */
static unsigned int parse_arg_o(const char *origptr)
{
	const char *ptr = origptr;
	int n;

	++ptr;
	if ((n = strtoul(ptr, (char **)&ptr, 10)) < MAX_OCTAVES) {
		glob_octave = n;
	} else {
		fprintf(stderr, "** Invalid command ignored: O%d\n", n);
	}

	return ptr - origptr;
}

/**
 * parse_arg_p - pause for a while
 */
static unsigned int parse_arg_p(const char *origptr)
{
	const char *ptr = origptr;
	unsigned long xpause;

	++ptr;
	if ((xpause = strtoul(ptr, (char **)&ptr, 10)) > 0)
		pcspkr_output(&pcsp, 0, 0, 48000 * 240 / (xpause * glob_spd));
	else
		fprintf(stderr, "** Invalid command ignored: P0\n");
	return ptr - origptr;
}

/**
 * parse_arg_t - set playing tempo
 */
static unsigned int parse_arg_t(const char *origptr)
{
	const char *ptr = origptr;
	int n;

	++ptr;
	if ((n = strtoul(ptr, (char **)&ptr, 10)) > 0)
		glob_spd = n;
	else
		fprintf(stderr, "** Invalid command ignored: T0\n");

	return ptr - origptr;
}

/**
 * parse_arg_x - parse QPF-VARPTR style commands
 */
static unsigned int parse_arg_x(const char *origptr)
{
	const char *ptr = origptr, *var_begin, *var_end = NULL, *data;
	char var_name[64] = {};
	size_t var_size;

	++ptr;
	while (HX_isspace(*ptr))
		++ptr;
	if (*ptr != '(') {
		fprintf(stderr, "** Syntax error, no '(' following 'X'\n");
		return ptr - origptr;
	}

	++ptr;
	while (HX_isspace(*ptr))
		++ptr;
	var_begin = ptr;
	while (!HX_isspace(*ptr) && *ptr != '\0' && *ptr != ')')
		var_end = ++ptr;
	while (HX_isspace(*ptr))
		++ptr;
	if (*ptr != ')') {
		fprintf(stderr, "** Syntax error, no ')' after 'X'\n");
		return ptr - origptr;
	}

	++ptr;
	var_size = var_end - var_begin;
	strncpy(var_name, var_begin, (var_size + 1 > sizeof(var_name)) ?
	        sizeof(var_name) : var_size);

	if ((data = HXmap_get(varmap, var_name)) == NULL) {
		fprintf(stderr, "** Variable \"%s\" not found\n", var_name);
		return ptr - origptr;
	}

	parse_str(data);
	return ptr - origptr;
}

static void parse_str(const char *ptr)
{
	while (*ptr != '\0') {
		if (HX_isspace(*ptr)) {
			++ptr;
			continue;
		}
		switch (*ptr) {
			case 'a' ... 'g':
				ptr += parse_arg_ag(ptr);
				break;
			case '<':
				if (--glob_octave < 0)
					glob_octave = 0;
				fprintf(stderr, "<");
				++ptr;
				break;
			case '>':
				if (++glob_octave > MAX_OCTAVES)
					glob_octave = MAX_OCTAVES;
				fprintf(stderr, ">");
				++ptr;
				break;
			case 'l':
				ptr += parse_arg_l(ptr);
				break;
			case 'm':
				ptr += parse_arg_m(ptr);
				break;
			case 'n':
				ptr += parse_arg_n(ptr);
				break;
			case 'o':
				ptr += parse_arg_o(ptr);
				break;
			case 'p':
				ptr += parse_arg_p(ptr);
				break;
			case 't':
				ptr += parse_arg_t(ptr);
				break;
			case 'x':
				ptr += parse_arg_x(ptr);
				break;
			default:
				fprintf(stderr, "** Unknown command ignored: %c\n", *ptr);
				break;
		}
	}
}

/**
 * parse_var - parse a variable definition ("$x o2...")
 */
static void parse_var(FILE *fp, char *line)
{
	hxmc_t *ws = HXmc_strinit(line);
	char *key = ws + 1, *val = key;

	while (!HX_isspace(*val) && *val != '\0')
		++val;
	*val++ = '\0';
	while (HX_isspace(*val))
		++val;

	HXmap_add(varmap, key, val);
	HXmc_free(ws);
}

static void parse_file(const char *fn)
{
	char *ln = NULL;
	FILE *fp;

	if (strcmp(fn, "-") == 0)
		fp = fdopen(STDIN_FILENO, "r");
	else
		fp = fopen(fn, "r");

	if (fp == NULL) {
		fprintf(stderr, "** Could not open %s: %s\n", fn, strerror(errno));
		return;
	}

	while (HX_getl(&ln, fp) != NULL) {
		HX_chomp(ln);
		HX_strlower(ln);
		if (*ln == '#' || *ln == '\0') {
			continue;
		} else if (*ln == '$') {
			parse_var(fp, ln);
			continue;
		}
		parse_str(ln);
	}

	HXmc_free(ln);
}

/**
 * init_maps - generate frequency and lookup tables
 */
static void init_maps(void)
{
#define ADD(k, v) HXmap_add(keymap, (k), reinterpret_cast(const void *, static_cast(long, (v))));
	int n;

	keymap = HXmap_init(HXMAPT_DEFAULT, HXMAP_SKEY);
	varmap = HXmap_init(HXMAPT_DEFAULT, HXMAP_SCKEY | HXMAP_SCDATA);
	if (keymap == NULL || varmap == NULL) {
		perror("HXmap_init()");
		exit(EXIT_FAILURE);
	}
	for (n = 0; n < MAX_OCTAVES * 12; ++n)
		notemap[n] = 440 * pow(2, (double)(n - 33) / 12);

	ADD("c-", -1);
	ADD("c", 0);
	ADD("c#", 1);
	ADD("c+", 1);
	ADD("d-", 1);
	ADD("d", 2);
	ADD("d#", 2);
	ADD("d+", 3);
	ADD("e-", 3);
	ADD("e", 4);
	ADD("e#", 5);
	ADD("e+", 5);
	ADD("f-", 4);
	ADD("f", 5);
	ADD("f#", 6);
	ADD("f+", 6);
	ADD("g-", 6);
	ADD("g", 7);
	ADD("g#", 8);
	ADD("g+", 8);
	ADD("a-", 8);
	ADD("a", 9);
	ADD("a#", 10);
	ADD("a+", 10);
	ADD("b-", 10);
	ADD("b", 11);
	ADD("b#", 12);
	ADD("b+", 12);
#undef ADD
}

int main(int argc, const char **argv)
{
	static const struct HXoption options_table[] = {
		{.sh = 'i', .type = HXTYPE_DOUBLE, .ptr = &pcsp.prop_sine,
		 .help = "Proportion of sine-wave calculation mixed in"},
		{.sh = 'q', .type = HXTYPE_DOUBLE, .ptr = &pcsp.prop_square,
		 .help = "Proportion of square-wave calculation mixed in"},
		{.sh = 'r', .type = HXTYPE_UINT, .ptr = &pcsp.sample_rate,
		 .help = "Sample rate (default: 48000)"},
		HXOPT_AUTOHELP,
		HXOPT_TABLEEND,
	};
	int ret;

	if ((ret = HX_init()) <= 0) {
		fprintf(stderr, "HX_init: %s\n", strerror(-ret));
		abort();
	}

	if (HX_getopt(options_table, &argc, &argv, HXOPT_USAGEONERR) !=
	    HXOPT_ERR_SUCCESS) {
		HX_exit();
		return EXIT_FAILURE;
	}

	pcsp.file_ptr = stdout;
	init_maps();

	if (argc == 1)
		parse_file("-");
	else
		while (--argc > 0)
			parse_file(*++argv);

	HX_exit();
	return EXIT_SUCCESS;
}

