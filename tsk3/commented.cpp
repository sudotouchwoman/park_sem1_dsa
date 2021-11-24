#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>

template <typename T>
class circular_buffer{
    // this is actually more like a usuall buffer but
    // the `expand_and_split()` method would double the buffer size
    // and move items from the old one with splitting it at specified `where` index
    // this needs to be done to, well, conserve the circular buffer property, otherwise
    // head and rear will continue to point to neighbouring items!

    // I do not check such things as index errors etc because the task is about different thing
    // however, I am wondering how such data structure should perform when asked to return a value
    // should it return by value? then what if there aren't any?
    // as far as I understood in C++ such problems are solved with value returning, the value at this index
    // may be overwritten later, so the underlying data structure should be copyable
    // but what about movable objects? Is it possible to return an rvalue?
    private:
        T *internal_buffer = nullptr;
        size_t _size = 0;
    public:
        circular_buffer() = default;
        ~circular_buffer() { delete []internal_buffer; };
        bool empty() { return internal_buffer == nullptr; };
        size_t expand_and_split(const size_t where);
        size_t size() { return _size - 1; };
        T& operator[](const size_t n);
};

template <typename T>
size_t circular_buffer<T>::expand_and_split(const size_t where){
    if (empty()){
        // I skip _size = 1
        // because somehow it worked out badly with pushing items
        // (when the deque is of shape
        // [ A ], both head and rear point to 0 item
        // when trying to push back, we get
        // [ 0, A ] (in case of integers, 0 is the default value)
        internal_buffer = new T[2]();
        _size = 2;
        return 0;
    }

    const size_t new_size = _size * 2;
    T *new_internal_buffer = new T[new_size]();

    for (size_t i = 0; i < _size and i < where; ++i)
        new_internal_buffer[i] = std::move(internal_buffer[i]);
    for (size_t i = where; i < _size; ++i)
        new_internal_buffer[_size + i] = std::move(internal_buffer[i]);

    delete []internal_buffer;
    internal_buffer = new_internal_buffer;
    _size = new_size;
    // the return value below represents this:
    // [--RH] (this method will be called with `where` = head = 3)
    // after doubling we should get this:
    // [--R----H]
    // so, current head is still the last item in the buffer,
    // but its index is now 4 + 3 = 7
    // which can be rewritten as 8 / 2 + 3 = 7
    // q.e.d.
    // note that in the case we want to push front,
    // the returned index should be decremented first
    return _size / 2 + where;
}

template <typename T>
T& circular_buffer<T>::operator[](const size_t n){
    return internal_buffer[n];
}

template <typename T>
    class deque{
    private:
        circular_buffer<T> buffer;
        size_t head = 0;
        size_t rear = 0;
        size_t items = 0;
    public:
        deque() = default;
        ~deque() = default;
        void push_back(const T item);
        void push_front(const T item);
        T pop_back();
        T pop_front();
};

template <>
void deque<int>::push_front(const int item){
    ++items;
    if (buffer.empty()){
        // circular_buffer must be empty
        // [] -> [H]
        buffer.expand_and_split(head);
        buffer[0] = item;
        return;
    }

    if (head == 0 and rear == buffer.size()){
        // circular_buffer must be full
        // [H##R]
        // [###R---H]
        // (# means filled buffer cell while - means empty cell)
        // we must double the buffer size and 
        buffer.expand_and_split(rear+1);
        head = buffer.size();
        buffer[head] = item;
        return;
    }
    if (head == 0){
        head = buffer.size();
        // we need to "curve" buffer border
        // [H######R----]
        // [#######R---H]
        // (# means filled buffer cell while - means empty cell)
        buffer[head] = item;
        if (items == 1) rear = head;
        return;
    }
    if (rear < head and head - rear == 1){
        // circular buffer is full
        // also head "curves" around the array border
        // like this:
        // [###RH###]
        // (# means filled buffer cell while - means empty cell)
        // thus after doubling the size head index should move
        // while the rear index remains fixed:
        // [###R---H####]
        //         ^ position of item to insert
        head = buffer.expand_and_split(head);
        buffer[--head] = item;
        return;
    }

    buffer[--head] = item;
    if (items == 1) rear = head;
}

