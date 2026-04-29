#include <chrono>
#include <iostream>
#include <thread>


#include "chess.hpp"
#include "move_ordering_testing.h"
#include "perft.h"
#include "uci.h"


int main() {
    uci();

    auto pos=chess::Board("k7/8/8/8/8/8/4P3/K7 w - - 0 1");
    mo_test();



    return 0;
}
