#ifndef ACP_POOL_HPP
#define ACP_POOL_HPP

#include <cstddef>
#include <initializer_list>
#include <new>
#include <vector>

class PoolAllocator {
public:
    PoolAllocator(std::size_t block_size, std::initializer_list<std::size_t> sizes);

    void *allocate(std::size_t n);

    void deallocate(void const *ptr);

private:
    struct SLAB {
        SLAB(size_t size_, size_t elem_size, std::byte &ptr_);
        size_t elem_size;
        std::byte *ptr;
        std::vector<bool> used;
    };

    static bool compare(const SLAB &a, const SLAB &b);
    size_t size_of_block;
    std::vector<std::byte> storage;
    std::vector<SLAB> slabs;
};

#endif  // ACP_POOL_HPP
