#include "../h_files/DoublyList.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

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

void DoublyList::serialize_text(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open file for writing");
    }

    out << size << "\n";
    Node* curr = head;
    while (curr != nullptr) {
        out << curr->data << "\n";
        curr = curr->next;
    }
}

void DoublyList::deserialize_text(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open file for reading");
    }

    clear();
    size_t count = 0;
    if (!(in >> count)) return;

    std::string dummy;
    std::getline(in, dummy); // Считываем перенос строки после числа count

    for (size_t i = 0; i < count; ++i) {
        std::string val;
        std::getline(in, val);
        push_back(val); // push_back сам настроит указатели prev и next
    }
}

void DoublyList::serialize_binary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open binary file for writing");
    }

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* curr = head;
    while (curr != nullptr) {
        size_t len = curr->data.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(curr->data.data(), len);
        curr = curr->next;
    }
}

// 4. Бинарная десериализация
void DoublyList::deserialize_binary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open binary file for reading");
    }

    clear();
    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        size_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));

        std::string val(len, '\0');
        in.read(&val[0], len);
        push_back(val);
    }
}

// === ДОБАВЛЕНО ===
bool DoublyList::insert_before(const std::string& target, const std::string& value) {
    Node* curr = head;
    while (curr != nullptr && curr->data != target) {
        curr = curr->next;
    }
    if (curr == nullptr) {
        return false;
    }
    if (curr == head) {
        push_front(value);
        return true;
    }
    Node* node = new Node(value);
    node->prev = curr->prev;
    node->next = curr;
    curr->prev->next = node;
    curr->prev = node;
    size++;
    return true;
}

bool DoublyList::insert_after(const std::string& target, const std::string& value) {
    Node* curr = head;
    while (curr != nullptr && curr->data != target) {
        curr = curr->next;
    }
    if (curr == nullptr) {
        return false;
    }
    if (curr == tail) {
        push_back(value);
        return true;
    }
    Node* node = new Node(value);
    node->prev = curr;
    node->next = curr->next;
    curr->next->prev = node;
    curr->next = node;
    size++;
    return true;
}

bool DoublyList::remove_before(const std::string& target) {
    Node* curr = head;
    while (curr != nullptr && curr->data != target) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->prev == nullptr) {
        return false;
    }
    Node* to_delete = curr->prev;
    if (to_delete == head) {
        pop_front();
        return true;
    }
    to_delete->prev->next = curr;
    curr->prev = to_delete->prev;
    delete to_delete;
    size--;
    return true;
}

bool DoublyList::remove_after(const std::string& target) {
    Node* curr = head;
    while (curr != nullptr && curr->data != target) {
        curr = curr->next;
    }
    if (curr == nullptr || curr->next == nullptr) {
        return false;
    }
    Node* to_delete = curr->next;
    if (to_delete == tail) {
        pop_back();
        return true;
    }
    curr->next = to_delete->next;
    to_delete->next->prev = curr;
    delete to_delete;
    size--;
    return true;
}

std::string DoublyList::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    Node* curr = head;
    for (size_t i = 0; i < index; ++i) {
        curr = curr->next;
    }
    return curr->data;
}

std::string DoublyList::front() const {
    if (head == nullptr) {
        throw std::underflow_error("List is empty");
    }
    return head->data;
}

std::string DoublyList::back() const {
    if (tail == nullptr) {
        throw std::underflow_error("List is empty");
    }
    return tail->data;
}

void DoublyList::print_reverse() const {
    Node* curr = tail;
    std::cout << "[";
    while (curr != nullptr) {
        std::cout << curr->data;
        if (curr->prev != nullptr) {
            std::cout << " <-> ";
        }
        curr = curr->prev;
    }
    std::cout << "]\n";
}