#ifndef STACK_H
#define STACK_H

#include <string>
#include <cstddef>

class Stack {
private:
    struct Node {
        std::string data;
        Node* next;
        Node(const std::string& val) {
            data = val;
            next = nullptr;
        }
    };

    Node* top_node;
    size_t size;

public:
    Stack();
    ~Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);

    void clear();

    void push(const std::string& value);
    void pop();
    std::string peek() const;
    size_t get_size() const;
    void print() const;
};

#endif