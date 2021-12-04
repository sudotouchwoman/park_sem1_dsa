#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <functional>

template <
    typename T,
    typename Comparator = std::function<bool(const T &, const T &)>>
void merge(T * const array, const size_t left, const size_t right, const size_t mid, const Comparator & is_less) {
    const size_t n_left = mid - left;
    const size_t n_right = right - mid;

    T *const L = new T[n_left]();
    T* const R = new T[n_right]();

    for (size_t i = 0; i < n_left; ++i)
        L[i] = std::move(array[i+left]);

    for (size_t i = 0; i < n_right; ++i)
        R[i] = std::move(array[i+mid]);

    size_t i_l = 0, i_r = 0, i_arr = left;
    while (i_l < n_left and i_r < n_right) {
        if (is_less(L[i_l], R[i_r]))
            array[i_arr++] = std::move(L[i_l++]);
        else
            array[i_arr++] = std::move(R[i_r++]);
    }
    
    while (i_l < n_left)
        array[i_arr++] = std::move(L[i_l++]);
    while (i_r < n_right)
        array[i_arr++] = std::move(R[i_r++]);

   delete [] L;
   delete [] R;
}

template <
    typename T,
    typename Comparator = std::function<bool(const T &, const T &)>>
void merge_sort(T* const array, const size_t left, const size_t right, const Comparator & is_less) {
    if (left >= right) return;
    const size_t mid = (right + left) / 2;
    merge_sort(array, left, mid, is_less);
    merge_sort(array, mid+1, right, is_less);
    merge(array, left, right, mid, is_less);
}

// easy come, easy go
// just some revision of operator overloading
struct hours {
    size_t come;
    size_t leave;
    hours(const size_t _come = 0, const size_t _leave = 0) : come(_come), leave(_leave) {}
    friend std::istream & operator>>(std::istream & in, hours & a) {
        size_t come = 0;
        size_t leave = 0;
        in >> come >> leave;
        a.come = come;
        a.leave = leave;
        return in;
    }
    friend std::ostream & operator<<(std::ostream & out, hours & a) {
        out << "Come: " << a.come << " Leave: " << a.leave << std::endl;
        return out;
    }
    bool operator==(const hours & other) {  // this one makes computing ads shorter
        if (come != other.come) return false;
        if (leave != other.leave) return false;
        return true;
    }
};

void run_merge_sort(std::istream & in, std::ostream & out) {
    const size_t n = [&in]() {
        size_t n = 0;
        in >> n;
        return n;
    }();

    size_t * const arr = new size_t [n]();

    for (size_t i = 0; i < n; ++i) {
        in >> arr[i];
    }

    auto is_less = [](const size_t & lhs, const size_t & rhs) {
        return lhs < rhs;
    };

    merge_sort<size_t>(arr, 0, n, is_less);

    for (size_t i = 0; i < n; ++i) {
        out << arr[i] << ' ';
    }

    out << '\n';
    delete [] arr;
}

void run_ad_computation(std::istream & in, std::ostream & out) {
    // get number of clients
    const size_t n_clients = [&in]() {
        size_t n_clients = 0;
        in >> n_clients;
        return n_clients;
    }();

    // just in case there will be no input
    if (n_clients == 0) {
        out << 0;
        return;
    }

    // create collection of clients
    hours * const clients = new hours[n_clients];
    
    // read the intervals data
    for (size_t i = 0; i < n_clients; ++i) {
        in >> clients[i];
    }

    // define comparator via lambda
    // the hours are sorted in forward-way by leave
    // and in backward way by come
    auto is_less = [](const hours & lhs, const hours & rhs) {
        if (lhs.leave < rhs.leave) return true;
        if (lhs.come > rhs.come) return false;
        return true;
    };

    merge_sort<hours>(clients, 0, n_clients, is_less);

    // write the sorted array to debug
    // for (size_t i = 0; i < n_clients; ++i) {
    //     std::cerr << clients[i];
    // }
    
    // write to out the number of ads
    out << [clients, n_clients]() {
        size_t n_ads = 2;  // there will be at least one client, i.e. 2 ads
        for (size_t i = 1; i < n_clients; ++i) {
            if (clients[i] == clients[i-1])
                //  [....]
                //  [....]
                // no new ads
                continue;
            if (clients[i].come == clients[i-1].leave) {
                // [....]
                // ``   [...]
                // +1 ad
                ++n_ads;
                continue;
            }

            if (clients[i].come > clients[i-1].leave)
                // [......]
                //          [...]
                // +2ads
                n_ads += 2;
            else
                // [......]
                //      [.]
                // +1 ad
                ++n_ads;
        }
        return n_ads;
    }();
    
    delete [] clients;
}

void test_string_msd_sort() {
    std::stringstream in;
    std::stringstream out;

    in << "5 1 10 10 12 1 10 1 10 23 24";
    run_ad_computation(in, out);
    assert(out.str() == "5");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "10 1 10 23 12 1 10 1 10 10 24";
    run_merge_sort(in, out);
    std::cerr << out.str();
    assert(out.str() == "1 1 1 10 10 10 10 12 23 24");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    std::cerr << "OK\n";
}

int main(int argc, char *argv[]) {
    test_string_msd_sort();
    // run_ad_computation(std::cin, std::cout);
    return EXIT_SUCCESS;
}
