CC = gcc
CFLAGS = -Wall -O2 -g
LIBS = -lev
OBJS = tosquitmo.o net.o config.o

tosquitmo: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

tosquitmo.o: tosquitmo.c tosquitmo.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

config.o: config.c config.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

net.o: net.c net.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

.PHONY: clean

clean:
	rm $(OBJS) tosquitmo
