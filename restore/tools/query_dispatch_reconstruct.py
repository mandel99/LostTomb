"""Repair the complete placement-query ABI group over existing C algorithms."""
from pathlib import Path
import argparse,json,re,sys
from bulk_reconstruct import ROOT,PIN,catalogue,native_bytes,sha,save,defined_symbols
SHARED={0x4e5330:'src/unit_01969.c',0x5463e0:'src/unit_02578.c'}
FORWARDERS={
 0x401400:('src/unit_00003.c',0x4e5330,0x4c,16),
 0x40d850:('src/unit_00101.c',0x4e5330,0x4c,25),
 0x416f70:('src/unit_00196.c',0x4e5330,0x4c,9),
 0x4e65f0:('src/unit_01995.c',0x4e5330,0x4c,2),
 0x5456c0:('src/unit_02564.c',0x5463e0,0x1e5c,167),
 0x54f3f0:('src/unit_02592.c',0x5463e0,0x5c88,513),
 0x555e00:('src/unit_02605.c',0x5463e0,0x4ee0,437),
 0x558b60:('src/unit_02618.c',0x5463e0,0x1cbc,158)}

def forward(ins):
 from capstone.x86_const import X86_OP_MEM,X86_OP_REG,X86_OP_IMM
 if len(ins)!=7 or [i.mnemonic for i in ins]!=['mov','lea','push','push','push','call','ret']:return None
 if (ins[0].op_str,ins[3].op_str,ins[4].op_str,ins[6].op_str)!=('edx, dword ptr [esp + 4]','eax','edx','4'):return None
 a,b=ins[1].operands
 if a.type!=X86_OP_REG or ins[1].reg_name(a.reg)!='eax'or b.type!=X86_OP_MEM or ins[1].reg_name(b.mem.base)!='ecx'or b.mem.index or b.mem.segment:return None
 if ins[2].operands[0].type!=X86_OP_IMM or ins[5].operands[0].type!=X86_OP_IMM:return None
 return ins[5].operands[0].imm,b.mem.disp,ins[2].operands[0].imm

def candidate(address,shared):
 name=f'restore_placement_check_{address:08x}';canonical=f'FUN_{address:08x}'
 lines=['#include "general_placement.h"'if shared else'#include "placement_context.h"',
  '/* Existing typed C query is reused unchanged. This is only ECX/RET adaptation.',
  '   Full EAX is forwarded; legacy cdecl callers migrate as one closed group. */',
  f'__declspec(naked) uint32_t {canonical}(void)','{',' __asm {']
 if shared:lines += ['  push dword ptr[esp+12]']*3+['  push ecx',f'  call {name}','  add esp,16','  ret 12']
 else:lines += ['  push dword ptr[esp+4]','  push ecx',f'  call {name}','  add esp,8','  ret 4']
 lines += [' }','}',''];return name,'\n'.join(lines).encode()

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 from native_test_pe import PE
 binary=ROOT/'build/smoke-game/Pharaoh.exe';assert sha(binary)==PIN;pe=PE(binary)
 cache,conflicts=catalogue();dis=Cs(CS_ARCH_X86,CS_MODE_32);dis.detail=True
 allowed=set(SHARED.values())|{v[0]for v in FORWARDERS.values()}
 for source in (ROOT/'src').glob('*.c'):
  for target in SHARED:
   if re.search(r'\bFUN_%08x\s*\('%target,source.read_text())and source.relative_to(ROOT).as_posix()not in allowed:raise ValueError('Unmigrated cdecl caller: '+str(source))
 members=[];algorithm_hashes={p:sha(ROOT/p)for p in ('runtime/general_placement.c','runtime/placement_context.c','include/general_placement.h','include/placement_context.h')}
 for address in sorted(set(SHARED)|set(FORWARDERS)):
  assert address not in conflicts;raw=native_bytes(cache[address]);assert raw and pe.data[address-pe.base:address-pe.base+len(raw)]==raw
  ins=list(dis.disasm(raw,address));source=SHARED[address]if address in SHARED else FORWARDERS[address][0]
  canonical=f'FUN_{address:08x}';obj=ROOT/'build/c-only/pharaoh_restored.dir/Release'/(Path(source).stem+'.obj')
  assert '_'+canonical in defined_symbols(obj)
  assert ins[-1].mnemonic=='ret'and ins[-1].op_str==('0xc'if address in SHARED else'4')
  rule=dict(kind='shared_member_query',native_cleanup=12)if address in SHARED else dict(kind='constant_member_query',child=FORWARDERS[address][1],offset=FORWARDERS[address][2],count=FORWARDERS[address][3],native_cleanup=4)
  if address in FORWARDERS:assert forward(ins)==FORWARDERS[address][1:]
  name,code=candidate(address,address in SHARED);dest=a.output/source
  dest.parent.mkdir(parents=True,exist_ok=True)
  if dest.exists():assert dest.read_bytes()==code,'Candidate differs'
  dest.write_bytes(code)
  members.append(dict(address=f'0x{address:08x}',source=source,name=name,canonical_symbol=canonical,source_definition_symbol=canonical,body_hash_scope='canonical_ABI_binding',algorithm_source='runtime/general_placement.c'if address in SHARED else'runtime/placement_context.c',algorithm_source_sha256=algorithm_hashes['runtime/general_placement.c'if address in SHARED else'runtime/placement_context.c'],logical_signature='uint32_t cdecl4(uint32_t self,uint32_t cell,uint32_t pieces,int32_t count)'if address in SHARED else'uint32_t cdecl2(uint32_t self,uint32_t cell)',native_bytes=len(raw),native_instructions=len(ins),native_final_mnemonic='ret',native_byte_sha256=__import__('hashlib').sha256(raw).hexdigest(),new_C_algorithm=False,creation_credit=0,original_source_sha256=sha(ROOT/source),rule=rule))
 save(a.output/'members.json',dict(members=members))
 save(a.output/'contracts.json',dict(native_sha256=PIN,entries=10,new_algorithms=0,repair='Both shared RET12 methods and all eight legacy cdecl callers migrate to exact ECX/RET4 facades together.',unchanged_algorithm_sources=algorithm_hashes,logical_result='Full EAX, including native high bytes; actual native/C comparison remains pending.',limits=['Native virtual predicates are limited to existing owned token families.','No host launch, compilation or behavior replay performed by this generator.']))
 print(json.dumps(dict(entries=len(members),native_instructions=sum(m['native_instructions']for m in members),new_algorithms=0,output=str(a.output))))

if __name__=='__main__':main()
