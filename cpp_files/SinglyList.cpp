#include "../h_files/SinglyList.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

using namespace std;

void SinglyList::clear(){
    while (head != nullptr){
        Node* temp = head; // Сохранение указателя на текущий узел
        head = head -> next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

SinglyList::SinglyList(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}

SinglyList::~SinglyList(){
    clear();
}

// Конструктор копирования - копия списка other
SinglyList::SinglyList(const SinglyList& other){
    head = nullptr;
    tail = nullptr;
    size = 0;

    Node* curr = other.head;
    while (curr != nullptr){
        push_back(curr -> data);
        curr = curr -> next;
    }
}
// Оператор присваивания, перезаписываем текущий список копией списка other
SinglyList& SinglyList::operator=(const SinglyList& other){ // Конструктор копирования, для создания локальной копии other из какого либо класса
    if (this == &other){
        return *this; // Защита от самоприсваивания
    }

    clear();

    Node* curr = other.head;
    while (curr != nullptr){
        push_back(curr -> data);
        curr = curr -> next;
    }
    return *this; // Возврат текущего объекта
}

void SinglyList::push_front(const string& value){ // Добавление в голову
    Node* node = new Node(value);
    node -> next = head; // новый узел ссылается на бывшую голову
    head = node;

    if (tail == nullptr){
        tail = node;
    }
    size++;
}

// Добавление элемента в хвост
void SinglyList::push_back(const string& value){
    Node* node = new Node(value);

    if (tail == nullptr){ // Если список пуст, то элемент становится и головой и хвостом
        head = node;
        tail = node;
    } else {
        tail -> next = node;
        tail = node;
    }
    size++;
}
// Удаление первого элемента из головы
void SinglyList::pop_front(){
    // Для начала обработка всех исключений (например пустой список)
    if (head == nullptr){
        throw underflow_error("List is empty"); // Список пуст
    }

    Node* temp = head;
    head = head -> next; // Сдвиг головы на следующий узел

    if (head == nullptr){
        tail = nullptr;
    }
    delete temp;
    size--;
}
// Удаление последнего элемента из массива
void SinglyList::pop_back(){   
    if (head == nullptr){
        throw underflow_error("List is empty");
    }  
    if (head == tail){ // если голова и хвост указывают на один узел то
        delete head;
        head = nullptr;
        tail = nullptr;
        size = 0;
        return;
    }
    Node* curr = head;  // Ищем последний узел
    while (curr -> next != tail){
        curr = curr->next;
    }

    delete tail;
    tail = curr; // Перепривязка узлов (предпоследний узел, является новым хвостом)
    tail -> next = nullptr;
    size--;
}

void SinglyList::remove_by_value(const string& value) {
    if (head == nullptr) {            
        return;
    }

    if (head->data == value) {// Если искомое значение находится в голове
        pop_front(); 
        return;
    }

    Node* curr = head;// Ищем узел, стоящий ПЕРЕД удаляемым
    while (curr->next != nullptr && curr->next->data != value) {
        curr = curr->next;
    }

    if (curr->next == nullptr) { 
        return;
    }

    Node* to_delete = curr->next;// Запоминаем адрес удаляемого узла
    curr->next = to_delete->next;

    if (to_delete == tail) {         
        tail = curr;                  
    }

    delete to_delete;               
    size--;                           
}

// Поиск значения в списке за O(n)
bool SinglyList::find(const string& value) const {
    Node* curr = head;                
    while (curr != nullptr) {         
        if (curr->data == value) {
            return true;
        }
        curr = curr->next;// Переходим к следующему узлу
    }
    return false;                   
}

// Получение текущего количества элементов за O(1)
size_t SinglyList::get_size() const {
    return size;
}

// Вывод содержимого списка
void SinglyList::print() const {
    Node* curr = head;              
    cout << "[";
    while (curr != nullptr) {
        cout << curr->data;// Выводим поле data текущего узла
        if (curr->next != nullptr) {  
            cout << " -> ";
        }
        curr = curr->next;           
    }
    cout << "]\n";
}

void SinglyList::serialize_text(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Cannot open file for writing");

    out << size << "\n";
    Node* curr = head; // или Node* curr = head
    while (curr != nullptr) {
        out << curr->data << "\n";
        curr = curr->next;
    }
}

// Текстовая десериализация
void SinglyList::deserialize_text(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("Cannot open file for reading");

    clear();
    size_t count = 0;
    if (!(in >> count)) return;

    string dummy;
    getline(in, dummy); // Съедаем остаток строки после числа count

    for (size_t i = 0; i < count; ++i) {
        std::string val;
        std::getline(in, val);
        push_back(val); // или push_back(val) — в зависимости от названия метода в твоём классе
    }
}

// Бинарная сериализация
void SinglyList::serialize_binary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("Cannot open binary file for writing");

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* curr = head;
    while (curr != nullptr) {
        size_t len = curr->data.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(curr->data.data(), len);
        curr = curr->next;
    }
}

//Бинарная десериализация
void SinglyList::deserialize_binary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("Cannot open binary file for reading");

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