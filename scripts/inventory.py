#!/usr/bin/env python3
"""Generate/check offline file provenance and direct header consumers."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
from urllib.parse import quote

ROOT = Path(__file__).resolve().parents[1]
OUTPUTS = {'docs/FILE_INVENTORY.md', 'docs/HEADER_CATALOG.md', 'provenance/file_inventory.json'}
LABELS = {'official_unchanged': '官方原有，当前未改',
          'official_modified': '官方原有，本地修改', 'local_native': '本地新增原生文件',
          'local_project': '本地工程/测试/文档',
          'imported_unverified': '已导入，官方身份未逐文件核验',
          'historical_evidence': '冻结来源/历史证据'}
SOURCE_EXT = {'.h', '.cc', '.cpp', '.l'}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def maintained(path):
    name = path.relative_to(ROOT).as_posix()
    parts = path.relative_to(ROOT).parts
    if parts[0] in {'runs', 'build', '.git'} or any(x == '__pycache__' for x in parts) or name.startswith('Code/output/') and name != 'Code/output/.gitkeep':
        return False
    if name in OUTPUTS or path.name in {'komondor_main', 'komondor_main.cxx', 'compcxx_komondor_main.h', 'lexer.cpp', '.build-config', '.build-config.tmp'}:
        return False
    if path.suffix in {'.o', '.pyc'} or name == 'Code/compcpp/cxx':
        return False
    return True


def header_role(name):
    if name == 'Code/structures/ap_observation.h':
        return '每 AP 独立只读观测 IO/本地报告缓存，禁止汇总全网状态'
    if name == 'Code/methods/mapc/mapc_methods.h':
        return '上游占位；不可当作工作实现'
    if name.startswith('Code/methods/node/node_'):
        return 'Node 实现聚合入口（仅 node.h）' if name.endswith('/node_impl.h') else 'Node 实现片段（仅 node_impl.h）'
    if name.startswith('Code/methods/utils/input_methods/'):
        return 'Komondor 实现片段（主入口按序包含）'
    if name in {'Code/structures/paper_replay.h', 'Code/structures/paper_edca.h', 'Code/structures/tdma_pilot.h', 'Code/structures/dl_cfend.h'}:
        return 'DL/TDMA 现有专用扩展，复用前确认语义'
    if name.startswith('Code/structures/') or name == 'Code/list_of_macros.h':
        return '公共类型/状态/常量，统一定义'
    if name.startswith('Code/methods/phy/'):
        return '公共 PHY 算法（保留现有包含顺序）'
    if name.startswith(('Code/methods/mac/', 'Code/methods/frames/')):
        return '公共 MAC/帧算法（保留现有包含顺序）'
    if name.startswith('Code/methods/utils/'):
        return '公共统计/日志辅助（检查前置依赖）'
    if name.startswith(('Code/COST/', 'Code/compcpp/')):
        return '引擎/生成器依赖'
    if name == 'Code/main/node.h':
        return '公共 Node 组件入口'
    if name == 'Code/main/traffic_generator.h':
        return '公共业务生成组件'
    return '保留的上游学习组件依赖，当前不启用'


def generate():
    audit = json.loads((ROOT/'provenance/comparison.json').read_text())
    imported = json.loads((ROOT/'provenance/import_manifest.json').read_text())['sha256']
    annotations = json.loads((ROOT/'provenance/file_annotations.json').read_text())['files']
    reference = {f['path']: f['official_or_parent_sha256'] for f in audit['base']['files']
                 if f['status'] != 'added'}
    materials = json.loads((ROOT/'provenance/official_materials.json').read_text())
    for name, evidence in materials['files'].items():
        if name in reference and reference[name] != evidence['sha256']:
            raise ValueError(f'Historical/source reference mismatch: {name}')
        reference[name] = evidence['sha256']
        if evidence['action'] != 'excluded' and not (ROOT/name).is_file():
            raise ValueError(f'Missing preserved official material: {name}')
    layers = audit['layers'][:3]
    paths = sorted(p for p in ROOT.rglob('*') if p.is_file() and maintained(p))
    rows = []
    for p in paths:
        name = p.relative_to(ROOT).as_posix()
        current = sha(p)
        native = name.startswith('Code/') and p.suffix in SOURCE_EXT
        official = reference.get(name)
        if official:
            status = 'official_unchanged' if current == official else 'official_modified'
        elif native:
            status = 'local_native'
        elif name.startswith('provenance/') and name not in {'provenance/file_annotations.json', 'provenance/validation.json'}:
            status = 'historical_evidence'
        elif name.startswith('Code/') or name in {'LICENSE', 'CITATION.cff'}:
            status = 'imported_unverified' if name in imported else 'local_project'
        else:
            status = 'local_project'
        if (native or name in {'Code/main/Makefile', 'Code/main/build_local'}) and status in {'official_modified', 'local_native'} and not annotations.get(name):
            raise ValueError(f'Missing file_annotations.json entry: {name}')
        changed_layers = [l['target'] for l in layers if any(
            f['path'] == name and f['status'] != 'identical' for f in l['comparison']['files'])]
        delta = None if name not in imported else current != imported[name]
        rows.append({'path': name, 'status': status, 'sha256': current,
                     'official_sha256': official, 'official_reference_commit': audit['official_commit'] if official else None, 'import_sha256': imported.get(name),
                     'changed_since_import': delta, 'historical_change_layers': changed_layers,
                     'note': annotations.get(name, '')})
    counts = Counter(r['status'] for r in rows)
    native_rows = [r for r in rows if r['path'].startswith('Code/') and Path(r['path']).suffix in SOURCE_EXT]
    native_counts = Counter(r['status'] for r in native_rows)
    payload = {'schema': 'komondor-file-inventory-v1', 'official_reference_commit': audit['official_commit'],
               'official_material_counts': materials['counts'],
               'reference_evidence': 'provenance/comparison.json native reference + provenance/official_materials.json complete fixed-commit tree, verified against Git blob SHA1',
               'classification_note': 'Identical/different bytes to fixed reference, not a proof of authorship or complete correctness. Imported non-source assets without official hashes remain unverified.',
               'excluded': 'runs, Code/output runtime files, build reports/stamps, generated COST output, simulator binary, compiler build objects, these generated inventories (avoid self-hashes)',
               'counts': dict(sorted(counts.items())), 'native_source_counts': dict(sorted(native_counts.items())), 'files': rows}
    md = ['# 当前逐文件来源与修改清单', '', '此文件由 `make inventory` 生成，禁止手工编辑。`make inventory-check` 检查是否过期。', '',
          f"固定官方参照：`{audit['official_commit']}`；只对该历史参照判断，非最新版本声明，非本地 checkout 祖先声明。", '',
          '文件身份与功能归属分开：官方文件内可有本地修改；本地修改不表示整份算法由我们实现。每个文件的当前/官方/导入 SHA256 和修改层次见 [机器清单](../provenance/file_inventory.json)。', '',
          '## 原生源码统计', '', '|身份|文件数|', '|---|---:|']
    md += [f'|{LABELS[k]}|{v}|' for k,v in sorted(native_counts.items())]
    md += ['', '统计仅包含 Code 下非生成 `.h/.cc/.cpp/.l`。构建文件、工具、配置、Python、测试与文档另列。', '',
           '## 官方原有、本地修改 / 本地新增原生文件', '', '|文件|身份|改动层次|当前说明|', '|---|---|---|---|']
    def link(name):
        return f'[{name}](<../{quote(name, safe="/")}>)'
    def stages(r):
        parts = [s.removeprefix('komondor-').removesuffix('-src') for s in r['historical_change_layers']]
        if r['changed_since_import']:
            parts.append('独立化后')
        return ' → '.join(parts) or '—'
    for r in native_rows:
        if r['status'] != 'official_unchanged':
            md.append(f"|{link(r['path'])}|{LABELS[r['status']]}|{stages(r)}|{r['note']}|")
    md += ['', '层次来自冻结的逐层比较；不是提交历史，也不是行级作者认定。完整已有修改见 [CHANGES.md](CHANGES.md)。', '',
           '## 官方原有、当前源码与参照相同', '', '|文件|身份|', '|---|---|']
    for r in native_rows:
        if r['status'] == 'official_unchanged':
            md.append(f"|{link(r['path'])}|{LABELS[r['status']]}|")
    md += ['', '## 官方配套材料及本地构建、工具、配置、测试与文档', '', '|文件|身份|说明|', '|---|---|---|']
    for r in rows:
        if r not in native_rows:
            note = r['note'] or ('保留导入文件；历史审计范围未覆盖，不能认定与官方字节相同。' if r['status']=='imported_unverified' else '')
            md.append(f"|{link(r['path'])}|{LABELS[r['status']]}|{note}|")
    md += ['', '生成产物 `komondor_main.cxx` / `compcxx_komondor_main.h`、仿真二进制、运行结果及生成器编译中间文件不作为人工维护源码。三个自动清单也不记录自己的哈希，以避免循环身份。', '',
           '官方原有功能包括协作状态机、Co-SR 四种功率策略、ULA/ZF、EDCA/DCB 等；文件改动不能将这些能力重新归为本地新增。未核验的导入文件也不能自动归为本地原创。', '']
    headers = [r for r in native_rows if r['path'].endswith('.h')]
    consumers = {r['path']: [] for r in headers}
    for r in rows:
        if Path(r['path']).suffix not in SOURCE_EXT:
            continue
        p = ROOT/r['path']
        for target in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', p.read_text(errors='replace'), re.M):
            resolved = (p.parent/target).resolve()
            if resolved.is_relative_to(ROOT):
                target_name = resolved.relative_to(ROOT).as_posix()
                if target_name in consumers:
                    consumers[target_name].append(r['path'])
    # Implementation fragments must have exactly one intended direct inclusion.
    for name, users in consumers.items():
        allowed = None
        if name == 'Code/methods/node/node_impl.h':
            allowed = 'Code/main/node.h'
        elif name.startswith('Code/methods/node/node_'):
            allowed = 'Code/methods/node/node_impl.h'
        elif name.startswith('Code/methods/utils/input_methods/'):
            allowed = 'Code/main/komondor_main.cc'
        if allowed is not None and users != [allowed]:
            raise ValueError(f'Implementation include rule violated: {name}: {users}; expected [{allowed}]')
    catalog = ['# 全部头文件职责、来源与直接使用者', '',
               '由 `make inventory` 静态扫描当前原生源码和 C++ 测试生成；不含 COST 生成文件，不含系统头。直接包含者不是完整传递依赖，也不表示运行时已启用。', '',
               '公共复用约束见 [SHARED_HEADERS.md](SHARED_HEADERS.md)。无直接包含者可能是保留依赖/占位，不能据此自动删除。', '',
               '|头文件|当前来源|职责/包含边界|直接包含者|', '|---|---|---|---|']
    for r in headers:
        users = '<br>'.join(link(n) for n in sorted(set(consumers[r['path']]))) or '无源码/测试直接包含者'
        catalog.append(f"|{link(r['path'])}|{LABELS[r['status']]}|{header_role(r['path'])}|{users}|")
    catalog += ['']
    return {'provenance/file_inventory.json': json.dumps(payload, ensure_ascii=False, indent=2)+'\n',
            'docs/FILE_INVENTORY.md': '\n'.join(md), 'docs/HEADER_CATALOG.md': '\n'.join(catalog)}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--check', action='store_true')
    args = p.parse_args()
    try:
        outputs = generate()
    except ValueError as exc:
        p.exit(1, str(exc)+'\n')
    stale = []
    for name, content in outputs.items():
        target = ROOT/name
        if args.check:
            if not target.exists() or target.read_text() != content:
                stale.append(name)
        else:
            target.write_text(content)
    if stale:
        p.exit(1, 'Inventory stale; review changes and run make inventory: '+', '.join(stale)+'\n')
    print('Inventory and header catalog are current.' if args.check else 'Updated inventory and header catalog.')


if __name__ == '__main__':
    main()
