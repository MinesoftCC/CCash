#pragma once
#include <string>
#include "../third_party/parallel-hashmap/parallel_hashmap/phmap.h"

struct User
{
    const std::string name, password;

    User(const std::string &name, const std::string &password) : name(name), password(password) {}
    phmap::node_hash_set<std::string> accounts; //local references to accounts with perms for performance at the cost of memory
};
