/* -*- c++ -*-
 *
 * $Id: child.h,v 1.1 2010-09-12 16:00:12 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_CHILD_H
#define GEESE_CHILD_H

struct Child {
    explicit Child(char** argv);
    ~Child();
    int child;
private:
    Child();
    Child(const Child&);
    Child& operator= (const Child&);
};

#endif
