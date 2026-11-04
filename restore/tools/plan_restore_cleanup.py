"""Plan local generated-output cleanup without touching reconstruction sources.

The plan is data for Cleanup-Restore.ps1. Referenced files, tracked files and
active builds are protected. No game, compiler or external program is launched.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import stat
import subprocess

ROOT = Path(__file__).resolve().parents[1]
ACTIVE = ('build/c-only', 'build/smoke-game', 'build/offline-verification-deps',
          'build/music-configuration-cmake-epochs', 'build/cleanup-20261104')
INTERMEDIATE = {'.pdb', '.ilk', '.obj', '.lib', '.idb', '.pch', '.ipdb', '.iobj',
                '.tlog', '.lastbuildstate', '.recipe', '.res', '.pyc', '.pyo'}
BUILD_PRODUCT = {'.exe', '.map', '.sln', '.slnx', '.vcxproj', '.filters', '.cmake',
                 '.exp', '.manifest', '.aps'}
REGENERABLE_EXPORT = 'build/bulk-type-batch/runs/known4527_guarded/facts'
REGENERABLE_PROJECT = 'build/bulk-type-batch/runs/known4527_guarded/project'


def inside(path, parent):
    return path == parent or path.startswith(parent + '/')


def plan(output):
    tracked = subprocess.check_output(['git', 'ls-files', '-z'], cwd=ROOT.parent)
    tracked = [x for x in tracked.decode('utf-8').split('\0') if x]
    protected = {p[len('restore/'):] for p in tracked if p.startswith('restore/')}
    reasons = {p: 'tracked source/evidence' for p in protected}

    def protect(candidate, reason):
        try:
            p = Path(candidate)
            if not p.is_absolute():
                p = ROOT / p
            rel = p.resolve().relative_to(ROOT).as_posix()
            if p.is_file():
                protected.add(rel)
                reasons.setdefault(rel, reason)
        except (ValueError, OSError):
            pass

    def json_paths(value):
        if isinstance(value, dict):
            for k, v in value.items():
                json_paths(k)
                json_paths(v)
        elif isinstance(value, list):
            for v in value:
                json_paths(v)
        elif isinstance(value, str) and (value.startswith(('build/', 'artifacts/', str(ROOT)))
                                         or value.lower().startswith(str(ROOT).lower())):
            protect(value, 'path retained in source/evidence')

    # Read project-owned text only, never generated CMake self-references.
    pattern = re.compile(r'\b(?:build|artifacts)/[^\s\x22\x27`<>\[\]{};|,)]+')
    inputs = {ROOT.parent / name for name in tracked}
    # Older phase reports are local artifacts, including their binary hashes.
    # Protect those references too, even when the reports are Git-ignored.
    for base in [ROOT / 'metadata', ROOT / 'artifacts']:
        inputs.update(base.rglob('*.json'))
    for p in sorted(inputs):
        if p.suffix.lower() not in {'.json', '.py', '.ps1', '.md', '.txt', '.c', '.h',
                                   '.mjs', '.js', '.yml', '.yaml', '.asm', '.inc'}:
            continue
        try:
            text = p.read_text(encoding='utf-8-sig')
        except (OSError, UnicodeError):
            continue
        if p.suffix.lower() == '.json':
            try:
                json_paths(json.loads(text))
            except ValueError:
                pass
        normalized = re.sub(r'\\+', '/', text)
        for match in pattern.findall(normalized):
            protect(match.rstrip('.:'), 'literal source/evidence dependency')

    # Differential drivers derive the MAP filename from the retained EXE.
    for rel in list(protected):
        if rel.lower().endswith('.exe'):
            protect(Path(rel).with_suffix('.map'), 'MAP companion of retained executable')

    files = {}
    reparse = []
    for directory, dirs, names in os.walk(ROOT, followlinks=False):
        for name in list(dirs):
            p = Path(directory) / name
            if p.lstat().st_file_attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT:
                reparse.append(p.relative_to(ROOT).as_posix())
                dirs.remove(name)
        for name in names:
            p = Path(directory) / name
            info = p.lstat()
            rel = p.relative_to(ROOT).as_posix()
            if info.st_file_attributes & stat.FILE_ATTRIBUTE_REPARSE_POINT:
                reparse.append(rel)
                continue
            files[rel] = info.st_size

    def pinned(rel):
        return rel in protected or any(inside(rel, p) for p in ACTIVE)

    selected = {}
    for rel, size in files.items():
        if pinned(rel):
            continue
        parts = rel.split('/')
        if '__pycache__' in parts:
            selected[rel] = 'Python cache'
        elif inside(rel, REGENERABLE_EXPORT):
            selected[rel] = 'unapplied optional typed-fact export; retained native PE can regenerate it'
        elif inside(rel, REGENERABLE_PROJECT):
            selected[rel] = 'closed optional typed-export Ghidra project'
        elif rel.startswith(('build/', 'artifacts/')):
            if any(p.endswith('.dir') or p == 'CMakeFiles' for p in parts[:-1]):
                selected[rel] = 'obsolete CMake intermediate directory'
            elif Path(rel).suffix.lower() in INTERMEDIATE:
                selected[rel] = 'unreferenced compiler/linker intermediate'
            elif Path(rel).suffix.lower() in BUILD_PRODUCT:
                selected[rel] = 'unreferenced old build product'
            elif rel.endswith('/CMakeCache.txt'):
                selected[rel] = 'obsolete CMake cache'
        elif len(parts) == 1 and Path(rel).suffix.lower() in INTERMEDIATE:
            selected[rel] = 'orphan root compiler intermediate'

    # Collapse only entirely selected trees; sources and protected files block it.
    descendant_count = {}
    selected_count = {}
    for rel in files:
        parts = rel.split('/')
        for n in range(1, len(parts)):
            directory = '/'.join(parts[:n])
            descendant_count[directory] = descendant_count.get(directory, 0) + 1
            if rel in selected:
                selected_count[directory] = selected_count.get(directory, 0) + 1
    collapsed = []
    for directory in sorted(descendant_count, key=lambda p: (p.count('/'), p)):
        if selected_count.get(directory) != descendant_count[directory]:
            continue
        if any(inside(directory, p) for p in collapsed):
            continue
        if any(inside(p, directory) for p in reparse):
            continue
        collapsed.append(directory)
    targets = [dict(path=d, kind='directory') for d in collapsed]
    targets += [dict(path=p, kind='file') for p in sorted(selected)
                if not any(inside(p, d) for d in collapsed)]
    total = sum(files[p] for p in selected)
    by_reason = {}
    for p, reason in selected.items():
        item = by_reason.setdefault(reason, dict(files=0, bytes=0))
        item['files'] += 1
        item['bytes'] += files[p]
    retained = {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest()
                for p in sorted(protected) if (ROOT / p).is_file()}
    report = dict(root=str(ROOT),delete_bytes=total,delete_files=len(selected),
                  targets=targets,by_reason=by_reason,active_prefixes=ACTIVE,
                  protected_files_sha256=retained,reparse_paths_skipped=reparse,
                  removed_facts_are_regenerable_and_unapplied=True)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2) + '\n')
    print(f'Plan: {len(selected)} files, {total / 1024**3:.3f} GiB, '
          f'{len(collapsed)} complete generated directories; {len(retained)} protected files')
    for reason, item in by_reason.items():
        print(f'{item["bytes"] / 1024**3:.3f} GiB {item["files"]:6} {reason}')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'build/cleanup-20261104/plan.json')
    plan(parser.parse_args().output.resolve())
