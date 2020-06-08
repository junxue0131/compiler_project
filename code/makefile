#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc 

CFLAGS = 

OBJS = main.o util.o scan.o parse.o caculate.o

tiny: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c globals.h util.h scan.h parse.h  caculate.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c parse.c

caculate.o: caculate.c caculate.h globals.h
	$(CC) $(CFLAGS) -c caculate.c

clean:
	rm -f tiny tm *.o

tiny: tiny


