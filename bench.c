#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench.h"

volatile sig_atomic_t done;

static void handler(int _)
{
	fputs("Handling...\n", stderr);
	done = 1;
}

void set(void)
{
	struct sigaction sa;

	fputs("Setting...\n", stderr);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1
	    || sigaction(SIGTERM, &sa, NULL) == -1)
		err(EXIT_FAILURE, "sigaction");
}

void put(struct timespec *t_i)
{
	struct timespec t_f;

	if (clock_gettime(CLOCK_REALTIME, &t_f) == -1)
		err(EXIT_FAILURE, "clock");
	t_f.tv_nsec -= t_i->tv_nsec;
	if (t_f.tv_nsec < 0) {
		t_f.tv_nsec += 1000 * 1000 * 1000;
		t_f.tv_nsec -= 1;
	}
	t_f.tv_sec -= t_i->tv_sec;
	(void)printf("%ld.%06ld\n", t_f.tv_sec, t_f.tv_nsec / 1000);
}
