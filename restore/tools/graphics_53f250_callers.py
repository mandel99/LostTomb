"""Read-only PE/Capstone audit of every retained Ghidra entry xref."""
from pathlib import Path
import hashlib, json, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET
from capstone.x86_const import X86_OP_IMM
ROOT=Path(__file__).resolve().parents[1]
image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes()
digest=hashlib.sha256(image).hexdigest()
assert digest=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
u16=lambda p:struct.unpack_from('<H',image,p)[0]
u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;base=u32(opt+28);table=opt+u16(pe+20)
sections=[(u32(table+i*40+12),u32(table+i*40+20),u32(table+i*40+16)) for i in range(u16(pe+6))]
def data(a,n):
 for rva,raw,length in sections:
  if rva<=a-base and a-base+n<=rva+length:return image[raw+a-base-rva:raw+a-base-rva+n]
 raise ValueError(hex(a))
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
def decode(raw,a):return [{'address':f'{i.address:08x}','bytes':i.bytes.hex(),'instruction':i.mnemonic+' '+i.op_str} for i in md.disasm(raw,a)]
def eax_mask(ins,registers):
 masks={'eax':0xffffffff,'ax':0xffff,'al':0xff,'ah':0xff00}
 value=0
 for r in registers:value|=masks.get(ins.reg_name(r),0)
 return value
def returned_value_flow(start):
 pending=[(start,0xffffffff)];seen=set();result=[]
 while pending:
  a,mask=pending.pop()
  if (a,mask) in seen or not mask:continue
  seen.add((a,mask));assert len(seen)<20000,hex(start)
  ins=next(md.disasm(data(a,15),a,count=1));reads,writes=ins.regs_access()
  row=dict(address=f'{a:08x}',instruction=ins.mnemonic+' '+ins.op_str,live_eax_bits=f'{mask:08x}')
  if CS_GRP_RET in ins.groups:result.append(dict(row,kind='forward_to_caller_return'));continue
  clears=ins.mnemonic in ['xor','sub'] and len(ins.operands)==2 and ins.op_str.split(', ')[0]==ins.op_str.split(', ')[1]
  if not clears and eax_mask(ins,reads)&mask:result.append(dict(row,kind='consumed'));continue
  if CS_GRP_CALL in ins.groups:continue
  mask&=~eax_mask(ins,writes)
  if CS_GRP_JUMP in ins.groups:
   if ins.operands[0].type==X86_OP_IMM:pending.append((ins.operands[0].imm,mask))
   else:result.append(dict(row,kind='unresolved_indirect_flow'))
   if ins.mnemonic=='jmp':continue
  pending.append((a+ins.size,mask))
 return sorted(result,key=lambda r:r['address'])
retained=json.loads((ROOT/'metadata/city-draw-next-native.json').read_text())
rows=[]
for xref in retained['xrefs']['items']:
 a=int(xref['from'],16);assert data(a,1)==b'\xe8' and a+5+struct.unpack('<i',data(a+1,4))[0]==0x53f250
 before=data(a-48,48);index=before.rfind(b'\xb9\xb0\x5b\xf5\x00')
 assert index>=0,(hex(a),before.hex())
 binding=before[index:];decoded=list(md.disasm(binding,a-48+index))
 assert sum(i.size for i in decoded)==len(binding)
 for ins in decoded[1:]:
  _,writes=ins.regs_access()
  assert not any(ins.reg_name(r) in ['ecx','cx','cl','ch'] for r in writes),(hex(a),str(ins))
 after=decode(data(a+5,48),a+5)
 rows.append(dict(call=f'{a:08x}',caller=xref['from_function'],ecx=0xf55bb0,
  binding_window=decode(binding,a-48+index),prefix_bytes=before.hex(),successor_instructions=after,
  eax_value_flow=returned_value_flow(a+5)))
report=dict(native_sha256=digest,method='Retained complete Ghidra entry xrefs; exact PE bytes; Capstone disassembly anchored at the ECX definition and call successor.',
 xref_count=len(rows),all_bind_same_global=all(x['ecx']==0xf55bb0 for x in rows),callers=rows,
 consumed_or_forwarded=[{'call':r['call'],'caller':r['caller'],'endpoints':r['eax_value_flow']} for r in rows if r['eax_value_flow']],
 color_word_consumers=[dict(entry=f'{a:08x}',entry_window=decode(data(a,80),a)) for a in [0x410700,0x4109d0,0x410a70]],
 limitations=['Caller windows are static evidence, not a dynamic reachability claim. Raw 48-byte prefixes are retained without claiming an instruction boundary at the window start.',
  'forward_to_caller_return identifies bits still present at a native RET, including incidental void epilogues. It is not proof of semantic consumption by that procedure or its caller. Existing C drawing interfaces must not be widened on that basis alone.',
  'The bounded forward static traversal stops on replacement calls or EAX overwrites and lists potential EAX consumers/forwarding returns. It does not establish dynamic reachability.'])
(ROOT/'metadata/graphics_53f250_callers.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'checked_call_sites':len(rows),'all_ecx':'0x00f55bb0'}))
for r in rows:
 if r['eax_value_flow']:print(r['call'],r['eax_value_flow'])
