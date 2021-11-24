#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>

template <typename T>
size_t binary_search(T* arr, T& k, const size_t len) {
    size_t left = 0;
    size_t right = len;

    while (left < right)
    {
        const size_t mid = (right + left) / 2;
        if (arr[mid] == k) return mid;

        if (arr[mid] > k) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }
    return left;  // did not found the item
}

void logic(std::istream &in, std::ostream &out) {
    size_t items = 0;
    in >> items;

    std::vector<size_t> arr;
    for (size_t i = 0; i < items; ++i){
        size_t item = 0;
        in >> item;
        arr.push_back(item);
    }

    size_t target = 0;
    in >> target;

    out << binary_search<size_t>(&arr[0], target, items);
}

int main(int argc, char *argv[]) {
    // test_logic();
    if (argc == 3) {
        // i/o from file if ones are specified via command line argument
        std::ifstream ifs;
        std::ofstream ofs;
        ifs.open(argv[1]);
        ofs.open(argv[2]);

        logic(ifs, ofs);
        
        ifs.close();
        ofs.close();
        return EXIT_SUCCESS;
    }

    logic(std::cin, std::cout);
    
    return EXIT_SUCCESS;
}
