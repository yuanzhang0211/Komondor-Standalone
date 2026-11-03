#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "list_of_macros.h"
#include "methods/mac/channel_access/edca_methods.h"
#include "structures/paper_edca.h"
int main() {
    const int all[3]={1,1,1};
    PaperEdca p;
    p.slots[0]=10; p.slots[1]=2; p.slots[2]=0;
    double expiry=p.schedule(0,all);
    assert(p.active_ac==1);
    assert(fabs(expiry-(SIFS+5*SLOT_TIME))<1e-12);
    p.win(expiry);
    assert(p.internal_collisions==1);
    assert(p.cw[2]==std::min(2*(GetAcCwMin(2)+1)-1,GetAcCwMax(2)));
    assert(p.slots[0]==7);
    p.attempted=true; p.success();
    assert(p.cw[1]==GetAcCwMin(1));
    p.release(expiry,false);
    PaperEdca q;
    q.slots[0]=3;
    int only_vo[3]={1,0,0};
    q.schedule(0,only_vo); q.freeze(1e-6);
    assert(q.slots[0]==3);
    assert(fabs(q.schedule(0.001,only_vo)-(0.001+ComputeAIFS(0)+3*SLOT_TIME))<1e-12);
    PaperEdca r;
    int vi[3]={0,1,0}, vi_be[3]={0,1,1};
    r.slots[1]=5;
    double original=r.schedule(0,vi);
    r.slots[2]=0;
    r.schedule(20e-6,vi_be);
    assert(r.deadline[1]==original);
    assert(r.active_ac==2);
    r.win(r.deadline[2]); r.attempted=true;
    int previous=r.cw[2];
    r.release(r.deadline[2]+1e-4,true);
    assert(r.cw[2]>previous);
    return 0;
}
