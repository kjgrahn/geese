# $Id: Makefile,v 1.50 2011-06-14 21:32:38 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
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

.PHONY: install
install:
	install -m755 geese_{pick,ref,world} $(INSTALLBASE)/bin/
	install -m755 geese_{fit,plot,css} $(INSTALLBASE)/bin/
	install -m644 geese_{pick,ref,world}.1 $(INSTALLBASE)/man/man1/
	install -m644 geese_{fit,plot,css}.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test.cc libtest.a libgeese.a
	$(RM) *.o Makefile.bak core TAGS
	$(RM) geese_*.1.ps
	$(RM) geese_{pick,ref,world,fit,css}
	$(RM) -r dep/

.PHONY: check checkv
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

%.1.ps : %.1
	groff -man $< >$@

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -O3
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g -O3

geese_pick: geese_pick.o libgeese.a
	$(CXX) -o $@ geese_pick.o -L. -lgeese -lanydim

geese_ref: geese_ref.o libgeese.a
	$(CXX) -o $@ geese_ref.o -L. -lgeese -lanydim

geese_world: geese_world.o libgeese.a
	$(CXX) -o $@ geese_world.o -L. -lgeese -lanydim

geese_fit: geese_fit.o libgeese.a
	$(CXX) -o $@ geese_fit.o -L. -lgeese -lanydim

geese_css: geese_css.o libgeese.a
	$(CXX) -o $@ geese_css.o -L. -lgeese -lanydim

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libgeese.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lgeese -lm

libgeese.a: md5.o
libgeese.a: md5pp.o
libgeese.a: sumdim.o
libgeese.a: split.o
libgeese.a: globbing.o
libgeese.a: child.o
libgeese.a: xvpixel.o
libgeese.a: worldfile.o
libgeese.a: library.o
libgeese.a: transform.o
libgeese.a: point.o
	$(AR) -r $@ $^

libtest.a: test_point.o
libtest.a: test_transform.o
libtest.a: test_md5.o
libtest.a: test_split.o
	$(AR) -r $@ $^

.PHONY: tags TAGS
tags: TAGS
TAGS:
	etags *.cc *.h

love:
	@echo "not war?"

# DO NOT DELETE

$(shell mkdir -p dep)
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
-include dep/*.d
