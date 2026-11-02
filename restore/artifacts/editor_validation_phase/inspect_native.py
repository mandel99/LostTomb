"""Pinned-byte static validation-function dossier, independent of shared Ghidra."""
from pathlib import Path
from collections import defaultdict
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];OUT=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone import x86 as X
raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();sha=lambda b:hashlib.sha256(b).hexdigest();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];table=opt+struct.unpack_from('<H',raw,pe+20)[0]
sections=[struct.unpack_from('<IIII',raw,table+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for vs,va,length,off in sections:
  if base+va<=a and a+n<=base+va+length:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
entries={0x409830,0x4ec360,0x4095c0,0x409650,0x4096e0,0x409760,0x4097c0,0x409820}
def body(entry):
 ins={};todo=[entry];edges=defaultdict(set);calls=[];tails=[]
 while todo:
  a=todo.pop()
  if a in ins:continue
  assert abs(a-entry)<0x10000 and len(ins)<30000
  i=next(cs.disasm(take(a,15),a));ins[a]=i;n=a+i.size
  if i.mnemonic.startswith('ret'):continue
  if i.mnemonic=='call':calls.append({'instruction':f'{a:08x}','target':f'{i.operands[0].imm:08x}' if i.operands[0].type==X.X86_OP_IMM else None,'operand':i.op_str});edges[a].add(n);todo.append(n);continue
  if i.mnemonic.startswith('j'):
   assert i.operands[0].type==X.X86_OP_IMM,'Unresolved indirect control flow'
   target=i.operands[0].imm
   if i.mnemonic=='jmp' and target in entries and target!=entry:tails.append({'instruction':f'{a:08x}','target':f'{target:08x}'})
   else:edges[a].add(target);todo.append(target)
   if i.mnemonic!='jmp':edges[a].add(n);todo.append(n)
   continue
  edges[a].add(n);todo.append(n)
 rows=[]
 for a,i in sorted(ins.items()):
  row={'address':f'{a:08x}','bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str,'register_reads':[cs.reg_name(r) for r in i.regs_access()[0]],'register_writes':[cs.reg_name(r) for r in i.regs_access()[1]],'memory':[]}
  for o in i.operands:
   if o.type==X.X86_OP_MEM:row['memory'].append({'width':o.size,'access':o.access,'base':cs.reg_name(o.mem.base),'index':cs.reg_name(o.mem.index),'scale':o.mem.scale,'displacement':f'{o.mem.disp&0xffffffff:08x}','signed_displacement':o.mem.disp})
  rows.append(row)
 return {'entry':f'{entry:08x}','instruction_count':len(rows),'instructions':rows,'edges':{f'{a:08x}':[f'{t:08x}' for t in sorted(ts)] for a,ts in edges.items()},'calls':calls,'tail_transfers':tails,'unresolved_indirect_control_flow':[]}
functions={f'{a:08x}':body(a) for a in sorted(entries)}
report={'native_sha256':digest,'scope':'All8 editor validation roots, original instruction CFGs; helper ABI must be qualified before implementation','functions':functions,'source_sha256':sha(Path(__file__).read_bytes())}
(OUT/'native.json').write_text(json.dumps(report,indent=2)+'\n')
for a,f in functions.items():print(a,f['instruction_count'],f['calls'],f['tail_transfers'])
