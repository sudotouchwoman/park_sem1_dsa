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
    T getRoot() const  { if (root == nullptr) return T(); return root->data; }
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
    // comparator function
    auto lt = [](const int & lhs, const int & rhs) {
        return lhs <= rhs;
    };

    // create empty binary tree and pass lambda
    // as the building parameter
    BinTree<int> BT(lt);

    int value = 0;
    // populate the tree
    while (in >> value) {
        BT.add(value);
    }

    bool all_same = true;
    int root = BT.getRoot();

    // lambda to be applied to each node
    // while traversing
    auto walker = [&](const int & a) {
        if (a != root) all_same = false;
    };

    BT.traverse(walker);
    out << ( all_same ? 1 : 0);
}

int main(int argc, char* argv[]) {
    // test_bintree();
    run_bintree(std::cin, std::cout);
}