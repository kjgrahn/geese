/*
 * $Id: child.cc,v 1.1 2010-09-12 16:00:12 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "child.h"

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <cstdlib>

#include <iostream>

Child::Child(char** argv)
{
    child = fork();
    if(child) return;

    execvp(argv[0], argv);
    std::cerr << "failed to exec '" << argv[0]
	      << "': " << strerror(errno) << '\n';
    exit(1);
}

Child::~Child()
{
    if(child>0) {
	kill(child, SIGINT);
	int status;
	wait(&status);
    }
}
