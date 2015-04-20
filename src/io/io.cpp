#include "io.h"

#include <cassert>

#include "asio_prot.h"

Io *Io::obj = nullptr;

Io::Io() {
    assert(!obj);
    Io::obj = this;
}

Io::~Io() {

}

void Io::start() {
    AsioProt::start();
}

void Io::stop() {
    syncer.enabled = false;
    AsioProt::stop();
}


