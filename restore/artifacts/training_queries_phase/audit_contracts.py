"""Complete retained native query CFG/stack/GPR audit; no project mutation."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM,X86_OP_IMM,X86_REG_ESP
sha=lambda b:hashlib.sha256(b).hexdigest()
raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=sha(raw)
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0];sec=opt+struct.unpack_from('<H',raw,pe+20)[0]
sections=[struct.unpack_from('<IIII',raw,sec+j*40+8) for j in range(struct.unpack_from('<H',raw,pe+6)[0])]
def take(a,n):
 for v,va,z,off in sections:
  if base+va<=a and a+n<=base+va+z:return raw[off+a-base-va:off+a-base-va+n]
 raise AssertionError(hex(a))
packet=json.loads((PHASE/'retained-native.json').read_text());assert packet['native_sha256']==digest
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
registers=['eax','ebx','ecx','edx','esi','edi','ebp'];aliases={}
for whole,word,lo,hi in [('eax','ax','al','ah'),('ebx','bx','bl','bh'),('ecx','cx','cl','ch'),('edx','dx','dl','dh')]:
 for name,mask in [(whole,0xffffffff),(word,0xffff),(lo,0xff),(hi,0xff00)]:aliases[name]=(registers.index(whole),mask)
for whole,word in [('esi','si'),('edi','di'),('ebp','bp')]:aliases[whole]=(registers.index(whole),0xffffffff);aliases[word]=(registers.index(whole),0xffff)
argcounts={0x443950:1,0x4439c0:1,0x443a20:1,0x443b20:1,0x443b90:1,0x443d90:1,0x447790:2,0x4c5240:1,0x4c5190:0}
saves={0x443950,0x443951,0x443956,0x443957,0x4439c0,0x4439c1,0x4439c6,0x4439c7,0x443a20,0x443a21,0x443a26,0x443a27,0x443b20,0x443b21,0x443b26,0x443b27,0x443b90,0x443d90}
reports=[]
for address,f in packet['functions'].items():
 entry=int(address,16);assert not f['assembly']['has_more'];instructions={}
 for row in f['assembly']['items']:
  a=int(row['address'],16);encoded=bytes.fromhex(row['bytes']);assert take(a,len(encoded))==encoded;instructions[a]=next(cs.disasm(encoded,a))
 edges={};calls={}
 for a,i in instructions.items():
  n=a+i.size
  if i.mnemonic.startswith('ret'):assert i.bytes==b'\xc3';edges[a]=[]
  elif i.mnemonic=='call':assert i.operands[0].type==X86_OP_IMM;calls[a]=i.operands[0].imm;assert calls[a] in argcounts;edges[a]=[n]
  elif i.mnemonic.startswith('j'):
   assert i.operands[0].type==X86_OP_IMM;edges[a]=[i.operands[0].imm]+([n] if i.mnemonic!='jmp' else [])
  else:edges[a]=[n]
  assert set(edges[a])<=set(instructions)
 deltas={entry:0};todo=[entry];stack=[]
 while todo:
  a=todo.pop();i=instructions[a];delta=deltas[a]
  for operand in i.operands:
   if operand.type==X86_OP_MEM and operand.mem.base==X86_REG_ESP:
    assert not operand.mem.index;relative=delta+operand.mem.disp;stack.append({'instruction':f'{a:08x}','entry_sp_relative':relative,'width':operand.size,'access':operand.access})
    assert relative in range(4,4+argcounts[entry]*4,4) and operand.size==4
  if i.mnemonic=='push':delta-=4
  elif i.mnemonic=='pop':delta+=4
  elif i.mnemonic in ['add','sub'] and i.op_str.startswith('esp,'):assert i.operands[1].type==X86_OP_IMM;delta+=i.operands[1].imm*(1 if i.mnemonic=='add' else -1)
  if i.mnemonic.startswith('ret'):assert delta==0
  for target in edges[a]:
   if target in deltas:assert deltas[target]==delta
   else:deltas[target]=delta;todo.append(target)
 assert set(deltas)==set(instructions)
 def transfer(a,state):
  i=instructions[a];out=state.copy()
  for reg in i.regs_access()[1]:
   name=cs.reg_name(reg)
   if name in aliases:index,mask=aliases[name];out[index]|=mask
  if i.mnemonic=='pop' and i.op_str in aliases:out[aliases[i.op_str][0]]=0
  if i.mnemonic=='call':
   # No callsite reads ECX/EDX before redefinition; only the declared return is needed.
   out[registers.index('eax')]=0xff if calls[a]==0x447790 else 0xffffffff
   out[registers.index('ecx')]=0;out[registers.index('edx')]=0
  return out
 states={entry:[0]*7};todo=[entry]
 while todo:
  a=todo.pop();out=transfer(a,states[a])
  for target in edges[a]:
   merged=[x&y for x,y in zip(states[target],out)] if target in states else out.copy()
   if target not in states or merged!=states[target]:states[target]=merged;todo.append(target)
 assert set(states)==set(instructions);missing=[]
 for a,i in instructions.items():
  zero=i.mnemonic in ['xor','sub'] and len(i.operands)==2 and i.op_str.split(', ')[0]==i.op_str.split(', ')[1]
  if a in saves or zero:continue
  for reg in i.regs_access()[0]:
   name=cs.reg_name(reg)
   if name in aliases:
    index,mask=aliases[name]
    if states[a][index]&mask!=mask:missing.append({'instruction':f'{a:08x}','register':name,'missing_bits':f'{mask&~states[a][index]:08x}'})
 assert not missing,missing
 reports.append({'entry':address,'instructions':len(instructions),'bytes':sum(i.size for i in instructions.values()),'complete_cfg':True,'argument_dwords':argcounts[entry],'stack_operands':sorted(stack,key=lambda x:x['instruction']),'incoming_GPR_inputs':missing,'ret_sites':[f'{a:08x}' for a,i in instructions.items() if i.mnemonic.startswith('ret')],'all_ret_delta':0,'calls':{f'{a:08x}':f'{v:08x}' for a,v in calls.items()},'return_role':'AL Boolean; incidental upper EAX' if entry==0x447790 else ('full EAX signed {-3,-2,-1,0,1}' if entry==0x4c5190 else ('full raw EAX signed amount' if entry==0x4c5240 else 'full EAX Boolean0/1'))})
assert sum(f['instructions'] for f in reports)==517
report={'native_sha256':digest,'functions':reports,'method':'Every retained instruction byte authenticated to pinned PE. Complete direct CFG per entry, exact stack deltas and incoming argument offsets. Bit-mask definite GPR definition over predecessor intersections; exact entry-save PUSHes excluded. POP restores incoming value as undefined; calls define only full EAX or AL return and invalidate ECX/EDX. No incoming GPR role, no member receiver, no indirect edge/call.','saved_entry_pushes':[f'{a:08x}' for a in sorted(saves)],'limitations':['Only declared stack arguments are inputs. Caller register and native full-return incidental bits are not part of a byte Boolean C return. Eligibility upper EAX examples and all21 callers are in the emulation/caller reports.','Syntactic CFG includes unchanged4C5240 modifier>1 branch; closed-real-graph mathematical dead-arm analysis is separate from this complete-CFG audit.'],'inputs':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p.read_bytes()) for p in [PHASE/'retained-native.json',Path(__file__)]}}
(PHASE/'abi-contract.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'functions':len(reports),'instructions':517,'implicit_GPR_inputs':0,'all_ret_delta':0},indent=2))
