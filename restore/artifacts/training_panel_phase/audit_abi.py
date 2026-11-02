"""Complete pinned training-renderer CFG and stack-input audit, without Ghidra mutation."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM,X86_OP_IMM,X86_REG_ESP
raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();sha=lambda b:hashlib.sha256(b).hexdigest();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sec=opt+struct.unpack_from('<H',raw,pe+20)[0];sections=[struct.unpack_from('<IIII',raw,sec+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for v,va,z,off in sections:
  if base+va<=a and a+n<=base+va+z:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
p=json.loads((PHASE/'retained-native.json').read_text());assert p['native_sha256']==digest and not p['function']['assembly']['has_more'];rows=p['function']['assembly']['items'];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;instructions={}
for row in rows:
 a=int(row['address'],16);encoded=bytes.fromhex(row['bytes']);assert take(a,len(encoded))==encoded;instructions[a]=next(cs.disasm(encoded,a))
tables={}
for site,table,count,mapping in [(0x55ae08,0x55bafc,4,None),(0x55b2ef,0x55bb0c,2,0x55bb14),(0x55b343,0x55bb20,2,0x55bb28),(0x55b559,0x55bb34,2,0x55bb3c),(0x55b5e4,0x55bb48,2,0x55bb50),(0x55b745,0x55bb5c,2,0x55bb64),(0x55b7d3,0x55bb70,2,0x55bb78)]:
 encoded=take(table,count*4);targets=list(struct.unpack('<%dI'%count,encoded));assert set(targets)<=set(instructions)
 mapped=take(mapping,12) if mapping else b'';assert not mapped or max(mapped)<count
 tables[site]={'table':table,'targets':targets,'hex':encoded.hex(),'compressed_mapping':mapping,'compressed_hex':mapped.hex(),'guard':'unsigned modeWORD<=3' if mapping is None else 'unsigned(selectionDWORD-24)<=11'}
edges={};calls={}
for a,i in instructions.items():
 n=a+i.size
 if i.mnemonic.startswith('ret'):assert i.bytes==b'\xc3';edges[a]=[]
 elif i.mnemonic=='call':assert i.operands[0].type==X86_OP_IMM;calls[a]=i.operands[0].imm;edges[a]=[n]
 elif i.mnemonic.startswith('j'):
  if i.operands[0].type==X86_OP_IMM:edges[a]=[i.operands[0].imm]+([n] if i.mnemonic!='jmp' else [])
  else:assert a in tables;edges[a]=tables[a]['targets']
 else:edges[a]=[n]
 assert set(edges[a])<=set(instructions)
# Native sprite member is thiscall and RET12; all other calls are caller-cleaned.
deltas={0x55ad30:0};todo=[0x55ad30];stack=[]
while todo:
 a=todo.pop();i=instructions[a];delta=deltas[a]
 for operand in i.operands:
  if operand.type==X86_OP_MEM and operand.mem.base==X86_REG_ESP:
   assert not operand.mem.index;relative=delta+operand.mem.disp;stack.append({'instruction':f'{a:08x}','stack_delta':delta,'displacement':operand.mem.disp,'entry_sp_relative':relative,'width':operand.size,'access':operand.access});assert relative<0,'Incoming argument or return-slot access'
 if i.mnemonic=='push':delta-=4
 elif i.mnemonic=='pop':delta+=4
 elif i.mnemonic in ['add','sub'] and i.op_str.startswith('esp,'):assert i.operands[1].type==X86_OP_IMM;delta+=i.operands[1].imm*(1 if i.mnemonic=='add' else -1)
 elif i.mnemonic=='call':delta+=12 if calls[a]==0x53e590 else 0
 if i.mnemonic.startswith('ret'):assert delta==0,'Native final stack imbalance'
 for target in edges[a]:
  if target in deltas:assert deltas[target]==delta,('Stack merge',hex(target),deltas[target],delta)
  else:deltas[target]=delta;todo.append(target)
assert set(deltas)==set(instructions)
cache_calls=[]
for path in (ROOT/'metadata/native-function-audit-cache').glob('batch-*.json'):
 data=json.loads(path.read_text())
 for f in data['entries']:
  body=f['assembly']['items']
  for j,row in enumerate(body):
   if row['bytes'].startswith('e8'):
    a=int(row['address'],16);encoded=bytes.fromhex(row['bytes'])
    if len(encoded)==5 and a+5+struct.unpack('<i',encoded[1:])[0]==0x55ad30:
     window=body[max(0,j-5):j+4]
     for w in window:assert take(int(w['address'],16),len(bytes.fromhex(w['bytes'])))==bytes.fromhex(w['bytes'])
     cache_calls.append({'owner':f['requested_address'],'owner_has_more':f['assembly']['has_more'],'call':row['address'],'window':window})
report={'native_sha256':digest,'native_instruction_count':len(instructions),'native_bytes':sum(i.size for i in instructions.values()),'complete_cfg':True,'entry_stack_delta':0,'ret_sites':[f'{a:08x}' for a,i in instructions.items() if i.mnemonic.startswith('ret')],'all_ret_deltas':0,'stack_operands':sorted(stack,key=lambda x:x['instruction']),'incoming_stack_arguments_read_or_written':[],'native_context_contract':'Pushed DWORD is neither read nor written anywhere in complete CFG. Native callee is RET0 and ignores context bits; caller owns cleanup. Zero-argument C body plus existing explicit facade can preserve this contract.','sprite_member':'Native call55ADA0 ECX=F55BB0, three pushed DWORD arguments, thiscall RET12. C plain facade must bind that receiver; controlled proof adapter must model native cleanup12.','jump_tables':{f'{a:08x}':v for a,v in tables.items()},'callee_targets':[f'{a:08x}' for a in sorted(set(calls.values()))],'cached_callers':cache_calls,'limitations':['Function metadata signature alone was not used as ABI evidence. Complete CFG stack dataflow includes all jump-table successors and exact sprite cleanup.','Cached callers may omit uncreated functions; no absence-of-callers inference. All retained native bytes authenticated.']}
(PHASE/'abi-contract.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'instructions':len(instructions),'stack_operands':len(stack),'context_reads':0,'cached_calls':len(cache_calls),'tables':len(tables)},indent=2))
