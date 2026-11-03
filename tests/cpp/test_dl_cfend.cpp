#include <cassert>
#include "structures/dl_cfend.h"
int main() {
    DlRound a; a.owner=0; a.peer=3; a.epoch=1; a.end=2;
    DlRound b=a; b.epoch=1.1;
    DlCfEnd s;
    s.reserve(a,0,1,2,1);
    s.reserve(DlRound(),6,1.1,3,1.1);
    assert(!s.release(a,3,1.2)); // A peer may not spoof the owner's release.
    assert(!s.release(b,0,1.2)); // A stale/different round cannot release this round.
    assert(s.release(a,0,1.2));
    assert(s.until(1.2)==3); // Independent reservation survives.
    assert(s.until(3)==3);
    s.reserve(a,0,1,2,1);
    assert(s.until(1.5)==2); // Lost CF-End: no release, ordinary expiration.
    assert(s.until(2)==2);
    assert(DlCfEnd::fits(1,2,.1,.2));
    assert(!DlCfEnd::fits(1.8,2,.1,.2));
    assert(!DlCfEnd::fits(2,2,0,.000052));
    DlRound c=a; c.owner=6;
    assert(!a.same(c));
    s.reserve(c,6,1,2,1);
    assert(s.foreign(a,1.1));
}
