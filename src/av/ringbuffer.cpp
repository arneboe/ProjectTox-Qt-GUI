#include "ringbuffer.hpp"

Ringbuffer::Ringbuffer(size_t size) : capacity(size + 1), tail(0), head(0)
{
    buffer = (chunk_t *)malloc(capacity * sizeof(chunk_t));
}

Ringbuffer::~Ringbuffer()
{
    delete[] buffer;
}

bool Ringbuffer::push(const chunk_t &item)
{
    const size_t currentTail = tail.load(std::memory_order_relaxed);
    const size_t nextTail    = increment(currentTail);

    if(nextTail != head.load(std::memory_order_acquire)) {
        buffer[currentTail] = item;
        tail.store(nextTail, std::memory_order_release);
        return true;
    } else {
        return false; // buffer is full
    }
}

bool Ringbuffer::pop(chunk_t &item)
{
    const size_t currentHead = head.load(std::memory_order_relaxed);

    if(currentHead == tail.load(std::memory_order_acquire)) {
        return false; // buffer is empty
    } else {
        item = buffer[currentHead];
        head.store(increment(currentHead), std::memory_order_release);
        return true;
    }
}

size_t Ringbuffer::increment(const size_t idx) const
{
    return (idx + 1) % capacity;
}
