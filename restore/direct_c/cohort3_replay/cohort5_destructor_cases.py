"""Normal destruction and real parent-EBP cleanup, with ordered element visits.

Only the proven one-byte empty element callback is controlled. Array iteration,
SEH registration, derived/base cleanup and every vtable write execute. Hardware
faults, throwing destructors and arbitrary exception personalities are unproved.
"""
import struct
S=0x32000000
SP=0x30008000
SELF=S+0x1000
FRAME=S+0x20000
LAYOUTS={
 0x4e5290:(0,0,[0x56f698]),0x545fa0:(0,0,[0x5701a0,0x56f698]),
 0x401390:(0x4c,16,[0x56f438,0x56f698]),0x40d780:(0x4c,25,[0x56f48c,0x56f698]),
 0x416f00:(0x4c,9,[0x56f4dc,0x56f698]),0x4e6530:(0x4c,2,[0x56f6e8,0x56f698]),
 0x545630:(0x1e5c,167,[0x570160,0x5701a0,0x56f698]),
 0x54f360:(0x5c88,513,[0x5701e8,0x5701a0,0x56f698]),
 0x555d70:(0x4ee0,437,[0x570228,0x5701a0,0x56f698]),
 0x558ad0:(0x1cbc,158,[0x570268,0x5701a0,0x56f698])}
FUNCLETS={
 0x56b3e0:(-0x44,0x4e5290),0x56b490:(-0x10,0x4e5290),
 0x56b4b0:(-0x44,0x4e5290),0x56b5a0:(-0x10,0x4e5290),
 0x56b5c0:(-0x44,0x4e5290),0x56b630:(-0x10,0x4e5290),
 0x56b6d0:(-0x44,0x4e5290),0x56b710:(-0x10,0x4e5290),
 0x56b810:(-0x44,0x545fa0),0x56bd80:(-0x10,0x545fa0),
 0x56bda0:(-0x44,0x545fa0),0x56cde0:(-0x10,0x545fa0),
 0x56ce00:(-0x44,0x545fa0),0x56dbe0:(-0x10,0x545fa0),
 0x56dc00:(-0x44,0x545fa0),0x56e120:(-0x10,0x545fa0)}

def w(value):return struct.pack('<I',value&0xffffffff)

def recipe(entry,seed,target=None):
 offset,count,tables=LAYOUTS[entry if target is None else target]
 self=SELF+seed*0x2000;frame=FRAME if not seed else SP+0x400
 visits=[[self+offset+i*52]for i in range(count-1,-1,-1)]+[[self+12]]
 c=dict(id=f'cohort5-dtor/{entry:08x}/{seed}',entry=entry,receiver=self if target is None else 0x89abcdef,
  arguments=[],named_arguments=[self]if target is None else[],initial_eax=0xa5b6c7d8^seed*0x77777777,
  initial_edx=0xfedcba98,eflags=0x202 if not seed else 0xad7,
  initial_fs0=0xffffffff,stores=[(self-16,b'\xa5'*(0x10000+32))],watched=[(self-16,0x10000+32)],
  write_ranges=[(self,4),(0,4)],write_trace_ranges=[(self,4)],expected_write_trace=[[self,4,t]for t in tables],
  result_mask=None,native_cleanup=0,named_cleanup=0,canonical_cleanup=0,canonical_registers_and_flags=False,
  endpoints=[dict(token='empty-element',native=0x51f780,compiled_symbols=['destroy_empty_element'],
   arity=1,native_arity=0,native_register_args={0:'ecx'},result_kind='void',native_cleanup=0,compiled_cleanup=0,
   observe=[[self,4]])],expected_endpoint_counts={'empty-element':len(visits)},expected_endpoint_arguments={'empty-element':visits})
 if target is None:
  # Canonical wrappers are in the original unit, not the new shared C owner.
  source={0x401390:1,0x40d780:98,0x416f00:194,0x4e5290:1967,0x4e6530:1992,
   0x545630:2562,0x545fa0:2575,0x54f360:2590,0x555d70:2603,0x558ad0:2616}[entry]
  c['canonical_owner']=f'unit_{source:05d}.obj'
 else:
  parent_offset,_=FUNCLETS[entry];c.update(initial_ebp=frame)
  c['stores'] += [(frame-128,b'\x5a'*256),(frame+parent_offset,w(self))]
  c['watched'].append((frame-128,256))
 return c

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']};out=[]
 for a in LAYOUTS:
  if a in present:out.extend(recipe(a,i)for i in range(2))
 for a,(_,target)in FUNCLETS.items():
  if a in present:out.extend(recipe(a,i,target)for i in range(2))
 return out
