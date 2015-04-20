#ifndef SERIALPROT_H
#define SERIALPROT_H

#include <vector>

#include "asio_prot.h"

class TaskSyncer;

class SerialProt : public AsioProt
{
public:
    struct SerialInfo {
        std::string id;
        std::wstring name;
    };
    static void enumPorts(std::vector<SerialInfo> &infos);

    explicit SerialProt(const std::string &id);
    virtual ~SerialProt();

    virtual void send(const CardMsgSp &msg, const IoSink &sink) override;
    virtual void recv(const IoSink &sink) override;
    virtual void cancel() override;

private:
    std::string id;
    asio::serial_port port;
    uint8_t noise[1024];
    asio::mutable_buffers_1 noiseBuf;
    bool opened;
    TaskSyncer *syncer;
};

#endif // SERIALPROT_H
