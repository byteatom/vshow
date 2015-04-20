#include "geo_attr.h"

GeoAttr::GeoAttr()
{

}

GeoAttr::~GeoAttr()
{

}

void GeoAttr::setConstraint(int horizontal, int vertical)
{
    horizontalMax = horizontal;
    verticalMax = vertical;
}
