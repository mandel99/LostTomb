"""Necessary ABI preflight for the shared live-context bridge, not the full gate."""
import struct
S=0x32000000
SP=0x30008000
CONTEXT=S+0x30000
OFFSETS=dict(edi=0,esi=4,ebp=8,ebx=16,edx=20,ecx=24,eax=28,eflags=32)

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']};out=[]
 for entry in (0x449440,0x4495c0,0x4495d0,0x449600,0x449630,0x4496b0,0x449b30):
  if entry not in present:continue
  for i,(eax,flags,kind,mode)in enumerate(((0x89abcdef,0x202,0,0),(0xfedcba10,0xac7,2,2))):
   receiver=S+0x1000;words=[0x25]if entry==0x449440 else[]
   obj=bytearray(256);obj[2]=kind;obj[0x60]=mode
   snapshot=struct.pack('<9I',0x456789ab,0x3456789a,0x23456789,SP-4,0x12345678,0x33333333,receiver,eax,flags)
   c=dict(id=f'c5-bridge/{entry:08x}/{i}',entry=entry,receiver=receiver,
    arguments=words,named_arguments=[CONTEXT],initial_eax=eax,eflags=flags,
    stores=[(receiver,bytes(obj)),(CONTEXT,snapshot)],watched=[(receiver,256)],
    write_ranges=[(CONTEXT,36)],result_mask=0xffffffff,result_memory={'named':[CONTEXT+28,4]},
    native_cleanup=4 if words else 0,named_cleanup=0,canonical_cleanup=4 if words else 0,
    canonical_registers_and_flags=True,canonical_flag_mask=0xffffffef,
    context_outputs={'named':{name:[CONTEXT+off,4]for name,off in OFFSETS.items()}},
    context_compare=list(OFFSETS),context_flag_mask=0xffffffef,
    machine_sp={'named':SP-0x20000},borrowed_native_stack=True)
   out.append(c)
 # A real caller-owned stack object stays at the same address on all routes.
 entry=0x449630
 if entry in present:
  receiver=SP+4;obj=bytearray(256);obj[2]=2;eax=0x13579bdf;flags=0xac7
  snapshot=struct.pack('<9I',0x456789ab,0x3456789a,0x23456789,SP-4,0x12345678,0x33333333,receiver,eax,flags)
  out.append(dict(id='c5-bridge/00449630/caller-stack-alias',entry=entry,receiver=receiver,
   arguments=[],named_arguments=[CONTEXT],initial_eax=eax,eflags=flags,
   stores=[(receiver,bytes(obj)),(CONTEXT,snapshot)],watched=[(receiver,256)],write_ranges=[(CONTEXT,36)],
   result_mask=0xffffffff,result_memory={'named':[CONTEXT+28,4]},native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
   canonical_registers_and_flags=True,canonical_flag_mask=0xffffffef,
   context_outputs={'named':{name:[CONTEXT+off,4]for name,off in OFFSETS.items()}},context_compare=list(OFFSETS),context_flag_mask=0xffffffef,
   machine_sp={'named':SP-0x20000},borrowed_native_stack=True))
 return out
