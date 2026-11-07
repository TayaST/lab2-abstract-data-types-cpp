#include <iostream>
#include <cstring> // для strlen

using namespace std;

// Функция проверки соответствия строки шаблону
bool is_match(const char* s, const char* p) {
    int n = strlen(s);
    int m = strlen(p);

    // Выделяем память под двумерный массив (n+1) x (m+1)
    bool** dp = new bool*[n + 1];
    for (int i = 0; i <= n; ++i) {
        dp[i] = new bool[m + 1];
        for (int j = 0; j <= m; ++j) {
            dp[i][j] = false; // Инициализация
        }
    }

    // Пустая строка соответствует пустому шаблону
    dp[0][0] = true;

    // Инициализация для шаблона, начинающегося со звездочек
    for (int j = 1; j <= m; ++j) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Заполняем таблицу
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (p[j - 1] == '*') {
                // Звездочка соответствует пустой последовательности или поглощает символ
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
                // Символы совпадают или это '?'
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    bool result = dp[n][m];

    // Очищаем память
    for (int i = 0; i <= n; ++i) {
        delete[] dp[i];
    }
    delete[] dp;

    return result;
}

void runWildcard() {
    cout << "\n=== Задание 4: Сопоставление с паттерном (Вариант 3) ===\n";
    
    // Используем статические массивы символов вместо string
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