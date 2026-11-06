"""Connected EH3 frame/continuation ABI recipes, using the shared runner.

Controlled callbacks can ask the guest's actual EH3 handler to dispatch. OS
unwind is a declared boundary; no host exception or original game is launched.
"""
import struct

S,SP,NLG=0x32000000,0x30008000,0x60492c
EXC,REG,DESC,FRAME,CTX,GUARD,TABLE,TABLE2,EXC2,PTRS,FRAME2=[S+i*0x1000 for i in range(1,12)]
CB0,CB1,SELECTED,DEST=0x31000900,0x31000a00,0x31000b00,0x31000c00
R=SP+0x800


def put(c,at,value):
 c['stores'].append((at,struct.pack('<I',value&0xffffffff)))


def base(entry,label,args,result=None):
 blocks=[EXC,REG,DESC,FRAME,CTX,GUARD,TABLE,TABLE2,EXC2,PTRS,FRAME2]
 c=dict(id=f'c7-eh3/{entry:08x}/{label}',entry=entry,receiver=0x89abcdef,
  arguments=args,named_arguments=args,eflags=0xad7,initial_edx=0x76543210,
  stores=[(p-32,b'\xa5'*256)for p in blocks]+[(0x18,struct.pack('<I',0))],
  watched=[(p-32,256)for p in blocks]+[(0,4),(NLG+4,8)],
  write_ranges=[(p-32,256)for p in blocks]+[(0,4),(NLG,12),(SP-64,128)],
  result_mask=result,native_cleanup=0,named_cleanup=0,canonical_cleanup=0,
  canonical_registers_and_flags=False,initial_fs0=GUARD)
 for at,value in [(EXC,0xc0000005),(EXC+4,0),(EXC+16,3),(EXC+20,0x19930520),(EXC+28,TABLE2),
  (EXC2,0xe06d7363),(EXC2+4,0),(EXC2+16,3),(EXC2+20,0x19930520),(EXC2+28,0),
  (PTRS,EXC),(PTRS+4,CTX),(GUARD,REG),(FRAME-4,0x12345678),(FRAME+8,1),
  (DESC,0x19930520),(DESC+4,2),(DESC+8,TABLE),(DESC+12,0),
  (TABLE,0xffffffff),(TABLE+4,CB0),(TABLE+8,0),(TABLE+12,CB1),
  (0x1266418,0x11223344),(0x126641c,0x55667788)]:put(c,at,value)
 c['watched'] += [(0x1266418,8)]
 c['write_ranges'] += [(0x1266418,8)]
 return c


def endpoint(token,at,value=0):
 return dict(token=token,controlled_address=at,arity=0,native_cleanup=0,
  compiled_cleanup=0,result_kind='full32',value=value)


def rtl():
 return dict(token='RtlUnwind',import_name='RtlUnwind',arity=4,
  native_cleanup=16,compiled_cleanup=16,result_kind='void',
  continuation_args={1:'compiled-unwind-continuation'},fs0_target_frame=True)


def row(parent,filter_at,filter_symbol,handler_at,handler_symbol):
 return dict(enclosing=parent,native_filter=filter_at,compiled_filter=filter_symbol,
  native_handler=handler_at,compiled_handler=handler_symbol)


SCOPES={
 'terminate':[row(-1,0,None,0x563895,'restore_crt_terminate_finally'),
  row(0,0x563881,'restore_crt_eh3_execute_terminate_filter',0x563885,'restore_crt_terminate_recover')],
 'cleanup':[row(-1,0x563c48,'restore_crt_eh3_cleanup_filter',0x563c52,'restore_crt_cleanup_recover')],
 'catch':[row(-1,0,None,0x563dce,'restore_crt_catch_finally'),
  row(0,0x563dac,'restore_crt_eh3_catch_filter',0x563db6,'restore_crt_catch_recover')]
}


