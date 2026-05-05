#ifndef TUNA2_UCI_H
#define TUNA2_UCI_H

#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <sstream>
#include <vector>
#include <mutex>
#include <chrono>

#include "root.h"
#include "search.h"

enum SearchState { IDLE=0, SEARCHING=1 };

inline std::atomic<bool> running = true;
inline std::atomic<SearchState> search_state = IDLE;

// time controls
inline std::atomic<int> wtime = 0, btime = 0;
inline std::atomic<int> winc = 0, binc = 0;
inline std::atomic<int> movetime = 0;
inline std::atomic<int> time_limit = 0;

inline std::chrono::high_resolution_clock::time_point start_time;

// shared position + mutex
inline chess::Board pos;
// ---------------- TIMER ----------------
inline void watch_timer() {
    while (running) {
        if (search_state == SEARCHING && time_limit > 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
            if (elapsed >= time_limit) {
                stop_flag = true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// ---------------- INPUT ----------------
inline void watch_input() {
    std::string line;
    while (running && std::getline(std::cin, line)) {
        try {
            if (line == "quit") {
                running = false;
                stop_flag = true;
            }
            else if (line == "uci") {
                std::cout << "id name Tuna2\nid author BryanEquinn\nuciok\n" << std::flush;
            }
            else if (line == "isready") {
                std::cout << "readyok\n" << std::flush;
            }
            else if (line == "ucinewgame") {
                tt.clear();
            }
            else if (line.starts_with("position")) {
                std::vector<std::string> words;
                std::stringstream ss(line);
                std::string w;
                while (ss >> w) words.push_back(w);
                if (words[1] == "startpos") {
                    pos.setFen(chess::constants::STARTPOS);
                } else if (words[1] == "fen") {
                    std::string fen;
                    size_t i = 2;
                    while (i < words.size() && words[i] != "moves") {
                        fen += words[i] + " ";
                        i++;
                    }
                    pos.setFen(fen);
                }

                auto it = std::ranges::find(words, "moves");
                if (it != words.end()) {
                    for (++it; it != words.end(); ++it) {
                        pos.makeMove(chess::uci::uciToMove(pos,*it));
                    }
                }
            }
            else if (line.starts_with("go")) {
                std::vector<std::string> words;
                std::stringstream ss(line);
                std::string w;
                while (ss >> w) words.push_back(w);

                movetime = 0;

                for (size_t i = 1; i + 1 < words.size(); i++) {
                    if (words[i] == "wtime") wtime = std::stoi(words[i+1]);
                    else if (words[i] == "btime") btime = std::stoi(words[i+1]);
                    else if (words[i] == "winc") winc = std::stoi(words[i+1]);
                    else if (words[i] == "binc") binc = std::stoi(words[i+1]);
                    else if (words[i] == "movetime") movetime = std::stoi(words[i+1]);
                }

                stop_flag = false;
                start_time = std::chrono::high_resolution_clock::now();
                search_state = SEARCHING;
            }
            else if (line == "stop") {
                stop_flag = true;
            }
            else if (line == "show") {
                std::cout << pos << std::flush;
            }
        }
        catch (...) {}
    }
}

// ---------------- MAIN LOOP ----------------
inline void uci(const int depth_limit = MAX_DEPTH) {
    std::thread input_thread(watch_input);
    std::thread timer_thread(watch_timer);

    while (running) {
        if (search_state == SEARCHING) {

            // time management
            if (movetime == 0) {
                int time_left = (pos.sideToMove() == chess::Color::WHITE) ? wtime.load() : btime.load();
                int inc = (pos.sideToMove() == chess::Color::WHITE) ? winc.load() : binc.load();
                time_limit = (time_left / 20) + (inc / 2);
            } else {
                time_limit = movetime.load();
            }

            Engine_return ret;
            ret = root(pos, time_limit, depth_limit);
            int elapsed = std::max(1, static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                       std::chrono::high_resolution_clock::now() - start_time).count()));
            if (ret.mate_in_n!=MAX_DEPTH) {
                std::cout << "info depth " << ret.depth
                      << " score mate " << ret.mate_in_n
                      << " nodes " << ret.nodes
                      << " nps " << (ret.nodes * 1000 / elapsed)
                      << "";
            }
            else {
                std::cout << "info depth " << ret.depth
                      << " score cp " << ret.value
                      << " nodes " << ret.nodes
                      << " nps " << (ret.nodes * 1000 / elapsed)
                      << "";
            }
            // auto p=pos;
            // std::vector<chess::Move> pv={chess::uci::uciToMove(p,ret.move)};
            // p.makeMove(chess::uci::uciToMove(p,ret.move));
            // Entry entry=tt[p.hash()];
            // chess::Move last_move{};
            // int counter=0;
            // do {
            //     counter++;
            //     auto move=entry.best_move;
            //     if (move==chess::Move()||!p.isLegal(move)){break;}
            //     p.makeMove(move);
            //     pv.push_back(move);
            //     last_move=move;
            //     entry=tt[p.hash()];
            //
            // } while (last_move!=chess::Move()&&counter<MAX_DEPTH);
            // for (auto m:pv){
            //     std::cout<<chess::uci::moveToUci(m)<<" ";
            // }
            std::cout<<"\n";

            std::cout<<"bestmove "<<ret.move<<"\n"<< std::flush;

            search_state=IDLE;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (input_thread.joinable()) input_thread.join();
    if (timer_thread.joinable()) timer_thread.join();
}

#endif