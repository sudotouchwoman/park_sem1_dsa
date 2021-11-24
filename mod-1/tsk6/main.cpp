#include <iostream>
#include <cassert>
#include <sstream>
#include <functional>
#include <random>

using engine = std::mt19937;

template <
    typename T,
    typename comparator = std::function<bool(const T&, const T&)>>
size_t partition(T * const array, const size_t left, const size_t right, const comparator cmp) {
    if (right - left < 2) return left;  // the array slice provided is too small for the algorithm

    const size_t pivot_idx = [left, right]() {
        // use random so that the index of
        // current pivot is a random integer
        // distributed within [left, riight - 2]
        std::random_device os_seed;
        const size_t seed = os_seed();
        engine generator(seed);
        std::uniform_int_distribution<size_t> distribution(left, right - 2);
        return distribution(generator);
    }();

    const T pivot = array[pivot_idx];
    std::swap(array[pivot_idx], array[right - 1]);
    size_t i = left;

    for (size_t j = left; j < right - 1; ++j) {
        // it should be highlighted that the comparator
        // should evaluate to true if and only if
        // the left operand is strictly smaller then the right
        if (cmp(pivot, array[j])) continue;  // array[j] > pivot
        std::swap(array[i++], array[j]);
    }

    std::swap(array[right - 1], array[i]);
    return i;
}

template <
    typename T,
    typename comparator = std::function<bool(const T&, const T&)>>
void k_th_order_statistic(T * const array, const size_t length, const size_t k, const comparator cmp) {
    size_t left = 0, right = length;

    while (true) {
        const size_t med = partition<T>(array, left, right, cmp);
        if (med == k) return;
        if (med < k) {
            left = med + 1;
            continue;
        }
        right = med;
    }
}

void find_set_statistics(std::istream & in, std::ostream & out) {
    const size_t size = [&in]() {
        size_t size = 0;
        in >> size;
        return size;
    }();

    std::vector<int> vec;
    for (size_t i = 0; i < size; ++i) {
        size_t value = 0;
        in >> value;
        vec.push_back(value);
    }

    const size_t perc_10 = size * 10 / 100;
    const size_t perc_90 = size * 90 / 100;
    const size_t median = size / 2;

    auto is_less = [](const int &a, const int &b){
        return a < b;
    };

    k_th_order_statistic<int>(&vec[0], vec.size(), perc_10, is_less);
    out << vec[perc_10] << "\n";

    k_th_order_statistic<int>(&vec[0], vec.size(), median, is_less);
    out << vec[median] << "\n";

    k_th_order_statistic<int>(&vec[0], vec.size(), perc_90, is_less);
    out << vec[perc_90] << "\n";
}

void test_kth_statistic() {
    std::stringstream in;
    std::stringstream out;

    in << "10 1 2 3 4 5 6 7 8 9 10";
    find_set_statistics(in, out);
    assert(out.str() == "2\n6\n10\n");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "10 2 1 5 4 8 3 9 7 10 6";
    find_set_statistics(in, out);
    assert(out.str() == "2\n6\n10\n");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    std::cerr << "OK\n";
}

int main(int argc, char *argv[]) {
    // test_kth_statistic();
    find_set_statistics(std::cin, std::cout);
    return EXIT_SUCCESS;
}