def registration(kind,depth=0):
 return dict(size=16,native_handler=0x561a90,compiled_handler_symbol='FUN_00561a90',
  fields=[(0,4),(12,4)],scope_rows=SCOPES[kind],chain_depth=depth)


def terminate(kind=None):
 e=dict(token='terminate',native=0x5637ee,compiled_symbols=['FUN_005637ee'],
  arity=0,native_cleanup=0,compiled_cleanup=0,result_kind='void',noreturn=True)
 if kind:e['fs0_registration']=registration(kind)
 return e


def dispatch(exception=EXC):
 return dict(native_handler=0x561a90,compiled_handler='FUN_00561a90',exception=exception,context=CTX)


def jump_cases():
 out=[]
 for flags in [0x202,0xad7,0x602]:
  c=base(0x5614c3,f'context-{flags:x}',[DEST,FRAME]);c['eflags']=flags
  put(c,FRAME-4,SP+0x1000)
  e=endpoint('continuation',DEST);e.update(result_kind='void',noreturn=True,
   registers=['eax','ecx','edx','ebx','esp','ebp','esi','edi'])
  c.update(endpoints=[e],noreturn_endpoint='continuation',
   expected_endpoint_counts={'continuation':1},
   context_compare=['eax','ecx','edx','ebx','esp','ebp','esi','edi','eflags'])
  out.append(c)
 return out


def handler_cases():
 out=[]
 for mode in ['empty','zero','negative','chain-mutation','selected-stop','selected-return','unwind','empty-unwind']:
  c=base(0x561a90,mode,[EXC,R,CTX,0],result=None if mode=='selected-stop'else 0xffffffff)
  c['initial_fs0']=R;c['stores'].append((R-8,b'\xa5'*80))
  c['watched'].append((R,64));c['write_ranges'].append((R-8,80))
  for at,value in [(R,GUARD),(R+4,0),(R+8,TABLE),(R+12,-1 if mode in ['empty','empty-unwind']else 1),
   (R+24,0x31415926),(R+28,R+0x40),
   (TABLE,-1),(TABLE+4,CB0),(TABLE+8,SELECTED),
   (TABLE+12,0),(TABLE+16,0 if mode=='unwind'else CB1),(TABLE+20,CB1)]:put(c,at,value)
  # Two rows use twelve-byte EH3 stride, unlike the C++ unwind map above.
  if mode=='unwind':put(c,TABLE+4,0)
  eps=[];counts={}
  if mode in ['empty','empty-unwind']:c['expected_eax']=1
  elif mode=='unwind':
   for token,at in [('finally-one',CB1),('finally-zero',SELECTED)]:
    e=endpoint(token,at,0x12345678);e.update(registers=['ecx','ebp'],observe=[(R+12,4)],
     fs0_registration=dict(size=16,native_handler=0x5617c8,
      compiled_handler_symbol='FUN_005617c8',fields=[(0,4),(8,4),(12,4)]))
    eps.append(e);counts[token]=1
   c['expected_eax']=1
  else:
   value=-1 if mode=='negative'else 1 if mode.startswith('selected')else 0
   first=endpoint('filter-one',CB1,value&0xffffffff)
   first.update(registers=['ecx','ebp'],private_register_inputs={'eax':dict(role='EXCEPTION_POINTERS',width=8)})
   eps.append(first);counts['filter-one']=1
   if mode in ['zero','chain-mutation','selected-return']:
    second=endpoint('filter-zero',CB0,0xffffffff if mode!='zero'else 0)
    second['registers']=['eax','ecx','ebp'];eps.append(second);counts['filter-zero']=1
   if mode=='chain-mutation':
    first['mutations']=[(R+8,4,TABLE2)]
    for at,value in [(TABLE2,-1),(TABLE2+4,CB0),(TABLE2+8,SELECTED),
                     (TABLE2+12,0),(TABLE2+16,CB1),(TABLE2+20,SELECTED)]:put(c,at,value)
   if mode.startswith('selected'):
    put(c,TABLE+20,SELECTED)
    chosen=endpoint('selected',SELECTED,0x98765432);chosen['registers']=['eax','ecx','ebp']
    if mode=='selected-stop':chosen.update(result_kind='void',noreturn=True);c['noreturn_endpoint']='selected'
    eps += [rtl(),chosen];counts.update(RtlUnwind=1,selected=1)
   c['expected_eax']=1 if mode=='zero'else 0
  if 'unwind'in mode:put(c,EXC+4,2);c['watched'].append((R-4,4))
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out


