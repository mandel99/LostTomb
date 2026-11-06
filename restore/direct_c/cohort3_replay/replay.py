"""Shared staged cohort replay. Only --run instantiates Unicorn; no host launch.

The initial case families cover byte lookups and96-bit carry/rounding. Coverage
of the entire cohort remains false until every selected entry has fresh cases.
"""
from pathlib import Path
import argparse,datetime,hashlib,json,struct,sys
sys.dont_write_bytecode=True
from registry import ROOT,LEDGER,digest,load,save,symbol
from cases import definitions
DATA_START,DATA_END=0x560000,0x126d000
STACK,STACK_SIZE,SP,STOP,SCRATCH=0x2ff00000,0x110000,0x30008000,0x31000000,0x32000000

def push_float32(uc,x,bits):
 """Controlled callback ST0 result; exact IEEE binary32 to x87 conversion."""
 assert 0<=bits<=0xffffffff
 fraction=bits&0x7fffff;exponent=(bits>>23)&255;sign=(bits>>16)&0x8000
 if exponent==255:mantissa=(1<<63)|(fraction<<40);encoded=0x7fff;tag=2
 elif exponent:mantissa=(0x800000|fraction)<<40;encoded=exponent-127+16383;tag=0
 elif fraction:
  leading=fraction.bit_length()-1;mantissa=fraction<<(63-leading);encoded=leading-149+16383;tag=0
 else:mantissa=encoded=0;tag=1
 status=uc.reg_read(x.UC_X86_REG_FPSW);tags=uc.reg_read(x.UC_X86_REG_FPTAG);top=(((status>>11)&7)-1)&7
 assert (tags>>(top*2))&3==3,'Controlled callback requires a free x87 return slot'
 uc.reg_write(x.UC_X86_REG_FP0+top,(mantissa,encoded|sign))
 uc.reg_write(x.UC_X86_REG_FPTAG,(tags&~(3<<(top*2)))|(tag<<(top*2)))
 uc.reg_write(x.UC_X86_REG_FPSW,(status&~0x3a00)|(top<<11))

def push_raw80(uc,x,payload):
 """Exact live x87 fixture; bytes are little-endian significand then sign/exponent."""
 raw=bytes.fromhex(payload);assert len(raw)==10
 mantissa=int.from_bytes(raw[:8],'little');exponent=int.from_bytes(raw[8:],'little')
 tag=1 if not(mantissa or exponent&0x7fff)else 0 if 0<exponent&0x7fff<0x7fff and mantissa>>63 else 2
 status=uc.reg_read(x.UC_X86_REG_FPSW);tags=uc.reg_read(x.UC_X86_REG_FPTAG);top=(((status>>11)&7)-1)&7
 assert (tags>>(top*2))&3==3,'Live input requires a free x87 slot'
 uc.reg_write(x.UC_X86_REG_FP0+top,(mantissa,exponent))
 uc.reg_write(x.UC_X86_REG_FPTAG,(tags&~(3<<(top*2)))|(tag<<(top*2)))
 uc.reg_write(x.UC_X86_REG_FPSW,(status&~0x3a00)|(top<<11))

def live_x87(uc,x):
 """Logical ST order, with empty physical register payloads excluded."""
 top=(uc.reg_read(x.UC_X86_REG_FPSW)>>11)&7;tags=uc.reg_read(x.UC_X86_REG_FPTAG);out=[]
 for i in range(8):
  physical=(top+i)&7;tag=(tags>>(physical*2))&3
  if tag==3:out.append(None)
  else:
   significand,exponent=uc.reg_read(x.UC_X86_REG_FP0+physical)
   out.append(dict(tag=tag,raw80=significand.to_bytes(8,'little').hex()+exponent.to_bytes(2,'little').hex()))
 return out

def executable_ranges(path,base):
 b=Path(path).read_bytes();nt=struct.unpack_from('<I',b,60)[0];n=struct.unpack_from('<H',b,nt+6)[0];table=nt+24+struct.unpack_from('<H',b,nt+20)[0];out=[]
 for i in range(n):
  p=table+i*40;size,rva=struct.unpack_from('<II',b,p+8)
  if struct.unpack_from('<I',b,p+36)[0]&0x20000000:out.append((base+rva,base+rva+size))
 return out

def import_slots(pe):
 b=pe.data;opt=struct.unpack_from('<I',b,60)[0]+24;table=struct.unpack_from('<I',b,opt+104)[0];out={}
 if not table:return out
 while any(b[table:table+20]):
  original,_,_,_,first=struct.unpack_from('<IIIII',b,table);lookup=original or first;i=0
  while (value:=struct.unpack_from('<I',b,lookup+i*4)[0]):
   if not value&0x80000000:
    name=bytes(b[value+2:b.index(0,value+2)]).decode('ascii');assert name not in out,name;out[name]=pe.base+first+i*4
   i+=1
  table+=20
 return out

