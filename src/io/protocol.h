#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <functional>

#include "card_msg.h"

typedef std::function<void (const CardMsgSp &msg)> IoSink;

class Protocol
{
public:
    explicit Protocol(Protocol *carrier);
    virtual ~Protocol();

    virtual void send(const CardMsgSp &msg, const IoSink &sink) = 0;
    virtual void recv(const IoSink &sink) = 0;
    virtual void cancel();

protected:
    Protocol *carrier;
};

#endif // PROTOCOL_H
