#include "../h_files/CompleteTree.h"
#include <iostream>
#include <queue> // Необходим для BFS (обхода в ширину)

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