def filter_cases():
 out=[]
 for code,count,magic,throw_info in [(0xc0000005,3,0x19930520,0),
  (0xe06d7363,3,0x19930520,0),(0xe06d7363,2,0x19930520,0),
  (0xe06d7363,3,0x19930521,0),(0xe06d7363,3,0x19930520,TABLE2)]:
  c=base(0x563e1c,f'{code:x}-{count}-{magic:x}-{throw_info:x}',[PTRS],result=0xffffffff)
  for at,value in [(EXC,code),(EXC+16,count),(EXC+20,magic),(EXC+28,throw_info)]:put(c,at,value)
  c['expected_eax']=int(code==0xe06d7363 and count==3 and magic==0x19930520 and throw_info==0)
  out.append(c)
 for cpp in [False,True]:
  c=base(0x563c7f,'cpp'if cpp else'foreign',[PTRS],result=None if cpp else 0xffffffff)
  if cpp:
   put(c,EXC,0xe06d7363);c.update(endpoints=[terminate()],expected_endpoint_counts={'terminate':1},noreturn_endpoint='terminate')
  else:c['expected_eax']=0
  out.append(c)
 return out


def terminate_cases():
 out=[]
 for mode in ['null','return','dispatch']:
  c=base(0x563844,mode,[]);put(c,0x604c24,0 if mode=='null'else CB0)
  eps=[terminate('terminate')];counts={'terminate':1}
  if mode!='null':
   e=endpoint('user-terminate',CB0);e.update(result_kind='void',fs0_registration=registration('terminate'))
   if mode=='dispatch':
    e['dispatch_fs0']=dispatch();eps.append(rtl());counts['RtlUnwind']=1
    c['expected_function_pointers']=[dict(address=NLG,native=0x563885,compiled_symbol='restore_crt_terminate_recover')]
   eps.insert(0,e);counts['user-terminate']=1
  c.update(endpoints=eps,expected_endpoint_counts=counts,noreturn_endpoint='terminate');out.append(c)
 return out



def terminate_unwind_case():
 c=base(0x563844,'unwind-finally',[])
 put(c,0x604c24,CB0);put(c,EXC+4,2)
 cb=endpoint('user-terminate',CB0)
 cb.update(result_kind='void',fs0_registration=registration('terminate'),dispatch_fs0=dispatch())
 end=terminate()
 end['fs0_registration']=registration('terminate',1)
 # The active local-unwind guard is a private stack object. Its parent,
 # table, state and final code token are checked instead of its address.
 c['watched'].remove((0,4))
 c.update(endpoints=[cb,end],expected_endpoint_counts={'user-terminate':1,'terminate':1},
  noreturn_endpoint='terminate',expected_function_pointers=[dict(address=NLG,native=0x563895,compiled_symbol='restore_crt_terminate_finally')])
 return [c]


