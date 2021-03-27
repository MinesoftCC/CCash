#pragma once
#include <vector>
#include "user.hpp"
#include "permission.hpp"

class Account
{
private:
    uint32_t balance;

public:
    phmap::node_hash_map<uint16_t, Clearance> user_certs; //Bank owner backdoor
    Account(uint32_t balance) : balance(balance) {}
    Account(uint32_t balance, uint16_t owner) : balance(balance)
    {
        user_certs.emplace(owner, Clearance::Admin);
    }
    uint32_t GetBal() const
    {
        return balance;
    }
    void SubBal(uint32_t amount)
    {
        balance -= amount;
    }
    void AddBal(uint32_t val)
    {
        balance += val;
    }

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = balance;

        Json::Value temp;
        for (const auto &c : user_certs)
        {
            temp[c.first] = (int)c.second;
        }
        res["user_certs"] = temp;
        return res;
    }
};
