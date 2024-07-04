#include "acp/Pool.hpp"

#include <algorithm>
#include <cassert>

bool PoolAllocator::compare(const SLAB &a, const SLAB &b) {
    return a.elem_size < b.elem_size;
}

PoolAllocator::PoolAllocator(std::size_t const block_size, std::initializer_list<std::size_t> sizes)
    : size_of_block(block_size), storage(sizes.size() * block_size) {
    auto t = sizes.begin();
    for (size_t i = 0; t != sizes.end(); i++) {
        slabs.emplace_back(block_size, *t, storage[i * block_size]);
        t++;
    }
    std::sort(slabs.begin(), slabs.end(), PoolAllocator::compare);
};

PoolAllocator::SLAB::SLAB(size_t size_, size_t elem_size, std::byte &ptr_)
    : elem_size(elem_size), ptr(&ptr_), used(size_ / elem_size, false){};

void *PoolAllocator::allocate(size_t n) {
    size_t d = std::lower_bound(slabs.begin(), slabs.end(), SLAB(static_cast<size_t>(0), n, storage[0]), compare) -
               slabs.begin();
    for (size_t i = 0; i < slabs[d].used.size(); ++i) {
        if (!slabs[d].used[i]) {
            slabs[d].used[i] = true;
            return (slabs[d].ptr + (slabs[d].elem_size * i));
        }
    }
    throw std::bad_alloc{};
}

void PoolAllocator::deallocate(void const *ptr) {
    auto b_ptr = static_cast<const std::byte *>(ptr);
    std::less_equal<const std::byte *> cmp;
    if (cmp(&storage[0], b_ptr) && cmp(b_ptr, &storage.back())) {
        size_t i = (b_ptr - &storage[0]) / size_of_block;
        assert((b_ptr - slabs[i].ptr) % slabs[i].elem_size == 0);
        slabs[i].used[(b_ptr - slabs[i].ptr) / slabs[i].elem_size] = false;
    }
}
