#include <iostream>
#include <cassert>
#include <sstream>
#include <functional>
#include <list>
#include <algorithm>

template<typename T, typename C = std::less<T>>
class BT {
    private:
        struct Node {
            bool is_leaf = false;
            std::list<T> keys;
            std::list<Node*> subtrees;
            Node(bool is_leaf): is_leaf(is_leaf) {}
        };

    bool isCompleteNode(const Node * const n) {
        return n->keys.size() == 2 * degree - 1;
    }

    void insertToIncomplete(Node * n, const T & key) {
        int index = n->keys.size() - 1;

        if (n->is_leaf) {
            auto fits = std::find_if(
                n->keys.begin(),
                n->keys.end(),
                [&key, &comp](const T & x) {
                    return comp(x, key);
                }
            );
            n->keys.insert(fits, key);
        } else {
            
        }
    }

    private:
        C comp;
        Node* root = nullptr;
        size_t degree;
    public:
        BT() = delete;
        BT(const size_t degree, C comp = C()):
            comp(comp), degree(degree) {}
        ~BT() { delete root; }

        void Add(const T& key) {
            if (root == nullptr) root = new Node(true);
        }


};



int main(int argc, char* argv[]) {
    return EXIT_SUCCESS;
}


