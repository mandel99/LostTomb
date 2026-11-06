"""Necessary ABI fixtures for connected CRT pointer/scalar-unit repairs."""
import struct
S=0x32000000
TEXT,HEAP,COPY,WIDE=[S+x*0x1000 for x in range(1,5)]
MB,READY,CMD,ENV,ENVV,ARGV,ARGC,PROGRAM,MODE=[0x1266a84,0x1266a80,0x1267aa4,0x1266400,0x1266458,0x1266450,0x126644c,0x1266468,0x126669c]
MODULE=0x1266598

def put(c,at,value):c['stores'].append((at,struct.pack('<I',value&0xffffffff)))
def base(entry,label,result=None):
 globals_=[MB,READY,CMD,ENV,ENVV,ARGV,ARGC,PROGRAM,MODE]
 blocks=[TEXT,HEAP,COPY,WIDE]
 c=dict(id=f'c7-startup/{entry:08x}/{label}',entry=entry,receiver=0x89abcdef,
  arguments=[],named_arguments=[],eflags=0xad7,stores=[(p,b'\xa5'*256)for p in blocks],
  watched=[(p,256)for p in blocks]+[(g,4)for g in globals_]+[(MODULE,0x104)],
  write_ranges=[(p,256)for p in blocks]+[(g,4)for g in globals_]+[(MODULE,0x104)],
  result_mask=result,native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
  canonical_registers_and_flags=False)
 for g in globals_:put(c,g,0xaabbccdd)
 for at,value in [(MB,0x100),(CMD,TEXT),(ENV,TEXT),(READY,0xaabbcc00),(MODE,0x101)]:put(c,at,value)
 c['stores'].append((MODULE,b'\xa5'*0x104))
 return c

def child(token,at,arity,value=0):
 return dict(token=token,native=at,compiled_symbols=[f'FUN_{at:08x}'],arity=arity,
  native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=value)
def imp(name,arity,value=0):
 return dict(token=name,import_name=name,arity=arity,native_cleanup=arity*4,
  compiled_cleanup=arity*4,result_kind='full32',value=value)

def tail_cases():
 out=[]
 for lead in [False,True]:
  c=base(0x56793c,'quoted-lead-byte'if lead else'unquoted-high-init',0xffffffff)
  text=b'"\x81"x" \t--ok\0'if lead else b'game.exe \t--ok\0'
  c['stores'].append((TEXT,text));c['expected_eax']=TEXT+(7 if lead else 10)
  eps=[];counts={}
  if lead:
   e=child('lead-byte',0x5697ef,1);e['responses']=[dict(value=1),dict(value=0)]
   eps=[e];counts['lead-byte']=2
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def multibyte_cases():
 out=[]
 for initial in [0,0x100]:
  c=base(0x569bf5,f'initial-{initial:x}');put(c,MB,initial)
  eps=[];counts={}
  if initial==0:
   e=child('set-codepage',0x569831,1,0x12345678);e['mutations']=[(MB,4,0xaabbcc00)]
   eps=[e];counts['set-codepage']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def arguments_cases():
 out=[]
 for fallback in [False,True]:
  c=base(0x567a4d,'empty-command-fallback'if fallback else'command-pointer')
  c['stores'] += [(TEXT,b'\0XYZ'if fallback else b'game.exe "two words" \x81z\0'),(0x1266840,b'\0'*257)]
  c['stores'].append((0x1266841+0x81,b'\x04'))
  module=imp('GetModuleFileNameA',3,5)
  module['mutations']=[(MODULE+i,1,v)for i,v in enumerate(b'F.exe\0')]
  malloc=child('allocate',0x561b68,1,HEAP)
  c.update(endpoints=[module,malloc],expected_endpoint_counts={'GetModuleFileNameA':1,'allocate':1})
  out.append(c)
 return out

def environment_vector_case():
 c=base(0x567994,'pointer-vector-and-ready-dword')
 strings=[b'=C:=D:\\',b'ONE=1',b'TWO=2']
 c['stores'].append((TEXT,b'\0'.join(strings)+b'\0\0'))
 strlen=child('length',0x563380,1);strlen['responses']=[dict(value=len(s))for s in strings+strings]
 malloc=child('allocate',0x561b68,1);malloc['responses']=[dict(value=p)for p in [HEAP,COPY,COPY+32]]
 copy=child('string-copy',0x566750,2);copy['result_kind']='void'
 free=child('release',0x561bdc,1);free['result_kind']='void'
 c.update(endpoints=[strlen,malloc,copy,free],expected_endpoint_counts={'length':6,'allocate':3,'string-copy':2,'release':1})
 return [c]

def environment_block_cases():
 out=[]
 for mode in [0,2,0x101]:
  c=base(0x567c9a,f'mode-{mode:x}',0xffffffff);put(c,MODE,mode)
  eps=[];counts={}
  if mode==0:
   c['stores'].append((WIDE,'Z=\u017e\0\0'.encode('utf-16le')))
   acquire=imp('GetEnvironmentStringsW',0,WIDE);acquire['mutations']=[(MODE,4,0xaabbcc00)]
   convert=imp('WideCharToMultiByte',8,8)
   allocate=child('allocate',0x561b68,1,HEAP)
   free=imp('FreeEnvironmentStringsW',1,0)
   eps=[acquire,convert,allocate,free];counts={'GetEnvironmentStringsW':1,'WideCharToMultiByte':2,'allocate':1,'FreeEnvironmentStringsW':1}
  elif mode==2:
   c['stores'].append((TEXT,b'A=1\0B=2\0\0'))
   acquire=imp('GetEnvironmentStrings',0,TEXT);allocate=child('allocate',0x561b68,1,HEAP)
   copy=child('byte-copy',0x563400,3);copy['result_kind']='void'
   free=imp('FreeEnvironmentStringsA',1,0)
   eps=[acquire,allocate,copy,free];counts={'GetEnvironmentStrings':1,'allocate':1,'byte-copy':1,'FreeEnvironmentStringsA':1}
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=0 if mode==0x101 else HEAP)
  out.append(c)
 return out

def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in tail_cases()+multibyte_cases()+arguments_cases()+environment_vector_case()+environment_block_cases()if c['entry']in present]
