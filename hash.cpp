#include <iostream>
#include <cstring>
#include <chrono> // Для замера времени

using namespace std;

const int TABLE_SIZE = 20011; 

// ==========================================
// 6.1. ХЕШ-ТАБЛИЦА С ЛИНЕЙНЫМ ПРОБИРОВАНИЕМ
// ==========================================
struct LinearHash {
    char keys[TABLE_SIZE][256];
    bool is_occupied[TABLE_SIZE];
    int collisions; // Счетчик коллизий

    void init() {
        for (int i = 0; i < TABLE_SIZE; ++i) is_occupied[i] = false;
        collisions = 0;
    }

    int hash(const char* str) {
        unsigned long h = 5381;
        int c;
        while ((c = *str++)) h = ((h << 5) + h) + c;
        return h % TABLE_SIZE;
    }

    int find_or_insert(const char* key) {
        int idx = hash(key);
        bool collided = false;
        while (is_occupied[idx]) {
            if (strcmp(keys[idx], key) == 0) return idx;
            idx = (idx + 1) % TABLE_SIZE; // Линейное пробирование
            collided = true;
        }
        if (collided) collisions++;
        strcpy(keys[idx], key);
        is_occupied[idx] = true;
        return idx;
    }
};

// ==========================================
// 6.1. ХЕШ-ТАБЛИЦА С ДВОЙНЫМ ХЕШИРОВАНИЕМ
// ==========================================
struct DoubleHash {
    char keys[TABLE_SIZE][256];
    bool is_occupied[TABLE_SIZE];
    int collisions;

    void init() {
        for (int i = 0; i < TABLE_SIZE; ++i) is_occupied[i] = false;
        collisions = 0;
    }

    int hash1(const char* str) {
        unsigned long h = 5381;
        int c;
        while ((c = *str++)) h = ((h << 5) + h) + c;
        return h % TABLE_SIZE;
    }

    int hash2(const char* str) {
        unsigned long h = 0;
        int c;
        while ((c = *str++)) h = (h * 31) + c;
        return 1 + (h % (TABLE_SIZE - 1));
    }

    int find_or_insert(const char* key) {
        int idx = hash1(key);
        int step = hash2(key);
        bool collided = false;
        while (is_occupied[idx]) {
            if (strcmp(keys[idx], key) == 0) return idx;
            idx = (idx + step) % TABLE_SIZE; // Двойное хеширование
            collided = true;
        }
        if (collided) collisions++;
        strcpy(keys[idx], key);
        is_occupied[idx] = true;
        return idx;
    }
};

// ==========================================
// ТЕСТ ДЛЯ ПОДЗАДАНИЯ 6.1 (ЭМПИРИЧЕСКИЙ АНАЛИЗ)
// ==========================================
void test_hash_methods() {
    cout << "\n--- Эмпирический анализ 6.1 ---\n";
    
    // ВАЖНО: Выделяем в КУЧЕ, чтобы не переполнить стек (таблицы весят по ~5 МБ)
    LinearHash* lh = new LinearHash();
    DoubleHash* dh = new DoubleHash();
    lh->init();
    dh->init();

    // Генерируем 5000 случайных строк
    const int N = 5000;
    char** test_strings = new char*[N];
    for (int i = 0; i < N; ++i) {
        test_strings[i] = new char[256];
        sprintf(test_strings[i], "key_%d_%d", i, rand() % 1000);
    }

    // Тест 1: Линейное пробирование
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        lh->find_or_insert(test_strings[i]);
    }
    auto end = chrono::high_resolution_clock::now();
    auto lh_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Тест 2: Двойное хеширование
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        dh->find_or_insert(test_strings[i]);
    }
    end = chrono::high_resolution_clock::now();
    auto dh_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Вывод результатов
    cout << "Вставка " << N << " элементов:\n";
    cout << "1) Линейное пробирование:\n";
    cout << "   Время: " << lh_time << " мс\n";
    cout << "   Коллизий (потребовалось шагов): " << lh->collisions << "\n";
    cout << "2) Двойное хеширование:\n";
    cout << "   Время: " << dh_time << " мс\n";
    cout << "   Коллизий (потребовалось шагов): " << dh->collisions << "\n";
    
    cout << "\nВывод: Двойное хеширование обычно дает меньше коллизий,\n";
    cout << "так как шаг зависит от самого ключа, а не просто +1.\n";
    cout << "Но оно требует больше вычислений (два хеша).\n";
    cout << "------------------------------\n";

    // Очистка памяти
    for (int i = 0; i < N; ++i) delete[] test_strings[i];
    delete[] test_strings;
    delete lh;
    delete dh;
}

// ==========================================
// 6.2. ЗАДАЧА "РОДОСЛОВНАЯ"
// ==========================================
struct Person {
    char name[256];
    int parent_idx;
    int descendants_count;
};

int count_descendants(int p_idx, Person* people, int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (people[i].parent_idx == p_idx) {
            count += 1 + count_descendants(i, people, n);
        }
    }
    return count;
}

void runHash() {
    // Сначала запускаем тест 6.1!
    test_hash_methods();

    cout << "\n=== Задание 6.2: Родословная (Вариант 9) ===\n";
    
    // Тоже выделяем в куче!
    DoubleHash* ht = new DoubleHash();
    ht->init();

    cout << "Введите число элементов (N): ";
    int n;
    cin >> n;
    cin.ignore();

    Person* people = new Person[n];
    int people_count = 0;

    auto get_person_idx = [&](const char* name) -> int {
        ht->find_or_insert(name); 
        for (int i = 0; i < people_count; ++i) {
            if (strcmp(people[i].name, name) == 0) return i;
        }
        strcpy(people[people_count].name, name);
        people[people_count].parent_idx = -1;
        people[people_count].descendants_count = 0;
        return people_count++;
    };

    for (int i = 0; i < n - 1; ++i) {
        char child[256], parent[256];
        cin >> child >> parent;
        int c_idx = get_person_idx(child);
        int p_idx = get_person_idx(parent);
        people[c_idx].parent_idx = p_idx;
    }

    // Считаем потомков до сортировки (пока индексы валидны)
    for (int i = 0; i < people_count; ++i) {
        people[i].descendants_count = count_descendants(i, people, people_count);
    }

    // Сортируем по алфавиту
    for (int i = 0; i < people_count - 1; ++i) {
        for (int j = 0; j < people_count - i - 1; ++j) {
            if (strcmp(people[j].name, people[j+1].name) > 0) {
                Person temp = people[j];
                people[j] = people[j+1];
                people[j+1] = temp;
            }
        }
    }

    cout << "\nВывод:\n";
    for (int i = 0; i < people_count; ++i) {
        cout << people[i].name << " " << people[i].descendants_count << "\n";
    }

    delete[] people;
    delete ht;
}