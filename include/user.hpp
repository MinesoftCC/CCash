#pragma once
#include <string>
#include "../third_party/parallel-hashmap/parallel_hashmap/phmap.h"

struct User
{
    const std::string name, password;

    User(const std::string &name, const std::string &password) : name(name), password(password) {}
    phmap::flat_hash_set<std::string> accounts; //local references to accounts with perms for performance at the cost of memory

    Json::Value Serialize() const
    {
        Json::Value res;
        res["name"] = name;
        res["password"] = password;

        Json::Value temp;
        uint8_t i = 0; //because accounts is never more then 25
        for (const std::string &s : accounts)
        {
            temp[i++] = s;
        }
        res["accounts"] = temp;
        return res;
    }
};
