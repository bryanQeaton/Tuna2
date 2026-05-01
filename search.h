#ifndef TUNA2_SEARCH_H
#define TUNA2_SEARCH_H
#include <atomic>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "tt.h"
#include "constants.h"
#include "chess.hpp"
#include "evaluation.h"

inline std::atomic stop_flag=false;

inline chess::Move killers[MAX_DEPTH][2]{};
inline void update_killer(const chess::Move &move,const int ply) {
    if (ply>=MAX_DEPTH){return;}
    if (move==killers[ply][0]){return;}
    killers[ply][1]=killers[ply][0];
    killers[ply][0]=move;
}
inline chess::Move refutation_move[64][64]{};
inline int score_history[2]{};
inline int history[64][64]{};

struct Search_return {
    int value=-BOUND;
    uint64_t nodes=0;
};

inline int see(chess::Board &pos,const chess::Square &sqr){
    int value=0;
    chess::Movelist captures;
    chess::movegen::legalmoves<chess::movegen::MoveGenType::CAPTURE>(captures,pos);

    std::vector<chess::Move> attackers;
    for(const auto &move:captures){
        if(move.to()==sqr) attackers.push_back(move);
    }

    if(attackers.empty()) return 0;

    chess::Move smallest_attacker=attackers[0];
    int smallest_value=mat[pos.at(smallest_attacker.from()).type()];

    for(size_t m=1;m<attackers.size();m++){
        int v=mat[pos.at(attackers[m].from()).type()];
        if(v<smallest_value){
            smallest_value=v;
            smallest_attacker=attackers[m];
        }
    }

    int piece_just_captured=mat[pos.at(smallest_attacker.to()).type()];
    pos.makeMove(smallest_attacker);
    value=std::max(0,piece_just_captured-see(pos,sqr));
    pos.unmakeMove(smallest_attacker);

    return value;
}
inline int see_move(chess::Board &pos,const chess::Move &move) {
    int value=0;
    int piece_just_captured=mat[pos.at(move.to()).type()];
    pos.makeMove(move);
    value=piece_just_captured-see(pos,move.to());
    pos.unmakeMove(move);
    return value;
}

