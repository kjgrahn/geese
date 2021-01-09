/* Copyright (c) 2021 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "canvas.h"

#include "transform.h"

#include <gd.h>
#include <cmath>

Canvas::Canvas(const std::string& file)
    : im {gdImageCreateFromFile(file.c_str())},
      white {gdImageColorAllocate(im, 255, 255, 255)},
      black {gdImageColorAllocate(im, 0, 0, 0)},
      eye {white, {10, 16}}
{}

Canvas::~Canvas()
{
    gdImageDestroy(im);
}

void Canvas::bullseye(const std::string& color, unsigned width)
{
    eye.color = white;
    if (color=="black") eye.color = black;
    eye.width.inner = width;
    eye.width.outer = width + 2;
}

void Canvas::plot(const Pixel& px)
{
    if (!im) return;

    int cx = std::lround(px.p.x);
    int cy = std::lround(px.p.y);

    auto circle = [&] (int width, int color) {
	gdImageFilledEllipse(im, cx, cy, width, width, color);
    };

    int bgcolor = (eye.color==white) ? black : white;
    circle(eye.width.outer, bgcolor);
    circle(eye.width.inner, eye.color);
}

const std::string Canvas::write() const
{
    const std::string name = "/tmp/foo.jpg";
    gdImageFile(im, name.c_str());
    return name;
}
