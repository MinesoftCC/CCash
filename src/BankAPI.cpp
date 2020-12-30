#include "BankAPI.h"

BankAPI::BankAPI(const std::string &init_pass) : internal(init_pass)
{
    this->internal.Load();
}

void BankAPI::Close(reqArgs, const std::string &pass) const
{
    auto resp = HttpResponse::newHttpResponse();
    if (this->internal.admin_pass == pass)
    {
        this->internal.Save();
        resp->setBody("<p>[Webserver Closed]</p>");
        callback(resp);
        LOG_INFO << "Server Closed";
        app().quit();
    }
    resp->setBody("<p>[Invalid Password]</p>");
    callback(resp);
}
void BankAPI::Save(reqArgs, const std::string &pass) const
{
    auto resp = HttpResponse::newHttpResponse();
    if (this->internal.admin_pass == pass)
    {
        LOG_INFO << "Saving...";
        this->internal.Save();
        resp->setBody("<p>[Webserver Saving]</p>");
        callback(resp);
    }
    resp->setBody("<p>[Invalid Password]</p>");
    callback(resp);
}

void BankAPI::Help(reqArgs) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("<p>/BankAPI/admin/close/{pass} (POST) <br />/BankAPI/admin/save/{pass} (POST) <br />/BankAPI/help (GET) <br />/BankAPI/admin/adduser/{name}/{pass}/{admin_pass} (POST) <br />/BankAPI/admin/deluser/{id}/{admin_pass} (POST) <br />/BankAPI/sendfunds/{from}/{to}/{amount}/{cred_id}/{from_pass} (POST) <br />/BankAPI/addaccount/{account_name}/{owner_id}/{owner_pass} (POST) <br />/BankAPI/admin/addaccount/{account_name}/{balance}/{owner_id}/{admin_pass} (POST) <br />/BankAPI/delaccount/{account_name}/{cred_id}/{pass} (POST) <br />/BankAPI/addperm/{account_name}/{new_id}/{perm}/{cred_id}/{cred_pass} (POST) <br />/BankAPI/delperm/{account_name}/{target_id}/{cred_id}/{cred_pass} (POST) <br />/BankAPI/aranked (GET) <br />/BankAPI/uranked (GET) <br />/BankAPI/finduser/{username} (GET)<br />/BankAPI/listusers (GET)<br />/BankAPI/listperms/{account_name} (GET) <br />/BankAPI/listaccounts/{id} (GET) <br />/BankAPI/total/{id} (GET) <br />/BankAPI/vpass/{id}/{pass} (POST)</p>");
    resp->setExpiredTime(0);
    callback(resp);
}
void BankAPI::AddUser(reqArgs, const std::string &name, const std::string &pass, const std::string &admin_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AddUser(name, pass, admin_pass)));
}
void BankAPI::DelUser(reqArgs, uint16_t id, const std::string &admin_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.DelUser(id, admin_pass)));
}
void BankAPI::SendFunds(reqArgs, const std::string &from, const std::string &to, uint32_t amount, uint16_t cred_id, const std::string &from_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.SendFunds(from, to, amount, cred_id, from_pass)));
}
void BankAPI::AddAccount(reqArgs, const std::string &account_name, uint16_t owner, const std::string &owner_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AddAccount(account_name, owner, owner_pass)));
}
void BankAPI::AdminAddAccount(reqArgs, const std::string &account_name, uint32_t balance, uint16_t owner, const std::string &admin_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AddAccount(account_name, balance, owner, admin_pass)));
}
void BankAPI::DelAccount(reqArgs, const std::string &account_name, uint16_t cred_id, const std::string &pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.DelAccount(account_name, cred_id, pass)));
}
void BankAPI::AdminAddPerm(reqArgs, const std::string &account_name, uint16_t new_id, uint8_t perm, const std::string& admin_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AddPerm(account_name, new_id, perm, admin_pass)));
}
void BankAPI::AddPerm(reqArgs, const std::string &account_name, uint16_t new_id, uint8_t perm, uint16_t cred_id, const std::string &cred_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AddPerm(account_name, new_id, perm, cred_id, cred_pass)));
}
void BankAPI::DelPerm(reqArgs, const std::string &account_name, uint16_t target_id, uint16_t cred_id, const std::string &cred_pass)
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.DelPerm(account_name, target_id, cred_id, cred_pass)));
}
void BankAPI::AccountsRanked(reqArgs) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.AccountsRanked()));
}
void BankAPI::UsersRanked(reqArgs) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.UsersRanked()));
}
void BankAPI::FindUser(reqArgs, const std::string &name) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.FindUser(name)));
}
void BankAPI::ListUsers(reqArgs) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.ListUsers()));
}
void BankAPI::ListPerms(reqArgs, const std::string &account_name) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.ListPerms(account_name)));
}
void BankAPI::ListAccounts(reqArgs, uint16_t id) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.ListAccounts(id)));
}
void BankAPI::Total(reqArgs, uint16_t id) const
{
    callback(HttpResponse::newHttpJsonResponse(this->internal.Total(id)));
}
void BankAPI::VerifyPassword(reqArgs, uint16_t id, const std::string &attempt) const
{
    auto resp = HttpResponse::newHttpJsonResponse(this->internal.VerifyPassword(id, attempt));
    LOG_INFO << "request to verify " << id << "'s password";
    callback(resp);
}
