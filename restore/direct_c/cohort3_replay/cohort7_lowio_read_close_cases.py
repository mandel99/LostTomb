"""Necessary offline text-read/close cases with real parent output cells."""
import struct
from cohort7_startup_cases import base as startup_base,put,TEXT,HEAP,COPY,child,imp
SP=0x30008000
FRAME=SP-16
TABLE,CAP,MODE,ERR,DOS=0x1266960,0x1266a60,0x60496c,0x1266430,0x1266434
INVALID=0xffffffff

def base(entry,label,args):
 c=startup_base(entry,label,0xffffffff)
 c['id']=c['id'].replace('c7-startup/','c7-view-read/')
 c.update(arguments=list(args),named_arguments=list(args),endpoints=[],expected_endpoint_counts={})
 c['stores'].append((TABLE,b'\0'*256))
 for at,value in [(TABLE,HEAP),(TABLE+4,HEAP),(CAP,64),(MODE,0x101),(ERR,0xaabbccdd),(DOS,0x12345678)]:put(c,at,value)
 for at,n in [(TABLE,264),(MODE,4),(ERR,8)]:c['watched'].append((at,n));c['write_ranges'].append((at,n))
 for at,h,flags in [(HEAP,0x71000000,0x81),(COPY,0x72000000,0x89)]:
  c['stores'].append((at,b''.join(struct.pack('<IBBH',h+i,flags,10,0xa55a)for i in range(32))))
 if entry==0x5660ed:
  c['stores'].append((FRAME,struct.pack('<II4B',0x11223344,0x55667788,0xa1,0xb2,0xc3,0x5a)))
  for at,n in [(FRAME,12),(SP+4,12)]:c['watched'].append((at,n));c['write_ranges'].append((at,n))
 return c

def rec(c,index=0,flags=0x81,state=10,handle=None):
 at=HEAP+index*8
 c['stores'].append((at+4,bytes([flags,state])))
 if handle is not None:put(c,at,handle)

def primary(payload=b'',count=None,value=1,at=TEXT):
 e=dict(value=value)
 if count is not None:e['writes_args']=[(3,4,count)]
 if payload:e['mutations']=[(at+i,1,v)for i,v in enumerate(payload)]
 return e

def tail(value=1,byte=None,count=None,mutations=()):
 e=dict(value=value,pointer_args={1:'read-tail',3:'read-count'},mutations=list(mutations))
 writes=[]
 if count is not None:writes.append((3,4,count))
 if byte is not None:writes.append((1,1,byte))
 if writes:e['writes_args']=writes
 return e

def read_endpoints(c,responses=(),errors=(),seek_response=None):
 read=imp('ReadFile',5);read.update(pointer_args={3:'read-count'},responses=list(responses))
 error=imp('GetLastError',0);error['responses']=[dict(value=v)for v in errors]
 seek=child('seek-back',0x5662e3,3)
 seek['responses']=[]if seek_response is None else[seek_response]
 c.update(endpoints=[read,error,seek],expected_endpoint_counts={'ReadFile':len(responses),'GetLastError':len(errors),'seek-back':int(seek_response is not None)})


