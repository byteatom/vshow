#include "asio_prot.h"

asio::io_service *AsioProt::service = nullptr;
asio::io_service::work *AsioProt::work = nullptr;
std::vector<std::thread *> AsioProt::threads;

void AsioProt::start()
{
    if (!service) {
        service = new asio::io_service;
        work = new asio::io_service::work(*service);
    }

    for (int i = 0; i < 1/*std::thread::hardware_concurrency()*/; ++i)	{
        threads.push_back(new std::thread([] {
            service->run();
        }));
    }
}

void AsioProt::stop()
{
    if (service) {
        delete work;
        work = nullptr;
        service->stop();
    }
    for (auto thread : threads)
    {
        thread->join();
        delete thread;
    }
    if (service) {
        delete service;
        service = nullptr;
    }
}

AsioProt::AsioProt(Protocol *carrier) :
    Protocol{carrier}
{
}

AsioProt::~AsioProt()
{

}
