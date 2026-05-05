#include <chrono>
#include <iostream>
#include <thread>


#include "chess.hpp"
#include "move_ordering_testing.h"
#include "perft.h"
#include "uci.h"


int main() {
    uci();
    //mo_test();
    // auto pos=chess::Board(chess::constants::STARTPOS);
    // std::cout<<attacks_from(pos,chess::Square::SQ_E3);

    return 0;
}
