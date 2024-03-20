// SPDX-License-Identifier: MIT
/*
 *	system clock info
 *	written by Jan Engelhardt, 2011
 */
#define _FILE_OFFSET_BITS 64
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libHX/defs.h>
#ifdef __linux__
#	ifndef CLOCK_MONOTONIC_RAW
#		define CLOCK_MONOTONIC_RAW 4
#	endif
#	ifdef CLOCK_REALTIME_COARSE
#		define CLOCK_REALTIME_COARSE 5
#	endif
#	ifdef CLOCK_MONOTONIC_COARSE
#		define CLOCK_MONOTONIC_COARSE 6
#	endif
#	ifdef CLOCK_BOOTTIME
#		define CLOCK_BOOTTIME 7
#	endif
#	ifdef CLOCK_REALTIME_ALARM
#		define CLOCK_REALTIME_ALARM 8
#	endif
#	ifndef CLOCK_BOOTTIME_ALARM
#		define CLOCK_BOOTTIME_ALARM 9
#	endif
#	ifndef CLOCK_SGI_CYCLE
#		define CLOCK_SGI_CYCLE 10
#	endif
#	ifndef CLOCK_TAI
#		define CLOCK_TAI 11
#	endif
#endif

#define E(s) {s, #s}

static const struct clock_desc {
	unsigned int id;
	const char *name;
} clocks[] = {
	E(CLOCK_REALTIME),
#ifdef CLOCK_MONOTONIC
	E(CLOCK_MONOTONIC),
#endif
#ifdef CLOCK_MONOTONIC_RAW
	E(CLOCK_MONOTONIC_RAW),
#endif
#ifdef CLOCK_PROCESS_CPUTIME_ID
	E(CLOCK_PROCESS_CPUTIME_ID),
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
	E(CLOCK_THREAD_CPUTIME_ID),
#endif
#ifdef CLOCK_MONOTONIC_RAW
	E(CLOCK_MONOTONIC_RAW),
#endif
#ifdef CLOCK_REALTIME_COARSE
	E(CLOCK_REALTIME_COARSE),
#endif
#ifdef CLOCK_MONOTONIC_COARSE
	E(CLOCK_MONOTONIC_COARSE),
#endif
#ifdef CLOCK_BOOTTIME
	E(CLOCK_BOOTTIME),
#endif
#ifdef CLOCK_REALTIME_ALARM
	E(CLOCK_REALTIME_ALARM),
#endif
#ifdef CLOCK_BOOTTIME_ALARM
	E(CLOCK_BOOTTIME_ALARM),
#endif
#ifdef CLOCK_SGI_CYCLE
	E(CLOCK_SGI_CYCLE),
#endif
#ifdef CLOCK_TAI
	E(CLOCK_TAI),
#endif
};

static const char *ci_resolution(char *buf, size_t size,
    const struct timespec *tp)
{
	static const char unit_names[][3] = {"ns", "µs", "ms", "s "};
	unsigned int unit_idx = 0;
	long nsec = tp->tv_nsec;

	if (tp->tv_sec > 0) {
		snprintf(buf, size, "%llu ns",
			tp->tv_sec * 1000000000ULL + nsec);
		return buf;
	}

	while (nsec >= 1000 && unit_idx < ARRAY_SIZE(unit_names) - 1 &&
	    nsec % 1000 == 0) {
		++unit_idx;
		nsec /= 1000;
	}
	snprintf(buf, size, "%lu %s", nsec, unit_names[unit_idx]);
	return buf;
}

int main(void)
{
	const struct clock_desc *c;
	struct timespec tp, res;
	unsigned int i;
	char buf[32];

	printf("%24s: %20s  %10s\n", "Name", "Current value", "Resolution");
	for (i = 0; i < ARRAY_SIZE(clocks); ++i) {
		c = &clocks[i];

		if (clock_gettime(c->id, &tp) < 0 ||
		    clock_getres(c->id, &res) < 0) {
			fprintf(stderr, "%s: %s\n", c->name, strerror(errno));
			continue;
		}

		printf("%24s: %10lu.%09lu  %10s\n",
			c->name,
			static_cast(unsigned long, tp.tv_sec),
			static_cast(unsigned long, tp.tv_nsec),
			ci_resolution(buf, sizeof(buf), &res));
	}
	return EXIT_SUCCESS;
}
