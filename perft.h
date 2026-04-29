#ifndef TUNA2_PERFT_H
#define TUNA2_PERFT_H

#include <iostream>
#include <chrono>
#include "chess.hpp"


//Computes the perft of the position for a given depth, using bulk-counting
//According to the https://www.chessprogramming.org/Perft site:
//Perft is a debugging function to walk the move generation tree of strictly legal moves to count
//all the leaf nodes of a certain depth, which can be compared to predetermined values and used to isolate bugs

inline uint64_t perft(chess::Board &pos,const int depth) {
	if (depth<=0){return 1ull;}
	chess::Movelist legal_moves;
	chess::movegen::legalmoves(legal_moves,pos);
	if (depth==1){return legal_moves.size();}
	uint64_t nodes=0;
	for (int m=0;m<legal_moves.size();m++) {
		pos.makeMove(legal_moves[m]);
		nodes+=perft(pos,depth-1);
		pos.unmakeMove(legal_moves[m]);
	}
	return nodes;
}

inline void test_perft() {
	chess::Board pos(chess::constants::STARTPOS);
	std::cout << pos;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	const auto n = perft(pos, 7);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	const auto diff = end - begin;

	std::cout << "Nodes: " << n << "\n";
	std::cout << "NPS: "
		<< static_cast<int>(n * 1000000.0 / std::chrono::duration_cast<std::chrono::microseconds>(diff).count())
		<< "\n";
	std::cout << "Time difference = "
		<< std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " [microseconds]\n";
}



#endif //TUNA2_PERFT_H
