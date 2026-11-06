"""Necessary AL/upper-EAX ABI cases; full advisor effect domains are deferred."""
import struct
S,SP,CONTEXT=0x32000000,0x30008000,0x32030000
SAVED=(0x12345678,0x23456789,0x3456789a,0x456789ab)
def w(v,n=4):return (v&((1<<(8*n))-1)).to_bytes(n,'little')
def definitions(registry):
 if not any(int(m['address'],16)==0x451b10 for m in registry['members']):return []
 out=[]
 for label,metric in [('empty',None),('below',15),('at',16),('signed-negative',0x8000),('high',0x7fff)]:
  for seed,eax in enumerate((0x89abcdef,0x1357bdf1)):
   flags=0x202 if not seed else 0xad7
   snapshot=b''.join(w(v)for v in [SAVED[3],SAVED[2],SAVED[1],SP-4,SAVED[0],0x76543210,0x89abcdef,eax,flags])
   # Keep a dirty upper WORD beside the actual signed WORD threshold.
   stores=[(0x7999d8,bytes(0x101e00)),(CONTEXT,snapshot),(0xeaac28,w(1)),(0xea5baa+0x49d8,w(0x76543210))]
   if metric is not None:stores += [(0x799ae0,b'\1'),(0x799af0,w(0xd2,2)),(0xea5baa+0x49d8,w(metric,2))]
   expected=(eax&0xffffff00)if metric is None else (eax&0xffff0000)|int(metric in (16,0x7fff))
   out.append(dict(id=f'c6-advisor-abi/{label}/{seed}',entry=0x451b10,receiver=0x89abcdef,
    arguments=[],named_arguments=[CONTEXT],initial_eax=eax,initial_edx=0x76543210,eflags=flags,
    stores=stores,watched=[(0x799ae0,264),(0xea5baa+0x49d8,4)],write_ranges=[(CONTEXT,36)],
    result_mask=0xffffffff,result_memory={'named':[CONTEXT+28,4]},expected_result=expected,
    native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_owner='unit_00789.obj',
    canonical_registers_and_flags=True,canonical_flag_mask=0xffffffef,
    machine_sp={'named':SP-0x20000},borrowed_native_stack=True,budget=12000000,
    gap_domains=['Only empty-record and type210 threshold branches are visited; type183 refresh child and malformed object/fault domains are deferred.',
     'The C logical result is AL; full physical EAX is compared here only because the captured native prestate is explicit. Undefined AF is excluded.']))
 return out
