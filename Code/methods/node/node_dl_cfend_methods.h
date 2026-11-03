#ifndef WIFI8_NODE_DL_CFEND_METHODS_H
#define WIFI8_NODE_DL_CFEND_METHODS_H

// Ledger only changes NAV semantics in the opt-in DL diagnostic backend.
void Node :: SyncDlNav(double released_until) {
    if (!dl_cfend.enabled || (node_state!=STATE_NAV && node_state!=STATE_SENSING)) return;
    double end=dl_cfend.until(SimTime());
    if (end<=SimTime()+1e-12 && released_until<=0) return; // Pure native traffic: no state/timer changes.
    // Do not clear/shorten ordinary native NAV just because the MAPC ledger is empty.
    // Only successful matching CF-End may truncate its own reservation ceiling.
    if (trigger_NAV_timeout.Active()) {
        double native_end=trigger_NAV_timeout.GetTime();
        if (released_until<=0 || native_end>released_until+1e-9)
            end=std::max(end,native_end);
    }
    if (end>SimTime()+1e-12) {
        if (node_state==STATE_SENSING) {
            PauseBackoff(); node_state=STATE_NAV;
            node_stats.last_time_not_in_nav=SimTime(); ++node_stats.times_went_to_nav;
        }
        if (!trigger_NAV_timeout.Active() || fabs(trigger_NAV_timeout.GetTime()-end)>1e-12)
            trigger_NAV_timeout.Set(FixTimeOffset(end,13,12));
    } else if (node_state==STATE_NAV) {
        trigger_NAV_timeout.Set(FixTimeOffset(SimTime()+1e-12,13,12));
    }
}

void Node :: HoldDlPeer() {
    PauseBackoff(); trigger_start_backoff.Cancel(); trigger_end_backoff.Cancel();
    dl_cfend.holding=true; dl_cfend.pending=false;
    node_state=STATE_DL_CFEND_HOLD;
    trigger_dl_cfend.Set(FixTimeOffset(std::max(SimTime()+1e-12,dl_cfend.round.end),13,12));
}

void Node :: QueueDlCfEnd(int peer_done) {
    if (!dl_cfend.enabled || !dl_cfend.round.valid()
            || dl_cfend.round.owner!=node_params.node_id) return;
    HoldDlPeer();
    if (!dl_cfend.allow_tx) return;
    printf("DL_CFEND_READY;time=%.12f;node=%d;reason=%s;epoch=%.12f\n",
        SimTime(),node_params.node_id,peer_done ? "peer_ack" : "no_peer",dl_cfend.round.epoch);
    double airtime=ComputeRtsTxTime80211ax(IEEE_BITS_OFDM_SYM_LEGACY); // 20-byte CF-End incl FCS.
    if (!DlCfEnd::fits(SimTime(),dl_cfend.round.end,SIFS,airtime)) {
        ++dl_cfend.skipped;
        printf("DL_CFEND_SKIP;time=%.12f;node=%d;reason=insufficient_time\n",SimTime(),node_params.node_id);
        return; // Keep reservation until the original ceiling, never overrun it.
    }
    dl_cfend.pending=true;
    trigger_dl_cfend.Set(FixTimeOffset(SimTime()+SIFS,13,12));
}

