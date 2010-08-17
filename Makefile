# $Id: Makefile,v 1.13 2010-08-17 19:48:43 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2004, 2010 J�rgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.

SHELL = /bin/sh

INSTALLBASE = /usr/local

all:

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
	$(RM) $(OUTS) *.o Makefile.bak core TAGS
	$(RM) *.pyc ChangeLog ChangeLog.bak MANIFEST

.PHONY: check
check: coordinate.py find.py library.py segrid.py transform.py vector.py
	for py in $^; \
	do PYTHONPATH=.. python $$py; \
	done

# The old C++ transform algorithms

CPPFLAGS=
CXXFLAGS= -W -Wall -pedantic -ansi -g
LDFLAGS=
LIBS=

OBJS=geese.o vector.o transform.o
OUTS=geese

geese: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

objs: $(OBJS)

TAGS: tags

tags:
	etags *.cc *.hh *.h

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc

love:
	@echo "not war?"

# DO NOT DELETE

geese.o: vector.h transform.h
transform.o: transform.h vector.h
vector.o: vector.h
