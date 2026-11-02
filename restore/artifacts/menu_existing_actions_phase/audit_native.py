"""Audit unchanged entry CFGs, prior body-proof scope and current source identity."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
sha=lambda b:hashlib.sha256(b).hexdigest();raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sec=opt+struct.unpack_from('<H',raw,pe+20)[0];sections=[struct.unpack_from('<IIII',raw,sec+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for v,va,z,off in sections:
  if base+va<=a and a+n<=base+va+z:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
cs=Cs(CS_ARCH_X86,CS_MODE_32);functions={};all_entries=set(packet['functions']);deep={'0041c740','0041ca10'}
for entry,f in packet['functions'].items():
 assert not f['assembly']['has_more'];rows=f['assembly']['items'];body={int(i['address'],16):i for i in rows};calls=[];branches=[];returns=[]
 for a,i in body.items():
  encoded=bytes.fromhex(i['bytes']);assert take(a,len(encoded))==encoded;d=next(cs.disasm(encoded,a));next_address=a+d.size
  if d.mnemonic.startswith('ret'):assert encoded==b'\xc3';returns.append(f'{a:08x}');continue
  if d.mnemonic=='call':
   target=f'{int(d.op_str,16):08x}';assert target in all_entries|deep;calls.append({'site':f'{a:08x}','target':target});assert next_address in body
  elif d.mnemonic.startswith('j'):
   target=int(d.op_str,16);assert target in body;branches.append([f'{a:08x}',f'{target:08x}'])
   if d.mnemonic!='jmp':assert next_address in body;branches.append([f'{a:08x}',f'{next_address:08x}'])
  else:assert next_address in body
 functions[entry]={'native_instruction_count':len(body),'complete_cfg':True,'unresolved_control_flow':[],'calls':calls,'branch_edges':branches,'ret0_sites':returns}
old=[]
for name,qualification in [('load-save-menu-emulation.json','Actual three entry bodies +actual selector/trim;18 dedicated direct entry cases among1968 total. Getter, memset and three caret bodies are controls. No per-instruction coverage or helper-exit mutation measured. Does not contain402BB0.'),('dropdown-menu-emulation.json','All four actions are controlled deep callbacks in17625 dropdown controller cases; source inclusion/dispatch is not an action-body proof.'),('family-options-emulation.json','402450 is a controlled nested action in2329 caller cases, including deliberately changed return state. Not its body proof.')]:
 file=ROOT/'metadata'/name;d=json.loads(file.read_text());drift=[k for k,v in d['source_hashes'].items() if sha((ROOT/k).read_bytes())!=v]
 old.append({'report':str(file.relative_to(ROOT)),'passed':d['passed'],'checked':d['checked'],'report_sha256':sha(file.read_bytes()),'current_source_drift':drift,'qualification':qualification})
assert not old[0]['current_source_drift']
sources=['src/unit_00020.c','runtime/load_save_menu.c','src/unit_00024.c','src/unit_00119.c','src/unit_01682.c','src/unit_01683.c','src/unit_01686.c','src/unit_02282.c','src/unit_01739.c','src/unit_01745.c']
report={'format':1,'native_sha256':digest,'shared_ghidra_mutated':False,'production_changes_required':False,'reason':'All four current complete bodies match native instructions in the declared valid state domain. New work supplies missing/stronger whole-entry composition evidence instead of unnecessary production rewrites.','functions':functions,'prior_proofs':old,'entry_contracts':{
 '00402450':{'C':'void FUN_00402450(void)','arguments':0,'implicit_input_gpr':[],'writes':'DWORD6059E8=0 before first profile getter; after selector: DWORD E20998=0, BYTE E38DDA=13, DWORD D3B1BC=1, BYTE D3B1C0=1. Actual memset clears64 atE37100, then rememberedE370BC is freshly copied; caret2 select→home→end; dirty BYTE1 last.'},
 '00402540':{'C':'void FUN_00402540(void)','arguments':0,'implicit_input_gpr':[],'writes':'Same transition/copy/caret chain but6059E8 preserved, D3B1C0 BYTE0 (save).'},
 '00402630':{'C':'void FUN_00402630(void)','arguments':0,'implicit_input_gpr':[],'writes':'Same transition/copy/caret chain but6059E8 preserved, D3B1C0 BYTE2 (delete).'},
 '00402BB0':{'C':'void FUN_00402bb0(void)','arguments':0,'implicit_input_gpr':[],'writes':'BYTE E38DDA=0 then BYTE E38DCA=1; no helper calls or other state change. This is the exact native state transition, not a host ExitProcess.'}},
 'composition':{'005169C0':'Actual unchanged2282 getter, full EAX pointer; UI profile NUL0..31bytes. Called twice in each three entry chain: outer construction, then actual selector independently fetches profile again because mode0.','004CDFF0':'Actual unchanged1739, cdecl unused first DWORD pointer +BYTE maps. Outer constructed pattern is observed exactly despite being unused by selector. Real selector makes a fresh pattern and enumerates/sorts through two typed boundary providers.','004CE3A0':'Actual unchanged1745 first-dot trim.','0040F8F0':'Actual unchanged119, cdecl destination DWORD pointer/size DWORD/fill BYTE. Entry domain64/0; direct cases sizes0..80 and low-byte fill extremes. Native REP memset22instructions, no guessed host substitute.','004CC500':'Actual unchanged1686, cdecl index DWORD; pointer/count/max/cursor strides4, all fields DWORD. Max signed, inclusive loop bound (max0 may count one char). Native captures max in ESI before selected/count writes. C reloads are equivalent in stable disjoint valid registry domain, no asynchronous/internal alias mutation claimed.','004CC3C0':'Actual unchanged1682 loads full selected DWORD then writes cursor[selected*4] DWORD0.','004CC3E0':'Actual unchanged1683 reloads full selected DWORD and full length DWORD into corresponding cursor; exit mutation of3C0 changes selected row and proves this reload.'},
 'boundaries':{'0041C740':'ECX6057C8 +pattern/flag, native RET8; explicit C RestoreFileList*self,char*pattern,char flag; low-byte return is ignored.','0041CA10':'ECX6057C8 +comparator562370, native RET4; explicit C RestoreFileList*self,typed comparator. Comparator identity never executes in oracle; fail-fast sentinel enforces this.'},
 'context_exclusions':['Selector4CE0AD (XOR EAX fallback) and its two incoming guard edges are unreachable for stable valid list loop. No count mutation during trim is used to manufacture a null pointer.','Eight unproved row/input routes sharing runtime/load_save_menu.c have typed aborting link sentinels. They must never execute; source compilation does not count their bodies as proved.','Proof compares actual plain C call-visible store order and final guarded bytes, not every internal machine store. All four action C bodies remain unchanged.'],
 'source_hashes':{s:sha((ROOT/s).read_bytes()) for s in sources},'evidence_hashes':{str(file.relative_to(ROOT)).replace('\\','/'):sha(file.read_bytes()) for file in [PHASE/'retained-native.json',ROOT/'metadata/load-save-native.json',ROOT/'tools/verify_load_save_menu_emulation.py',ROOT/'tests/load_save_menu.c',Path(__file__)]}}
(PHASE/'scope-and-native-contracts.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'entries':len(functions),'native_instructions':sum(x['native_instruction_count'] for x in functions.values()),'prior_direct_entry_cases':18,'production_changes_required':False},indent=2))
