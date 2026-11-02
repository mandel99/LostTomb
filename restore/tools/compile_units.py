"""Compile every generated unit and retain diagnostics; never replace failures."""
import concurrent.futures
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
VSWHERE = Path(os.environ['ProgramFiles(x86)'])/'Microsoft Visual Studio/Installer/vswhere.exe'
VS = Path(subprocess.check_output([str(VSWHERE),'-latest','-products','*','-requires',
    'Microsoft.VisualStudio.Component.VC.Tools.x86.x64','-property','installationPath'],text=True).strip())
TOOLSET = max((VS/'VC/Tools/MSVC').iterdir(), key=lambda p:tuple(map(int,p.name.split('.'))))
CL = TOOLSET/'bin/Hostx64/x86/cl.exe'
SDK = max((Path(os.environ['ProgramFiles(x86)'])/'Windows Kits/10/Include').iterdir(),
          key=lambda p:tuple(map(int,p.name.split('.'))))
INCLUDES = [CL.parents[3]/'include', SDK/'ucrt', SDK/'shared', SDK/'um', SDK/'winrt']
PCH = ROOT/'build/restore_types.pch'


def compile_one(unit):
    source = ROOT / unit['file']
    obj = ROOT / 'build/obj' / (source.stem + '.obj')
    result = subprocess.run([str(CL), '/nologo', '/TC', '/c', '/w', '/I'+str(ROOT/'include'),
                             *['/I'+str(p) for p in INCLUDES],
                             '/Yurestore_types.h', '/Fp'+str(PCH),
                             '/Fo'+str(obj), str(source)], capture_output=True)
    output = result.stdout.decode('utf-8', errors='replace') + result.stderr.decode('utf-8', errors='replace')
    log = ROOT / 'build/logs' / (source.stem + '.log')
    log.write_text(output, encoding='utf-8')
    return dict(name=unit['name'], source=unit['file'], ok=result.returncode == 0, log=str(log.relative_to(ROOT)))


if __name__ == '__main__':
    for p in ['build/obj', 'build/logs']:
        (ROOT/p).mkdir(parents=True, exist_ok=True)
    pch_source = ROOT/'build/pch.c'
    pch_source.write_text('#include "restore_types.h"\n')
    pch_result = subprocess.run([str(CL), '/nologo', '/TC', '/c', '/w', '/I'+str(ROOT/'include'),
        *['/I'+str(p) for p in INCLUDES], '/Ycrestore_types.h', '/Fp'+str(PCH),
        '/Fo'+str(ROOT/'build/pch.obj'), str(pch_source)],capture_output=True)
    if pch_result.returncode:
        sys.stdout.buffer.write(pch_result.stdout+pch_result.stderr)
        sys.exit(pch_result.returncode)
    units = json.loads((ROOT/'build/units.json').read_text())
    if '--failed' in sys.argv and (ROOT/'build/compilation.json').exists():
        failed = {x['source'] for x in json.loads((ROOT/'build/compilation.json').read_text()) if not x['ok']}
        units = [u for u in units if u['file'] in failed]
    results = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        for i, result in enumerate(pool.map(compile_one, units)):
            results.append(result)
            if (i+1)%250 == 0:
                print(f"Compiled {i+1}/{len(units)}; failures {sum(not x['ok'] for x in results)}", flush=True)
    if '--failed' in sys.argv and (ROOT/'build/compilation.json').exists():
        by_source = {x['source']:x for x in results}
        results = [by_source.get(x['source'],x) for x in json.loads((ROOT/'build/compilation.json').read_text())]
    (ROOT/'build/compilation.json').write_text(json.dumps(results,indent=2))
    print(json.dumps(dict(compiled=len(results), failed=sum(not x['ok'] for x in results))))
    sys.exit(any(not x['ok'] for x in results))
