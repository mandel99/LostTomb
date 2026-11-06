"""Ten necessary offline allocator/handle-action fixtures; no host execution."""
import struct
from cohort7_startup_cases import base as startup_base,put,HEAP,COPY,child,imp
TABLE,CAP,MODE,ERR,DOS=0x1266960,0x1266a60,0x60496c,0x1266430,0x1266434
INVALID=0xffffffff

def base(entry,label,args=()):
 c=startup_base(entry,label,0xffffffff)
 c['id']=c['id'].replace('c7-startup/','c7-record-lowio-actions/')
 c.update(arguments=list(args),named_arguments=list(args),endpoints=[],expected_endpoint_counts={})
 c['stores'].append((TABLE,b'\0'*256))
 for at,value in [(CAP,64),(MODE,1),(ERR,0xaabbccdd),(DOS,0x12345678)]:put(c,at,value)
 for at,n in [(TABLE,264),(MODE,4),(ERR,8)]:c['watched'].append((at,n));c['write_ranges'].append((at,n))
 return c

def block(c,at,handle=0x70000000):
 c['stores'].append((at,b''.join(struct.pack('<IBBH',handle+i,0x81,0x3c,0x5aa5)for i in range(32))))

def record(c,at,index,handle,flags):
 c['stores'].append((at+index*8,struct.pack('<IBBH',handle&0xffffffff,flags,0x3c,0x5aa5)))

def malloc(c,values=(),mutations=None):
 e=child('allocate',0x561b68,1)
 e['responses']=[dict(value=v,mutations=(mutations or {}).get(i,[]))for i,v in enumerate(values)]
 c.update(endpoints=[e],expected_endpoint_counts={'allocate':len(values)},expected_endpoint_arguments={'allocate':[[256]for _ in values]})

def std(c,arguments=None,mutations=None):
 e=imp('SetStdHandle',2,0)
 if arguments is None:
  e['responses']=[]
  c.update(endpoints=[e],expected_endpoint_counts={'SetStdHandle':0})
 else:
  e['mutations']=mutations or []
  c.update(endpoints=[e],expected_endpoint_counts={'SetStdHandle':1},expected_endpoint_arguments={'SetStdHandle':[arguments]})

def allocator_cases():
 out=[]
 c=base(0x566840,'existing-second-block-closed-byte');block(c,HEAP);block(c,COPY,0x71000000)
 put(c,TABLE,HEAP);put(c,TABLE+4,COPY);record(c,COPY,3,0xf1234567,0x80)
 malloc(c);c['expected_eax']=35;out.append(c)
 c=base(0x566840,'new-second-block-fresh-capacity');block(c,HEAP);put(c,TABLE,HEAP);put(c,CAP,32)
 malloc(c,[COPY],{0:[(CAP,4,256)]});c['expected_eax']=32;out.append(c)
 c=base(0x566840,'first-null-slot-allocation-failure');block(c,HEAP);put(c,TABLE,HEAP);put(c,CAP,32)
 malloc(c,[0]);c['expected_eax']=INVALID;out.append(c)
 c=base(0x566840,'all64-slots-open-independent-of-capacity');block(c,HEAP);put(c,CAP,1)
 for i in range(64):put(c,TABLE+i*4,HEAP)
 malloc(c);c['expected_eax']=INVALID;out.append(c)
 return out

def setter_cases():
 out=[]
 c=base(0x5668d5,'DWORD-mode101-skips-API',[0,0xf1234567]);block(c,HEAP);put(c,TABLE,HEAP);put(c,MODE,0x101)
 record(c,HEAP,0,INVALID,0x80);std(c);c['expected_eax']=0;out.append(c)
 c=base(0x5668d5,'captured-slot-fresh-block-after-callback',[1,0x81234567]);block(c,HEAP);block(c,COPY,0x71000000);put(c,TABLE,HEAP)
 record(c,HEAP,1,INVALID,0x80)
 std(c,[0xfffffff5,0x81234567],[(TABLE,4,COPY),(CAP,4,0)])
 c['expected_eax']=0;out.append(c)
 c=base(0x5668d5,'occupied-second-block-error',[35,0xf7654321]);block(c,COPY);put(c,TABLE+4,COPY)
 std(c);c['expected_eax']=INVALID;out.append(c)
 return out

def free_cases():
 out=[]
 c=base(0x56694c,'captured-slot-clear-fresh-block-after-callback',[2]);block(c,HEAP);block(c,COPY,0x71000000);put(c,TABLE,HEAP)
 record(c,COPY,2,0x71234567,0x80)
 std(c,[0xfffffff4,0],[(TABLE,4,COPY),(CAP,4,0)])
 c['expected_eax']=0;out.append(c)
 c=base(0x56694c,'closed-flags-byte-error',[35]);block(c,COPY);put(c,TABLE+4,COPY);record(c,COPY,3,0xf1234567,0x80)
 std(c);c['expected_eax']=INVALID;out.append(c)
 c=base(0x56694c,'unsigned-capacity-error-before-table',[INVALID]);put(c,TABLE,0xdeadbeef)
 std(c);c['expected_eax']=INVALID;out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in allocator_cases()+setter_cases()+free_cases()if c['entry']in present]
