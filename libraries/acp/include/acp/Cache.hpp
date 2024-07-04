#ifndef ACP_CACHE_HPP
#define ACP_CACHE_HPP

#include <cstddef>
#include <list>
#include <new>
#include <ostream>

template <class Key, class KeyProvider, class Allocator>
class Cache {
public:
    template <class... AllocArgs>
    Cache(const size_t cache_size, AllocArgs &&...alloc_args)
        : m_max_size(cache_size), m_alloc(std::forward<AllocArgs>(alloc_args)...) {}

    size_t size() const { return m_queue.size(); }

    bool empty() const { return m_queue.size() == 0; }

    template <class T>
    T &get(const Key &key);

    std::ostream &print(std::ostream &strm) const;

    friend std::ostream &operator<<(std::ostream &strm, const Cache &cache) { return cache.print(strm); }

private:
    const size_t m_max_size;
    Allocator m_alloc;
    struct Elements {
        explicit Elements(KeyProvider *elem_) : elem(elem_){};
        KeyProvider *elem;
        bool used = false;
    };
    std::list<Elements> m_queue;
};

template <class Key, class KeyProvider, class Allocator>
template <class T>
inline T &Cache<Key, KeyProvider, Allocator>::get(const Key &key) {
    auto first  = m_queue.begin();
    auto second = m_queue.end();
    auto iq     = [&key](const Elements &e) { return *e.elem == key; };
    bool flag   = false;
    //    for (auto ) {
    //        if (first ==) {
    //            flag = true;
    //            break;
    //        }
    //    }
    for (; first != second; ++first) {
        if (iq(*first)) {
            flag = true;
            break;
        }
    }
    if (flag) {
        first->used = true;
        return *static_cast<T *>(first->elem);
    }
    if (size() == m_max_size) {
        while (m_queue.back().used) {
            m_queue.splice(m_queue.begin(), m_queue, std::prev(m_queue.end()));
            m_queue.front().used = false;
        }
        m_alloc.template destroy<KeyProvider>(m_queue.back().elem);
        m_queue.pop_back();
    }
    T *element = m_alloc.template create<T>(key);
    m_queue.emplace_front(element);
    return *element;
};
template <class Key, class KeyProvider, class Allocator>
inline std::ostream &Cache<Key, KeyProvider, Allocator>::print(std::ostream &strm) const {
    return strm << "<empty>\n";
};

#endif  // ACP_CACHE_HPP
