"""Authenticate the full refresh controller and expose width/ABI review facts."""
from pathlib import Path
import hashlib,json,re,struct,sys
ROOT=Path(__file__).resolve().parents[2];HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_MEM
d=json.loads((HERE/'native-provider.json').read_text());image=(ROOT/'build/smoke-game/Pharaoh.exe').read_bytes();sha=lambda b:hashlib.sha256(b).hexdigest();assert sha(image)==d['native_sha256']
u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0];pe=u32(60);opt=pe+24;base=u32(opt+28);s=opt+u16(pe+20)
sections=[(u32(s+i*40+12),u32(s+i*40+16),u32(s+i*40+20)) for i in range(u16(pe+6))]
def raw(a,n):
 for start,length,offset in sections:
  rva=a-base
  if start<=rva and rva+n<=start+length:return image[offset+rva-start:offset+rva-start+n]
 raise AssertionError(hex(a))
asm=d['assembly'];assert not asm['has_more'] and asm['total']==len(asm['items'])==917
cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fields={};jumps=[];calls=[];ops=set()
for j,x in enumerate(asm['items']):
 a=int(x['address'],16);b=bytes.fromhex(x['bytes']);assert raw(a,len(b))==b,x['address'];i=next(cs.disasm(b,a,1));ops.add(i.mnemonic)
 for operand in i.operands:
  if operand.type==X86_OP_MEM:
   m=operand.mem
   if 0x400000<=m.disp<0x1000000:
    key=f'{m.disp:08x}';fields.setdefault(key,[]).append({'instruction':x['address'],'text':x['instruction'],'width':operand.size,'base':i.reg_name(m.base),'index':i.reg_name(m.index),'scale':m.scale,'access':operand.access})
 if i.mnemonic.startswith('j'):jumps.append({'address':x['address'],'mnemonic':i.mnemonic,'target':i.op_str})
 if i.mnemonic=='call':calls.append({'address':x['address'],'target':i.op_str,'before':asm['items'][max(0,j-12):j],'after':asm['items'][j+1:j+5]})
assert all(j['target'].startswith('0x') for j in jumps),'indirect jump needs table closure'
addresses={int(i['address'],16) for i in asm['items']}
assert all(int(j['target'],16) in addresses for j in jumps)
text=(ROOT/'src/unit_01867.c').read_text();bindings=[]
for address,uses in sorted(fields.items()):
 m=re.search(r'^#define DAT_'+address+r' (.*)$',text,re.M)
 bindings.append({'address':address,'direct_widths':sorted({u['width'] for u in uses if not u['base'] and not u['index']}),'indexed_widths':sorted({u['width'] for u in uses if u['base'] or u['index']}),'current_C':m[1] if m else None,'uses':uses})
out={'format':1,'native_sha256':d['native_sha256'],'authenticated_native_instructions':917,'direct_CFG_targets_closed':True,'indirect_jumps':[],'conditional_branches':sum(j['mnemonic']!='jmp' for j in jumps),'call_sites':len(calls),'unique_call_targets':len({c['target'] for c in calls}),'mnemonics':sorted(ops),'fields':bindings,'calls':calls,'source_sha256':sha((ROOT/'src/unit_01867.c').read_bytes()),'C_equivalence_verified':False}
(HERE/'native-review.json').write_text(json.dumps(out,indent=2)+'\n')
(HERE/'native.asm').write_text('\n'.join(i['address']+' '+i['bytes']+' '+i['instruction'] for i in asm['items'])+'\n')
print(json.dumps({k:out[k] for k in ['authenticated_native_instructions','direct_CFG_targets_closed','conditional_branches','call_sites','unique_call_targets','mnemonics']}))
print(json.dumps([{k:r[k] for k in ['address','direct_widths','indexed_widths','current_C']} for r in bindings]))
