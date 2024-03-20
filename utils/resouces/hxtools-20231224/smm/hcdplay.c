// SPDX-License-Identifier: MIT
/*
 *	hcdplay - Command-line interface to autonomous background CD playback
 *	written by Jan Engelhardt, 2013
 */
#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libHX/option.h>
#include <sys/ioctl.h>
#if defined(HAVE_LINUX_CDROM_H)
#	include <linux/cdrom.h>
#	define DEFAULT_ROM "/dev/sr0"
#elif defined(HAVE_SYS_CDIO_H)
#	include <sys/cdio.h>
#	define DEFAULT_ROM "/dev/cd0"
#else
#	error hcdplay not prepared for this platform
#endif

static char *cdev;
static unsigned int dump_toc, do_start, do_pause, do_resume;
static unsigned int do_play, do_stop, do_eject, do_close;

static bool cdp_get_options(int *argc, const char ***argv)
{
	static const struct HXoption option_table[] = {
		{.sh = 'C', .ln = "device", .type = HXTYPE_NONE, .ptr = &do_close,
		 .help = "Close the CD-ROM"},
		{.sh = 'D', .ln = "device", .type = HXTYPE_STRING, .ptr = &cdev,
		 .help = "CD-ROM device path", .htyp = "FILE"},
		{.sh = 'E', .ln = "eject",  .type = HXTYPE_NONE, .ptr = &do_eject,
		 .help = "Eject the CD-ROM"},
		{.sh = 'P', .ln = "pause",  .type = HXTYPE_NONE, .ptr = &do_pause,
		 .help = "Pause playback"},
		{.sh = 'R', .ln = "resume", .type = HXTYPE_NONE, .ptr = &do_resume,
		 .help = "Resume playback"},
		{.sh = 'S', .ln = "stop",   .type = HXTYPE_NONE, .ptr = &do_stop,
		 .help = "Stop playback"},
		{.sh = 'T', .ln = "toc",    .type = HXTYPE_NONE, .ptr = &dump_toc,
		 .help = "Show TOC information"},
		{.sh = 'p', .ln = "play",   .type = HXTYPE_NONE, .ptr = &do_play,
		 .help = "Playback track # or tracks #-#"},
		{.sh = 's', .ln = "start",  .type = HXTYPE_NONE, .ptr = &do_start,
		 .help = "Start playback (?)"},
		HXOPT_AUTOHELP,
		HXOPT_TABLEEND,
	};
	int ret;

	ret = HX_getopt(option_table, argc, argv, HXOPT_USAGEONERR);
	if (ret != HXOPT_ERR_SUCCESS)
		return false;
	if (cdev == NULL)
		cdev = DEFAULT_ROM;
	return true;
}

#ifdef HAVE_LINUX_CDROM_H
static int ioc_allow(int fd) { return 0; }
static int ioc_eject(int fd) { return ioctl(fd, CDROMEJECT); }
static int ioc_close(int fd) { return ioctl(fd, CDROMCLOSETRAY); }
static int ioc_start(int fd) { return ioctl(fd, CDROMSTART); }
static int ioc_pause(int fd) { return ioctl(fd, CDROMPAUSE); }
static int ioc_resume(int fd) { return ioctl(fd, CDROMRESUME); }
static int ioc_stop(int fd) { return ioctl(fd, CDROMSTOP); }
static int ioc_readtoc(int fd, uint8_t *first, uint8_t *last) {
	struct cdrom_tochdr t;
	int ret = ioctl(fd, CDROMREADTOCHDR, &t);
	if (ret < 0)
		return ret;
	*first = t.cdth_trk0;
	*last  = t.cdth_trk1;
	return 0;
}
static int ioc_playtrk(int fd, uint8_t first, uint8_t last) {
	struct cdrom_ti t;
	t.cdti_trk0 = first;
	t.cdti_trk1 = last;
	t.cdti_ind0 = t.cdti_ind1 = 0;
	return ioctl(fd, CDROMPLAYTRKIND, t);
}
#endif
#ifdef HAVE_SYS_CDIO_H
static int ioc_allow(int fd) { return ioctl(fd, CDIOCALLOW); }
static int ioc_eject(int fd) { return ioctl(fd, CDIOCEJECT); }
static int ioc_close(int fd) { return ioctl(fd, CDIOCCLOSE); }
static int ioc_start(int fd) { return ioctl(fd, CDIOCSTART); }
static int ioc_pause(int fd) { return ioctl(fd, CDIOCPAUSE); }
static int ioc_resume(int fd) { return ioctl(fd, CDIOCRESUME); }
static int ioc_stop(int fd) { return ioctl(fd, CDIOCSTOP); }
static int ioc_readtoc(int fd, uint8_t *first, uint8_t *last) {
	struct ioc_toc_header t;
	int ret = ioctl(fd, CDIOREADTOCHEADER, &t);
	if (ret < 0)
		return ret;
	*first = t.starting_track;
	*last  = t.ending_track;
	return 0;
}
static int ioc_playtrk(int fd, int first, int last) {
	struct ioc_play_track t;
	t.start_track = first;
	t.end_track = last;
	t.start_index = t.end_index = 1;
	return ioctl(fd, CDIOCPLAYTRACKS, &t);
}
#endif

static int do_perform(int fd, int argc, const char **argv)
{
	/*
	 * Ejecting and closing the CD-ROM tray does not require reading
	 * of the TOC header first (at least on FreeBSD).
	 */
	if (do_eject) {
		ioc_allow(fd);
		if (ioc_eject(fd) < 0)
			perror("ioctl CDROMEJECT");
		/*
		 * It is pointless to do anything after CD-ROM had been
		 * ejected, so exit early.
		 */
		return EXIT_SUCCESS;
	}
	if (do_close) {
		ioc_allow(fd);
		if (ioc_close(fd) < 0)
			perror("ioctl CDROMCLOSETRAY");
	}
	uint8_t first = 0, last = 0;
	if (ioc_readtoc(fd, &first, &last) < 0) {
		perror("ioctl CDROMREADTOCHDR");
		return EXIT_FAILURE;
	}
	if (dump_toc)
		printf("Tracks: %u-%u\n", first, last);
	if (do_start)
		if (ioc_start(fd) < 0)
			perror("ioctl CDROMSTART");
	if (do_pause)
		if (ioc_pause(fd) < 0)
			perror("ioctl CDROMPAUSE");
	if (do_resume)
		if (ioc_resume(fd) < 0)
			perror("ioctl CDROMRESUME");
	if (do_play) {
		if (argc >= 2)
			first = strtoul(argv[1], NULL, 0);
		if (argc >= 3)
			last = strtoul(argv[2], NULL, 0);
		if (ioc_playtrk(fd, first, last) < 0)
			perror("ioctl CDROMPLAYTRKIND");
	}
	if (do_stop)
		if (ioc_stop(fd) < 0)
			perror("ioctl CDROMSTOP");
	return EXIT_SUCCESS;
}

int main(int argc, const char **argv)
{
	int fd;

	if (!cdp_get_options(&argc, &argv))
		return EXIT_FAILURE;

	fd = open(cdev, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open %s: %s\n",
		        cdev, strerror(errno));
		return EXIT_FAILURE;
	}

	int ret = do_perform(fd, argc, argv);
	close(fd);
	return ret;
}
