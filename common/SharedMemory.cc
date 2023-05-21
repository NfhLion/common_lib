#include "SharedMemory.h"

// ====================== SharedMemory::Buffer ======================= //
void SharedMemory::Buffer::init() {
    isCreate_ = true;
    pthread_mutexattr_init(&attr_);
    pthread_mutexattr_setpshared(&attr_, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex_, &attr_);
    memset(buffer_, 0, sizeof(buffer_));
    begin_ = 0;
    end_ = 0;
    cap_ = SM_BUF_SIZE;
}

void SharedMemory::Buffer::destroy() {
    pthread_mutexattr_destroy(&attr_);
    pthread_mutex_destroy(&mutex_);
}

void SharedMemory::Buffer::append(const char* msg, size_t len) {
    if (len > overage()) {
        return;
    }
    strncpy(buffer_ + begin_, msg, len);
    end_ += len;
}

void SharedMemory::Buffer::gets(char* buffer, size_t len) {
    if (len > size()) {
        return;
    }
    strncpy(buffer, buffer_ + begin_, len);
    begin_ += len;
    reset();
}

void SharedMemory::Buffer::reset() {
    if (begin_ == 0) {
        return ;
    }
    for (int i = 0; i < size(); ++i) {
        buffer_[i] = buffer_[begin_ + i];
    }
    end_ = size();
    begin_ = 0;
}

// ====================== SharedMemory ======================= //
void SharedMemory::createAndconnect() {
    shmId_ = shmget(key_t(shmKey_), sizeof(Buffer), 0644|IPC_CREAT);
    if (shmId_ < 0) {
        perror("Failed to shmget");
        exit(1);
    }
    smRet_ = memoryMap(shmId_);
    Buffer* smMsg = static_cast<Buffer*>(smRet_);
    if (smMsg->isCreate() == 0) {
        smMsg->init();
    }
}

void* SharedMemory::memoryMap(int shmId) {
    void* res = shmat(shmId, NULL, 0);
    if (res == nullptr) {
        perror("Failed to shmat");
        exit(1);
    }
    return res;
}

void SharedMemory::disconnect() {
    if (smRet_ != NULL) {
        int ret = shmdt(smRet_);
        if (ret < 0) {
            perror("Failed to shmdt");
            exit(1);
        }
    }
}

void SharedMemory::destroy() {
    Buffer* smMsg = static_cast<Buffer*>(smRet_);
    if (smMsg->isCreate() != 0) {
        smMsg->destroy();
    }
    int ret = shmctl(shmId_, IPC_RMID, NULL);
    if (ret < 0) {
        perror("Failed to shmctl");
        exit(1);
    }
}

size_t SharedMemory::write(const char* msg, size_t len) {
    if (len == 0) {
        return len;
    }
    Buffer* smMsg = static_cast<Buffer*>(smRet_);
    while (true) {
        smMsg->lock();
        if (!smMsg->full()) {
            if (len > smMsg->overage()) {
                len = smMsg->overage();
            }
            smMsg->append(msg, len);
            smMsg->unlock();
            printf("write smMsg len = %ld\n", len);
            break;
        } else {
            smMsg->unlock();
            printf("write smMsg is not have len = %ld\n", len);
            sleep(2);
        }
    }
    return len;
}

size_t SharedMemory::write(const std::string& msg) {
    return write(msg.c_str(), msg.length());
}

size_t SharedMemory::read(char* buffer, size_t len) {
    if (len == 0) {
        return len;
    }
    Buffer* smMsg = static_cast<Buffer*>(smRet_);
    while (true) {
        smMsg->lock();
        if (!smMsg->empty()) {
            if (len > smMsg->size()) {
                len = smMsg->size();
            }
            smMsg->gets(buffer, len);
            smMsg->unlock();
            printf("read smMsg len = %ld\n", len);
            break;
        } else {
            smMsg->unlock();
            printf("read smMsg is not have len = %ld\n", len);
            sleep(2);
        }
    }
    return len;
}

size_t SharedMemory::readAll(char* buffer) {
    Buffer* smMsg = static_cast<Buffer*>(smRet_);
    return read(buffer, smMsg->size());
}