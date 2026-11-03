// Conservative round-scoped DL CF-End model, not a full 802.11 dual-NAV implementation.
#ifndef WIFI8_DL_CFEND_H
#define WIFI8_DL_CFEND_H
#include <map>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <string>
struct DlRound {
    int owner, peer;
    double epoch, end;
    DlRound() : owner(-1), peer(-1), epoch(-1), end(0) {}
    bool valid() const { return owner>=0 && peer>=0 && epoch>=0 && end>epoch; }
    bool same(const DlRound &r) const {
        return valid() && r.valid() && owner==r.owner && peer==r.peer && epoch==r.epoch;
    }
};
struct DlCfEnd {
    typedef std::pair<int,double> Key;
    bool enabled, allow_tx, holding, pending, transmitting, rx_valid;
    int rx_source, rx_type;
    double rx_stamp, rx_sinr, rx_end;
    DlRound round, rx_round;
    std::map<Key,double> reservations;
    std::map<Key,double> tagged_rounds;
    unsigned long sent, received, released, lost, skipped;
    DlCfEnd() : enabled(false), allow_tx(false), holding(false), pending(false), transmitting(false), rx_valid(false),
        rx_source(-1), rx_type(-1), rx_stamp(-1), rx_sinr(0), rx_end(0),
        sent(0), received(0), released(0), lost(0), skipped(0) {}
    void load() {
        const char *p=std::getenv("WIFI8_DL_CFEND");
        enabled=p && (std::string(p)=="0" || std::string(p)=="1");
        allow_tx=p && std::string(p)=="1";
    }
    void prune(double now) {
        for (std::map<Key,double>::iterator it=tagged_rounds.begin();it!=tagged_rounds.end();) {
            if (it->second<=now+1e-12) { std::map<Key,double>::iterator old=it++; tagged_rounds.erase(old); }
            else ++it;
        }
        for (std::map<Key,double>::iterator it=reservations.begin();it!=reservations.end();) {
            if (it->second<=now+1e-12) { std::map<Key,double>::iterator old=it++; reservations.erase(old); }
            else ++it;
        }
    }
    double until(double now) {
        prune(now); double t=now;
        for (std::map<Key,double>::const_iterator it=reservations.begin();it!=reservations.end();++it)
            t=std::max(t,it->second);
        return t;
    }
    void reserve(const DlRound &r, int source, double stamp, double expiry, double now) {
        prune(now); if (expiry<=now) return;
        Key key=r.valid() ? Key(r.owner,r.epoch) : Key(source,stamp);
        reservations[key]=std::max(reservations[key],expiry);
        if (r.valid()) tagged_rounds[key]=expiry;
    }
    bool has_round(double now) { prune(now); return !tagged_rounds.empty(); }
    bool release(const DlRound &r, int sender, double now) {
        prune(now);
        if (!r.valid() || sender!=r.owner) return false;
        tagged_rounds.erase(Key(r.owner,r.epoch));
        return reservations.erase(Key(r.owner,r.epoch))!=0;
    }
    bool foreign(const DlRound &r, double now) {
        prune(now);
        for (std::map<Key,double>::const_iterator it=reservations.begin();it!=reservations.end();++it)
            if (it->first!=Key(r.owner,r.epoch)) return true;
        return false;
    }
    static bool fits(double now, double deadline, double sifs, double airtime) {
        return now+sifs+airtime<=deadline+1e-12;
    }
};
#endif
