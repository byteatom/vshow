#ifndef IMAGEATTR_H
#define IMAGEATTR_H

#include <stdint.h>
#include "util/image_tool.h"

namespace ImageAttr {

typedef uint32_t Argb;
/*
Qt::color0:         white 0xFFFFFFFF
Qt::color1:         black 0xFF000000
*/
static const Argb BG_COLOR_NONE = 0x0;  //Qt::transparent
static const Argb BG_COLOR_NONE_TOP = 0x01000000;
static const Argb BG_COLOR_DEFAULT = 0xFF000000;

static const Argb OPAQUE_WHITE = 0xFFFFFFFF;
static const Argb OPAQUE_BLACK = 0xFF000000;
static const Argb OPAQUE_RED = 0xFFFF0000;
static const Argb OPAQUE_BLUE = 0xFF0000FF;
static const Argb OPAQUE_GREEN = 0xFF00FF00;

static const Argb OPAQUE_ORANGERED = 0xFFFF4500;
static const Argb OPAQUE_FIREBRICK = 0xFFB22222;
static const Argb OPAQUE_SEAGREEN = 0xFF2E8B57;
static const Argb OPAQUE_DARKGREEN = 0xFF006400;
static const Argb OPAQUE_ROYALBLUE = 0xFF4169E1;
static const Argb OPAQUE_STEELBLUE = 0xFF4682B4;

static const Argb OPAQUE_VIOLET = 0xFF8A2BE2;
static const Argb OPAQUE_INDIGO = 0xFF4B0082;
static const Argb OPAQUE_SKY_BLUE = 0xFF87CEEB;
static const Argb OPAQUE_LIGHT_SKY_BLUE = 0xFF87CEFA;
static const Argb OPAQUE_LIGHT_BLUE = 0xFF4B0082;
static const Argb OPAQUE_LIGHT_STEEL_BLUE = 0xFFB0C4DE;

}

#endif // IMAGEATTR_H
