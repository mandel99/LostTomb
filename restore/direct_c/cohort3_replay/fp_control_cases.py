"""Finite x87 environment cases; nested decoder/encoder algorithms execute."""
import struct

SP=0x30008000
CONTROL_WORDS=(0x37f,0x27f,0x7f,0x77f,0xb7f,0xf7f)

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 assert {0x5640fe,0x5687bf,0x5687f4}<=present
 out=[]
 settings=((0,0),(0,0x8001f),(0x1f,0x1f),(0x300,0x300),
           (0x100,0x300),(0x200,0x300),(0x80000,0x80000),
           (0x12345678,0xffffffff))
 for entry in (0x5687bf,0x5687f4):
  for cw in CONTROL_WORDS:
   for i,(value,mask)in enumerate(settings):
    args=[value,mask]
    named=args+[SP+8]if entry==0x5687bf else args
    out.append(dict(id=f'fp-control/{entry:08x}/{cw:x}/{i}',entry=entry,
     receiver=0x22222222,arguments=args,named_arguments=named,eflags=0x202,
     # Only control87 exposes a native output-slot pointer in its named API.
     # controlfp's by-value mask storage is a compiler spill, not an output.
     stores=[],watched=[(SP+8,4)]if entry==0x5687bf else[],write_ranges=[],result_mask=0xffffffff,
     native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
     canonical_registers_and_flags=False,initial_fpcw=cw,compare_x87=True))
 probes=(('classic',4195835.0,3145727.0,1.0),
         ('exact',6.0,3.0,0.0),('positive-threshold',1.0,3.0,1.0),
         ('negative-threshold',1.0,3.0,-1.0),
         ('unordered',float('nan'),3.0,0.0))
 for cw in CONTROL_WORDS:
  for label,numerator,divisor,threshold in probes:
   stores=[(at,struct.pack('<d',value))for at,value in
           ((0x5706b0,numerator),(0x5706b8,divisor),(0x56f520,threshold))]
   out.append(dict(id=f'fp-control/005640fe/{cw:x}/{label}',entry=0x5640fe,
    receiver=0x22222222,arguments=[],named_arguments=[],eflags=0x202,
    stores=stores,watched=[(at,len(payload))for at,payload in stores],
    write_ranges=[],result_mask=0xffffffff,native_cleanup=0,named_cleanup=0,
    canonical_cleanup=0,canonical_registers_and_flags=False,
    initial_fpcw=cw,compare_x87=True))
 return out
