#ifndef SINGLY_LIST_H
#define SINGLY_LIST_H

#include <string>
#include <cstddef>

class SinglyList{
private:
    struct Node{
        std::string data;
        Node* next;
        Node(const std::string& val){
            data = val;
            next = nullptr;
        }
    };
    Node* head;
    Node* tail;
    size_t size;


public:
    SinglyList();
    ~SinglyList();

    SinglyList(const SinglyList& other);
    SinglyList& operator=(const SinglyList& other);

    void clear();

    void push_front(const std::string& value);
    void push_back(const std::string& value);
    void pop_front();
    void pop_back();
    void remove_by_value(const std::string& value);

    bool find(const std::string& value) const;
    size_t get_size() const;
    void print() const;
};

#endif