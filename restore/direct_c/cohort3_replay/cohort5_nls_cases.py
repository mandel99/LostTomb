"""Private cohort5 NLS recipes; generation only, no host execution.

Imports are controlled Win32 boundaries. The real bounded-byte dependency and
both normal dynamic-allocation/SEH paths execute. Only initialized wide bytes
are compared; CPINFO padding and unused allocation bytes are not observations.
"""
import struct

S=0x32000000
SP=0x30008000
RECORD=SP+0x100
FIRST,SECOND,OUTPUT=S+0x11000,S+0x12000,S+0x13000
MAP,COMPARE=0x56947e,0x56ac16
MAP_CACHE,COMPARE_CACHE,DEFAULT_CP=0x12666e0,0x126670c,0x126649c
MAP_APIS={'LCMapStringW':6,'LCMapStringA':6,'MultiByteToWideChar':6,'WideCharToMultiByte':8}
COMPARE_APIS={'CompareStringW':6,'CompareStringA':6,'MultiByteToWideChar':6,'GetCPInfo':2}


def w(value):
 return struct.pack('<I',value&0xffffffff)


def words(values):
 return b''.join(w(value)for value in values)


def response(value,**kw):
 return dict(value=value,**kw)


def public_bytes(payload):
 return [[OUTPUT+i,1,value]for i,value in enumerate(payload)]


def api(name,arity,responses):
 return dict(token=name,import_name=name,arity=arity,result_kind='full32',value=0,
  native_cleanup=arity*4,compiled_cleanup=arity*4,responses=responses)


def base(entry,label,args,backend,first=b'Ab\0unused',second=b'zY\0tail'):
 cache=MAP_CACHE if entry==MAP else COMPARE_CACHE
 n=len(args)*4
 return dict(id=f'c5-nls/{entry:08x}/{label}',entry=entry,receiver=0x89abcdef,
  arguments=args,named_arguments=[RECORD],eflags=0x202,initial_edx=0x76543210,
  initial_fs0=0xffffffff,stores=[(cache,w(backend)),(DEFAULT_CP,w(932)),
   (FIRST,b'\x7d'*64),(FIRST,first),(SECOND,b'\x6b'*64),(SECOND,second),(OUTPUT,b'\xa5'*64)],
  watched=[(cache,4),(SP+4,n),(OUTPUT,64)],
  route_watched={'named':[(cache,4),(RECORD,n),(OUTPUT,64)]},
  route_stores={'named':[(RECORD,words(args).hex())]},
  write_ranges=[(0,4),(cache,4),(OUTPUT,64)],result_mask=0xffffffff,
  native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
  canonical_registers_and_flags=False,canonical_owner='crt_nls_mapping_entries.obj')


def finish(case,apitypes,calls,result):
 case.update(endpoints=[api(name,arity,calls.get(name,[]))for name,arity in apitypes.items()],
  expected_endpoint_counts={name:len(calls.get(name,[]))for name in apitypes},expected_result=result)
 return case


def conversion(payload,role,fail=None):
 """One query/copy pair; the copy fills exactly its advertised WCHAR count."""
 count=len(payload)//2
 if fail=='query':return [response(0)]
 copy=response(0 if fail=='copy'else count,pointer_args={4:role})
 if fail!='copy':copy['writes_arg_bytes']=[[4,0,payload.hex()]]
 return [response(count),copy]


