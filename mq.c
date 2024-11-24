#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int done;

static void handler(int signum)
{
	done = 1;
}

int main(void)
{
	char buf[BUFSIZ];
	mqd_t mqs[2];
	pid_t pid;
	unsigned long long count;

	for (int i = 0; i < 2; ++i) {
		sprintf(buf, "%d", i);
		mqs[i] = mq_open(buf, O_RDWR, O_CREAT);
	}
	if ((pid = fork()) {
		struct sigaction sa;

		sa.sa_handler = handler;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGTERM, &sa, NULL);
		mq_send(mqs[1], &pid, sizeof(pid), 0);
	}
	count = 0;
	while (!done) {
		mq_receive(mqs[pid == 0], buf, sizeof(buf), NULL);
		++count;
		mq_send(mqs[pid != 0], buf, sizeof(buf), 0);
	}
	if (pid) {
		join(pid);
		printf("%llu\n", count);
	}
	return 0;
}
