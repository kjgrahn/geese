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
	python ./setup.py install --force
	install -m755 geese_{pick,ref,world} $(INSTALLBASE)/bin/
	install -m755 geese_{fit,plot,css} $(INSTALLBASE)/bin/
	install -m644 geese_{pick,ref,world}.1 $(INSTALLBASE)/man/man1/
	install -m644 geese_{fit,plot,css}.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test.cc libtest.a libgeese.a
	$(RM) *.o Makefile.bak core TAGS
	$(RM) *.pyc ChangeLog ChangeLog.bak MANIFEST
	$(RM) geese_*.1.ps
	$(RM) geese_{pick,ref,world,fit,css}

.PHONY: check checkv
check: pycheck
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

.PHONY: pycheck
pycheck: test.py
	PYTHONPATH=.. ./test.py

%.1.ps : %.1
	groff -man $< >$@

CXXFLAGS=-Wall -Wextra -pedantic -Wold-style-cast -std=c++98 -g -O3
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
libgeese.a: version.o
	$(AR) -r $@ $^

libtest.a: test_point.o
libtest.a: test_transform.o
libtest.a: test_md5.o
libtest.a: test_split.o
	$(AR) -r $@ $^

.PHONY: tags
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc *.c

love:
	@echo "not war?"

# DO NOT DELETE

child.o: child.h
geese_css.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_css.o: md5pp.h md5.h split.h
geese_fit.o: library.h transform.h point.h
geese_pick.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_pick.o: md5pp.h md5.h
geese_ref.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_ref.o: md5pp.h md5.h sumdim.h
geese_world.o: library.h transform.h point.h worldfile.h md5pp.h md5.h
geese_world.o: sumdim.h
globbing.o: globbing.h
library.o: library.h transform.h point.h regex.h worldfile.h globbing.h
md5pp.o: md5pp.h md5.h
point.o: point.h
split.o: split.h
sumdim.o: sumdim.h md5pp.h md5.h
test_md5.o: md5.h md5pp.h
test_point.o: point.h
test_split.o: split.h
test_transform.o: transform.h point.h
transform.o: transform.h point.h
worldfile.o: worldfile.h transform.h point.h
xvpixel.o: xvpixel.h transform.h point.h
md5.o: md5.h
