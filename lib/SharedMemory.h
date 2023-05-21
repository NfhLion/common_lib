#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>

#define SM_BUF_SIZE 8192

class SharedMemory {
private:
    class Buffer {
    public:
        Buffer() = default;
        ~Buffer() = default;

        void init();

        void destroy();

        bool isCreate() const { return isCreate_; }
        int begin() const { return begin_; }
        int end() const { return end_; }
        size_t capacity() const { return cap_; }
        size_t size() const { return end_ - begin_; }
        size_t overage() const { return cap_ - end_; }
        bool empty() const { return begin_ == end_; }
        bool full() const { return end_ == cap_; }

        void append(const char* msg, size_t len);

        void gets(char* buffer, size_t len);

        void reset();

        int lock() {
            return pthread_mutex_lock(&mutex_);
        }

        int unlock() {
            return pthread_mutex_unlock(&mutex_);
        }

    private:
        bool isCreate_;

        pthread_mutex_t mutex_;
        pthread_mutexattr_t attr_;

        char buffer_[SM_BUF_SIZE];
        int begin_;
        int end_;
        size_t cap_;
    };

public:
    SharedMemory(int shmKey) 
        : shmKey_(shmKey),
          smRet_(nullptr)
    {}
    ~SharedMemory() {}

    void createAndconnect();

    void disconnect();

    void destroy();

    size_t write(const char* msg, size_t len);

    size_t write(const std::string& msg);

    size_t read(char* buffer, size_t len);

    size_t readAll(char* buffer);

private:
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;

    void* memoryMap(int shmId);

private:
    int shmKey_;
    int shmId_;
    void* smRet_;
};

#endif // SHARED_MEMORY_H