def cleanup_cases():
 out=[]
 for mode in ['empty','two','mutate-local','mutate-table','cpp','forced-recovery']:
  c=base(0x563be1,mode,[FRAME,0,DESC,-1]);put(c,FRAME+8,-1 if mode=='empty'else 1)
  eps=[];counts={}
  for token,at in [('cleanup-one',CB1),('cleanup-zero',CB0)]:
   e=endpoint(token,at,0x13579bdf);e.update(registers=['eax','ecx','ebp'],
    observe=[(FRAME+8,4),(SP-32,4)],fs0_registration=registration('cleanup'))
   eps.append(e);counts[token]=0 if mode=='empty'else 1
  if mode=='mutate-local':eps[0]['mutations']=[(SP-32,4,123)]
  if mode=='mutate-table':
   eps[0]['mutations']=[(DESC+8,4,TABLE2)]
   for at,value in [(TABLE2,-1),(TABLE2+4,CB0),(TABLE2+8,0),(TABLE2+12,CB1)]:put(c,at,value)
  if mode in ['cpp','forced-recovery']:
   eps[0]['dispatch_fs0']=dispatch()
   if mode=='cpp':
    put(c,EXC,0xe06d7363);eps.append(terminate('cleanup'));counts.update(terminate=1);counts['cleanup-zero']=0;c['noreturn_endpoint']='terminate'
   else:
    forced=dict(token='controlled-filter',native=0x563c7f,compiled_symbols=['FUN_00563c7f'],
     arity=1,native_cleanup=0,compiled_cleanup=0,result_kind='full32',value=1,
     private_input_args={0:dict(role='EXCEPTION_POINTERS',width=8)})
    eps += [forced,rtl()];counts.update(**{'controlled-filter':1,'RtlUnwind':1})
  c.update(endpoints=eps,expected_endpoint_counts=counts);out.append(c)
 return out


def catch_cases():
 out=[]
 for mode in ['zero','nonzero','mutate-arguments','rethrow']:
  c=base(0x563d10,mode,[EXC,FRAME,CTX,DESC,CB0,2,0x100],result=0xffffffff)
  put(c,EXC,0xe06d7363)
  value=0 if mode=='zero'else 0x12345678
  cb=endpoint('catch-body',CB0,value);cb.update(registers=['eax','ecx','ebp'],
   observe=[(0x1266418,4),(0x126641c,4)],fs0_registration=registration('catch',1))
  release=dict(token='release',native=0x56400a,compiled_symbols=['FUN_0056400a'],
   arity=2,native_cleanup=0,compiled_cleanup=0,result_kind='void',
   observe=[(FRAME-4,4),(0x1266418,4),(0x126641c,4)])
  eps=[cb,release];counts={'catch-body':1,'release':int(value!=0)}
  if mode=='mutate-arguments':cb['mutations']=[(SP+4,4,EXC2),(SP+8,4,FRAME2)]
  if mode=='rethrow':
   cb['dispatch_fs0']=dispatch(EXC2);eps.append(rtl());counts.update(RtlUnwind=1,release=0);value=0
   c['expected_function_pointers']=[dict(address=NLG,native=0x563dce,compiled_symbol='restore_crt_catch_finally')]
  c.update(endpoints=eps,expected_endpoint_counts=counts,expected_eax=value);out.append(c)
 return out



def caller_cases():
 c=base(0x563c95,'nonlocal-result',[EXC,FRAME,CTX,0,DESC,TABLE2+64,0,TABLE2,2,0])
 put(c,FRAME-4,SP+0x1000);put(c,FRAME+8,-1)
 put(c,TABLE2,-1);put(c,TABLE2+4,0);put(c,TABLE2+76,CB0)
 cb=endpoint('catch-body',CB0,DEST)
 cb.update(registers=['eax','ecx','ebp'],fs0_registration=registration('catch',1))
 end=endpoint('continuation',DEST)
 end.update(result_kind='void',noreturn=True,registers=['eax','ebx','esp','ebp'])
 c.update(endpoints=[rtl(),cb,end],expected_endpoint_counts={'RtlUnwind':1,'catch-body':1,'continuation':1},
  noreturn_endpoint='continuation')
 return [c]


def definitions(registry):
 present={int(m['address'],16)for m in registry['members']}
 return [c for c in jump_cases()+handler_cases()+filter_cases()+terminate_cases()+terminate_unwind_case()+cleanup_cases()+catch_cases()+caller_cases() if c['entry']in present]
