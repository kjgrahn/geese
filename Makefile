# $Id: Makefile,v 1.50 2011-06-14 21:32:38 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2004, 2010 J�rgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/bash
INSTALLBASE=/usr/local

.PHONY: all
all: geese_pick
all: geese_ref
all: geese_world
all: geese_fit
all: geese_plot
all: geese_css
all: tests

.PHONY: install
install:
	install -m755 geese_{pick,ref,world} $(INSTALLBASE)/bin/
	install -m755 geese_{fit,plot,css} $(INSTALLBASE)/bin/
	install -m644 geese_{pick,ref,world}.1 $(INSTALLBASE)/man/man1/
	install -m644 geese_{fit,plot,css}.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test/test.cc test/libtest.a libgeese.a
	$(RM) *.o test/*.o core TAGS
	$(RM) geese_*.1.ps
	$(RM) geese_{pick,ref,world,fit,plot,css}
	$(RM) -r dep/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

%.1.ps : %.1
	groff -man $< >$@

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++14 -g -O3
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g -O3
CPPFLAGS=
ARFLAGS=rTP

geese_pick: geese_pick.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim

geese_ref: geese_ref.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim

geese_world: geese_world.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim

geese_fit: geese_fit.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim

geese_plot: geese_plot.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim -lgd

geese_css: geese_css.o libgeese.a
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -lgeese -lanydim

test/test.cc: test/libtest.a
	orchis -o$@ $^

tests: test/test.o libgeese.a test/libtest.a
	$(CXX) -o $@ $< -L. -Ltest/ -ltest -lgeese -lm

libgeese.a: md5.o
libgeese.a: md5pp.o
libgeese.a: sumdim.o
libgeese.a: hexread.o
libgeese.a: split.o
libgeese.a: globbing.o
libgeese.a: files...o
libgeese.a: xv.o
libgeese.a: xvpixel.o
libgeese.a: canvas.o
libgeese.a: worldfile.o
libgeese.a: library.o
libgeese.a: parse.o
libgeese.a: transform.o
libgeese.a: point.o
	$(AR) $(ARFLAGS) $@ $^

library.o: CXXFLAGS+=-Wno-misleading-indentation

test/libtest.a: test/point.o
test/libtest.a: test/transform.o
test/libtest.a: test/library.o
test/libtest.a: test/parse.o
test/libtest.a: test/md5.o
test/libtest.a: test/hexread.o
test/libtest.a: test/split.o
test/libtest.a: test/files.o
	$(AR) $(ARFLAGS) $@ $^

test/%.o: CPPFLAGS+=-I.

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.cc *.h

love:
	@echo "not war?"

# DO NOT DELETE

$(shell mkdir -p dep{,/test})
DEPFLAGS=-MT $@ -MMD -MP -MF dep/$*.Td
COMPILE.cc=$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o: %.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	@mv dep/$*.{Td,d}

dep/%.d: ;
dep/test/%.d: ;
-include dep/*.d
-include dep/test/*.d
