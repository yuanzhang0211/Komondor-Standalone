"""Regression for native rule Co-TDMA and independent, local AP observation streams."""
from pathlib import Path
from datetime import datetime
from collections import Counter
import csv
import json
import math
import os
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'scripts'))
from read_ap_state import iter_ap_states, latest_ap_state


def rows(path):
    with path.open(newline='') as stream:
        reader = csv.DictReader(stream, delimiter=';')
        return reader.fieldnames, list(reader)


def write_nodes(path, fields, data):
    with path.open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter=';')
        writer.writeheader(); writer.writerows(data)


def protocol(log):
    # Compare deterministic radio/transaction events, not wall-clock engine timing.
    return [line for line in log.splitlines() if line.startswith(('AUDIT_TX;', 'FIX_COMMIT;', 'FIX_CONTROL;', 'AUDIT_TDMA;', 'AUDIT_DECISION;'))]


def run_case(label, nodes, mapc, config, *, observe=True, seed=1, duration=.2, check_budget=True):
    out = OUTPUT/label
    command = [sys.executable, str(ROOT/'scripts/run.py'), '--mode', 'tdma' if mapc else 'none',
               '--traffic', 'native', '--nodes', str(nodes), '--config', str(config),
               '--seed', str(seed), '--time', str(duration), '--output', str(out)]
    if mapc: command += ['--mapc', str(mapc)]
    if observe: command += ['--observe']
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=60)
    assert result.returncode == 0, (label, result.stderr, (out/'stdout.log').read_text(errors='replace')[-1000:])
    log = (out/'stdout.log').read_text()
    assert 'ERROR:' not in log and 'runtime error:' not in log
    metadata = json.loads((out/'run.json').read_text())
    assert metadata['simulation_completed'] and metadata['learning_enabled'] is False
    _, node_rows = rows(nodes)
    aps = {i: row['wlan_code'] for i,row in enumerate(node_rows) if row['node_type'] == '0'}
    streams = {}
    if observe:
        expected = {f'ap_{ap}.jsonl' for ap in aps}
        assert {p.name for p in (out/'states').iterdir()} == expected
        for ap, wlan in aps.items():
            records = list(iter_ap_states(out/'states', ap))
            assert records[0]['event'] == 'ready' and records[-1]['event'] == 'final'
            assert latest_ap_state(out/'states', ap) == records[-1]
            assert records[-1]['time_s'] == duration
            for record in records:
                assert record['ap_id'] == ap and record['wlan_code'] == wlan
                assert record['ledger_balance'] == 0, (label, record)
                assert record['generated_mpdus'] == (record['acked_mpdus'] + record['overflow_mpdus'] + record['source_replaced_mpdus'] + record['queue_packets'])
                assert record['queue_packets'] >= 0 and record['queue_hol_age_s'] >= 0
                if record['peer_report'] is not None:
                    report = record['peer_report']
                    assert set(report) == {'source_id','received_at_s','has_data'}
                    assert report['source_id'] == record['selected_peer_id'] != ap
                    assert report['received_at_s'] <= record['time_s']
                if record['event'] == 'tx':
                    frame = record['frame']; assert frame['source_id'] == ap
                    if frame['type'] == 0:
                        assert frame['mpdus'] > 0
                        assert record['queue_packets'] >= frame['mpdus']
                        if check_budget and record['own_data_budget_s'] > 0:
                            assert frame['duration_s'] <= record['own_data_budget_s'] + 1e-9, ('over_budget',label,record)
            streams[ap] = records
        assert all(math.isfinite(v) for stream in streams.values() for row in stream for v in row.values() if isinstance(v,float))
    else:
        assert not (out/'states').exists()
    SUMMARY.append({'case':label,'ap_count':len(aps),'duration_s':duration,'seed':seed,'passed':True,
                    'observed':observe,'evidence':str(out.relative_to(ROOT))})
    return streams, log, out


