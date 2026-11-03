// C++98 application trace replay, independent of MAC RNG.
#ifndef WIFI8_PAPER_REPLAY_H
#define WIFI8_PAPER_REPLAY_H
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
struct PaperArrival {
    double time; int destination, ac, bytes, flow, application;
    PaperArrival() : time(0), destination(-1), ac(0), bytes(0), flow(0), application(0) {}
};
struct PaperReplay {
    bool enabled;
    std::vector<PaperArrival> arrivals;
    unsigned int cursor;
    int fragment;
    int sharing_policy, queue_capacity;
    bool shared_ll_only, original_mapc, log_arrivals;
    double own_need, peer_budget, txop_start, txop_limit;
    PaperReplay() : enabled(false), cursor(0), fragment(0), sharing_policy(1), queue_capacity(256),
        shared_ll_only(false), original_mapc(false), log_arrivals(true), own_need(0), peer_budget(0), txop_start(0),txop_limit(0) {}
    void load(int node) {
        const char *path=std::getenv("WIFI8_PAPER_TRACE");
        if (!path) return;
        enabled=true;
        log_arrivals=std::getenv("WIFI8_PAPER_COMPACT_LOG")==NULL;
        const char *policy=std::getenv("WIFI8_PAPER_SHARING");
        if (policy && std::string(policy)=="dl_ll_demand") sharing_policy=2;
        else if (policy && std::string(policy)!="equal_txs") throw std::runtime_error("Unknown sharing policy");
        const char *capacity=std::getenv("WIFI8_PAPER_AC_QUEUE");
        if (capacity) queue_capacity=std::atoi(capacity);
        if (queue_capacity<1 || queue_capacity>4096) throw std::runtime_error("Invalid per-AC queue capacity");
        std::ifstream input(path);
        if (!input) throw std::runtime_error("Missing paper application trace");
        std::string line; double previous=-1;
        while (std::getline(input,line)) {
            if (line.empty() || line[0]=='#') continue;
            std::replace(line.begin(),line.end(),';',' ');
            std::istringstream row(line);
            PaperArrival a; int source;
            if (!(row >> a.time >> source >> a.destination >> a.ac >> a.bytes
                      >> a.flow >> a.application)) throw std::runtime_error("Bad paper trace");
            if (source!=node) continue;
            if (a.time<previous || a.time<0 || a.bytes<=0 || a.ac<0 || a.ac>2)
                throw std::runtime_error("Invalid trace ordering or payload");
            previous=a.time; arrivals.push_back(a);
        }
    }
    bool empty() const { return cursor>=arrivals.size(); }
    double time() const { return empty() ? 1e100 : arrivals[cursor].time; }
    PaperArrival current() const {
        if (empty()) throw std::runtime_error("Replay exhausted");
        return arrivals[cursor];
    }
    int payload_bytes() const { return std::min(1472,current().bytes-fragment*1472)+36; }
    void consume() { if (++fragment*1472 >= current().bytes) { ++cursor; fragment=0; } }
};
#endif
