"""Finite list member cases; reset, calendar and reference scans execute fully."""
import struct
S=0x32001000

def w(v,n=4):return (v&((1<<(8*n))-1)).to_bytes(n,'little')

def base(entry,label,args=()):
 return dict(id=f'c5-event-list/{entry:08x}/{label}',entry=entry,receiver=S,
  arguments=list(args),named_arguments=[S,*args],initial_eax=0x89abcdef,
  initial_edx=0x76543210,eflags=0xad7,stores=[(S-16,b'\xa5'*0x5040)],
  watched=[(S-16,0x5040)],write_ranges=[(S,0x5000)],
  native_cleanup=0 if entry==0x4489c0 else 4,named_cleanup=0,
  canonical_cleanup=0 if entry==0x4489c0 else 4,
  canonical_registers_and_flags=False,result_mask=None if entry==0x4489c0 else 0xff if entry==0x448e30 else 0xffffffff,
  canonical_owner={0x4489c0:'unit_00683.obj',0x448d10:'unit_00687.obj',0x448e30:'unit_00688.obj'}[entry],
  budget=2000000,gap_domains=['Finite allocated nonoverlapping records; no faults, asynchronous mutation or volatile-register equivalence.'])

def records(count):
 data=bytearray()
 for i in range(max(count,1)):
  r=bytearray((i*17+j*11)&255 for j in range(124))
  r[0:2]=w(i,2);r[40:44]=w(0)
  for off,v in zip((36,38,82,84),(-1,0,i+1,0x7fff)):r[off:off+2]=w(v,2)
  data.extend(r)
 return bytes(data)

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']};out=[]
 for salt in (0,255):
  c=base(0x4489c0,str(salt));c['stores'].append((S,bytes((i+salt)&255 for i in range(124))));out.append(c)
 for count,limited in ((0,0),(1,1),(3,0),(119,1),(120,1),(139,0),(140,0),(140,1),(120,256),(120,257)):
  c=base(0x448d10,f'count{count}-limited{limited}',(limited,))
  c['stores'] += [(S,w(count)),(S+4,records(count))]
  rejected=(bool(limited&255)and count>119)or count>139
  c['expected_result']=0 if rejected else S+4+count*124
  c['endpoints']=[dict(token='capacity-warning',native=0x4ce6e0,compiled_symbols=['FUN_004ce6e0'],arity=3,
   native_cleanup=0,compiled_cleanup=0,result_kind='void')]
  c['expected_endpoint_counts']={'capacity-warning':int(count>139 and not(limited&255))}
  out.append(c)
 # Test reusable type2 records with an absent, town or actor reference.
 for ref in ('absent','town','actor'):
  c=base(0x448d10,f'reuse-{ref}',(0,));r=bytearray(records(3));r[2]=2;r[20:22]=w(1,2);r[40:44]=w(12);r[50]=0
  c['stores'] += [(S,w(3)),(S+4,bytes(r)),(0x7848d8,w(0,2)),(0x78d7bc,w(0)),
   (0xbcba58,b'\0'*(0xc8ad42-0xbcba58+0x200))]
  if ref=='town':c['stores'] += [(0xbcba58,b'\1'),(0xbcbad0,w(0,2))]
  if ref=='actor':c['stores'] += [(0xbcd786,b'\x5c'),(0xbcd78a,b'\1'),(0xbcd8f8,w(0))]
  c['expected_result']=S+4 if ref=='absent'else S+4+3*124
  out.append(c)
 for count,index in ((0,0),(1,-1),(1,0),(1,1),(3,-1),(3,0),(3,1),(3,2),(3,3),(140,0),(140,139)):
  for scroll in sorted({0,count,count+4}):
   c=base(0x448e30,f'count{count}-index{index}-scroll{scroll}',(index,))
   c['stores'] += [(S,w(count)),(S+4,records(count)),(S+0x48ac,w(scroll)),(S+0x48b0,w(scroll))]
   c['expected_masked_result']=int(0<=index<count);out.append(c)
 return [c for c in out if c['entry']in present]
