// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2007 Jan Engelhardt
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <libHX/string.h>

struct del {
	void operator()(FILE *x) const { fclose(x); }
};

static void do_file(const char *filename)
{
	std::ifstream strm;
	strm.open(filename);
	if (!strm.is_open()) {
		fprintf(stderr, "Cannot read %s: %s\n", filename, strerror(errno));
		return;
	}
	printf("  # () [] {}\n");
	std::string line;
	ssize_t p1 = 0, p2 = 0, p3 = 0;
	size_t lnum = 0;
	do {
		getline(strm, line);
		HX_chomp(line.data());
		if (strm.rdstate() & (std::ios_base::eofbit |
		    std::ios_base::failbit | std::ios_base::badbit))
			break;
		p1 += std::count(&line[0], &line[line.size()], '(');
		p1 -= std::count(&line[0], &line[line.size()], ')');
		p2 += std::count(&line[0], &line[line.size()], '[');
		p2 -= std::count(&line[0], &line[line.size()], ']');
		p3 += std::count(&line[0], &line[line.size()], '{');
		p3 -= std::count(&line[0], &line[line.size()], '}');
		printf("%3zu %2zd %2zd %2zd %s\n", ++lnum, p1, p2, p3, line.c_str());
	} while (true);
	printf("\n");
}

int main(int argc, const char **argv)
{
	while (*++argv != nullptr)
		do_file(*argv);
	return EXIT_SUCCESS;
}
