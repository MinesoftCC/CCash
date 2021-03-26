#pragma once
#include <fstream>
#include <drogon/HttpController.h>
#include "account.hpp"
using namespace drogon;

bool BalComp(const std::pair<std::string, uint32_t> &a, const std::pair<std::string, uint32_t> &b);

class Bank
{
private:
    uint16_t user_id = 0; //assuming less then 2^16 users will be registered
    phmap::flat_hash_map<uint16_t, User> users;
    phmap::flat_hash_map<std::string, Account> accounts; //max of (2^16)*100

    //internal implementation
    bool ValidCreds(const std::string &cred_pass, const Account &account_name, uint16_t cred_id, Clearance level);

public:
    const std::string admin_pass;

    Bank() = delete;
    Bank(const std::string &init_pass);

    Json::Value AddUser(const std::string &name, const std::string &pass, const std::string &admin_pass);
    Json::Value DelUser(uint16_t id, const std::string &admin_pass);
    Json::Value SendFunds(const std::string &from, const std::string &to, uint32_t amount, uint16_t cred_id, const std::string &from_pass);
    Json::Value AddAccount(const std::string &account_name, uint16_t owner, const std::string &owner_pass);                   //client version
    Json::Value AddAccount(const std::string &account_name, uint32_t balance, uint16_t owner, const std::string &admin_pass); //admin version
    Json::Value DelAccount(const std::string &account_name, uint16_t cred_id, const std::string &pass);
    Json::Value AddPerm(const std::string &account_name, uint16_t new_id, uint16_t perm, const std::string &admin_pass);
    Json::Value AddPerm(const std::string &account_name, uint16_t new_id, uint16_t perm, uint16_t cred_id, const std::string &cred_pass);
    Json::Value DelPerm(const std::string &account_name, uint16_t target_id, uint16_t cred_id, const std::string &cred_pass);
    Json::Value AccountsRanked() const;
    Json::Value UsersRanked() const;
    Json::Value FindUser(const std::string &name) const;
    Json::Value ListUsers() const;
    Json::Value ListPerms(const std::string &account_name) const;
    Json::Value ListAccounts(uint16_t id) const;
    Json::Value Total(uint16_t id) const;
    Json::Value VerifyPassword(uint16_t id, const std::string attempt) const;

    void Save() const;
    void Load();
};
