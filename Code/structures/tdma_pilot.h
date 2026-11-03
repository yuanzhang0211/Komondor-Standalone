// Co-TDMA pilot controls; C++98, independent of COST and the radio model.
#ifndef WIFI8_TDMA_PILOT_H
#define WIFI8_TDMA_PILOT_H
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>

struct TdmaPilot {
    bool enabled;
    int ap, sta[2], selected, own_us, peer_us, policy, peer_selected;
    double rate[2];
    unsigned long rng[2];
    double next_time[2];
    TdmaPilot() : enabled(false), ap(-1), selected(-1), own_us(0), peer_us(0), policy(0), peer_selected(-1) {}
    double uniform(int i) {
        // Park-Miller with Schrage decomposition: independent of rand()/MAC.
        long k = rng[i] / 127773;
        long x = 16807 * (long)(rng[i] - k * 127773) - 2836 * k;
        if (x <= 0) x += 2147483647;
        rng[i] = (unsigned long)x;
        return (double)x / 2147483647.0;
    }
    double interval(int i) { return rate[i] > 0 ? -std::log(uniform(i))/rate[i] : 1e100; }
    void load(int node) {
        const char *path = std::getenv("WIFI8_TDMA_PLAN");
        if (!path) return;
        const char *choice = std::getenv("WIFI8_TDMA_POLICY");
        if (choice) {
            std::string name(choice);
            if (name == "round_robin") policy=1;
            else if (name == "queue_equal") policy=2;
            else if (name == "queue_adaptive") policy=3;
            else if (name != "fixed") throw std::runtime_error("Unknown TDMA policy");
        }
        std::ifstream input(path);
        if (!input) throw std::runtime_error("Cannot open WIFI8_TDMA_PLAN");
        std::string line;
        while (std::getline(input, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::replace(line.begin(), line.end(), ';', ' ');
            std::istringstream row(line);
            int a, s0, s1, chosen, own, peer;
            double r0, r1;
            unsigned long seed;
            if (!(row >> a >> s0 >> s1 >> chosen >> own >> peer >> r0 >> r1 >> seed))
                throw std::runtime_error("Malformed TDMA plan row");
            std::string trailing;
            if (row >> trailing) throw std::runtime_error("Trailing TDMA plan fields");
            if (a != node) continue;
            if (enabled || s0 == s1 || (chosen != s0 && chosen != s1) || own <= 0 || peer <= 0
                    || own+peer > 5000 || !(r0 >= 0 && r0 <= 1e6 && r1 >= 0 && r1 <= 1e6)
                    || seed == 0 || seed > 2147483646UL)
                throw std::runtime_error("Invalid or duplicate TDMA plan");
            enabled = true; ap = a; sta[0] = s0; sta[1] = s1; selected = chosen;
            own_us = own; peer_us = peer; rate[0] = r0; rate[1] = r1;
            for (int i=0; i<2; ++i) {
                rng[i] = 1 + ((seed + (unsigned long)(a+1)*1009UL + (unsigned long)i*9176UL)
                             % 2147483646UL);
                next_time[i] = interval(i);
            }
        }
        if (!enabled) throw std::runtime_error("AP missing from TDMA plan");
    }
    int event_index() const { return next_time[0] <= next_time[1] ? 0 : 1; }
    double event_time() const { return next_time[event_index()]; }
    int consume() {
        int i=event_index(); next_time[i] += interval(i); return sta[i];
    }
    double own_s() const { return own_us * 1e-6; }
    double peer_s() const { return peer_us * 1e-6; }
};

// Central wired-controller assumption: current queue counters and past ACK rates.
// No reward, future outcome, or simulator-computed MCS is exposed here.
struct TdmaQueueState {
    int q[2], next_rr;
    double rate_mbps[2];
    TdmaQueueState() : next_rr(0) {
        q[0]=q[1]=0; rate_mbps[0]=rate_mbps[1]=20.0;
    }
};
static TdmaQueueState tdma_network[2];

struct TdmaDecision { int sta_index[2], budget_us[2]; };

inline TdmaDecision ChooseTdma(const TdmaQueueState *state, int policy, int total_us) {
    if (policy < 1 || policy > 3 || total_us < 4)
        throw std::runtime_error("Invalid online TDMA decision request");
    TdmaDecision d;
    for (int ap=0; ap<2; ++ap) {
        int i=state[ap].next_rr;
        if (policy > 1 && state[ap].q[1-i] > state[ap].q[i]) i=1-i;
        if (state[ap].q[i] == 0 && state[ap].q[1-i] > 0) i=1-i;
        d.sta_index[ap]=i;
    }
    int quarter=2;
    if (policy == 3) {
        double demands[2];
        for (int ap=0; ap<2; ++ap) {
            int i=d.sta_index[ap];
            if (!(state[ap].rate_mbps[i] > 0)) throw std::runtime_error("Invalid past rate");
            demands[ap]=state[ap].q[i]*12000.0/state[ap].rate_mbps[i];
        }
        if (demands[0] > 0 && demands[1] > 0) {
            double alpha=demands[0]/(demands[0]+demands[1]);
            for (int candidate=1; candidate<=3; ++candidate)
                if (std::fabs(candidate*.25-alpha) < std::fabs(quarter*.25-alpha)-1e-12)
                    quarter=candidate;
        }
    }
    d.budget_us[0]=total_us*quarter/4;
    d.budget_us[1]=total_us-d.budget_us[0];
    return d;
}

struct TdmaStaCounts {
    long generated, dropped, acked;
    double sum_delay;
    TdmaStaCounts() : generated(0), dropped(0), acked(0), sum_delay(0) {}
};
#endif
