"""Versioned private batch guards/inventory. No production apply operation."""
import hashlib,json,os,re,struct,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PRIVATE=ROOT/'build/bulk-type-batch';PIN='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
def sha(path):
 h=hashlib.sha256()
 with Path(path).open('rb') as f:
  for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
 return h.hexdigest()
def confined(path):
 q=Path(path).resolve()
 if not q.is_relative_to(PRIVATE.resolve()) or q==PRIVATE.resolve():raise ValueError('Output must be strictly inside build/bulk-type-batch')
 return q
def root_snapshot(previous=None):
 from concurrent.futures import ThreadPoolExecutor
 start=time.time();paths=[]
 for directory,dirs,files in os.walk(ROOT):
  dirs[:]=[d for d in dirs if d not in ['.git','__pycache__'] and Path(directory)/d not in [PRIVATE,ROOT/'tools/bulk_type_export_v2']]
  paths.extend(Path(directory)/name for name in files)
 def inspect(p):
  rel=p.relative_to(ROOT).as_posix()
  try:
   stat=p.stat();old=(previous or {}).get(rel)
   if old and old.get('stable_read') and old.get('size')==stat.st_size and old.get('mtime_ns')==stat.st_mtime_ns:row=dict(old);row['hash_reused_for_unchanged_stat']=True
   else:row={'size':stat.st_size,'mtime_ns':stat.st_mtime_ns,'sha256':sha(p),'hash_reused_for_unchanged_stat':False}
   after=p.stat();row['stable_read']=after.st_size==stat.st_size and after.st_mtime_ns==stat.st_mtime_ns
  except OSError as error:row={'read_error':str(error)}
  return rel,row
 result={}
 with ThreadPoolExecutor(max_workers=8) as pool:
  for n,(rel,row) in enumerate(pool.map(inspect,paths),1):
   result[rel]=row
   if n%10000==0:print('Protected root snapshot',n,'files',round(time.time()-start,1),'seconds',flush=True)
 return result
def differences(before,after):return [{'path':p,'before':before.get(p),'after':after.get(p),'pilot_mutated':False} for p in sorted(before.keys()|after.keys()) if before.get(p,{}).get('sha256')!=after.get(p,{}).get('sha256') or 'read_error'in before.get(p,{}) or 'read_error'in after.get(p,{})]
def create_seed(run):
 units=json.loads((ROOT/'units.json').read_text());mapping={int(q['provenance']['address'],16):q['file'] for q in units if q.get('provenance')};protected=set();protected_sources=set();evidence=[]
 for p in (ROOT/'metadata').glob('*emulation*.json'):
  try:d=json.loads(p.read_text(encoding='utf-8-sig'))
  except (ValueError,OSError):continue
  if isinstance(d,dict) and d.get('passed'):
   for address in d.get('functions',{}):
    if re.fullmatch(r'(?:0x)?[0-9a-fA-F]{8}',str(address)):protected.add(int(address,16))
   for source,h in d.get('source_hashes',{}).items():
    q=ROOT/source
    if q.is_file() and q.resolve().is_relative_to(ROOT) and sha(q)==h:protected_sources.add(q.resolve())
 for entry,path in mapping.items():
  if (ROOT/path).resolve() in protected_sources:protected.add(entry)
 for p in (ROOT/'artifacts').rglob('*.c'):
  if '_phase' not in p.as_posix():continue
  if p.name.startswith('unit_'):
   try:i=int(p.stem.split('_')[1]);u=units[i];protected.add(int(u['provenance']['address'],16))
   except (ValueError,IndexError,KeyError):pass
  text=p.read_text(encoding='utf-8-sig',errors='replace')
  for n in re.findall(r'(?m)^\w[^;\n]*\b\w*?([0-9a-fA-F]{8})\s*\([^;\n]*\)\s*(?:\n\s*)?\{',text):protected.add(int(n,16))
 approvals=json.loads((ROOT/'metadata/full-function-review-approvals.json').read_text())
 for q in approvals.get('functions',[])+approvals.get('additional_native_entries',[]):
  if q.get('address'):protected.add(int(q['address'],16))
 image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();assert hashlib.sha256(image).hexdigest()==PIN;nt=struct.unpack_from('<I',image,60)[0];sec=nt+24+struct.unpack_from('<H',image,nt+20)[0];sections=[struct.unpack_from('<IIII',image,sec+i*40+8) for i in range(struct.unpack_from('<H',image,nt+6)[0])]
 def raw(a,n):
  r=a-0x400000
  for vs,rv,rs,ro in sections:
   if rv<=r<rv+max(vs,rs):return image[ro+r-rv:ro+r-rv+n]
  raise ValueError(hex(a))
 overrides=json.loads((ROOT/'tools/bulk_type_export_v2/protection_overrides.json').read_text());protected.update(int(q,16) for q in overrides['explicit_protected_entries']);conflicts={q['entry']:q for q in overrides.get('boundary_conflicts',[])}
 functions=[];total=0
 for p in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
  d=json.loads(p.read_text());assert d['native_sha256']==PIN;evidence.append({'path':p.relative_to(ROOT).as_posix(),'sha256':sha(p)})
  for q in d['entries']:
   assert q['status']=='exact_ghidra_function';a=q['assembly'];assert not a['has_more'] and a['total']==len(a['items']);entry=int(q['requested_address'],16);ranges=[];expected=[]
   for ins in sorted(a['items'],key=lambda v:int(v['address'],16)):
    pc=int(ins['address'],16);b=bytes.fromhex(ins['bytes']);assert raw(pc,len(b))==b;expected.append([ins['address'],ins['bytes']]);end=pc+len(b)-1
    if ranges and pc==int(ranges[-1][1],16)+1:ranges[-1][1]=f'{end:08x}'
    else:ranges.append([f'{pc:08x}',f'{end:08x}'])
   functions.append({'entry':f'{entry:08x}','unit':mapping.get(entry),'protected_function':entry in protected,'body_ranges':ranges,'expected_instructions':expected,'native_instruction_count':len(expected),'cache_source':p.relative_to(ROOT).as_posix(),'boundary_conflict':conflicts.get(f'{entry:08x}')});total+=len(expected)
 assert len(functions)==len({q['entry'] for q in functions})==4527
 assert set(mapping)<=set(int(q['entry'],16) for q in functions),'Unit entry absent from authenticated cache'
 seed={'native_sha256':PIN,'known_cache_only':True,'new_callback_discovery':False,'functions':sorted(functions,key=lambda q:q['entry']),'native_instructions_pinned':total,'cache_evidence':evidence,'unit_mapping_sha256':sha(ROOT/'units.json'),'protected_functions':[f'{q:08x}' for q in sorted(protected)],'protected_sources':[p.relative_to(ROOT).as_posix() for p in sorted(protected_sources)],'deferred_actor_seed_inventory':{'path':overrides['future_actor_seed_inventory'],'sha256':sha(ROOT/overrides['future_actor_seed_inventory']),'additional_actor_entries_seeded':False},'boundary_conflicts':list(conflicts.values())};(run/'seed.json').write_text(json.dumps(seed,separators=(',',':'))+'\n');return seed
