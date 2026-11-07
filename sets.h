#ifndef SETS_H
#define SETS_H

#include <iostream>
#include "../forward_list.h" // Подключаем твой ForwardList из ЛР1

using namespace std;

const int TABLE_SIZE = 10007;

// Наше Множество теперь использует списки из ЛР1
struct HashSet {
    ForwardList table[TABLE_SIZE]; 
};

void set_init(HashSet& s);
void set_add(HashSet& s, const char* value);
void set_del(HashSet& s, const char* value);
bool set_at(const HashSet& s, const char* value);
void set_print(const HashSet& s);

void runSets(int argc, char* argv[]);

#endif