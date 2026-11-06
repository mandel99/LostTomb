"""Shared read-only cached result-use windows for the assigned25 entries."""
from pathlib import Path
import json,re,hashlib,sys
sys.dont_write_bytecode=True
from author_inputs import TARGETS,ROOT,OUT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 calls={a:{}for a in TARGETS};files=[]
 for p in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
  d=json.loads(p.read_bytes());files.append(dict(path=p.relative_to(ROOT).as_posix(),sha256=digest(p)))
  for r in d['entries']:
   ii=r.get('assembly',{}).get('items',[])
   for j,i in enumerate(ii):
    match=re.fullmatch(r'CALL 0x([0-9a-fA-F]+)',i['instruction'])
    if not match or int(match.group(1),16)not in calls:continue
    a=int(match.group(1),16);calls[a].setdefault(i['address'],dict(caller_entry=r['requested_address'],cache=p.relative_to(ROOT).as_posix(),window=ii[max(0,j-3):j+15]))
 out=dict(cache_inputs=files,members=[dict(entry=f'{a:08x}',direct_calls=list(calls[a].values()))for a in TARGETS]);(OUT/'consumer-evidence.json').write_bytes((json.dumps(out,indent=2)+'\n').encode())
 for a in TARGETS:
  print(f'{a:08x} callers={len(calls[a])}')
  if a in(0x446a30,0x46f190,0x4867e0,0x486810,0x4a2320,0x4eb800,0x4f3690,0x4f4320,0x4f4440):
   for c in calls[a].values():
    w=c['window'];j=next(j for j,i in enumerate(w)if i['instruction']==f'CALL 0x{a:08x}');print(' | '.join(i['address']+' '+i['instruction']for i in w[j:j+5]))
if __name__=='__main__':main()
