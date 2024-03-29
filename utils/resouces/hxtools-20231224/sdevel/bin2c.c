// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2004-2008,2013-2015 Jan Engelhardt
/*
 *	bin2c - convert arbitrary files into C variable definitions
 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libHX/ctype_helper.h>
#include <libHX/defs.h>
#include <libHX/option.h>
#include <libHX/string.h>
#if CHAR_BIT > 8
#	error Sorry, we do not support CHAR_BIT > 8 yet.
#endif

/**
 * @cfp: 	file handle for output of the definition
 * 		(this can be the same as @hfile!)
 * @hfp:	file handle for output of the declaration
 * @vname:	name for the variable inside the outputted program
 * @ifp:	input file handle
 * @ifile:	input file name (as on the command line)
 * @ifile_path:	real file path (after possibly prepending @btc_prefix_directory)
 * @isize:	size of the input file in bytes
 */
struct btc_state {
	FILE *cfp, *hfp;
	hxmc_t *vname;
	FILE *ifp;
	const char *ifile, *ifile_path, *guard_name;
	size_t isize;
};

struct btc_operations {
	void (*global_header)(struct btc_state *);
	void (*global_footer)(struct btc_state *);
	void (*file_predecl)(struct btc_state *);
	void (*file_content)(struct btc_state *);
	void (*func_header)(struct btc_state *);
};

static hxmc_t *btc_cfile, *btc_hfile;
static hxmc_t *btc_guard_name;
static char *btc_prefix_directory;
static unsigned int btc_verbose, btc_emit_wxbitmap, btc_emit_ultra;
static const struct btc_operations *btc_ops;
static int btc_strip = -1;

static char *btc_tblquote(const void *vsrc, size_t input_size)
{
	const unsigned char *src = vsrc;
	size_t quoted_size = input_size * 5;
	char *out, *p;

	p = out = malloc(quoted_size + 1);
	if (out == NULL)
		abort();
	for (; input_size-- > 0; ++src) {
		if (*src == 0) {
			*p++ = '0';
		} else if (*src <= 07) {
			*p++ = '0';
			*p++ = '0' + (*src & 0007);
		} else if (HX_isprint(*src) && *src != '\'' && *src != '\\') {
			*p++ = '\'';
			*p++ = *src;
			*p++ = '\'';
		} else if (*src <= 077) {
			*p++ = '0';
			*p++ = '0' + ((*src & 0070) >> 3);
			*p++ = '0' + (*src & 0007);
		} else {
			*p++ = '0';
			*p++ = '0' + ((*src & 0700) >> 6);
			*p++ = '0' + ((*src & 0070) >> 3);
			*p++ = '0' + (*src & 0007);
		}
		*p++ = ',';
	}
	*p = '\0';
	return out;
}

/*
 * character/sequence		chance		expansion
 * 00,22,5C			3/256		2 (\x)
 * 01..07,digit			7/256*10/256	4 (\000 .. \007)
 * 01..07,nondigit		7/256*246/256	2 (\0 .. \7)
 * 08..1F,digit			24/256*10/256	4 (\010 .. \031)
 * 08..1F,nondigit		24/256*246/256	3 (\10 .. \31)
 * 20..21,23..3E,40..5B,5D..7E	92/256		1
 * 3F:3F			1/256*1/256	3 (\077)
 * 3F:!3F			1/256*255/256	1 (?)
 * 7F..FF			129/256		4 (\177 .. \377)
 * average bytes per byte around 2.74411 (the 3F cases are hard to determine)
 */
static char *btc_memquote(const void *vsrc, size_t input_size)
{
	size_t quoted_size = input_size * 4 + 1;
	char *out, *p;
	const unsigned char *src = vsrc;
	bool qthis, qnext;

	p = out = malloc(quoted_size + 1);
	qnext = input_size > 1 && (src[0] == '\"' || src[0] == '\\' || !HX_isprint(src[0]));
	for (; input_size-- > 0; ++src) {
		qthis = qnext;
		qnext = input_size > 0 && (!HX_isprint(src[1]) ||
		        src[1] == '\"' || src[1] == '\\' ||
		        (src[1] == '?' && src[0] == '?' && !qthis));
		if (!qthis) {
			*p++ = *src;
			continue;
		}
		bool full = input_size == 0 || (!qnext && HX_isdigit(src[1]));
		*p++ = '\\';
		if (*src == '\"' || *src == '\\') {
			*p++ = *src;
			continue;
		}
		if (full || *src > 0070)
			*p++ = '0' + ((*src & 0700) >> 6);
		if (full || *src > 0007)
			*p++ = '0' + ((*src & 0070) >> 3);
		*p++ = '0' + (*src & 0007);
	}
	*p = '\0';
	return out;
}

