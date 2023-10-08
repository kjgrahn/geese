/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef GEESE_VERSION_H
#define GEESE_VERSION_H

namespace version {

    inline const char* geese()   { return "geese"; }
    inline const char* version() { return "2.12"; }
    inline const char* copyright() {
	return "Copyright (c) 2004--2011, 2021, 2023 Jörgen Grahn";
    }
}

#endif
