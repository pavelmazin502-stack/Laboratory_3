#include "../h_files/Queue.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Queue::Queue() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

Queue::~Queue() {
    clear();
}

Queue::Queue(const Queue& other) {
    head = nullptr;
    tail = nullptr;
    size = 0;

    Node* curr = other.head;
    while (curr != nullptr) {
        push(curr->data);
        curr = curr->next;
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this == &other) {
        return *this;
    }

    clear();

    Node* curr = other.head;
    while (curr != nullptr) {
        push(curr->data);
        curr = curr->next;
    }

    return *this;
}

void Queue::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

void Queue::push(const string& value) {
    Node* node = new Node(value);
    if (tail == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    size++;
}

void Queue::pop() {
    if (head == nullptr) {
        throw underflow_error("Queue is empty");
    }
    Node* temp = head;
    head = head->next;
    
    if (head == nullptr) {
        tail = nullptr;
    }
    
    delete temp;
    size--;
}

string Queue::peek() const {
    if (head == nullptr) {
        throw underflow_error("Queue is empty");
    }
    return head->data;
}

size_t Queue::get_size() const {
    return size;
}

void Queue::print() const {
    Node* curr = head;
    cout << "Front -> [";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << "]\n";
}