#ifndef GEOATTR_H
#define GEOATTR_H

#include <functional>

#include "attr.h"

class GeoAttr
{
public:
    GeoAttr();
    ~GeoAttr();

    void setConstraint(int horizontal, int vertical);

    Attr<int> x;
    Attr<int> y;
    Attr<int> width;
    Attr<int> height;

    int horizontalMax;
    int verticalMax;
};

#endif // GEOATTR_H
