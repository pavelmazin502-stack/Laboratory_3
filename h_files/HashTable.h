#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <cstddef>

class HashTable {
private:
    struct Node { // Внутренняя структура узла для разрешения коллизий
        std::string key; // ключ
        std::string value; // значение
        Node* next;
        Node(const std::string& k, const std::string& v) {
            key = k;
            value = v;
            next = nullptr;
        }
    };

    Node** buckets; // динамический массив указателей
    size_t capacity; // вместимость массива
    size_t size; // количество сохраненных пар

    size_t hash_function(const std::string& key) const; // хе-функция (строка - число)

public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    void clear();

    void insert(const std::string& key, const std::string& value);
    void remove(const std::string& key);
    bool contains(const std::string& key) const;
    std::string get(const std::string& key) const;
    
    size_t get_size() const;
    void print() const;
};

#endif