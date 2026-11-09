#include "sets.h"
#include <fstream>
#include <cstring>

using namespace std;

void set_init(HashSet& s) { 
    for (int i = 0; i < TABLE_SIZE; ++i) flist_init(s.table[i]); 
}

unsigned long hash_func(const char* str) {
    unsigned long hash = 5381; 
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

// Теперь возвращает true, если добавили, и false, если такой элемент уже есть
bool set_add(HashSet& s, const char* value) {
    unsigned long idx = hash_func(value);
    FNode* curr = s.table[idx].head;
    
    // Проверка на дубликат
    while (curr != nullptr) { 
        if (curr->value == value) return false; 
        curr = curr->next; 
    }
    
    // Если элемента нет, добавляем
    FNode* newNode = new FNode; 
    newNode->value = value; 
    newNode->next = s.table[idx].head;
    s.table[idx].head = newNode;
    if (s.table[idx].tail == nullptr) s.table[idx].tail = newNode;
    s.table[idx].count++;
    
    return true;
}

void set_del(HashSet& s, const char* value) {
    unsigned long idx = hash_func(value);
    FNode* curr = s.table[idx].head; 
    FNode* prev = nullptr;
    while (curr != nullptr) {
        if (curr->value == value) {
            if (prev == nullptr) s.table[idx].head = curr->next;
            else prev->next = curr->next;
            if (curr == s.table[idx].tail) s.table[idx].tail = prev;
            delete curr; 
            s.table[idx].count--; 
            return;
        }
        prev = curr; 
        curr = curr->next;
    }
}

bool set_at(const HashSet& s, const char* value) {
    unsigned long idx = hash_func(value); 
    FNode* curr = s.table[idx].head;
    while (curr != nullptr) { if (curr->value == value) return true; curr = curr->next; }
    return false;
}

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

// Сохранение в файл
void save_to_file(const HashSet& s, const string& filename) {
    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "ERROR: cannot open file for writing\n";
        return;
    }
    for (int i = 0; i < TABLE_SIZE; ++i) {
        FNode* curr = s.table[i].head;
        while (curr != nullptr) {
            file << "SETADD " << curr->value << "\n";
            curr = curr->next;
        }
    }
    file.close();
    cout << "OK: saved to " << filename << "\n";
}

// Обработка одной команды
void process_query(HashSet& s, const string& query, bool& should_exit) {
    char command[256] = {0}; 
    char arg[1024] = {0};
    
    int i = 0; 
    int len = query.length();
    while (i < len && query[i] == ' ') i++;
    int c_idx = 0; 
    while (i < len && query[i] != ' ') command[c_idx++] = query[i++];
    while (i < len && query[i] == ' ') i++;
    int a_idx = 0; 
    while (i < len && query[i] != ' ' && query[i] != '\n' && query[i] != '\r') arg[a_idx++] = query[i++];
    
    if (strcmp(command, "SETADD") == 0) { 
        if (set_add(s, arg)) {
            cout << "OK\n"; 
        } else {
            cout << "ERROR\n";
        }
    } else if (strcmp(command, "SETDEL") == 0) { 
        set_del(s, arg); 
        cout << "OK\n"; 
    } else if (strcmp(command, "SET_AT") == 0) { 
        if (set_at(s, arg)) cout << "YES\n"; 
        else cout << "NO\n"; 
    } else if (strcmp(command, "SAVE") == 0) {
        string path = (a_idx > 0) ? string(arg) : "database.txt";
        save_to_file(s, path);
    } else if (strcmp(command, "EXIT") == 0 || strcmp(command, "exit") == 0) {
        should_exit = true;
    } else {
        cout << "ERROR\n";
    }
}

void runSets(int argc, char* argv[]) {
    // Интерактивный режим
    if (argc <= 1) {
        cout << "\n=== Задание 2: АТД Множество (Интерактив) ===\n";
        cout << "Команды: SETADD x, SETDEL x, SET_AT x\n";
        cout << "Для сохранения: SAVE [путь_к_файлу]\n";
        cout << "Для выхода: exit\n";
        
        HashSet s; 
        set_init(s); 
        string line;
        bool should_exit = false;
        
        while (!should_exit) {
            cout << "> "; 
            getline(cin, line);
            if (!line.empty()) process_query(s, line, should_exit);
        }
        return;
    }

    // Режим аргументов командной строки
    string filePath = "";
    string queryStr = "";

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filePath = argv[++i];
        } else if ((arg == "--query" || arg == "--") && i + 1 < argc) {
            if (arg == "--" && i + 1 < argc && string(argv[i+1]) == "query") ++i;
            if (i + 1 < argc) queryStr = argv[++i];
        }
    }

    if (filePath.empty() || queryStr.empty()) {
        cout << "Ошибка: неверные аргументы.\n";
        return;
    }

    HashSet s;
    set_init(s);
    ifstream file(filePath.c_str());
    if (!file.is_open()) return;

    string line;
    bool ignore_exit = false;
    while (getline(file, line)) {
        if (!line.empty()) process_query(s, line, ignore_exit);
    }
    file.close();

    process_query(s, queryStr, ignore_exit);
}