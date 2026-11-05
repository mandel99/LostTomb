"""Finite normal flow and explicit termination boundaries for CRT exceptions."""
import struct
S=0x32000000
FRAME,EXC,REG,DESC,CTX,GUARD,TABLE,DISPATCH,POINTERS,THROW=[S+i*0x1000 for i in range(1,11)]
CB,CB2=0x31000b00,0x31000c00
NLG=0x60492c
ENTRIES=(0x561875,0x5640a0,0x5637ee,0x56870b,0x56158a,0x5615de,
         0x563935,0x5617a8,0x5617c8,0x5617ea,0x561852,0x56187e)

def w(value):return struct.pack('<I',value&0xffffffff)

def case(entry,label,args,named=None,result=0xffffffff,cleanup=0,seh=False):
 blocks=(FRAME,EXC,REG,DESC,CTX,GUARD,TABLE,DISPATCH,POINTERS,THROW)
 stores=[(p-32,b'\xa5'*320)for p in blocks]+[(NLG-4,b'\xa5'*20)]
 watched=[(p-32,320)for p in blocks]+[(NLG-4,20)]
 c=dict(id=f'exception/{entry:08x}/{label}',entry=entry,receiver=0x22222222,
  arguments=args,named_arguments=args if named is None else named,eflags=0xad7,
  stores=stores,watched=watched,write_ranges=[(NLG,12)],result_mask=result,
  native_cleanup=cleanup,named_cleanup=0,canonical_cleanup=cleanup,
  canonical_registers_and_flags=False,initial_ebp=FRAME)
 owner='crt_exception_entries.obj'if entry in (0x561875,0x5640a0,0x5637ee,0x56870b)else'crt_exception_dispatch_entries.obj'if entry in (0x56158a,0x5615de,0x563935)else'crt_local_unwind_entries.obj'
 c['canonical_owner']=owner
 if seh:c.update(initial_fs0=0xffffffff);c['write_ranges'].append((0,4))
 return c

def put(c,address,value):c['stores'].append((address,w(value)))

def endpoint(token,address,arity=0,value=0x13579bdf,cleanup=0):
 return dict(token=token,controlled_address=address,arity=arity,result_kind='full32',
  value=value,native_cleanup=cleanup,compiled_cleanup=cleanup)

def abort_endpoint():
 return dict(token='abort',native=0x5686f4,compiled_symbols=['FUN_005686f4'],arity=0,
  result_kind='void',native_cleanup=0,compiled_cleanup=0,noreturn=True)

def notify_cases():
 out=[]
 for entry in (0x561875,0x56187e):
  for destination in (0x13579bdf,0xfdb97531):
   for code in (0,0x100,0xfedcba98):
    named=[destination,code,FRAME]if entry==0x561875 else[destination,FRAME]
    c=case(entry,f'{destination:x}/{code:x}',[0x76543210],named,cleanup=4)
    c.update(initial_eax=destination,receiver=code,canonical_registers_and_flags=True)
    put(c,FRAME+8,code);out.append(c)
 return out

def invoke_cases():
 out=[]
 for entry in (0x5640a0,0x56158a):
  for code in (0x100,0x101,0xfedcba98):
   for moved in (False,True):
    args=[CB,FRAME,code]if entry==0x5640a0 else[FRAME,DESC,CB,0xffffffff if moved else 2,code]
    c=case(entry,f'{code:x}/{int(moved)}',args,cleanup=12 if entry==0x5640a0 else 0,seh=entry==0x56158a)
    e=endpoint('handler',CB,value=0x89abcdef);e.update(registers=['eax','ecx','ebp'])
    if moved:e['clobber_ebp']=FRAME+0x60
    if entry==0x56158a:
     e['fs0_registration']=dict(size=20,native_handler=0x5615de,compiled_handler_symbol='FUN_005615de',fields=[(0,4),(8,4),(12,4),(16,4)])
    c.update(endpoints=[e],expected_endpoint_counts={'handler':1});out.append(c)
 return out

def unwind_cases():
 out=[]
 for flags in (0,2,4,6,0x40,0x80000002):
  c=case(0x5617c8,f'flags-{flags:x}',[EXC,REG,CTX,DISPATCH]);put(c,EXC+4,flags)
  c['write_ranges'].append((DISPATCH,4));out.append(c)
 for mode in ('wrong-handler','equal','unequal'):
  c=case(0x561852,mode,[],seh=True);c['initial_fs0']=GUARD
  put(c,GUARD+4,0x12345678);put(c,GUARD+8,2);put(c,GUARD+12,REG);put(c,REG+12,3 if mode=='unequal'else 2)
  if mode!='wrong-handler':
   c['function_pointers']=[dict(address=GUARD+4,native=0x5617c8,compiled_symbol='FUN_005617c8')]
   c['watched'].remove((GUARD-32,320));c['watched'] += [(GUARD-32,36),(GUARD+8,280)]
  out.append(c)
 for mode in ('empty','stop','filtered','cleanup','two-cleanups','callback-mutates-level'):
  c=case(0x5617ea,mode,[REG,1 if mode=='stop'else 0xffffffff],[REG,1 if mode=='stop'else 0xffffffff,FRAME],seh=True)
  level=0xffffffff if mode=='empty'else 1
  put(c,REG+8,TABLE);put(c,REG+12,level);put(c,FRAME+8,0x12344321)
  put(c,TABLE,0xffffffff);put(c,TABLE+4,0);put(c,TABLE+8,CB2)
  put(c,TABLE+12,0 if mode=='two-cleanups'else 0xffffffff)
  put(c,TABLE+16,1 if mode=='filtered'else 0);put(c,TABLE+20,CB)
  c['write_ranges'].append((REG+12,4))
  e=endpoint('cleanup',CB);e.update(registers=['eax','ecx','ebp'],observe=[(REG+12,4)])
  e['fs0_registration']=dict(size=16,native_handler=0x5617c8,compiled_handler_symbol='FUN_005617c8',fields=[(0,4),(8,4),(12,4)])
  if mode=='callback-mutates-level':e['mutations']=[(REG+12,4,0)]
  e2=endpoint('cleanup-two',CB2);e2.update(registers=['eax','ecx','ebp'],observe=[(REG+12,4)],fs0_registration=e['fs0_registration'])
  c.update(endpoints=[e,e2],expected_endpoint_counts={'cleanup':int(mode in ('cleanup','two-cleanups','callback-mutates-level')),'cleanup-two':int(mode in ('two-cleanups','callback-mutates-level'))})
  out.append(c)
 for target in (REG,REG+0x40):
  c=case(0x5617a8,f'target-{target:x}',[target],result=None)
  c['endpoints']=[dict(token='rtl-unwind',import_name='RtlUnwind',arity=4,native_cleanup=16,compiled_cleanup=16,result_kind='void',continuation_args={1:'target-ip'})]
  c['expected_endpoint_counts']={'rtl-unwind':1};out.append(c)
 return out

