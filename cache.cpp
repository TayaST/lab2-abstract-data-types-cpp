#include <iostream>
#include <string>
#include <cstring>
#include "../list.h" // Двусвязный список из ЛР1

using namespace std;

const int HASH_SIZE = 10007;

// Узел хеш-таблицы (хранит ключ, значение и указатель на узел списка ЛР1)
struct CacheHashNode {
    int key;
    int value;
    LNode* list_node; 
    CacheHashNode* next;
};

struct LRUCache {
    int capacity;
    int current_size;
    DoublyList list; 
    CacheHashNode* hash_table[HASH_SIZE];

    void init(int cap) {
        capacity = cap;
        current_size = 0;
        dlist_init(list);
        for (int i = 0; i < HASH_SIZE; ++i) hash_table[i] = nullptr;
    }

    int hash_func(int key) { return key % HASH_SIZE; }

    CacheHashNode* find_hash_node(int key) {
        int idx = hash_func(key);
        CacheHashNode* curr = hash_table[idx];
        while (curr != nullptr) {
            if (curr->key == key) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    void add_hash_node(int key, int value, LNode* list_node) {
        int idx = hash_func(key);
        CacheHashNode* newNode = new CacheHashNode;
        newNode->key = key;
        newNode->value = value;
        newNode->list_node = list_node;
        newNode->next = hash_table[idx];
        hash_table[idx] = newNode;
    }

    void remove_hash_node(int key) {
        int idx = hash_func(key);
        CacheHashNode* curr = hash_table[idx];
        CacheHashNode* prev = nullptr;
        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) hash_table[idx] = curr->next;
                else prev->next = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    LNode* silent_push_head(const string& val) {
        LNode* newNode = new LNode;
        newNode->value = val;
        newNode->prev = nullptr;
        newNode->next = list.head;
        if (list.head != nullptr) list.head->prev = newNode;
        list.head = newNode;
        if (list.tail == nullptr) list.tail = newNode;
        list.count++;
        return newNode;
    }

    void move_to_head(LNode* node) {
        if (node == list.head) return;
        if (node->prev != nullptr) node->prev->next = node->next;
        if (node->next != nullptr) node->next->prev = node->prev;
        if (node == list.tail) list.tail = node->prev;
        node->prev = nullptr;
        node->next = list.head;
        if (list.head != nullptr) list.head->prev = node;
        list.head = node;
    }

    void remove_tail() {
        if (list.tail == nullptr) return;
        LNode* tail_node = list.tail;
        int key_to_remove = stoi(tail_node->value);
        remove_hash_node(key_to_remove);
        if (list.tail->prev != nullptr) {
            list.tail = list.tail->prev;
            list.tail->next = nullptr;
        } else {
            list.head = nullptr;
            list.tail = nullptr;
        }
        delete tail_node;
        list.count--;
    }

    void set(int key, int value) {
        CacheHashNode* node = find_hash_node(key);
        if (node != nullptr) {
            node->value = value;
            move_to_head(node->list_node);
        } else {
            if (current_size == capacity) {
                remove_tail();
                current_size--;
            }
            LNode* new_node = silent_push_head(to_string(key));
            add_hash_node(key, value, new_node);
            current_size++;
        }
    }

    int get(int key) {
        CacheHashNode* node = find_hash_node(key);
        if (node == nullptr) return -1;
        move_to_head(node->list_node);
        return node->value;
    }

    void print_cache() {
        cout << "Cache: ";
        LNode* curr = list.head;
        while (curr != nullptr) {
            CacheHashNode* hn = find_hash_node(stoi(curr->value));
            if (hn) cout << "[" << hn->key << "->" << hn->value << "] ";
            curr = curr->next;
        }
        cout << "\n";
    }
};

void runCache() {
    cout << "\n=== Задание 7: LRU Кэш (Вариант 1) ===\n";
    int cap, q;
    cout << "Введите емкость кэша (cap) и количество запросов (Q): ";
    cin >> cap >> q;
    cin.ignore();

    LRUCache cache;
    cache.init(cap);

    cout << "Вводите запросы (например: SET 1 2 или GET 1):\n";
    for (int i = 0; i < q; ++i) {
        string cmd;
        cin >> cmd;
        if (cmd == "SET") {
            int x, y;
            cin >> x >> y;
            cache.set(x, y);
        } else if (cmd == "GET") {
            int x;
            cin >> x;
            cout << cache.get(x) << " ";
        }
    }
    cout << "\n";
}