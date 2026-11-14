#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <cstddef>

class Queue {
private:
    struct Node {
        std::string data;
        Node* next;
        Node(const std::string& val) {
            data = val;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    size_t size;

public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);

    void clear();

    void push(const std::string& value);
    void pop();
    std::string peek() const;
    size_t get_size() const;
    void print() const;
};

#endif