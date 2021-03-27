#include "bank.h"

bool BalComp(const std::pair<std::string, uint32_t> &a, const std::pair<std::string, uint32_t> &b)
{
    return a.second > b.second;
}

bool Bank::ValidCreds(const std::string &cred_pass, const Account &account_temp, uint16_t cred_id, Clearance level)
{
    const auto &creds = account_temp.user_certs;
    if (creds.contains(cred_id) && users.contains(cred_id)) //both has a credential and is a user that exists
        if (creds.at(cred_id) >= level && users.at(cred_id).password == cred_pass)
            return true;
    return false;
}

Bank::Bank(const std::string &init_pass) : admin_pass(init_pass) {} ////bank owner as first account

Json::Value Bank::AddUser(const std::string &name, const std::string &pass, const std::string &admin_pass)
{
    Json::Value resp;

    //---JSON injection prevention---
    bool spec_flag = false;
    for (char c : name)
    {
        if (!std::isalpha(c))
        {
            spec_flag = true;
            break;
        }
    }
    if (spec_flag)
    {
        resp["content"] = "name cannot contain special characters";
        resp["value"] = 0;
        return resp;
    }
    //-------------------------------

    if (this->admin_pass != admin_pass)
    {
        resp["content"] = "invalid admin password";
        resp["value"] = 0;
        return resp;
    }
    if (users.contains(user_id))
    {
        resp["content"] = "user with the next id already exists";
        resp["value"] = 0;
        ++user_id; //increment so that next request may work
        return resp;
    }

    users.emplace(user_id++, User(name, pass));
    resp["content"] = "user added";
    resp["value"] = 1;
    LOG_INFO << "User \"" << name << "\" Added";
    Save();
    return resp;
}
Json::Value Bank::DelUser(uint16_t id, const std::string &admin_pass)
{
    Json::Value resp;
    if (this->admin_pass != admin_pass)
    {
        resp["content"] = "invalid admin pass";
        resp["value"] = 0;
        return resp;
    }
    if (!users.contains(id))
    {
        resp["content"] = "user doesnt exist";
        resp["value"] = 0;
        return resp;
    }

    //perm cleanup when user is deleted
    for (const auto &a : users.at(id).accounts)
    {
        accounts.at(a).user_certs.erase(id);
    }

    users.erase(id);
    resp["content"] = "user deleted";
    resp["value"] = 1;
    LOG_INFO << "User \"" << id << "\" Deleted";
    Save();
    return resp;
}

