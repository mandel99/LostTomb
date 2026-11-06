import struct
SELF=0x32008000
N=0x48b8

def definitions(registry=None):
 out=[]
 for fill in [0xa5,0x5a]:
  c=dict(id='c7-event-constructor/0044ba20/fill-%02x'%fill,entry=0x44ba20,
   receiver=SELF,arguments=[],named_arguments=[SELF],canonical_arguments=[SELF],
   eflags=0xad7,stores=[(SELF-16,bytes([fill])*(N+32))],
   watched=[(SELF-16,N+32),(0,4)],write_ranges=[(SELF,N),(0,4)],
   initial_fs0=0x33445566,result_mask=0xffffffff,expected_eax=SELF,
   native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
   canonical_registers_and_flags=False,endpoints=[],expected_endpoint_counts={},
   compare_x87_values=True,compare_x87_control=True,initial_fpcw=0x27f,
   initial_x87=['0000000000000080ff3f','000000000000008000c0'],budget=1000000,
   authored_final_memory=[(SELF,'00000000'),(SELF+4,'0000'),
    (SELF+4+149*124,'9500'),(SELF+0x48ac,'ffffffff'),
    (SELF+0x48b0,'00000000'),(SELF+0x43d6,'02'),(SELF+0x48b4,'0000'),
    (SELF-16,bytes([fill]).hex()*16),(SELF+N,bytes([fill]).hex()*16)])
  out.append(c)
 return out
