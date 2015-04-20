#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <memory>

#define freenull(p) \
do {\
    if (p) {\
        delete p;\
        p = nullptr;\
    }\
}while(0)

class ByteBuf {
public:
    explicit ByteBuf(const ByteBuf &) = delete;
    explicit ByteBuf(size_t size, void *src = nullptr);
    ByteBuf& operator=(const ByteBuf&) = delete;
    ~ByteBuf();

    size_t size;
    uint8_t *buf;
};

typedef std::shared_ptr<ByteBuf> ByteBufSp;
#define makeByteBufSp(...) std::make_shared<ByteBuf>(__VA_ARGS__)

#endif // MEMORY_H
