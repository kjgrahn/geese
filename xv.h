/* Copyright (c) 2010, 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef GEESE_XV_H
#define GEESE_XV_H

#include <string>

namespace xv {

    /**
     * An 'xv foo.jpg' child process, with files just inherited from
     * the parent.
     *
     * You can wait for it to die, or kill-and-wait when the object is
     * destroyed.
     */
    class Child {
    public:
	explicit Child(const std::string& path);
	Child(const Child&) = delete;
	Child& operator= (const Child&) = delete;

	void wait();
	~Child();

    private:
	int child;
    };

    /**
     * Like 'xv -', and then you feed xv(1) the image file on stdin.
     *
     * You can wait for it to die, or kill-and-wait when the object is
     * destroyed.  Or, I suppose, write and get SIGPIPE.
     */
    class Sink {
    public:
	Sink();
	Sink(const Sink&) = delete;
	Sink& operator= (const Sink&) = delete;

	bool write(const void* buf, size_t len);
	void eof();

	void wait();
	~Sink();

    private:
	int child = 0;
	int fd;
    };
}

#endif
