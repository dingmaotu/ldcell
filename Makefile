CPPFLAGS = -DNDEBUG
CFLAGS = -c -O2 -std=c99 -Wall
LIBS = -lm

APP = bin/ldcell
BUILDDIR = build

.PHONY: all release debug clean

all: release

release: $(APP)

debug: CFLAGS = -c -g -std=c99 -Wall
debug: CPPFLAGS = -UNDEBUG
debug: $(APP)

$(BUILDDIR):
	@mkdir -p $@
	@mkdir -p bin

build/numhelpers.o build/ldcell.o: | $(BUILDDIR)

$(APP): build/ldcell.o build/numhelpers.o
	$(CC) -o $@ $(LIBS) $^

build/numhelpers.o: src/numhelpers.c src/numhelpers.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

build/ldcell.o: src/ldcell.c src/numhelpers.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(BUILDDIR)/* bin/*
