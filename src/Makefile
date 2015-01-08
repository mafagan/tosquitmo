CC = gcc
CFLAGS = -O2 -g
LIBS = -lev -lpthread
OBJS = database.o memory_pool.o signal_handle.o \
	   net_read_handle.o tosquitmo.o net.o config.o \
	   thread_pool.o

tosquitmo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)
	mv tosquitmo ../bin/

tosquitmo.o: tosquitmo.c tosquitmo.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

config.o: config.c config.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

net.o: net.c net.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

net_read_handle.o: net_read_handle.c net_read_handle.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

signal_handle.o: signal_handle.c signal_handle.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

memory_pool.o: memory_pool.c memory_pool.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

database.o: database.c database.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

thread_pool.o: thread_pool.c thread_pool.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

.PHONY: clean

clean:
	rm -rf $(OBJS) ../bin/*
