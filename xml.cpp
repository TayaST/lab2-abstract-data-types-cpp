#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "../stack.h" // Подключаем стек из ЛР1

using namespace std;

// "Тихая" обертка для добавления в стек (чтобы не печаталось "OK")
void silent_push(Stack& s, const string& value) {
    SNode* newNode = new SNode;
    newNode->value = value;
    newNode->next = s.top;
    s.top = newNode;
    s.count++;
}

// "Тихая" обертка для извлечения из стека
bool silent_pop(Stack& s, string& outValue) {
    if (s.top == nullptr) return false;
    SNode* temp = s.top;
    outValue = temp->value;
    s.top = s.top->next;
    delete temp;
    s.count--;
    return true;
}

// Функция проверки корректности XML-строки
bool is_valid_xml(const string& str) {
    Stack s;
    stack_init(s); // Инициализация из ЛР1
    
    size_t i = 0;
    while (i < str.length()) {
        if (str[i] == '<') {
            size_t end_pos = str.find('>', i);
            if (end_pos == string::npos) {
                stack_free(s);
                return false; // Нет закрывающей скобки >
            }
            
            string tag_content = str.substr(i + 1, end_pos - i - 1);
            
            // Удаляем пробелы внутри тега (на случай, если они там есть)
            string clean_tag = "";
            for (char c : tag_content) {
                if (c != ' ') clean_tag += c;
            }
            
            if (clean_tag.empty()) {
                stack_free(s);
                return false; // Пустой тег <>
            }

            if (clean_tag[0] == '/') {
                // Закрывающий тег
                string tag_name = clean_tag.substr(1);
                string top_tag;
                if (!silent_pop(s, top_tag) || top_tag != tag_name) {
                    stack_free(s);
                    return false; // Несовпадение тегов или стек пуст
                }
            } else {
                // Открывающий тег
                silent_push(s, clean_tag);
            }
            i = end_pos + 1;
        } else if (str[i] == '>') {
            stack_free(s);
            return false; // Лишняя закрывающая скобка >
        } else {
            i++; // Пропускаем буквы и пробелы
        }
    }
    
    bool is_empty = (s.top == nullptr);
    stack_free(s);
    return is_empty;
}

// Основная функция запуска задания 1
void runXML() {
    cout << "\n=== Задание 1: Восстановление XML-строки (Вариант 7) ===\n";
    cout << "Введите поврежденную строку (или 'exit' для выхода):\n";
    
    string input;
    getline(cin, input);
    
    if (input == "exit") return;

    // Алфавит для замены: строчные буквы, <, >, /
    string alphabet = "abcdefghijklmnopqrstuvwxyz<>/";
    
    // Перебираем каждый символ в строке
    for (size_t i = 0; i < input.length(); ++i) {
        char original_char = input[i];
        
        // Пробуем заменить на каждый символ из алфавита
        for (char c : alphabet) {
            if (c == original_char) continue; // Если символ тот же, пропускаем
            
            input[i] = c;
            if (is_valid_xml(input)) {
                cout << "Исправленная строка: " << input << "\n";
                return;
            }
        }
        input[i] = original_char; // Возвращаем как было, если не подошло
    }
    
    cout << "Incorrect\n";
}