def read_cases():
 out=[]
 c=base(0x5660ed,'unsigned-descriptor-error',[INVALID,TEXT,4]);read_endpoints(c);c['expected_eax']=INVALID;out.append(c)
 c=base(0x5660ed,'zero-request-keeps-unwritten-output-cells',[0,TEXT,0]);read_endpoints(c);c['expected_eax']=0;out.append(c)
 c=base(0x5660ed,'binary-full-BOOL-and-count',[0,TEXT,5]);rec(c,flags=1)
 read_endpoints(c,[primary(b'abc',3,0x100)]);c['expected_eax']=3;out.append(c)
 c=base(0x5660ed,'pending-pipe-byte-decrements-real-count',[0,TEXT,3]);rec(c,flags=9,state=ord('X'))
 read_endpoints(c,[primary(b'YZ',2,at=TEXT+1)]);c['expected_eax']=3;out.append(c)
 c=base(0x5660ed,'primary-denied-errno-before-DOS',[0,TEXT,4]);read_endpoints(c,[primary(value=0)],[5])
 c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,9],[DOS,4,5]]);out.append(c)
 c=base(0x5660ed,'broken-pipe-drops-pending-count-result',[0,TEXT,3]);rec(c,flags=9,state=ord('X'))
 read_endpoints(c,[primary(value=0)],[109]);c['expected_eax']=0;out.append(c)
 c=base(0x5660ed,'primary-failure-error0-still-translates',[0,TEXT,4]);read_endpoints(c,[primary(value=0)],[0]);c['expected_eax']=INVALID;out.append(c)
 data=b'A\r\nB\x1aTAIL'
 c=base(0x5660ed,'text-CRLF-CTRLZ-and-real-cursor',[0,TEXT,len(data)]);rec(c,flags=0x85)
 read_endpoints(c,[primary(data,len(data))]);c['expected_eax']=3;out.append(c)
 c=base(0x5660ed,'first-trailing-CR-LF-no-seek',[0,TEXT,1]);read_endpoints(c,[primary(b'\r',1),tail(byte=10,count=1)]);c['expected_eax']=1;out.append(c)
 c=base(0x5660ed,'pipe-tail-fresh-record-lookahead',[0,TEXT,2]);rec(c,flags=0x89)
 read_endpoints(c,[primary(b'Q\r',2),tail(byte=ord('Z'),count=1,mutations=[(TABLE,4,COPY)])]);c['expected_eax']=2;out.append(c)
 c=base(0x5660ed,'tail-failure-error0-reuses-count-and-unwritten-byte',[0,TEXT,1]);c['stores'].append((FRAME+11,b'\n'))
 read_endpoints(c,[primary(b'\r',1),tail(value=0)],[0]);c['expected_eax']=1;out.append(c)
 c=base(0x5660ed,'regular-tail-seek-ignored-and-byte-reloaded',[0,TEXT,2])
 read_endpoints(c,[primary(b'Q\r',2),tail(byte=10,count=1)],seek_response=dict(value=INVALID,mutations=[(FRAME+11,1,ord('Z'))]))
 c['expected_endpoint_arguments']={'seek-back':[[0,INVALID,1]]};c['expected_eax']=2;out.append(c)
 return out


def close_imports(c,values=(),errors=()):
 close=imp('CloseHandle',1);close['responses']=[dict(value=v)for v in values]
 error=imp('GetLastError',0);error['responses']=[dict(value=v)for v in errors]
 std=imp('SetStdHandle',2);std['responses']=[]
 c['endpoints'] += [close,error,std]
 c['expected_endpoint_counts'].update(CloseHandle=len(values),GetLastError=len(errors),SetStdHandle=0)


def close_cases():
 out=[]
 c=base(0x5647eb,'closed-flags-ordered-error',[35]);rec(c,3,flags=0x80)
 getter=child('get-handle',0x5669c6,1);getter['responses']=[]
 free=child('free-handle',0x56694c,1);free['responses']=[]
 c.update(endpoints=[getter,free],expected_endpoint_counts={'get-handle':0,'free-handle':0},expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[DOS,4,0],[ERR,4,9]])
 close_imports(c);out.append(c)
 c=base(0x5647eb,'shared-stdout-stderr-exact-getter-order',[1]);shared=0xf1234567;rec(c,1,handle=shared);rec(c,2,handle=shared)
 getter=child('get-handle',0x5669c6,1);getter['responses']=[dict(value=shared)]*3
 c.update(endpoints=[getter],expected_endpoint_counts={'get-handle':3},expected_endpoint_arguments={'get-handle':[[1],[2],[1]]},expected_eax=0)
 close_imports(c);out.append(c)
 c=base(0x5647eb,'captured-close-error-fresh-slot-after-free',[35])
 getter=child('get-handle',0x5669c6,1);getter['responses']=[dict(value=0x71234567),dict(value=0x72345678)]
 free=child('free-handle',0x56694c,1,INVALID);free['mutations']=[(TABLE+4,4,COPY),(CAP,4,0),(COPY+3*8+4,1,0xf7),(DOS,4,0x11223344),(ERR,4,22)]
 c.update(endpoints=[getter,free],expected_endpoint_counts={'get-handle':2,'free-handle':1},expected_endpoint_arguments={'get-handle':[[35],[35]],'free-handle':[[35]],'CloseHandle':[[0x72345678]],'GetLastError':[[]]},expected_eax=INVALID,write_trace_ranges=[(ERR,8),(COPY+3*8+4,1)])
 close_imports(c,[0],[6]);out.append(c)
 c=base(0x5647eb,'invalid-getter-ignored-free-failure',[35]);rec(c,3,handle=INVALID)
 close_imports(c);c['expected_eax']=0;out.append(c)
 c=base(0x5647eb,'ordinary-close-full-BOOL-success',[35]);close_imports(c,[0x100]);c['expected_endpoint_arguments']={'CloseHandle':[[0x71000003]]};c['expected_eax']=0;out.append(c)
 return out


def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in read_cases()+close_cases()if c['entry']in present]
