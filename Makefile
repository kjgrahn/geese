# $Id: Makefile,v 1.37 2010-09-18 07:29:22 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL=/bin/sh

INSTALLBASE=/usr/local

.PHONY: all
all: geese_pick
all: geese_ref
all: geese_plot

# Why not just use distutils all the way for installing this?  Because
# as far as I can see, it doesn't really support installing anything
# but modules.

.PHONY: install
install:
	python ./setup.py install --force
	install -m755 geese_pick geese_ref geese_plot $(INSTALLBASE)/bin/
	install -m644 geese_pick.1 geese_ref.1 geese_plot.1 $(INSTALLBASE)/man/man1/

.PHONY: clean
clean:
	$(RM) tests test.cc libtest.a libgeese.a
	$(RM) *.o Makefile.bak core TAGS
	$(RM) *.pyc ChangeLog ChangeLog.bak MANIFEST
	$(RM) geese_*.1.ps
	$(RM) geese_pick geese_ref

.PHONY: check checkv
#check: pycheck
check: tests
	./tests
checkv: tests
	valgrind -q ./tests -v

pycheck: coordinate.py find.py library.py segrid.py transform.py vector.py
	for py in $^; \
	do PYTHONPATH=.. python $$py; \
	done

%.1.ps : %.1
	groff -man $< >$@

# C++ transform algorithms

CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -g -O3
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g -O3

geese_pick: geese_pick.o libgeese.a
	$(CXX) -o $@ geese_pick.o -L. -lgeese

geese_ref: geese_ref.o libgeese.a
	$(CXX) -o $@ geese_ref.o -L. -lgeese

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
geese_pick.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
geese_ref.o: library.h transform.h point.h child.h xvpixel.h worldfile.h
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
