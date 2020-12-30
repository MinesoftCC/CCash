# CCash

A webserver hosting a virtual bank of sorts to be used in MC via the CC mod, intended to be connected to future services. Written in C++

during this quarentine I spontaneously decided to redo an old project idea I had for a MC bank, it unfortunately had many flaws that I wont go into (the original attempt), but this new implementation with an external approach not only solves those it also adds new features! This is intended to be ran along side a MC server with sockets that the lua computers of CC talk to. By moving the computation from in game to this program it enables both future devolopment with less of a concern on hardware resources and scalability, furthermore it wouldnt even have to be ran on the same server as its completely external.

uses https://github.com/greg7mdp/parallel-hashmap for the hashmap

uses https://github.com/an-tao/drogon for webserver/logging stuff

## Build

```
sudo git clone --recurse-submodules https://github.com/MinesoftCC/CCash 
cd CCash
sudo mkdir build
cd build
sudo cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make
sudo ./CCash
```

## Features

**Performance**

```
* In-game performance isn't impacted
* Extremely Low Latency
* NOT written in lua lol
* No overhead of virtualization
* written in C++
```

**Convenient**

```
you can use the bank anywhere, irregardless of position or server even.
In the future a client program or web browser interface could be made
```

**Low Cost in-game**

```
any computer in CC can use the bank, no ender modem or advanced computer required
```

**Security**

```
compared to an in-game implementation that can be destroyed, modified, or hacked.
The amount of effort to compromise this external solution would be much higher
```

**Logging**

```
all relevant commands/errors are logged in a drogon.txt file
```

## Notes:

```
* max 100 accounts per user, be that owned or otherwise
* While "CC" is through out this program OC works just as well, anything that can interact with HTTP can use it.
* I know http is quite vulnerable but this is for MC and I doubt someone will have access to the servers traffic
* If being accessed locally the CC config file's "blocked_domains" needs to have "127.0.0.0/8" removed
* I could have used a database but considering the scale of this project that seemed a bit overkill.
* I originally had multi-threaded everything but the overhead was greater then profit.
```

## Saving

I heavily recommend using a script that saves the banks state every x minutes such as

```
10 * * * * curl -X POST http://127.0.0.1/BankAPI/admin/save/{password}
# a cron job
```

how to use cron: https://opensource.com/article/17/11/how-use-cron-linux

If you already have such a script for backing up the mc server I recommend appending
a call to `http://127.0.0.1/BankAPI/admin/save/{password}` to it.
If you dont save the state before shutting the server down the bank will be lost

similiarly if you have a script for closing the server I recommend
appending a `http://127.0.0.1/BankAPI/admin/close/{password}` call to it

## Connected Services

if you wanted to you could have a seperate server full of CC webserver programs interconnected. Your MC server would interact with this server providing a lot of functionality

:sparkles:implemented:

```
* Leaderboard, this just basically continually calls uranked and aranked
* ATM for exchanging a chosen resource like diamonds or netherite and bank currency
* Market, Jolly is working on implementing a market, https://github.com/STBoyden/market-api
```

:construction: just ideas:

```
* Gambling
* Delivery Service, could act as an extension to the market
* a Cute front-end
```

if you want to make a connected service for this webserver then check out `http://127.0.0.1/BankAPI/help` for a list of the methods and there description

**Commands**

```
/BankAPI/admin/close/{pass} (POST) 
/BankAPI/admin/save/{pass} (POST) 
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
/BankAPI/vpass/{id}/{pass} (POST) 
```
