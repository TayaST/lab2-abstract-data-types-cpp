#include <iostream>
#include <string>

using namespace std;

// Прототипы функций для каждого задания
void runXML();                     // Задание 1
void runSets(int argc, char* argv[]); // Задание 2
void runSubsets();                 // Задание 3
void runWildcard();                // Задание 4
void runTree();                    // Задание 5
void runHash();                    // Задание 6
void runCache();                   // Задание 7

int main(int argc, char* argv[]) {
    // Если программа запущена с аргументами командной строки (например, --file),
    // сразу передаем управление во 2-е задание (АТД Множество)
    if (argc > 1) {
        runSets(argc, argv);
        return 0;
    }

    int choice;
    while (true) {
        cout << "\n============================================\n";
        cout << "       Лабораторная работа №2               \n";
        cout << "============================================\n";
        cout << "1. Восстановление XML-строки\n";
        cout << "2. АТД Множество\n";
        cout << "3. Операции с множеством\n";
        cout << "4. Сопоставление с паттерном\n";
        cout << "5. АВЛ-сбалансированность дерева\n";
        cout << "6. Хеш-таблица и Родословная\n";
        cout << "7. LRU-кэш\n";
        cout << "0. Выход\n";
        cout << "============================================\n";
        cout << "Выбор: ";
        
        if (!(cin >> choice)) {
            // Защита (если ввели букву вместо цифры)
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // Очищаем буфер после ввода цифры

        if (choice == 0) {
            cout << "Выход из программы...\n";
            break;
        }
        else if (choice == 1) runXML();
        else if (choice == 2) runSets(argc, argv);
        else if (choice == 3) runSubsets();
        else if (choice == 4) runWildcard();
        else if (choice == 5) runTree();
        else if (choice == 6) runHash();
        else if (choice == 7) runCache();
        else cout << "Неверный выбор. Попробуйте снова.\n";
    }
    return 0;
}