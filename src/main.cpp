#include <iostream>
#include "store.h"
#include "server.h"

int main () {
    Store store;
    Server server(6380);
    server.run(store);
    return 0;
}