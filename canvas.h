/* Copyright (c) 2021 Jörgen Grahn
 * All rights reserved.
 */
#ifndef GEESE_CANVAS_H
#define GEESE_CANVAS_H

#include "xv.h"

#include <string>

class Pixel;
struct gdImageStruct;
struct gdIOCtx;

/**
 * A canvas with a background image loaded from file.
 * You can put small circles on the canvas, and export it to a
 * temporary JPEG or PNG file, and that's about it.
 *
 * Implemented using libgd.
 */
class Canvas {
public:
    Canvas(const std::string& file, bool use_jpeg);
    Canvas(const Canvas&) = delete;
    Canvas& operator= (const Canvas&) = delete;
    ~Canvas();

    void bullseye(const std::string& color, unsigned width);
    void plot(const Pixel& px);
    void write(xv::Sink& sink) const;

private:
    gdImageStruct* const im;
    void (* const writep) (gdImageStruct* im, gdIOCtx* out);
    const int white;
    const int black;
    struct Bullseye {
	int color;
	struct {
	    unsigned inner;
	    unsigned outer;
	} width;
    } eye;
};

#endif