static char *btc_strquote(const char *src)
{
	return btc_memquote(src, strlen(src));
}

static void btc_generic_global_header(struct btc_state *state)
{
	fprintf(state->hfp, "/* Autogenerated by hxtools bin2c */\n");
	if (state->guard_name)
		fprintf(state->hfp, "#ifndef %s\n#define %s 1\n\n",
		        state->guard_name, state->guard_name);
	fprintf(state->hfp, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n");
}

static void btc_generic_global_footer(struct btc_state *state)
{
	fprintf(state->hfp, "\n#ifdef __cplusplus\n} /* extern \"C\" */\n#endif\n");
	if (state->guard_name)
		fprintf(state->hfp, "\n\n#endif /* %s */\n", state->guard_name);
}

/**
 * Output the binary stream in a "normal" fashion.
 */
static void btc_stdc_file_content(struct btc_state *state)
{
	char input_buf[4096], *output_buf;
	size_t input_len;

	fprintf(state->cfp, "/* Autogenerated from %s */\n", state->ifile);
	if (state->cfp != state->hfp) {
		fprintf(state->hfp, "extern const unsigned char bin2c_%s[%" HX_SIZET_FMT "u];\n",
		        state->vname, state->isize);
		fprintf(state->cfp, "const unsigned char bin2c_%s[%" HX_SIZET_FMT "u] = {",
		        state->vname, state->isize);
	} else {
		fprintf(state->cfp,
		        "static const unsigned char bin2c_%s[%" HX_SIZET_FMT "u] = {",
		        state->vname, state->isize);
	}

	while ((input_len = fread(input_buf, 1, sizeof(input_buf),
	    state->ifp)) > 0)
	{
		output_buf = btc_tblquote(input_buf, input_len);
		if (output_buf == NULL)
			abort();
		fwrite(output_buf, strlen(output_buf), 1, state->cfp);
		free(output_buf);
	}
	fprintf(state->cfp, "};\n");
}

static const struct btc_operations btc_stdc_ops = {
	.global_header = btc_generic_global_header,
	.global_footer = btc_generic_global_footer,
	.file_content  = btc_stdc_file_content,
};

/**
 * Output the binary stream as a string literal.
 * "Ultra mode".
 *
 * C++ is very picky about sizes. "ABC" is of type char[4] and must be
 * assigned to a char[N] with N >= 4. In C, "ABC" could be assigned to
 * char[N] with N >= 3 too.
 * Yes, this means the array contains a trailing \0 byte.
 * You can either live with it (JPEG and PNG seem to permit trailing garbage),
 * or substract 1 when using sizeof/ARRAY_SIZE.
 */
static void btc_ultra_file_content(struct btc_state *state)
{
	char input_buf[4096], *output_buf;
	size_t input_len;

	fprintf(state->cfp, "/* Autogenerated from %s */\n", state->ifile);

	if (state->cfp != state->hfp) {
		fprintf(state->hfp, "extern const unsigned char bin2c_%s[%" HX_SIZET_FMT "u];\n",
		        state->vname, state->isize + 1);
		fprintf(state->cfp, "const unsigned char bin2c_%s[%" HX_SIZET_FMT "u] = \"",
		        state->vname, state->isize + 1);
	} else {
		fprintf(state->cfp,
		        "static const unsigned char bin2c_%s[%" HX_SIZET_FMT "u] = \"",
		        state->vname, state->isize + 1);
	}

	while ((input_len = fread(input_buf, 1, sizeof(input_buf),
	    state->ifp)) > 0)
	{
		output_buf = btc_memquote(input_buf, input_len);
		if (output_buf == NULL)
			abort();
		fwrite(output_buf, strlen(output_buf), 1, state->cfp);
		free(output_buf);
	}
	fprintf(state->cfp, "\";\n");
}

static const struct btc_operations btc_ultra_ops = {
	.global_header = btc_generic_global_header,
	.global_footer = btc_generic_global_footer,
	.file_content  = btc_ultra_file_content,
};

static void btc_wxbitmap_global_header(struct btc_state *state)
{
	fprintf(state->hfp, "/* Autogenerated by hxtools bin2c */\n");
	if (state->guard_name != NULL)
		fprintf(state->hfp, "#ifndef %s\n#define %s 1\n\n",
		        state->guard_name, state->guard_name);

	fprintf(state->hfp, "class wxBitmap;\n\n");
	fprintf(state->hfp, "extern \"C\" void bin2c_init_%s(void);\n",
	        state->guard_name);

	fprintf(state->cfp, "#include <wx/bitmap.h>\n"
	        "#include <wx/image.h>\n"
	        "#include <wx/mstream.h>\n");
}

static void btc_wxbitmap_global_footer(struct btc_state *state)
{
	fprintf(state->cfp, /* { */ "}\n");
	if (state->guard_name != NULL)
		fprintf(state->hfp, "\n\n#endif /* %s */\n", state->guard_name);
}

static void btc_wxbitmap_file_predecl(struct btc_state *state)
{
	fprintf(state->cfp, "wxBitmap *bin2c_%s;\n", state->vname);
	fprintf(state->hfp, "extern wxBitmap *bin2c_%s;\n",
	        state->vname);
}

static void btc_wxbitmap_func_header(struct btc_state *state)
{
	fprintf(state->cfp, "void bin2c_init_%s(void)\n{\n" /* } */,
	        state->guard_name);
}

static void btc_wxbitmap_file_content(struct btc_state *state)
{
	char input_buf[4096], *output_buf;
	size_t input_len;

	fprintf(state->cfp, "\t{\n\t\twxMemoryInputStream sm(\"");
	while ((input_len = fread(input_buf, 1, sizeof(input_buf),
	    state->ifp)) > 0)
	{
		output_buf = btc_memquote(input_buf, input_len);
		if (output_buf == NULL)
			abort();
		fwrite(output_buf, strlen(output_buf), 1, state->cfp);
		free(output_buf);
	}
	fprintf(state->cfp, "\", %" HX_SIZET_FMT "u);\n\t\tbin2c_%s = new wxBitmap(wxImage(sm, wxBITMAP_TYPE_ANY), -1);\n\t}\n",
	        state->isize, state->vname);
}

static const struct btc_operations btc_wxbitmap_ops = {
	.global_header = btc_wxbitmap_global_header,
	.global_footer = btc_wxbitmap_global_footer,
	.file_predecl  = btc_wxbitmap_file_predecl,
	.file_content  = btc_wxbitmap_file_content,
	.func_header   = btc_wxbitmap_func_header,
};

/**
 * Construct a variable identifier from the given input filename.
 * Just substitute all non-alphanumeric characters by underscore.
 */
static hxmc_t *btc_construct_vname(const char *cfile)
{
	const char *eof;
	hxmc_t *vname;
	int strip = btc_strip;
	char *p;

	/* move @cfile forward as many paths as btc_strip specifies */
	for (; strip > 0; --strip) {
		while (*cfile != '/' && *cfile != '\0')
			++cfile;
		while (*cfile == '/')
			++cfile;
	}
	/* move @eof backward as many paths as btc_strip specifies */
	eof = cfile + strlen(cfile) - 1;
	for (; strip < 0; ++strip) {
		while (eof >= cfile && *eof == '/')
			--eof;
		while (eof >= cfile && *eof != '/')
			--eof;
		cfile = eof + 1;
	}

	vname = HXmc_strinit(cfile);
	if (vname == NULL)
		return NULL;
	if (!HX_isalpha(*cfile) && *cfile != '_') {
		/*
		 * The identifier must begin with [a-z_]; in any case, it
		 * cannot begin with a digit or any special character.
		 */
		if (HXmc_strpcat(&vname, "_") == NULL)
			/* can happen but unlikely: just abort w/o errormsg */
			abort();
		p = vname + 1;
	} else {
		p = vname;
	}
	for (; *cfile != '\0'; ++cfile)
		*p++ = HX_isalnum(*cfile) ? *cfile : '_';
	return vname;
}

/**
 * Process a single input file.
 */
static int btc_process_single(struct btc_state *state)
{
	struct stat sb;

	if (stat(state->ifile_path, &sb) != 0) {
		fprintf(stderr, "ERROR: Cannot stat %s: %s\n",
		        state->ifile, strerror(errno));
		return -errno;
	}

	state->ifp = fopen(state->ifile_path, "r");
	if (state->ifp == NULL) {
		fprintf(stderr, "bin2c: ERROR: Could not open %s for "
		        "reading: %s\n", state->ifile, strerror(errno));
		return -errno;
	}
	state->isize = sb.st_size;
	state->vname = btc_construct_vname(state->ifile);
	btc_ops->file_content(state);
	HXmc_free(state->vname);
	fclose(state->ifp);
	return 0;
}

static int btc_process_single_pf(struct btc_state *state)
{
	if (btc_prefix_directory == NULL) {
		state->ifile_path = state->ifile;
		return btc_process_single(state);
	}

	hxmc_t *newfile = HXmc_strinit(btc_prefix_directory);
	int ret = 0;

	if (newfile == NULL || HXmc_strcat(&newfile, "/") == NULL ||
	    HXmc_strcat(&newfile, state->ifile) == NULL) {
		ret = -errno;
	} else {
		state->ifile_path = newfile;
		ret = btc_process_single(state);
	}
	HXmc_free(newfile);
	return ret;
}

/**
 * Process all files given in @argv.
 */
static int btc_start(const char **argv)
{
	struct btc_state state;
	const char **arg;
	char *result;
	int ret = 0;

	state.hfp = fopen(btc_hfile, "w");
	if (state.hfp == NULL) {
		fprintf(stderr, "bin2c: ERROR: Could nt open %s for writing: "
		        "%s\n", btc_hfile, strerror(errno));
		return -errno;
	}

	if (btc_cfile != NULL) {
		state.cfp = fopen(btc_cfile, "w");
		if (state.cfp == NULL) {
			fprintf(stderr, "bin2c: ERROR: Could not open %s for "
			        "writing: %s\n", btc_cfile, strerror(errno));
			return -errno;
		}
		fprintf(state.cfp, "/* Autogenerated by hxtools bin2c */\n");
		result = btc_strquote(btc_hfile);
		fprintf(state.cfp, "#include \"%s\"\n", result);
		free(result);
	} else {
		state.cfp = state.hfp;
	}

	state.guard_name = btc_guard_name;
	btc_ops->global_header(&state);

	if (btc_ops->file_predecl != NULL)
		for (arg = argv + 1; *arg != NULL; ++arg) {
			state.ifile = *arg;
			state.vname = btc_construct_vname(state.ifile);
			btc_ops->file_predecl(&state);
			HXmc_free(state.vname);
		}

	if (btc_ops->func_header != NULL)
		btc_ops->func_header(&state);

	for (arg = argv + 1; *arg != NULL; ++arg) {
		state.ifile = *arg;
		ret = btc_process_single_pf(&state);
		if (ret != 0)
			break;
	}

	btc_ops->global_footer(&state);
	fclose(state.hfp);
	if (state.cfp != state.hfp)
		fclose(state.cfp);
	if (ret != EXIT_SUCCESS) {
		if (btc_cfile != NULL)
			unlink(btc_cfile);
		if (btc_hfile != NULL)
			unlink(btc_hfile);
	}
	return ret;
}

static const struct HXoption btc_option_table[] = {
	{.sh = 'C', .type = HXTYPE_MCSTR, .ptr = &btc_cfile,
	 .help = "Filename for the output .c file", .htyp = "FILE"},
	{.sh = 'D', .type = HXTYPE_STRING, .ptr = &btc_prefix_directory,
	 .help = "Prefix input filenames with this directory", .htyp = "DIR"},
	{.sh = 'G', .type = HXTYPE_MCSTR, .ptr = &btc_guard_name,
	 .help = "Name for the header's include guard"},
	{.sh = 'H', .type = HXTYPE_MCSTR, .ptr = &btc_hfile,
	 .help = "Filename for the output .h file", .htyp = "FILE"},
	{.sh = 'p', .type = HXTYPE_INT, .ptr = &btc_strip,
	 .help = "Strip N path components (keep -N if N is negative)", .htyp = "N"},
	{.sh = 'v', .type = HXTYPE_NONE, .ptr = &btc_verbose,
	 .help = "Be verbose during operation"},
	{.ln = "wxbitmap", .type = HXTYPE_NONE, .ptr = &btc_emit_wxbitmap,
	 .help = "Generate wxBitmap variables rather than plain data"},
	{.ln = "ultra", .type = HXTYPE_NONE, .ptr = &btc_emit_ultra,
	 .help = "Generate variables using +1-sized(!) string literals"},
	HXOPT_AUTOHELP,
	HXOPT_TABLEEND,
};

/**
 * Detect any C/non-C file naming and either return the corresponding
 * header file suffix, or report the suffix problem.
 */
static const char *btc_known_c_suffix(const char *s)
{
	bool modern_cp, archaic_cp;

	if (strcmp(s, ".c") == 0)
		return ".h";

	modern_cp = strcmp(s, ".cpp") == 0 || strcmp(s, ".cxx") == 0;
	archaic_cp = strcmp(s, ".cc") == 0 || strcmp(s, ".C") == 0 ||
	             strcmp(s, ".cp") == 0 || strcmp(s, ".CPP") == 0;
	if (modern_cp || archaic_cp)
		fprintf(stderr, "bin2c: WARNING: bin2c outputs C code, not "
		        "C++ -- It is wrong to call the output file %s!\n", s);
	else
		fprintf(stderr, "bin2c: WARNING: The suffix %s is "
		        "unknown!\n", s);
	return NULL;
}

/**
 * Detect any C++/non-C++ file naming and either return the corresponding
 * header file suffix, or report the suffix problem.
 */
static const char *btc_known_cpp_suffix(const char *s)
{
	if (strcmp(s, ".c") == 0)
		fprintf(stderr, "bin2c: WARNING: bin2c is set to outputs "
		        "C++/wxWidgets code, not C -- It is wrong to call the "
		        "output file %s!\n", s);

	if (strcmp(s, ".cpp") == 0)
		return ".hpp";
	else if (strcmp(s, ".cxx") == 0)
		return ".hxx";
	else
		fprintf(stderr, "bin2c: WARNING: The suffix %s is "
		        "unknown!\n", s);
	/* The other extensions are a bunch of whack */
	return NULL;
}

/**
 * Construct a filename from the given .c-ish filename.
 * Warn about C++ naming, because we are really generating a C program,
 * not a C++ program.
 */
static hxmc_t *btc_construct_hname(const char *cfile)
{
	hxmc_t *hfile = HXmc_strinit(cfile);
	const char *suffix, *repl;

	if (hfile == NULL)
		return NULL;
	suffix = strrchr(hfile, '.');
	if (suffix == NULL) {
		if (HXmc_strcat(&hfile, ".h") == NULL)
			goto out;
		return hfile;
	}
	if (btc_emit_wxbitmap)
		repl = btc_known_cpp_suffix(suffix);
	else
		repl = btc_known_c_suffix(suffix);
	if (repl == NULL) {
		if (HXmc_strcat(&hfile, ".h") == NULL)
			goto out;
		return hfile;
	}

	/* We have a replacement… */
	if (HXmc_trunc(&hfile, suffix - hfile) == NULL)
		abort(); /* never to happen */
	if (HXmc_strcat(&hfile, repl) == NULL)
		goto out;
	return hfile;

 out:
	HXmc_free(hfile);
	return NULL;
}

/**
 * Construct an identifer name for the guarding #define from the filename.
 * Transformation: a-z -> A-Z, 0-9 -> (keep), * -> underscore.
 */
static hxmc_t *btc_construct_guard(const char *s)
{
	char *p, *guard = HXmc_strinit(s);

	if (guard == NULL)
		return NULL;
	if (!HX_isalpha(*s) && *s != '_') {
		/*
		 * The identifier must begin with [a-z_]; in any case, it
		 * cannot begin with a digit or any special character.
		 */
		if (HXmc_strpcat(&guard, "_") == NULL)
			/* can happen but unlikely: just abort w/o errormsg */
			abort();
		p = guard + 1;
	} else {
		p = guard;
	}

	for (; *s != '\0'; ++s)
		*p++ = HX_isalnum(*s) ? HX_toupper(*s) : '_';
	return guard;
}

/**
 * Parse options and fill in any defaults for the variables.
 */
static bool btc_get_options(int *argc, const char ***argv)
{
	int ret;

	ret = HX_getopt(btc_option_table, argc, argv, HXOPT_USAGEONERR);
	if (ret != HXOPT_ERR_SUCCESS)
		return false;
	if (btc_cfile != NULL && btc_hfile == NULL)
		btc_hfile = btc_construct_hname(btc_cfile);
	if (btc_hfile == NULL) {
		fprintf(stderr, "bin2c: you need to specify -C or -H, or both\n");
		return false;
	}
	if (btc_cfile != NULL && btc_guard_name == NULL)
		btc_guard_name = btc_construct_guard(btc_hfile);
	if (btc_verbose) {
		printf("C program file: %s\n",
		       (btc_cfile != NULL) ? btc_cfile : "");
		printf("C header file: %s\n", btc_hfile);
		printf("Header guard name: %s\n",
		       (btc_guard_name != NULL) ? btc_guard_name : "");
	}
	btc_ops = btc_emit_wxbitmap ? &btc_wxbitmap_ops :
	          btc_emit_ultra ? &btc_ultra_ops : &btc_stdc_ops;
	return true;
}

int main(int argc, const char **argv)
{
	int ret;

	if (!btc_get_options(&argc, &argv))
		return EXIT_FAILURE;
	ret = btc_start(argv);
	HXmc_free(btc_cfile);
	HXmc_free(btc_hfile);
	HXmc_free(btc_guard_name);
	return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
