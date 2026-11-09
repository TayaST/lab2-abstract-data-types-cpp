#ifndef SETS_H
#define SETS_H
#include <iostream>
#include "../forward_list.h"

using namespace std;

const int TABLE_SIZE = 10007;

struct HashSet {
    ForwardList table[TABLE_SIZE];
};

void set_init(HashSet& s);
bool set_add(HashSet& s, const char* value); // Теперь возвращает bool
void set_del(HashSet& s, const char* value);
bool set_at(const HashSet& s, const char* value);
void set_print(const HashSet& s);
void runSets(int argc, char* argv[]);

#endif