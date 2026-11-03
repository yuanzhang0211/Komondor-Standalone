// Node implementation fragment: included once from node_impl.h. Local AP state only.
#ifndef KOMONDOR_NODE_OBSERVATION_METHODS_H
#define KOMONDOR_NODE_OBSERVATION_METHODS_H
void Node :: ObserveAp(const char *event, const Notification *frame) {
    if (!ap_observation.enabled()) return;
    const double now=SimTime();
    if (std::string(event)=="access_won") { ap_observation.report_source=-1; ap_observation.report_time_s=-1; ap_observation.allocation_valid=false; }
    if (frame && frame->packet_type==PACKET_TYPE_ICR && frame->destination_id==node_params.node_id) {
        ap_observation.report_source=frame->source_id;
        ap_observation.report_time_s=now;
        ap_observation.report_has_data=frame->mapc_has_data!=0;
    }
    if (std::string(event)=="rule_allocation" || std::string(event)=="solo_fallback"
            || std::string(event)=="peer_grant_received") ap_observation.allocation_valid=true;
    const bool active=(node_state!=STATE_SENSING && node_state!=STATE_NAV) || std::string(event)=="access_won";
    const bool group=active && wlan.mapc_enabled && mapc_active_group_idx>=0
        && mapc_active_group_idx<wlan.num_mapc_groups;
    double oldest=0;
    for (unsigned int i=0;i<buffer.m_queue.size();++i)
        oldest=std::max(oldest,std::max(0.0,now-buffer.m_queue[i].timestamp_generated));
    long balance=(long)node_stats.num_packets_generated-(long)node_stats.num_packets_dropped
        -(long)full_buffer_replaced-(long)tx_mpdu.acknowledged.size()-buffer.TotalQueueSize();
    double backoff=ca_state.remaining_backoff;
    if (trigger_end_backoff.Active()) backoff=std::max(0.0,trigger_end_backoff.GetTime()-now);
    const bool data_pending=node_state==STATE_TX_DATA || node_state==STATE_WAIT_ACK || node_state==STATE_RX_ACK;
    std::ostringstream out; out << std::setprecision(17);
    out << "\"event\":" << ApJsonString(event) << ",\"time_s\":" << ApJsonNumber(now)
        << ",\"ap_id\":" << node_params.node_id << ",\"wlan_code\":" << ApJsonString(wlan.wlan_code)
        << ",\"state_id\":" << node_state << ",\"state_name\":" << ApJsonString(ApStateName(node_state))
        << ",\"role\":" << ApJsonString(!group ? (node_state==STATE_NAV ? "deferring" : "contending") : coordinator_ap_id==NODE_ID_NONE ? "coordinator" : "participant")
        << ",\"traffic_model\":" << node_params.traffic_model << ",\"traffic_ac\":" << current_traffic_type
        << ",\"rule\":" << ApJsonString(wlan.mapc_enabled ? "equal_split_round_robin" : "csma_ca") << ",\"mapc_enabled\":" << (wlan.mapc_enabled ? "true" : "false")
        << ",\"configured_group_ids\":[";
    for (int g=0;g<wlan.num_mapc_groups;++g) { if (g) out << ','; out << wlan.mapc_group_ids[g]; }
    out << "],\"associated_sta_ids\":[";
    for (int sta=0;sta<wlan.num_stas;++sta) { if (sta) out << ','; out << wlan.list_sta_id[sta]; }
    out << "],\"group_id\":";
    if (group) out << wlan.mapc_group_ids[mapc_active_group_idx]; else out << "null";
    out << ",\"coordinator_id\":";
    if (group) out << (coordinator_ap_id==NODE_ID_NONE ? node_params.node_id : coordinator_ap_id); else out << "null";
    out << ",\"selected_peer_id\":";
    if (group && coordinator_ap_id==NODE_ID_NONE && mapc_selected_peer_id>=0) out << mapc_selected_peer_id; else out << "null";
    out << ",\"peer_slot_active\":" << ((!group || coordinator_ap_id!=NODE_ID_NONE || !ap_observation.allocation_valid)
        ? "null" : mapc_peer_has_data ? "true" : "false");
    out << ",\"destination_id\":" << current_destination_id
        << ",\"queue_packets\":" << buffer.TotalQueueSize()
        << ",\"selected_queue_packets\":" << buffer.QueueSize()
        << ",\"queue_hol_age_s\":" << ApJsonNumber(oldest)
        << ",\"generated_mpdus\":" << node_stats.num_packets_generated
        << ",\"overflow_mpdus\":" << node_stats.num_packets_dropped
        << ",\"source_replaced_mpdus\":" << full_buffer_replaced
        << ",\"acked_mpdus\":" << tx_mpdu.acknowledged.size()
        << ",\"ledger_balance\":" << balance
        << ",\"in_flight_mpdus\":" << (data_pending ? tx_mpdu.pending.size() : 0)
        << ",\"cw_min\":" << ca_state.current_cw_min << ",\"cw_max\":" << ca_state.current_cw_max
        << ",\"backoff_remaining_s\":" << ApJsonNumber(backoff)
        << ",\"nav_until_s\":";
    if (trigger_NAV_timeout.Active()) out << ApJsonNumber(trigger_NAV_timeout.GetTime()); else out << "null";
    out << ",\"primary_channel\":" << node_params.current_primary_channel
        << ",\"channel_power_pw\":" << ApJsonNumber(channel_power[node_params.current_primary_channel])
        << ",\"sensitivity_pw\":" << ApJsonNumber(current_pd)
        << ",\"tx_power_total_pw\":" << ApJsonNumber(current_tx_power)
        << ",\"mcs\":";
    if (num_channels_tx>0 && current_modulation>0) out << current_modulation-1; else out << "null";
    out << ",\"channel_left\":" << current_left_channel << ",\"channel_right\":" << current_right_channel
        << ",\"own_data_budget_valid\":" << ((group && ap_observation.allocation_valid) ? "true" : "false")
        << ",\"own_data_budget_s\":" << ApJsonNumber(group && ap_observation.allocation_valid ? mapc_txop_per_ap_data_duration : 0)
        << ",\"peer_report\":";
    // Only a successfully received ICR from the selected peer can supply peer information.
    if (group && coordinator_ap_id==NODE_ID_NONE && ap_observation.report_source==mapc_selected_peer_id
            && ap_observation.report_time_s>=0) {
        out << "{\"source_id\":" << ap_observation.report_source << ",\"received_at_s\":" << ApJsonNumber(ap_observation.report_time_s)
            << ",\"has_data\":" << (ap_observation.report_has_data ? "true" : "false") << '}';
    } else out << "null";
    out << ",\"frame\":";
    if (frame) {
        out << "{\"type\":" << frame->packet_type << ",\"source_id\":" << frame->source_id
            << ",\"destination_id\":" << frame->destination_id << ",\"duration_s\":" << ApJsonNumber(frame->tx_duration)
            << ",\"mpdus\":" << frame->tx_info.num_packets_aggregated
            << ",\"mcs\":" << (frame->packet_type==PACKET_TYPE_DATA ? frame->modulation_id-1 : -1) << '}';
    } else out << "null";
    ap_observation.write(out.str());
}
#endif
