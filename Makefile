# $Id: Makefile,v 1.27 2010-09-11 18:15:05 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL = /bin/sh

INSTALLBASE = /usr/local

.PHONY: all
all: geese_pickc
#all: geese_pick
all: geese_plot

# Why not just use distutils all the way for installing this?  Because
# as far as I can see, it doesn't really support installing anything
# but modules.

.PHONY: install
install:
	python ./setup.py install --force
	install -m755 geese_pick geese_plot $(INSTALLBASE)/bin/
	install -m644 geese_pick.1 geese_plot.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test.cc libtest.a libgeese.a
	$(RM) *.o Makefile.bak core TAGS
	$(RM) *.pyc ChangeLog ChangeLog.bak MANIFEST
	$(RM) geese_*.1.ps
	$(RM) geese_pickc

.PHONY: check checkv
#check: pycheck
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

pycheck: coordinate.py find.py library.py segrid.py transform.py vector.py world.py
	for py in $^; \
	do PYTHONPATH=.. python $$py; \
	done

%.1.ps : %.1
	groff -man $< >$@

# C++ transform algorithms

CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -g -O3

geese_pickc: geese_pick.o libgeese.a
	$(CXX) -o $@ geese_pick.o -L. -lgeese

test.cc: libtest.a
	testicle -o$@ $^

tests: test.o libgeese.a libtest.a
	$(CXX) -o $@ test.o -L. -ltest -lgeese -lm

libgeese.a: library.o
libgeese.a: transform.o
libgeese.a: point.o
	$(AR) -r $@ $^

libtest.a: test_point.o
libtest.a: test_transform.o
	$(AR) -r $@ $^

.PHONY: tags
tags: TAGS
TAGS:
	etags *.cc *.h

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc

love:
	@echo "not war?"

# DO NOT DELETE

geese_pick.o: library.h transform.h point.h
library.o: library.h transform.h point.h regex.h
point.o: point.h
test_point.o: point.h
test_transform.o: transform.h point.h
transform.o: transform.h point.h
