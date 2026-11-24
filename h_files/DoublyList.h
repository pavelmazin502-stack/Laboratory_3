#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

#include <string>
#include <cstddef>

class DoublyList {
private:
    struct Node {
        std::string data;
        Node* prev;
        Node* next;
        Node(const std::string& val) {
            data = val;
            prev = nullptr;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    size_t size;

public:
    DoublyList();
    ~DoublyList();

    DoublyList(const DoublyList& other);
    DoublyList& operator=(const DoublyList& other);

    void clear();

    void push_front(const std::string& value);
    void push_back(const std::string& value);
    void pop_front();
    void pop_back();
    void remove_by_value(const std::string& value);

    bool find(const std::string& value) const;
    size_t get_size() const;
    void print() const;

    // Текстовая сериализация и десериализация
    void serialize_text(const std::string& filename) const;
    void deserialize_text(const std::string& filename);

    // Бинарная сериализация и десериализация
    void serialize_binary(const std::string& filename) const;
    void deserialize_binary(const std::string& filename);

    // вставка до/после 
    bool insert_before(const std::string& target, const std::string& value);
    bool insert_after(const std::string& target, const std::string& value);

    //  удаление до/после 
    bool remove_before(const std::string& target);
    bool remove_after(const std::string& target);

    // разные способы чтения
    std::string get(size_t index) const;
    std::string front() const;
    std::string back() const;
    void print_reverse() const;
};

#endif