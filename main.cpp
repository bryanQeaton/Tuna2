#include <chrono>
#include <iostream>
#include <thread>


#include "chess.hpp"
#include "move_ordering_testing.h"
#include "perft.h"
#include "uci.h"


int main() {
    //mo_test();
    uci();

    //auto pos=chess::Board("r1bqkb1r/ppp1pppp/2n2n2/3N4/3P4/5N2/PPP1PPPP/R1BQKB1R b KQkq - 0 4");
    //std::cout<<evaluation(pos)<<"\n";
    //search(pos,-BOUND,BOUND,8,0,false,false,chess::Move());

    return 0;
}
