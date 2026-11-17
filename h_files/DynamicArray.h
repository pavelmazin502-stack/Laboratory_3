#ifndef DYNAMIC_ARRAY_h
#define DYNAMIC_ARRAY_h

#include <cstddef>
#include <string>

class DynamicArray{
private:
    std::string* data; // Указатель на данные иу
    size_t size; // размер нашего динамического массива
    size_t capacity; // Размер динамической памяти, выделенной под массив
    void resize(size_t new_capacity); // Функция добавления памяти
public:
    DynamicArray();
    explicit DynamicArray(size_t inital_capacity); // Уникальным способом создается объект
    ~DynamicArray();
    
    DynamicArray(const DynamicArray& other); // Конструктор копирования
    DynamicArray& operator=(const DynamicArray& other); // Оператор присваивания

    void push_back(const std::string& value);
    void insert_at(const std::string& value, size_t index); // Добавление по индексу

    const std::string& get(size_t index) const; // Получение элемента по индексу

    void remove_at(size_t index); // Удаление элемента по индексу
    void set(size_t index, const std::string& value);

    size_t get_size() const;

    void clear();

    void print() const;

    // Текстовая сериализация и десериализация
    void serialize_text(const std::string& filename) const;
    void deserialize_text(const std::string& filename);

    // Бинарная сериализация и десериализация
    void serialize_binary(const std::string& filename) const;
    void deserialize_binary(const std::string& filename);


};



#endif