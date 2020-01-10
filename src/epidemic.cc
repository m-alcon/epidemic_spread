#include "network.h"

int main() {
    Network net (10000, 20, 50, 0.5, 0.2);
    net.write(cout);
    return 0;
}