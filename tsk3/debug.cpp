#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>

template <typename T>
class circular_buffer{
    private:
        T *internal_buffer = nullptr;
        size_t _size = 0;
    public:
        circular_buffer() = default;
        ~circular_buffer() { delete []internal_buffer; };
        bool empty() { return internal_buffer == nullptr; };
        size_t double_size(const size_t where);
        size_t size() { return _size - 1; };
        T& operator[](const size_t n);
        void debug_print(const size_t h, const size_t r){
            std::cerr << "buffer contains: ";
            for (size_t i = 0; i < _size; ++i){
                if (i == r) std::cerr << "[";
                if (i == h) std::cerr << "{";
                std::cerr << " " << internal_buffer[i] << " ";
                if (i == h) std::cerr << "}";
                if (i == r) std::cerr << "]";
            }
            std::cerr << '\n';
        }
};

template <typename T>
size_t circular_buffer<T>::double_size(const size_t where){
    if (empty()){
        // I skip _size = 1
        // because somehow it worked out badly with pushing items
        // (when the deque is of shape
        // [ A ], both head and rear point to 0 item
        // when trying to push back, we get
        // [ 0, A ] (in case of integers, 0 is the default value)
        // then rear
        internal_buffer = new T[2]();
        _size = 2;
        return 0;
    }
    std::cerr << "doubles size\n";

    size_t new_size = _size * 2;
    T *new_internal_buffer = new T[new_size]();

    for (size_t i = 0; i < _size and i < where; ++i)
        new_internal_buffer[i] = std::move(internal_buffer[i]);
    for (size_t i = where; i < _size; ++i)
        new_internal_buffer[_size + i] = std::move(internal_buffer[i]);

    delete []internal_buffer;
    internal_buffer = new_internal_buffer;
    _size = new_size;
    std::cerr << "doubled and returned " << _size / 2 + where<< '\n';
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
    private:
    public:
        deque() = default;
        ~deque() = default;
        void push_back(const T item);
        void push_front(const T item);
        T pop_back();
        T pop_front();
        void print() { buffer.debug_print(head, rear); };
};

template <>
void deque<int>::push_front(const int item){
    ++items;
    if (buffer.empty()){
        // circular_buffer must be empty
        // [] -> [H]
        buffer.double_size(head);
        buffer[0] = item;
        return;
    }

    if (head == 0 and rear == buffer.size()){
        // circular_buffer must be full
        // [H##R]
        // [###R---H]
        // we must double the buffer size and 
        buffer.double_size(rear+1);
        head = buffer.size();
        buffer[head] = item;
        return;
    }
    if (head == 0){
        head = buffer.size();
        // [H######R----]
        // [#######R---H]
        buffer[head] = item;
        if (items == 1) rear = head;
        return;
    }
    if (rear < head and head - rear == 1){
        // circular buffer is full
        // also head "curves" around the array border
        // like this:
        // [###RH###]
        // thus after doubling the size head index should move
        // while the rear index remains fixed:
        // [###R---H####]
        //         ^ position of item to insert
        head = buffer.double_size(head);
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
        // circular_buffer must be empty
        // [] -> [H]
        buffer.double_size(rear);
        buffer[0] = item;
        return;
    }

    if (head == 0 and rear == buffer.size()){
        // circular_buffer must be full
        // [H##R]
        // [H###R---]
        buffer.double_size(rear+1);
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
        // thus after doubling the size head index should move
        // while the rear index remains fixed:
        // [####R---H###]
        //      ^ position of item to insert
        head = buffer.double_size(head);
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
        const size_t old_head = head;
        head = 0;
        const int old_item = buffer[old_head];
        buffer[old_head] = 0;
        if (!items) rear = head;
        return old_item;
    }
    const int old_item = buffer[head];
    buffer[head++] = 0;
    if (!items) rear = head;
    return old_item;
}

template <>
int deque<int>::pop_back(){
    if (!items) return (-1);
    --items;
    if (rear == 0){
        // this is the case when rear 
        // must hop to the last element from the first
        // [R***] -> [***R]
        // move rear index to the last item if rear points to the 0-st item
        const size_t old_rear = rear;
        const int old_item = buffer[old_rear];
        buffer[old_rear] = 0;
        rear = buffer.size();
        if (!items) head = rear;
        return old_item;
    }
    const int old_item = buffer[rear];
    buffer[rear--] = 0;
    if (!items) head = rear;
    return old_item;
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
    std::cerr << n_tasks << " commands in total\n";

    for (size_t i = 0; i < n_tasks; ++i){
        size_t command = 0;
        int expected_behavior = 0;
        int real_behavior = 0;
        in >> command >> expected_behavior;
        std::cerr << i+1 << ") ";
        
        switch (command){
            case PUSH_FRONT:
                std::cerr << "PUSHES FRONT: " << expected_behavior << '\n'; 
                TestDeque.push_front(expected_behavior);
                break;

            case POP_FRONT:
                real_behavior = TestDeque.pop_front();
                std::cerr << "POPS FRONT: expected: " << expected_behavior << ", got " << real_behavior << '\n'; 
                if (real_behavior != expected_behavior){
                    out << "NO";
                    return;
                    }
                break;

            case PUSH_BACK:
                std::cerr << "PUSHES BACK: " << expected_behavior << '\n'; 
                TestDeque.push_back(expected_behavior);
                break;

            case POP_BACK:
                real_behavior = TestDeque.pop_back();
                std::cerr << "POPS BACK: expected: " << expected_behavior << ", got " << real_behavior << '\n'; 
                if (real_behavior != expected_behavior){
                    out << "NO";
                    return;
                    }
                break;
        }
        TestDeque.print();
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

    in << "3\n" << "1 15 1 35 2 35";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "8\n" << "1 15 1 35 4 15 2 35 1 55 2 55 3 42 4 42";
    execute_commands(in, out);
    assert(out.str() == "YES");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    in << "4\n" << "1 15 1 35 4 -1 2 35";
    execute_commands(in, out);
    assert(out.str() == "NO");
}

int main(int argc, char *argv[]){
    // test_logic();
    if (argc == 2){
        std::ifstream ifs;
        ifs.open(argv[1]);
        execute_commands(ifs, std::cout);
        ifs.close();
        return EXIT_SUCCESS;
    }
    
    execute_commands(std::cin, std::cout);
    return EXIT_SUCCESS;
}