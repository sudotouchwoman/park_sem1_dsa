#include <iostream>
#include <sstream>
#include <cassert>

size_t mode_binsearch(const int* array, const size_t length){

    size_t left = 0;
    size_t right = [array, length](){
        size_t i = 1;
        // immediately invoked lambda 
        // to find the upper (right) bound of binary search
        // via exponential search
        // i.e., while current item in array
        // is greater than its left neighbour, double the index of current item
        // if index reaches length, perform search for the entire array
        for (; i < length && array[i-1] < array[i]; i <<= 1);
        return i < length ? i : length - 1;
    }();

    while (left < right)
    {
        // iterative binary search cycle
        // compare middle item with its right neighbour
        // (We should not forget that `/` operator
        // drops the remainder of the resulting fraction for integers in C & C++)
        // as for the rest, this is a vanilla binary search,
        // the only difference is that we do not search some given element
        // but instead look for the certain condition (can I call this derivative of the array?)
        // either move left border or the right one
        // in the end `left` will point to `m`

        // `left` must move to `mid + 1` because otherwise
        //  we could encounter infinite loop, e.g., in case A = {1, 2}
        // `left` would not move from 0 as 1 < 2
        const size_t mid = (left + right) / 2;
        if (array[mid] > array[mid + 1])
            right = mid;
        else
            left = mid + 1;
    }

    return left;
}

void find_mode(std::istream &in, std::ostream& out){
    // io wrapper for the `mode_binsearch` function
    size_t len = 0;
    in >> len;
    int *A = new int[len];
    for (size_t i = 0; i < len; ++i) in >> A[i];
    out << mode_binsearch(A, len);
    delete []A;
}

void test_logic(){
    // some simple tests I wrote for this problem
    std::stringstream in;
    std::stringstream out;

    in << "3\n" << "1 4 2";
    find_mode(in, out);
    assert(out.str() == "1");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "2\n" << "-15 -8";
    find_mode(in, out);
    assert(out.str() == "1");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "2\n" << "-15 -88";
    find_mode(in, out);
    assert(out.str() == "0");
    std::stringstream().swap(in);
    std::stringstream().swap(out);
    
    in << "10\n" << "1 2 3 4 5 6 7 6 5 4";
    find_mode(in, out);
    assert(out.str() == "6");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "4\n" << "-10 -4 0 -3";
    find_mode(in, out);
    assert(out.str() == "2");
}

int main(int argc, char *argv[]){
    // test_logic();
    find_mode(std::cin, std::cout);
    return EXIT_SUCCESS;
}