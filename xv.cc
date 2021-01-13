/* Copyright (c) 2010, 2021 Jörgen Grahn
 * All rights reserved.
 */
#include "xv.h"

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <array>

namespace {

    int wait_for(pid_t pid)
    {
	int status;
	waitpid(pid, &status, 0);
	return status;
    }

    char* next(char*& p)
    {
	while(*p) p++;
	p++;
	return p;
    }

    constexpr char nil = '\0';

    /**
     * Constructing the argv for execvp(2).
     */
    template <size_t N>
    class Argv {
    public:
	explicit Argv(const std::string& s)
	    : buf {s}
	{
	    char* p = &buf.front();
	    char** q = val.data();
	    *q++ = p;
	    for (size_t i=1; i<N; i++) {
		*q++ = next(p);
	    }
	    *q++ = nullptr;
	}
	Argv(const Argv&) = delete;
	Argv& operator= (const Argv&) = delete;

	std::array<char*, N+1> val;
    private:
	std::string buf;
    };
}

using xv::Child;

Child::Child(const std::string& path)
{
    child = fork();
    if(child) return;

    std::ostringstream oss;
    oss << "xv" << nil << path;
    Argv<2> argv {oss.str()};
    execvp("xv", argv.val.data());
    std::cerr << "failed to exec 'xv': " << strerror(errno) << '\n';
    exit(1);
}

void Child::wait()
{
    if(child>0) {
	wait_for(child);
	child = 0;
    }
}

Child::~Child()
{
    if(child>0) {
	kill(child, SIGINT);
	wait();
    }
}

using xv::Sink;

Sink::Sink()
{
    int pipe[2];
    const int& rfd = pipe[0];
    const int& wfd = pipe[1];
    int err = pipe2(pipe, 0);
    if (err) return;

    child = fork();
    if (child) {
	fd = wfd;
	close(rfd);
	return;
    }

    close(wfd);
    dup2(rfd, 0); close(rfd);

    std::ostringstream oss;
    oss << "xv" << nil << '-';
    Argv<2> argv {oss.str()};
    execvp("xv", argv.val.data());
    std::cerr << "failed to exec 'xv': " << strerror(errno) << '\n';
    exit(1);
}

bool Sink::write(const void* buf, size_t len)
{
    ssize_t rc = ::write(fd, buf, len);
    if (rc==-1) return false;
    return static_cast<size_t>(rc) == len;
}

void Sink::eof()
{
    close(fd);
}

void Sink::wait()
{
    if(child>0) {
	wait_for(child);
	child = 0;
    }
}

Sink::~Sink()
{
    if(child>0) {
	kill(child, SIGINT);
	wait();
    }
}
