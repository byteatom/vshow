#include "udp_prot.h"

UdpProt::UdpProt(uint32_t ip, uint16_t  port) :
    AsioProt{nullptr},
    socket(*service)
{

}

UdpProt::~UdpProt()
{

}

void UdpProt::send(const CardMsgSp &msg, const IoSink &sink)
{

}

void UdpProt::recv(const IoSink &sink)
{
}

void UdpProt::cancel()
{
    //socket.cancel();
    AsioProt::cancel();
}
