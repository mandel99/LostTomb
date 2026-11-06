"""Extract the assigned frozen25's cached native instructions/current owners."""
from pathlib import Path
import json,hashlib,re,sys,struct
sys.dont_write_bytecode=True
OUT=Path(__file__).resolve().parent;ROOT=OUT.parents[2]
sys.path[:0]=[str(ROOT/'direct_c/cohort3_replay'),str(ROOT/'direct_c')]
from registry import symbol
from native_test_pe import PE
TARGETS=tuple(int(s,16)for s in'416A20 446A30 461950 46A320 46B320 46F030 46F190 476750 47D720 47F200 47FB70 47FC40 482A90 482B50 4867E0 486810 488840 4A2320 4A38A0 4DE200 4E7200 4EB800 4F3690 4F4320 4F4440'.split())
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save(p,v):Path(p).write_bytes((json.dumps(v,indent=2)+'\n').encode())
def main():
 meta=json.loads((ROOT/'metadata/cohort8-source100-local-value-repairs.json').read_bytes());members=[next(m for m in meta['members']if int(m['address'],16)==a)for a in TARGETS]
 profile=json.loads((ROOT/'build/actor-animal-following/cohort8-current-images.json').read_bytes())['compiled_images'];maps={k:(ROOT/v['map_path']).read_text()for k,v in profile.items()};pe=PE(ROOT/'build/smoke-game/Pharaoh.exe')
 rows=[];evidence=[];text=[];sources={}
 for m in members:
  a=int(m['address'],16);recs=[]
  for path in m['native_cache_inputs']:
   d=json.loads((ROOT/path).read_bytes());assert d['native_sha256']==digest(ROOT/'build/smoke-game/Pharaoh.exe');recs +=[r for r in d['entries']if int(r['requested_address'],16)==a]
  assert len(recs)==1;rec=recs[0];assert not rec['assembly']['has_more'];evidence.append(rec);end=int(rec['info']['body_end'],16)
  if a==0x4a38a0:assert end==0x4a3a93 and pe.data[0x4a3a94-pe.base]==0xc3;end=0x4a3a94
  for i in rec['assembly']['items']:
   at=int(i['address'],16)-pe.base;raw=bytes.fromhex(i['bytes']);assert pe.data[at:at+len(raw)]==raw
  src=(ROOT/m['source']).read_text();sig=re.search(r'^([^\n;]*\bFUN_%08x\([^;]*?\))\s*\n'%a,src,re.M).group(1);sources[m['source']]=digest(ROOT/m['source']);providers={}
  for label,t in maps.items():
   hit=symbol(t,m['name'])
   if hit:assert Path(m['source']).stem+'.obj'in hit['owner']
   providers[label]=hit
  assert providers['retained']
  rows.append(dict(address=m['address'],symbol=m['name'],source=m['source'],source_sha256=sources[m['source']],source_signature=sig,providers=providers,owner_object=Path(m['source']).stem+'.obj',native_end_inclusive=end,native_contiguous_scope_sha256=hashlib.sha256(pe.data[a-pe.base:end-pe.base+1]).hexdigest(),native_cache_inputs=m['native_cache_inputs']))
  text+=['\n###'+m['address']+' '+m['source']+' '+sig]+[i['address']+' '+i['bytes']+' '+i['instruction']for i in rec['assembly']['items']]
 constants={}
 for a,n in[(0x5f6bdc,4),(0x5f80fc,8)]:
  raw=bytes(pe.data[a-pe.base:a-pe.base+n*4]);constants[f'{a:08x}']=dict(address=hex(a),bytes_hex=raw.hex(),words_signed=list(struct.unpack('<'+'i'*n,raw)))
 save(OUT/'author-inputs.json',dict(members=rows,source_guards=sources,compiled_images=profile,constant_tables=constants));save(OUT/'native-evidence.json',dict(native_sha256=digest(ROOT/'build/smoke-game/Pharaoh.exe'),members=evidence));(OUT/'native.asm.txt').write_text('\n'.join(text)+'\n',encoding='utf-8')
 print(json.dumps(dict(entries=len(rows),instructions=sum(len(r['assembly']['items'])for r in evidence),retained_only=[r['address']for r in rows if not r['providers']['game']or not r['providers']['diagnostic']])) )
if __name__=='__main__':main()
