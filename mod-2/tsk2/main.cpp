#include <functional>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>

// implement binary tree and in-order traversal
// inputs: N, number of elements
// N elements (integers)
// outputs: given integers bun in sorted order

template
<typename T,
typename LT = std::function<bool(const T &, const T &)>>
class BinTree {
private:
    struct Node {
        Node * left = nullptr;
        Node * right = nullptr;
        T data;
        Node() = default;
        Node(T && data): data(data) {}
        Node(const T & data) : data(data) {}
    };
    Node* root = nullptr;
    LT lt;
public:
    BinTree() = delete;
    BinTree(const LT & lt) : lt(lt) {}
    ~BinTree();
    void add(T);
    void traverse(const std::function<void(const T &)> & func) const;
};

template<typename T, typename LT>
void BinTree<T, LT>::add(T item) {
    if (root == nullptr) {
        root = new Node();
        root->data = item;
        return;
    }

    auto walker = root;

    while (true) {
        // naive yet non-recursive
        // intem insertion
        // traverse the tree down to the empty
        // node in order to attach the value to it
        if (lt(item, walker->data)) {

            if (walker->left != nullptr) {
                walker = walker->left;
                continue;
            }

            walker->left = new Node(std::move(item));
            break;            
        }

        if (walker->right != nullptr) {
            walker = walker->right;
            continue;
        }

        walker->right = new Node(std::move(item));
        break;
    }
}

template<typename T, typename LT>
void BinTree<T, LT>::traverse(const std::function<void(const T &)> & func) const {
    std::stack<Node*> nodes_to_walk;
    auto walker = root;

    while (walker != nullptr or not nodes_to_walk.empty()) {

        // push the left child of current node
        // until we reach the empty node
        while(walker != nullptr) {
            nodes_to_walk.push(walker);
            walker = walker->left;
        }

        // grab the top item and pass it to the
        // given predicate
        walker = nodes_to_walk.top();
        nodes_to_walk.pop();
        func(walker->data);
        // then reassign the walker to the
        // right child of the grabbed item
        walker = walker->right;
    }
}


template<typename T, typename LT>
BinTree<T, LT>::~BinTree() {
    std::queue<Node*> nodes_to_walk;
    if (root != nullptr) nodes_to_walk.push(root);

    while(not nodes_to_walk.empty()) {
        auto walker = nodes_to_walk.front();
        nodes_to_walk.pop();
        if (walker->left != nullptr) nodes_to_walk.push(walker->left);
        if (walker->right != nullptr) nodes_to_walk.push(walker->right);
        delete walker;
    }
}

void run_bintree(std::istream & in, std::ostream & out) {
    const size_t N = [&in]() {
        size_t N = 0;
        in >> N;
        return N;
    }();

    // comparator function
    auto lt = [](const long & lhs, const long & rhs) {
        return lhs <= rhs;
    };

    // create empty binary tree and pass lambda
    // as the building parameter
    BinTree<long> BT(lt);

    // populate the tree
    for (size_t i = 0; i < N; ++i) {
        long value = 0;
        in >> value;
        BT.add(value);
    }

    // lambda to be applied to each node
    // while traversing
    // note this last empty space!
    auto walker = [&out](const long & a) {
        out << a << ' ';
    };

    BT.traverse(walker);
}

// simple test from contest
// just to make sure everything worked out smoothly
void test_bintree() {
    std::stringstream in;
    std::stringstream out;

    in << "10 9 10 4 3 2 7 8 5 1 6";
    run_bintree(in, out);
    assert(out.str() == "1 2 3 4 5 6 7 8 9 10 ");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    std::cerr << "OK\n";
}

int main(int argc, char* argv[]) {
    // test_bintree();
    run_bintree(std::cin, std::cout);
}