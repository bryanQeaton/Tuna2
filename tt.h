#ifndef CHESSBOT_TT_H
#define CHESSBOT_TT_H
#include "constants.h"
#include "chess.hpp"
#define PREFETCH(addr) __builtin_prefetch(addr, 0, 3)

//tt
struct alignas(64) Entry {
    uint64_t hash=0;
    int value=0;
    int flag=0;
    int depth=0;
    chess::Move best_move=chess::Move();
    bool is_quiesc=false;
    bool operator==(const Entry & entry) const = default;
};

constexpr size_t tt_size_kb = 1024 * HASH_SIZE;   // HASH_SIZE in MiB
constexpr size_t tt_size_bytes = tt_size_kb * 1024;
constexpr size_t tt_entries = tt_size_bytes / sizeof(Entry);
constexpr size_t tt_size = 1ULL << (64 - __builtin_clzll(tt_entries - 1)); // next power of two
static_assert((tt_size & (tt_size - 1)) == 0, "TT size must be power of two");
constexpr uint64_t mask = tt_size - 1;
struct TT {
    Entry table[tt_size][BUCKET_SIZE]{};
    const Entry* get_bucket(const uint64_t hash) const {
        const uint64_t idx = hash & mask;
        return table[idx];
    }
    void clear() {for (auto &slot:table) {for (auto &bucket:slot) {bucket=Entry();}}}
    Entry &operator[](const uint64_t hash) {
        const uint64_t idx = hash & mask;
        Entry *bucket = table[idx];
        for (int i = 0; i < BUCKET_SIZE; ++i) {
            if (bucket[i].hash == hash) return bucket[i];
        }
        int replace = 0;
        int minDepth = bucket[0].depth;
        for (int i = 1; i < BUCKET_SIZE; ++i) {
            if (bucket[i].depth < minDepth) {
                minDepth = bucket[i].depth;
                replace = i;
            }
        }
        return bucket[replace];
    }
};
inline auto tt=TT();





#endif //CHESSBOT_TT_H