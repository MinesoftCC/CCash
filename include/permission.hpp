#pragma once
#include <string>

enum class Clearance
{
    //anyone can view balance
    Specatator,   //can view log
    Collaborator, //can send money
    Manager,      //can del and add permissions
    Admin,        //can del account
};
