#include <iostream>
#include <cstring>
#include <chrono>

using namespace std;

const int TABLE_SIZE = 20011; 

// ==========================================
// 6.1. ХЕШ-ТАБЛИЦА С ЛИНЕЙНЫМ ПРОБИРОВАНИЕМ
// ==========================================
struct LinearHash {
    char keys[TABLE_SIZE][256];
    int  values[TABLE_SIZE];      
    bool is_occupied[TABLE_SIZE];
    bool is_deleted[TABLE_SIZE];
    int collisions;

    void init() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            is_occupied[i] = false;
            is_deleted[i]  = false;
            values[i]      = 0;
        }
        collisions = 0;
    }

    int hash(const char* str) {
        unsigned long h = 5381;
        int c;
        while ((c = *str++)) h = ((h << 5) + h) + c;
        return h % TABLE_SIZE;
    }

    bool insert(const char* key, int value) {
        int idx = hash(key);
        int first_deleted = -1;
        bool collided = false;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i) % TABLE_SIZE;

            if (is_occupied[pos] && strcmp(keys[pos], key) == 0) {
                values[pos] = value; 
                return true;
            }
            if (!is_occupied[pos] && is_deleted[pos] && first_deleted == -1)
                first_deleted = pos;
            if (!is_occupied[pos] && !is_deleted[pos]) {
                int target = (first_deleted != -1) ? first_deleted : pos;
                strcpy(keys[target], key);
                values[target] = value;
                is_occupied[target] = true;
                is_deleted[target]  = false;
                if (collided) collisions++;
                return true;
            }
            collided = true;
        }
        if (first_deleted != -1) {
            strcpy(keys[first_deleted], key);
            values[first_deleted] = value;
            is_occupied[first_deleted] = true;
            is_deleted[first_deleted]  = false;
            if (collided) collisions++;
            return true;
        }
        return false;
    }

    bool search(const char* key) {
        int idx = hash(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i) % TABLE_SIZE;
            if (!is_occupied[pos] && !is_deleted[pos])
                return false;
            if (is_occupied[pos] && strcmp(keys[pos], key) == 0)
                return true;
        }
        return false;
    }

    bool remove(const char* key) {
        int idx = hash(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i) % TABLE_SIZE;
            if (!is_occupied[pos] && !is_deleted[pos])
                return false;
            if (is_occupied[pos] && strcmp(keys[pos], key) == 0) {
                is_occupied[pos] = false;
                is_deleted[pos]  = true;
                return true;
            }
        }
        return false;
    }
};

// ==========================================
// 6.1. ХЕШ-ТАБЛИЦА С ДВОЙНЫМ ХЕШИРОВАНИЕМ
// ==========================================
struct DoubleHash {
    char keys[TABLE_SIZE][256];
    int  values[TABLE_SIZE];
    bool is_occupied[TABLE_SIZE];
    bool is_deleted[TABLE_SIZE];
    int collisions;

    void init() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            is_occupied[i] = false;
            is_deleted[i]  = false;
            values[i]      = 0;
        }
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

    bool insert(const char* key, int value) {
        int idx  = hash1(key);
        int step = hash2(key);
        int first_deleted = -1;
        bool collided = false;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i * step) % TABLE_SIZE;

            if (is_occupied[pos] && strcmp(keys[pos], key) == 0) {
                values[pos] = value;
                return true;
            }
            if (!is_occupied[pos] && is_deleted[pos] && first_deleted == -1)
                first_deleted = pos;
            if (!is_occupied[pos] && !is_deleted[pos]) {
                int target = (first_deleted != -1) ? first_deleted : pos;
                strcpy(keys[target], key);
                values[target] = value;
                is_occupied[target] = true;
                is_deleted[target]  = false;
                if (collided) collisions++;
                return true;
            }
            collided = true;
        }
        if (first_deleted != -1) {
            strcpy(keys[first_deleted], key);
            values[first_deleted] = value;
            is_occupied[first_deleted] = true;
            is_deleted[first_deleted]  = false;
            if (collided) collisions++;
            return true;
        }
        return false;
    }

    bool search(const char* key) {
        int idx  = hash1(key);
        int step = hash2(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i * step) % TABLE_SIZE;
            if (!is_occupied[pos] && !is_deleted[pos])
                return false;
            if (is_occupied[pos] && strcmp(keys[pos], key) == 0)
                return true;
        }
        return false;
    }

    bool remove(const char* key) {
        int idx  = hash1(key);
        int step = hash2(key);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            int pos = (idx + i * step) % TABLE_SIZE;
            if (!is_occupied[pos] && !is_deleted[pos])
                return false;
            if (is_occupied[pos] && strcmp(keys[pos], key) == 0) {
                is_occupied[pos] = false;
                is_deleted[pos]  = true;
                return true;
            }
        }
        return false;
    }
};

// ==========================================
// ТЕСТ ДЛЯ ПОДЗАДАНИЯ 6.1
// ==========================================
void test_hash_methods() {
    cout << "\n--- Эмпирический анализ 6.1 ---\n";
    
    LinearHash* lh = new LinearHash();
    DoubleHash* dh = new DoubleHash();
    lh->init();
    dh->init();

    const int N = 5000;
    char** test_strings = new char*[N];
    for (int i = 0; i < N; ++i) {
        test_strings[i] = new char[256];
        sprintf(test_strings[i], "key_%d_%d", i, rand() % 1000);
    }

    // Вставка теперь с передачей значения (i)
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        lh->insert(test_strings[i], i);
    }
    auto end = chrono::high_resolution_clock::now();
    auto lh_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        dh->insert(test_strings[i], i);
    }
    end = chrono::high_resolution_clock::now();
    auto dh_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Поиск теперь возвращает только true/false
    bool found1 = lh->search(test_strings[0]);
    bool found2 = dh->search(test_strings[N - 1]);

    cout << "Вставка " << N << " элементов:\n";
    cout << "1) Линейное пробирование:\n";
    cout << "   Время: " << lh_time << " мс\n";
    cout << "   Коллизий: " << lh->collisions << "\n";
    cout << "2) Двойное хеширование:\n";
    cout << "   Время: " << dh_time << " мс\n";
    cout << "   Коллизий: " << dh->collisions << "\n";

    cout << "------------------------------\n";

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
    test_hash_methods();

    cout << "\n=== Задание 6.2: Родословная (Вариант 9) ===\n";
    
    DoubleHash* ht = new DoubleHash();
    ht->init();

    cout << "Введите число элементов (N): ";
    int n;
    cin >> n;
    cin.ignore();

    Person* people = new Person[n];
    int people_count = 0;

    auto get_person_idx = [&](const char* name) -> int {
        // Если имени нет, вставляем его (передаем people_count как значение)
        if (!ht->search(name)) {
            ht->insert(name, people_count);
        }

        // Так как search не возвращает значение, ищем индекс перебором
        for (int i = 0; i < people_count; ++i) {
            if (strcmp(people[i].name, name) == 0) return i;
        }

        // Если человека не было — создаем новую запись
        strcpy(people[people_count].name, name);
        people[people_count].parent_idx = -1;
        people[people_count].descendants_count = 0;
        people_count++;
        return people_count - 1;
    };

    for (int i = 0; i < n - 1; ++i) {
        char child[256], parent[256];
        cin >> child >> parent;
        int c_idx = get_person_idx(child);
        int p_idx = get_person_idx(parent);
        people[c_idx].parent_idx = p_idx;
    }

    // Считаем потомков
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