from pathlib import Path
import sys,struct,json,collections
root=Path(__file__).resolve().parent.parent;sys.path.insert(0,str(root/'build/offline-verification-deps'))
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
image=(root/'build/smoke-game/Pharaoh.exe').read_bytes();u16=lambda p:struct.unpack_from('<H',image,p)[0];u32=lambda p:struct.unpack_from('<I',image,p)[0]
pe=u32(0x3c);opt=pe+24;table=opt+u16(pe+20);base=u32(opt+28)
decoder=Cs(CS_ARCH_X86,CS_MODE_32)
targets={0x55f449,0x55f561,0x55f5a1,0x55f5f4,0x55f639,0x55f0e8}
output=[];history=collections.deque(maxlen=12)
for i in range(u16(pe+6)):
 p=table+40*i
 if not u32(p+36)&0x20000000:continue
 raw=u32(p+20);data=image[raw:raw+u32(p+16)]
 decoder.skipdata=True
 for ins in decoder.disasm(data,base+u32(p+12)):
  if ins.mnemonic=='call' and ins.op_str.startswith('0x') and int(ins.op_str,16) in targets:
   output.append({'callee':ins.op_str,'call':hex(ins.address),'preceding':[f'{a:x}: {m} {o}' for a,m,o in history]})
  history.append((ins.address,ins.mnemonic,ins.op_str))
(root/'metadata/sound-stream-callers-native.json').write_text(json.dumps(output,indent=2)+'\n')
print(json.dumps(output,indent=2))
