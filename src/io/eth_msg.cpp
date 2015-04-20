#include "eth_msg.h"

#include <algorithm>
#include <cassert>

#include "card_msg.h"

const EthMsg::Addr EthMsg::DST_SEND{0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
const EthMsg::Addr EthMsg::DST_RECV{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

EthMsg::EthMsg(CardMsg *cardMsg, Addr &src) :
    MsgBuf{*cardMsg}
{
    assert(cardMsg->size <= DATA_LEN_MAX);
    resize(sizeof(Head), cardMsg->size < DATA_LEN_MIN ? DATA_LEN_MIN - cardMsg->size : 0);
    Head *head = (Head *)msg;
    head->dst = DST_SEND;
    head->src = src;
    head->type = TYPE_SEND;
}

EthMsg::EthMsg(size_t msgLen, void *msg) :
    MsgBuf{msgLen, msg}
{
}

EthMsg::~EthMsg()
{
}
