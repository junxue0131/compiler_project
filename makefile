#
# makefile for TINY
# Borland C Version
# K. Louden 2/3/98
#

CC = gcc -ansi

CFLAGS = 

OBJS = main.o util.o scan.o parse.o symtab.o analyze.o caculate.o

tiny: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c globals.h util.h scan.h parse.h analyze.h caculate.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c parse.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) $(CFLAGS) -c analyze.c

caculate.o: caculate.c globals.h caculate.h
	$(CC) $(CFLAGS) -c caculate.c

clean:
	rm -f tiny tm *.o

tm: tm.c
	$(CC) $(CFLAGS) -etm tm.c -o tm 

tiny: tiny

tm: tm

all: tiny tm

