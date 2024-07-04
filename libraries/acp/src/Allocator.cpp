#include "acp/Allocator.hpp"

AllocatorWithPool::AllocatorWithPool(const std::size_t size, std::initializer_list<std::size_t> sizes)
    : PoolAllocator(size, sizes){};

template <class T, class... Args>
T *create(Args &&...args);

template <class T>
void destroy(void *ptr);