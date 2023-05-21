#include "SharedMemory.h"

#include <iostream>

#define SM_KEY 0x1573

int main() {
    SharedMemory* readSm = new SharedMemory(SM_KEY);
    readSm->createAndconnect();

    char msg[17] = {'\0'};
    size_t len = readSm->read(msg, 16);
    std::cout << "read SharedMemory msg: " << msg << std::endl;

    readSm->disconnect();

    delete readSm;

    return 0;
}