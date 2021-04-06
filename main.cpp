//----------------------------------------------------------------------------
//
//  Example for LRU & Optimal cache in C++
//
//----------------------------------------------------------------------------
#include <iostream>
#include <map>
#include <iterator>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <set>

#define SIZE_ARRAY 30

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

    int sz_;
    using ListIt = typename std::list<Cache_page<KeyT>>::iterator;
    std::unordered_map<KeyT, ListIt> hash_{};

public:

    std::list<Cache_page<KeyT>> cache_ {};
    std::set<int> cache_set_ {};
    std::map<int, Page<KeyT>> cache_map_ {};

    explicit cache_t(int sz) : sz_(sz) {}

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

    using MasIt = typename std::array<Page<KeyT>, SIZE_ARRAY>::iterator;

    int predict_index_find(const Page<KeyT>* elem, MasIt current, MasIt end, int index) {
        for (auto it = current; it < end; ++it) {
            ++index;
            if (elem ->id_ == it->id_) {
                return index;
            }
        }
        return INT32_MAX;
    }

    bool lookup_Optimal(Page<KeyT>* elem, MasIt current, MasIt end, int index) {
        Cache_page<KeyT> cp {std::move(*elem), 0};
        cp.next_index_ = predict_index_find(elem, current, end, index);
        auto hit = std::find_if(cache_map_.begin(), cache_map_.end(), [cp](const auto& iter){
            return (cp.page_.id_ == iter.second.id_);
        });
        if (hit == cache_map_.end()) {
            if (!cache_map_full()) {
                if(cp.next_index_ != INT32_MAX) {
                    cache_map_.insert({cp.next_index_, cp.page_});
                    std::cout << "inserted " << cp.next_index_;
                }
            }
            else if (cache_map_full()) {
                cache_map_.insert({cp.next_index_, cp.page_});
                std::cout << "cache full" << std::endl;
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

int main() {
    cache_t cache(6);
    int id[SIZE_ARRAY] = {1, 2, 3, 1, 5, 1, 2, 7, 9, 5, 4, 3, 2, 6, 5, 4, 8, 7, 5, 1, 2, 6, 8, 4, 9, 7, 5, 3, 2, 1};
    std::array<Page<int>, SIZE_ARRAY> page_mas;
    for(int i = 0; i < page_mas.size(); ++i)
        page_mas[i].id_ = id[i];
    int hit_LRU = 0;
    for(auto & page_ma : page_mas) {
        if(cache.lookup_LRU(&page_ma)) {
            std::cout << "+ ";
            hit_LRU++;
        }
        else
            std::cout << "- ";
        std::cout << "{ ";
        for(const auto &x : cache.cache_) {
            std::cout << x.page_.id_ << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << "hit_LRU = " << hit_LRU << std::endl << std::endl << std::endl;

    cache.clear();

    int hit_Optimal = 0;

    for(int i = 0; i < page_mas.size(); ++i) {
        if(cache.lookup_Optimal(&page_mas[i], page_mas.begin() + i + 1, page_mas.end(), i)) {
            std::cout << "hit " << page_mas[i].id_ << std::endl;
            hit_Optimal++;
            std::cout << " + ";
        }
        else
            std::cout << " - ";
        std::cout << "{ ";
        for(const auto &x : cache.cache_map_) {
            std::cout << "(ne_i=" << x.first << ", " << "id=" << x.second.id_ << ") ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << "hit_Optimal = " << hit_Optimal << std::endl;
}
