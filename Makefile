# $Id: Makefile,v 1.2 2003-01-04 16:09:29 grahn Exp $
#
# Makefile
#
# Copyright (c) 1999--2003 Jörgen Grahn <jgrahn@algonet.se>
# All rights reserved.

SHELL = /bin/sh

INSTALLBASE = /usr/local

CPPFLAGS=
CFLAGS=
CXXFLAGS= -W -Wall -pedantic -ansi -g
LDFLAGS=
LIBS=

OBJS=geese.o vector.o
OUTS=geese

all: $(OUTS)

geese: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

objs: $(OBJS)

install:
	false

TAGS: tags

tags:
	etags *.cc *.hh *.h

depend:
	makedepend -- $(CFLAGS) -- -Y *.cc

clean:
	rm -f $(OUTS) *.o Makefile.bak core TAGS

love:
	@echo "not war?"

# DO NOT DELETE

geese.o: vector.h
vector.o: vector.h
