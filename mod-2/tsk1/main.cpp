#include <algorithm>
#include <functional>
#include <cassert>
#include <vector>
#include <iostream>
#include <sstream>

// implement hash table with open addressing ("set of strings")
// collisions should be resolved via double hashing
// hash table should have the following interface functions:
// add - add new key if not present
// remove - remove given key if present
// has - check whether key is present
// the table should be dynamic: if the number of items/capacity ratio
// is greater than 3/4, rehashing should be performed

enum cell {
    empty = 1,
    deleted,
    filled
};

template<
    typename Key,
    typename Hasher = std::function<size_t(const Key &, const size_t i)>,
    typename isEqual = std::function<bool(const Key &, const Key &)>>
class HashTable {
private:
    struct HashNode {
        cell status = cell::empty;
        Key key;
    };

    const double alpha = 0.75;
    size_t items = 0;
    size_t size = 8;
    std::vector<HashNode> buckets = std::vector<HashNode>(8);

    const Hasher hasher;
    const isEqual equal;
private:
    bool must_resize() const;
    void resize();
public:
    HashTable() = delete;
    HashTable(const Hasher & hasher, const isEqual & equal) :
        hasher(hasher), equal(equal) {}
    ~HashTable() = default;

    bool add(const Key &);
    bool remove(const Key &);
    bool has(const Key &) const;
};

template<
    typename Key,
    typename Hasher,
    typename isEqual>
bool HashTable<Key, Hasher, isEqual>::must_resize() const {
    return static_cast<double>(items) / static_cast<double>(size) >= alpha;
}

template<
    typename Key,
    typename Hasher,
    typename isEqual>
void HashTable<Key, Hasher, isEqual>::resize() {
    auto moved = std::move(buckets);
    size *= 2;
    items = 0;
    buckets = std::vector<HashNode>(size);

    for (const auto & bucket : moved) {
        if (bucket.status != cell::filled) continue;
        add(bucket.key);
    }
}

template<
    typename Key,
    typename Hasher,
    typename isEqual>
bool HashTable<Key, Hasher, isEqual>::has(const Key & key) const {
    size_t iter = 0;
    while (iter < size) {
        const size_t idx = hasher(key, iter++) % size;
        
        if (buckets[idx].status == cell::empty) return false;

        if (buckets[idx].status == cell::filled
            and equal(key, buckets[idx].key)) return true;
    }
    return false;
}

template<
    typename Key,
    typename Hasher,
    typename isEqual>
bool HashTable<Key, Hasher, isEqual>::add(const Key & key) {
    if (must_resize()) resize();

    size_t iter = 0;
    size_t first_deleted = 0;
    bool encountered_deleted = false;

    while (iter < size) {
        const size_t idx = hasher(key, iter++) % size;

        if (buckets[idx].status == cell::filled
            and equal(key, buckets[idx].key)) return false;

        if (buckets[idx].status == cell::deleted and not encountered_deleted) {
            encountered_deleted = true;
            first_deleted = idx;
        }

        if (buckets[idx].status == cell::empty) {
            if (encountered_deleted) {
                buckets[first_deleted].status = cell::filled;
                buckets[first_deleted].key = key;
                ++items;
                return true;
            }
            buckets[idx].status = cell::filled;
            buckets[idx].key = key;
            ++items;
            return true;
        }
    }
    if (encountered_deleted) {
        buckets[first_deleted].status = cell::filled;
        buckets[first_deleted].key = key;
        ++items;
        return true;
    }
    return false;
}

template<
    typename Key,
    typename Hasher,
    typename isEqual>
bool HashTable<Key, Hasher, isEqual>::remove(const Key & key) {
    if (items == 0) return false;

    size_t iter = 0;
    while (iter < size) {
        const size_t idx = hasher(key, iter++) % size;
        
        if (buckets[idx].status == cell::empty) return false;

        if (buckets[idx].status == cell::filled
            and equal(key, buckets[idx].key)) {
                buckets[idx].status = cell::deleted;
                --items;
                return true;
            }
    }
    return false;
}

void run_hashtable(std::istream & in, std::ostream & out) {
    auto defequal = [](const auto & a, const auto & b) { return a == b; };
    
    auto h2 = [](const std::string & s) {
        size_t hash = 0;
        for (const auto & c : s)
            hash = hash * 13 + c;
        return hash;
    };

    auto string_hasher = [&h2](const std::string & s, const size_t i) {
        const auto hash = h2(s);
        return hash + i*(hash | 1);
    };

    HashTable<std::string> HT(string_hasher, defequal);

    bool result;
    char command = 0;
    std::string key;
    while (in >> command >> key) {
        switch (command) {
            case '+':
                result = HT.add(key);
                break;
            case '-':
                result = HT.remove(key);
                break;
            case '?':
                result = HT.has(key);
                break;
            default:
                break;
        }
        out << (result ? "OK\n" : "FAIL\n");
    }
}

void test_hashtable() {
    std::stringstream in;
    std::stringstream out;

    in << "+ hello + hello + world - world - hello - world + hello ? world ? hello";
    run_hashtable(in, out);
    assert(out.str() == "OK\nFAIL\nOK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
    std::stringstream().swap(in);
    std::stringstream().swap(out);

    std::cerr << "OK\n";
}

int main(int keyc, char* keyv[]) {
    // test_hashtable();
    run_hashtable(std::cin, std::cout);
    return EXIT_SUCCESS;
}
