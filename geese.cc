/* $Id: geese.cc,v 1.5 2003-01-05 23:19:02 grahn Exp $
 *
 * geese.cc
 *
 * Copyright (c) 2003 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */

#include <iostream>

#include "vector.h"
#include "transform.h"



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
