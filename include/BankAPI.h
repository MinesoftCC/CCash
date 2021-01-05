#pragma once
#include "bank.h"

#define reqArgs const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class BankAPI : public drogon::HttpController<BankAPI, false>
{
private:
  Bank internal;

public:
  BankAPI() = delete;
  BankAPI(const std::string &);
  void Close(reqArgs, const std::string &) const;

  void Help(reqArgs) const;
  void AddUser(reqArgs, const std::string &, const std::string &, const std::string &);
  void DelUser(reqArgs, const uint16_t, const std::string &);
  void SendFunds(reqArgs, const std::string &, const std::string &, uint32_t, uint16_t, const std::string &);
  void AddAccount(reqArgs, const std::string &, uint16_t, const std::string &);
  void AdminAddAccount(reqArgs, const std::string &, uint32_t, uint16_t, const std::string &);
  void DelAccount(reqArgs, const std::string &, uint16_t, const std::string &);
  void AdminAddPerm(reqArgs, const std::string &, uint16_t, uint16_t, const std::string &);
  void AddPerm(reqArgs, const std::string &, uint16_t, uint16_t, uint16_t, const std::string &);
  void DelPerm(reqArgs, const std::string &, uint16_t, uint16_t, const std::string &);
  void AccountsRanked(reqArgs) const;
  void UsersRanked(reqArgs) const;
  void FindUser(reqArgs, const std::string &) const;
  void ListUsers(reqArgs) const;
  void ListPerms(reqArgs, const std::string &) const;
  void ListAccounts(reqArgs, uint16_t) const;
  void Total(reqArgs, uint16_t) const;
  void VerifyPassword(reqArgs, uint16_t, const std::string &) const;

  METHOD_LIST_BEGIN
  METHOD_ADD(BankAPI::Close, "/admin/close/{pass}", Post);
  METHOD_ADD(BankAPI::Help, "/help", Get); //incomplete
  METHOD_ADD(BankAPI::AddUser, "/admin/adduser/{name}/{pass}/{admin_pass}", Post);
  METHOD_ADD(BankAPI::DelUser, "/admin/deluser/{id}/{admin_pass}", Post); //should be Delete but CC only has Post/Get
  METHOD_ADD(BankAPI::SendFunds, "/sendfunds/{from}/{to}/{amount}/{cred_id}/{from_pass}", Post);
  METHOD_ADD(BankAPI::AddAccount, "/addaccount/{account_name}/{owner_id}/{owner_pass}", Post);
  METHOD_ADD(BankAPI::AdminAddAccount, "/admin/addaccount/{account_name}/{balance}/{owner_id}/{admin_pass}", Post);
  METHOD_ADD(BankAPI::DelAccount, "/delaccount/{account_name}/{cred_id}/{pass}", Post);
  METHOD_ADD(BankAPI::AdminAddPerm, "/admin/addperm/{account_name}/{new_id}/{perm}/{admin_pass}", Post);
  METHOD_ADD(BankAPI::AddPerm, "/addperm/{account_name}/{new_id}/{perm}/{cred_id}/{cred_pass}", Post);
  METHOD_ADD(BankAPI::DelPerm, "/delperm/{account_name}/{target_id}/{cred_id}/{cred_pass}", Post);
  METHOD_ADD(BankAPI::AccountsRanked, "/aranked", Get);
  METHOD_ADD(BankAPI::UsersRanked, "/uranked", Get);
  METHOD_ADD(BankAPI::FindUser, "/finduser/{username}", Get);
  METHOD_ADD(BankAPI::ListUsers, "/listusers", Get);
  METHOD_ADD(BankAPI::ListPerms, "/listperms/{account_name}", Get);
  METHOD_ADD(BankAPI::ListAccounts, "/listaccounts/{id}", Get);
  METHOD_ADD(BankAPI::Total, "/total/{id}", Get);
  METHOD_ADD(BankAPI::VerifyPassword, "/verfiypass/{id}/{pass}", Post);
  METHOD_LIST_END
};
