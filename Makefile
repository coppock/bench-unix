all: socketpair cond

socketpair: socketpair.o bench.o
	$(CC) $(CFLAGS) $(LDFLAGS) -osocketpair socketpair.o bench.o $(LDLIBS)

cond: cond.o bench.o
	$(CC) $(CFLAGS) $(LDFLAGS) -ocond cond.o bench.o $(LDLIBS)
