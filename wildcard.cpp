#include <iostream>
#include <cstring>
#include "../array.h"

using namespace std;

void silent_array_push(MyArray& a, const string& value) {
    // Если массив заполнен, увеличиваем емкость
    if (a.length == a.capacity) {
        a.capacity = (a.capacity == 0) ? 1 : a.capacity * 2;
        string* newData = new string[a.capacity];
        for (int i = 0; i < a.length; ++i) {
            newData[i] = a.data[i];
        }
        delete[] a.data;
        a.data = newData;
    }
    a.data[a.length] = value;
    a.length++;
}

void silent_array_set(MyArray& a, int index, const string& value) {
    if (index >= 0 && index < a.length) {
        a.data[index] = value;
    }
}

bool is_match(const char* s, const char* p) {
    int n = strlen(s); 
    int m = strlen(p);

    MyArray dp;
    array_init(dp); 

    // Заполняем таблицу нулями (символами '0')
    for (int i = 0; i <= n; ++i) {
        string zero_row(m + 1, '0');
        silent_array_push(dp, zero_row); 
    }

    // dp[0][0] = true.
    string row0 = dp.data[0];
    row0[0] = '1';
    silent_array_set(dp, 0, row0); 

    // Инициализация для шаблона со звездочками
    for (int j = 1; j <= m; ++j) {
        if (p[j - 1] == '*') {
            if (dp.data[0][j - 1] == '1') {
                string row = dp.data[0];
                row[j] = '1';
                silent_array_set(dp, 0, row);
            }
        }
    }

    // Заполняем таблицу
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            string current_row = dp.data[i];
            bool current_val = false;

            if (p[j - 1] == '*') {
                if (dp.data[i][j - 1] == '1' || dp.data[i - 1][j] == '1') current_val = true;
            } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                if (dp.data[i - 1][j - 1] == '1') current_val = true;
            }

            current_row[j] = current_val ? '1' : '0';
            silent_array_set(dp, i, current_row); // <--- И ТУТ ТИХОЕ
        }
    }

    bool result = (dp.data[n][m] == '1');
    array_free(dp); // Эта функция тоже тихая

    return result;
}

void runWildcard() {
    cout << "\n=== Задание 4: Сопоставление с паттерном (Вариант 3) ===\n";
    char sequence[1024];
    char pattern[1024];
    cout << "Введите последовательность символов: ";
    cin.getline(sequence, 1024);
    cout << "Введите шаблон (можно использовать '?' и '*'): ";
    cin.getline(pattern, 1024);

    if (is_match(sequence, pattern)) {
        cout << "\nРезультат: Соответствует\n";
    } else {
        cout << "\nРезультат: Не соответствует\n";
    }
}