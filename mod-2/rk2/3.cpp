#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_set>

struct Node {
    std::string product_name;
    int color;
    int size;
    bool operator==(const Node & other) const {
        const bool same_color = color == other.color;
        const bool same_size = size == other.size;
        const bool same_name = product_name == other.product_name;
        return same_color and same_size and same_name;
    }
    friend std::istream & operator>>(std::istream & in, Node & n) {
        in >> n.product_name >> n.color >> n.size;
        return in;
    }
};

namespace std
{
    template<>
    struct hash<Node> {
        size_t operator()(const Node &n) const {
            const size_t prime = 331;
            const size_t str_prime = 73;

            size_t h { hash<int>()(n.size)};
            h = h * prime + hash<int>()(n.color);

            for (const auto & c: n.product_name) {
                h += str_prime * hash<char>()(c);
            }
            
            return h;
        }
    };
}  // namespace std


void run_hashtable(std::istream & in, std::ostream & out) {

    std::unordered_set<Node> nodes_set;

    auto tmp_node = Node();

    bool result;
    char command = 0;
    std::string key;

    while (in >> command >> tmp_node) {
        switch (command) {
            case '+':
                result = not nodes_set.count(tmp_node);
                nodes_set.insert({ tmp_node});
                break;
            case '-':
                result = nodes_set.count(tmp_node);
                nodes_set.erase(tmp_node);
                break;
            case '?':
                result = nodes_set.count(tmp_node);
                break;
            default:
                break;
        }
        out << (result ? "OK\n" : "FAIL\n");
    }
}

int main(int keyc, char* keyv[]) {
    // test_hashtable();
    run_hashtable(std::cin, std::cout);
    return EXIT_SUCCESS;
}
