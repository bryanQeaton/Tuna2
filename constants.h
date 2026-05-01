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

inline int mg_pawn_mo_table[64]={
-18, 0, -9, 11, -19, -2, 5, 7,
108, 93, 37, 81, 70, 106, 31, -3,
0, 18, 18, 48, 89, 32, 25, -13,
13, 24, -16, 24, 35, 48, 33, 30,
-16, -3, -15, 18, 12, -10, 27, -20,
-19, 9, -27, 9, -50, -1, 49, -17,
-33, 12, -29, -29, -25, -9, 65, -23,
-14, -7, -31, -16, -19, 18, -26, -11
};

inline int mg_knight_mo_table[64]={
-154, -80, -45, -25, 68, -123, -36, -107,
-95, -60, 80, 54, 37, 38, 25, -30,
-59, 33, 50, 68, 53, 108, 79, 58,
-41, 28, -27, 62, 70, 122, 23, 10,
-41, -28, -3, 22, 35, 11, 8, 2,
-14, 20, 11, -15, 17, 7, 46, -19,
-15, -56, -13, -15, 2, -11, -3, 35,
-86, -34, -71, -53, -7, -31, -55, -28
};

inline int mg_bishop_mo_table[64]={
-16, -2, -86, -68, -56, -36, -6, -36,
-9, 15, -28, 4, 34, 76, -3, -79,
-16, 26, 53, 35, 36, 62, 61, 11,
0, 26, 10, 53, 42, 26, 16, -2,
-21, 11, 6, 22, 71, -2, 16, 10,
-14, 5, 38, 38, 2, 10, 17, 21,
-13, -8, 20, -12, 36, 50, 17, -30,
-68, 10, -21, -31, -15, -27, -41, -12
};

inline int mg_rook_mo_table[64]={
32, 43, 38, 47, 80, 10, 15, 63,
51, 27, 63, 65, 65, 42, 41, 47,
11, -10, 26, 48, 22, 35, 47, 14,
-1, -20, 9, 49, 33, 47, 1, -15,
-12, -11, -7, -24, -11, -15, -12, -37,
-58, -95, -26, -4, -10, 4, 3, -15,
-17, -10, -49, 16, -1, 2, -15, -63,
-11, -24, -13, 25, 13, 6, -13, 5
};

inline int mg_queen_mo_table[64]={
-70, -4, 30, 36, 93, 59, 49, 44,
-23, -37, -15, -23, -53, 36, 10, 48,
-9, -12, 2, 13, 29, 17, 68, 76,
-33, -20, -8, 6, -23, -5, -28, 39,
-18, -54, 21, 12, -34, -17, -33, -28,
-15, -13, -7, 10, -2, 32, 15, 0,
-44, -26, 17, -5, -4, 38, -24, 19,
7, 9, -9, 22, -13, -39, -20, -57
};

inline int mg_king_mo_table[64]={
-62, 4, 7, -11, -57, -36, 15, 24,
67, -1, -13, 23, -39, 32, -28, -26,
12, 45, 16, -35, -42, 14, 28, -52,
-38, -29, -23, -52, -35, -32, 1, -41,
-59, 24, -28, -50, -40, -67, -21, -56,
25, 27, -54, -42, -17, -49, 21, -32,
-17, 30, -25, -85, -31, -9, 33, 0,
14, 27, -12, -64, 0, -25, 2, -25
};

inline int eg_pawn_mo_table[64]={
1, -23, -30, 16, -8, -35, -24, 33,
185, 192, 166, 126, 151, 146, 156, 200,
88, 73, 77, 57, 73, 63, 77, 85,
41, 33, 11, 10, -36, -11, 7, 7,
11, 44, 23, -35, -19, -4, -2, -5,
33, -5, 8, 16, 2, -7, -22, -1,
-1, 8, 14, 0, 24, 10, -7, -15,
-14, -6, 9, 17, 6, 9, 15, 6
};

inline int eg_knight_mo_table[64]={
-63, -75, -29, -24, -39, -51, -68, -117,
-41, -16, -47, -15, -3, -48, -39, -53,
-36, -6, -3, -33, -34, -53, -47, -54,
-7, -2, 9, 13, -3, 26, 35, -22,
-30, 13, -8, 46, 21, 31, -7, -33,
-26, -11, 27, 42, 18, 0, -22, -11,
-32, -14, 0, -3, -24, 3, -15, -41,
-20, -38, -47, -44, -25, -11, -57, -48
};

inline int eg_bishop_mo_table[64]={
-14, -26, -11, -29, -19, 24, -22, -21,
31, -8, 20, 4, 13, 0, -20, -10,
28, -9, 1, 27, 4, 9, -25, -4,
5, 11, 26, -5, 12, -8, 13, 11,
-11, 26, 23, 32, 25, 41, -20, -29,
-1, 12, 11, -9, 13, 13, -10, -8,
-19, 8, -5, -8, 8, -23, -1, -22,
-9, -5, -15, -8, -29, -18, -6, -37
};

inline int eg_rook_mo_table[64]={
3, 15, 35, -16, 24, 24, -5, -43,
23, 2, 4, -3, -8, 19, 7, -10,
12, 16, 18, -23, -22, -9, 4, 9,
-6, -4, 22, -12, 3, -19, 32, 8,
-6, -9, -1, 40, 10, -25, -1, -7,
-17, -11, 23, -20, 27, 15, -7, -31,
-20, -29, -21, -1, -15, 20, 7, -28,
-33, 13, 5, 23, 2, 1, -1, 11
};

inline int eg_queen_mo_table[64]={
-8, 4, 38, 30, 44, 6, 27, 17,
-8, -5, 43, 5, 52, 22, 26, 14,
-38, 28, 10, 26, 30, 4, 28, -14,
30, 19, -4, 75, 55, 49, 59, 46,
-23, 44, 41, 42, 32, 50, 19, 12,
-44, -19, 41, -23, -30, 32, -2, -3,
-30, -29, -10, -18, 23, -54, -53, -17,
-40, -22, -27, -53, 19, -2, -37, -79
};

inline int eg_king_mo_table[64]={
-39, -59, -41, -67, -31, -1, 36, -19,
-30, 40, 11, 24, 3, 40, 23, 20,
5, 35, 46, 0, 54, 7, 25, 1,
-37, 6, 13, -33, 2, 28, -27, -6,
-44, 6, -3, 36, 37, 47, 6, -42,
21, -5, -12, 61, -3, 37, 24, 8,
-7, -43, -6, 44, -23, 27, -28, 2,
-45, -60, -7, -23, -11, -25, -60, -34
};

inline int *mg_mo_tables[6]={
    mg_pawn_mo_table,
    mg_knight_mo_table,
    mg_bishop_mo_table,
    mg_rook_mo_table,
    mg_queen_mo_table,
    mg_king_mo_table
};
inline int *eg_mo_tables[6]={
    eg_pawn_mo_table,
    eg_knight_mo_table,
    eg_bishop_mo_table,
    eg_rook_mo_table,
    eg_queen_mo_table,
    eg_king_mo_table
};



#endif //TUNA2_CONSTANTS_H
