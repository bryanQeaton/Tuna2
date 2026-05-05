#ifndef TUNA2_ROOT_H
#define TUNA2_ROOT_H
#include <cstdint>

#include "constants.h"
#include "search.h"

struct Engine_return {
    int value{};
    int mate_in_n{};
    int depth{};
    uint64_t nodes{};
    std::string move{};
};


inline Engine_return root(chess::Board &pos,const int time_limit,const int max_depth=MAX_DEPTH) {
    chess::Movelist legal_moves;
    chess::movegen::legalmoves(legal_moves,pos);
    Engine_return best={-BOUND,MAX_DEPTH,0,1,chess::uci::moveToUci(legal_moves[0])};
    Engine_return last=best;
    for (int d=0;d<max_depth;d++) {
        if (legal_moves.size()==1&&d>=4){break;} //get a value for the move but dont spend forever on it
        int alph=-BOUND;
        uint64_t nodes=0;
        constexpr int beta=BOUND;
        best={-BOUND,MAX_DEPTH,0,1,chess::uci::moveToUci(legal_moves[0])};
        for (int m=0;m<legal_moves.size();m++) {
            pos.makeMove(legal_moves[m]);
            Search_return sr;
            if (m==0) {sr=search(pos,-beta,-alph,d,0,false,legal_moves[m]);}
            else {
                sr=search(pos,-alph-1,-alph,d,0,false,legal_moves[m]);
                if (-sr.value>alph) {
                    sr=search(pos,-beta,-alph,d,0,false,legal_moves[m]);
                }
            }
            pos.unmakeMove(legal_moves[m]);
            alph=std::max(alph,-sr.value);
            legal_moves[m].setScore(-sr.value);
            nodes+=sr.nodes;
            if (-sr.value>best.value) {
                best.value=-sr.value;
                best.mate_in_n=mate_in_n(-sr.value);
                best.nodes=0;
                best.depth=d;
                best.move=chess::uci::moveToUci(legal_moves[m]);
            }
            if (stop_flag){break;}
        }
        if (!stop_flag) {
            std::ranges::sort(legal_moves,[](const chess::Move a,const chess::Move b){return a.score()>b.score();});
            last=best;
            last.nodes=nodes;
        }
        else{break;}
        //sort here
    }

    return last;
}



#endif //TUNA2_ROOT_H
