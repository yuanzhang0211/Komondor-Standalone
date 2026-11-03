#!/usr/bin/env python3
"""Inspect the local build tools or maintain a compiler/flags dependency stamp."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import shlex
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def output(command):
    return subprocess.check_output(command, stderr=subprocess.STDOUT, text=True, timeout=30).strip()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--stamp', type=Path, help='Update a dependency stamp only when compiler/flags change')
    parser.add_argument('--record', type=Path, help='Save successful environment inspection as JSON')
    args = parser.parse_args()
    try:
        cxx = shlex.split(os.environ.get('CXX', 'g++'))
        if not cxx:
            raise ValueError('CXX is empty')
        settings = {k: os.environ.get(k, '') for k in ('PROFILE', 'CXXFLAGS', 'CPPFLAGS', 'LDFLAGS')}
        settings.update(compiler=cxx, compiler_path=shutil.which(cxx[0]),
                        compiler_version=output(cxx+['--version']).splitlines()[0])
        if args.stamp:
            text = json.dumps(settings, sort_keys=True, indent=2)+'\n'
            if not args.stamp.exists() or args.stamp.read_text() != text:
                temporary = args.stamp.with_suffix('.tmp')
                temporary.write_text(text)
                temporary.replace(args.stamp)
            return
        if sys.version_info < (3, 10):
            raise ValueError('Python >= 3.10 required')
        if platform.system() != 'Linux':
            raise ValueError('Current supported build platform is Linux; other platforms require separate validation')
        make_version = output(['make', '--version']).splitlines()[0]
        if not make_version.startswith('GNU Make '):
            raise ValueError('GNU make required')
        generator = ROOT/'Code/COST/cxx'
        # The generator reports this input error only after the loader starts it.
        probe = subprocess.run([str(generator)], capture_output=True, text=True, timeout=10)
        if 'no input file' not in probe.stdout+probe.stderr:
            raise ValueError('Bundled COST generator did not start; rebuild with flex for this platform')
        with tempfile.TemporaryDirectory(prefix='komondor-toolchain-') as name:
            source = Path(name)/'probe.cc'
            binary = Path(name)/'probe'
            source.write_text('#include <vector>\nint main() { std::vector<int> v(1, 0); return v[0]; }\n')
            flags = shlex.split(settings['CXXFLAGS']) or ['-std=c++98']
            subprocess.run(cxx+shlex.split(settings['CPPFLAGS'])+flags+[str(source), '-o', str(binary)]+
                           shlex.split(settings['LDFLAGS']), check=True, timeout=60)
            subprocess.run([str(binary)], check=True, timeout=10)
        report = {'system': platform.system(), 'architecture': platform.machine(),
                  'os_release': Path('/etc/os-release').read_text() if Path('/etc/os-release').exists() else None,
                  'python': sys.version.split()[0], 'python_executable': sys.executable,
                  'make': make_version, 'flex': shutil.which('flex'),
                  'generator_sha256': hashlib.sha256(generator.read_bytes()).hexdigest(),
                  'settings': settings, 'compiler_probe_passed': True,
                  'scope': 'Toolchain availability check; simulator correctness requires make test.'}
        if args.record:
            args.record.parent.mkdir(parents=True, exist_ok=True)
            args.record.write_text(json.dumps(report, indent=2)+'\n')
        print(json.dumps(report, indent=2))
    except (OSError, ValueError, subprocess.SubprocessError) as exc:
        parser.exit(1, f'Build environment check failed: {exc}\n')


if __name__ == '__main__':
    main()