OUTPUT = ROOT/'runs'/('basic_native_'+datetime.now().strftime('%Y%m%d_%H%M%S_%f'))
OUTPUT.mkdir(parents=True)
SUMMARY = []
with tempfile.TemporaryDirectory(prefix='native-tdma-inputs-') as folder:
    tmp = Path(folder)
    official = ROOT/'Code/input/examples/mapc_example'
    config = ROOT/'Code/config_models'
    on, a, _ = run_case('official_2ap_observed', official/'input_nodes_mapc_example.csv', official/'mapc_cotdma_example.csv', config, duration=.05)
    _, b, _ = run_case('official_2ap_unobserved', official/'input_nodes_mapc_example.csv', official/'mapc_cotdma_example.csv', config, duration=.05, observe=False)
    repeat, c, _ = run_case('official_2ap_repeat', official/'input_nodes_mapc_example.csv', official/'mapc_cotdma_example.csv', config, duration=.05)
    assert repeat == on and protocol(c) == protocol(a), 'Identical seed changed local observations'
    assert protocol(a) == protocol(b), 'Observer changed the native protocol trace'
    assert any(s['event']=='icr_received' for records in on.values() for s in records)
    assert all(records[-1]['acked_mpdus'] > 0 for records in on.values())
    # Existing native node/MAPC files remain unchanged; generated variants are test-only.
    fields, original = rows(official/'input_nodes_mapc_example.csv')
    for model in (1,2,3):
        data = [dict(row) for row in original]
        for row in data:
            row['traffic_model'] = str(model)
            row['traffic_load(pkts/s)'] = '600' if row['node_type']=='0' else '0'
        nodes = tmp/f'traffic_{model}.csv'; write_nodes(nodes, fields, data)
        for seed in (1,7):
            run_case(f'traffic_{model}_seed_{seed}',nodes,official/'mapc_cotdma_example.csv',config,seed=seed)
    # Empty peer, packet loss and bandwidth asymmetry exercise reports and timeout paths.
    for label in ('empty_peer', 'packet_loss', 'narrow_peer'):
        data = [dict(row) for row in original]
        for row in data:
            row['traffic_model']='1' if label=='empty_peer' else '99'
            if label=='empty_peer': row['traffic_load(pkts/s)']='0' if row['wlan_code']=='B' else '600'
            if label=='packet_loss': row['constant PER']='0.25'
            if label=='narrow_peer' and row['wlan_code']=='B': row['max_channel_allowed']='0'
        nodes=tmp/(label+'.csv');write_nodes(nodes,fields,data)
        observed,_,_=run_case(label,nodes,official/'mapc_cotdma_example.csv',config,duration=.25)
        if label=='empty_peer':
            assert any(s['peer_report'] is not None and not s['peer_report']['has_data']
                       for records in observed.values() for s in records)
        if label=='packet_loss':
            assert any(s['event']=='ack_timeout' for records in observed.values() for s in records)
    fixture = ROOT/'examples/multi_ap_native'
    observed,_,_ = run_case('four_aps_variable_sta_ids',fixture/'nodes.csv',fixture/'mapc.csv',fixture/'config_models',duration=.3)
    assert set(observed) == {0,2,5,7}
    assert all(records[-1]['acked_mpdus']>0 for records in observed.values())
    assert {s['peer_report']['source_id'] for r in observed.values() for s in r if s['event']=='icr_received'} == set(observed)
    # Pin only contention parameters to exercise per-group round-robin with many rounds.
    f,data = rows(fixture/'nodes.csv')
    for row in data:
        if row['node_type']=='0':
            cw = '0' if row['wlan_code']=='A' else '1023'
            row['cw_min']=cw; row['cw_max']=cw; row['cw_adaptation']='0'
    focused=tmp/'groups_nodes.csv'; write_nodes(focused,f,data)
    groups,_,_ = run_case('overlapping_groups',focused,fixture/'mapc_groups.csv',fixture/'config_models',duration=.5)
    choices = {1:set(),2:set()}
    for s in groups[0]:
        if s['event']=='access_won': choices[s['group_id']].add(s['selected_peer_id'])
    assert choices == {1:{2,5},2:{5,7}}, choices
    # More than eight peers: no fixed [8] array and no two-AP ID arithmetic.
    f,template = rows(fixture/'nodes.csv'); data=[]
    for index in range(10):
        for sta in (0,1):
            row=dict(template[0 if sta==0 else 1]); name=f'W{index}'
            row.update(node_code=name+('_AP' if sta==0 else '_STA'),wlan_code=name,node_type=str(sta))
            row['x(m)']=str(index*.6+sta*.2); row['y(m)']=str(sta)
            cw='0' if index==0 else '1023';row['cw_min']=cw;row['cw_max']=cw;row['cw_adaptation']='0'
            data.append(row)
    ten=tmp/'ten.csv'; write_nodes(ten,f,data)
    mapc=tmp/'ten_mapc.csv'; mapc.write_text('GroupID;Method;CoordinatedBSSIds;ExtraParams\n1;CO_TDMA;'+','.join(f'W{i}' for i in range(10))+';\n')
    many,_,_ = run_case('ten_aps_nine_peers',ten,mapc,fixture/'config_models',duration=.5)
    assert len(many)==10
    reports={s['peer_report']['source_id'] for s in many[0] if s['event']=='icr_received'}
    assert reports == set(range(2,20,2)), reports
    # Native standalone CSMA remains available with one local stream per AP.
    run_case('native_without_mapc',official/'input_nodes_mapc_example.csv',None,config,duration=.05)
    # Failure paths must fail clearly, not modulo zero or silently report completion.
    for label,body in [('one_member','1;CO_TDMA;A;'),('unknown_member','1;CO_TDMA;A,Z;'),('duplicate_member','1;CO_TDMA;A,A;')]:
        bad=tmp/(label+'.csv');bad.write_text('GroupID;Method;CoordinatedBSSIds;ExtraParams\n'+body+'\n')
        dest=OUTPUT/('invalid_'+label)
        p=subprocess.run([sys.executable,str(ROOT/'scripts/run.py'),'--nodes',str(official/'input_nodes_mapc_example.csv'),
                          '--mapc',str(bad),'--output',str(dest)],capture_output=True,text=True,timeout=20)
        assert p.returncode != 0, label
    # The reader never opens another AP file and tolerates incomplete trailing writes.
    states=tmp/'reader';states.mkdir()
    sample=dict(on[0][0]);sample['sequence']=0
    (states/'ap_0.jsonl').write_text(json.dumps(sample)+'\n'+ '{"unfinished":')
    assert latest_ap_state(states,0)==sample
    try: latest_ap_state(states,2)
    except FileNotFoundError: pass
    else: raise AssertionError('Reader accessed a different AP stream')
report={'schema':'basic-native-cotdma-validation-v1','cases':SUMMARY,'case_count':len(SUMMARY),
        'observation_does_not_change_protocol':True,'per_ap_local_streams':True,
        'no_demand_policy_or_agent':True,'scope':'Native equal-split/round-robin and local observations; not a live Agent action interface or full standard conformance.'}
(ROOT/'provenance/basic_native_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
