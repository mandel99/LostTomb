"""Pure case recipes. Additional cohort families remain explicit pending work."""
import struct
from formatter_cases import definitions as formatter_definitions
SCRATCH=0x32000000

def definitions(registry):
 out=[]
 for m in registry['members']:
  a=int(m['address'],16);rule=m.get('rule')or{}
  if rule.get('kind')=='lookup':
   self=SCRATCH+0x1000
   for index,value in [(0,0),(1,0xa5),(127,128),(255,255)]:
    for flags in [0x202,0xad7]:
     table=rule['table_base']+index
     out.append(dict(id=f'lookup/{a:08x}/{index}/{flags:x}',entry=a,
      receiver=self,arguments=[],named_arguments=[self],eflags=flags,
      stores=[(self,b'\xa5'*64),(self+rule['offset'],bytes([index])),(table,bytes([value]))],
      watched=[(self,64),(rule['table_base']&~4095,4096)],write_ranges=[],
      expected_eax=value,result_mask=0xffffffff,native_cleanup=0,named_cleanup=0,
      canonical_cleanup=0,canonical_registers_and_flags=True))
  elif a in [0x56896e,0x5689c4]:
   words=SCRATCH+0x1000
   for label,values in [('zero',[0,0,0]),('carry',[0xffffffff]*3),('mixed',[0x13579bdf,0x2468ace0,0x80000001])]:
    for bit in [0,31,32,63,64,95]:
     out.append(dict(id=f'bit96/{a:08x}/{label}/{bit}',entry=a,receiver=0x22222222,
      arguments=[words,bit],named_arguments=[words,bit],eflags=0x202,
      stores=[(words,b'\xa5'*64),(words,struct.pack('<III',*values))],
      watched=[(words,64)],write_ranges=[(words,12)],result_mask=0xffffffff,
      native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False))
 out.extend(formatter_definitions(registry))
 return out
