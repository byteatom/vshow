#include "msg_buf.h"

#include <cassert>

MsgBuf::MsgBuf(size_t size, void *msg) :
    size{size},
    buf{makeByteBufSp(BEFOR_HEAD_SPACE + size + AFTER_TAIL_SPACE)},
    msg{buf->buf + BEFOR_HEAD_SPACE},
    iok{false}
{
    if (msg) memcpy(this->msg, msg, size);
}

MsgBuf::~MsgBuf()
{
}

void MsgBuf::resize(std::ptrdiff_t head, std::ptrdiff_t tail)
{
    assert(msg - head >= buf->buf
           && msg - head < buf->buf + buf->size
           && size + head + tail > 0
           && size + head + tail <= buf->size);
    msg -= head;
    size += (head + tail);
}
