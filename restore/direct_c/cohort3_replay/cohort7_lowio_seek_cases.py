"""Twelve necessary offline seek, length and OS-error cases."""
import struct
from cohort7_startup_cases import base as startup_base,put,HEAP,COPY,child,imp
TABLE,CAP,ERR,DOS=0x1266960,0x1266a60,0x1266430,0x1266434
INVALID=0xffffffff

def base(entry,label,args=(),result=0xffffffff):
 c=startup_base(entry,label,result)
 c['id']=c['id'].replace('c7-startup/','c7-monument-seek/')
 c.update(arguments=list(args),named_arguments=list(args),endpoints=[],expected_endpoint_counts={})
 c['stores'].append((TABLE,b'\0'*256))
 for at,value in [(TABLE,HEAP),(TABLE+4,HEAP),(CAP,64),(ERR,0xaabbccdd),(DOS,0x12345678)]:put(c,at,value)
 for at,n in [(TABLE,264),(ERR,8)]:c['watched'].append((at,n));c['write_ranges'].append((at,n))
 for at,first,flags in [(HEAP,0x71000000,0x83),(COPY,0x72000000,0xf7)]:
  c['stores'].append((at,b''.join(struct.pack('<IBBH',first+i,flags,0x33,0x55aa)for i in range(32))))
 return c

def api_seek(c,position=None,last_error=None,mutations=()):
 seek=imp('SetFilePointer',4,0 if position is None else position)
 if position is None:seek['responses']=[]
 else:seek['mutations']=list(mutations)
 error=imp('GetLastError',0,0 if last_error is None else last_error)
 if last_error is None:error['responses']=[]
 c.update(endpoints=[seek,error],expected_endpoint_counts={'SetFilePointer':int(position is not None),'GetLastError':int(last_error is not None)})
 return c

def seek_cases():
 out=[]
 c=base(0x5662e3,'unsigned-descriptor-error',[INVALID,-42,2]);api_seek(c);c['expected_eax']=INVALID;out.append(c)
 c=base(0x5662e3,'invalid-os-handle-preserves-DOS',[0,0,1]);put(c,HEAP,INVALID)
 api_seek(c);c['expected_eax']=INVALID;out.append(c)
 c=base(0x5662e3,'success-high-position-fresh-captured-cell',[35,-42,0x80000002])
 api_seek(c,0x81234567,mutations=[(TABLE+4,4,COPY),(CAP,4,0)])
 c['expected_endpoint_arguments']={'SetFilePointer':[[0x71000003,0xffffffd6,0,0x80000002]]}
 c['expected_eax']=0x81234567;out.append(c)
 c=base(0x5662e3,'ffffffff-lastError0-is-success',[0,0,1]);api_seek(c,INVALID,0)
 c['expected_endpoint_arguments']={'SetFilePointer':[[0x71000000,0,0,1]],'GetLastError':[[]]}
 c['expected_eax']=INVALID;out.append(c)
 c=base(0x5662e3,'ffffffff-nonzero-error-translates',[0,0,2]);api_seek(c,INVALID,5)
 c['expected_endpoint_arguments']={'SetFilePointer':[[0x71000000,0,0,2]],'GetLastError':[[]]}
 c['expected_eax']=INVALID;out.append(c)
 return out

def length_cases():
 out=[]
 for label,values,mutations,expected in [
  ('current-failure-skips-END',[INVALID],{},INVALID),
  ('END-failure-still-restores',[17,INVALID,0x55667788],{1:[(DOS,4,5),(ERR,4,13)],2:[(DOS,4,0x12345678),(ERR,4,22)]},INVALID),
  ('high-END-survives-restore-failure',[17,0x81234567,INVALID],{2:[(DOS,4,5),(ERR,4,13)]},0x81234567)]:
  c=base(0x56b1b6,label,[35]);seek=child('seek',0x5662e3,3)
  seek['responses']=[dict(value=v,mutations=mutations.get(i,[]))for i,v in enumerate(values)]
  arguments=[[35,0,1]]
  if len(values)>1:arguments.append([35,0,2])
  if len(values)>2:arguments.append([35,17,0])
  c.update(endpoints=[seek],expected_endpoint_counts={'seek':len(values)},expected_endpoint_arguments={'seek':arguments},expected_eax=expected)
  out.append(c)
 return out

def translator_cases():
 out=[]
 for label,code,values in [
  ('last-table-row-full-DOS',1816,[12]),
  ('access-denied-fallback19',19,[13]),
  ('executable-format-fallback188',188,[8]),
  ('above-format-range-ordered8-then22',INVALID,[8,22])]:
  c=base(0x5672c8,label,[code],None)
  c['write_trace_ranges']=[(ERR,8)]
  c['expected_write_trace']=[[DOS,4,code]]+[[ERR,4,v]for v in values]
  out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in seek_cases()+length_cases()+translator_cases()if c['entry']in present]
