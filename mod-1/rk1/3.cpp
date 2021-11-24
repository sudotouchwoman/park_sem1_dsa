#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>

struct box {
    size_t id = 0;
    size_t x = 0;
    size_t y = 0;
    size_t z = 0;
};

size_t min(const box & bx) {
    if (bx.x <= bx.y and bx.x <= bx.z) return bx.x;
    if (bx.y <= bx.x and bx.y <= bx.z) return bx.y;
    if (bx.z <= bx.y and bx.z <= bx.x) return bx.z;
    return 0;
}

size_t max(const box & bx) {
    if (bx.x >= bx.y and bx.x >= bx.z) return bx.x;
    if (bx.y >= bx.x and bx.y >= bx.z) return bx.y;
    if (bx.z >= bx.y and bx.z >= bx.x) return bx.z;
    return 0;
}

size_t med(const box & bx) {
    const size_t mindim = min(bx);
    const size_t maxdim = max(bx);
    const size_t sumdim = bx.x + bx.y + bx.z;
    return sumdim - maxdim - mindim;
}

bool cmp_boxes(const box &lhs, const box &rhs) {
    return (max(lhs) > max(rhs) and med(lhs) > med(rhs) and min(lhs) > min(rhs));
}

void insertion_sort(box *const boxes, const size_t length, bool (*cmp)(const box & lhs, const box & rhs)) {
    for (size_t i = 1; i < length; ++i) {
        const box tmp = boxes[i];
        size_t j = i;
        while ( j > 0 and not cmp(tmp, boxes[j-1])) {
            boxes[j] = boxes[j-1];
            --j;
        }
        boxes[j] = tmp;
    }
}

void logic(std::istream &in, std::ostream &out) {
    size_t items = 0;
    in >> items;

    std::vector<box> boxes;
    for (size_t i = 0; i < items; ++i){
        size_t x = 0, y = 0, z = 0;
        in >> x >> y >> z;
        box bx = {.id = i, .x = x, .y = y, .z = z};
        boxes.push_back(bx);
    }

    insertion_sort(&boxes[0], items, cmp_boxes);

    for (size_t i = 0; i < items; ++i) {
        out << boxes[i].id;
    }


    // out << binary_search<size_t>(&arr[0], target, items);
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
