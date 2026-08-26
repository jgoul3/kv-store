#pragma once
#include "store.h"

class Server {
public: 
    explicit Server (int port);
    void run(Store& store);

private:
    int port_;
};