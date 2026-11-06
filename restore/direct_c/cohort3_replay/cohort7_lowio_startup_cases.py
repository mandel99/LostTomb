"""Six necessary offline low-I/O startup cases; no host execution."""
import struct
from cohort7_startup_cases import base as startup_base,put,TEXT,HEAP,COPY,WIDE,child,imp
TABLE,CAP=0x1266960,0x1266a60
ENTRY=0x566a03
INVALID=0xffffffff

def base(label):
 c=startup_base(ENTRY,label,None)
 c['id']=c['id'].replace('c7-startup/','c7-scalar-lowio/')
 c['stores'].append((TABLE,b'\0'*256));put(c,CAP,0xaabbccdd)
 c['watched'].append((TABLE,264));c['write_ranges'].append((TABLE,264))
 return c

def startup(c,count=None,flags=None,handles=None,cb=None):
 endpoint=imp('GetStartupInfoA',1);endpoint.update(result_kind='void',pointer_args={0:'STARTUPINFOA'})
 if count is None:reserved,pointer=0,0
 else:
  flags=flags if flags is not None else bytes([9])*max(count,0)
  handles=handles if handles is not None else [0x71000000+i for i in range(max(count,0))]
  data=struct.pack('<I',count&0xffffffff)+bytes(flags)+b''.join(struct.pack('<I',v&0xffffffff)for v in handles)
  c['stores'].append((TEXT,data));reserved=len(data)if cb is None else cb;pointer=TEXT
 endpoint['writes_arg_bytes']=[(0,50,struct.pack('<HI',reserved,pointer).hex())]
 return endpoint

def endpoints(c,start,alloc_values,std_values,type_values,set_count,mutations_std=None,mutations_type=None):
 allocate=child('allocate',0x561b68,1)
 allocate['responses']=[dict(value=v)for v in alloc_values]
 std=imp('GetStdHandle',1)
 std['responses']=[dict(value=v,mutations=(mutations_std or {}).get(i,[]))for i,v in enumerate(std_values)]
 typ=imp('GetFileType',1)
 typ['responses']=[dict(value=v,mutations=(mutations_type or {}).get(i,[]))for i,v in enumerate(type_values)]
 final=imp('SetHandleCount',1,0x12345678)
 c.update(endpoints=[allocate,start,std,typ,final],expected_endpoint_counts={
  'allocate':len(alloc_values),'GetStartupInfoA':1,'GetStdHandle':len(std_values),'GetFileType':len(type_values),'SetHandleCount':1})
 c['expected_endpoint_arguments']={'allocate':[[256]for _ in alloc_values],'SetHandleCount':[[set_count]]}
 return c

def lowio_startup_cases():
 out=[]
 c=base('no-inheritance-standard-types')
 endpoints(c,startup(c),[HEAP],[INVALID,0x71000001,0x71000002],[2,3],32)
 c['expected_endpoint_arguments'].update(GetStdHandle=[[0xfffffff6],[0xfffffff5],[0xfffffff4]],GetFileType=[[0x71000001],[0x71000002]])
 out.append(c)
 c=base('inherited-byte-flags-unaligned-handles')
 flags=bytes([9,1,0,1,9]);handles=[0x71000100,0x71000101,0x71000102,INVALID,0x71000104]
 endpoints(c,startup(c,5,flags,handles),[HEAP],[0x71000099],[3,0x102],32)
 c['expected_endpoint_arguments'].update(GetStdHandle=[[0xfffffff4]],GetFileType=[[handles[1]],[0x71000099]])
 c['read_ranges']=[(TEXT+4,5)]
 c['expected_reads']=[[TEXT+4,1],[TEXT+4,1],[TEXT+5,1],[TEXT+5,1],[TEXT+6,1],[TEXT+8,1],[TEXT+8,1]]
 out.append(c)
 c=base('negative-count-skips-inheritance')
 endpoints(c,startup(c,-1,cb=1),[HEAP],[INVALID]*3,[],32)
 c['expected_endpoint_arguments'].update(GetStdHandle=[[0xfffffff6],[0xfffffff5],[0xfffffff4]])
 out.append(c)
 c=base('extra-block-allocation-failure')
 endpoints(c,startup(c,33),[HEAP,0],[],[],32)
 out.append(c)
 c=base('inherited-callback-reloads-payload-and-table')
 original=0x01020304
 endpoints(c,startup(c,1,b'\1',[original]),[HEAP],[],[1],32,
  mutations_type={0:[(TABLE,4,COPY),(TEXT+5,4,0x77665544),(TEXT+4,1,0xa9)]})
 c['expected_endpoint_arguments'].update(GetFileType=[[original]])
 out.append(c)
 c=base('standard-captured-record-and-fresh-next-block')
 for i in range(3):put(c,COPY+i*8,INVALID)
 endpoints(c,startup(c),[HEAP],[0x70010001,0x70010002,INVALID],[3,0x102],256,
  mutations_std={0:[(TABLE,4,COPY)]},mutations_type={0:[(HEAP+4,1,0x21),(CAP,4,256)]})
 c['expected_endpoint_arguments'].update(GetStdHandle=[[0xfffffff6],[0xfffffff5],[0xfffffff4]],GetFileType=[[0x70010001],[0x70010002]])
 out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return lowio_startup_cases()if ENTRY in present else []

