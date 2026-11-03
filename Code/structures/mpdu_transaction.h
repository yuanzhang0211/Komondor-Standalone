#ifndef MAPC_MPDU_TRANSACTION_H
#define MAPC_MPDU_TRANSACTION_H
#include <vector>
#include <set>
#include <stdexcept>

// Identity, not a mutable aggregation counter, is the ACK/queue commit boundary.
struct MpduTransaction {
    std::vector<int> pending;
    std::set<int> acknowledged;
    int destination;
    MpduTransaction() : destination(-1) {}
    void begin(const std::vector<int> &ids, int peer) {
        pending = ids;
        destination = peer;
    }
    bool matches(int peer, int packet_id) const {
        return !pending.empty() && peer == destination && packet_id == pending.front();
    }
    bool prefix_matches(const std::vector<int> &prefix) const {
        if (prefix != pending || pending.empty()) return false;
        for (size_t i = 0; i < pending.size(); ++i)
            if (acknowledged.count(pending[i])) return false;
        return true;
    }
    void commit() {
        for (size_t i = 0; i < pending.size(); ++i)
            if (!acknowledged.insert(pending[i]).second)
                throw std::runtime_error("duplicate MPDU ACK commit");
        pending.clear();
    }
};
#endif
