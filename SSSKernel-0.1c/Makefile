CC=gcc
CFLAGS=-O3 -ansi

all: doubleKernel tripleKernel
tripleKernel: shared.o tripleKernel.o
doubleKernel: shared.o doubleKernel.o

clean:	
	rm -f *.o tripleKernel doubleKernel

