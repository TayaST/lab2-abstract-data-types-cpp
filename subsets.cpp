#include <iostream>
#include <string>
#include <cstring>
#include "sets.h" // Подключаем наше множество из Задания 2 (оно уже на базе ЛР1)

using namespace std;

// ==========================================
// ЗАДАНИЕ 3: ОПЕРАЦИИ НАД МНОЖЕСТВАМИ
// ==========================================

// 1. Объединение (A U B)
void set_union(const HashSet& a, const HashSet& b, HashSet& result) {
    set_init(result);
    // Добавляем все элементы из A
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = a.table[i].head; // <--- Используем FNode из ЛР1
        while (curr != nullptr) {
            set_add(result, curr->value.c_str());
            curr = curr->next;
        }
    }
    // Добавляем все элементы из B
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = b.table[i].head;
        while (curr != nullptr) {
            set_add(result, curr->value.c_str());
            curr = curr->next;
        }
    }
}

// 2. Пересечение (A ∩ B)
void set_intersection(const HashSet& a, const HashSet& b, HashSet& result) {
    set_init(result);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = a.table[i].head;
        while (curr != nullptr) {
            if (set_at(b, curr->value.c_str())) {
                set_add(result, curr->value.c_str());
            }
            curr = curr->next;
        }
    }
}

// 3. Разность (A \ B)
void set_difference(const HashSet& a, const HashSet& b, HashSet& result) {
    set_init(result);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = a.table[i].head;
        while (curr != nullptr) {
            if (!set_at(b, curr->value.c_str())) {
                set_add(result, curr->value.c_str());
            }
            curr = curr->next;
        }
    }
}

void runSubsets() {
    cout << "\n=== Задание 3: Операции над множествами (Вариант 3) ===\n";
    
    HashSet A, B, Result;
    set_init(A);
    set_init(B);

    cout << "Введите элементы множества A через пробел (Enter для конца):\n";
    string line;
    getline(cin, line);
    
    // Парсим строку на слова и добавляем в A
    char buffer[1024];
    int b_idx = 0;
    for (size_t i = 0; i <= line.length(); ++i) {
        if (i == line.length() || line[i] == ' ') {
            if (b_idx > 0) {
                buffer[b_idx] = '\0';
                set_add(A, buffer);
                b_idx = 0;
            }
        } else {
            buffer[b_idx++] = line[i];
        }
    }

    cout << "Введите элементы множества B через пробел (Enter для конца):\n";
    getline(cin, line);
    b_idx = 0;
    for (size_t i = 0; i <= line.length(); ++i) {
        if (i == line.length() || line[i] == ' ') {
            if (b_idx > 0) {
                buffer[b_idx] = '\0';
                set_add(B, buffer);
                b_idx = 0;
            }
        } else {
            buffer[b_idx++] = line[i];
        }
    }

    cout << "\nМножество A: "; set_print(A); cout << "\n";
    cout << "Множество B: "; set_print(B); cout << "\n";

    cout << "\nРезультаты:\n";
    
    set_union(A, B, Result);
    cout << "1. Объединение (A U B): "; set_print(Result); cout << "\n";

    set_intersection(A, B, Result);
    cout << "2. Пересечение (A ∩ B): "; set_print(Result); cout << "\n";

    set_difference(A, B, Result);
    cout << "3. Разность (A \\ B): "; set_print(Result); cout << "\n";
}