#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <functional>

#define DEF_MIN_ITEMS 2

// Interface of the heap consists of:
// 

template <typename T, typename Comparator>
class heap {
    private:
        T* _internal_buffer = nullptr;
        size_t _capacity = 0;  // current capacity of the internal buffer
        size_t _items = 0;  // current number of items in the heap: 
        bool _empty = true;  // flag as size_t is used so that _items just cannot be set to -1
        const Comparator _cmp; // comparator function instance, maybe lambda
    private:
        bool _isFull() { return _items == _capacity; }
        void _DoubleSize();
        void _SiftDown(size_t);
        void _SiftUp(size_t);
    public:
        explicit heap(const Comparator cmp = Comparator()): _cmp(cmp) {}
        ~heap() { delete [] _internal_buffer; }
        bool IsEmpty() { return _empty; }
        size_t size() { return _items; }
        void Push(const T&);
        void Heapify();
        void InsertIntoHeap(const T&);
        void RemoveTop();
        T& Top() { return _internal_buffer[0]; }
};

template <typename T, typename Comparator>
void heap<T, Comparator>::_DoubleSize() {
    // std::cerr << "DOUBLES SIZE\n";
    // double the capacity of internal buffer
    // if empty, initialize the buffer
    if (_empty) {
        _internal_buffer = new T[DEF_MIN_ITEMS]();
        _capacity = DEF_MIN_ITEMS;
        _empty = false;
        return;
    }
    const size_t new_capacity = _capacity * 2;
    T* new_internal_buffer = new T[new_capacity]();

    for (size_t i = 0; i < _items; ++i) {
        new_internal_buffer[i] = std::move(_internal_buffer[i]);
    }

    delete [] _internal_buffer;
    _internal_buffer = new_internal_buffer;
    _capacity = new_capacity;
}

template <typename T, typename Comparator>
void heap<T, Comparator>::Push(const T& value) {
    // std::cerr << "PUSHES\n";
    // push item to the end of the internal buffer
    // this should be used before calling Heapify()!
    if (_isFull()) _DoubleSize();
    _internal_buffer[_items++] = value;
}

template <typename T, typename Comparator>
void heap<T, Comparator>::InsertIntoHeap(const T& value) {
    // std::cerr << "INSERTS INTO HEAP\n";
    // this method pushes the item into the internal buffer
    // and resolves heap property violations by sifting it up
    Push(value);
    _SiftUp(_items-1);
}

template <typename T, typename Comparator>
void heap<T, Comparator>::RemoveTop() {
    // std::cerr << "REMOVES TOP\n";
    // remove the top element of the heap
    // basically swap it with the last and sift
    // the latter down
    // the Q is: are there going to be memleaks?
    // in my scenario, no new elements are being add,
    // but what if we added one into the heap?
    // doing this headlessly would overwrite the existing object...
    // should the top item be deleted? guess so
    // yes, it turned out that the destructor is being called
    // so no problems with overwriting the heap items
    if (_empty) return;

    if (_items == 1) {
        // the heap contains single element
        // thus for effective memory management
        // the buffer should be cleared
        delete [] _internal_buffer;
        _internal_buffer = nullptr;
        _empty = true;
        _capacity = 0;
        _items = 0;
        return;
    }

    // removing basically means swapping first with the last elems
    // and reducing the heap size with further
    // sifting down the 1st elem to preserve the heap property
    std::swap(_internal_buffer[--_items], _internal_buffer[0]);
    _SiftDown(0);
}

template <typename T, typename Comparator>
void heap<T, Comparator>::Heapify() {
    // fine iterative heapify which works for O(n)
    // std::cerr << "HEAPIFY\n";
    // std::cerr << "there are " << _items << " items in the heap\n";
    if (_empty) return;
    for (size_t i = _items; i > 0; --i) {
        // I figured out a trick to handle unsigned integer type :)
        _SiftDown(i - 1);
    }
}

template <typename T, typename Comparator>
void heap<T, Comparator>::_SiftDown(size_t idx) {
    while (true) {
        // std::cerr << "SiftDown called idx=" << idx << "\n";
        // sift down element iteratively to
        // preserve the heap property
        // i.e., the parent node is "larger"
        // than both of its children
        // parenthesis mean that min-heap basically works samely,
        // only the comparator has to be changed
        const size_t largest = [&]() {
            size_t largest = idx;
            const size_t left_c = idx * 2 + 1;
            const size_t right_c = idx * 2 + 2;

            // find max element within given node and its children (if any)
            if (left_c < _items and _cmp(_internal_buffer[left_c], _internal_buffer[largest]))
                // left child > idx
                largest = left_c;
            if (right_c < _items and _cmp(_internal_buffer[right_c], _internal_buffer[largest]))
                // right child > idx
                largest = right_c;

            return largest;
        }();

        if (largest == idx) break;  // heap property is fixed
        std::swap(_internal_buffer[idx], _internal_buffer[largest]);
        idx = largest;
    }
}

