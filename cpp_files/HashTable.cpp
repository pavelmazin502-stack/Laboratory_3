#include "../h_files/HashTable.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

using namespace std;

// Вычисление хеш-кода для строки
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
// Текстовая сериализация
void HashTable::serialize_text(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        throw runtime_error("Cannot open file for writing");
    }

    // Записываем общее количество пар (ключ-значение)
    out << size << "\n";

    // Проходим по всем корзинам и по цепочкам коллизий внутри них
    for (size_t i = 0; i < capacity; ++i) {
        Node* curr = buckets[i];
        while (curr != nullptr) {
            out << curr->key << "\n";
            out << curr->value << "\n";
            curr = curr->next;
        }
    }
}

// Текстовая десериализация
void HashTable::deserialize_text(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        throw runtime_error("Cannot open file for reading");
    }

    clear();
    size_t count = 0;
    if (!(in >> count)) return;

    string dummy;
    getline(in, dummy); // Съедаем перенос строки после числа count

    // Каждая пара занимает две строки: сначала ключ, затем значение
    for (size_t i = 0; i < count; ++i) {
        string k, v;
        getline(in, k);
        getline(in, v);
        insert(k, v);
    }
}

// Бинарная сериализация
void HashTable::serialize_binary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out.is_open()) {
        throw runtime_error("Cannot open binary file for writing");
    }

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (size_t i = 0; i < capacity; ++i) {
        Node* curr = buckets[i];
        while (curr != nullptr) {
            size_t k_len = curr->key.size();
            out.write(reinterpret_cast<const char*>(&k_len), sizeof(k_len));
            out.write(curr->key.data(), k_len);

            size_t v_len = curr->value.size();
            out.write(reinterpret_cast<const char*>(&v_len), sizeof(v_len));
            out.write(curr->value.data(), v_len);

            curr = curr->next;
        }
    }
}

// Бинарная десериализация
void HashTable::deserialize_binary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        throw runtime_error("Cannot open binary file for reading");
    }

    clear();
    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        size_t k_len = 0;
        in.read(reinterpret_cast<char*>(&k_len), sizeof(k_len));
        string k(k_len, '\0');
        in.read(&k[0], k_len);

        size_t v_len = 0;
        in.read(reinterpret_cast<char*>(&v_len), sizeof(v_len));
        string v(v_len, '\0');
        in.read(&v[0], v_len);

        insert(k, v);
    }
}