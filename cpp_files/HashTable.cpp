#include "../h_files/HashTable.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Вычисление хеш-кода для строки=
size_t HashTable::hash_function(const string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + static_cast<unsigned char>(c); // Хеш функция
    }
    return hash % capacity;
}
// Инициализация пустой хеш-таблицы
HashTable::HashTable() {
    capacity = 128;// корзины
    size = 0; // количество пар
    buckets = new Node*[capacity];// массив указателей
    for (size_t i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}

HashTable::~HashTable() {
    clear();
    delete[] buckets;
}
// оператор копирования
HashTable::HashTable(const HashTable& other) {
    capacity = other.capacity;
    size = 0;
    buckets = new Node*[capacity];
    for (size_t i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }

    for (size_t i = 0; i < capacity; i++) {
        Node* curr = other.buckets[i];
        while (curr != nullptr) {
            insert(curr->key, curr->value);
            curr = curr->next;
        }
    }
}
// оператор присваивания
HashTable& HashTable::operator=(const HashTable& other) {
    if (this == &other) {
        return *this;
    }

    clear();
    delete[] buckets;

    capacity = other.capacity;
    size = 0;
    buckets = new Node*[capacity];
    for (size_t i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }

    for (size_t i = 0; i < capacity; i++) {
        Node* curr = other.buckets[i];
        while (curr != nullptr) {
            insert(curr->key, curr->value);
            curr = curr->next;
        }
    }
    return *this;
}

void HashTable::clear() {
    for (size_t i = 0; i < capacity; i++) {
        Node* curr = buckets[i];
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    size = 0;
}

void HashTable::insert(const string& key, const string& value) {
    size_t index = hash_function(key);
    Node* curr = buckets[index];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }

    Node* node = new Node(key, value);
    node->next = buckets[index];
    buckets[index] = node;
    size++;
}

void HashTable::remove(const string& key) {
    size_t index = hash_function(key);
    Node* curr = buckets[index];
    Node* prev = nullptr;

    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) {
                buckets[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

bool HashTable::contains(const string& key) const {
    size_t index = hash_function(key);
    Node* curr = buckets[index];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

string HashTable::get(const string& key) const {
    size_t index = hash_function(key);
    Node* curr = buckets[index];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    throw runtime_error("Key not found");
}

size_t HashTable::get_size() const {
    return size;
}

void HashTable::print() const {
    cout << "{\n";
    for (size_t i = 0; i < capacity; i++) {
        Node* curr = buckets[i];
        if (curr != nullptr) {
            cout << "  [" << i << "]: ";
            while (curr != nullptr) {
                cout << "{" << curr->key << ": " << curr->value << "}";
                if (curr->next != nullptr) cout << " -> ";
                curr = curr->next;
            }
            cout << "\n";
        }
    }
    cout << "}\n";
}