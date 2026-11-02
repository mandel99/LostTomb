from pathlib import Path
import hashlib,json,re,struct
R=Path.cwd();H=R/'artifacts/map_refresh_postpass8_phase';b=(R/'build/smoke-game/Pharaoh.exe').read_bytes();pin='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d';assert hashlib.sha256(b).hexdigest()==pin;u16=lambda a:struct.unpack_from('<H',b,a)[0];u32=lambda a:struct.unpack_from('<I',b,a)[0];pe=u32(60);o=pe+24;t=o+u16(pe+20);base=u32(o+28)
def original(a,n):
 r=a-base
 for j in range(u16(pe+6)):
  q=t+j*40;v=u32(q+12);sz=u32(q+16)
  if v<=r and r+n<=v+sz:return b[u32(q+20)+r-v:u32(q+20)+r-v+n]
 raise AssertionError(hex(a))
windows=[];provenance={}
for p in (R/'metadata/native-function-audit-cache').glob('batch-*.json'):
 d=json.loads(p.read_text());assert d['native_sha256']==pin
 for e in d['entries']:
  if e.get('status')!='exact_ghidra_function':continue
  items=e['assembly']['items']
  for k,it in enumerate(items):
   if re.fullmatch(r'CALL 0x0047a220',it['instruction'],re.I):
    selected=items[max(k-16,0):k+9]
    for x in selected:
     raw=bytes.fromhex(x['bytes']);assert original(int(x['address'],16),len(raw))==raw
    windows.append({'caller':e['requested_address'],'call':it['address'],'window':selected,'complete_caller_instructions':len(items),'complete_caller_verified_by_this_window':False});provenance[p.relative_to(R).as_posix()]=hashlib.sha256(p.read_bytes()).hexdigest()
report={'format':1,'native_sha256':pin,'original_process_executed':False,'canonical_entry':'0047a220','native_ABI':'void cdecl(int32 x,int32 y,int32 set); incomingECXunused, RET0, callerdiscards consumedbyvalue slots','actual_C_entry':'void FUN_0047a220(int32_t,int32_t,int32_t)','cached_incoming_call_windows':windows,'cache_hashes':provenance,'C_cases':0,'qualification':'Static ABI/callsite byte evidence only, not whole caller algorithm proof. Catalogue includes Ghidra function objects only; no closed whole-game incoming catalogue claim.'}
(H/'postpass8-abi.json').write_text(json.dumps(report,indent=2)+'\n');print(len(windows),[(w['caller'],w['call']) for w in windows])
