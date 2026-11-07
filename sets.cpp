#include "sets.h"
#include <fstream>
#include <cstring>

using namespace std;

// Инициализация (используем flist_init из ЛР1)
void set_init(HashSet& s) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        flist_init(s.table[i]);
    }
}

// Хеш-функция
unsigned long hash_func(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

// Добавление элемента (O(1) в среднем)
void set_add(HashSet& s, const char* value) {
    unsigned long idx = hash_func(value);
    FNode* curr = s.table[idx].head;
    
    // Проверка на дубликат
    while (curr != nullptr) {
        if (strcmp(curr->value.c_str(), value) == 0) return;
        curr = curr->next;
    }
    
    // Вставка вручную (чтобы не было "OK" от ЛР1)
    FNode* newNode = new FNode;
    newNode->value = value; // string автоматически конвертирует char*
    newNode->next = s.table[idx].head;
    s.table[idx].head = newNode;
    if (s.table[idx].tail == nullptr) s.table[idx].tail = newNode;
    s.table[idx].count++;
}

// Удаление элемента
void set_del(HashSet& s, const char* value) {
    unsigned long idx = hash_func(value);
    FNode* curr = s.table[idx].head;
    FNode* prev = nullptr;
    
    while (curr != nullptr) {
        if (strcmp(curr->value.c_str(), value) == 0) {
            if (prev == nullptr) {
                s.table[idx].head = curr->next;
            } else {
                prev->next = curr->next;
            }
            if (curr == s.table[idx].tail) {
                s.table[idx].tail = prev;
            }
            delete curr;
            s.table[idx].count--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// Проверка наличия
bool set_at(const HashSet& s, const char* value) {
    unsigned long idx = hash_func(value);
    FNode* curr = s.table[idx].head;
    while (curr != nullptr) {
        if (strcmp(curr->value.c_str(), value) == 0) return true;
        curr = curr->next;
    }
    return false;
}

// Вывод множества
void set_print(const HashSet& s) {
    cout << "{";
    bool first = true;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = s.table[i].head;
        while (curr != nullptr) {
            if (!first) cout << ", ";
            cout << curr->value;
            first = false;
            curr = curr->next;
        }
    }
    cout << "}";
}

// Обработка одного запроса
void process_query(HashSet& s, const string& query) {
    char command[256] = {0};
    char arg[1024] = {0};
    
    // Ручной парсер (без stringstream)
    int i = 0;
    int len = query.length();
    while (i < len && query[i] == ' ') i++;
    int c_idx = 0;
    while (i < len && query[i] != ' ') command[c_idx++] = query[i++];
    while (i < len && query[i] == ' ') i++;
    int a_idx = 0;
    while (i < len && query[i] != ' ' && query[i] != '\n' && query[i] != '\r') arg[a_idx++] = query[i++];
    
    if (strcmp(command, "SETADD") == 0) {
        set_add(s, arg);
        cout << "OK\n";
    } else if (strcmp(command, "SETDEL") == 0) {
        set_del(s, arg);
        cout << "OK\n";
    } else if (strcmp(command, "SET_AT") == 0) {
        if (set_at(s, arg)) cout << "YES\n";
        else cout << "NO\n";
    }
}

// Точка входа для Задания 2
void runSets(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "\n=== Задание 2: АТД Множество (на базе списков ЛР1) ===\n";
        cout << "Вводите: SETADD x, SETDEL x, SET_AT x. Выход: exit\n";
        HashSet s;
        set_init(s);
        string line;
        while (true) {
            cout << "> ";
            getline(cin, line);
            if (line == "exit") break;
            if (!line.empty()) process_query(s, line);
        }
        return;
    }

    // Парсинг аргументов командной строки (как было)
    string filePath = "", queryStr = "";
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filePath = argv[++i];
        else if ((arg == "--query" || arg == "--") && i + 1 < argc) {
            if (arg == "--" && i + 1 < argc && string(argv[i+1]) == "query") ++i;
            if (i + 1 < argc) queryStr = argv[++i];
        }
    }

    if (filePath.empty() || queryStr.empty()) return;

    HashSet s;
    set_init(s);
    ifstream file(filePath.c_str());
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (!line.empty()) process_query(s, line);
    }
    process_query(s, queryStr);
}