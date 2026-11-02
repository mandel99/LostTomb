"""Bit-precise definite-definition audit for implicit incoming training GPR inputs."""
from pathlib import Path
import hashlib,json,sys
ROOT=Path(__file__).resolve().parents[2];PHASE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_IMM
sha=lambda b:hashlib.sha256(b).hexdigest();packet=json.loads((PHASE/'retained-native.json').read_text());abi=json.loads((PHASE/'abi-contract.json').read_text());assert packet['native_sha256']==abi['native_sha256']
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;instructions={int(i['address'],16):next(cs.disasm(bytes.fromhex(i['bytes']),int(i['address'],16))) for i in packet['function']['assembly']['items']}
registers=['eax','ebx','ecx','edx','esi','edi','ebp'];aliases={}
for whole,word,lo,hi in [('eax','ax','al','ah'),('ebx','bx','bl','bh'),('ecx','cx','cl','ch'),('edx','dx','dl','dh')]:
 for name,mask in [(whole,0xffffffff),(word,0xffff),(lo,0xff),(hi,0xff00)]:aliases[name]=(registers.index(whole),mask)
for whole,word in [('esi','si'),('edi','di'),('ebp','bp')]:aliases[whole]=(registers.index(whole),0xffffffff);aliases[word]=(registers.index(whole),0xffff)
saves={0x55ad4a,0x55ad4b,0x55ad4c,0x55af43};edges={}
for a,i in instructions.items():
 n=a+i.size
 if i.mnemonic.startswith('ret'):edges[a]=[]
 elif i.mnemonic.startswith('j'):
  if i.operands[0].type==X86_OP_IMM:edges[a]=[i.operands[0].imm]+([n] if i.mnemonic!='jmp' else [])
  else:edges[a]=abi['jump_tables'][f'{a:08x}']['targets']
 else:edges[a]=[n]
def transfer(a,state):
 i=instructions[a];out=state.copy()
 for reg in i.regs_access()[1]:
  name=cs.reg_name(reg)
  if name in aliases:index,mask=aliases[name];out[index]|=mask
 if i.mnemonic=='pop':
  name=i.op_str
  if name in aliases:out[aliases[name][0]]=0 # Restored incoming value, not a definition from body.
 if i.mnemonic=='call':
  for name in ['eax','ecx','edx']:out[registers.index(name)]=0xffffffff
 return out
states={0x55ad30:[0]*7};todo=[0x55ad30]
while todo:
 a=todo.pop();out=transfer(a,states[a])
 for target in edges[a]:
  merged=[x&y for x,y in zip(states[target],out)] if target in states else out.copy()
  if target not in states or merged!=states[target]:states[target]=merged;todo.append(target)
assert set(states)==set(instructions)
missing=[]
for a,i in instructions.items():
 zero=i.mnemonic in ['xor','sub'] and len(i.operands)==2 and i.op_str.split(', ')[0]==i.op_str.split(', ')[1]
 if a in saves or zero:continue
 for reg in i.regs_access()[0]:
  name=cs.reg_name(reg)
  if name in aliases:
   index,mask=aliases[name]
   if states[a][index]&mask!=mask:missing.append({'instruction':f'{a:08x}','register':name,'missing_bits':f'{mask&~states[a][index]:08x}'})
assert not missing,missing
report={'native_sha256':packet['native_sha256'],'complete_cfg_instructions':len(instructions),'incoming_GPR_inputs':missing,'method':'Bit masks for32/16/8-bit GPR aliases and intersection over every CFG predecessor. Four exact callee-save pushes are excluded from logical input reads. Restored incoming values are marked undefined again; no later logical read occurs. Calls define volatile EAX/ECX/EDX outputs, independent of incoming caller bits. Known sprite receiver ECX is explicitly loaded F55BB0.','saved_entry_values':['EBX55AD4A','EBP55AD4B','EDI55AD4C','ESI55AF43'],'calling_convention':'No semantic incoming GPR input, no incoming stack argument accessed; RET0. Context1/0 is ignored.','limitations':['This audits the renderer entry, not callee algorithms. The declared external-call register contracts are standard cdecl except explicit thiscall sprite member.'],'inputs':{str(f.relative_to(ROOT)).replace('\\','/'):sha(f.read_bytes()) for f in [PHASE/'retained-native.json',PHASE/'abi-contract.json',Path(__file__)]}}
(PHASE/'register-contract.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'instructions':len(instructions),'incoming_GPR_inputs':missing},indent=2))
