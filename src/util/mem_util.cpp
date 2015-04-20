#include "mem_util.h"

ByteBuf::ByteBuf(size_t size, void *src) :
    size{size},
    buf{new uint8_t[size]}
{
    if (src) memcpy(buf, src, size);
}

ByteBuf::~ByteBuf()
{
    freenull(buf);
    size = 0;
}
