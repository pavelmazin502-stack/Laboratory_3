#include "../h_files/Stack.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Stack::Stack() {
    top_node = nullptr;
    size = 0;
}

Stack::~Stack() {
    clear();
}

Stack::Stack(const Stack& other) {
    top_node = nullptr;
    size = 0;

    if (other.top_node == nullptr) {
        return;
    }

    top_node = new Node(other.top_node->data);
    Node* current_new = top_node;
    Node* current_old = other.top_node->next;

    while (current_old != nullptr) {
        current_new->next = new Node(current_old->data);
        current_new = current_new->next;
        current_old = current_old->next;
    }
    size = other.size;
}

Stack& Stack::operator=(const Stack& other) {
    if (this == &other) {
        return *this;
    }

    clear();

    if (other.top_node != nullptr) {
        top_node = new Node(other.top_node->data);
        Node* current_new = top_node;
        Node* current_old = other.top_node->next;

        while (current_old != nullptr) {
            current_new->next = new Node(current_old->data);
            current_new = current_new->next;
            current_old = current_old->next;
        }
        size = other.size;
    }

    return *this;
}

void Stack::clear() {
    while (top_node != nullptr) {
        Node* temp = top_node;
        top_node = top_node->next;
        delete temp;
    }
    size = 0;
}

void Stack::push(const string& value) {
    Node* node = new Node(value);
    node->next = top_node;
    top_node = node;
    size++;
}

void Stack::pop() {
    if (top_node == nullptr) {
        throw underflow_error("Stack is empty");
    }
    Node* temp = top_node;
    top_node = top_node->next;
    delete temp;
    size--;
}

string Stack::peek() const {
    if (top_node == nullptr) {
        throw underflow_error("Stack is empty");
    }
    return top_node->data;
}

size_t Stack::get_size() const {
    return size;
}

void Stack::print() const {
    Node* curr = top_node;
    cout << "Top -> [";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << "]\n";
}