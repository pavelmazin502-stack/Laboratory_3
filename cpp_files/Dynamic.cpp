#include "../h_files/DynamicArray.h"
#include <iostream>
#include <stdexcept>

using namespace std; 

// Для начала напишем жизненный цикл и управление памятью
DynamicArray::DynamicArray(){
    data = nullptr;
    size = 0;
    capacity = 0;
}
// Деструктор
DynamicArray::~DynamicArray(){
    delete[] data;
}

void DynamicArray::resize(size_t new_capacity) { // Добавление динамической памяти
    string* new_data = new string[new_capacity];
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = move(data[i]); // Перекидывание данных из старой памяти в новую
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

// Конструктор копирования если вкратце то создает новый массив под хранение
DynamicArray::DynamicArray(const DynamicArray& other) {
    size = other.size;
    capacity = other.capacity;
    if (capacity > 0) {
        data = new string[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    } else {
        data = nullptr;
    }
}

// Оператор присваивания
DynamicArray& DynamicArray::operator=(const DynamicArray& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;

    size = other.size;
    capacity = other.capacity;
    if (capacity > 0) {
        data = new string[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    } else {
        data = nullptr;
    }

    return *this;
}

// Добавление элемента в конец
void DynamicArray::push_back(const string& value) {
    if (size == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[size] = value;
    ++size;
}

// Добавление элемента по индексу (порядок параметров как в твоем .h файле)
void DynamicArray::insert_at(const string& value, size_t index) {
    if (index > size) {
        throw out_of_range("Index out of range");
    }
    if (size == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    for (size_t i = size; i > index; --i) {
        data[i] = move(data[i - 1]);
    }
    data[index] = value;
    ++size;
}

// Получение элемента по индексу
const string& DynamicArray::get(size_t index) const {
    if (index >= size) {
        throw out_of_range("Index out of range");
    }
    return data[index];
}

// Удаление элемента по индексу
void DynamicArray::remove_at(size_t index) {
    if (index >= size) {
        throw out_of_range("Index out of range");
    }
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = move(data[i + 1]);
    }
    --size;
    data[size].clear();
}

// Замена элемента по индексу
void DynamicArray::set(size_t index, const string& value) {
    if (index >= size) {
        throw out_of_range("Index out of range");
    }
    data[index] = value;
}

// Длина массива
size_t DynamicArray::get_size() const {
    return size;
}

// Чтение / вывод всего массива
void DynamicArray::print() const {
    for (size_t i = 0; i < size; ++i) {
        cout << "[" << i << "]: " << data[i] << "\n";
    }
}