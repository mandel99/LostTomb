"""Compact read-only triage of legacy C bodies present in the link map.

Patterns are review candidates, never sufficient authority to rewrite types.
No decompiler dumps, executables or per-instruction reports are generated.
"""
from pathlib import Path
import argparse,collections,hashlib,json,re
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--map',type=Path,default=ROOT/'build/c-only/Release/PharaohRestoreC.map')
p.add_argument('--limit',type=int,default=20)
p.add_argument('--output',type=Path)
a=p.parse_args()
rows=[];counts=collections.Counter();scanned=0
units={row['file']:row['name'] for row in json.loads((ROOT/'units.json').read_text())}
linked=set(re.findall(r'pharaoh_restored:(unit_\d+\.obj)',a.map.read_text()))
for obj in sorted(linked):
    path=ROOT/'src'/obj.replace('.obj','.c')
    if not path.exists():continue
    src=path.read_text(encoding='utf-8-sig');body=re.sub(r'^\s*#.*$','',src,flags=re.M)
    scanned+=1;hits=[];score=0
    for m in re.finditer(r'^#define\s+(DAT_\w+)\s+\(\*\(([^)]*)\*\)0x[0-9a-f]+u\)',src,re.M):
        name,typ=m[1],m[2].strip()
        if typ in ['undefined4','int','uint']:
            for line in body.splitlines():
                if re.search(r'&'+name+r'\s*\+[^;\n]*\*',line):
                    hits.append(('scaled_address',name));score+=5;break
        if typ in ['undefined','undefined1']:
            # Address-only bases are deliberately excluded. Scalar reads/writes
            # still require same-function native-width evidence before repair.
            scalar=re.sub(r'&'+name+r'\b','',body)
            if re.search(r'\b'+name+r'\b',scalar):
                hits.append(('byte_scalar',name));score+=1
                if name in ['DAT_006059e8','DAT_0070e488','DAT_00bca304','DAT_00bca308','DAT_0070e120']:score+=2
    if not hits:continue
    kinds=collections.Counter(k for k,n in hits);counts.update(kinds.keys())
    file=str(path.relative_to(ROOT)).replace('\\','/')
    rows.append(dict(function=units.get(file),file=file,score=score,
        patterns=dict(kinds),examples=[n for k,n in hits[:5]],source_sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
rows.sort(key=lambda r:(-r['score'],r['file']))
report=dict(format=1,status='heuristic triage; no blanket patch approval',
    scope='linked object presence does not prove runtime reachability',
    linked_legacy_bodies_scanned=scanned,candidate_functions=len(rows),
    functions_by_pattern=dict(counts),shown=min(a.limit,len(rows)),priorities=rows[:a.limit])
if a.output:a.output.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
