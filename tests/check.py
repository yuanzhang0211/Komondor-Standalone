"""Native invariants and an independently relocated clean build, without pytest."""
from pathlib import Path
import json
import os
import shlex
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def events(log, prefix):
    return [dict(x.split('=', 1) for x in line.split(';')[1:])
            for line in log.splitlines() if line.startswith(prefix+';')]


with tempfile.TemporaryDirectory(prefix='komondor-independent-') as folder:
    tmp = Path(folder)
    for src in sorted((ROOT/'tests/cpp').glob('*.cpp')):
        exe = tmp/src.stem
        subprocess.run(shlex.split(os.environ.get('CXX', 'g++')) +
                       shlex.split(os.environ.get('CPPFLAGS', '')) +
                       shlex.split(os.environ.get('CXXFLAGS', '-std=c++98')) +
                       ['-I', str(ROOT/'Code'), str(src), '-o', str(exe)] +
                       shlex.split(os.environ.get('LDFLAGS', '')), check=True)
        subprocess.run([str(exe)], check=True)
    assert not any(p.is_symlink() for p in ROOT.rglob('*')), 'project must contain real files'
    relocated = tmp/'standalone'
    def copy_ignore(directory, names):
        directory = Path(directory)
        ignored = {'__pycache__', '.git'} & set(names)
        if directory == ROOT:
            ignored.update({'runs', 'build'} & set(names))
        if directory == ROOT/'Code/main':
            ignored.update({'komondor_main', 'komondor_main.cxx', 'compcxx_komondor_main.h',
                            '.build-config', '.build-config.tmp'} & set(names))
        if directory == ROOT/'Code/output':
            ignored.update(set(names)-{'.gitkeep'})
        return ignored
    shutil.copytree(ROOT, relocated, ignore=copy_ignore)
    subprocess.run([sys.executable, str(relocated/'scripts/inventory.py'), '--check'], check=True)
    with (tmp/'build.log').open('w') as stream:
        subprocess.run(['make', '-C', str(relocated), 'build'], stdout=stream, stderr=subprocess.STDOUT, check=True)
    # A modification to a PHY subdirectory must regenerate COST output.
    h = relocated/'Code/methods/phy/beamforming_methods.h'
    latest = (relocated/'Code/main/komondor_main.cxx').stat().st_mtime + 2
    os.utime(h, (latest, latest))
    dry = subprocess.check_output(['make', '-n', '-C', str(relocated/'Code/main')], text=True)
    assert '../COST/cxx komondor_main.cc' in dry
    cases = [('none','replay','equal_txs'), ('tdma','replay','equal_txs'),
             ('tdma','replay','dl_ll_demand'), ('none','native','equal_txs'),
             ('tdma','native','equal_txs'), ('sr','native','equal_txs'), ('bf','native','equal_txs')]
    summary = []
    for i,(mode,traffic,policy) in enumerate(cases):
        out=tmp/f'case_{i}'
        command = [sys.executable, str(relocated/'scripts/run.py'), '--mode',mode,
                   '--traffic',traffic,'--output',str(out)]
        if traffic == 'replay': command += ['--policy',policy]
        subprocess.run(command, check=True,
                       stdout=subprocess.DEVNULL, cwd=tmp, timeout=60)
        log=(out/'stdout.log').read_text()
        assert 'stopped at 0.050000' in log
        assert 'runtime error:' not in log and 'ERROR: AddressSanitizer' not in log
        assert 'FIX_ERROR' not in log
        if traffic == 'replay':
            finals=events(log,'PAPER_FINAL')
            assert len(finals)==4
            assert sum(int(e['generated']) for e in finals)==700
            for e in finals:
                assert int(e['generated'])==sum(int(e[k]) for k in ('dropped','acked','queued'))
            assert events(log,'PAPER_RX')
            if mode=='tdma':
                grants=events(log,'PAPER_GRANT')
                assert grants and any(float(e['peer_budget'])>0 for e in grants)
                assert sum(int(e['sent']) for e in events(log,'DL_CFEND_FINAL'))>0
                for e in grants:
                    assert float(e['own'])+float(e['peer_budget'])<=float(e['available'])+1e-9
            identity=json.loads((out/'run.json').read_text())
            assert identity['learning_enabled'] is False
        if mode=='bf':
            assert any(e['enabled']=='1' for e in events(log,'AUDIT_BEAM'))
        summary.append({'mode':mode,'traffic':traffic,'policy':policy,'passed':True})
    report={'native_unit_tests':len(list((ROOT/'tests/cpp').glob('*.cpp'))),'relocated_clean_build':True,'no_symlinks':True,
            'header_dependency_tracking':True,'preserved_official_materials_in_relocated_copy':True,'runtime_cases':summary,
            'scope':'Engineering smoke/invariants; SR/BF PHY correctness is not validated.'}
    (ROOT/'provenance/validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
