"""Read-only original-instruction return-consumption audit for the walking scheduler."""
from pathlib import Path
import hashlib,json,struct,sys
ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone import x86 as X
raw=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();digest=hashlib.sha256(raw).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
pe=struct.unpack_from('<I',raw,60)[0];opt=pe+24;base=struct.unpack_from('<I',raw,opt+28)[0]
table=opt+struct.unpack_from('<H',raw,pe+20)[0];sections=[]
for j in range(struct.unpack_from('<H',raw,pe+6)[0]):
 p=table+j*40;vs,va,rs,off=struct.unpack_from('<IIII',raw,p+8);sections.append((base+va,rs,off))
def bytes_at(address,n):
 for start,length,offset in sections:
  if start<=address<start+length:return raw[offset+address-start:offset+address-start+n]
 raise AssertionError(hex(address))
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
packet=json.loads((ROOT/'metadata/actor-callback-human-queries-native.json').read_text());refs=packet['functions']['004a5da0']['xrefs'];assert not refs['has_more']
rows=[];aliases={}
for index,names in enumerate([['EAX','AX','AL','AH'],['ECX','CX','CL','CH'],['EDX','DX','DL','DH'],['EBX','BX','BL','BH'],['ESP','SP'],['EBP','BP'],['ESI','SI'],['EDI','DI']]):
 for name,(mask,shift) in zip(names,[(0xffffffff,0),(0xffff,0),(255,0),(0xff00,8)]):aliases[getattr(X,'X86_REG_'+name)]=(index,mask,shift)
def reg_taint(state,reg):
 index,mask,shift=aliases.get(reg,(0,0,0));return(state[index]&mask)>>shift
def set_reg(state,reg,value):
 if reg in aliases:
  index,mask,shift=aliases[reg];state[index]=(state[index]&~mask)|((value<<shift)&mask)
  if index==0:state[8]=0
for ref in refs['items']:
 if ref['type']!='UNCONDITIONAL_CALL':continue
 call=int(ref['from'],16);i=next(cs.disasm(bytes_at(call,15),call));assert i.mnemonic=='call' and i.operands[0].imm==0x4a5da0
 pending=[(call+i.size,(0xffffffff,0,0,0,0,0,0,0,1))];visited=set();terminals=[];window={}
 while pending:
  a,saved=pending.pop()
  if (a,saved) in visited:continue
  assert len(visited)<1024;visited.add((a,saved));state=list(saved);i=next(cs.disasm(bytes_at(a,15),a));window[a]={'address':f'{a:08x}','bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str}
  reads,writes=i.regs_access();ops=i.operands
  zero=i.mnemonic in ['xor','sub'] and len(ops)==2 and all(o.type==X.X86_OP_REG for o in ops) and ops[0].reg==ops[1].reg and ops[0].reg in aliases
  address_use=i.mnemonic!='lea' and any(o.type==X.X86_OP_MEM and(reg_taint(state,o.mem.base) or reg_taint(state,o.mem.index)) for o in ops)
  if address_use:terminals.append({'address':f'{a:08x}','kind':'consumed_address','instruction':window[a]['instruction']});continue
  if zero:set_reg(state,ops[0].reg,0)
  elif i.mnemonic in ['mov','movzx','movsx'] and ops[0].type==X.X86_OP_REG:
   source=reg_taint(state,ops[1].reg) if ops[1].type==X.X86_OP_REG else 0
   if i.mnemonic=='movsx' and source&(1<<(ops[1].size*8-1)):source|=0xffffffff<<ops[1].size*8
   set_reg(state,ops[0].reg,source)
  elif i.mnemonic=='lea':
   m=ops[1].mem;source=reg_taint(state,m.base)|(reg_taint(state,m.index)*m.scale)
   # Adding/scaling leftward cannot move upper taint into already-replaced low
   # bits. Conservatively retain all bits above the first affected position.
   if source:source=(0xffffffff<<((source&-source).bit_length()-1))&0xffffffff
   set_reg(state,ops[0].reg,source)
  else:
   if any(reg_taint(state,r) for r in reads):terminals.append({'address':f'{a:08x}','kind':'consumed','instruction':window[a]['instruction']});continue
   for r in writes:set_reg(state,r,0)
  if not any(state[:8]):terminals.append({'address':f'{a:08x}','kind':'overwritten','instruction':window[a]['instruction']});continue
  if i.mnemonic.startswith('ret'):terminals.append({'address':f'{a:08x}','kind':'forwarded_at_return','remaining_original_eax_mask':f'{state[0]:08x}','eax_untouched':bool(state[8]),'instruction':window[a]['instruction']});continue
  if i.mnemonic=='call':terminals.append({'address':f'{a:08x}','kind':'next_call_boundary','remaining_original_eax_mask':f'{state[0]:08x}','instruction':window[a]['instruction']});continue
  taint=tuple(state)
  if i.mnemonic.startswith('j'):
   assert i.operands[0].type==X.X86_OP_IMM
   pending.append((i.operands[0].imm,taint))
   if i.mnemonic!='jmp':pending.append((a+i.size,taint))
  else:pending.append((a+i.size,taint))
 kinds={t['kind'] for t in terminals};assert kinds<= {'overwritten','forwarded_at_return','next_call_boundary'},(hex(call),terminals)
 untouched=any(t['kind']=='forwarded_at_return' and t.get('eax_untouched') for t in terminals)
 rows.append({'call':f'{call:08x}','caller':ref.get('from_function'),'classification':'untouched_return_forwarding' if untouched else 'no_direct_consumption','terminals':terminals,'instructions':[window[a] for a in sorted(window)]})
counts={kind:sum(r['classification']==kind for r in rows) for kind in ['no_direct_consumption','untouched_return_forwarding']}
assert counts=={'no_direct_consumption':129,'untouched_return_forwarding':3},counts
report={'native_sha256':digest,'passed':True,'entry':'004a5da0','direct_calls':len(rows),'counts':counts,'scope':'Track original EAX bits through all8 general registers on every direct caller path until fully overwritten, semantically used, returned or the next direct call. MOV copies and LEA arithmetic propagate the remaining original bits; memory stores/addresses and other observable reads would fail. Partial AL/AX writes only kill their own bits. The next direct call is an explicit volatile-register boundary, not an audited callee body. Three returns forward the whole untouched register; some other returns retain incidental upper bits after replacing AL. This does not establish a semantic scheduler result.','rows':rows,'source_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest()}
(ROOT/'metadata/actor-walking-caller-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'passed':True,'direct_calls':len(rows),'counts':counts}))
