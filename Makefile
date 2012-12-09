CFLAGS=-c -O2 -std=c99 -Wall -DNDEBUG
LIBS=-lm

all: bin/ldcell

dev: CFLAGS=-c -std=c99 -Wall -g
dev: all

build:
	@mkdir -p build
	@mkdir -p bin

bin/ldcell: build build/ldcell.o build/numhelpers.o
	$(CC) -o $@ $(LIBS) build/ldcell.o build/numhelpers.o

build/numhelpers.o: build src/numhelpers.c src/numhelpers.h
	$(CC) $(CFLAGS) -o $@ src/numhelpers.c

build/ldcell.o: build src/ldcell.c src/numhelpers.h
	$(CC) $(CFLAGS) -o $@ src/ldcell.c

clean:
	rm -f build/* bin/*