void Node :: DlCfEndTimer(trigger_t &) {
    if (!dl_cfend.enabled) return;
    if (dl_cfend.transmitting) {
        outportSelfFinishTX(cfend_notification);
        dl_cfend.transmitting=false; dl_cfend.holding=false;
        dl_cfend.release(dl_cfend.round,node_params.node_id,SimTime());
        dl_cfend.round=DlRound();
        RestartNode(FALSE); SyncDlNav(); return;
    }
    if (!dl_cfend.holding) return;
    if (!dl_cfend.pending) {
        dl_cfend.holding=false; dl_cfend.round=DlRound();
        RestartNode(FALSE); SyncDlNav(); return;
    }
    dl_cfend.pending=false;
    double airtime=ComputeRtsTxTime80211ax(IEEE_BITS_OFDM_SYM_LEGACY);
    bool busy=dl_cfend.foreign(dl_cfend.round,SimTime());
    for (int ch=icf_notification.left_channel;ch<=icf_notification.right_channel;++ch)
        if (channel_power[ch]>current_pd) busy=true;
    if (busy || !DlCfEnd::fits(SimTime(),dl_cfend.round.end,0,airtime)) {
        ++dl_cfend.skipped;
        printf("DL_CFEND_SKIP;time=%.12f;node=%d;reason=busy_or_budget\n",SimTime(),node_params.node_id);
        trigger_dl_cfend.Set(FixTimeOffset(std::max(SimTime()+1e-12,dl_cfend.round.end),13,12));
        return;
    }
    current_left_channel=icf_notification.left_channel;
    current_right_channel=icf_notification.right_channel;
    current_tx_duration=airtime; node_state=STATE_TX_CFEND;
    cfend_notification=GenerateNotification(PACKET_TYPE_CFEND,NODE_ID_MAPC_BROADCAST,0,0,SimTime(),airtime);
    cfend_notification.frame_length=160;
    cfend_notification.modulation_id=MODULATION_BPSK_1_2;
    cfend_notification.tx_info.nav_time=0;
    cfend_notification.tx_info.dl_round=dl_cfend.round;
    cfend_notification.tx_info.beamforming_active=BEAMFORMING_DISABLED;
    dl_cfend.transmitting=true; ++dl_cfend.sent;
    printf("DL_CFEND_TX;time=%.12f;node=%d;epoch=%.12f;end=%.12f;duration=%.12f\n",
        SimTime(),node_params.node_id,dl_cfend.round.epoch,dl_cfend.round.end,airtime);
    EmitStartTX(cfend_notification);
    trigger_dl_cfend.Set(FixTimeOffset(SimTime()+airtime,13,12));
}

bool Node :: HandleDlStart(const Notification &n) {
    if (!dl_cfend.enabled) return false;
    // Every new interferer can destroy a CF-End/peer-ACK already in reception.
    if (dl_cfend.rx_valid) {
        double interference=0;
        for (std::map<int,double>::const_iterator it=power_received_per_node.begin();it!=power_received_per_node.end();++it)
            if (it->first!=dl_cfend.rx_source) interference+=it->second;
        double sinr=UpdateSINR(power_received_per_node[dl_cfend.rx_source],interference);
        dl_cfend.rx_sinr=std::min(dl_cfend.rx_sinr,sinr);
        if (sinr<node_params.capture_effect) dl_cfend.rx_valid=false;
    }
    bool free_rx=node_state==STATE_NAV || node_state==STATE_SENSING || node_state==STATE_DL_CFEND_HOLD;
    double interference=0;
    for (std::map<int,double>::const_iterator it=power_received_per_node.begin();it!=power_received_per_node.end();++it)
        if (it->first!=n.source_id) interference+=it->second;
    double signal=power_received_per_node[n.source_id];
    double sinr=UpdateSINR(signal,interference);
    bool decoded=free_rx && CanDecodePacket(node_params.current_primary_channel,n,sinr,
        node_params.capture_effect,current_pd,signal,node_params.capture_effect_model);
    if (decoded && n.packet_type!=PACKET_TYPE_CFEND) {
        const DlRound &r=n.tx_info.dl_round;
        // Use the conservative TXOP ceiling for tagged exchanges; ordinary NAVs remain separate.
        double end=r.valid() ? r.end : SimTime()+n.tx_info.nav_time;
        bool addressed_retry=node_state==STATE_NAV &&
            (n.destination_id==node_params.node_id ||
             (n.packet_type==PACKET_TYPE_ICF && n.mapc_target_ap_id==node_params.node_id));
        // A NAV-blocked addressee cannot reply: a retry must NOT renew that block forever.
        // Ordinary non-MAPC traffic outside an active tagged round keeps native NAV behavior.
        bool track=r.valid() || (dl_cfend.has_round(SimTime()) && n.destination_id!=node_params.node_id);
        if (n.tx_info.nav_time>0 && track && !addressed_retry)
            dl_cfend.reserve(r,n.source_id,n.timestamp,end,SimTime());
        if (n.packet_type==PACKET_TYPE_ICF && n.mapc_target_ap_id==node_params.node_id
                && wlan.FindMapcGroupIdx(n.mapc_group_id)>=0) dl_cfend.round=r;
    }
    bool peer_ack=dl_cfend.holding && n.packet_type==PACKET_TYPE_ACK
        && n.destination_id==dl_cfend.round.peer && dl_cfend.round.same(n.tx_info.dl_round);
    if (n.packet_type==PACKET_TYPE_CFEND || peer_ack) {
        if (n.packet_type==PACKET_TYPE_CFEND && !decoded) ++dl_cfend.lost;
        if (decoded && !dl_cfend.rx_valid) {
            dl_cfend.rx_valid=true; dl_cfend.rx_source=n.source_id; dl_cfend.rx_type=n.packet_type;
            dl_cfend.rx_stamp=n.timestamp; dl_cfend.rx_round=n.tx_info.dl_round; dl_cfend.rx_sinr=sinr;
            dl_cfend.rx_end=SimTime()+n.tx_duration;
            if (node_state==STATE_SENSING) PauseBackoff();
        }
        if (n.packet_type==PACKET_TYPE_CFEND && free_rx) return true;
    }
    return node_state==STATE_DL_CFEND_HOLD || node_state==STATE_TX_CFEND;
}

