#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType> >::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType> >::const_iterator;

    explicit HashMap(Hash hash = Hash())
            : hash(hash)
            , element_counter(0)
    {
        hash_table.resize(16);
    }

    HashMap(const HashMap& other)
            : hash(other.hash_function())
            , element_counter(0)
    {
        hash_table.resize(16);
        for (auto it = other.begin(); it != other.end(); ++it) {
            insert(*it);
        }
    }

    template<typename Iterator>
    HashMap(Iterator begin, Iterator end, Hash hash = Hash())
            : hash(hash)
            , element_counter(0)
    {
        hash_table.resize(16);
        for (auto it = begin; it != end; ++it) {
            insert(*it);
        }
    }

    HashMap(std::initializer_list<std::pair<const KeyType, ValueType> > elements_list, Hash hash = Hash())
            : hash(hash)
            , element_counter(0)
    {
        hash_table.resize(16);
        for (auto& element : elements_list) {
            insert(element);
        }
    }

    ~HashMap() = default;

    const HashMap& operator=(const HashMap& other) {
        clear();
        hash_table.resize(16);
        for (auto it = other.begin(); it != other.end(); ++it) {
            insert(*it);
        }
        return other;
    }

    size_t size() const {
        return element_counter;
    }

    bool empty() const {
        return element_counter == 0;
    }

    Hash hash_function() const {
        return hash;
    }

    void insert(std::pair<const KeyType, ValueType> element) {
        if (element_counter > hash_table.size() / 2) {
            resize(hash_table.size() * 2);
        }
        size_t element_hash = hash(element.first) % hash_table.size();
        if (hash_table[element_hash].size) {
            if (find(element.first) != element_list.end()) {
                return;
            }
            hash_table[element_hash].begin = element_list.insert(hash_table[element_hash].begin, element);
        } else {
            hash_table[element_hash].begin = element_list.insert(element_list.end(), element);
        }
        ++hash_table[element_hash].size;
        ++element_counter;
    }

    void erase(KeyType element_key) {
        auto it = find(element_key);
        if (it != element_list.end()) {
            size_t element_hash = hash(element_key) % hash_table.size();
            if (it == hash_table[element_hash].begin) {
                ++hash_table[element_hash].begin;
            }
            element_list.erase(it);
            --hash_table[element_hash].size;
            --element_counter;
        }
    }

    const_iterator find(KeyType element_key) const {
        size_t element_hash = hash(element_key) % hash_table.size();
        if (hash_table[element_hash].size) {
            auto it = hash_table[element_hash].begin;
            for (size_t i = 0; i != hash_table[element_hash].size; ++i, ++it) {
                if (it->first == element_key) {
                    return it;
                }
            }
        }
        return element_list.end();
    }

    iterator find(KeyType element_key) {
        size_t element_hash = hash(element_key) % hash_table.size();
        if (hash_table[element_hash].size) {
            auto it = hash_table[element_hash].begin;
            for (size_t i = 0; i != hash_table[element_hash].size; ++i, ++it) {
                if (it->first == element_key) {
                    return it;
                }
            }
        }
        return element_list.end();
    }

    ValueType& operator[](KeyType element_key) {
        auto it = find(element_key);
        if (it == element_list.end()) {
            insert(std::make_pair(element_key, ValueType()));
            it = find(element_key);
        }
        return it->second;
    }

    const ValueType& at(KeyType element_key) const {
        auto it = find(element_key);
        if (it == element_list.end()) {
            throw std::out_of_range("Invalid key");
        }
        return it->second;
    }

    void clear() {
        for (auto element : element_list) {
            size_t element_hash = hash(element.first) % hash_table.size();
            hash_table[element_hash].size = 0;
        }
        element_list.clear();
        element_counter = 0;
    }

    iterator begin() {
        return element_list.begin();
    }

    iterator end() {
        return element_list.end();
    }

    const_iterator begin() const {
        return element_list.begin();
    }

    const_iterator end() const {
        return element_list.end();
    }

private:
    struct Section {
        size_t size;
        iterator begin;

        Section()
                : size(0)
        { }

        ~Section() = default;
    };

    void resize(size_t new_size) {
        std::list<std::pair<const KeyType, ValueType> > tmp = element_list;
        hash_table.clear();
        hash_table.resize(new_size);
        element_list.clear();
        element_counter = 0;
        for (auto element : tmp) {
            insert(element);
        }
    }

    Hash hash;
    std::vector<Section> hash_table;
    std::list<std::pair<const KeyType, ValueType> > element_list;
    size_t element_counter;
};
