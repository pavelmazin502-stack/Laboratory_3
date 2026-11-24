#include "../h_files/CompleteTree.h"
#include <iostream>
#include <queue> // Необходим для BFS (обхода в ширину)
#include <vector>
#include <stdexcept>
#include <fstream>

using namespace std;

// Конструктор: изначально дерево пустое
CompleteTree::CompleteTree() {
    root = nullptr;
}

// Приватный рекурсивный метод для полного удаления всех потомков узла
void CompleteTree::destroy_tree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy_tree(node->left);  // Рекурсивно удаляем левое поддерево
    destroy_tree(node->right); // Рекурсивно удаляем правое поддерево
    delete node;               // Удаляем сам узел
}

// Деструктор: вызывает рекурсивную очистку (RAII)
CompleteTree::~CompleteTree() {
    clear();
}

// Очистка дерева с обнулением корня
void CompleteTree::clear() {
    destroy_tree(root);
    root = nullptr;
}

// Приватный рекурсивный метод для глубокого копирования узлов
CompleteTree::TreeNode* CompleteTree::copy_tree(TreeNode* node) {
    if (node == nullptr) {
        return nullptr;
    }
    // Создаём новый узел с таким же значением
    TreeNode* new_node = new TreeNode(node->val);
    // Рекурсивно копируем левого и правого потомков
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    return new_node;
}

// Конструктор копирования
CompleteTree::CompleteTree(const CompleteTree& other) {
    root = copy_tree(other.root);
}

// Оператор присваивания (защита от утечек и самоприсваивания)
CompleteTree& CompleteTree::operator=(const CompleteTree& other) {
    if (this == &other) {
        return *this;
    }
    clear(); // Очищаем старое дерево
    root = copy_tree(other.root); // Копируем новое
    return *this;
}

// Вставка узла в BFS-порядке (гарантирует заполнение уровней строго слева направо)
void CompleteTree::insert(const string& val) {
    TreeNode* node = new TreeNode(val);
    
    if (root == nullptr) {
        root = node;
        return;
    }
    
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        
        // Если левого потомка нет — вставляем туда
        if (curr->left == nullptr) {
            curr->left = node;
            return;
        } else {
            q.push(curr->left); // Иначе добавляем левого в очередь
        }
        
        // Если правого потомка нет — вставляем туда
        if (curr->right == nullptr) {
            curr->right = node;
            return;
        } else {
            q.push(curr->right); // Иначе добавляем правого в очередь
        }
    }
}

// Поиск элемента через обход в ширину (BFS)
bool CompleteTree::find(const string& val) const {
    if (root == nullptr) {
        return false;
    }
    
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        
        if (curr->val == val) {
            return true;
        }
        
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
    return false;
}

// Алгоритм проверки свойства "Complete" (Почти полное дерево)
bool CompleteTree::is_complete() const {
    if (root == nullptr) {
        return true;
    }
    

    
    queue<TreeNode*> q;
    q.push(root);
    bool seen_null = false; // Флаг: встречали ли мы отсутствующего потомка
    
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        
        if (curr == nullptr) {
            seen_null = true; // Заметили "дырку" в дереве
        } else {
            // Если мы уже видели "дырку", но после неё идёт реальный узел — дерево не Complete
            if (seen_null) {
                return false;
            }
            // Добавляем потомков в очередь (даже если они nullptr)
            q.push(curr->left);
            q.push(curr->right);
        }
    }
    return true;
}

// Печать дерева по уровням (BFS обход)
void CompleteTree::print() const {
    if (root == nullptr) {
        cout << "Tree is empty\n";
        return;
    }
    
    queue<TreeNode*> q;
    q.push(root);
    
    cout << "[ ";
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        
        cout << curr->val << " ";
        
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
    cout << "]\n";
}

// Текстовая сериализация (BFS-обход по уровням)
void CompleteTree::serialize_text(const string& filename) const {
    ofstream out(filename);
    if (!out.is_open()) {
        throw runtime_error("Cannot open file for writing");
    }

    vector<string> vals;
    if (root != nullptr) {
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            vals.push_back(curr->val);
            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
    }

    out << vals.size() << "\n";
    for (const auto& v : vals) {
        out << v << "\n";
    }
}

// Текстовая десериализация
void CompleteTree::deserialize_text(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        throw runtime_error("Cannot open file for reading");
    }

    clear();
    size_t count = 0;
    if (!(in >> count)) return;

    string dummy;
    getline(in, dummy); // Съедаем перенос строки после числа count

    for (size_t i = 0; i < count; ++i) {
        string v;
        getline(in, v);
        insert(v); // Вставка в BFS-порядке точно восстановит исходное дерево
    }
}

// Бинарная сериализация
void CompleteTree::serialize_binary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out.is_open()) {
        throw runtime_error("Cannot open binary file for writing");
    }

    vector<string> vals;
    if (root != nullptr) {
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            vals.push_back(curr->val);
            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
    }

    size_t count = vals.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& v : vals) {
        size_t len = v.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(v.data(), len);
    }
}

// Бинарная десериализация
void CompleteTree::deserialize_binary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        throw runtime_error("Cannot open binary file for reading");
    }

    clear();
    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        size_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));

        string v(len, '\0');
        in.read(&v[0], len);
        insert(v);
    }
}