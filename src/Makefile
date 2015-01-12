CC = gcc
CFLAGS = -fno-strict-aliasing -Wall -O2 -g
LIBS = -lev -lpthread
OBJS = database.o memory_pool.o signal_handle.o \
	   net_read_handle.o tosquitmo.o net.o config.o \
	   thread_pool.o

tosquitmo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

tosquitmo.o: tosquitmo.c tosquitmo.h
	$(CC) $(CFLAGS) -c $< -o $@

config.o: config.c config.h
	$(CC) $(CFLAGS) -c $< -o $@

net.o: net.c net.h
	$(CC) $(CFLAGS) -c $< -o $@

net_read_handle.o: net_read_handle.c net_read_handle.h
	$(CC) $(CFLAGS) -c $< -o $@

signal_handle.o: signal_handle.c signal_handle.h
	$(CC) $(CFLAGS) -c $< -o $@

memory_pool.o: memory_pool.c memory_pool.h
	$(CC) $(CFLAGS) -c $< -o $@

database.o: database.c database.h
	$(CC) $(CFLAGS) -c $< -o $@

thread_pool.o: thread_pool.c thread_pool.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJS) tosquitmo
