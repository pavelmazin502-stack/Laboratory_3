#include "../h_files/DynamicArray.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

using namespace std; 

void DynamicArray::clear(){
    size = 0;
}
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

// Сохранение в текстовый файл
void DynamicArray::serialize_text(const std::string& filename) const{
    std::ofstream out(filename);

    if (!out.is_open()){
        throw runtime_error("Не удалось открыть файл для записи");
    }
    out << size << "\n";
    for (size_t i = 0; i < size; ++i){
        out << data[i] << "\n";
    }
}

void DynamicArray::deserialize_text(const std::string& filename) {
    std::ifstream in(filename); 
    if (!in.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения");
    }
    
    clear(); //Очистка данных
    size_t count = 0;
    if (!(in >> count)) return; // Считываем количество элементов; если файл пуст — выходим
    
    std::string dummy;
    std::getline(in, dummy); 
    
    for (size_t i = 0; i < count; ++i) {
        std::string val;
        std::getline(in, val); // Читаем строку целиком (включая возможные пробелы)
        push_back(val);        // Добавляем элемент в массив
    }
}

//Сохранение в бинарный файл
void DynamicArray::serialize_binary(const std::string& filename) const {
    // Флаг std::ios::binary отключает текстовое преобразование символов переноса строки
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось открыть бинарный файл для записи");
    }
    
    // Записываем переменную size как набор сырых байтов (8 байт для size_t)
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    for (size_t i = 0; i < size; ++i) {
        size_t len = data[i].size(); // Узнаём длину текущей строки в байтах
        // Сначала записываем саму длину строки
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        // Затем записываем ровно len байт символов из памяти строки
        out.write(data[i].data(), len);
    }
}

// 4. Загрузка из бинарного файла
void DynamicArray::deserialize_binary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Не удалось открыть бинарный файл для чтения");
    }
    
    clear(); // Очищаем старый массив
    size_t count = 0;
    // Читаем ровно sizeof(count) байт и кладём их в переменную count
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    for (size_t i = 0; i < count; ++i) {
        size_t len = 0;
        // Считываем длину следующей строки
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        
        // Создаём строку нужного размера len, заполненную нулями, и читаем прямо в её буфер
        std::string val(len, '\0');
        in.read(&val[0], len);
        
        push_back(val);
    }
}