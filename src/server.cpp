#include "server.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <thread>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port) : port_(port) {}

static void handleClient (int clientFd, Store& store) {
    char buffer[4096];
    
    while (true) {
        ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            break;
        }

        buffer[bytesRead] = '\0';

        std::istringstream iss(buffer);
        std::string command;
        iss >> command;

        std::string response;
        if (command == "SET") {
            std::string key, value;
            iss >> key >> value;
            store.set(key, value);
            response = "OK\n";
        }
        else if (command == "GET") {
            std::string key;
            iss >> key;
            auto result = store.get(key);
            response = result ? (*result + "\n") : "(nil)\n";
        }
        else if (command == "DEL") {
            std::string key;
            iss >> key;
            bool removed = store.del(key);
            response = removed ? "1\n" : "0\n";
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            response = "ERROR: unknown command\n";
        }

        write(clientFd, response.c_str(), response.size());
    }
    std::cout << "Client disconnected" << std::endl;
    close(clientFd);
}

void Server::run (Store& store) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        std::cerr <<"Failed to create socket." << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if(bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed on port. " << port_ << std::endl;
        close(serverFd);
        return;
    }

    if (listen(serverFd, 10) < 0) {
        std::cerr << "Listen Failed. " << std::endl;
        close(serverFd);
        return;
    }

    std::cout << "kv_store listening on port " << port_ << std::endl;

    while(true) {
        int clientFd = accept(serverFd, nullptr, nullptr);
        if (clientFd < 0) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }
        std::cout << "Client connected." <<std::endl;

        std::thread worker(handleClient, clientFd, std::ref(store));
        worker.detach();
    }
}