def dispatch_cases():
 out=[]
 for flags in (0,2,0x66):
  for depth in (0,0x80000001):
   c=case(0x5615de,f'{flags:x}/{depth:x}',[EXC,REG,CTX]);c['eflags']=0x602;c['expected_flags']=(0x400,0)
   for at,value in ((EXC,0xc0000005),(EXC+4,flags),(DESC,0x19930520),(DESC+4,0),(DESC+12,0),(REG+8,DESC),(REG+12,FRAME),(REG+16,depth)):put(c,at,value)
   out.append(c)
 for mode in ('foreign','foreign-high-depth','rethrow-empty','cpp-empty','cpp-must-catch'):
  cpp=mode.startswith('cpp')or mode.startswith('rethrow');must=int(mode=='cpp-must-catch')
  depth=0x80000001 if mode=='foreign-high-depth'else 0
  args=[EXC,FRAME,CTX,0x76543210,DESC,must,depth,REG]
  c=case(0x563935,mode,args,result=None)
  for at,value in ((FRAME+8,0),(DESC,0x19930520),(DESC+4,3),(DESC+12,0),(DESC+16,TABLE),(EXC,0xe06d7363 if cpp else 0xc0000005),(EXC+16,3),(EXC+20,0x19930520),(EXC+28,0 if mode=='rethrow-empty'else THROW),(0x1266418,0)):put(c,at,value)
  c['watched'].append((0x1266418,4));eps=[];counts={}
  if not cpp:
   eps.append(dict(token='foreign-search',native=0x563adc,compiled_symbols=['FUN_00563adc'],arity=8,native_cleanup=0,compiled_cleanup=0,result_kind='void'));counts['foreign-search']=1
  if must:
   eps.append(dict(token='release-exception',native=0x56400a,compiled_symbols=['FUN_0056400a'],arity=2,native_cleanup=0,compiled_cleanup=0,result_kind='void'));counts['release-exception']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out

def termination_cases():
 out=[]
 for handler in (0,CB):
  c=case(0x5637ee,f'handler-{handler:x}',[],result=None,seh=True)
  put(c,0x1266414,handler);c['watched'].append((0x1266414,4))
  c.update(noreturn_endpoint='abort',endpoints=[endpoint('terminate-handler',CB),abort_endpoint()],expected_endpoint_counts={'terminate-handler':int(bool(handler)),'abort':1});out.append(c)
 for mode in ('no-filter','invalid','valid','validator-reloads','cpp-terminate'):
  terminate=mode=='cpp-terminate'
  c=case(0x56870b,mode,[POINTERS],cleanup=4,result=None if terminate else 0xffffffff,seh=terminate)
  for at,value in ((POINTERS,EXC),(POINTERS+4,CTX),(EXC,0xe06d7363 if terminate else 0xc0000005),(EXC+16,3),(EXC+20,0x19930520),(0x12666a4,0 if mode=='no-filter'else CB),(0x1266414,0)):put(c,at,value)
  c['watched'] += [(0x12666a4,4),(0x1266414,4)];c['write_ranges'].append((0x12666a4,4))
  # Native5687A7 ends MOV EAX,ESI with ESI explicitly0/1. Its consumer
  # tests full EAX; an AL-only false with dirty high bits is not reachable.
  validator=dict(token='validate-filter',native=0x5687a7,compiled_symbols=['FUN_005687a7'],arity=1,native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=0 if mode=='invalid'else 1,observe=[(0x12666a4,4)])
  if mode=='validator-reloads':validator['mutations']=[(0x12666a4,4,CB2)]
  callback=endpoint('filter',CB,1,0xffffffff,4);callback['observe']=[(0x12666a4,4)]
  callback2=endpoint('filter-two',CB2,1,0x12345678,4);callback2['observe']=[(0x12666a4,4)]
  c.update(endpoints=[validator,callback,callback2,abort_endpoint()],expected_endpoint_counts={'validate-filter':int(mode not in ('no-filter','cpp-terminate')),'filter':int(mode=='valid'),'filter-two':int(mode=='validator-reloads'),'abort':int(terminate)})
  if terminate:c['noreturn_endpoint']='abort'
  out.append(c)
 return out

def definitions(registry):
 assert set(ENTRIES)<={int(m['address'],16)for m in registry['members']}
 out=notify_cases()+invoke_cases()+unwind_cases()+dispatch_cases()+termination_cases()
 assert {c['entry']for c in out}==set(ENTRIES)
 return out
