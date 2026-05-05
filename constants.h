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
inline constexpr int HASH_SIZE=16;
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
constexpr chess::Bitboard passed_pawns(const chess::Board &pos,const chess::Color &side) {
    const auto w_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::WHITE);
    const auto b_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::BLACK);
    chess::Bitboard passed=0;
    chess::Bitboard pawns;
    if (side==chess::Color::WHITE){pawns=w_pawns;}
    else{pawns=b_pawns;}
    while (pawns) {
        chess::Bitboard pawn=1ull<<pawns.pop();
        chess::Bitboard mask;
        if (side==chess::Color::WHITE) {
            mask=pawn<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            auto temp=mask;
            mask|=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&b_pawns)) {
                passed|=pawn;
            }
        }
        else {
            mask=pawn>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            auto temp=mask;
            mask|=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&w_pawns)) {
                passed|=pawn;
            }
        }
    }
    return passed;
}
constexpr chess::Bitboard backwards_pawns(const chess::Board &pos,const chess::Color side) {
    const auto w_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::WHITE);
    const auto b_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::BLACK);
    chess::Bitboard backwards=0;
    chess::Bitboard pawns;
    if (side==chess::Color::WHITE){pawns=w_pawns;}
    else{pawns=b_pawns;}
    while (pawns) {
        chess::Bitboard pawn=1ull<<pawns.pop();
        chess::Bitboard mask;
        if (side==chess::Color::WHITE) {
            mask=pawn;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            auto temp=mask;
            mask=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&w_pawns)) {
                backwards|=pawn;
            }
        }
        else {
            mask=pawn;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            auto temp=mask;
            mask=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&b_pawns)) {
                backwards|=pawn;
            }
        }
    }
    return backwards;
}
constexpr chess::Bitboard isolated_pawns(const chess::Board &pos,const chess::Color side) {
    const auto w_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::WHITE);
    const auto b_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::BLACK);
    chess::Bitboard isolated=0;
    chess::Bitboard pawns;
    if (side==chess::Color::WHITE){pawns=w_pawns;}
    else{pawns=b_pawns;}
    while (pawns) {
        chess::Bitboard pawn=1ull<<pawns.pop();
        chess::Bitboard mask;
        if (side==chess::Color::WHITE) {
            mask=pawn;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            auto temp=mask;
            mask=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&w_pawns)) {
                isolated|=pawn;
            }
        }
        else {
            mask=pawn;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask>>8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            mask|=mask<<8;
            auto temp=mask;
            mask=temp<<1&0xfefefefefefefefe;
            mask|=temp>>1&0x7f7f7f7f7f7f7f7f;
            if (!(mask&b_pawns)) {
                isolated|=pawn;
            }
        }
    }
    return isolated;
}
constexpr chess::Bitboard blocked_pawns(const chess::Board &pos,const chess::Color side) {
    const auto w_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::WHITE);
    const auto b_pawns=pos.pieces(chess::PieceType::PAWN,chess::Color::BLACK);
    chess::Bitboard blocked=0;
    chess::Bitboard pawns;
    if (side==chess::Color::WHITE){pawns=w_pawns;}
    else{pawns=b_pawns;}
    while (pawns) {
        chess::Bitboard pawn=1ull<<pawns.pop();
        chess::Bitboard mask;
        if (side==chess::Color::WHITE) {
            mask=pawn<<8;
            if (mask&b_pawns) {
                blocked|=pawn;
            }
        }
        else {
            mask=pawn>>8;
            if (mask&w_pawns) {
                blocked|=pawn;
            }
        }
    }
    return blocked;
}
constexpr chess::Bitboard attacks_from(const chess::Board &pos,const chess::Square &sqr,const chess::Piece &piece) {
    auto pt=piece.type();
    if (pt==0) {return chess::attacks::pawn(piece.color(),sqr);}
    if (pt==1) {return chess::attacks::knight(sqr);}
    if (pt==2) {return chess::attacks::bishop(sqr,pos.occ());}
    if (pt==3) {return chess::attacks::rook(sqr,pos.occ());}
    if (pt==4) {return chess::attacks::queen(sqr,pos.occ());}
    if (pt==5) {return chess::attacks::king(sqr);}
    return {0};
}


#endif //TUNA2_CONSTANTS_H
