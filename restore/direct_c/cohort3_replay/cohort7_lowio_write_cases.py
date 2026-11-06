"""Necessary offline low-I/O write cases with actual native stack cells."""
import struct
from cohort7_startup_cases import base as startup_base,put,TEXT,HEAP,COPY,child,imp
SP=0x30008000
FRAME=SP-1048
TABLE,CAP,ERR,DOS=0x1266960,0x1266a60,0x1266430,0x1266434
INVALID=0xffffffff
COUNT,CRS,TOTAL,CURSOR=FRAME+1032,FRAME+1028,FRAME+1036,FRAME+1040

def base(label,data=b'Q',flags=1,descriptor=35,count=None):
 n=len(data)if count is None else count
 c=startup_base(0x5664d5,label,0xffffffff)
 c['id']=c['id'].replace('c7-startup/','c7-final-state-write/')
 c.update(arguments=[descriptor,TEXT,n],named_arguments=[descriptor,TEXT,n],endpoints=[],expected_endpoint_counts={})
 c['stores'] += [(TABLE,b'\0'*256),(TEXT,data),(FRAME,b'\xa5'*1044)]
 for at,v in [(TABLE,HEAP),(TABLE+4,HEAP),(CAP,64),(ERR,0xaabbccdd),(DOS,0x12345678),(CRS,0x22334455),(COUNT,0x11223344),(TOTAL,0x55667788),(CURSOR,0x99aabbcc)]:put(c,at,v)
 for at,h in [(HEAP,0x71000000),(COPY,0x72000000)]:c['stores'].append((at,b''.join(struct.pack('<IBBH',h+i,1,10,0xa55a)for i in range(32))))
 c['stores'].append((HEAP+3*8+4,bytes([flags])))
 for at,size in [(TABLE,264),(ERR,8),(FRAME,1044),(SP+4,12)]:c['watched'].append((at,size));c['write_ranges'].append((at,size))
 if len(data)>256:c['watched'].append((TEXT,len(data)))
 return c

def response(value=1,written=None,mutations=(),text=None,observe=()):
 e=dict(value=value,mutations=list(mutations))
 if written is not None:e['writes_args']=[(3,4,written)]
 if text is not None:
  width=min(len(text),256)
  assert width>0
  e['private_input_args']={1:dict(role='write-text-prefix',width=width)}
 if observe:e['observe']=list(observe)
 return e

def endpoints(c,responses=(),errors=(),seek=None):
 write=imp('WriteFile',5);write.update(pointer_args={3:'write-count'},responses=list(responses))
 error=imp('GetLastError',0);error['responses']=[dict(value=v)for v in errors]
 append=child('append-seek',0x5662e3,3);append['responses']=[]if seek is None else[seek]
 c.update(endpoints=[write,error,append],expected_endpoint_counts={'WriteFile':len(responses),'GetLastError':len(errors),'append-seek':int(seek is not None)})
 if seek is not None:c['expected_endpoint_arguments']={'append-seek':[[35,0,2]]}

def binary_args(c,count,handle=0x71000003):
 c.setdefault('expected_endpoint_arguments',{})['WriteFile']=[[handle,TEXT,count,['stack-output','write-count'],0]]

def definitions(registry):
 if not any(int(m['address'],16)==0x5664d5 for m in registry['members']):return []
 out=[]
 c=base('unsigned-descriptor-error',descriptor=INVALID);endpoints(c);c['expected_eax']=INVALID;out.append(c)
 c=base('zero-request-precedes-append',flags=0xa1,count=0);endpoints(c);c['expected_eax']=0;out.append(c)
 c=base('binary-unwritten-count-is-real-stack');endpoints(c,[response()]);binary_args(c,1);c['expected_eax']=0x11223344;out.append(c)
 c=base('append-fresh-cell-switches-to-binary',b'xy',flags=0xa1)
 endpoints(c,[response(written=2)],seek=dict(value=INVALID,mutations=[(TABLE+4,4,COPY),(CAP,4,0),(COPY+3*8,4,0x72345678)]))
 binary_args(c,2,0x72345678);c['expected_eax']=2;out.append(c)
 c=base('denied-errno-before-DOS');endpoints(c,[response(value=0)],[5]);c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,9],[DOS,4,5]]);out.append(c)
 c=base('other-error-translates-full-DOS');endpoints(c,[response(value=0)],[6]);c['expected_eax']=INVALID;out.append(c)
 c=base('FALSE-error0-is-no-space');endpoints(c,[response(value=0)],[0]);c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,28],[DOS,4,0]]);out.append(c)
 c=base('zero-progress-device-CTRLZ',b'\x1a',flags=0x41);endpoints(c,[response(written=0)]);c['expected_eax']=0;out.append(c)
 c=base('text-LF-complete',b'A\nB',flags=0x81);endpoints(c,[response(written=4,text=b'A\r\nB')]);c['expected_eax']=3;out.append(c)
 c=base('partial-write-subtracts-all-packed-CRs',b'\n\n\n',flags=0x83);endpoints(c,[response(written=1,text=b'\r\n\r\n\r\n')]);c['expected_eax']=0xfffffffe;out.append(c)
 c=base('1025-byte-flush-and-fresh-next-handle',b'A'*1023+b'\nB',flags=0x81)
 boundary=[(FRAME+1023,1),(FRAME+1024,1),(FRAME+1025,1)]
 endpoints(c,[response(written=1025,text=b'A'*1023+b'\r\n',observe=boundary,mutations=[(TABLE+4,4,COPY),(CAP,4,0)]),response(written=1,text=b'B',observe=boundary)])
 c['expected_eax']=1025;out.append(c)
 c=base('failure-after-progress-keeps-unwritten-count',b'A'*1024+b'\n',flags=0x81)
 endpoints(c,[response(written=1024,text=b'A'*1024),response(value=0,text=b'\r\n')],[5]);c['expected_eax']=1023;out.append(c)
 c=base('text-TRUE-keeps-current-error-cell',b'Q',flags=0x81)
 endpoints(c,[response(written=0,text=b'Q',mutations=[(SP+4,4,5)])]);c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,9],[DOS,4,5]]);out.append(c)
 c=base('binary-TRUE-resets-error-cell',b'Q')
 endpoints(c,[response(written=0,mutations=[(SP+4,4,5)])]);c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,28],[DOS,4,0]]);out.append(c)
 c=base('append-zero-count-text-still-no-space',b'Q',flags=0xa1)
 endpoints(c,seek=dict(value=INVALID,mutations=[(SP+12,4,0)]));c.update(expected_eax=INVALID,write_trace_ranges=[(ERR,8)],expected_write_trace=[[ERR,4,28],[DOS,4,0]]);out.append(c)
 return out
