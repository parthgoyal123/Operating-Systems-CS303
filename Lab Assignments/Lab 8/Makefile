# Lab8 Makefile
# Parth Goyal
# CS303 Operating Systems

CC=gcc
CFLAGS=-I. -Wall -pthread 
DEPS = buffer.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

producer_consumer: main.o  
	gcc -o producer_consumer -Wall -pthread  main.c -I.

clean:
	rm -f *.o