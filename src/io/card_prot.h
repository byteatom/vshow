#ifndef CARDPROT_H
#define CARDPROT_H

#include "asio_prot.h"

typedef std::function<void (size_t len, uint8_t *data)> DataSink;

class TaskSyncer;

class CardProt : public Protocol
{
public:
    explicit CardProt(CardType cardType, std::string &id);
    explicit CardProt(CardType cardType, Protocol *carrier);
    virtual ~CardProt();

    virtual void send(const CardMsgSp &msg, const IoSink &sink) override;
    virtual void recv(const IoSink &sink) override;
    virtual void cancel() override;

    void rpc(uint16_t addr, uint8_t type, uint16_t dataLen, uint8_t  *data,
             const DataSink &sink);
    void rpc(uint8_t type, const DataSink &sink);
    void rpc(uint8_t type, uint16_t dataLen, uint8_t  *data, const DataSink &sink);

    CardType cardType;
    uint8_t outerMsgType;
private:
    static const int RETRY_MAX = 5;
    TaskSyncer *syncer;
    bool freeCarrier;
    asio::deadline_timer timer;
    void session(const CardMsgSpListSp &reqs, const CardMsgSpListSp &rsps, const DataSink &sink);
};

#endif // CARDPROT_H