def execute(registry,cases,directory):
 sys.path[:0]=[str(ROOT/'build/offline-verification-deps'),str(ROOT/'direct_c')]
 import unicorn
 from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID
 import unicorn.x86_const as x
 from native_test_pe import PE
 registers=[x.UC_X86_REG_EAX,x.UC_X86_REG_ECX,x.UC_X86_REG_EDX,x.UC_X86_REG_EBX,x.UC_X86_REG_ESP,x.UC_X86_REG_EBP,x.UC_X86_REG_ESI,x.UC_X86_REG_EDI]
 saved=[0x12345678,0x23456789,0x3456789a,0x456789ab];native=PE(ROOT/registry['native']['path']);seed=bytearray(DATA_END-DATA_START)
 raw=native.data[DATA_START-native.base:DATA_END-native.base];seed[:len(raw)]=raw
 machines={}
 for label,item in [('native',registry['native']),*registry['compiled_images'].items()]:
  pe=native if label=='native'else PE(ROOT/item['path']);u=Uc(UC_ARCH_X86,UC_MODE_32);size=(len(pe.data)+4095)&~4095
  u.mem_map(pe.base,size);u.mem_write(pe.base,bytes(pe.data));end=pe.base+size
  if DATA_START<pe.base:u.mem_map(DATA_START,pe.base-DATA_START)
  if DATA_END>end:u.mem_map(end,(DATA_END-end+4095)&~4095)
  u.mem_map(0,4096);u.mem_map(STACK,STACK_SIZE);u.mem_map(STOP,4096);u.mem_map(SCRATCH,0x50000)
  state=dict(active=False,label=label,map_text=''if label=='native'else(ROOT/item['map_path']).read_text(),imports=import_slots(pe),allowed=executable_ranges(ROOT/item['path'],pe.base),faults=[])
  def code(uc,address,size,state):
   if not state['active']:return
   if address in state['endpoints']:
    spec=state['endpoints'][address];visit=state['endpoint_visits'].get(spec['token'],0);state['endpoint_visits'][spec['token']]=visit+1
    if 'responses'in spec:
     assert visit<len(spec['responses']),('unexpected extra endpoint call',spec['token'])
     spec={**spec,**spec['responses'][visit]}
    physical_sp=uc.reg_read(x.UC_X86_REG_ESP);sp=physical_sp;model_frame=None
    def get(at,width=4):return int.from_bytes(uc.mem_read(at,width),'little')
    if spec.get('_compiled_frame_endpoint'):
     assert state['label']!='native' and spec['result_kind']=='void',('frame endpoint requires logical void',spec['token'])
     assert not any(spec.get(k)for k in ['native_register_args','compiled_register_args','compiled_receiver_arg']),('unsupported frame register-argument ABI',spec['token'])
     assert spec.get('native_arity',spec['arity'])==spec.get('compiled_arity',spec['arity'])==spec['arity'],('unsupported frame arity conversion',spec['token'])
     assert not any(spec.get(k)for k in ['fs0_registration','fs0_target_frame','dispatch_fs0','resume_fs_registration','noreturn']),('unsupported frame control endpoint',spec['token'])
     # Actual MSVC x86 cdecl argument is the live struct pointer. Header layout:
     # r[8] at0, flags at32, boundary at36, sizeof40; r[4] is native-model ESP.
     model_frame=get(physical_sp+4)
     assert model_frame%4==0 and STACK<=model_frame and model_frame+40<=STACK+STACK_SIZE,('live model frame',spec['token'])
     dispatcher=symbol(state['map_text'],spec['compiled_frame_dispatcher'])
     assert dispatcher and get(model_frame+36)==dispatcher['va'],('model dispatcher',spec['token'])
     sp=get(model_frame+16)
     assert sp%4==0 and STACK<=sp and sp+4+spec['arity']*4<=STACK+STACK_SIZE,('live model stack',spec['token'])
     assert get(sp)in spec['compiled_frame_return_sites'],('model return site',spec['token'],hex(get(sp)))
    def endpoint_register(name):
     if model_frame is None:return uc.reg_read(getattr(x,'UC_X86_REG_'+name.upper()))
     assert name.upper()in ['EAX','ECX','EDX','EBX','ESP','EBP','ESI','EDI']
     return get(model_frame+4*['EAX','ECX','EDX','EBX','ESP','EBP','ESI','EDI'].index(name.upper()))
    arity=spec.get('native_arity',spec['arity'])if state['label']=='native'else spec.get('compiled_arity',spec['arity'])
    raw=[get(sp+4+i*4)for i in range(arity)];logical=list(raw)
    roles={int(i):role for i,role in spec.get('pointer_args',{}).items()};private=[]
    for i,role in roles.items():
     assert 0<=i<len(raw)and STACK<=raw[i]<STACK+STACK_SIZE-3,('private output pointer',spec['token'],i,hex(raw[i]))
     private.append(raw[i]);logical[i]=['stack-output',role]
    for i,info in spec.get('private_input_args',{}).items():
     i=int(i)
     if info.get('nullable')and raw[i]==0:
      logical[i]=0;continue
     if 'cstring_max'in info:
      limit=info['cstring_max'];assert 0<limit<=512 and STACK<=raw[i]and raw[i]+limit<=STACK+STACK_SIZE
      data=bytes(uc.mem_read(raw[i],limit));assert b'\0'in data,('unterminated private input',spec['token'])
      logical[i]=['stack-cstring',info['role'],data[:data.index(0)+1].hex()];continue
     segments=info.get('segments')or[[0,info['width']]]
     assert i not in roles and segments and all(0<=off<=256 and 0<width<=256 and STACK<=raw[i]+off and raw[i]+off+width<=STACK+STACK_SIZE for off,width in segments),('private input pointer',spec['token'])
     payload=[bytes(uc.mem_read(raw[i]+off,width)).hex()for off,width in segments]
     logical[i]=['stack-input',info['role'],payload if 'segments'in info else payload[0]]
     if info.get('indirect_inputs'):
      indirect=[]
      for item in info['indirect_inputs']:
       offset,width=item['offset'],item['width']
       assert 0<=offset<=252 and 0<width<=256 and STACK<=raw[i]+offset and raw[i]+offset+4<=STACK+STACK_SIZE
       target=get(raw[i]+offset)
       assert STACK<=target and target+width<=STACK+STACK_SIZE,('private referenced object',spec['token'])
       if item.get('bound_role'):
        assert state['private_roles'].get(item['bound_role'])==target,('forwarded output object',spec['token'],item['bound_role'])
       indirect.append([item['role'],bytes(uc.mem_read(target,width)).hex()])
      logical[i]=['stack-input',info['role'],dict(segments=payload,indirect_inputs=indirect)]
    for i,role in spec.get('continuation_args',{}).items():
     i=int(i);assert raw[i]==get(sp),('actual return continuation',spec['token']);logical[i]=['return-continuation',role]
    for i,role in roles.items():
     if role in spec.get('bind_pointer_roles',[]):
      previous=state['private_roles'].setdefault(role,raw[i])
      assert previous==raw[i],('changed bound output object',spec['token'],role)
    assert len(private)==len(set(private)),('unexpected output alias',spec['token'])
    if state['label']=='native'and'native_receiver'in spec:
     assert uc.reg_read(x.UC_X86_REG_ECX)==spec['native_receiver'],('endpoint receiver',spec['token'])
    if state['label']!='native'and'compiled_receiver_arg'in spec:
     i=spec['compiled_receiver_arg'];assert raw[i]==spec['native_receiver'],('compiled receiver',spec['token']);logical.pop(i)
    if state['label']=='native':
     for i,name in sorted((int(i),name)for i,name in spec.get('native_register_args',{}).items()):
      assert name.upper()in['EAX','ECX','EDX','EBX','EBP','ESI','EDI']
      logical.insert(i,uc.reg_read(getattr(x,'UC_X86_REG_'+name.upper())))
    assert len(logical)==spec['arity'],('endpoint logical arity',spec['token'])
    event=dict(token=spec['token'],arguments=logical,observed=[[at,width,get(at,width)]for at,width in spec.get('observe',[])])
    if spec.get('registers'):event['registers']={name:endpoint_register(name)for name in spec['registers']}
    if spec.get('private_register_inputs'):
     event['private_register_inputs']={}
     for name,info in spec['private_register_inputs'].items():
      at=endpoint_register(name);width=info['width']
      assert 0<width<=256 and STACK<=at and at+width<=STACK+STACK_SIZE
      event['private_register_inputs'][name]=[info['role'],bytes(uc.mem_read(at,width)).hex()]
    if spec.get('fs0_registration'):
     record=spec['fs0_registration'];at=get(0)
     for _ in range(record.get('chain_depth',0)):at=get(at)
     assert STACK<=at and at+record['size']<=STACK+STACK_SIZE
     target=record['native_handler']if state['label']=='native'else symbol(state['map_text'],record['compiled_handler_symbol'])['va']
     assert get(at+4)==target,('active registration handler',spec['token'])
     event['registration']=dict(handler_token=hex(record['native_handler']),fields=[[off,width,get(at+off,width)]for off,width in record['fields']])
     if record.get('scope_rows'):
      table=get(at+8);scope_rows=[]
      for i,row in enumerate(record['scope_rows']):
       values=[get(table+i*12+offset)for offset in (0,4,8)]
       wanted=[row['enclosing']&0xffffffff]
       for role in ('filter','handler'):
        native_value=row['native_'+role]
        wanted.append(native_value if state['label']=='native' or not native_value else symbol(state['map_text'],row['compiled_'+role])['va'])
       assert values==wanted,('relocated EH3 scope',spec['token'],i)
       scope_rows.append([row['enclosing'],hex(row['native_filter']),hex(row['native_handler'])])
      event['registration']['scopes']=scope_rows
    state['events'].append(event)
    physical_event=dict(token=spec['token'],arguments=raw)
    if model_frame is not None:
     physical_event['live_model_frame']=dict(address=model_frame,stack_pointer=sp,return_site=get(sp),
      registers=[get(model_frame+i*4)for i in range(8)],flags=get(model_frame+32),dispatcher=get(model_frame+36))
     physical_event['physical_C_arguments']=[model_frame]
     physical_event['physical_C_stack_pointer']=physical_sp
    state['physical_endpoint_arguments'].append(physical_event)
    for i,width,value in spec.get('writes_args',[]):
     assert int(i)in roles and width in [1,2,4]
     uc.mem_write(raw[int(i)],(value&((1<<(8*width))-1)).to_bytes(width,'little'))
    for i,offset,payload in spec.get('writes_arg_bytes',[]):
     i=int(i);data=bytes.fromhex(payload);assert i in roles and 0<=offset<=256 and 0<len(data)<=256
     at=raw[i]+offset;assert STACK<=at and at+len(data)<=STACK+STACK_SIZE
     uc.mem_write(at,data)
    for at,width,value in spec.get('mutations',[]):
     assert width in [1,2,4]and any(lo<=at and at+width<=lo+n for lo,n in state['write_ranges'])
     uc.mem_write(at,(value&((1<<(8*width))-1)).to_bytes(width,'little'))
    kind=spec['result_kind']
    if kind=='full32':uc.reg_write(x.UC_X86_REG_EAX,spec['value']&0xffffffff)
    elif kind=='al8':uc.reg_write(x.UC_X86_REG_EAX,(uc.reg_read(x.UC_X86_REG_EAX)&0xffffff00)|(spec['value']&255))
    elif kind=='x87_float32':push_float32(uc,x,spec['value'])
    else:assert kind=='void','Unknown endpoint result contract'
    for key,reg in [('clobber_ecx',x.UC_X86_REG_ECX),('clobber_edx',x.UC_X86_REG_EDX),('clobber_ebp',x.UC_X86_REG_EBP)]:
     if key in spec:
      if model_frame is None:uc.reg_write(reg,spec[key]&0xffffffff)
      else:
       index={'clobber_ecx':1,'clobber_edx':2,'clobber_ebp':5}[key]
       uc.mem_write(model_frame+index*4,struct.pack('<I',spec[key]&0xffffffff))
    if spec.get('fs0_target_frame'):
     assert spec.get('import_name')=='RtlUnwind' and raw[0]!=0
     uc.mem_write(0,struct.pack('<I',raw[0]))
    if spec.get('dispatch_fs0'):
     dispatch=spec['dispatch_fs0'];record=get(0)
     wanted=dispatch['native_handler']if state['label']=='native'else symbol(state['map_text'],dispatch['compiled_handler'])['va']
     for _ in range(8):
      assert STACK<=record and record+16<=STACK+STACK_SIZE,('dispatch record',spec['token'])
      if get(record+4)==wanted:break
      record=get(record)
     else:raise AssertionError(('missing dispatch target',spec['token']))
     sentinel=STOP+0xff0
     # End the synthetic return block before Unicorn decodes past its page.
     uc.mem_write(sentinel,b'\xc3')
     state['endpoints'][sentinel]=dict(token='dispatch-return',arity=0,native_cleanup=0,compiled_cleanup=0,result_kind='void',noreturn=True)
     call_sp=sp-20
     uc.mem_write(call_sp,struct.pack('<5I',sentinel,dispatch['exception'],record,dispatch['context'],0))
     uc.reg_write(x.UC_X86_REG_ESP,call_sp)
     uc.reg_write(x.UC_X86_REG_EIP,wanted)
     return
    if spec.get('resume_fs_registration'):
     record=get(0)
     assert STACK<=record and record+40<=STACK+STACK_SIZE,('saved registration',spec['token'])
     target,saved_sp,saved_bp=(get(record+offset)for offset in (24,28,32))
     assert STACK<=saved_sp<STACK+STACK_SIZE and STACK<=saved_bp<STACK+STACK_SIZE
     uc.reg_write(x.UC_X86_REG_EBX,record)
     uc.reg_write(x.UC_X86_REG_ESP,saved_sp)
     uc.reg_write(x.UC_X86_REG_EBP,saved_bp)
     uc.reg_write(x.UC_X86_REG_EIP,target)
     return
    if spec.get('noreturn'):
     state['terminated']=spec['token'];uc.reg_write(x.UC_X86_REG_EIP,STOP);uc.emu_stop();return
    cleanup=spec['native_cleanup']if state['label']=='native'else spec['compiled_cleanup']
    assert cleanup>=0 and cleanup%4==0
    if model_frame is not None:
     # Model RET and physical cdecl RET are distinct. Preserve actual live r0.
     model_cleanup=spec['native_cleanup'];assert model_cleanup>=0 and model_cleanup%4==0
     uc.mem_write(model_frame+16,struct.pack('<I',sp+4+model_cleanup))
     uc.reg_write(x.UC_X86_REG_ESP,physical_sp+4);uc.reg_write(x.UC_X86_REG_EIP,get(physical_sp));return
    uc.reg_write(x.UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(x.UC_X86_REG_EIP,get(sp));return
   if state['active']and address!=STOP and not any(lo<=address<hi for lo,hi in state['allowed']):
    state['faults'].append(dict(kind='non-executable-or-unknown-target',address=hex(address)));uc.emu_stop()
  def write(uc,access,address,size,value,state):
   if state['active']and any(lo<=address and address+size<=lo+n for lo,n in state.get('write_trace_ranges',[])):
    state['write_trace'].append([address,size,value&((1<<(size*8))-1)])
   if state['active']and not(STACK<=address and address+size<=STACK+STACK_SIZE)and not any(lo<=address and address+size<=lo+n for lo,n in state['write_ranges']):
    state['faults'].append(dict(kind='unexpected-write',address=hex(address),size=size));uc.emu_stop()
  def read(uc,access,address,size,value,state):
   if state['active']and any(lo<=address and address+size<=lo+n for lo,n in state.get('read_ranges',[])):
    if state.get('read_skip_return_fetches')and bytes(uc.mem_read(uc.reg_read(x.UC_X86_REG_EIP),1))in(b'\xc2',b'\xc3'):
     state['return_reads'].append([address,size]);return
    state['reads'].append([address,size])
  def invalid(uc,access,address,size,value,state):
   if state['active']:
    state['faults'].append(dict(kind='invalid-memory',access=access,address=hex(address),size=size,eip=hex(uc.reg_read(x.UC_X86_REG_EIP)),esp=hex(uc.reg_read(x.UC_X86_REG_ESP))))
   return False
  u.hook_add(UC_HOOK_CODE,code,state);u.hook_add(UC_HOOK_MEM_WRITE,write,state);u.hook_add(UC_HOOK_MEM_READ,read,state);u.hook_add(UC_HOOK_MEM_INVALID,invalid,state);machines[label]=(u,state)
 sources={name:digest(ROOT/name)for name in json.loads(LEDGER.read_bytes())['source_hashes']}
 for folder in ['src','runtime','include']:
  for path in (ROOT/folder).rglob('*'):
   if path.suffix in ['.c','.h','.inc']:sources[path.relative_to(ROOT).as_posix()]=digest(path)
 for path in Path(__file__).parent.glob('*.py'):sources[path.relative_to(ROOT).as_posix()]=digest(path)
 binding=dict(schema='cohort3-staged-replay-binding-v1',registry=registry,source_hashes=sources,unicorn_version=unicorn.__version__,no_host_game=True)
 save(directory/'binding.json',binding)
 save(directory/'case-inputs.json',[{k:([[a,v.hex()]for a,v in value]if k=='stores'else value)for k,value in c.items()}for c in cases])
 members={int(m['address'],16):m for m in registry['members']};rows=[];attempts=0
 for case in cases:
  m=members[case['entry']];reference=None;reference_regs=None;reference_context=None;reference_writes=None;reference_x87=None;reference_live_x87=None;reference_reads=None;routes=[];failures=[]
  preserved=[case.get('initial_'+name,value)for name,value in zip(['ebx','ebp','esi','edi'],saved)]
  for label,(u,state)in machines.items():
   if label=='native':selected=[('native',case['entry'],case['arguments'])]
   else:
    provider=m['providers'][label];selected=[]
    if provider['named']:selected.append(('named',provider['named']['va'],case['named_arguments']))
    canonical=provider['canonical']
    if canonical and (Path(m['source']).stem+'.obj'in canonical['owner']or case.get('canonical_owner')and case['canonical_owner']in canonical['owner']):
     selected.append(('canonical',canonical['va'],case.get('canonical_arguments',case['arguments'])))
   if 'routes'in case:
    requested=set(case['routes'])
    assert {'native','named'}<=requested<= {'native','named','canonical'},('invalid API route scope',case['id'])
    assert case.get('routes_reason'),('missing API route scope reason',case['id'])
    selected=[item for item in selected if item[0]in requested]
   for route,pc,args in selected:
    state.update(active=False,faults=[],private_roles={},write_ranges=case['write_ranges'],write_trace=[],write_trace_ranges=case.get('write_trace_ranges',[]),events=[],physical_endpoint_arguments=[],endpoints={},endpoint_visits={},terminated=None,reads=[],return_reads=[],read_ranges=case.get('read_ranges',[]),read_skip_return_fetches=case.get('read_skip_return_fetches',False));errors=[]
    import_fixups=[]
    for spec in case.get('endpoints',[]):
     assert spec['result_kind']in ['void','al8','full32','x87_float32']and 0<=spec['arity']<=16 and 0<=spec.get('compiled_arity',spec['arity'])<=16 and 0<=spec.get('native_arity',spec['arity'])<=16
     if spec['result_kind']!='void':assert 'value'in spec,('missing endpoint result',spec['token'])
     if 'import_name'in spec:
      pcs=[STOP+0x100+len(import_fixups)*16];import_fixups.append((state['imports'][spec['import_name']],pcs[0]))
     elif 'controlled_address'in spec:
      at=spec['controlled_address'];assert STOP<at<STOP+4096;pcs=[at]
     elif label=='native':pcs=[spec['native']]
     else:
      pcs=[value['va']for name in spec['compiled_symbols']if(value:=symbol(state['map_text'],name))]
      assert pcs,('missing compiled endpoint',label,spec['token'])
     for at in pcs:
      assert at not in state['endpoints']or state['endpoints'][at]['token']==spec['token']
      state['endpoints'][at]=spec
     if label!='native':
      for name in spec.get('compiled_frame_symbols',[]):
       value=symbol(state['map_text'],name)
       assert value and spec['result_kind']=='void',('missing/nonvoid compiled frame endpoint',label,spec['token'],name)
       assert spec.get('compiled_frame_dispatcher')and spec.get('compiled_frame_return_sites'),('unbound frame layout/site',spec['token'])
       at=value['va'];assert at not in state['endpoints'],('duplicate frame endpoint',name)
       state['endpoints'][at]={**spec,'_compiled_frame_endpoint':True}
    u.mem_write(DATA_START,bytes(seed));u.mem_write(SCRATCH,b'\xa5'*0x50000);u.mem_write(SP-0x2000,b'\xcc'*0x4000);u.mem_write(0,b'\xcc'*4096)
    if 'initial_fs0'in case:
     assert (0,4)in case['write_ranges']or[0,4]in case['write_ranges']
     u.mem_write(0,struct.pack('<I',case['initial_fs0']))
    for at,value in case['stores']:u.mem_write(at,value)
    fixed=list(import_fixups);expected_fixed=list(import_fixups)
    for at,value in import_fixups:u.mem_write(at,struct.pack('<I',value))
    for fixup in case.get('function_pointers',[]):
     value=fixup['native']if label=='native'else symbol(state['map_text'],fixup['compiled_symbol'])['va']
     assert not any(at<fixup['address']+4 and fixup['address']<at+n for at,n in case['watched']),('physical relocation watched',fixup)
     u.mem_write(fixup['address'],struct.pack('<I',value));fixed.append((fixup['address'],value))
     final_value=fixup.get('expected_final_value',value)
     assert isinstance(final_value,int)and 0<=final_value<=0xffffffff,('invalid final pointer value',fixup)
     expected_fixed.append((fixup['address'],final_value))
    machine_sp=case.get('machine_sp',{}).get(route,SP)
    assert STACK+0x2000<=machine_sp and machine_sp+0x2000<=STACK+STACK_SIZE
    if machine_sp!=SP:u.mem_write(machine_sp-0x2000,b'\xcc'*0x4000)
    initial=[case.get('initial_eax',0xa5b6c7d8),case['receiver'],case.get('initial_edx',0x33333333),preserved[0],machine_sp,*preserved[1:]]
    for reg,value in zip(registers,initial):u.reg_write(reg,value)
    for reg,value in [(x.UC_X86_REG_EFLAGS,case['eflags']),(x.UC_X86_REG_FPCW,case.get('initial_fpcw',0x37f)),(x.UC_X86_REG_FPSW,0),(x.UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    for payload in reversed(case.get('initial_x87',[])):push_raw80(u,x,payload)
    if 'initial_fpsw'in case:
     u.reg_write(x.UC_X86_REG_FPSW,(u.reg_read(x.UC_X86_REG_FPSW)&0x3800)|(case['initial_fpsw']&~0x3800))
    prefix=case.get('stack_prefix',{}).get(route,[]);assert len(prefix)<=8
    if route=='named'and case.get('borrowed_native_stack'):
     assert machine_sp<SP-0x10000
     u.mem_write(SP,struct.pack('<I',STOP))
     for i,value in enumerate(case['arguments']):u.mem_write(SP+4+i*4,struct.pack('<I',value&0xffffffff))
    for i,value in enumerate(prefix):u.mem_write(machine_sp+i*4,struct.pack('<I',value&0xffffffff))
    return_slot=machine_sp+len(prefix)*4;u.mem_write(return_slot,struct.pack('<I',STOP))
    for i,value in enumerate(args):u.mem_write(return_slot+4+i*4,struct.pack('<I',value&0xffffffff))
    for at,payload in case.get('route_stores',{}).get(route,[]):u.mem_write(at,bytes.fromhex(payload))
    watched=case.get('route_watched',{}).get(route,case['watched'])
    assert [n for at,n in watched]==[n for at,n in case['watched']],'ABI logical views must have identical widths'
    attempts+=1;state['active']=True
    try:u.emu_start(pc,STOP,count=case.get('budget',200000))
    except Exception as exc:errors.append(dict(kind='execution-error',detail=repr(exc)))
    finally:state['active']=False
    values=[u.reg_read(r)for r in registers];flags=u.reg_read(x.UC_X86_REG_EFLAGS);ram=[bytes(u.mem_read(a,n))for a,n in watched]
    result_register=case.get('result_registers',{}).get(route,'eax').upper();assert result_register in ['EAX','ECX','EDX']
    result=u.reg_read(getattr(x,'UC_X86_REG_'+result_register))
    result_memory=case.get('result_memory',{}).get(route)
    if result_memory:
     at,width=result_memory;assert width in[1,2,4];result=int.from_bytes(u.mem_read(at,width),'little')
    for at,payload in case.get('authored_final_memory',[]):
     wanted=bytes.fromhex(payload);actual=bytes(u.mem_read(at,len(wanted)))
     if actual!=wanted:errors.append(dict(kind='authored-final-memory',address=hex(at),expected=wanted.hex(),actual=actual.hex()))
    context=None
    if case.get('context_outputs'):
     values_by_name=dict(zip(['eax','ecx','edx','ebx','esp','ebp','esi','edi'],values));values_by_name['eflags']=flags
     locations=case['context_outputs'].get(route,{})
     context={name:int.from_bytes(u.mem_read(*locations[name]),'little')if name in locations else values_by_name[name]for name in case['context_compare']}
     if 'eflags'in context:context['eflags']&=case.get('context_flag_mask',0xffffffff)
    if route in case.get('expected_route_results',{})and result!=case['expected_route_results'][route]:
     errors.append(dict(kind='authored-route-result',register=result_register,actual=hex(result),expected=hex(case['expected_route_results'][route])))
    fp=[u.reg_read(r)for r in [x.UC_X86_REG_FPCW,x.UC_X86_REG_FPSW,x.UC_X86_REG_FPTAG]]
    fp_live=live_x87(u,x)
    if u.reg_read(x.UC_X86_REG_EIP)!=STOP or state['faults']:errors.append(dict(kind='return/fault',faults=state['faults']))
    if case.get('noreturn_endpoint'):
     if state['terminated']!=case['noreturn_endpoint']:errors.append(dict(kind='missing-noreturn-boundary'))
    else:
     if state['terminated']:errors.append(dict(kind='unexpected-noreturn-boundary'))
     if values[4]!=machine_sp+4+case[route+'_cleanup']:errors.append(dict(kind='stack-cleanup',actual=hex(values[4])))
     if [values[i]for i in [3,5,6,7]]!=preserved:errors.append(dict(kind='nonvolatile-registers'))
     if 'initial_fs0'in case and int.from_bytes(u.mem_read(0,4),'little')!=case['initial_fs0']:errors.append(dict(kind='exception-chain-not-restored'))
    if 'expected_flags'in case:
     mask,wanted=case['expected_flags']
     if flags&mask!=wanted:errors.append(dict(kind='authored-flags',actual=hex(flags),mask=hex(mask),expected=hex(wanted)))
    for at,value in expected_fixed:
     if int.from_bytes(u.mem_read(at,4),'little')!=value:errors.append(dict(kind='function-pointer-slot-modified',address=hex(at)))
    for fixup in case.get('expected_function_pointers',[]):
     value=fixup['native']if label=='native'else symbol(state['map_text'],fixup['compiled_symbol'])['va']
     assert not any(at<fixup['address']+4 and fixup['address']<at+n for at,n in case['watched'])
     if int.from_bytes(u.mem_read(fixup['address'],4),'little')!=value:errors.append(dict(kind='expected-function-pointer',address=hex(fixup['address'])))
    if route=='native':
     reference=(None if case['result_mask']is None else result&case['result_mask'],ram,state['events']);reference_regs=(values,flags);reference_context=context;reference_writes=list(state['write_trace']);reference_x87=fp;reference_live_x87=fp_live;reference_reads=list(state['reads'])
     if 'expected_eax'in case and values[0]!=case['expected_eax']:errors.append(dict(kind='native-authored-result',actual=hex(values[0])))
     if 'expected_result'in case and result!=case['expected_result']:errors.append(dict(kind='native-authored-logical-result',actual=hex(result)))
     if 'expected_masked_result'in case and result&case['result_mask']!=case['expected_masked_result']:errors.append(dict(kind='native-authored-masked-result',actual=hex(result&case['result_mask'])))
     if 'expected_reads'in case and state['reads']!=case['expected_reads']:errors.append(dict(kind='native-authored-probe-reads'))
     if 'expected_write_trace'in case and state['write_trace']!=case['expected_write_trace']:errors.append(dict(kind='native-authored-write-sequence'))
     counts={token:sum(e['token']==token for e in state['events'])for token in case.get('expected_endpoint_counts',{})}
     if counts!=case.get('expected_endpoint_counts',{}):errors.append(dict(kind='native-authored-endpoint-counts',actual=counts,expected=case['expected_endpoint_counts']))
     for token,expected in case.get('expected_endpoint_arguments',{}).items():
      actual=[e['arguments']for e in state['events']if e['token']==token]
      if actual!=expected:errors.append(dict(kind='native-authored-endpoint-arguments',token=token))
    else:
     if reference[:2]!=(None if case['result_mask']is None else result&case['result_mask'],ram):
      differences=[]
      for (base,n),a,b in zip(watched,reference[1],ram):
       if a!=b:
        i=next(i for i,(aa,bb)in enumerate(zip(a,b))if aa!=bb);differences.append(dict(address=hex(base+i),native=a[i],compiled=b[i]))
      errors.append(dict(kind='native-C-result-or-memory',native_result=reference[0],compiled_result=result,memory_differences=differences))
     if reference[2]!=state['events']:errors.append(dict(kind='native-C-endpoint-sequence'))
     if reference_writes!=state['write_trace']:errors.append(dict(kind='native-C-write-sequence',native=reference_writes,compiled=state['write_trace']))
     if case.get('compare_x87')and reference_x87!=fp:errors.append(dict(kind='native-C-x87-state',native=reference_x87,compiled=fp))
     if case.get('compare_x87_values')and reference_live_x87!=fp_live:errors.append(dict(kind='native-C-x87-values',native=reference_live_x87,compiled=fp_live))
     if case.get('compare_x87_control',case.get('compare_x87_values',False))and reference_x87[0]!=fp[0]:errors.append(dict(kind='native-C-x87-control',native=reference_x87[0],compiled=fp[0]))
     if case.get('read_ranges')and reference_reads!=state['reads']:errors.append(dict(kind='native-C-probe-reads',native=reference_reads,compiled=state['reads']))
     if context!=reference_context:errors.append(dict(kind='native-C-context-outputs',native=reference_context,compiled=context))
     if route=='canonical'and case['canonical_registers_and_flags']and(reference_regs[0]!=values or(reference_regs[1]^flags)&case.get('canonical_flag_mask',0xffffffff)):
      errors.append(dict(kind='canonical-registers-or-flags'))
    row=dict(image=label,route=route,eax=hex(values[0]),logical_result=dict(register=result_register,memory=result_memory,value=hex(result)),context_outputs=context,eflags=hex(flags),x87=fp,x87_values=fp_live if case.get('compare_x87_values')else None,probe_reads=list(state['reads']),write_trace=list(state['write_trace']),control_return_reads=list(state['return_reads']),termination=state['terminated'],physical_argument_words=[int.from_bytes(u.mem_read(return_slot+4+i*4,4),'little')for i in range(len(args))],events=list(state['events']),physical_endpoint_arguments=list(state['physical_endpoint_arguments']),physical_function_pointers=fixed,errors=errors)
    routes.append(row);failures+=errors
  assert any(r['image']=='retained'and r['route']=='named'for r in routes),m['name']
  rows.append(dict(id=case['id'],entry=hex(case['entry']),routes=routes,passed=not failures))
  if len(rows)%8==0:print('Fresh staged cases:',len(rows),'/',len(cases),flush=True)
 integrity=[]
 try:
  if load(True)!=registry:integrity.append('Input/artifact registry changed during execution')
 except Exception as exc:integrity.append(repr(exc))
 for name,sha in sources.items():
  if digest(ROOT/name)!=sha:integrity.append('source changed during replay: '+name)
 compared=sorted({hex(c['entry'])for c in cases});pending=[m['address']for m in registry['members']if hex(int(m['address'],16))not in compared]
 report=dict(schema='cohort3-staged-native-C-comparison-v1',source_entries=registry['source_entries'],entries_compared=len(compared),cases=len(rows),invocations=attempts,passed_cases=sum(r['passed']for r in rows),failed_cases=sum(not r['passed']for r in rows),compared_entries=compared,pending_entries=pending,cases_detail=rows,binding_sha256=digest(directory/'binding.json'),case_inputs_sha256=digest(directory/'case-inputs.json'),cohort_validation_complete=False,whole_game_equivalence=False,host_execution=False,scope='Fresh authored cases for the selected families; actual helpers execute unless endpoints are declared. Pending entries and unvisited domains remain explicit.')
 report.update(inputs_stable=not integrity,input_integrity_errors=integrity)
 save(directory/'result.json',report);return report

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--run',action='store_true');p.add_argument('--group',action='append',default=[]);p.add_argument('--case-id',action='append',default=[]);p.add_argument('--output-dir',type=Path,default=ROOT/'build/actor-animal-following/cohort3-verification/runs');a=p.parse_args()
 registry=load(a.run);cases=definitions(registry)
 if a.group:cases=[c for c in cases if c['id'].split('/')[0]in a.group]
 if a.case_id:cases=[c for c in cases if c['id']in a.case_id]
 assert cases and len({c['id']for c in cases})==len(cases),'Empty or duplicate case selection'
 if not a.run:print(json.dumps(dict(source_entries=registry['source_entries'],planned_cases=len(cases),planned_entries=len({c['entry']for c in cases}),execution_performed=False)));return
 directory=a.output_dir/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f');directory.mkdir(parents=True,exist_ok=False)
 r=execute(registry,cases,directory);print(json.dumps(dict(source_entries=r['source_entries'],entries_compared=r['entries_compared'],cases=r['cases'],invocations=r['invocations'],failed_cases=r['failed_cases'],inputs_stable=r['inputs_stable'],cohort_validation_complete=False,result=str(directory/'result.json'))));sys.exit(bool(r['failed_cases']or not r['inputs_stable']))

if __name__=='__main__':main()
