#include "../h_files/Stack.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>

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

// Текстовая сериализация (записываем от вершины top_node ко дну)
void Stack::serialize_text(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        throw runtime_error("Cannot open file for writing");
    }

    out << size << "\n";
    Node* curr = top_node;
    while (curr != nullptr) {
        out << curr->data << "\n";
        curr = curr->next;
    }
}

// Текстовая десериализация (читаем в вектор и пушим с конца, чтобы сохранить вершину)
void Stack::deserialize_text(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        throw runtime_error("Cannot open file for reading");
    }

    clear();
    size_t count = 0;
    if (!(in >> count)) return;

    string dummy;
    getline(in, dummy); // Съедаем перенос строки после числа count

    vector<string> temp(count);
    for (size_t i = 0; i < count; ++i) {
        getline(in, temp[i]);
    }

    // Заталкиваем в обратном порядке, чтобы первый элемент файла снова стал вершиной
    for (size_t i = count; i > 0; --i) {
        push(temp[i - 1]);
    }
}

// Бинарная сериализация
void Stack::serialize_binary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out.is_open()) {
        throw runtime_error("Cannot open binary file for writing");
    }

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* curr = top_node;
    while (curr != nullptr) {
        size_t len = curr->data.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(curr->data.data(), len);
        curr = curr->next;
    }
}

// Бинарная десериализация
void Stack::deserialize_binary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        throw runtime_error("Cannot open binary file for reading");
    }

    clear();
    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    vector<string> temp(count);
    for (size_t i = 0; i < count; ++i) {
        size_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        temp[i].resize(len);
        in.read(&temp[i][0], len);
    }

    // Заталкиваем в обратном порядке
    for (size_t i = count; i > 0; --i) {
        push(temp[i - 1]);
    }
}