Json::Value Bank::VerifyPassword(uint16_t id, const std::string attempt) const
{
    Json::Value resp;
    if (!users.contains(id))
    {
        resp["content"] = "account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (users.at(id).password != attempt)
    {
        resp["content"] = "password is incorrect";
        resp["value"] = 0;
        return resp;
    }
    resp["content"] = "password is correct";
    resp["value"] = 1;
    Save();
    return resp;
}

Json::Value Bank::SendFunds(const std::string &from, const std::string &to, uint32_t amount, uint16_t cred_id, const std::string &from_pass)
{
    Json::Value resp;
    if (!accounts.contains(from))
    {
        resp["content"] = "\"from\" account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!accounts.contains(to))
    {

        resp["content"] = "\"to\" account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!ValidCreds(from_pass, accounts.at(from), cred_id, Clearance::Collaborator)) //is Collaborator
    {
        resp["content"] = "invalid creds";
        resp["value"] = 0;
        return resp;
    }
    if (accounts.at(from).GetBal() < amount)
    {
        resp["content"] = "balance must be greator then or equal to the amount being sent";
        resp["value"] = 0;
        return resp;
    }

    accounts.at(from).SubBal(amount);
    accounts.at(to).AddBal(amount);
    resp["content"] = "funds sent";
    resp["value"] = 1;
    LOG_INFO << from << " sent " << to << " $" << amount;
    Save();
    return resp;
}
Json::Value Bank::AddAccount(const std::string &account_name, uint16_t owner, const std::string &owner_pass) //client
{
    Json::Value resp;

    //---JSON injection prevention---
    bool spec_flag = false;
    for (char c : account_name)
    {
        if (!std::isalpha(c))
        {
            spec_flag = true;
            break;
        }
    }
    if (spec_flag)
    {
        resp["content"] = "name cannot contain special characters";
        resp["value"] = 0;
        return resp;
    }
    //-------------------------------

    if (accounts.contains(account_name))
    {
        resp["content"] = "account already exists";
        resp["value"] = 0;
        return resp;
    }
    if (!users.contains(owner))
    {
        resp["content"] = "user doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (users.at(owner).accounts.size() >= 100) //every user can only create 100 accounts
    {
        resp["content"] = "max accounts of 100 reached";
        resp["value"] = 0;
        return resp;
    }
    if (users.at(owner).password != owner_pass)
    {
        resp["content"] = "invalid password";
        resp["value"] = 0;
        return resp;
    }

    accounts.emplace(account_name, Account(0, owner));
    users.at(owner).accounts.emplace(account_name); //Owner ref to the new account
    resp["content"] = "account added";
    resp["value"] = 1;
    LOG_INFO << "Account " << account_name << " Created by " << owner;
    Save();
    return resp;
}
Json::Value Bank::AddAccount(const std::string &account_name, uint32_t balance, uint16_t owner, const std::string &admin_pass) //admin
{
    Json::Value resp;
    if (this->admin_pass != admin_pass)
    {
        resp["content"] = "invalid admin password";
        resp["value"] = 0;
        return resp;
    }
    if (!users.contains(owner) || accounts.contains(account_name))
    {
        resp["content"] = "invalid password";
        resp["value"] = 0;
        return resp;
    }

    accounts.emplace(account_name, Account(balance, owner));
    users.at(owner).accounts.emplace(account_name); //Owner ref to the new account
    resp["content"] = "account added";
    resp["value"] = 1;
    LOG_INFO << "Account " << account_name << " Created for " << owner << " with balance of " << balance;
    Save();
    return resp;
}
Json::Value Bank::DelAccount(const std::string &account_name, uint16_t cred_id, const std::string &pass)
{
    Json::Value resp;
    if (!accounts.contains(account_name))
    {
        resp["content"] = "account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!ValidCreds(pass, accounts.at(account_name), cred_id, Clearance::Admin)) //is Admin
    {
        resp["content"] = "invalid credentials";
        resp["value"] = 0;
        return resp;
    }

    //cleanup
    for (const auto &u : accounts.at(account_name).user_certs)
    {
        users.at(u.first).accounts.erase(account_name);
    }

    accounts.erase(account_name);
    resp["content"] = "account deleted";
    resp["value"] = 1;
    LOG_INFO << "Account " << account_name << " Deleted by " << cred_id;
    Save();
    return resp;
}
Json::Value Bank::ListAccounts(uint16_t id) const
{
    Json::Value resp;
    if (!users.contains(id))
        return resp;
    for (const std::string &a : users.at(id).accounts)
    {
        resp[a]["clearance"] = (int)accounts.at(a).user_certs.at(id);
        resp[a]["balance"] = accounts.at(a).GetBal();
    }
    return resp;
}
Json::Value Bank::AccountsRanked() const
{
    Json::Value resp;
    std::vector<std::pair<std::string, uint32_t>> temp_vec;
    for (auto &a : accounts)
    {
        temp_vec.push_back({a.first, a.second.GetBal()});
    }
    std::sort(temp_vec.begin(), temp_vec.end(), BalComp);
    for (uint32_t i = 0; i < temp_vec.size(); ++i)
    {
        resp[i]["name"] = temp_vec[i].first;
        resp[i]["balance"] = temp_vec[i].second;
    }
    return resp;
}
Json::Value Bank::UsersRanked() const
{
    Json::Value res;
    uint32_t sum = 0;
    std::vector<std::pair<std::string, uint32_t>> temp_vec;
    for (const auto &u : users)
    {
        sum = 0;
        for (const auto &a : u.second.accounts)
        {
            if (accounts.at(a).user_certs.at(u.first) >= Clearance::Collaborator)
                sum += accounts.at(a).GetBal();
        }
        temp_vec.push_back({u.second.name + '#' + std::to_string(u.first), sum});
    }
    std::sort(temp_vec.begin(), temp_vec.end(), BalComp);
    Json::Value temp;
    for (uint16_t i = 0; i < temp_vec.size(); ++i)
    {
        temp["name"] = temp_vec[i].first;
        temp["balance"] = temp_vec[i].second;
        res[i] = temp;
    }
    return res;
}
Json::Value Bank::FindUser(const std::string &name) const
{
    Json::Value resp;
    for (const auto &u : users)
    {
        if (std::search(u.second.name.begin(), u.second.name.end(), name.begin(), name.end()) != name.end())
        {
            resp[u.first] = u.second.name;
        }
    }
    return resp;
}
Json::Value Bank::ListUsers() const
{
    Json::Value resp;
    for (const auto &u : users)
    {
        resp[u.first] = u.second.name;
    }
    return resp;
}
Json::Value Bank::Total(uint16_t id) const
{
    Json::Value resp;
    if (!users.contains(id))
        return resp;
    uint32_t sum = 0;
    const auto &user_accounts_temp = users.at(id).accounts;
    for (const auto &a : users.at(id).accounts)
    {
        if (accounts.at(a).user_certs.at(id) >= Clearance::Collaborator)
            sum += accounts.at(a).GetBal();
    }
    resp["name"] = users.at(id).name;
    resp["perm_count"] = (uint8_t)users.at(id).accounts.size();
    resp["balance"] = sum;
    return resp;
}
Json::Value Bank::AddPerm(const std::string &account_name, uint16_t new_id, uint16_t perm, const std::string &admin_pass)
{
    Json::Value resp;
    if (admin_pass != this->admin_pass)
    {
        resp["content"] = "invalid admin pass";
        resp["value"] = 0;
        return resp;
    }
    if (!accounts.contains(account_name))
    {
        resp["content"] = "account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!users.contains(new_id))
    {
        resp["content"] = "user doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (users.at(new_id).accounts.size() >= 100)
    {
        resp["content"] = "max account connections reached at 100";
        resp["value"] = 0;
        return resp;
    }
    if (accounts.at(account_name).user_certs.contains(new_id))
    {
        resp["content"] = "cant add cert as that user already has a perm";
        resp["value"] = 0;
        return resp;
    }

    accounts.at(account_name).user_certs.emplace(new_id, (Clearance)perm);
    users.at(new_id).accounts.emplace(account_name);
    resp["content"] = "perm added";
    resp["value"] = 1;
    LOG_INFO << "Perm Added for " << new_id << " with level " << perm;
    Save();
    return resp;
}
Json::Value Bank::AddPerm(const std::string &account_name, uint16_t new_id, uint16_t perm, uint16_t cred_id, const std::string &cred_pass)
{
    Json::Value resp;
    if (!accounts.contains(account_name))
    {
        resp["content"] = "account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!users.contains(new_id))
    {
        resp["content"] = "user doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (users.at(new_id).accounts.size() >= 100)
    {
        resp["content"] = "max account connections reached at 100";
        resp["value"] = 0;
        return resp;
    }
    if (!ValidCreds(cred_pass, accounts.at(account_name), cred_id, Clearance::Manager))
    {
        resp["content"] = "invalid credentials";
        resp["value"] = 0;
        return resp;
    }
    if (accounts.at(account_name).user_certs.contains(new_id))
    {
        resp["content"] = "cant add cert as that user already has a perm";
        resp["value"] = 0;
        return resp;
    }
    std::cout << (int)perm << ' ' << (int)accounts.at(account_name).user_certs.at(cred_id) << '\n';
    if (perm > (int)accounts.at(account_name).user_certs.at(cred_id))
    {
        resp["content"] = "unable to create perms higher then your own";
        resp["value"] = 0;
        return resp;
    }

    accounts.at(account_name).user_certs.emplace(new_id, (Clearance)perm);
    users.at(new_id).accounts.emplace(account_name);
    resp["content"] = "perm added";
    resp["value"] = 1;
    LOG_INFO << "Perm Added for " << new_id << " with level " << perm;
    Save();
    return resp;
}
Json::Value Bank::DelPerm(const std::string &account_name, uint16_t target_id, uint16_t cred_id, const std::string &cred_pass) //manager perms required
{
    Json::Value resp;
    if (!accounts.contains(account_name))
    {
        resp["content"] = "account doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (!ValidCreds(cred_pass, accounts.at(account_name), cred_id, Clearance::Manager))
    {
        resp["content"] = "invalid credentials";
        resp["value"] = 0;
        return resp;
    }
    if (!accounts.at(account_name).user_certs.contains(target_id)) //if that perm exists
    {
        resp["content"] = "perm specified doesnt exist";
        resp["value"] = 0;
        return resp;
    }
    if (accounts.at(account_name).user_certs.at(target_id) >= accounts.at(account_name).user_certs.at(cred_id)) //manager cant delete admin or manager
    {
        resp["content"] = "you cant delete a perm higher then your credentials";
        resp["value"] = 0;
        return resp;
    }

    accounts.at(account_name).user_certs.erase(target_id);
    resp["content"] = "perm deleted";
    resp["value"] = 1;
    LOG_INFO << "Perm of " << target_id << " has been Deleted";
    Save();
    return resp;
}
Json::Value Bank::ListPerms(const std::string &account_name) const
{
    Json::Value resp;
    if (!accounts.contains(account_name))
    {
        resp["content"] = "account doesnt exist";
        resp["vaue"] = 0;
    }

    uint32_t i = 0;
    for (const auto &p : accounts.at(account_name).user_certs)
    {
        resp[i]["id"] = p.first;
        resp[i]["clearance"] = (int)p.second;
        ++i;
    }
    return resp;
}

void Bank::Save() const
{
    Json::StreamWriterBuilder builder;
    const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    {
        std::ofstream user_save("users.json");
        Json::Value temp;
        temp["id"] = user_id;
        for (const auto &u : users)
        {
            temp["users"][u.first] = u.second.Serialize();
        }
        writer->write(temp, &user_save);
        user_save.close();
        //LOG_INFO << "Users Saved";
    }
    {
        std::ofstream account_save("accounts.json");
        Json::Value temp;
        for (const auto &a : accounts)
        {
            temp[a.first] = a.second.Serialize();
        }
        writer->write(temp, &account_save);
        account_save.close();
        //LOG_INFO << "Accounts Saved";
    }
}
void Bank::Load()
{
    Json::CharReaderBuilder builder;

    {
        Json::Value temp;
        std::ifstream user_save("users.json");
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, user_save, &temp, &errs))
        {
            user_save.close();
            LOG_DEBUG << "Loading Users ERROR: " << errs;
        }
        else
        {
            user_save.close();
            LOG_INFO << "Parsing Users Suceeded";
            user_id = temp["id"].asUInt();
            for (uint16_t i = 0; i < temp["users"].size(); ++i)
            {
                if (temp["users"].isValidIndex(i)) //excluding holes
                {
                    User temp_user(temp["users"][i]["name"].asString(), temp["users"][i]["password"].asString());
                    for (uint8_t j = 0; j < temp["users"][i]["accounts"].size(); ++j) //each account
                    {
                        temp_user.accounts.emplace(temp["users"][i]["accounts"][j].asString());
                    }
                    users.emplace(i, temp_user);
                }
            }
            LOG_INFO << "Users Loaded";
        }
    }
    {
        Json::Value temp;
        std::ifstream account_save("accounts.json");
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, account_save, &temp, &errs))
        {
            LOG_DEBUG << "Loading Accounts ERROR: " << errs;
            account_save.close();
        }
        else
        {
            account_save.close();
            LOG_INFO << "Parsing Accounts Suceeded";
            auto members = temp.getMemberNames();
            for (const std::string &v : members)
            {
                Account account_temp(temp[v]["balance"].asUInt());          //32 bit
                for (uint16_t i = 0; i < temp[v]["user_certs"].size(); ++i) //each cert
                {
                    account_temp.user_certs.emplace(i, (Clearance)temp[v]["user_certs"][i].asInt());
                }
                accounts.emplace(v, account_temp);
            }
            LOG_INFO << "Accounts Loaded";
        }
    }
}