template <>
void deque<int>::push_back(const int item){
    ++items;
    if (buffer.empty()){
        // circular_buffer must be empty,
        // so add a single item
        // [] -> [H]
        buffer.expand_and_split(rear);
        buffer[0] = item;
        return;
    }

    if (head == 0 and rear == buffer.size()){
        // circular_buffer must be full
        // [H##R]
        // [H###R---]
        buffer.expand_and_split(rear+1);
        buffer[++rear] = item;
        return;
    }
    if (rear == buffer.size()){
        // [----H######R]
        // [R---H#######]
        rear = 0;
        buffer[rear] = item;
        if (items == 1) head = rear;
        return;
    }
    if (rear < head and head - rear == 1){
        // circular buffer is full
        // also head "curves" around the array border
        // like this:
        // [###RH###] 
        // (# means filled buffer cell while - means empty cell)
        // thus after doubling the size head index should move
        // while the rear index remains fixed:
        // [####R---H###]
        //      ^ position of item to insert
        head = buffer.expand_and_split(head);
        // I use + 1 because otherwise 
        buffer[++rear] = item;
        return;
    }

    buffer[++rear] = item;
    if (items == 1) head = rear;
}

template <>
int deque<int>::pop_front(){
    if (!items) return (-1);
    --items;
    if (head == buffer.size()){
        // [***H] -> [H***]
        // move head index to the 0 if head points to the last item
        // (skip circle)
        const size_t old_head = head;
        head = 0;
        if (!items) rear = head; 
        // in case we are left without items
        // head and rear should point to the same empty cell
        // I personally dislike such way but have no idea
        // how to refactor this (bc it kinda smells)
        // at least, I did not look into the web how these
        // circular things should work
        return buffer[old_head];
    }
    if (!items) rear = head + 1;
    return buffer[head++];
}

template <>
int deque<int>::pop_back(){
    if (!items) return (-1);
    --items;
    if (rear == 0){
        // this is the case when rear 
        // must hop to the last element from the first
        // [R...] -> [...R] (dots mean something inside the buffer)
        // move rear index to the last item if it points to the 0-st item 
        // (skip circle)
        const size_t old_rear = rear;
        rear = buffer.size();
        if (!items) head = rear;
        return buffer[old_rear];
    }
    if (!items) head = rear - 1;
    return buffer[rear--];
}

enum COMMANDS{
    PUSH_FRONT = 1,
    POP_FRONT,
    PUSH_BACK,
    POP_BACK
};

void execute_commands(std::istream &in, std::ostream &out){
    size_t n_tasks = 0;
    deque<int> TestDeque;
    in >> n_tasks;

    for (size_t i = 0; i < n_tasks; ++i){
        size_t command = 0;
        int expected_behavior = 0;
        int real_behavior = 0;
        in >> command >> expected_behavior;
        
        switch (command){
            case PUSH_FRONT:
                TestDeque.push_front(expected_behavior);
                break;

            case POP_FRONT:
                real_behavior = TestDeque.pop_front();
                if (real_behavior != expected_behavior){
                    out << "NO";
                    return;
                    }
                break;

            case PUSH_BACK:
                TestDeque.push_back(expected_behavior);
                break;

            case POP_BACK:
                real_behavior = TestDeque.pop_back();
                if (real_behavior != expected_behavior){
                    out << "NO";
                    return;
                    }
                break;
        }
    }
    out << "YES";
}

void test_logic(){
    // some simple tests I wrote for this problem
    std::stringstream in;
    std::stringstream out;

    in << "3\n" << "1 44 3 50 2 44";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "2\n" << "2 -1 4 -1";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "10\n" << "1 100 1 200 1 300 1 400 1 500 1 600 2 600 4 100 4 200 2 500";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "8\n" << "1 15 1 35 4 15 2 35 1 55 2 55 3 42 4 42";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "10\n" << "3 44 3 25 1 99 3 500 3 300 2 99 4 300 4 500 2 44 2 25";
    execute_commands(in, out);
    assert(out.str() == "YES");
}

int main(int argc, char *argv[]){
    // test_logic();
    if (argc == 3){
        // i/o from file if ones are specified via command line argument
        std::ifstream ifs;
        std::ofstream ofs;
        ifs.open(argv[1]);
        ofs.open(argv[2]);

        execute_commands(ifs, ofs);
        
        ifs.close();
        ofs.close();
        return EXIT_SUCCESS;
    }
    
    execute_commands(std::cin, std::cout);
    return EXIT_SUCCESS;
}