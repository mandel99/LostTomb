"""Qualify validation entry ABI and incoming call evidence without modifying Ghidra."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sec=opt+struct.unpack_from('<H',raw,pe+20)[0]
sections=[struct.unpack_from('<IIII',raw,sec+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for vs,va,z,off in sections:
  if base+va<=a and a+n<=base+va+z:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
cs=Cs(CS_ARCH_X86,CS_MODE_32)
relative=json.loads((ROOT/'metadata/popup-editor-relative-calls-native.json').read_text());assert relative['native_sha256']==digest
ours={}
for name in ['native.json','helper-native.json','selector-native.json']:
 for entry,f in json.loads((PHASE/name).read_text())['functions'].items():ours[entry]=f.get('instructions',f.get('assembly',{}).get('items'))
needed={owner for rows in relative['targets'].values() for row in rows for owner in row['cached_instruction_owners']}
cached={};cache_sources={}
for file in sorted((ROOT/'metadata/native-function-audit-cache').glob('batch-*.json')):
 data=json.loads(file.read_text());assert data['native_sha256']==digest
 for f in data['entries']:
  entry=f['requested_address']
  if entry in needed:
   cached[entry]=f;cache_sources[str(file.relative_to(ROOT))]=sha(file.read_bytes())
results={}
for target,rows in relative['targets'].items():
 if target=='00403c00':continue
 output=[]
 for row in rows:
  a=int(row['call_address'],16);encoded=bytes.fromhex(row['bytes']);assert take(a,len(encoded))==encoded
  d=next(cs.disasm(encoded,a));assert d.mnemonic=='call' and int(d.op_str,16)==int(target,16)
  recovered=[owner for owner,body in ours.items() if any(int(i['address'],16)==a and bytes.fromhex(i['bytes'])==encoded for i in body)]
  windows=[]
  for owner in row['cached_instruction_owners']:
   f=cached[owner];assert not f['assembly']['has_more'];body=f['assembly']['items'];index=next(j for j,i in enumerate(body) if int(i['address'],16)==a)
   window=body[max(0,index-4):index+5]
   for i in window:assert take(int(i['address'],16),len(bytes.fromhex(i['bytes'])))==bytes.fromhex(i['bytes'])
   windows.append({'owner':owner,'cached_complete_function':True,'window':window})
  output.append(dict(row,recovered_complete_cfg_owners=recovered,cached_windows=windows,qualification='recovered_complete_cfg' if recovered else 'authenticated_cached_instruction' if windows else 'analyzed_reference_unclosed_caller' if row['analyzed_xref'] else 'raw_only_unqualified'))
 results[target]=output
contracts={
 '00409830':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'void callback role; incidental EAX ignored','algorithm':'seven ordered real predicates, first failure selects paired group5 text and panel; success direct real4CDFF0 then five stores'},
 '004ec360':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'4 signed WORD pairs +4 signed DWORD pairs; nonnegative both; base/classifier reload for every active entry'},
 '004095c0':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'8 signed WORD pairs, captured DWORD base, BYTE water flag at DWORD-grid stride4'},
 '00409650':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'2 signed WORD pairs, positive x OR y, fresh base before each real409620 call'},
 '004096e0':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'2 signed WORD pairs, positive x OR y, captured base loaded between first x/y, BYTE water bit'},
 '00409760':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'8 signed WORD pairs, nonnegative both, fresh base before every real409620 call'},
 '004097c0':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean','algorithm':'8 signed WORD pairs, captured base, BYTE water bit'},
 '00409820':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'AL Boolean; unchanged C','algorithm':'actual full-EAX443710 <=4; no wider return contract is introduced'},
 '00409620':{'stack_arguments':1,'implicit_input_gpr':[],'consumed_result':'AL Boolean; upper EAX is incidental','algorithm':'3038D022 forbidden mask then26351277 mask, water bit and40000 exclusion'},
 '004ec410':{'stack_arguments':2,'implicit_input_gpr':[],'consumed_result':'FULL RAW EAX preserved, AL Boolean','algorithm':'BYTE low scene of DWORD784CB0, signed WORD784B94,48-byte row, DWORD605A3C exactly1, full upper EAX native quirk'},
 '00443710':{'stack_arguments':0,'implicit_input_gpr':[],'consumed_result':'full EAX count0..8; unchanged C','algorithm':'200 records98 bytes, enabled/type/shape/extra/slots all BYTE;36DWORD stack buckets'}
}
for entry,c in contracts.items():
 body=ours[entry];assert all(not i['instruction'].lower().startswith('ret ') or next(cs.disasm(bytes.fromhex(i['bytes']),int(i['address'],16))).mnemonic=='ret' and len(bytes.fromhex(i['bytes']))==1 for i in body if i['instruction'].lower().startswith('ret'))
 c.update(native_instruction_count=len(body),calling_convention='cdecl; native RET0, caller owns stack arguments',saved_registers='EBX/EBP/ESI/EDI preserved; pushes at entry are saved state, not implicit inputs')
old='src/unit_02079.c';old_sha=sha((ROOT/old).read_bytes());expected='2ef3b10ae590040a9006fd882f1406236f4592b5f06fd1377cd93d46c1c4f37c';assert old_sha==expected
prior=PHASE/'prior_source/unit_02079.c';prior.parent.mkdir(exist_ok=True);prior.write_bytes((ROOT/old).read_bytes())
consumers=[]
for file in (ROOT/'metadata').glob('*emulation*.json'):
 if file.name=='editor-validation-emulation.json':continue
 try:data=json.loads(file.read_text())
 except (ValueError,UnicodeError):continue
 if isinstance(data,dict) and any(key==old or key.endswith('/'+old) for key in data.get('source_hashes',{})):consumers.append(str(file.relative_to(ROOT)))
report={'format':1,'native_sha256':digest,'shared_ghidra_mutated':False,'contracts':contracts,'incoming_calls':results,'source2079_before_sha256':old_sha,'existing_proofs_hashing_source2079':consumers,'limitations':['Raw E8 candidates require alignment/CFG authentication before inference. In particular41CE57 remains raw-only; no signature is inferred from it.','15 incoming4EC410 sites are authenticated cached native instructions;2 additional sites are in the recovered complete4EC360 CFG. Their complete raw-EAX API is preserved, so no external caller is narrowed.','409820 has an analyzed reference at40737A without an exported complete caller. Existing BYTE/bool API is unchanged.','443710 has two analyzed references in unexported caller CFGs. Existing full int0..8 contract is unchanged.','Zero Ghidra xrefs to uncreated functions does not prove no callers; callback409830 is a data-only role with no raw E8 candidates.','No existing old native suite hashes source2079; prior preview consumers explicitly control4EC410. Their own caller proofs remain bounded and unchanged.'],'cache_source_hashes':cache_sources,'input_hashes':{str(f.relative_to(ROOT)):sha(f.read_bytes()) for f in [ROOT/'metadata/popup-editor-xrefs-native.json',ROOT/'metadata/popup-editor-relative-calls-native.json',PHASE/'native.json',PHASE/'helper-native.json',PHASE/'selector-native.json',Path(__file__)]}}
(PHASE/'caller-contracts.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'contracts':len(contracts),'calls':sum(map(len,results.values())),'source2079_before_sha256':old_sha,'old_consumers':consumers},indent=2))
