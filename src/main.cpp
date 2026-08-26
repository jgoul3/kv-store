#include <iostream>
#include "store.h"
#include "server.h"

int main () {
    Store store;
    Server server(6830);
    server.run(store);
    return 0;
}