#ifndef _IO_H_
#define _IO_H_

#include <signal.h>
#include <time.h>

extern volatile sig_atomic_t done;

void set(void);
void put(struct timespec *);

#endif
