#include "SharedMemory.h"

#include <iostream>
#include <string>

#define SM_KEY 0x1573

int main() {
    SharedMemory* writeSm = new SharedMemory(SM_KEY);

    writeSm->createAndconnect();
    std::string msg = "0123456789000000|01234567890000";
    size_t len = writeSm->write(msg.c_str(), msg.length());
    while (len < msg.length()) {
        len += writeSm->write(msg.c_str() + len, msg.length() - len);
    }

    writeSm->disconnect();
    delete writeSm;
}