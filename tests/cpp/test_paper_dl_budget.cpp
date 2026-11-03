#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "structures/dl_cfend.h"
#include "methods/phy/frame_duration_methods.h"
int main() {
    DlCfEnd nav;
    DlRound r; r.owner=0; r.peer=20; r.epoch=1; r.end=2;
    assert(!nav.has_round(1));
    nav.reserve(DlRound(),40,1,1.5,1);
    assert(!nav.has_round(1.1)); // Ordinary reservations are not MAPC rounds.
    nav.reserve(r,0,1,2,1);
    assert(nav.has_round(1.1));
    assert(nav.release(r,0,1.2));
    assert(!nav.has_round(1.2));
    assert(nav.until(1.2)==1.5); // CF-End must preserve unrelated traffic.
    double wide=ComputeDataTxTime80211ax(1,12064,4680);
    double narrow=ComputeDataTxTime80211ax(1,12064,1170);
    assert(narrow>wide); // An old wide-band one-MPDU budget is not a narrow-band budget.
    assert(!DlCfEnd::fits(1,1+wide,0,narrow));
    assert(DlCfEnd::fits(1,1+narrow,0,narrow));
}
