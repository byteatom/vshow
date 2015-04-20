#include "card_msg.h"

#include <cassert>
#include <algorithm>

#include "qlog/qlog.h"
#include "eth_msg.h"

MsgCfg CardMsg::cfg[CARD_TYPE_MAX][SenderCardMsg::TYPE_MAX];

void CardMsg::setTypeWishLen(CardType role, int type, uint32_t len)
{
    cfg[role][type].wishLen = len;
}

uint32_t CardMsg::getTypeWishLen(CardType role, int type)
{
    return cfg[role][type].wishLen;
}

CardMsg::CardMsg(uint16_t addr, uint8_t type, uint32_t	offset,
                 size_t dataLen, void *data) :
    MsgBuf{sizeof(Head) + dataLen + sizeof(Tail)},
    outerMsgType{0},
    retryCnt{0}
{
    set(addr, type, offset, dataLen, data);
}

CardMsg::CardMsg(size_t dataLen) :
    MsgBuf{sizeof(Head) + dataLen + sizeof(Tail)},
    outerMsgType{0},
    retryCnt{0}
{
}

CardMsg::CardMsg(uint16_t addr, uint8_t type, uint32_t	offset,
                 MsgBuf *msgBuf) :
    MsgBuf{*msgBuf},
    outerMsgType{0},
    retryCnt{0}
{
    size_t dataLen = msgBuf->size;
    resize(sizeof(Head), sizeof(Tail));
    set(addr, type, offset, dataLen);
}

CardMsg::CardMsg(CardMsg *outerMsg) :
    MsgBuf{*outerMsg},
    outerMsgType{outerMsg->outerMsgType},
    retryCnt{0}
{
#pragma warning(push)
#pragma warning(disable : 4146)
    resize(-sizeof(Head), -sizeof(Tail));
#pragma warning(pop)
}

CardMsg::CardMsg(EthMsg *ethMsg) :
    MsgBuf{*ethMsg},
    outerMsgType{0},
    retryCnt{0}
{
#pragma warning(push)
#pragma warning(disable : 4146)
    resize(-ethMsg->headLen(), -ethMsg->tailLen());
#pragma warning(pop)
    size = sizeof(Head) + dataLen() + sizeof(Tail);
    iok = true;
}

CardMsg::~CardMsg()
{

}

void CardMsg::set(uint16_t addr, uint8_t type, uint32_t offset,
                            uint16_t dataLen, void *data)
{
    Head *head = (Head *)msg;
    head->preamble = PREAMBLE;
    head->addr = hton2(addr);
    head->type = type;
    head->offset = hton4(offset);
    head->dataLen = hton2(dataLen);

    Tail *tail = (Tail *)(head->data + dataLen);
    tail->postamble = POSTAMBLE;

    if (data)
        memcpy(head->data, data, dataLen);

    sum();
}


void CardMsg::sum()
{
    Head *head = (Head *)msg;
    Tail *tail = (Tail *)(head->data + ntoh2(head->dataLen));
    tail->sum = 0;
    for (uint8_t *iter = msg; iter < &(tail->sum); ++iter) {
        tail->sum += *iter;
    }
}

bool CardMsg::checkSum()
{
    Head *head = (Head *)msg;
    Tail *tail = (Tail *)(head->data + ntoh2(head->dataLen));
    uint8_t sum = 0;
    for (uint8_t *iter = msg; iter < &(tail->sum); ++iter) {
        sum += *iter;
    }
    //qlog << "checksum error " << (sum == tail->sum) << qdebug;
    return (sum == tail->sum);
}

uint8_t CardMsg::innerType()
{
    Head *head = (Head*)msg;
    if (head->type == 0x80 || head->type== 0x81)
        return (head + 1)->type;
    else
        return head->type;
}

uint16_t CardMsg::innerDataLen()
{
    Head *head = (Head*)msg;
    if (head->type == 0x80 || head->type== 0x81)
        return ntoh2((head + 1)->dataLen);
    else
        return ntoh2(head->dataLen);
}

CardMsgSpListSp splitMsg(CardType role, uint16_t addr, uint8_t type,
                uint16_t dataLen, uint8_t *data)
{
    CardMsgSpListSp msgs = makeCardMsgSpListSp;

    uint32_t wishLen = CardMsg::getTypeWishLen(role, type);
    if (wishLen > DATA_LEN_MAX) {
        for (uint32_t offset = 0;
            offset < wishLen;
            offset+=DATA_LEN_MAX) {
            CardMsgSp msg = makeCardMsgSp(addr, type, offset, dataLen, data);
            msgs->push_back(msg);
        }
    } else if (dataLen > DATA_LEN_MAX) {
        int32_t leftLen = dataLen;
        uint8_t *chunkData = data;
        do {
            uint16_t chunkLen = (uint16_t)std::min(leftLen, (int32_t)DATA_LEN_MAX);
            CardMsgSp msg = makeCardMsgSp(addr, type, chunkData - data, chunkLen, chunkData);
            msgs->push_back(msg);
            leftLen -= chunkLen;
            chunkData += chunkLen;
        } while (leftLen > 0);
    } else {
        CardMsgSp msg = makeCardMsgSp(addr, type, 0, dataLen, data);
        msgs->push_back(msg);
    }

    return msgs;
}

ByteBufSp joinMsgs(CardType role, const CardMsgSpListSp &msgs)
{
    CardMsgSp msgLast = msgs->back();
    uint32_t wishLen = CardMsg::getTypeWishLen(role, msgLast->type());

    ByteBufSp bytes;
    if (wishLen > DATA_LEN_MAX) {
        uint32_t count = 0;

        for (CardMsgSp msg: *msgs) {
            count += msg->dataLen();
        }

        bytes = makeByteBufSp(count);
        uint8_t *pos = bytes->buf;
        for (CardMsgSp msg: *msgs) {
            uint16_t len = msg->dataLen();
            memcpy(pos, msg->data(), len);
            pos += len;
        }
        assert(pos <= bytes->buf + bytes->size);
    } else {
        bytes = makeByteBufSp(msgLast->dataLen());
        memcpy(bytes->buf, msgLast->data(), msgLast->dataLen());
    }

    return bytes;
}
