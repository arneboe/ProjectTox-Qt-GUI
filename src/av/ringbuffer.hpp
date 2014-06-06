#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <atomic>
#include <cstddef>
#include <stdlib.h>

class Ringbuffer
{
public:
    struct chunk_t
    {
        char* data;
        size_t size;
    };

    Ringbuffer(size_t size);
    virtual ~Ringbuffer();

    bool push(const chunk_t& item);
    bool pop(chunk_t& item);

private:
    chunk_t* buffer;
    const size_t capacity;
    std::atomic<size_t> tail;
    std::atomic<size_t> head;

    size_t increment(const size_t idx) const;
};

#endif // RINGBUFFER_HPP
