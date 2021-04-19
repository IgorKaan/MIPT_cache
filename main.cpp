#include <iostream>
#include "cache.hpp"

int main() {

    cache_t cache(6);
    unsigned hit_LRU = 0;
    unsigned hit_Optimal = 0;

    int id[SIZE_ARRAY] = {1, 2, 3, 1, 5, 1, 2, 7, 9, 5, 4, 3, 2, 6, 5, 4, 8, 7, 5, 1, 2, 6, 8, 4, 9, 7, 5, 3, 2, 1};

    std::array<Page<int>, SIZE_ARRAY> page_mas;

    for (int i = 0; i < page_mas.size(); ++i)
        page_mas[i].id_ = id[i];

    for (auto &page_ma : page_mas) {
        if (cache.lookup_LRU(&page_ma)) {
            hit_LRU++;
        }
    }

    cache.clear();

    for (int i = 0; i < page_mas.size(); ++i) {
        if (cache.lookup_Optimal(&page_mas[i], page_mas.begin() + i + 1, page_mas.end(), i)) {
            hit_Optimal++;
        }
    }

    std::cout << "hit LRU = " << hit_LRU << std::endl;
    std::cout << "hit Optimal = " << hit_Optimal << std::endl;
}