template <typename T, typename Comparator>
void heap<T, Comparator>::_SiftUp(size_t idx) {
    // std::cerr << "SiftUp called idx=" << idx << "\n";
    if (idx == 0) return;

    while (true) {
        // sift up all element iteratively
        // to preserve the heap property
        // the process is simular to the one of _SiftDown,
        // but it is easier to find the smallest node
        const size_t smallest = [&]() {
            size_t smallest = idx;
            const size_t parent = idx / 2;

            // find smallest element within given node and its parent
            if (not _cmp(_internal_buffer[smallest], _internal_buffer[parent]))
                // parent <= idx
                smallest = parent;

            return smallest;
        }();

        if (smallest == idx) break;  // heap property is fixed
        std::swap(_internal_buffer[idx], _internal_buffer[smallest]);
        idx = smallest;
    }
}

// Process structure is a wrapper that contains process priority, total time to work and worked time
// I thought of making P and T const, but a problem came out: we cannot reassign const values...
// operator= should be overriden and those components are left mutable
// this way, I consider this implementation to be quite dirty
// it combines such fine things as overriding << operator with dirty comparator definition
// and lots of dumb checks in heap...

struct Process {
    mutable size_t P;
    mutable size_t T;
    size_t t = 0;
    Process(const size_t _P = 0, const size_t _T = 0) : P(_P), T(_T) {};
    Process(const Process & other) : P(other.P), T(other.T), t(other.t) {};
    ~Process() = default;
    Process operator=(const Process & other) {
        P = other.P ;
        T = other.T;
        t = other.t ;
        return *this; };
    friend std::ostream & operator<<(std::ostream &, const Process &);
};

std::ostream & operator<<(std::ostream & out, const Process & obj) {
    out << "About process: P=" << obj.P << " T=" << obj.T << " t=" << obj.t << "\n";
    return out;
}

// Driver code: creates min heap, pops and inserts processes and counts interruptions
// the heap is initially treated as ordinary dynamic array
// then the Heapify() method is called to build the heap based on provided comparator
// I wonder, what is more graceful: created a template attribute for comparator
// or providing the heap with external comparator each time
// guess that by creating a method like "SetComparator()" I can solve both problems 

void run_process_manager(std::istream & in, std::ostream & out) {
    const size_t n_proc = [&in]() {
        size_t n;
        in >> n;
        return n;
    }();

    heap<Process, std::function<bool(const Process&, const Process&)>> jobs(
        [](const Process& lhs, const Process& rhs) {
            // shows debug info
            // god forgive me this is so bad
            // std::cerr << "### comparator called ###\n";
            // std::cerr << "left operand: P: " << lhs.P << " T: " << lhs.T << " t: " << lhs.t;
            // std::cerr << " Priority: " << lhs.P * (lhs.t + 1) << '\n';
            // std::cerr << "right operand: P: " << rhs.P << " T: " << rhs.T << " t: " << rhs.t;
            // std::cerr << " Priority: " << rhs.P * (rhs.t + 1) << '\n';
            return lhs.P * (lhs.t + 1) > rhs.P * (rhs.t + 1);
            });

    for (size_t i = 0; i < n_proc; ++i) {
        size_t P = 0, T = 0;
        in >> P >> T;
        jobs.Push(Process(P, T));
    }

    jobs.Heapify();

    size_t interruptions = 0;
    while (not jobs.IsEmpty()) {
        ++interruptions;
        // std::cerr << "--- Interruptions: " << interruptions << "---\n";
        
        Process most_relevant = jobs.Top();
        // std::cerr << most_relevant;
        jobs.RemoveTop();

        most_relevant.t = most_relevant.t + most_relevant.P;

        if (most_relevant.t >= most_relevant.T) {
            // std::cerr << "Process finished, not returning to job pool\n";
            continue;
        }

        // std::cerr << "Process not finished, puts back to job pool\n";
        // std::cerr << most_relevant;
        jobs.InsertIntoHeap(most_relevant);
    }

    out << interruptions;
}

void test_process_manager() {
    std::stringstream in;
    std::stringstream out;
    
    in << "1 1 2";
    run_process_manager(in, out);
    assert(out.str() == "2");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "1 2 2";
    run_process_manager(in, out);
    assert(out.str() == "1");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "3 1 10 1 5 2 5";
    run_process_manager(in, out);
    assert(out.str() == "18");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "3 1 5 1 10 2 5";
    run_process_manager(in, out);
    assert(out.str() == "18");
    std::stringstream().swap(in);
    std::stringstream().swap(out);
    std::cerr << "OK\n";
}



int main(int argc, char *argv[]) {
    // test_process_manager();
    run_process_manager(std::cin, std::cout);
    return EXIT_SUCCESS;
}
