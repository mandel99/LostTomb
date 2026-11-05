"""Complete native templates with C algorithms and explicit i386 output ABI."""

def recognize(core):
 from capstone.x86_const import X86_OP_REG,X86_OP_IMM,X86_OP_MEM
 def field(ins,index,width=4):
  if len(ins.operands)<=index:return None
  p=ins.operands[index]
  if p.type==X86_OP_MEM and p.size==width and ins.reg_name(p.mem.base)=='ecx'and not(p.mem.index or p.mem.segment):return p.mem.disp
  return None
 def exact(index,mnemonic,operand):return (core[index].mnemonic,core[index].op_str)==(mnemonic,operand)
 if len(core)==3 and core[-1].mnemonic=='ret'and not core[-1].op_str:
  clear=field(core[0],0);read=field(core[1],1)
  if clear is not None and read is not None and core[0].mnemonic=='and'and len(core[0].operands)==2 and core[0].operands[1].type==X86_OP_IMM and core[0].operands[1].imm==0 and core[1].mnemonic=='mov'and core[1].op_str.startswith('eax, '):
   return dict(kind='clear_read',result='uint32_t',offset=read,clear_offset=clear,width=4,native_cleanup=0,native_flags='and_zero',signature='uint32_t cdecl1(uint32_t self)')
 if len(core)==16:
  count=field(core[0],1)
  fixed={1:('xor','eax, eax'),2:('push','esi'),3:('test','edx, edx'),5:('mov','esi, dword ptr [esp + 8]'),7:('cmp','dword ptr [ecx], esi'),9:('inc','eax'),11:('cmp','eax, edx'),13:('or','eax, 0xffffffff'),14:('pop','esi'),15:('ret','4')}
  if count is not None and core[0].mnemonic=='mov'and core[0].op_str.startswith('edx, ')and all(exact(i,*pair)for i,pair in fixed.items()):
   branches=((4,'jle',13),(8,'je',14),(12,'jl',7))
   if all(core[i].mnemonic==op and core[i].operands[0].type==X86_OP_IMM and core[i].operands[0].imm==core[target].address for i,op,target in branches):
    adds=[]
    for i in (6,10):
     op=core[i]
     if op.mnemonic!='add'or len(op.operands)!=2 or op.operands[0].type!=X86_OP_REG or op.reg_name(op.operands[0].reg)!='ecx'or op.operands[1].type!=X86_OP_IMM:break
     adds.append(op.operands[1].imm)
    if len(adds)==2 and adds[1]>0:
     return dict(kind='find_key',result='uint32_t',offset=adds[0],count_offset=count,stride=adds[1],width=4,native_cleanup=4,native_flags='equal_cmp_or_not_found',signature='uint32_t cdecl3(uint32_t self,uint32_t key,int32_t *native_count)')
 if len(core)==15:
  factor=field(core[1],1,1);amount=field(core[2],1)
  fixed={0:('xor','eax, eax'),3:('imul','ecx, eax'),4:('mov','eax, 0x51eb851f'),5:('imul','ecx'),6:('sar','edx, 7'),7:('mov','ecx, edx'),8:('shr','ecx, 0x1f'),9:('add','edx, ecx'),10:('mov','eax, edx'),11:('cmp','eax, 1'),13:('mov','eax, 1'),14:('ret','')}
  if factor is not None and amount is not None and core[1].mnemonic==core[2].mnemonic=='mov'and core[1].op_str.startswith('al, ')and core[2].op_str.startswith('ecx, ')and all(exact(i,*pair)for i,pair in fixed.items())and core[12].mnemonic=='jge'and core[12].operands[0].imm==core[14].address:
   return dict(kind='scale_clamp',result='uint32_t',offset=amount,factor_offset=factor,width=4,divisor=400,minimum=1,native_cleanup=0,native_flags='cmp_quotient_one',signature='uint32_t cdecl3(uint32_t self,int32_t *native_quotient,uint32_t *native_sign)')
 return None

