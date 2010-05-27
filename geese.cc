/* $Id: geese.cc,v 1.7 2010-05-27 21:16:13 grahn Exp $
 *
 * geese.cc
 *
 * Copyright (c) 2003 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */

#include <iostream>
#include <string>

#include "vector.h"
#include "transform.h"


/**
 * 
 */
class Map
{
public:
    Map(const std::string& path,
	int width, int height,
	const Vector& a1, const Vector& a2,
	const Vector& b1, const Vector& b2)
	: _path(path),
	  _width(width), _height(height),
	  _pt(PlaneTransform(a1, a2, b1, b2)),
	  _mt(width) {}

    bool contains(const Vector& v) const;

private:
    std::string _path;
    int _width;
    int _height;
    PlaneTransform _pt;
    BitmapTransform _mt;
};


int main()
{
    using std::cout;
    using std::endl;

    const Vector a2(1, 3);
    const Vector b2(0, 1);

    PlaneTransform tt(Vector(0,0), a2,
		      Vector(1,0), b2);

    cout << angle(Vector(0,0), Vector(1,0)) << "; "
	 << angle(a2, b2) << endl;

    cout << Vector(0,0) << endl;
    cout << Vector(1,0) << endl;
    cout << a2 << endl;
    cout << b2 << endl;
    cout << endl;

    cout << tt(Vector(0,0)) << endl;
    cout << tt(Vector(1,0)) << endl;
    cout << tt(Vector(0,1)) << endl;
    cout << tt(Vector(1,1)) << endl;
    cout << tt(Vector(1.2,1.2)) << endl;

    return 0;
}
