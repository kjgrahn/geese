# $Id: Makefile,v 1.46 2011-02-05 16:27:51 grahn Exp $
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

.PHONY: install
install:
	python ./setup.py install --force
	install -m755 geese_{pick,ref,world,fit,plot} $(INSTALLBASE)/bin/
	install -m644 geese_{pick,ref,world,fit,plot}.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test.cc libtest.a libgeese.a
	$(RM) *.o Makefile.bak core TAGS
	$(RM) *.pyc ChangeLog ChangeLog.bak MANIFEST
	$(RM) geese_*.1.ps
	$(RM) geese_{pick,ref,world,fit}

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
	$(CXX) -o $@ geese_pick.o -L. -lgeese

geese_ref: geese_ref.o libgeese.a
	$(CXX) -o $@ geese_ref.o -L. -lgeese

geese_world: geese_world.o libgeese.a
	$(CXX) -o $@ geese_world.o -L. -lgeese

geese_fit: geese_fit.o libgeese.a
	$(CXX) -o $@ geese_fit.o -L. -lgeese

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libgeese.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lgeese -lm

libgeese.a: md5.o
libgeese.a: md5pp.o
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
geese_fit.o: library.h transform.h point.h
geese_pick.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_pick.o: md5pp.h md5.h
geese_ref.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_ref.o: md5pp.h md5.h
geese_world.o: library.h transform.h point.h worldfile.h md5pp.h md5.h
globbing.o: globbing.h
library.o: library.h transform.h point.h regex.h worldfile.h globbing.h
md5pp.o: md5pp.h md5.h
point.o: point.h
test_md5.o: md5.h md5pp.h
test_point.o: point.h
test_transform.o: transform.h point.h
transform.o: transform.h point.h
worldfile.o: worldfile.h transform.h point.h
xvpixel.o: xvpixel.h transform.h point.h
md5.o: md5.h
