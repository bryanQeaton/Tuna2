//
// Created by bryan on 2026-04-25.
//

#ifndef TUNA2_CONSTANTS_H
#define TUNA2_CONSTANTS_H
#include "chess.hpp"

//#=======Constants=======#
inline constexpr int BOUND=1000000;
inline constexpr int DRAW_SCORE=0;
inline constexpr int MAX_DEPTH=100;
inline constexpr int ABORT_SCORE=-123456789;
inline constexpr int HASH_SIZE=64;
inline constexpr int BUCKET_SIZE=4;

constexpr bool is_decisive(const int value) {
    if (value>=BOUND-MAX_DEPTH||value<=-BOUND+MAX_DEPTH){return true;}
    return false;
}
constexpr bool is_win(const int value) {
    if (value>=BOUND-MAX_DEPTH){return true;}
    return false;
}
constexpr bool is_loss(const int value) {
    if (value<=-BOUND+MAX_DEPTH){return true;}
    return false;
}
//returns max_depth if no mate in n
constexpr int mate_in_n(const int value) {
    if (!is_decisive(value)){return MAX_DEPTH;}
    if (BOUND-value<MAX_DEPTH&&BOUND-value>=0){return (BOUND-value)/2+1;}
    if (-(-BOUND+value)){return -(BOUND+value)/2-1;}
    return MAX_DEPTH;
}
constexpr bool is_capture(const chess::Board &pos,const chess::Move &move) {return pos.at(move.to())!=chess::Piece::NONE;}
constexpr bool is_promo(const chess::Move &move){return move.typeOf()==chess::Move::PROMOTION;}
constexpr bool is_quiet(const chess::Board &pos,const chess::Move &move){return pos.at(move.to())==chess::Piece::NONE&&!is_promo(move);}
constexpr bool has_non_pawn_material_stm(const chess::Board &pos) {
    auto pieces=pos.us(pos.sideToMove());
    pieces^=pos.pieces(chess::PieceType::PAWN,pos.sideToMove());
    pieces^=pos.pieces(chess::PieceType::KING,pos.sideToMove());
    return !pieces.empty();
}
constexpr int adjusted_tt_store_value(const int value, const int depth_to_root) {
    if (is_win(value)){return value+depth_to_root;}
    if (is_loss(value)){return value-depth_to_root;}
    return value;
}
constexpr int adjusted_tt_retrieval_value(const int value, const int depth_to_root) {
    if (is_win(value)){return value-depth_to_root;}
    if (is_loss(value)){return value+depth_to_root;}
    return value;
}


#endif //TUNA2_CONSTANTS_H
