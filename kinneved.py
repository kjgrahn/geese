#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

import Image
import ImageDraw
import coordinate

r""" http://water.nchu.edu.tw/main/LER_EBOOK/SCANMAP/WORKING_WITH_THE_SCANNED_M.HTM
documents the .tfw files "world files" as

1.05
     The dimension of a pixel in map units in the x direction (the x
     scale).

     To determine this scaling factor, first find a distinct object,
     e.g. house or road or some linear feature on both the GIS basemap
     (shapefile) and the scanned image.  On the GIS map, use ArcView
     to measure the length of the object.  Then examine the same
     feature in the scanned image (you can also view it with ArcView).
     Count the number of pixels for that object.  The length of the
     object measured from the GIS basemap divided by the number of
     pixels will give you the scaling factor.

     If the pixels are not equal-sided squares, then you should find a
     linear feature running in the east-west direction for the x scale
     factor.
0.00000
     Rotation term for column.
0.00000
     Rotation term for column.
-1.05
    The dimension of a pixel in map units in the y map units.

    See discussion on x scale factor above.  Use a linear feature
    running in the north-south direction if pixels are not equal-sided
    squares.
-4300.21
    The x coordinate of the center of the upper-left pixel in map units.
-6157.00
    The y coordinate of the center of the upper-left pixel in map units.
"""

places = (('Rogestorp',                   64460, 13605),
          ('dammen Kinnarp',              64400, 13650),
          ('Hudenedammen',                64410, 13660),
          ('Backgården, N Slutarp',       64430, 13650),
          ('Tovarpasjön',                 64450, 13620),
          ('Horsödammen',                 64440, 13650),
          ('damm S Mönarps gård',         64430, 13620),
          ('göl Svarte mosse, Grolanda',  64400, 13590),
          ('Simonstorpadammen, Grolanda', 64410, 13570),
          ('Kaffedammen, Göteve',         64450, 13590),
          ('Karbosjön',                   64360, 13630))

"""
5.000000
0
0
-5.000000
1356800.500000
6447181.500000
"""

img = Image.open('kinneved.jpeg')
dimg = ImageDraw.Draw(img)

w, h = img.size
toPixel = coordinate.Transform((13568.00, 64471.81 - 0.05*h), (0, 0),
                               (13568.00 + 0.05*w, 64471.81), (w, h))

for name, y, x in places:
    mx, my = toPixel((x,y))
    my = h - my
    print name, mx, my
    dimg.ellipse((mx-10,my-10,mx+10,my+10), fill=(200,0,0))
for x in xrange(13570, 13670, 10):
    for y in xrange(64300, 64470, 10):
        mx, my = toPixel((x, y))
        my = h - my
        dimg.ellipse((mx-5,my-5,mx+5,my+5), fill=(0,0,0))

fromPixel = toPixel.inverse()

for name, x, y in (('Tovarpasjön', 1194, 350),
                   ('Örnkullen', 1294, 450),
                   ('Skånumabron',  990, 758),
                   ('Kärret Mönarp', 1343, 677),
                   ('Kjellbergs väg', 1506, 755),
                   ('Olle i Sörby', 1442,1273),
                   ('Svenstorp', 1389,1207),
                   ('Sånnum', 1319,1022),
                   ('kåken Naglarp', 1211,1119),
                   ('Svenstorpa lada', 1093, 964),
                   ('Alarpa korsväg', 1256,1345),
                   ('Fröje',  581,1655),
                   ('Mossebo',  468,1578),
                   ('Granet',  813,1597),
                   ('Karbosjön', 1386,2169),
                   ('Kinneveds k:a', 1618,1324),
                   ('Horsö', 1672, 455),
                   ('Rogestorp', 1580,  88),
                   ('kvarnen Slutarp', 1642, 864),
                   ('Väddholmen',  838, 989),
                   ('Hudenedammen', 1924,1049),
                   ('Tovarpaberget', 1042, 284),
                   ('Hunnarumpen',  217,1172),
                   ('Göteve k:a',  457,  44),
                   ('Vårkumla k:a', 2138,1229)):
    x, y = fromPixel((x, h-y))
    print '%20s: %d %d' % (name, y, x)
    mx, my = toPixel((x,y))
    my = h - my
    dimg.ellipse((mx-10,my-10,mx+10,my+10), fill=(100,0,0))

img.save('kinneved2.jpeg', 'JPEG', quality=30)
