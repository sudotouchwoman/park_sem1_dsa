#include <iostream>
#include <sstream>
#include <cassert>

size_t invert_kth_bit(const size_t N, const short K){
    return ( N & 1 << K ? ~( ~N | 1 << K ) : N | 1 << K );
}

void run_inversion(std::istream &in, std::ostream& out){
    size_t N = 0;
    short K = 0;
    in >> N >> K;
    out << invert_kth_bit(N, K);
    return;
}

void test_inversion(){
    std::stringstream in;
    std::stringstream out;

    in << "7 2";
    run_inversion(in, out);
    assert(out.str() == "3");
    std::stringstream().swap(in);
    std::stringstream().swap(out);
    
    in << "0 0";
    run_inversion(in, out);
    assert(out.str() == "1");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "5 0";
    run_inversion(in, out);
    assert(out.str() == "4");

    return;
}

int main(int argc, char *argv[]){
    // test_inversion();
    size_t N = 0;
    short K = 0;
    std::cin >> N >> K;
    std::cout << invert_kth_bit(N, K) << std::endl;
    return EXIT_SUCCESS;
}