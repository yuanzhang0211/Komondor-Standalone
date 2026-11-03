#!/usr/bin/env python3
"""Run native Komondor in an isolated directory; Python standard library only."""
import argparse
import csv
import json
import math
import os
from pathlib import Path
import shutil
import subprocess
from datetime import datetime

ROOT = Path(__file__).resolve().parents[1]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--mode', choices=['none', 'tdma', 'sr', 'bf'], default='tdma')
    p.add_argument('--traffic', choices=['native', 'replay'], default='native')
    p.add_argument('--policy', choices=['equal_txs', 'dl_ll_demand'], default=None)
    p.add_argument('--time', type=float, default=0.05)
    p.add_argument('--seed', type=int, default=1)
    p.add_argument('--nodes', type=Path, default=None)
    p.add_argument('--trace', type=Path, default=ROOT/'examples/two_bss/traffic.csv')
    p.add_argument('--mapc', type=Path, help='Custom MAPC CSV (required for custom coordinated topology)')
    p.add_argument('--config', type=Path, default=ROOT/'examples/two_bss/config_models')
    p.add_argument('--observe', action='store_true', help='Write a separate local state stream for every AP (native traffic only)')
    p.add_argument('--output', type=Path, help='New result directory; existing paths are rejected')
    a = p.parse_args()
    custom_nodes = a.nodes is not None
    a.nodes = a.nodes or ROOT/('examples/two_bss/nodes_bf.csv' if a.mode == 'bf' else 'examples/two_bss/nodes.csv')
    if not math.isfinite(a.time) or a.time <= 0:
        p.error('--time must be finite and positive')
    if a.mode in ('sr', 'bf') and a.traffic == 'replay':
        p.error('SR/BF development examples use --traffic native; DL replay sharing is TDMA-specific')
    if a.mapc and a.mode == 'none':
        p.error('--mapc requires a coordinated mode')
    if custom_nodes and a.mode != 'none' and not a.mapc:
        p.error('custom --nodes requires --mapc for coordinated modes')
    if a.traffic == 'native' and a.policy is not None:
        p.error('--policy is a replay-only option; native Co-TDMA uses its existing equal-split/round-robin rules')
    if a.observe and (a.traffic != 'native' or a.mode not in ('tdma','none')):
        p.error('--observe currently supports native rule-based runs')
    if a.traffic == 'replay' and a.policy is None:
        a.policy = 'dl_ll_demand'
    binary = ROOT/'Code/main/komondor_main'
    if not binary.is_file():
        p.error('build first: make build')
    inputs = {'nodes.csv': a.nodes, 'config_models': a.config}
    if a.mode != 'none':
        inputs['mapc.csv'] = a.mapc or ROOT/f'examples/two_bss/mapc_{a.mode}.csv'
    if a.traffic == 'replay':
        inputs['traffic.csv'] = a.trace
    for source in inputs.values():
        if not source.is_file():
            p.error(f'missing input: {source}')
    with a.nodes.open(newline='') as stream:
        header = csv.DictReader(stream, delimiter=';').fieldnames or []
    if 'tx_power' not in header or 'sensitivity' not in header:
        p.error('unsupported legacy node CSV schema: current inputs require tx_power/sensitivity columns; preserve and migrate historical inputs separately')
    if a.mode != 'none':
        with inputs['mapc.csv'].open(newline='') as stream:
            methods = {r.get('Method', '').strip() for r in csv.DictReader(stream, delimiter=';')}
        if methods != {'CO_'+a.mode.upper()}:
            p.error('--mode must match every group in the MAPC file')
    out = (a.output or ROOT/'runs'/datetime.now().strftime('%Y%m%d_%H%M%S_%f')).resolve()
    out.mkdir(parents=True, exist_ok=False)
    for name in ('main', 'input', 'output'):
        (out/name).mkdir()
    for name, source in inputs.items():
        shutil.copy2(source, out/'config_models' if name == 'config_models' else out/'input'/name)
    # Prevent inherited research settings from silently enabling other paths.
    env = {k: v for k, v in os.environ.items() if not k.startswith(('WIFI8_', 'KOMONDOR_AP_STATE_'))}
    settings = {}
    if a.traffic == 'replay':
        settings = {'WIFI8_PAPER_TRACE': str(out/'input/traffic.csv'),
                    'WIFI8_PAPER_SHARING': a.policy, 'WIFI8_PAPER_AC_QUEUE': '256',
                    'WIFI8_DL_CFEND': '1'}
    env.update(settings)
    cmd = [str(binary), '--nodes', '../input/nodes.csv', '--time', str(a.time),
           '--seed', str(a.seed), '--code', 'standalone', '--out', '../output/metrics.txt',
           '--logs-sys', '0', '--logs-node', '0', '--save-node', '0', '--save-agent', '0']
    if a.mode != 'none':
        cmd += ['--mapc', '../input/mapc.csv']
    if a.observe:
        cmd += ['--state-dir', str(out/'states')]
    import hashlib
    def sha(path):
        return hashlib.sha256(path.read_bytes()).hexdigest()
    metadata = {'mode': a.mode, 'traffic': a.traffic, 'policy': a.policy if a.traffic == 'replay' else None,
                'command': cmd, 'environment': settings, 'learning_enabled': False, 'observation': 'per_ap_local_jsonl' if a.observe else None,
                'native_rule': 'equal_split_round_robin' if a.traffic == 'native' and a.mode == 'tdma' else None,
                'binary_sha256': sha(binary), 'input_sha256': {name: sha(source) for name, source in inputs.items()},
                'note': 'SR/BF are development entry points, not validated physical models.'}
    (out/'run.json').write_text(json.dumps(metadata, indent=2)+'\n')
    with (out/'stdout.log').open('w') as log:
        result = subprocess.run(cmd, cwd=out/'main', env=env, stdout=log, stderr=subprocess.STDOUT)
    metadata['returncode'] = result.returncode
    log_text = (out/'stdout.log').read_text(errors='replace')
    metadata['simulation_completed'] = '# CostSimEng' in log_text and 'stopped at ' in log_text
    if result.returncode == 0 and not metadata['simulation_completed']:
        metadata['error'] = 'Native process exited before simulation completion'
    if a.observe:
        metadata['ap_state_files'] = [str(path.relative_to(out)) for path in sorted((out/'states').glob('ap_*.jsonl'))]
    (out/'run.json').write_text(json.dumps(metadata, indent=2)+'\n')
    print(out)
    raise SystemExit(result.returncode if result.returncode else (0 if metadata['simulation_completed'] else 1))


if __name__ == '__main__':
    main()
