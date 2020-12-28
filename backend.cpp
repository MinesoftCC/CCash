#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <drogon/HttpClient.h>
#include "BankAPI.h"

int main(int argc, char *argv[])
{
        if (argc != 3)
        {
                std::cout << "Usage: sudo ./CCash <bank password> <run in background (true or false)>\n";
                return 0;
        }
        if (geteuid() != 0)
        {
                std::cout << "ERROR: CCash MUST be ran as root\n";
                return 0;
        }
        std::cout << "process started, PID: " << getpid() << "\n\n";
        auto primary_bank = std::make_shared<BankAPI>(argv[1]);
        app().setLogPath("./").setLogLevel(trantor::Logger::kDebug).addListener("0.0.0.0", 80).registerController(primary_bank);

        if (argv[2] == "true")
        {
                app().enableRunAsDaemon();
        }

        auto handlerInfo = app().getHandlersInfo();
        for (auto &info : handlerInfo)
        {
                std::cout << std::get<0>(info);
                switch (std::get<1>(info))
                {
                case Get:
                        std::cout << " (GET) ";
                        break;
                case Post:
                        std::cout << " (POST) ";
                        break;
                case Delete:
                        std::cout << " (DELETE) ";
                        break;
                case Put:
                        std::cout << " (PUT) ";
                        break;
                case Options:
                        std::cout << " (OPTIONS) ";
                        break;
                case Head:
                        std::cout << " (HEAD) ";
                        break;
                case Patch:
                        std::cout << " (PATCH) ";
                        break;
                default:
                        break;
                }
                std::cout << '\n';
        }
        app().run();

        return 0;
}
