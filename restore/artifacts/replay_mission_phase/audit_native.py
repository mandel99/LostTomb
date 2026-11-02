"""Pinned-byte CFG, profile-limit and concrete existing C-binding audit; no Ghidra mutation."""
from pathlib import Path
import hashlib,json,re,struct,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sec=opt+struct.unpack_from('<H',raw,pe+20)[0]
sections=[struct.unpack_from('<IIII',raw,sec+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for v,va,z,off in sections:
  if base+va<=a and a+n<=base+va+z:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
ui=json.loads((PHASE/'profile-ui-native.json').read_text());assert ui['native_sha256']==digest
cs=Cs(CS_ARCH_X86,CS_MODE_32);facts={}
for entry in ['00418510','004181e0','005169c0']:
 f=packet['functions'][entry];assert not f['assembly'].get('has_more',False);rows=f['assembly']['items'];body={int(i['address'],16):i for i in rows};calls=[];edges=[];returns=[]
 for a,i in body.items():
  encoded=bytes.fromhex(i['bytes']);assert take(a,len(encoded))==encoded;d=next(cs.disasm(encoded,a));next_address=a+d.size
  if d.mnemonic.startswith('ret'):assert encoded==b'\xc3';returns.append(f'{a:08x}');continue
  if d.mnemonic=='call':calls.append({'site':f'{a:08x}','target':f'{int(d.op_str,16):08x}'});assert next_address in body
  elif d.mnemonic.startswith('j'):
   target=int(d.op_str,16);assert target in body;edges.append([f'{a:08x}',f'{target:08x}'])
   if d.mnemonic!='jmp':assert next_address in body;edges.append([f'{a:08x}',f'{next_address:08x}'])
  else:assert next_address in body
 facts[entry]={'instruction_count':len(body),'body_ranges':[[rows[0]['address'],f'{max(body)+len(bytes.fromhex(body[max(body)]["bytes"]))-1:08x}']],'direct_calls':calls,'branch_edges':edges,'ret0_sites':returns,'unresolved_control_flow':[],'entry_contract':'cdecl, no pushed arguments or implicit input GPR; EBX/EBP/ESI/EDI saved when used','result':'full pointer EAX' if entry=='005169c0' else 'void semantic role; incidental EAX ignored'}
for f in [*packet['functions'].values(),ui['function']]:
 assert not f['assembly'].get('has_more',False)
 for i in f['assembly']['items']:assert take(int(i['address'],16),len(bytes.fromhex(i['bytes'])))==bytes.fromhex(i['bytes'])
window=ui['profile_registration_window'];assert any(i['address']=='00540bdc' and i['bytes']=='6a1f' for i in window);assert any(i['address']=='00540bde' and i['bytes']=='68748ee300' for i in window)
old=ROOT/'src/unit_00222.c';before=sha(old.read_bytes());prior=PHASE/'prior_source/unit_00222.c';prior.parent.mkdir(exist_ok=True);prior.write_bytes(old.read_bytes())
consumers=[]
for file in (ROOT/'metadata').glob('*emulation*.json'):
 if file.name=='replay-mission-emulation.json':continue
 try:data=json.loads(file.read_text())
 except (ValueError,UnicodeError):continue
 if isinstance(data,dict) and any(key=='src/unit_00222.c' or key.endswith('/src/unit_00222.c') for key in data.get('source_hashes',{})):consumers.append(str(file.relative_to(ROOT)))
bindings={
 '005169C0':{'source':'src/unit_02282.c','C':'char *FUN_005169c0(void)','native':'38 instructions, no args RET0, EAX EADEA4. Real unchanged getter copies literal save\\ then actual E38E74 profile.'},
 '00561A0A':{'source':'src/unit_02802.c','C':'undefined4 *FUN_00561a0a(undefined4 path,undefined4 mode)','native':'6 instructions, cdecl two DWORD pointer arguments RET0. Forwards path/mode plus40 to5619EA. Existing C integer pointer-sized ABI is preserved exactly; result converted as object pointer to restore_legacy_FILE*.'},
 '00561994':{'source':'src/unit_02800.c','C':'undefined4 FUN_00561994(restore_legacy_FILE *stream)','native':'35 instructions, cdecl one pointer RET0, full EAX ignored by replay. Native flag offset12/file16 match original32-byte FILE wrapper, not host FILE.'},
 '004D51B0':{'source':'src/unit_01827.c','C':'undefined4 FUN_004d51b0(undefined4 filename)','native':'62 instructions, first argument fetched at4D51CC [ESP+C] after two saves; only caller-input stack fetch in body. All returns RET0. Replay passes one filename DWORD pointer; result ignored.'},
 '005405B0':{'source':'src/unit_02537.c','C':'void FUN_005405b0(char *name,char kind)','native':'275 instructions, E37078 pointer first DWORD; second argument consumed BYTE. RET0.'},
 '004CB6A0':{'source':'src/unit_01654.c','C':'void FUN_004cb6a0(void)','native':'104 instructions, campaign fallback; not replaced by a C# missing-file notice.'},
 '0041AA20':{'source':'src/unit_00238.c','C':'void FUN_0041aa20(undefined4 first,undefined4 mode,int value)','native':'Caller4181E0 pushes1000,53,0; caller cleans12. Endpoint is named, no instruction algorithm claim.'},
 '00418140':{'source':'src/unit_00220.c','C':'int FUN_00418140(void)','native':'Caller4181E0 consumes full raw EAX as DWORD D3B124. C int result is converted to uint32 without narrowing. Endpoint named; no algorithm claim.'}}
for value in bindings.values():value['source_sha256']=sha((ROOT/value['source']).read_bytes())
report={'format':1,'native_sha256':digest,'shared_ghidra_mutated':False,'functions':facts,'source222_before_sha256':before,'existing_proofs_hashing_source222':consumers,'bindings':bindings,'profile_domain':{'storage':'32 bytes E38E74..E38E93, initialized as8DWORDs by actual family flows','native_ui_function':'00540B50','registration_max':'31 from PUSH1F at540BDC and pointerE38E74 at540BDE before4CC560','length_domain':'NUL-terminated0..31byte name; core valid-name corpus uses letters,digits,spaces,_,-,.','prefix_hex':'736176655c','max_getter_bytes_including_NUL':37,'separator_hex':'5c','autosave_hex':'6175746f736176655f7265706c61792e736176','max_replay_bytes_including_NUL':57,'native_filename_buffer_bytes':100,'policy':'No production truncation, fallback or guard for out-of-domain malformed unterminated names is invented.'},'stack_note':'Fclose and loader are two separate one-argument cdecl calls. Native retains the old fclose argument until common ADD ESP,8; this does not make4D51B0 a two-argument function. Adapter endpoints each pop only return address.','limitations':['Native caller xrefs were not globally re-exported in this phase. Existing helper ABI is retained, callback418510 remains a zero-argument data-table role. No absence-of-callers inference is made.','The seven declared deep endpoints remain controlled for deterministic equivalence; source identity/prototype audit does not prove their whole algorithms.','No previous native proof hashes source222; old campaign callers explicitly control4181E0. Root may retain those old reports unchanged and rerun the new suite on applied source.'],'input_hashes':{str(file.relative_to(ROOT)).replace('\\','/'):sha(file.read_bytes()) for file in [PHASE/'retained-native.json',PHASE/'profile-ui-native.json',ROOT/'metadata/dropdown-action-native-cfg.json',Path(__file__)]}}
(PHASE/'native-contracts.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'complete_owned_and_profile_instructions':sum(x['instruction_count'] for x in facts.values()),'source222_before_sha256':before,'old_consumers':consumers,'all_retained_native_bytes_authenticated':True},indent=2))
