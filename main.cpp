#include <iostream>
#include <assert.h>
#include "cache.hpp"

int main(int argc, char** argv) {

    int cache_size = 0;
    std::cout << "Enter Cache size:" << std::endl;
    std::cin >> cache_size;
    std::cout << "Cache size: " << cache_size << std::endl;
    assert(cache_size > 0);

    int n_pages = 0;

    std::cout << "Enter number of pages:" << std::endl;
    std::cin >> n_pages;
    std::cout << "Number of pages: " << n_pages << std::endl;
    assert(n_pages > 0);

    cache_t cache(cache_size);
    std::vector<Page<int>> page_massive_vector;

    for (int i = 0; i < n_pages; ++i) {
        int id = 0;
        std::cout << "Enter page " << i + 1 << " id: "  << std::endl;
        std::cin >> id;
        std::cout << id << std::endl;
        page_massive_vector.push_back(Page<int> { id });
    }

    int hit_LRU = 0;
    int hit_Optimal = 0;

    for (auto &page_elem : page_massive_vector) {
        if (cache.lookup_LRU(&page_elem)) {
            hit_LRU++;
        }
    }

    cache.clear();

    for (unsigned i = 0; i < page_massive_vector.size(); ++i) {
        if (cache.lookup_Optimal(&page_massive_vector[i], page_massive_vector.begin() + i + 1, page_massive_vector.end(), i)) {
            hit_Optimal++;
        }
    }

    std::cout << "hit LRU = " << hit_LRU << std::endl;
    std::cout << "hit Optimal = " << hit_Optimal << std::endl;

}
