#ifndef MSGBUF_H
#define MSGBUF_H

#include <cstdint>

#include "util/mem_util.h"

class MsgBuf
{
public:
    static const size_t BEFOR_HEAD_SPACE = 64;
    static const size_t AFTER_TAIL_SPACE = 256;

    explicit MsgBuf(const MsgBuf&) = default;
    explicit MsgBuf(size_t size, void *msg = nullptr);
    MsgBuf& operator=(const MsgBuf&) = delete;
    virtual ~MsgBuf();

    void resize(std::ptrdiff_t head, std::ptrdiff_t tail);

    size_t size;
    ByteBufSp buf;
    uint8_t *msg;
    bool iok;
};

#endif // MSGBUF_H
