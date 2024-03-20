// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2002-2007 Jan Engelhardt
/*
 *	pcspkr_pcm.c - output as PCM
 */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcspkr.h"

/**
 * dynamic_qi
 * @pcsp:	pc speaker struct
 * @x:		time
 *
 * A pure sine wave does not sound at all like a PC speaker. A square wave on
 * the other hand is very hard (hardness, not difficulty). Mix square and sine
 * according to parameters.
 */
static inline double dynamic_qi(const struct pcspkr *pcsp, double x)
{
	double i = sin(x);
	double q = (i > 0) ? 1 : -1;

	return (pcsp->prop_square * q + pcsp->prop_sine * i) /
	       (pcsp->prop_square + pcsp->prop_sine);
}

/**
 * pcspkr_output - produce waves
 * @state:	pcspkr state
 * @frequency:	tone frequency
 * @duration:	duration, in samples
 *		(number of samples in a second given by @state->sampling_rate)
 * @af_pause:	after-pause, in samples
 *
 * Outputs 16-bit PCM samples of a @freq Hz tone for @duration to @sp->fp,
 * with an optional pause.
 */
void pcspkr_output(const struct pcspkr *pcsp, long frequency,
    long duration, long af_pause)
{
	int16_t value;
	long sample;

	for (sample = 0; sample < duration; ++sample) {
		value = 32767 * dynamic_qi(pcsp,
		        2.0 * M_PI * frequency * sample / pcsp->sample_rate) *
		        pcsp->volume;
		fwrite(&value, sizeof(value), 1, pcsp->file_ptr);
	}
	value = 0;
	for (sample = 0; sample < af_pause; ++sample)
		fwrite(&value, sizeof(value), 1, pcsp->file_ptr);
}

void pcspkr_silence(const struct pcspkr *pcsp, long duration)
{
	static const uint16_t value_16 = 0;
	long sample;

	for (sample = 0; sample < duration; ++sample)
		fwrite(&value_16, sizeof(value_16), 1, pcsp->file_ptr);
}
