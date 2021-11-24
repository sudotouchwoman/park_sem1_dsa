#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>

class MyShinyStack {
    char* inner_buffer = nullptr;
    const size_t size;
    size_t height = 0;
    public:
    const char GetTop();
    void Pop() { --height; }
    void PushTop(const char c);
    MyShinyStack(const size_t _size);
    ~MyShinyStack() { delete[] inner_buffer; }
};

MyShinyStack::MyShinyStack(const size_t _size) : size(_size){
    inner_buffer = new char[size]();
}

void MyShinyStack::PushTop(const char c) {
    inner_buffer[height++] = c;
}

const char MyShinyStack::GetTop() {
    return inner_buffer[height];
}

bool is_anagram(const std::string a, const std::string b) {
    
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
