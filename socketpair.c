#include <sys/socket.h>

#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include "bench.h"

int main(void)
{
	int fds[2], pid, buf;

	if (socketpair(PF_LOCAL, SOCK_STREAM, 0, fds) == -1)
		err(EXIT_FAILURE, "socketpair");

	if ((pid = fork()) == -1) {
		err(EXIT_FAILURE, "fork");
	}
	if (pid) {
		set();
		write(fds[1], &pid, sizeof(pid));
	}
	while (!done) {
		struct timespec t;

		if (pid && clock_gettime(CLOCK_REALTIME, &t) == -1)
			err(EXIT_FAILURE, "clock");
		for (int i = 0; !done && i < 1000; ++i) {
			read(fds[pid == 0], &buf, sizeof(buf));
			write(fds[pid != 0], &pid, sizeof(pid));
		}
		if (pid) put(&t);
	}
	if (pid && waitpid(pid, NULL, 0) == -1) err(EXIT_FAILURE, "wait");
	return 0;
}
