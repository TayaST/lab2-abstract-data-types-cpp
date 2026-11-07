#include <iostream>
#include <cmath> // для std::abs
#include "../tree.h" // Подключаем дерево из ЛР1 (проверь путь, если нужно)

using namespace std;

// Вспомогательная функция для вычисления высоты поддерева
int get_height(TNode* node) {
    if (node == nullptr) return 0;
    int left_h = get_height(node->left);
    int right_h = get_height(node->right);
    return 1 + max(left_h, right_h);
}

// Рекурсивная проверка на АВЛ-сбалансированность
bool is_avl_balanced(TNode* node) {
    if (node == nullptr) return true;

    // Получаем высоты левого и правого поддеревьев
    int left_h = get_height(node->left);
    int right_h = get_height(node->right);

    // Если разница больше 1, то дерево не сбалансировано
    if (abs(left_h - right_h) > 1) return false;

    // Рекурсивно проверяем левое и правое поддеревья
    return is_avl_balanced(node->left) && is_avl_balanced(node->right);
}

// "Тихая" вставка, чтобы не засорять консоль выводом из ЛР1
void silent_tree_insert(CompleteBinaryTree& t, int value) {
    TNode* newNode = new TNode(value);
    
    if (t.root == nullptr) {
        t.root = newNode;
        t.count = 1;
        return;
    }

    TNode* curr = t.root;
    while (true) {
        if (value < curr->value) {
            if (curr->left == nullptr) {
                curr->left = newNode;
                break;
            }
            curr = curr->left;
        } else if (value > curr->value) {
            if (curr->right == nullptr) {
                curr->right = newNode;
                break;
            }
            curr = curr->right;
        } else {
            // Если элемент уже есть, ничего не делаем (дерево поиска)
            delete newNode;
            return;
        }
    }
    t.count++;
}

void runTree() {
    cout << "\n=== Задание 5: АВЛ-сбалансированность (Вариант 3) ===\n";
    cout << "Введите последовательность целых чисел (0 для завершения):\n";

    CompleteBinaryTree t;
    t.root = nullptr;
    t.count = 0;

    int val;
    while (cin >> val && val != 0) {
        silent_tree_insert(t, val);
    }

    if (is_avl_balanced(t.root)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}