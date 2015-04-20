#ifndef UDPPROT_H
#define UDPPROT_H

#include "asio_prot.h"

class UdpProt : public AsioProt
{
public:
    explicit UdpProt(uint32_t ip, uint16_t  port);
    virtual ~UdpProt();

    virtual void send(const CardMsgSp &msg, const IoSink &sink) override;
    virtual void recv(const IoSink &sink) override;
    virtual void cancel() override;

private:
    asio::ip::udp::socket socket;
};

#endif // UDPPROT_H
