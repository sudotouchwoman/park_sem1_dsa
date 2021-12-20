#include <iostream>
#include <cassert>
#include <sstream>
#include <functional>

// enum for
// old-skool comparator
enum comp {
    less,
    even,
    more
};

// the possible balance
enum balance {
    heavy_left = -2,
    heavy_right = 2,
    balanced = 0
};

enum commands {
    insert_troop = 1,
    remove_troop = 2
};

template<typename T>
struct DefaultTripleComparator {
    int operator()(const T & lhs, const T & rhs) {
        if (lhs < rhs) return less;
        if (lhs == rhs) return even;
        return more;
    }
};

template<typename T, typename C = DefaultTripleComparator<T>>
class AVLBST {
    private:

    // Node helper structure
    // contains additional properties:
    // height and index of the given node
    // (the troops should always know their order)
    struct Node {
        T data;
        Node * left = nullptr;
        Node * right = nullptr;
        int height = 1;
        int index = 1;
        Node() = default;
        Node(const T & data): data(data) {}
        ~Node() = default;

        int balance_factor() const { return get_height(right) - get_height(left); }

        // helper func to erase the subtree
        // correctly, this said,
        // recursively remove its subtrees
        static void erase_subtrees(Node* p) {
            if (p == nullptr) return;
            erase_subtrees(p->left);
            erase_subtrees(p->right);
            delete p;
            }

        // get (safely) the index of a given node
        // safely means that this one can ba applied to nullptr's
        static int get_index(const Node * const p) {
            if (p == nullptr) return 0;
            return p->index;
        }

        // analogous to the index static method
        static int get_height(const Node * const p) {
            if (p == nullptr) return 0;
            return p->height;
        }
    };

    // remove parent of a subtree
    // this said, fing the minimal
    // value in the right subtree
    // and set it as the new parent

    Node * pop_parent(Node * p, Node * & to_swap) {
        if (p == nullptr) {
            to_swap = nullptr;
            return nullptr;
        }
        if (p->left == nullptr) {
            to_swap = p;
            return p->right;
        }
        p->left = pop_parent(p->left, to_swap);
        return balance(p);
    }

    // recursively try to remove the node
    // at the given index

    Node* remove_r(const int index, Node * n) {
        if (n == nullptr) return nullptr;

        const int left_index = Node::get_index(n->left);

        if (left_index < index)
            n->right = remove_r(index - left_index - 1, n->right);
        else if (left_index > index)
            n->left = remove_r(index, n->left);
        else {
            Node * left = n->left;
            Node * right = n->right;

            delete n;

            if (right == nullptr) return left;

            Node * min;

            right = pop_parent(right, min);
            min->right = right;
            min->left = left;

            return balance(min);
        }
        return balance(n);
    }

    // try to insert the key into the tree
    // remember the index

    Node* insert_r(const T & key, Node * p, int & index) {
        if (p == nullptr) return new Node(key);

        const int sign = comp(key, p->data);

        if (sign == less)
            p->left = insert_r(key, p->left, index);
        else {
            index += Node::get_index(p->left) + 1;
            p->right = insert_r(key, p->right, index);
        }

        return balance(p);
    }

    // remove violations of AVL-Tree property

    Node* balance(Node * n) {
        fix_height(n);
        fix_index(n);
        const int bf = n->balance_factor();

        if (bf == heavy_left) {
            if (n->left->balance_factor() > balanced)
                n->left = rotate_left(n->left);
            return rotate_right(n);
        }
        if (bf == heavy_right) {
            if (n->right->balance_factor() < balanced)
                n->right = rotate_right(n->right);
            return rotate_left(n);
        }
        return n;
    }

    // methods to keep tree properties fine

    void fix_height(Node * n) {
        if (n == nullptr) return;
        n->height = std::max(Node::get_height(n->left), Node::get_height(n->right)) + 1;
    }

    void fix_index(Node * n) {
        if (n == nullptr) return;
        n->index = Node::get_index(n->left) + Node::get_index(n->right) + 1;
    }

    // simple rotation methods

    Node * rotate_left(Node * n) {
        Node * tmp = n->right;
        n->right = tmp->left;
        tmp->left = n;

        fix_height(n);
        fix_index(n);

        fix_height(tmp);
        fix_index(tmp);

        return tmp;
    }

    Node * rotate_right(Node * n) {
        Node * tmp = n->left;
        n->left = tmp->right;
        tmp->right = n;

        fix_height(n);
        fix_index(n);

        fix_height(tmp);
        fix_index(tmp);

        return tmp;
    }

    private:
        Node * root = nullptr;
        C comp = C();
    public:
        AVLBST() = default;
        AVLBST(const C & comp): comp(comp) {}
        ~AVLBST() { Node::erase_subtrees(root); }

        bool has(const T & key) const { return has_r(key, root); }

        int insert(const T & key) {
            int index = 0;
            root = insert_r(key, root, index);
            return index;
        }

        void remove(const int & index) {
            root = remove_r(index, root);
        }
};

void run_AVL(std::istream & in, std::ostream & out) {

    // one will need the inverse comparator, as
    // the troops should stay in the decreasing height order!
    auto inverse_comparator = [](const int & a, const int & b) {
        if (a < b) return more;
        else if (a > b) return less;
        return even;
    };

    AVLBST<int, std::function<int(const int &, const int &)>> AVL(
        inverse_comparator
    );

    size_t N = 0;
    size_t key = 0;
    size_t command = 0;

    in >> N;

    for (size_t i = 0; i < N; ++i) {
        in >> command >> key;

        switch (command) {
            case commands::insert_troop: {
                out << AVL.insert(key) << '\n';
                break;
            }

            case commands::remove_troop: {
                AVL.remove(key);
                break;
            }

            default: break;
        }
    }
}

int main(int argc, char* argv[]) {
    run_AVL(std::cin, std::cout);
    return EXIT_SUCCESS;
}


