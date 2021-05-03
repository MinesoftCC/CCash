# CCash

A webserver hosting a virtual bank of sorts to be used in MC via the CC mod, intended to be connected to future services. Written in C++

during this quarentine I spontaneously decided to redo an old project idea I had for a MC bank, it unfortunately had many flaws that I wont go into (the original attempt), but this new implementation with an external approach not only solves those it also adds new features! This is intended to be ran along side a MC server with sockets that the lua computers of CC talk to. By moving the computation from in game to this program it enables both future devolopment with less of a concern on hardware resources and scalability, furthermore it wouldnt even have to be ran on the same server as its completely external.

uses https://github.com/greg7mdp/parallel-hashmap for the hashmap

uses https://github.com/an-tao/drogon for webserver/logging stuff

## Build
drogon depedencies
```
sudo apt install libjsoncpp-dev
sudo apt install uuid-dev
sudo apt install openssl
sudo apt install libssl-dev
```
building this program
```
git clone --recurse-submodules https://github.com/MinesoftCC/CCash
cd CCash
sudo mkdir build
cd build
sudo cmake ..
sudo make
sudo ./CCash root true
```

## Features

**Performance**

* In-game performance isn't impacted
* Entirely in memory (although it saves to disk)
* NOT written in lua lol
* No overhead of virtualization

**Convenient**

you can use the bank anywhere, irregardless of position or server even.
In the future a client program or web browser interface could be made

**Low Cost in-game**

any computer in CC can use the bank, no ender modem or advanced computer required

**Security**

compared to an in-game implementation that can be destroyed, modified, or hacked.
The amount of effort to compromise this external solution would be much higher

**Logging**

all relevant commands/errors are logged in a drogon.txt file

## Notes:

* max 100 accounts per user, be that owned or otherwise
* While "CC" is through out this program OC works just as well, anything that can interact with HTTP can use it.
* I know http is quite vulnerable but this is for MC and I doubt someone will have access to the servers traffic
* If being accessed locally the CC config file's "blocked_domains" needs to have "127.0.0.0/8" removed
* I could have used a database but considering the scale of this project that seemed a bit overkill.
* I originally had multi-threaded everything but the overhead was greater then profit.

## Connected Services

if you wanted to you could have a seperate server full of CC webserver programs interconnected. Your MC server would interact with this server providing a lot of functionality

:sparkles:implemented:

* Leaderboard, this just basically continually calls uranked and aranked            [found at https://github.com/MinesoftCC/CCServices/tree/main/Leaderboard]
* ATM for exchanging a chosen resource like diamonds or netherite and bank currency [found at https://github.com/MinesoftCC/CCServices/tree/main/ATM]
* Market, written by Sam                                                            [found at https://github.com/MinesoftCC/market-api]

:construction: just ideas:

* Gambling
* Delivery Service, could act as an extension to the market
* a Cute front-end

if you want to make a connected service for this webserver then check out `http://127.0.0.1/BankAPI/help` for a list of the methods and there description

## Commands

```
/BankAPI/admin/close/{pass} (POST)
/BankAPI/help (GET)
/BankAPI/admin/adduser/{name}/{pass}/{admin_pass} (POST)
/BankAPI/admin/deluser/{id}/{admin_pass} (POST)
/BankAPI/sendfunds/{from}/{to}/{amount}/{cred_id}/{from_pass} (POST)
/BankAPI/addaccount/{account_name}/{owner_id}/{owner_pass} (POST)
/BankAPI/admin/addaccount/{account_name}/{balance}/{owner_id}/{admin_pass} (POST)
/BankAPI/delaccount/{account_name}/{cred_id}/{pass} (POST)
/BankAPI/admin/addperm/{account_name}/{new_id}/{perm}/{admin_pass} (POST)
/BankAPI/addperm/{account_name}/{new_id}/{perm}/{cred_id}/{cred_pass} (POST)
/BankAPI/delperm/{account_name}/{target_id}/{cred_id}/{cred_pass} (POST)
/BankAPI/aranked (GET)
/BankAPI/uranked (GET)
/BankAPI/finduser/{username} (GET)
/BankAPI/listusers (GET)
/BankAPI/listperms/{account_name} (GET)
/BankAPI/listaccounts/{id} (GET)
/BankAPI/total/{id} (GET)
/BankAPI/verifypass/{id}/{pass} (POST)
```

## Permissions

```
0:Specatator,   //can view
1:Collaborator, //can send money
2:Manager,      //can del and add permission
3:Admin,        //can del account
```
