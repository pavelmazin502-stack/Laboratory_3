#include "../h_files/DoublyList.h"
#include <iostream>
#include <stdexcept>

using namespace std;

DoublyList::DoublyList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

DoublyList::~DoublyList() {
    clear();
}

DoublyList::DoublyList(const DoublyList& other) {
    head = nullptr;
    tail = nullptr;
    size = 0;

    Node* curr = other.head;
    while (curr != nullptr) {
        push_back(curr->data);
        curr = curr->next;
    }
}

DoublyList& DoublyList::operator=(const DoublyList& other) {
    if (this == &other) {
        return *this;
    }

    clear();

    Node* curr = other.head;
    while (curr != nullptr) {
        push_back(curr->data);
        curr = curr->next;
    }

    return *this;
}

void DoublyList::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

void DoublyList::push_front(const string& value) {
    Node* node = new Node(value);
    
    if (head != nullptr) {
        head->prev = node;
    } else {
        tail = node;
    }
    
    node->next = head;
    head = node;
    size++;
}

void DoublyList::push_back(const string& value) {
    Node* node = new Node(value);
    
    if (tail != nullptr) {
        tail->next = node;
    } else {
        head = node;
    }
    
    node->prev = tail;
    tail = node;
    size++;
}

void DoublyList::pop_front() {
    if (head == nullptr) {
        throw underflow_error("List is empty");
    }

    Node* temp = head;
    head = head->next;
    
    if (head != nullptr) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    
    delete temp;
    size--;
}

void DoublyList::pop_back() {
    if (tail == nullptr) {
        throw underflow_error("List is empty");
    }

    Node* temp = tail;
    tail = tail->prev;
    
    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    
    delete temp;
    size--;
}

void DoublyList::remove_by_value(const string& value) {
    Node* curr = head;
    while (curr != nullptr && curr->data != value) {
        curr = curr->next;
    }
    
    if (curr == nullptr) {
        return;
    }

    if (curr == head) {
        pop_front();
    } else if (curr == tail) {
        pop_back();
    } else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        size--;
    }
}

bool DoublyList::find(const string& value) const {
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->data == value) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

size_t DoublyList::get_size() const {
    return size;
}

void DoublyList::print() const {
    Node* curr = head;
    cout << "[";
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " <-> ";
        }
        curr = curr->next;
    }
    cout << "]\n";
}