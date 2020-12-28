#pragma once
#include <vector>
#include "user.hpp"
#include "permission.hpp"

class Account
{
private:
    uint32_t balance;

public:
    phmap::node_hash_map<uint16_t, Clearance> user_certs{{0, Clearance::Admin}}; //Bank owner backdoor

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
};