def emit(address,canonical,r):
 if r['kind']not in ('clear_read','find_key','scale_clamp'):return None
 name=f'restore_object_{r["kind"]}_{address:08x}'
 head=['#include <stdint.h>','#if !defined(_MSC_VER) || !defined(_M_IX86)','#error This entry requires the original i386 ABI.','#endif',
       '/* Complete native template. Arithmetic/address wrap is explicitly32-bit.',
       '   Canonical volatile outputs and final flags are reconstructed separately.',
       '   Nonfaulting ordinary-memory inputs only; no whole-game equivalence claim. */']
 offset=r['offset']&0xffffffff
 if r['kind']=='clear_read':
  clear=r['clear_offset']&0xffffffff
  code=f'''uint32_t {name}(uint32_t self)
{{
 *(volatile uint32_t *)(uintptr_t)(self+0x{clear:08x}u)&=0u;
 return *(volatile uint32_t *)(uintptr_t)(self+0x{offset:08x}u);
}}
__declspec(naked) void {canonical}(void)
{{
 __asm {{
  push ecx
  push edx
  push ecx
  call {name}
  add esp,4
  pop edx
  pop ecx
  push 0
  and dword ptr[esp],0
  lea esp,[esp+4]
  ret
 }}
}}
'''
 elif r['kind']=='find_key':
  count=r['count_offset']&0xffffffff;stride=r['stride']
  code=f'''uint32_t {name}(uint32_t self,uint32_t key,int32_t *native_count)
{{
 int32_t count=*(volatile int32_t *)(uintptr_t)(self+0x{count:08x}u);
 uint32_t index;
 if(native_count)*native_count=count;
 for(index=0;count>0&&index<(uint32_t)count;++index)
  if(*(volatile uint32_t *)(uintptr_t)(self+0x{offset:08x}u+index*{stride}u)==key)return index;
 return 0xffffffffu;
}}
__declspec(naked) void {canonical}(void)
{{
 __asm {{
  push ebp
  mov ebp,esp
  sub esp,8
  mov [ebp-8],ecx
  lea eax,[ebp-4]
  push eax
  push dword ptr[ebp+8]
  push ecx
  call {name}
  add esp,12
  mov ecx,[ebp-8]
  mov edx,[ebp-4]
  mov esp,ebp
  pop ebp
  test edx,edx
  jle failed
  add ecx,0{offset:x}h
  cmp eax,-1
  je exhausted
  push eax
  imul eax,{stride}
  add ecx,eax
  pop eax
  cmp eax,eax
  ret 4
 exhausted:
  push eax
  mov eax,edx
  imul eax,{stride}
  add ecx,eax
  pop eax
 failed:
  or eax,-1
  ret 4
 }}
}}
'''
 else:
  factor=r['factor_offset']&0xffffffff
  code=f'''uint32_t {name}(uint32_t self,int32_t *native_quotient,uint32_t *native_sign)
{{
 uint32_t factor=*(volatile uint8_t *)(uintptr_t)(self+0x{factor:08x}u);
 uint32_t amount=*(volatile uint32_t *)(uintptr_t)(self+0x{offset:08x}u);
 int32_t product=(int32_t)(factor*amount);
 int32_t quotient=product/400;
 if(native_quotient)*native_quotient=quotient;
 if(native_sign)*native_sign=product<0?1u:0u;
 return quotient<1?1u:(uint32_t)quotient;
}}
__declspec(naked) void {canonical}(void)
{{
 __asm {{
  push ebp
  mov ebp,esp
  sub esp,8
  lea eax,[ebp-8]
  lea edx,[ebp-4]
  push edx
  push eax
  push ecx
  call {name}
  add esp,12
  mov edx,[ebp-8]
  mov ecx,[ebp-4]
  mov esp,ebp
  pop ebp
  cmp edx,1
  ret
 }}
}}
'''
 return name,('\n'.join(head)+'\n'+code).encode()
