#include "protocol.h"

Protocol::Protocol(Protocol *carrier) :
    carrier{carrier}
{
}

Protocol::~Protocol()
{
}

void Protocol::cancel()
{
    if (carrier) carrier->cancel();
}

