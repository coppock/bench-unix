#include <err.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "bench.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int value;

static void touch(int val)
{
	int c;

	while (value == val)
		if ((c = pthread_cond_wait(&cond, &mutex)) != 0)
			errc(EXIT_FAILURE, c, "pthread_cond_wait");
	value = val;
	if ((c = pthread_cond_signal(&cond)) != 0)
		errc(EXIT_FAILURE, c, "pthread_cond_signal");
}

static void *start_routine(void *_)
{
	int c;

	if ((c = pthread_mutex_lock(&mutex)) != 0)
		errc(EXIT_FAILURE, c, "pthread_mutex_lock");
	while (!done) touch(1);
	if ((c = pthread_mutex_unlock(&mutex)) != 0)
		errc(EXIT_FAILURE, c, "pthread_mutex_unlock");
	return NULL;
}

int main(void)
{
	int c;
	pthread_t thread;

	set();
	if ((c = pthread_create(&thread, NULL, start_routine, NULL)) != 0)
		errc(EXIT_FAILURE, c, "pthread_create");
	if ((c = pthread_mutex_lock(&mutex)) != 0)
		errc(EXIT_FAILURE, c, "pthread_mutex_lock");
	while (!done) {
		struct timespec t;

		if (clock_gettime(CLOCK_REALTIME, &t) == -1)
			err(EXIT_FAILURE, "clock_gettime");
		for (int i = 0; !done && i < 1000; ++i) touch(0);
		put(&t);
	}
	if ((c = pthread_mutex_unlock(&mutex)) != 0)
		errc(EXIT_FAILURE, c, "pthread_mutex_unlock");
	if ((c = pthread_join(thread, NULL)) != 0)
		errc(EXIT_FAILURE, c, "pthread_join");
	return 0;
}
