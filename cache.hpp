#ifndef CACHE_CPP
#define CACHE_CPP

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <array>
#include <list>
#include <map>

template<typename KeyT = int>
struct Page {
    KeyT id_{};
};

template<typename KeyT = int>
bool operator == (Page<KeyT> c1, Page<KeyT> c2)
{
    return c1.id_ == c2.id_;
}
template<typename KeyT = int>
bool operator < (Page<KeyT> c1, Page<KeyT> c2)
{
    return c1.id_ < c2.id_;
}

template<typename KeyT = int>
struct Cache_page {
    Page<KeyT> page_{};
    int next_index_{};
};

template<typename KeyT = int>
bool operator < (Cache_page<KeyT> c1, Cache_page<KeyT> c2)
{
    return c1.next_index_ < c2.next_index_;
}

template<typename KeyT = int>
class cache_t {
private:

    unsigned sz_;
    using ListIt = typename std::list<Cache_page<KeyT>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_{};
    std::list<Cache_page<KeyT>> cache_ {};
    std::map<int, Page<KeyT>> cache_map_ {};

public:

    explicit cache_t(unsigned sz) : sz_(sz) {}

    void clear() {
        cache_.clear();
        hash_.clear();
    }

    bool cache_full() const {
        return (cache_.size() == sz_);
    }

    bool cache_map_full() const {
        return (cache_map_.size() >= sz_);
    }

    bool lookup_LRU(const Page<KeyT>* elem) {
        auto hit = hash_.find(elem->id_);
        if (hit == hash_.end()) {
            if (cache_full()) {
                hash_.erase(cache_.back().page_.id_);
                cache_.pop_back();
            }
            cache_.push_front({*elem, 0});
            hash_[elem->id_] = cache_.begin();
            return false;
        }

        auto eltit = hit->second;
        if (eltit != cache_.begin())
            cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
        return true;
    }

    using VecIt = typename std::vector<Page<KeyT>>::iterator;

    int predict_index_find(const Page<KeyT>* elem, VecIt current, VecIt end, int index) {
        for (auto it = current; it < end; ++it) {
            ++index;
            if (elem ->id_ == it->id_) {
                return index;
            }
        }
        return INT32_MAX;
    }

    bool lookup_LFU(Page<KeyT>* elem, VecIt current, VecIt end, int index) {
        Cache_page<KeyT> cp {std::move(*elem), 0};
        cp.next_index_ = predict_index_find(elem, current, end, index);
        auto hit = std::find_if(cache_map_.begin(), cache_map_.end(), [cp](const auto& iter){
            return (cp.page_.id_ == iter.second.id_);
        });
        if (hit == cache_map_.end()) {
            if (!cache_map_full()) {
                if(cp.next_index_ != INT32_MAX) {
                    cache_map_.insert({cp.next_index_, cp.page_});
                }
            }
            else if (cache_map_full()) {
                cache_map_.insert({cp.next_index_, cp.page_});
                cache_map_.erase( --(cache_map_.end()) );
            }
            return false;
        }
        else {
            cache_map_.erase(cache_map_.begin());
            if(cp.next_index_ != INT32_MAX)
                cache_map_.insert({cp.next_index_, cp.page_});
        }
        return true;
    }

    bool lookup_Optimal(Page<KeyT>* elem, VecIt current, VecIt end, int index) {
        Cache_page<KeyT> cp {std::move(*elem), 0};
        cp.next_index_ = predict_index_find(elem, current, end, index);
        auto hit = std::find_if(cache_map_.begin(), cache_map_.end(), [cp](const auto& iter){
            return (cp.page_.id_ == iter.second.id_);
        });
        if (hit == cache_map_.end()) {
            if (!cache_map_full()) {
                if(cp.next_index_ != INT32_MAX) {
                    cache_map_.insert({cp.next_index_, cp.page_});
                }
            }
            else if (cache_map_full()) {
                cache_map_.insert({cp.next_index_, cp.page_});
                cache_map_.erase( --(cache_map_.end()) );
            }
            return false;
        }
        else {
            cache_map_.erase(cache_map_.begin());
            if(cp.next_index_ != INT32_MAX)
                cache_map_.insert({cp.next_index_, cp.page_});
        }
        return true;
    }
};

#endif

