#include "SharedMemory.h"

#include <iostream>

#define SM_KEY 0x1573

int main() {
    SharedMemory* delShm = new SharedMemory(SM_KEY);
    delShm->createAndconnect();

    delShm->destroy();

    delete delShm;

    return 0;
}