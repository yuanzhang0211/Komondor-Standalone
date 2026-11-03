// Three per-AC contention states. Radio, timers, AIFS, CW and draws stay native.
// Include after native edca_methods.h. C++98.
#ifndef WIFI8_PAPER_EDCA_H
#define WIFI8_PAPER_EDCA_H
struct PaperEdca {
    int cw[3], stage[3], slots[3], active_ac;
    double began[3], deadline[3];
    bool running[3], in_access, attempted;
    unsigned long internal_collisions;
    PaperEdca() : active_ac(2), in_access(false), attempted(false), internal_collisions(0) {
        for (int a=0;a<3;++a) {
            cw[a]=GetAcCwMin(a); stage[a]=0; slots[a]=-1;
            began[a]=0; deadline[a]=1e100; running[a]=false;
        }
    }
    void update(int ac, bool success) {
        int lower=0;
        HandleBackoffEDCA(success ? RESET_CW : INCREASE_CW,ac,&stage[ac],&lower,&cw[ac]);
        slots[ac]=-1;
    }
    void freeze(double now) {
        for (int a=0;a<3;++a) if (running[a]) {
            double idle=std::max(0.0,now-began[a]-ComputeAIFS(a));
            int consumed=(int)floor((idle+1e-12)/SLOT_TIME);
            slots[a]=std::max(0,slots[a]-consumed);
            running[a]=false;
        }
    }
    double schedule(double now,const int count[3]) {
        double earliest=1e100;
        for (int a=0;a<3;++a) {
            if (!count[a]) { slots[a]=-1; running[a]=false; deadline[a]=1e100; continue; }
            if (!running[a]) {
                if (slots[a]<0) slots[a]=(int)floor(ComputeBackoffEDCA(0,cw[a])/SLOT_TIME+0.5);
                began[a]=now; deadline[a]=now+ComputeAIFS(a)+slots[a]*SLOT_TIME;
                running[a]=true;
            }
            if (deadline[a]<earliest-1e-12) { earliest=deadline[a]; active_ac=a; }
        }
        return earliest;
    }
    void win(double now) {
        bool collision[3];
        for (int a=0;a<3;++a)
            collision[a]=a!=active_ac && running[a] && deadline[a]<=now+1e-11;
        freeze(now);
        for (int a=0;a<3;++a) if (collision[a]) { update(a,false); ++internal_collisions; }
        slots[active_ac]=-1; in_access=true; attempted=false;
    }
    void success() { if (in_access && attempted) update(active_ac,true); attempted=false; }
    void release(double now,bool timeout) {
        freeze(now);
        if (in_access && attempted && timeout) update(active_ac,false);
        in_access=false; attempted=false;
    }
};
#endif
