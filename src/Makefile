CC = gcc
CFLAGS = -Wall -O2 -g
LIBS = -lev
OBJS = net_read_handle.o tosquitmo.o net.o config.o

tosquitmo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

tosquitmo.o: tosquitmo.c tosquitmo.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

config.o: config.c config.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

net.o: net.c net.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

net_read_handle.o: net_read_handle.c net_read_handle.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

.PHONY: clean

clean:
	rm $(OBJS) tosquitmo
