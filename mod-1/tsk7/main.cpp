#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#define ASCII_RADIX 256
#define MAX_STRINGS 100000

class StringMSD {
    private:
        const size_t radix;
        const size_t cutoff = 30;  // lower threshold for radix sort
        std::string *auxilary_strings = nullptr;
    private:
        void insertion_sort(std::string strings[], const size_t low, const size_t high);
        void radix_sort(std::string strings[], const size_t low, const size_t high, const size_t d);
    public:
        StringMSD(const size_t _radix): radix(_radix) {}
        ~StringMSD() {}
        void sort(std::string [], const size_t);
};

void StringMSD::sort(std::string strings[], const size_t n_strings) {
    auxilary_strings = new std::string[n_strings]();
    radix_sort(strings, 0, n_strings, 0);
    delete [] auxilary_strings;
}

void StringMSD::insertion_sort(std::string strings[], const size_t low, const size_t high) {
    for (size_t i = low; i < high; ++i) {
        size_t j = i;
        const std::string tmp = strings[i];
        while ( j > 0 and tmp.compare(strings[j-1]) < 0 ) {
            strings[j] = std::move(strings[j-1]);
            --j;
        }
        strings[j] = std::move(tmp);
    }
}

void StringMSD::radix_sort(std::string strings[], const size_t low, const size_t high, const size_t d) {
    size_t *const bins = new size_t[radix]();

    for (register size_t i = low; i < high; ++i) {
        // populate bins (count frequency for each value)
        ++bins[ strings[i][d] % radix ];
    }

    for (register size_t i = 1; i < radix; ++i) {
        // map counts to indices
        bins[i] += bins[i - 1];
    }

    for (register size_t i = high; i > low; --i) {
        // copy from original array to corresponding index in
        // the auxilary array
        // note decrementing bin value each time
        // these indices drive me crazy, cache optimization sucks
        auxilary_strings[ --bins[ strings[i - 1][d] % radix ] ] = std::move(strings[i - 1]);
    }

    for (register size_t i = low; i < high; ++i) {
        // copy back (the indices are shifted, thus subtract low)
        strings[i] = std::move(auxilary_strings[i - low]);
    }

    for (register size_t i = 1; i < radix - 1; ++i) {
        // only call itself for element if there was
        // something in the bin otherwise skip
        // the terminating null ('\0') symbol
        // is basically 0 thus just skip such empty strings
        if (bins[i] == bins[i+1]) continue;

        // if the number of items is small, use insertion sort instead
        if (bins[i+1] - bins[i] < cutoff)
            insertion_sort(strings, low + bins[i], low + bins[i+1]);
        else
            radix_sort(strings, low + bins[i], low + bins[i+1], d + 1);
    }
    delete [] bins;
}

void run_string_msd_sort(std::istream & in, std::ostream & out, const size_t max_strings) {
    std::vector<std::string> strings;
    strings.reserve(max_strings);
    std::string s;

    // little hack to let user limit string input
    // when calling (e.g., to read from std::cin)
    while (in >> s) {
        strings.emplace_back(s);
    }

    StringMSD sorter(ASCII_RADIX);
    sorter.sort(&strings[0], strings.size());

    for (register size_t i = 0; i < strings.size(); ++i) {
        out << strings[i] << '\n';
    }
}

void test_string_msd_sort() {
    std::stringstream in;
    std::stringstream out;

    in << "ab a aaa aa";
    run_string_msd_sort(in, out, MAX_STRINGS);
    assert(out.str() == "a\naa\naaa\nab\n");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    std::cerr << "OK\n";
}

int main(int argc, char *argv[]) {
    // test_string_msd_sort();
    run_string_msd_sort(std::cin, std::cout, MAX_STRINGS);
    return EXIT_SUCCESS;
}