inline Search_return quiesence(chess::Board &pos,int alph,const int beta,const int depth_to_root,const bool zw) {
    if (stop_flag){return {ABORT_SCORE,0};}
    const uint64_t hash=pos.hash();
    PREFETCH(tt.get_bucket(hash));
    if (pos.isInsufficientMaterial()||pos.isRepetition()){return {DRAW_SCORE,1};}
    //tt
    const int alph_orig=alph;
    auto best_move=chess::Move();
    Entry &entry=tt[hash];
    if (entry.hash==hash&&entry.is_quiesc) {
        if (entry.depth<=depth_to_root) {
            if (entry.flag==0){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
            if (entry.flag==-1&&entry.value>=beta){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
            if (entry.flag==1&&entry.value<=alph){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
        }
        best_move=entry.best_move;
    }
    int standng_pat=-BOUND;
    bool in_check=true;
    if (!pos.inCheck()) {
        in_check=false;
        standng_pat=evaluation(pos);
        if (standng_pat>=beta){return {standng_pat,1};}
        if (standng_pat>alph){alph=standng_pat;}
    }
    chess::Movelist legal_moves;
    if (in_check) {chess::movegen::legalmoves(legal_moves,pos);}
    else {chess::movegen::legalmoves<chess::movegen::MoveGenType::CAPTURE>(legal_moves,pos);}
    if (legal_moves.empty()) {
        if (in_check){return {-BOUND+depth_to_root,1};}
        return {standng_pat,1};
    }
    if (pos.isHalfMoveDraw()){return {DRAW_SCORE,1};}


    int value=standng_pat;
    uint64_t nodes=1;
    for (int m=0;m<legal_moves.size();m++) {
        //tt move
        if (legal_moves[m]==best_move){legal_moves[m].setScore(32766);}
        //mvv-lva
        // if (is_capture(pos,legal_moves[m])) {
        //     const int mvvlva=mat[static_cast<int>(pos.at(legal_moves[m].to()).type().internal())]*10-
        //     mat[static_cast<int>(pos.at(legal_moves[m].from()).type().internal())];
        //     legal_moves[m].setScore(legal_moves[m].score()+mvvlva);
        // }
        legal_moves[m].setScore(legal_moves[m].score()+see_move(pos,legal_moves[m]));
    }
    for (int m=0;m<legal_moves.size();m++) {
        //see base move pruning
        //move ordering
        int best_value=-BOUND;
        int best_idx=0;
        for (int n=m;n<legal_moves.size();n++) {
            if (legal_moves[n].score()>best_value) {
                best_value=legal_moves[n].score();
                best_idx=n;
            }
        }
        if (best_idx!=m) {std::swap(legal_moves[m],legal_moves[best_idx]);}
        if (legal_moves[m].score()<-100){continue;}
        //iterate
        pos.makeMove(legal_moves[m]);
        Search_return sr;
        if (m==0||zw) {sr=quiesence(pos,-beta,-alph,depth_to_root+1,zw);}
        else {
            sr=quiesence(pos,-alph-1,-alph,depth_to_root+1,true);
            if (-sr.value>alph&&beta-alph>1) {
                sr=quiesence(pos,-beta,-alph,depth_to_root+1,zw);
            }
        }
        pos.unmakeMove(legal_moves[m]);
        //update values
        value=std::max(value,-sr.value);
        nodes+=sr.nodes;
        //ab
        if (value>alph) {
            best_move=legal_moves[m];
            if (value>=beta) {
                break;
            }
            alph=value;
        }
    }
    //tt
    if (depth_to_root<=entry.depth) { //depth based replacement scheme
        entry.value=adjusted_tt_store_value(value,depth_to_root);
        entry.hash=hash;
        entry.best_move=best_move;
        entry.is_quiesc=true;
        if (value<=alph_orig) {entry.flag=1;}
        else if (value>=beta) {entry.flag=-1;}
        else{entry.flag=0;}
        entry.depth=depth_to_root;
    }
    return {value,nodes};
}

inline Search_return search(chess::Board &pos,int alph,int beta,int depth,const int depth_to_root,const bool zw,const bool nw,const chess::Move &last_move,const chess::Move exclude=chess::Move()) {
    if (stop_flag){return {ABORT_SCORE,0};}
    const uint64_t hash=pos.hash();
    PREFETCH(tt.get_bucket(hash));
    const bool is_pv=!zw;
    if (pos.isInsufficientMaterial()||pos.isRepetition()){return {DRAW_SCORE,1};}
    const int eval=evaluation(pos);
    const bool in_check=pos.inCheck();
    if (depth<=0) {
        if (depth>=MAX_DEPTH||!in_check||eval>=800||!is_pv) {
            const auto quiesc=quiesence(pos,alph,beta,depth_to_root,zw);
            return {quiesc.value,quiesc.nodes};
        }
        depth=1;
    }
    bool improving=eval>=score_history[pos.sideToMove()];
    score_history[pos.sideToMove()]=eval;
    //tt
    const int alph_orig=alph;
    auto best_move=chess::Move();
    Entry &entry=tt[hash];
    if (entry.hash==hash&&!entry.is_quiesc) {
        if (entry.depth>=depth) {
            if (entry.flag==0){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
            if (entry.flag==-1&&entry.value>=beta){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
            if (entry.flag==1&&entry.value<=alph){return {adjusted_tt_retrieval_value(entry.value,depth_to_root),1};}
        }
        best_move=entry.best_move;
    }
    //mate distance pruning
    alph=std::max(alph,-BOUND+depth_to_root);
    beta=std::min(beta,BOUND-depth_to_root);
    if (alph>=beta) {return {alph,1};}

    //rfp
    if (!is_pv&&!in_check&&eval>=beta+100*depth-250) {
        //return {eval(pos),1ull,depth_to_root,move_history};
        auto sr=quiesence(pos,alph,beta,depth_to_root,zw);
        return {sr.value,sr.nodes};
    }
    //razoring
    if (depth<=4&&!is_pv&&!in_check&&eval<alph-150) {
        //return {eval(pos),1ull,depth_to_root,move_history};
        auto sr=quiesence(pos,alph,beta,depth_to_root,zw);
        return {sr.value,sr.nodes};
    }


    if (!is_pv&&!nw&&!in_check&&has_non_pawn_material_stm(pos)&&eval>=beta) {
        auto lm=chess::Move::make(chess::Square::SQ_A1,chess::Square::SQ_A1);
        if (pos.sideToMove()) {
            lm=chess::Move::make(chess::Square::SQ_A2,chess::Square::SQ_A2);
        }
        pos.makeNullMove();
        const auto sr=search(pos,-beta,-beta+1,depth-3-(depth/3),depth_to_root+1,zw,true,lm);
        pos.unmakeNullMove();
        if (-sr.value>=beta){return {-sr.value,sr.nodes};}
    }
    chess::Movelist legal_moves;
    chess::movegen::legalmoves(legal_moves,pos);
    if (legal_moves.empty()) {
        if (in_check){return {-BOUND+depth_to_root,1};}
        return {DRAW_SCORE,1};
    }
    if (pos.isHalfMoveDraw()){return {DRAW_SCORE,1};}

    //singular extension check
    int singular_extension=0;
    if (legal_moves.size()>1&&entry.hash==hash&&entry.depth>=depth&&(entry.flag==-1||entry.flag==0)&&entry.best_move!=chess::Move()&&!nw) {
        auto sr=search(pos,entry.value-350-1,entry.value-350,depth/2,depth_to_root,true,true,last_move,best_move);
        if (sr.value<entry.value-350) {
            singular_extension=1;
        }
    }
    //not improving score reduction
    if (!is_pv&&!improving&&!in_check&&singular_extension==0&&depth<=4&&legal_moves.size()>1) {
        depth-=1;
    }

    //if (legal_moves.size()==1){depth+=1;} //poor performance

    int value=-BOUND;
    int phase=pos.occ().count();
    phase=(phase*256)/32;
    phase=std::clamp(phase,0,256);

    uint64_t nodes=1;
    for (auto&move:legal_moves) {
        //mvv-lva
        if (is_capture(pos,move)) {
            const int mvvlva=mat[static_cast<int>(pos.at(move.to()).type().internal())]*10-
            mat[static_cast<int>(pos.at(move.from()).type().internal())];
            int undefended_bonus=0;
            if (!pos.isAttacked(move.to(),~pos.sideToMove())) {
                undefended_bonus=mat[static_cast<int>(pos.at(move.to()).type().internal())];
            }
            move.setScore(move.score()+mvvlva+10000+see_move(pos,move)*10+undefended_bonus);
        }
        //killers
        if (move==killers[depth_to_root][0]){move.setScore(move.score()+10000);}
        else if (move==killers[depth_to_root][1]){move.setScore(move.score()+10000);}
        //promo
        if (is_promo(move)){move.setScore(move.score()+13000);}
        //castling
        if (move.typeOf()==chess::Move::CASTLING){move.setScore(move.score()+8000);}
        //psqt
        if (pos.sideToMove()==chess::Color::WHITE) {
            int mg=mg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][flip(move.to().index())]-mg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][flip(move.from().index())];
            int eg=eg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][flip(move.to().index())]-eg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][flip(move.from().index())];
            move.setScore(move.score()+(mg*phase+eg*(256-phase))/256);
        }
        else {
            int mg=mg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][move.to().index()]-mg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][move.from().index()];
            int eg=eg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][move.to().index()]-eg_mo_tables[static_cast<int>(pos.at(move.from()).type().internal())][move.from().index()];
            move.setScore(move.score()+(mg*phase+eg*(256-phase))/256);
        }
        //refutation
        if (move==refutation_move[last_move.from().index()][last_move.to().index()]) {
            move.setScore(move.score()+7500);
        }
        //null move refutation
        if (!pos.sideToMove()) {
            const auto sqr=chess::Square(chess::Square::SQ_A2);
            if (move==refutation_move[sqr.index()][sqr.index()]) {
                move.setScore(move.score()+3000);
            }
        }
        else {
            const auto sqr=chess::Square(chess::Square::SQ_A1);
            if (move==refutation_move[sqr.index()][sqr.index()]) {
                move.setScore(move.score()+3000);
            }
        }
        if (pos.givesCheck(move)!=chess::CheckType::NO_CHECK){move.setScore(move.score()+10000);}
        //poor moves
        if (pos.isAttacked(move.from(),!pos.sideToMove())&&!pos.isAttacked(move.from(),pos.sideToMove())) {
                move.setScore(move.score()+mat[static_cast<int>(pos.at(move.from()).type().internal())]*2);
        }
        //history
        move.setScore(move.score()+history[move.from().index()][move.to().index()]);
        //tt move - must be last
        if (move==best_move){move.setScore(30066);}
    }
    for (int m=0;m<legal_moves.size();m++) {
        if (legal_moves[m]==exclude){continue;}
        //move ordering
        int best_value=-BOUND;
        int best_idx=0;
        for (int n=m;n<legal_moves.size();n++) {
            if (legal_moves[n].score()>best_value) {
                best_value=legal_moves[n].score();
                best_idx=n;
            }
        }
        if (best_idx!=m) {
            std::swap(legal_moves[m],legal_moves[best_idx]);
        }
        //iterate
        //if (m>0&&zw&&see_move(pos,legal_moves[m])<-100){continue;}
        pos.makeMove(legal_moves[m]);

        Search_return sr;
        if (m==0||zw) {sr=search(pos,-beta,-alph,depth-1+singular_extension,depth_to_root+1,zw,nw,legal_moves[m]);}
        else {
            int lmr=0;
            if (exclude==chess::Move()&&is_quiet(pos,legal_moves[m])){lmr=1+log(m)+log(depth_to_root+sqrt(depth)*2);}
            sr=search(pos,-alph-1,-alph,depth-1-lmr,depth_to_root+1,true,nw,legal_moves[m]);
            if (-sr.value>alph&&is_pv) {
                sr=search(pos,-beta,-alph,depth-1,depth_to_root+1,zw,nw,legal_moves[m]);
            }
        }
        pos.unmakeMove(legal_moves[m]);
        //update values
        if (-sr.value>value) {
            value=-sr.value;
        }
        nodes+=sr.nodes;
        //ab
        if (value>alph) {
            best_move=legal_moves[m];
            refutation_move[last_move.from().index()][last_move.to().index()]=legal_moves[m];
            if (value>=beta) {
                history[legal_moves[m].from().index()][legal_moves[m].to().index()]+=depth*depth;
                if (!is_capture(pos,legal_moves[m])&&!is_promo(legal_moves[m])) {
                    update_killer(legal_moves[m],depth_to_root);
                }
                break;
            }
            history[legal_moves[m].from().index()][legal_moves[m].to().index()]-=depth;

            alph=value;
        }
    }


    //tt
    if (depth>=entry.depth||entry.is_quiesc) { //depth based replacement scheme
        entry.value=adjusted_tt_store_value(value,depth_to_root);
        entry.hash=hash;
        entry.best_move=best_move;
        entry.is_quiesc=false;
        if (value<=alph_orig) {entry.flag=1;}
        else if (value>=beta) {entry.flag=-1;}
        else{entry.flag=0;}
        entry.depth=depth;
    }
    return {value,nodes};
}






#endif //TUNA2_SEARCH_H
