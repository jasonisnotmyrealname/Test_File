/****************************************************************************
 *
 *   Copyright (c) 2012, 2013 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file drv_hrt.c
 *
 * High-resolution timer with callouts and timekeeping.
 */


#include <drv_hrt.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

static hrt_abstime px4_timestart = 0;

/*
 * Convert a timespec to absolute time.
 */
hrt_abstime ts_to_abstime(struct timespec *ts)
{
	hrt_abstime	result;

	result = (hrt_abstime)(ts->tv_sec) * 1000000;
	result += ts->tv_nsec / 1000;

	return result;
}

int clock_gettime(int clk_id, struct timespec *t)
{
	struct timeval now;
	int rv = gettimeofday(&now, nullptr);

	if (rv) {
		return rv;
	}

	t->tv_sec = now.tv_sec;
	t->tv_nsec = now.tv_usec * 1000;

	return 0;
}

/*
 * Get absolute time.
 */
hrt_abstime hrt_absolute_time(void)
{

	hrt_abstime ret;

	ret = _hrt_absolute_time_internal();

	return ret;
}

/*
 * Get absolute time.
 */
hrt_abstime _hrt_absolute_time_internal(void)
{
	struct timespec ts;

	if (!px4_timestart) {
		px4_clock_gettime(CLOCK_MONOTONIC, &ts);
		px4_timestart = ts_to_abstime(&ts);
	}

	px4_clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts_to_abstime(&ts) - px4_timestart;
}

