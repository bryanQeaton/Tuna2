#ifndef TUNA2_MOVE_ORDERING_TESTING_H
#define TUNA2_MOVE_ORDERING_TESTING_H
#include <chrono>
#include <cstdint>
#include <string>

#include "chess.hpp"
#include "search.h"

inline std::string fens[]={
"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
"rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
"r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq - 0 3",
"r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/2N2N2/PPP2PPP/R1BQKB1R b KQkq - 1 4",
"r1bqk2r/pppp1ppp/2n2n2/4p3/3PP3/2N2N2/PPP2PPP/R1BQKB1R w KQkq - 2 5",
"r1bqk2r/pppp1ppp/2n2n2/4p3/3PP3/2N2N2/PPP1BPPP/R1BQK2R b KQkq - 3 5",
"r1bq1rk1/pppp1ppp/2n2n2/4p3/3PP3/2N2N2/PPP1BPPP/R1BQ1RK1 w - - 4 6",
"r2q1rk1/ppp2ppp/2np1n2/4p3/3PP3/2N2N2/PPP1BPPP/R1BQ1RK1 w - - 0 7",
"r2q1rk1/ppp2ppp/2np1n2/4p3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 b - - 1 7",
"r2q1rk1/pp3ppp/2np1n2/2p1p3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 w - c6 0 8",
"rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1",
"rnbqkbnr/pppp1ppp/8/4p3/3P4/4P3/PPP2PPP/RNBQKBNR b KQkq - 0 2",
"rnbqkbnr/pp1p1ppp/2p5/4p3/3P4/4P3/PPP2PPP/RNBQKBNR w KQkq - 0 3",
"rnbqkbnr/pp1p1ppp/2p5/4p3/3P4/2N1P3/PPP2PPP/R1BQKBNR b KQkq - 1 3",
"r1bqkbnr/pp1p1ppp/2np4/4p3/3P4/2N1P3/PPP2PPP/R1BQKBNR w KQkq - 2 4",
"r1bqkbnr/pp1p1ppp/2np4/4p3/3P4/2N1PN2/PPP2PPP/R1BQKB1R b KQkq - 3 4",
"r1bqk2r/pp1p1ppp/2np1n2/4p3/3P4/2N1PN2/PPP2PPP/R1BQKB1R w KQkq - 4 5",
"r1bqk2r/pp1p1ppp/2np1n2/4p3/3P4/2N1PN2/PPP1BPPP/R1BQK2R b KQkq - 5 5",
"r1bq1rk1/pp1p1ppp/2np1n2/4p3/3P4/2N1PN2/PPP1BPPP/R1BQ1RK1 w - - 6 6",
"r2q1rk1/pp1p1ppp/2np1n2/2b1p3/3P4/2N1PN2/PPP1BPPP/R1BQ1RK1 w - - 7 7",
"r3k2r/pppq1ppp/2npbn2/4p3/2BPP3/2N2N2/PPP2PPP/R2Q1RK1 w kq - 0 10",
"r1bq1rk1/pppn1ppp/3p1n2/3Pp3/2P1P3/2N2N2/PP3PPP/R1BQ1RK1 w - - 0 11",
"r2q1rk1/pp2bppp/2np1n2/2p1p3/2P1P3/2N2N2/PP2BPPP/R1BQ1RK1 w - - 0 12",
"r2q1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N2N2/PP2BPPP/R1BQ1RK1 w - - 0 13",
"r2q1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N2N2/PPQ1BPPP/R1B2RK1 b - - 1 13",
"r2q1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N2N2/PPQ1BPPP/R1B2RK1 w - - 2 14",
"r1bq1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N2N2/PPQ1BPPP/R1B2RK1 w - - 3 15",
"r1bq1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N1BN2/PPQ2PPP/R1B2RK1 b - - 4 15",
"r1bq1rk1/pp1bbppp/2np1n2/2p1p3/2P1P3/2N1BN2/PPQ2PPP/R1B2RK1 w - - 5 16",
"8/5pk1/6p1/5p2/5P2/6P1/5PK1/8 w - - 0 1",
"8/6k1/5pp1/5p2/5P2/5PP1/6K1/8 w - - 0 1",
"8/6k1/6p1/5p2/5P2/6P1/6K1/8 w - - 0 1",
"8/7k/6p1/5p2/5P2/6P1/6K1/8 w - - 0 1",
"8/6k1/6p1/5p2/5P2/6P1/7K/8 w - - 0 1",
"8/6k1/5pp1/5p2/5P2/5PP1/7K/8 w - - 0 1",
"8/6k1/5pp1/5p2/5P2/5PP1/6K1/8 b - - 1 1",
"rnbq1rk1/ppp2ppp/3b1n2/3pp3/3PP3/2N2N2/PPP2PPP/R1BQ1RK1 w - - 0 8",
"rnbq1rk1/ppp2ppp/3b1n2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 b - - 1 8",
"rnbq1rk1/ppp2ppp/3b1n2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 w - - 2 9",
"r3k2r/ppp2ppp/2n1bn2/3pp3/3PP3/2N2N2/PPP2PPP/R2Q1RK1 w kq - 0 10",
"r3k2r/ppp2ppp/2n1bn2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 b kq - 1 10",
"r3k2r/ppp2ppp/2n1bn2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 w kq - 2 11",
"rnbqkbnr/pppppppp/8/8/4P3/7N/PPPP1PPP/RNBQKB1R b KQkq - 1 1",
"rnbqkbnr/pppppppp/8/8/3PP3/8/PPP2PPP/RNBQKBNR b KQkq - 0 1",
"rnbqkbnr/pppppppp/8/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
"rnbqkbnr/pppp1ppp/8/4p3/4P3/3N4/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
"r1bqkbnr/pppp1ppp/2n5/4p3/4P3/2N5/PPPP1PPP/R1BQKBNR b KQkq - 2 3",
"r1bqkbnr/pppp1ppp/2n5/4p3/2BPP3/5N2/PPP2PPP/RNBQK2R b KQkq - 3 4",
"r1bq1rk1/pppp1ppp/2n2n2/4p3/3PP3/5N2/PPP1BPPP/RNBQ1RK1 w - - 4 6",
"r2q1rk1/ppp2ppp/2np1n2/4p3/3PP3/5N2/PPP1BPPP/RNBQ1RK1 w - - 0 7",
"r2q1rk1/ppp2ppp/2np1n2/2b1p3/3PP3/5N2/PPP1BPPP/RNBQ1RK1 w - - 1 7",
"r2q1rk1/pp3ppp/2np1n2/2p1p3/3PP3/5N2/PPP1BPPP/RNBQ1RK1 w - - 2 8",
"8/5pk1/6p1/5p2/5P2/5KP1/8/8 w - - 0 1",
"8/6k1/5pp1/5p2/5P2/6P1/5K2/8 w - - 0 1",
"8/6k1/6p1/5p2/5P2/5KP1/8/8 w - - 0 1",
"8/7k/6p1/5p2/5P2/5KP1/8/8 w - - 0 1",
"8/6k1/6p1/5p2/5P2/5KP1/8/8 b - - 1 1",
"r1bq1rk1/ppp2ppp/3b1n2/3pp3/3PP3/2N2N2/PPP2PPP/R1BQ1RK1 b - - 1 8",
"r1bq1rk1/ppp2ppp/3b1n2/3pp3/3PP3/2N1BN2/PPP2PPP/R2Q1RK1 w - - 2 9",
"r3k2r/ppp2ppp/2n1bn2/3pp3/3PP3/2N2N2/PPP2PPP/R2Q1RK1 b kq - 1 10",
};

inline void mo_test(const int depth=8) {
    uint64_t nodes=0;
    auto t0=std::chrono::high_resolution_clock::now();
    for (const auto &fen:fens) {
        auto pos=chess::Board(fen);
        const auto sr=search(pos,-BOUND,BOUND,depth,0,false,false,chess::Move());
        std::cout<<sr.nodes<<" | "<<fen<<"\n";
        nodes+=sr.nodes;
    }
    auto t1=std::chrono::high_resolution_clock::now();
    auto time=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
    std::cout<<"#=========================#\ntime:"<<time<<"\n";
    std::cout<<"nodes:"<<nodes<<"\n";

}





#endif //TUNA2_MOVE_ORDERING_TESTING_H