bool Node :: HandleDlFinish(const Notification &n) {
    if (!dl_cfend.enabled) return false;
    bool match=dl_cfend.rx_source==n.source_id && fabs(dl_cfend.rx_end-SimTime())<1e-9
        && dl_cfend.rx_type==n.packet_type;
    if (match) {
        bool valid=dl_cfend.rx_valid; dl_cfend.rx_valid=false;
        double released_until=0;
        const char *drop=std::getenv("WIFI8_DL_CFEND_DIAGNOSTIC_DROP_RX");
        if (n.packet_type==PACKET_TYPE_CFEND && drop && std::atoi(drop)==node_params.node_id) valid=false;
        if (n.packet_type==PACKET_TYPE_CFEND) {
            if (valid) {
                ++dl_cfend.received;
                double before=dl_cfend.until(SimTime());
                bool removed=dl_cfend.release(n.tx_info.dl_round,n.source_id,SimTime());
                if (removed) { ++dl_cfend.released; released_until=n.tx_info.dl_round.end; }
                printf("DL_CFEND_RX;time=%.12f;node=%d;source=%d;released=%d;before=%.12f;after=%.12f;sinr_db=%.6f\n",
                    SimTime(),node_params.node_id,n.source_id,removed,before,dl_cfend.until(SimTime()),
                    ConvertPower(LINEAR_TO_DB,dl_cfend.rx_sinr));
            } else {
                ++dl_cfend.lost;
                printf("DL_CFEND_LOST;time=%.12f;node=%d;source=%d;nav_until=%.12f\n",
                    SimTime(),node_params.node_id,n.source_id,dl_cfend.until(SimTime()));
            }
            SyncDlNav(released_until);
            if (node_state==STATE_SENSING && dl_cfend.until(SimTime())<=SimTime()) ScheduleBackoffAfterDIFS();
        } else if (valid && dl_cfend.holding && dl_cfend.round.same(n.tx_info.dl_round)) QueueDlCfEnd(1);
    }
    return n.packet_type==PACKET_TYPE_CFEND || node_state==STATE_DL_CFEND_HOLD || node_state==STATE_TX_CFEND;
}
#endif
