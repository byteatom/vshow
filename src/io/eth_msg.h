#ifndef ETHMSG_H
#define ETHMSG_H

#include <cstdint>

#include "msg_buf.h"

class CardMsg;

class EthMsg : public MsgBuf
{
public:
    enum {
        ADDR_SIZE = 6,
        TYPE_SEND = 0xA1AA,
        TYPE_RECV = 0xA2AA,
        DATA_LEN_MIN = 46,
        DATA_LEN_MAX = 1500,
        MSG_LEN_MIN = 60
    };
#pragma pack(push)
#pragma pack(1)
    struct Addr {
        uint8_t addr[ADDR_SIZE];
    };
    struct Head
    {
        Addr dst;
        Addr src;
        uint16_t type;
#pragma warning(push)
#pragma warning(disable : 4200)
        uint8_t data[0];
#pragma warning(pop)
    };
#pragma pack(pop)
    static const Addr DST_SEND;
    static const Addr DST_RECV;
    static size_t headLen(){return sizeof(Head);}
    static size_t tailLen(){return 0;}

    explicit EthMsg(const EthMsg &) = delete;
    explicit EthMsg(CardMsg *cardMsg, Addr &src); //for send
    explicit EthMsg(size_t msgLen, void *msg); //for recv
    EthMsg& operator=(const EthMsg&) = delete;
    virtual ~EthMsg();
};

typedef std::shared_ptr<EthMsg> EthMsgSp;
#define makeEthMsgSp(...) std::make_shared<EthMsg>(__VA_ARGS__)

#endif // ETHMSG_H
