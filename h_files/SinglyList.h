#ifndef SINGLY_LIST_H
#define SINGLY_LIST_H

#include <cstddef>
#include <string>

class SinglyList {
private:
    struct Node {
        std::string data;
        Node* next;
    };

    Node* head;
    Node* tail;
    size_t size;

public:
    SinglyList();
    ~SinglyList();
    SinglyList(const SinglyList& other);
    SinglyList& operator=(const SinglyList& other);

    void push_front(const std::string& value);
    void push_back(const std::string& value);
    bool insert_before(const std::string& target, const std::string& value);
    bool insert_after(const std::string& target, const std::string& value);

    void pop_front();
    void pop_back();
    bool remove_before(const std::string& target);
    bool remove_after(const std::string& target);
    bool remove_by_value(const std::string& value);

    bool find(const std::string& value) const;
    std::string front() const;
    size_t get_size() const;
    bool empty() const;
    void clear();
    void print() const;

    void serialize_text(const std::string& filename) const;
    void deserialize_text(const std::string& filename);
    void serialize_binary(const std::string& filename) const;
    void deserialize_binary(const std::string& filename);
};

#endif