def map_wide(label,backend=1,path='success',count=7,codepage=0,strict=0,
             first=b'Ab\0tail',wide=b'A\0b\0',mapped=b'A\0B\0',output=b'AB',capacity=16):
 args=[0x409,0x200,FIRST,count,OUTPUT,capacity,codepage,strict]
 c=base(MAP,label,args,backend,first)
 conv=conversion(wide,'map-source-wide',{'size-fail':'query','convert-fail':'copy'}.get(path))
 calls={'MultiByteToWideChar':conv}
 wide_calls=[]
 if backend==0:wide_calls.append(response(1))
 if path not in('size-fail','convert-fail'):
  inp={2:dict(role='map-source-wide',width=len(wide))}
  wide_calls.append(response(0 if path=='map-size-fail'else len(mapped)//2,private_input_args=inp))
  if path!='map-size-fail':
   copy=response(0 if path=='map-copy-fail'else len(mapped)//2,
    private_input_args=inp,pointer_args={4:'map-result-wide'})
   if path!='map-copy-fail':copy['writes_arg_bytes']=[[4,0,mapped.hex()]]
   wide_calls.append(copy)
   if path!='map-copy-fail':
    final=response(0 if path=='encode-fail'else len(output),
     private_input_args={2:dict(role='map-result-wide',width=len(mapped))})
    if path!='encode-fail'and capacity:final['mutations']=public_bytes(output)
    calls['WideCharToMultiByte']=[final]
 calls['LCMapStringW']=wide_calls
 return finish(c,MAP_APIS,calls,0 if path!='success'else len(output))


def map_sortkey(label,capacity,path='success',backend=1):
 key=b'\x0e\x02\x01\x01\0'
 args=[0x409,0x400,FIRST,7,OUTPUT,capacity,0,1]
 c=base(MAP,label,args,backend)
 inp={2:dict(role='map-source-wide',width=4)}
 maps=([response(1)]if backend==0 else[])+[response(len(key),private_input_args=inp)]
 if capacity>=len(key):
  copy=response(0 if path=='copy-fail'else len(key),private_input_args=inp)
  if path!='copy-fail':copy['mutations']=public_bytes(key)
  maps.append(copy)
 result=len(key)if capacity==0 or capacity>=len(key)and path!='copy-fail'else 0
 return finish(c,MAP_APIS,{'LCMapStringW':maps,
  'MultiByteToWideChar':conversion(b'A\0b\0','map-source-wide')},result)


def compare_wide(label,backend=1,path='success',counts=(7,7),codepage=0,
                 first=b'Ab\0tail',second=b'zY\0tail',
                 first_wide=b'A\0b\0',second_wide=b'z\0Y\0',result=1,flags=0):
 args=[0x409,flags,FIRST,counts[0],SECOND,counts[1],codepage]
 c=base(COMPARE,label,args,backend,first,second)
 first_fail={'first-size-fail':'query','first-copy-fail':'copy'}.get(path)
 conv=conversion(first_wide,'compare-first-wide',first_fail)
 wide_calls=[response(2)]if backend==0 else[]
 if path not in('first-size-fail','first-copy-fail'):
  second_fail={'second-size-fail':'query','second-copy-fail':'copy'}.get(path)
  conv+=conversion(second_wide,'compare-second-wide',second_fail)
  if path not in('second-size-fail','second-copy-fail'):
   wide_calls.append(response(0 if path=='compare-fail'else result,
    private_input_args={2:dict(role='compare-first-wide',width=len(first_wide)),
                        4:dict(role='compare-second-wide',width=len(second_wide))}))
 calls={'MultiByteToWideChar':conv,'CompareStringW':wide_calls}
 return finish(c,COMPARE_APIS,calls,result if path=='success'else 0)


def cpinfo(maxsize=2):
 # Actual UINT and unsigned BYTE fields. The two trailing padding bytes are
 # neither initialized nor compared. Win32 lead ranges are terminated.
 leads=b'\x81\x9f\xe0\xfc'+b'\0'*8 if maxsize==2 else b'\0'*12
 return w(maxsize)+b'?\0'+leads


def compare_zero(label,counts,first=b'Aq\0',second=b'By\0',codepage=0,
                 maxsize=2,cp_success=True,result=2,convert=None,needs_cpinfo=None):
 c=base(COMPARE,label,[0x409,0,FIRST,counts[0],SECOND,counts[1],codepage],1,first,second)
 calls={}
 if needs_cpinfo is None:needs_cpinfo=counts[0]!=counts[1]and counts[0]<=1 and counts[1]<=1
 if needs_cpinfo:
  info=response(int(cp_success),pointer_args={1:'cpinfo-fields'})
  if cp_success:info['writes_arg_bytes']=[[1,0,cpinfo(maxsize).hex()]]
  calls['GetCPInfo']=[info]
 if convert=='zero-first':calls['MultiByteToWideChar']=[response(0)]
 if convert=='zero-second':
  calls['MultiByteToWideChar']=conversion(b'A\0\0\0','compare-first-wide')+[response(0)]
 return finish(c,COMPARE_APIS,calls,result)


def definitions(registry):
 out=[]
 # ANSI uses the real trimmed source count and preserves an unused codepage.
 for label,count,text,capacity,result,payload,flags in (
  ('ansi-trim',7,b'Ab\0tail',16,2,b'AB',0x200),
  ('ansi-exhausted-nonnul',2,b'AbX\0',16,2,b'AB',0x200),
  ('ansi-exhausted-nul',2,b'Ab\0',16,2,b'AB',0x200),
  ('ansi-terminated-minus-one',-1,b'Ab\0',16,3,b'AB\0',0x200),
  ('ansi-empty-trim',4,b'\0abc',16,0,b'',0x200),
  ('ansi-zero-count',0,b'Ab\0',16,0,b'',0x200),
  ('ansi-size-query',2,b'Ab\0',0,2,b'',0x200),
  ('ansi-capacity-failure',2,b'Ab\0',1,0,b'',0x200),
  ('ansi-sortkey',2,b'Ab\0',16,5,b'\x0e\x02\x01\x01\0',0x400)):
  c=base(MAP,label,[0x409,flags,FIRST,count,OUTPUT,capacity,1252,0],2,text)
  r=response(result)
  if payload:r['mutations']=public_bytes(payload)
  out.append(finish(c,MAP_APIS,{'LCMapStringA':[r]},result))
 # Each converted or mapped private buffer has independent, initialized bytes.
 for strict,cp in ((0,0),(1,0),(-1,1252),(0,1252)):
  out.append(map_wide(f'wide-strict-{strict}-cp-{cp}',strict=strict,codepage=cp))
 out.append(map_wide('wide-distinct-size-counts',count=8,first=b'a\0unused',wide=b'a\0',mapped=b'A\0',output=b'A'))
 out.append(map_wide('wide-cp932-byte-wide-counts',count=7,strict=1,first=b'\x82\xa0\0tail',wide=b'\x42\x30',mapped=b'\x42\x30',output=b'\x82\xa0'))
 out.append(map_wide('wide-minus-one-includes-terminator',count=-1,first=b'Ab\0',wide=b'A\0b\0\0\0',mapped=b'A\0B\0\0\0',output=b'AB\0'))
 out.append(map_wide('wide-destination-size-query',capacity=0))
 for path in ('size-fail','convert-fail','map-size-fail','map-copy-fail','encode-fail'):
  out.append(map_wide('wide-'+path,path=path))
 out.append(map_wide('wide-zero-count-size-failure',path='size-fail',count=0))
 out.append(map_wide('wide-empty-trim-size-failure',path='size-fail',count=4,first=b'\0abc'))
 for capacity in (0,4,5,12):out.append(map_sortkey(f'sortkey-capacity-{capacity}',capacity))
 out.append(map_sortkey('sortkey-copy-failure',5,'copy-fail'))
 out.append(map_wide('probe-wide-success',backend=0))
 out.append(map_sortkey('probe-wide-sortkey',5,backend=0))
 for label,ansi_result in (('probe-ansi-success',1),('probe-both-fail',0)):
  c=base(MAP,label,[0x409,0x200,FIRST,7,OUTPUT,16,0,1],0)
  calls={'LCMapStringW':[response(0)],'LCMapStringA':[response(ansi_result)]}
  if ansi_result:calls['LCMapStringA'].append(response(2,mutations=public_bytes(b'AB')))
  out.append(finish(c,MAP_APIS,calls,2 if ansi_result else 0))
 out.append(finish(base(MAP,'unknown-backend',[0x409,0x200,FIRST,7,OUTPUT,16,0,1],3),MAP_APIS,{},0))

 # Distinct source pointers and lengths exercise both actual argument slots.
 for label,counts,first,second,result,flags in (
  ('ansi-trim-distinct',(7,8),b'Ab\0tail',b'z\0unused',1,0),
  ('ansi-trim-first-only',(2,-1),b'AbX\0',b'Ab\0',2,0),
  ('ansi-minus-one-both',(-1,-1),b'z\0',b'A\0',3,0),
  ('ansi-ignore-case',(1,1),b'aX\0',b'AY\0',2,1),
  ('ansi-zero-count',(0,1),b'A\0',b'BX\0',0,0),
  ('ansi-empty-trim',(3,2),b'\0xx',b'BC\0',0,0)):
  c=base(COMPARE,label,[0x409,flags,FIRST,counts[0],SECOND,counts[1],1252],2,first,second)
  out.append(finish(c,COMPARE_APIS,{'CompareStringA':[response(result)]},result))
 out.append(compare_wide('wide-distinct-lengths',counts=(7,8),second=b'z\0unused',second_wide=b'z\0'))
 out.append(compare_wide('wide-explicit-codepage',codepage=1252,result=3))
 out.append(compare_wide('wide-cp932-byte-wide-counts',counts=(2,2),first=b'\x82\xa0X\0',second=b'\x82\xa2Y\0',first_wide=b'\x42\x30',second_wide=b'\x44\x30'))
 out.append(compare_wide('wide-ignore-case',counts=(1,1),first=b'aX\0',second=b'AY\0',first_wide=b'a\0',second_wide=b'A\0',result=2,flags=1))
 out.append(compare_wide('wide-minus-one-both',counts=(-1,-1),first=b'Ab\0',second=b'zY\0',first_wide=b'A\0b\0\0\0',second_wide=b'z\0Y\0\0\0'))
 out.append(compare_wide('wide-positive-and-minus-one',counts=(2,-1),first=b'AbX\0',second=b'zY\0',second_wide=b'z\0Y\0\0\0'))
 for path in ('first-size-fail','first-copy-fail','second-size-fail','second-copy-fail','compare-fail'):
  out.append(compare_wide('wide-'+path,path=path))
 out.append(compare_wide('probe-wide-success',backend=0))
 for label,ansi_result in (('probe-ansi-success',2),('probe-both-fail',0)):
  c=base(COMPARE,label,[0x409,0,FIRST,7,SECOND,8,0],0)
  calls={'CompareStringW':[response(0)],'CompareStringA':[response(ansi_result)]}
  if ansi_result:calls['CompareStringA'].append(response(1))
  out.append(finish(c,COMPARE_APIS,calls,1 if ansi_result else 0))
 out.append(finish(base(COMPARE,'unknown-backend',[0x409,0,FIRST,7,SECOND,8,0],3),COMPARE_APIS,{},0))

 out.append(compare_zero('zero-both',(0,0)))
 out.append(compare_zero('zero-first-other-long',(0,2),result=1))
 out.append(compare_zero('zero-second-other-long',(2,0),result=3))
 # Both unsigned range boundaries and two separate CP932 lead ranges.
 for byte in (0x80,0x81,0x9f,0xa0,0xdf,0xe0,0xfc,0xfd):
  lead=0x81<=byte<=0x9f or 0xe0<=byte<=0xfc
  out.append(compare_zero(f'zero-first-range-{byte:02x}',(0,1),second=bytes([byte])+b'Y\0',result=2 if lead else 1))
  out.append(compare_zero(f'zero-second-range-{byte:02x}',(1,0),first=bytes([byte])+b'X\0',result=2 if lead else 3))
 for counts,result in (((0,1),1),((1,0),3)):
  out.append(compare_zero(f'zero-sbcs-{counts[0]}-{counts[1]}',counts,first=b'\x81X\0',second=b'\x81Y\0',codepage=1252,maxsize=1,result=result))
  out.append(compare_zero(f'zero-cpinfo-failure-{counts[0]}-{counts[1]}',counts,cp_success=False,result=0))
 out.append(compare_zero('zero-first-minus-one-second',(0,-1),second=b'B\0',result=0,convert='zero-first'))
 out.append(compare_zero('minus-one-first-zero-second',(-1,0),first=b'A\0',result=0,convert='zero-second'))
 # Positive trimming itself creates the zero-count branches before CPINFO.
 out.append(compare_zero('trim-first-to-zero',(3,1),first=b'\0xx',second=b'\x81Y\0',result=2,needs_cpinfo=True))
 out.append(compare_zero('trim-second-to-zero',(1,3),first=b'\x81X\0',second=b'\0yy',result=2,needs_cpinfo=True))
 present={int(member['address'],16)for member in registry['members']}
 return [case for case in out if case['entry']in present]
