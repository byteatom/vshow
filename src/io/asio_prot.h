#ifndef ASIOPROT_H
#define ASIOPROT_H

#include <thread>

#ifndef Q_MOC_RUN
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
using namespace boost;
#endif

#include "protocol.h"

class AsioProt : public Protocol
{
public:
    static asio::io_service *service;

    explicit AsioProt(Protocol *carrier);
    virtual ~AsioProt();

    static void start();
    static void stop();

private:
    static asio::io_service::work *work;
    static std::vector<std::thread *> threads;

};

#endif // ASIOPROT_H
