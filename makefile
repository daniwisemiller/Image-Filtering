# Written by David Chelberg 
# Edited by Danielle Wisemiller
# Very simple makefile for the code in this directory
#
# last-modified: Tue Dec 8 2:08 2017

# -O4 optimizes at a reasonably high level

CC = g++ -O4

.c: $@.h
	$(CC)  $@.c $(LDLIBS) -o $@

.cc: $@.h
	$(CC)  $@.cc $(LDLIBS) -o $@

all: prog2

pnm.o: pnm.cc pnm.h
	$(CC)  pnm.cc -c $(INCS)

prog2.o: prog2.cc
	$(CC) prog2.cc -c $(INCS)

prog2: prog2.o pnm.o
	$(CC)  prog2.o pnm.o $(INCS) $(LDLIBS) -o prog2

clean:
	rm pnm.o prog2.o  prog2
