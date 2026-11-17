#ifndef COMPLETE_TREE_H
#define COMPLETE_TREE_H

#include <string>

class CompleteTree {
private:
    // Структура узла бинарного дерева
    struct TreeNode {
        std::string val;   // Значение узла
        TreeNode* left;    // Указатель на левого потомка
        TreeNode* right;   // Указатель на правого потомка
        
        // Конструктор узла
        TreeNode(const std::string& v) {
            val = v;
            left = nullptr;
            right = nullptr;
        }
    };

    TreeNode* root; // Корень дерева

    // Приватные вспомогательные методы 
    void destroy_tree(TreeNode* node);
    TreeNode* copy_tree(TreeNode* node);

public:
    CompleteTree();                                    // Конструктор
    ~CompleteTree();                                   // Деструктор
    CompleteTree(const CompleteTree& other);           // Конструктор копирования
    CompleteTree& operator=(const CompleteTree& other);// Оператор присваивания

    void clear();

    void insert(const std::string& val);               // Вставка (TINSERT) с сохранением Complete
    bool find(const std::string& val) const;           // Поиск значения (TFIND)
    bool is_complete() const;                          // Проверка свойства Complete (TISCOMPLETE)
    void print() const;                                // Печать дерева по уровням (BFS)

    // Текстовая сериализация и десериализация
    void serialize_text(const std::string& filename) const;
    void deserialize_text(const std::string& filename);

    // Бинарная сериализация и десериализация
    void serialize_binary(const std::string& filename) const;
    void deserialize_binary(const std::string& filename);
};

#endif // COMPLETE_TREE_H