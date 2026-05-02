#include <chrono>
#include <iostream>
#include <thread>


#include "chess.hpp"
#include "move_ordering_testing.h"
#include "perft.h"
#include "Tuner.h"
#include "uci.h"


int main() {
    uci();
    //mo_test();



    auto pos=chess::Board(chess::constants::STARTPOS);
    std::cout<<see_move(pos,chess::uci::uciToMove(pos,"e2e4"));
    //tune_mo_tables(50,4,2);
    //Tuner::tune(2000,25,0,1,10000);




    return 0;
}
