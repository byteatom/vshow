#include "serial_prot.h"

#include "util/enumser.h"
#include "io.h"
#include "qlog/qlog.h"

void SerialProt::enumPorts(std::vector<SerialInfo> &infos)
{
    std::vector<unsigned int> nums;
    std::vector<std::wstring> names;

    if (!CEnumerateSerial::UsingSetupAPI1(nums, names))
        CEnumerateSerial::UsingSetupAPI2(nums, names);

    for (size_t i = 0; i < nums.size(); ++i) {
        if (names[i].find(L"Prolific") != std::wstring::npos)
            infos.push_back(SerialInfo{std::string("COM") + std::to_string(nums[i]), names[i]});
    }
}

SerialProt::SerialProt(const std::string &id):
    AsioProt{nullptr},
    id(id),
    port{*service},
    noiseBuf{noise, sizeof(noise)},
    opened{false},
    syncer{&Io::obj->syncer}
{
    system::error_code err;
    port.open(id, err);
    opened = (!err && port.is_open());
    if (!opened) return;

    port.set_option(asio::serial_port_base::baud_rate(115200));
    port.set_option(asio::serial_port_base::character_size(8));
    port.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
    port.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
    port.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
}

SerialProt::~SerialProt()
{
    if (!opened) return;

    system::error_code err;
    port.close(err);
}

void SerialProt::send(const CardMsgSp &msg, const IoSink &sink)
{
    if (!opened) {
        msg->iok = false;
        if (sink) sink(msg);
        return;
    }

    /*port.read_some(noiseBuf, [](system::error_code err, size_t len){

    });
    timer.expires_from_now(boost::posix_time::millisec(10));
    timer.async_wait([this](const system::error_code& err){
        port.cancel();
    }*/

    qlog<< "async_write >>, len:" << msg->size
        << ", type:" << showbase << hex << msg->innerType() << dec
        << ", dataLen:" << msg->innerDataLen() << qdebug;

    asio::async_write(port, asio::buffer(msg->msg, msg->size),
                [this, msg, sink](system::error_code err, size_t len) {

                    qlog<< "async_write <<, len:" << len
                        << ", type:" << showbase << hex << msg->innerType() << dec
                        << ", dataLen:" << msg->innerDataLen()
                        << ", err:" << err.value() << qdebug;
                    //::Sleep(100);
                    msg->iok = (!err && (len == msg->size));
                    if (sink && err != asio::error::operation_aborted)
                        (*syncer)(new Task([msg, sink](){sink(msg);}));
                });
}

void SerialProt::recv(const IoSink &sink)
{
    CardMsgSp msg = makeCardMsgSpRecv;

    if (!opened) {
        msg->iok = false;
        if (sink) sink(msg);
        return;
    }

    qlog << "async_read >>, len:" << sizeof(uint8_t) << qdebug;

    asio::async_read(port, asio::buffer(msg->msg, sizeof(uint8_t)),
        [this, msg, sink](system::error_code err, size_t len) {

            qlog << "async_read <<, len:" << len
                 << ", err:" << err.value() << qdebug;

            msg->iok = (!err && len == sizeof(uint8_t));
            if (!msg->iok) {
                if (sink && err != asio::error::operation_aborted)
                    (*syncer)(new Task([msg, sink](){sink(msg);}));
                return;
            }
            if (CardMsg::PREAMBLE != *(msg->msg)) {
                qlog << "async_read << noise" << qdebug;
                recv(sink);
                return;
            }

            qlog << "async_read >>, len:" << msg->headLen() - sizeof(uint8_t)<< qdebug;

            asio::async_read(port, asio::buffer(msg->msg + sizeof(uint8_t),
                                                msg->headLen() - sizeof(uint8_t)),
                [this, msg, sink](system::error_code err, size_t len) {

                    qlog<< "async_read <<, len:" << len
                        << ", type:" << showbase << hex << msg->type() << dec
                        << ", dataLen:" << msg->dataLen()
                        << ", err:" << err.value() << qdebug;

                    msg->iok = (!err && len == msg->headLen() - sizeof(uint8_t));
                    if (!msg->iok) {
                        if (sink && err != asio::error::operation_aborted)
                            (*syncer)(new Task([msg, sink](){sink(msg);}));
                        return;
                    }

                    qlog << "async_read >>, len:" << msg->dataLen() + msg->tailLen() << qdebug;

                    asio::async_read(port,
                        asio::buffer(msg->msg + msg->headLen(),
                                     msg->dataLen() + msg->tailLen()),
                        [this, msg, sink](system::error_code err, size_t len) {

                            qlog<< "async_read <<, len:" << len
                                << ", type:" << showbase << hex << msg->innerType() << dec
                                << ", dataLen:" << msg->innerDataLen()
                                << ", err:" << err.value() << qdebug;

                            msg->iok = (!err && len == msg->dataLen() + msg->tailLen());
                            if (sink && err != asio::error::operation_aborted)
                                (*syncer)(new Task([msg, sink](){sink(msg);}));
                    });
                });
        });
}

void SerialProt::cancel()
{
    if (!opened) return;

    //timer.cancel();
    port.cancel();
    /*port.close();
     * port.open();
     */
    AsioProt::cancel();
}
