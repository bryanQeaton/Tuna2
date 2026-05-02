#ifndef TUNA2_MOVE_ORDERING_TESTING_H
#define TUNA2_MOVE_ORDERING_TESTING_H
#include <chrono>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <random>
std::mt19937 gen(std::random_device{}());

#include "chess.hpp"
#include "search.h"

inline std::string fens[]={
"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
"rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
"r2qkr2/p1pp1ppp/1pn1pn2/2P5/3Pb3/2N1P3/PP3PPP/R1B1KB1R b KQq -",
"r4rk1/3bppb1/p3q1p1/1p1p3p/2pPn3/P1P1PN1P/1PB1QPPB/1R3RK1 b - -",
"4Q3/8/8/8/6k1/4K2p/3N4/5q2 b - -",
"r4rk1/1Qpbq1bp/p1n2np1/3p1p2/3P1P2/P1NBPN1P/1P1B2P1/R4RK1 b - -",
"r1bqk2r/2p2ppp/2p5/p3pn2/1bB5/2NP2P1/PPP1NP1P/R1B1K2R w KQkq -",
"8/8/4kp2/8/5K2/6p1/6P1/8 b - -",
"r4rk1/3p2pp/p7/1pq2p2/2n2P2/P2Q3P/2P1NRP1/R5K1 w - -",
"2rqk1n1/p6p/1p1pp3/8/4P3/P1b5/R2N1PPP/3QR1K1 w - -",
"1r4k1/2qb1pb1/2p2P1p/8/p7/N1BB3P/P5P1/2Q2R1K b - -",
"R7/1r6/5p2/8/P4k2/8/1p6/4K3 w - -",
"r3k2r/1p1nqp2/p2p2pp/2pP4/6QN/2N5/PPP2PPP/R1B3K1 w kq -",
"6KN/8/8/q7/5k2/8/8/8 w - -",
"4r1k1/p2P1ppp/P7/R3Pp2/5b2/3K1q2/1r3P2/3R4 w - -",
"8/1k6/1P6/3R3p/6p1/6r1/8/4K3 w - -",
"2rq1rk1/5pbp/p3p1p1/1p1PPn2/5P2/P1NQ2P1/1B5P/R4RK1 b - -",
"4rrk1/1pp4p/p2pb1pq/8/2Pn4/2NB3P/PP3PP1/1R2R1K1 b - -",
"2kr4/Qp3p1p/6p1/2PnB1q1/3N4/P7/5PPP/4R1K1 b - -",
"1k2q2r/p1p1bpp1/1pn1p2p/4P2B/2P1b3/4BN2/PP2QPPP/3R2K1 b - -",
"b7/2p2ppk/1p2p2p/p3P2P/P3pK2/1P2P3/1bP1BPP1/3R4 b - -",
"r1b1kb1r/pp4pp/2n1pn2/8/3N1B2/5N2/Pq2PPPP/R2QKB1R w KQkq -",
"8/7P/5k2/p1K5/P7/1P5r/8/8 w - -",
"r1b1k2r/pp1p1p1p/2n2np1/2q5/2PP4/8/PBP1BPPP/1R1QK2R b Kkq -",
"r2q1rk1/1pnbbpp1/p1np3p/P1p1p3/4P3/1P1P1NP1/2PB1PBP/R2Q1RK1 w - -",
"2r3k1/r4ppp/1q1bb3/8/3p3P/1P3B2/P2BQPP1/RR4K1 b - -",
"8/8/4k1p1/2r5/2P1R1KP/4P3/5P2/8 b - -",
"3rk2r/pb2ppb1/1pp2npp/8/2P4B/2N3P1/PP2PPBP/2R2RK1 b k -",
"8/8/3k4/8/8/3KP3/2Q5/8 w - -",
"r1b1kb1r/pp2pppp/1q3n2/3p4/PP6/2P2N2/3NPPPP/R2QKB1R b KQkq -",
"r1b2rk1/1p2b1pp/p2qp3/P3np2/8/2PP2N1/5PPP/RBBQR1K1 b - -",
"2bb1rk1/1p4p1/2p1p2p/8/Q3P3/P4N2/1P2NPPP/6K1 b - -",
"4r1k1/7p/pp2N3/2pR1p2/P1p5/2P4P/1P4P1/6K1 w - -",
"4n1k1/2p2pp1/2P4p/8/1N1p1P1P/Pp4P1/1P3P2/6K1 w - -",
"8/3N2k1/1p2p2p/8/2pP1p2/r6P/3r1P2/5RK1 w - -",
"3rr1k1/1pp1bpp1/4q2p/nP1pP3/7P/1QPR1NB1/5PP1/3R2K1 w - -",
"r4rk1/ppqn2pp/2pb1n2/3pp3/8/3P2P1/PPPN1PBP/R1BQK2R w KQ -",
"r1bqkb1r/1p2nppp/p1n5/2ppP3/8/2PB1N2/PP1P1PPP/R1BQ1RK1 b kq -",
"8/1n4k1/8/3K4/1P6/3N4/8/8 w - -",
"2r2b2/2q3k1/2n1bppp/pB1p4/N2P4/4P3/PR3PP1/1Q3RK1 b - -",
"r1b2rk1/pp2bppp/8/1N1Pp3/2p1P3/PnN1q3/RPQ1B1PP/5RK1 w - -",
"8/1p6/2b5/2b5/2P5/8/2K2k2/8 b - -",
"r4rk1/5p1p/4p1p1/p1p1B1b1/PpQ5/1P1P4/2P2PKP/R3R3 b - -",
"3rkb1r/2p2ppp/p4n2/1p2p3/2BnPP2/P1N1B3/1PP2P1P/R1K4R w k -",
"r3kb1r/1pp1nppp/2n5/1p2P3/4N1b1/4B1P1/PPP2P1P/R3K1NR w kq -",
"r4rk1/pbpqbppn/1pn1p3/3p4/P1PP1B2/4PN2/1P1N1PPP/R2Q1RK1 b - -",
"8/1p4k1/8/1n2P3/5P2/p2K4/5r2/R7 w - -",
"r4rk1/1pp3pp/8/p1nPR2Q/P7/1P5P/2P2qP1/3R2K1 w - -",
"r1bqk2r/pp1n1pp1/3bpn2/3p3p/3P3P/5NP1/PP1NPPB1/R1BQK2R b KQkq -",
"8/5kp1/1K5p/8/6P1/8/8/8 b - -",
"2r5/1p1kb1p1/4pP2/pP1p2p1/1n1P4/1N2B3/1P3PPP/5RK1 b - -",
"r4rk1/pp2qp1p/2np1p2/3p2N1/3P4/2P1P1P1/PP4BP/RN1b1RK1 w - -",
"8/8/4K3/8/8/8/6k1/8 w - -",
"8/8/4p1k1/2p3p1/8/8/1q6/4K3 b - -",
"8/5Q2/8/5K2/3p1P1Q/p7/2k5/8 w - -",
"r3r1k1/2p1np2/pp1p3p/6p1/3PN3/2PBP2P/P5P1/1R1Q2K1 w - -",
"8/k7/5K2/8/5Q2/8/8/8 b - -",
"r5r1/2p2pk1/1pb4p/p3R3/P1P1p3/1P4P1/2PN2KP/R7 b - -",
"r2q1rk1/1p2bppp/p2pbn2/8/1n2P3/1NNP4/1B2BPPP/R2Q1RK1 w - -",
"8/8/4p3/2p3P1/2P1k1P1/8/3K4/8 b - -",
"r1b1k1nr/pp3ppp/2pqp3/3n4/3P4/5N2/PPP1QPPP/R3KB1R w KQkq -",
"8/2R2B2/6k1/4r3/7K/2pb4/8/8 b - -",
"r5k1/1ppb3p/p2P2P1/3p1p2/8/1QPB3P/PP1b2P1/R5K1 b - -",
"8/6K1/8/8/8/3k4/8/8 w - -",
"8/p7/1p2k3/2p5/b1P1BP1B/3K4/1b6/8 w - -",
"8/8/5b2/1P1k4/8/4R3/K7/7q w - -",
"8/8/6k1/7p/5K1P/5PP1/8/5R2 w - -",
"r1bq1rk1/pp1nbpp1/7p/2p1p3/P3P3/2N5/1PPBBPPP/R2Q1RK1 w - -",
"3R4/6p1/4p1P1/n1p1P1k1/8/4K1p1/8/8 w - -",
"r5k1/pB1b1ppp/3qpb2/8/3n1N2/6P1/4PP1P/2BQ1RK1 b - -",
"5r2/6k1/8/6p1/1p2pb1P/1P5b/2P1BK2/6R1 w - -",
"8/4k3/5b1p/6pP/8/1Bp2P2/4K3/8 b - -",
"r1bq1rk1/pp1n2p1/3bpn1p/3p1p2/3P4/1N1Q1NP1/PP2PP1P/R1B2RKB b - -",
"8/2R5/8/4PKPk/8/5r2/8/8 w - -",
"2r3k1/pp2p1b1/8/3q3P/3p3p/3P3P/PP1N1r1K/R2QR3 w - -",
"rn1qk2r/ppp1bppp/8/3pP3/3P4/2N2N2/PPQP2PP/R1B1K2R b KQkq -",
"4Q3/8/p2k4/7P/5RP1/2K5/P7/8 b - -",
"r4r2/q4pkp/p2b4/3p4/P4P2/3QP2P/P5P1/3R1RK1 w - -",
"k7/8/5P1K/8/8/8/5q2/7q w - -",
"2r3k1/pp3p1p/2B1b1p1/2n1p3/8/2N4P/PPP2PP1/2KR4 w - -",
"R7/5k2/3p1p2/2p1pbpP/8/2K1PP2/8/8 w - -",
"3r4/7k/7p/2B3p1/8/5nPP/8/2R4K w - -",
"rq2r1k1/pbp4p/1p4p1/3n1p2/PPQPnP2/5N2/1B3PBP/R4RK1 b - -",
"r1bqk3/ppp2ppr/5p2/8/1P1N4/8/PP3PPP/R1BQK2R b KQq -",
"8/8/p5pk/2R5/1nP2p1P/4r3/4BK2/8 w - -",
"8/2R5/7k/4r3/5RPP/3K4/8/8 b - -",
"1q5r/4k2p/2pbbpr1/3ppp1Q/8/P1P1PP1P/4B1P1/1NKR2R1 b - -",
"r2q1rk1/p1p2p2/2p2bp1/4p2p/N3P3/2P2QPP/PP3P2/R3K2R w KQ -",
"2kr2r1/p1p1bp2/2p1b2p/6P1/P3Q3/2PP3P/q2B1P2/2KR1BR1 w - -",
"8/5R1k/3P3p/8/2P4P/1p6/r7/4K3 b - -",
"4nr2/6pp/pkp3b1/2p5/8/2P2N1P/PP1N1PP1/R5K1 w - -",
"8/5Nk1/8/p4p1P/5K2/1P6/P7/8 w - -",
"r1bqk2r/ppp1bp1p/2np1n2/6N1/2P3p1/6P1/PP1PPPBP/R1BQK1NR b KQkq -",
"r4rk1/pp1bqppp/2pb1n2/3p4/2PPp3/PPN1P3/1B2BPPP/R2Q1RK1 w - -",
"r2qk2r/pb1pbppp/1pn1p3/1Np5/2P5/1P3N1P/PB1PBRP1/R2Q2K1 b kq -",
"r2q1rk1/p1p1bppp/4p3/2np4/5Pb1/4PN1P/PP1QB1P1/RN3RK1 b - -",
"3r2k1/pp4p1/6qp/8/P1b5/5P1P/4NP1K/4R2R w - -",
"r2qkb1Q/p4p1p/2p3p1/3p4/6b1/2P2NP1/P1P2PBP/R3K2R b KQq -",
"rnbq1rk1/p3bppp/5n2/2pp4/8/2N1PN2/PP2BPPP/R1BQ1RK1 w - -",
"1r3rk1/3bqpp1/1ppp3p/p7/P1PPPnP1/4NB2/2QB1PP1/1R3RK1 b - -",
"8/1p4k1/p7/b7/4P1Bp/5K2/2P5/8 w - -",
"r1bqk2r/pppp1ppp/5n2/4n3/2PP4/P5P1/5P1P/R1BQKBNR b KQkq -",
"8/8/5k2/8/5P2/1K6/8/3R4 w - -",
"r4rk1/pBp3bp/4bnp1/4pp2/2P5/P1N3P1/1P2PP1P/R1BR2K1 b - -",
"r2q1rk1/pp1bbp2/3p1n1p/2pP4/2P5/6P1/PP2PPBP/R2QK1NR w KQ -",
"rnbqk1nr/ppp2pbp/6p1/3pPp2/3P4/5N2/PPP3PP/RN1QKB1R w KQkq -",
"5r1k/2pbq1pp/8/2p2p2/P2P4/2P2R2/Q5PP/6K1 w - -",
"r3kb1r/1p1n1ppp/p2p4/2pPp3/4n1Bq/2P5/PP3PPP/RNBQ1RK1 w kq -",
"r1bq1rk1/pp2b1pp/2n1pp2/2p5/2P3n1/PPN2NP1/4PPBP/R1BQ1RK1 b - -",
"6k1/1p4pp/p7/3r1p2/8/1P4P1/P6P/3K1B2 w - -",
"r1b1kbnr/1p2nppp/pq2p3/3P4/3p1P2/2N2N2/PPP1B1PP/R1BQK2R w KQkq -",
"r2qkb1r/pp3ppp/2n1n3/2p5/4N3/3P4/PP3PPP/2RQKB1R w Kkq -",
"8/8/3b4/p6B/7P/4P1P1/1Bbk1K2/8 b - -",
"r1bq1rk1/pp2ppb1/3p1np1/6N1/2Pn4/2N4P/PP2BPP1/R3QRK1 w - -",
"5r2/8/1pR5/8/1p6/1r2k1P1/3R2K1/8 w - -",
"6RQ/6P1/p7/6K1/4k3/5r2/8/8 b - -",
"r1bq1rk1/pp1n2bp/2p3p1/4pp2/2P5/P1P1BNP1/4PPBP/R2Q1RK1 b - -",
"5k2/8/3p4/1P1P1p2/B2p1K1P/3P4/8/8 b - -",
"6k1/p2pqpp1/5n1p/1Qp5/8/n5P1/P3PP1P/5RK1 w - -",
"8/pp6/6k1/4Rp2/P7/1P2K3/2P2P2/3r4 w - -",
"r1bqk2r/1p1n1pbp/p2p2p1/2pP3n/4P3/P1N2N1P/1PP2PP1/R2QKB1R w KQkq -",
"r1bq1rk1/1p1nbppp/p1n1p3/2ppP3/3P4/P1P2N1P/1P1NBPP1/R1BQ1RK1 w - -",
"r3kb1r/pp1npppp/8/3p1b2/3P1B2/4P2P/PP1K1PP1/2R2BNR w kq -",
"8/P4k2/2R5/5K2/8/3r4/1p6/8 b - -",
"r2q1rk1/p4pb1/npp2ppp/3p4/3P2b1/4QNP1/PPPNPPBP/R4RK1 w - -",
"r1b4r/ppp1kpp1/5n1p/8/1n1P4/2NB4/PP2KPPP/3R2NR w - -",
"8/5p2/3b1k2/8/4P1p1/8/8/1K6 b - -",
"8/5pk1/2P2bpp/r3p3/4P3/1P3PP1/4BK1P/3R4 b - -",
"r2qkb1r/pQ1npppb/5n1p/3p4/3P1BP1/2N1PN1P/PP2BP2/R3K2R b KQkq -",
"8/8/2k4P/4NK2/2r5/5P2/p7/8 b - -",
"7k/8/6Q1/3K2P1/7q/8/8/8 w - -",
"8/8/6KP/3R4/8/1k6/8/8 b - -",
"rn2k2r/2p1bppp/2b5/pp2p3/P5n1/2N2N2/1PP1PPBP/R1BR2K1 b kq -",
"r1bqk2r/pppn1pp1/7p/3P4/1b1P4/5NP1/PP1N1PP1/R2QKB1R b KQkq -",
"8/8/6R1/7P/3k4/6K1/8/r7 w - -",
"8/8/1K6/8/R7/8/1k1n4/8 w - -",
"4r1k1/ppp2p2/6p1/3R3p/5P2/8/PPP3PP/5RK1 w - -",
"8/7K/8/5Q2/5P2/k7/8/8 w - -",
"2r4k/1B5p/p3p3/P4p1P/7q/2r5/5PP1/3RR1K1 b - -",
"8/5pk1/8/3b4/p6P/1p6/1Br1RKP1/8 b - -",
"r2qk2r/p1p2pb1/2np1npp/1P2p3/4P3/2NP1BPP/PP3P2/R1BQK2R b KQkq -",
"3rb1k1/p6p/6p1/2pP1p2/2B2p2/P6P/5PP1/1R4K1 b - -",
"5rk1/pb4p1/2n4p/1NQ5/4p3/2P1P1qP/PP4B1/3R2K1 b - -",
"2Q5/8/4p3/1p1pk3/7R/5K2/3r4/8 b - -",
"8/p6k/1p1p4/2pB1p2/2Pn4/3P2Q1/PP6/7K b - -",
"rnbq1rk1/pp4pp/2pb1n2/3p1p2/P2P4/6P1/1P1BPPBP/RN1QNRK1 b - -",
"2r2rk1/ppN1nppp/5q2/8/3p4/3B4/PPPQ1PPP/3R2K1 w - -",
"8/5R2/5K2/8/4r3/5k2/8/8 w - -",
"rnb1k2r/2p1bppp/1p2pn2/pP6/3NP3/P1NB4/5PPP/R1BQK2R b KQkq -",
"r1b3k1/4b3/p6p/n7/p3Np2/3B1N1P/1q2QPP1/R4RK1 b - -",
"8/8/k7/b6Q/8/1K6/1Q6/8 w - -",
"3q1rk1/p4p2/1p2b1pp/4Q3/4B3/P5P1/1P3P1P/2R3K1 b - -",
"2r2rk1/pp2pp1p/3q2p1/3p1b2/3R4/1PB3PP/P3PPB1/4QRK1 b - -",
"8/8/6k1/5R2/6r1/8/5K2/8 w - -",
"rn1qkb1r/pp2pp1p/6p1/1b2P3/8/2Q5/PP3PPP/R1B1K1NR w KQkq -",
"rnbqk2r/1p2bpp1/p3pn1p/2PpN3/3P1B2/3BP3/P1P2PPP/RN1Q1RK1 w kq -",
"rnbqk2r/1pp2ppp/4pn2/pPB5/2P5/P4N2/3PPPPP/RN1QKB1R b KQkq -",
"r1br2k1/pp2bppp/2nR4/4p3/1PN1P3/2P2N2/P1n2PPP/4KB1R w K -",
"r2r4/pp2N1kp/5pp1/2P5/PP1b4/3R4/2P3PP/1R4K1 w - -",
"r2qkbnr/pp3pp1/6b1/3pB3/6p1/1P1P3P/P2NPP2/R2QKB1R w KQkq -",
"rn3rk1/pb5p/4q1pQ/1Bpp4/4pP2/1P2P3/P2N2PP/R4RK1 b - f3",
"r2qk2r/pp1nbppp/5n2/4p3/8/4BP1P/PPP2PP1/R2QKB1R w KQkq -",
"4b1k1/8/6pp/8/2Pq1PQ1/1P5P/6K1/8 w - -",
"8/8/1p1p1k2/3P4/2P5/8/2K5/8 b - -",
"8/1p4p1/4k2p/2p4P/P1P2PP1/3K4/8/8 b - -",
"8/7B/1p1k1p2/3p3p/7P/1Pb2K2/5P2/8 b - -",
"8/1pbr2p1/2p2pkp/8/1P1Bb3/4P1PP/4BP2/2R3K1 b - -",
"rkr5/1b3ppQ/pb2p3/4q3/3N4/1PP2PP1/PB1P2KP/5R2 b - -",
"8/6R1/2b3K1/8/7k/8/8/4r3 w - -",
"6k1/8/8/8/3b1q2/5P2/8/6K1 w - -",
"r2qkbnr/pp2pppp/2n5/2p5/2B2Pb1/2N2pP1/PPPP3P/R1BQK2R w KQkq -",
"rnbq1rk1/1pp1n1b1/1p1p2pp/3Np3/2P1Pp2/3P1N2/PP2BPPP/R2QK2R w KQ -",
"r2qk1nr/pp1n2pp/5p2/2bp4/5B2/4P3/PPP2PPP/RN2K1NR w KQkq -",
"r1bq1rk1/ppp2pB1/2np2pp/8/2P3P1/1P1Bn2P/P4P2/R2QK1NR w KQ -",
"8/5k2/1R6/5K2/8/8/8/6r1 b - -",
"5r1k/1pp3p1/p2p4/8/3N4/8/bPP3PP/6K1 w - -",
"r1bq1rk1/pp2bppn/4p3/2np4/5B2/2P1PN2/PPQ2PPP/RN3RK1 w - -",
"8/8/8/7P/8/1k6/7B/5QK1 b - -",
"6R1/6K1/2k5/8/8/8/8/8 w - -",
"8/8/P7/1r3p1p/2kbK2P/4P1P1/5P2/8 w - -",
"2r3k1/1p4bR/2p1p1p1/Pp3rP1/3PpP2/2B4R/5K2/8 w - -",
"5k2/8/1pB5/6KP/P7/8/8/8 b - -",
"8/8/6p1/8/3Kbk2/1p6/7p/1r6 w - -",
"r4rk1/pp1b2pp/1b1B4/3P4/4Pp2/1P3R2/1P2p1PP/2R4K b - -",
"r3kbnr/ppp2ppp/2n5/3p1B2/8/P1P2P2/4KPPP/RNB4R b kq -",
"2rr2k1/p4pb1/1p4p1/3b3p/P6P/2P2P2/3B1P2/1R2K2R w K -",
"r3kb1r/pb3ppp/2n1pq2/2p5/Q7/2P2N2/PP3PPP/R1B1KB1R w KQkq -",
"8/5R2/3Q4/2P4p/4k1r1/8/1P3P1K/8 b - -",
"r1bq1rk1/1pp1bpp1/p4p1p/3p4/P2P3P/5NP1/2P1PPB1/RNQ2RK1 b - -",
"3r1rk1/1pp1bp2/2n4p/4p1pq/Q7/2NPB1P1/PP2PP1P/R4RK1 w - -",
"5k2/8/p3p3/R6p/5PpP/4K3/8/8 b - -",
"6k1/2rq4/2p3pp/1p1p1p2/p1nP1P2/P2N2P1/1P1nR1BP/2R3K1 b - -",
"r2qk2r/pb2bpp1/1pnppn1p/2p5/2PP1B2/P1N2NP1/1P2PP1P/R2QKR1B b Qkq -",
"2k4r/p1p5/1pP2pp1/4pn1p/1B4nP/2P2N2/P1P1QPP1/1K1R4 w - -",
"r1bq1rk1/ppp2pp1/2n1pn1p/1NP5/3P4/1P6/P3B1PN/R1BQ1RK1 b - -",
"7r/Bppn1pbp/5p2/3k2p1/8/8/PP1N1PPP/4R1K1 b - -",
"r2qkb1r/pbpp1ppp/1p2pn2/8/1n5P/1P3NP1/PBPPPPB1/RN1QK2R b KQkq -",
"1r1k1r2/p3ppb1/2p2npp/8/P1B5/2N5/1Pb2PPP/R1B1R1K1 w - -",
"r5k1/5p2/2p3pp/q2p4/8/6P1/3Q1P1P/4R1K1 w - -",
"2bqkb1r/1ppp1ppp/4pn2/4N3/1r6/2N1P3/PBPPBPPP/1R1QK2R b Kk -",
"3r4/5kpp/2p5/6R1/n1P1K3/8/P1P2P1P/8 b - -",
"8/8/2K5/8/4k3/1q6/r7/8 b - -",
"8/3k4/8/6R1/5p1p/1n1K1B1P/6P1/8 b - -",
"8/8/8/8/7p/4k1n1/8/2K5 b - -",
"rn1q1rk1/pbp1bppp/1p2p3/3p4/1P1P4/2P1PN2/P3BPPP/R2Q1NK1 b - -",
"2rq1rk1/1b2p3/1p3p1p/p2p4/3P4/4P1PP/PP2P1BK/R2Q1R2 w - -",
"5R2/8/p7/2K3k1/1P6/8/8/7r w - -",
"rnb1r1k1/pp3ppp/2p2N2/2bqp3/8/6P1/PPPPQPBP/R1B1K1NR b KQ -",
"8/8/3r3k/6p1/4pp2/1p6/5P2/Q4K2 w - -",
"rn2k2r/1p3ppp/2pqpn2/p2p4/3P4/1P2PN2/PB2BPPP/1R1Q1RK1 w kq -",
"rnbq1rk1/1p2ppbp/p2p1np1/3P4/1PP5/2N2N2/PB3PPP/R2QKB1R b KQ -",
"rn4k1/p3p3/2p1p1pp/7B/8/8/NPP2PPP/4RbK1 b - -",
"8/8/8/7K/3b1kP1/8/8/1B6 b - -",
"8/1p3kB1/p7/P7/1b2R1p1/1K2P1P1/5P2/8 b - -",
"6k1/5p1p/N5p1/8/8/b3rP1P/6P1/3R2K1 w - -",
"r4rk1/1b3p1p/1p1p2p1/pPnPp1Nn/4P3/1P4PP/P1B2P2/R4RK1 b - -",
"rn1k1b1r/pb3p1p/1p1p4/2pP1p2/2P5/8/PP3PPP/RQ2KB1R b KQ -",
"r1bq1rk1/ppp2p1p/6p1/4n3/4N3/8/PPP1QPPP/R1B2RK1 w - -",
"r2qrnk1/1p3pp1/2p2n1p/p7/2b4N/2N1P1P1/P1QB1PBP/1R3RK1 w - -",
"5Q2/8/4K3/8/2k1P3/5R2/8/8 w - -",
"3q1rk1/1b2bppp/pp2p3/6N1/P2P4/1P6/5PPP/2nR2K1 w - -",
"4k3/8/8/6P1/3B1P2/2K5/7P/8 b - -",
"r4rk1/1q3p2/3P1b2/5p2/4p3/4P3/1BP2P2/1K1R1Q1R w - -",
"6r1/8/8/8/1R6/1K6/8/4k3 w - -",
"Q1Q1Q3/8/1K3p2/7p/7p/6k1/8/8 b - -",
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

inline uint64_t mo_test(const int depth=8,const bool verbose=true) {
    uint64_t nodes=0;
    auto t0=std::chrono::high_resolution_clock::now();
    for (const auto &fen:fens) {
        auto pos=chess::Board(fen);
        tt.clear();
        for (auto &killer:killers) {
            killer[0]=chess::Move();
            killer[1]=chess::Move();
        }
        for (auto &n:history) {
            for (auto &m:n) {
                m=0;
            }
        }
        for (auto &ref:refutation_move) {
            for (auto &r:ref) {
                r=chess::Move();
            }
        }
        for (auto &n:history2ply) {
            for (auto &m:n) {
                for (auto &j:m) {
                    for (auto &k:j) {
                        k=0;

                    }
                }
            }
        }
        const auto sr=search(pos,-BOUND,BOUND,depth,0,false,false,chess::Move());
        if (verbose) std::cout<<sr.nodes<<" | "<<fen<<"\n";
        nodes+=sr.nodes;
    }
    auto t1=std::chrono::high_resolution_clock::now();
    auto time=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
    if (verbose) {
        std::cout<<"#=========================#\ntime:"<<time<<"\n";
        std::cout<<"nodes:"<<nodes<<"\n";
    }
    return nodes;
}
static int mg_nudge[6][64] = {0};
static int eg_nudge[6][64] = {0};
void generate_nudge(int bound) {
    std::uniform_int_distribution<int> dist(-bound,bound);
    if (bound < 0) bound = -bound;
    for (int piece = 0; piece < 6; ++piece) {
        for (int sq = 0; sq < 64; ++sq) {
            // if (piece!=0) {
            //     mg_nudge[piece][sq]=0;
            //     eg_nudge[piece][sq]=0;
            //     continue;
            // }
            int delta_mg=dist(gen)*dist(gen)*dist(gen);
            int delta_eg=dist(gen)*dist(gen)*dist(gen);
            mg_nudge[piece][sq]=delta_mg;
            eg_nudge[piece][sq]=delta_eg;
        }
    }
}
void add_nudge() {
    for (int piece = 0; piece < 6; ++piece) {
        int* mg_tbl = mg_mo_tables[piece];
        int* eg_tbl = eg_mo_tables[piece];
        for (int sq = 0; sq < 64; ++sq) {
            mg_tbl[sq] += mg_nudge[piece][sq];
            eg_tbl[sq] += eg_nudge[piece][sq];
        }
    }
}
void subtract_nudge() {
    for (int piece = 0; piece < 6; ++piece) {
        int* mg_tbl = mg_mo_tables[piece];
        int* eg_tbl = eg_mo_tables[piece];
        for (int sq = 0; sq < 64; ++sq) {
            mg_tbl[sq] -= mg_nudge[piece][sq];
            eg_tbl[sq] -= eg_nudge[piece][sq];
        }
    }
}
void print_table(const char* name, const int table[64]) {
    std::cout << "inline int " << name << "[64]={\n";
    for (int i = 0; i < 64; ++i) {
        std::cout << table[i];
        if (i < 63) std::cout << ',';
        // Add a newline after every 8 numbers
        if ((i + 1) % 8 == 0)
            std::cout << '\n';
        else
            std::cout << ' ';
    }
    std::cout << "};\n\n";
}
void print_globals() {
    print_table("mg_pawn_mo_table", mg_pawn_mo_table);
    print_table("mg_knight_mo_table", mg_knight_mo_table);
    print_table("mg_bishop_mo_table", mg_bishop_mo_table);
    print_table("mg_rook_mo_table", mg_rook_mo_table);
    print_table("mg_queen_mo_table", mg_queen_mo_table);
    print_table("mg_king_mo_table", mg_king_mo_table);

    print_table("eg_pawn_mo_table", eg_pawn_mo_table);
    print_table("eg_knight_mo_table", eg_knight_mo_table);
    print_table("eg_bishop_mo_table", eg_bishop_mo_table);
    print_table("eg_rook_mo_table", eg_rook_mo_table);
    print_table("eg_queen_mo_table", eg_queen_mo_table);
    print_table("eg_king_mo_table", eg_king_mo_table);
}

void tune_mo_tables(const int samples=100,const int depth=8,const int bound=5) {
    for (int i=0;i<samples;i++) {
        uint64_t loss=mo_test(depth,false);
        std::cout<<"sample:"<<i+1<<" loss:"<<loss<<"\n";
        generate_nudge(bound);
        add_nudge();
        if (mo_test(depth,false)>loss) {
            subtract_nudge();
        }
    }


    print_globals();
}




#endif //TUNA2_MOVE_ORDERING_TESTING_H
