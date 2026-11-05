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
 from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
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
    spec=state['endpoints'][address];sp=uc.reg_read(x.UC_X86_REG_ESP)
    def get(at,width=4):return int.from_bytes(uc.mem_read(at,width),'little')
    arity=spec['arity']if state['label']=='native'else spec.get('compiled_arity',spec['arity'])
    raw=[get(sp+4+i*4)for i in range(arity)];logical=list(raw)
    roles={int(i):role for i,role in spec.get('pointer_args',{}).items()};private=[]
    for i,role in roles.items():
     assert 0<=i<len(raw)and STACK<=raw[i]<STACK+STACK_SIZE-3,('private output pointer',spec['token'],i,hex(raw[i]))
     private.append(raw[i]);logical[i]=['stack-output',role]
    for i,info in spec.get('private_input_args',{}).items():
     i=int(i);width=info['width'];assert i not in roles and 0<width<=256 and STACK<=raw[i]and raw[i]+width<=STACK+STACK_SIZE,('private input pointer',spec['token'])
     logical[i]=['stack-input',info['role'],bytes(uc.mem_read(raw[i],width)).hex()]
    for i,role in spec.get('continuation_args',{}).items():
     i=int(i);assert raw[i]==get(sp),('actual return continuation',spec['token']);logical[i]=['return-continuation',role]
    assert len(private)==len(set(private)),('unexpected output alias',spec['token'])
    if state['label']=='native'and'native_receiver'in spec:
     assert uc.reg_read(x.UC_X86_REG_ECX)==spec['native_receiver'],('endpoint receiver',spec['token'])
    if state['label']!='native'and'compiled_receiver_arg'in spec:
     i=spec['compiled_receiver_arg'];assert raw[i]==spec['native_receiver'],('compiled receiver',spec['token']);logical.pop(i)
    assert len(logical)==spec['arity'],('endpoint logical arity',spec['token'])
    event=dict(token=spec['token'],arguments=logical,observed=[[at,width,get(at,width)]for at,width in spec.get('observe',[])])
    if spec.get('registers'):event['registers']={name:uc.reg_read(getattr(x,'UC_X86_REG_'+name.upper()))for name in spec['registers']}
    if spec.get('fs0_registration'):
     record=spec['fs0_registration'];at=get(0);assert STACK<=at and at+record['size']<=STACK+STACK_SIZE
     target=record['native_handler']if state['label']=='native'else symbol(state['map_text'],record['compiled_handler_symbol'])['va']
     assert get(at+4)==target,('active registration handler',spec['token'])
     event['registration']=dict(handler_token=hex(record['native_handler']),fields=[[off,width,get(at+off,width)]for off,width in record['fields']])
    state['events'].append(event)
    state['physical_endpoint_arguments'].append(dict(token=spec['token'],arguments=raw))
    for i,width,value in spec.get('writes_args',[]):
     assert int(i)in roles and width in [1,2,4]
     uc.mem_write(raw[int(i)],(value&((1<<(8*width))-1)).to_bytes(width,'little'))
    for at,width,value in spec.get('mutations',[]):
     assert width in [1,2,4]and any(lo<=at and at+width<=lo+n for lo,n in state['write_ranges'])
     uc.mem_write(at,(value&((1<<(8*width))-1)).to_bytes(width,'little'))
    kind=spec['result_kind']
    if kind=='full32':uc.reg_write(x.UC_X86_REG_EAX,spec['value']&0xffffffff)
    elif kind=='al8':uc.reg_write(x.UC_X86_REG_EAX,(uc.reg_read(x.UC_X86_REG_EAX)&0xffffff00)|(spec['value']&255))
    elif kind=='x87_float32':push_float32(uc,x,spec['value'])
    else:assert kind=='void','Unknown endpoint result contract'
    for key,reg in [('clobber_ecx',x.UC_X86_REG_ECX),('clobber_edx',x.UC_X86_REG_EDX),('clobber_ebp',x.UC_X86_REG_EBP)]:
     if key in spec:uc.reg_write(reg,spec[key]&0xffffffff)
    if spec.get('noreturn'):
     state['terminated']=spec['token'];uc.reg_write(x.UC_X86_REG_EIP,STOP);uc.emu_stop();return
    cleanup=spec['native_cleanup']if state['label']=='native'else spec['compiled_cleanup']
    assert cleanup>=0 and cleanup%4==0
    uc.reg_write(x.UC_X86_REG_ESP,sp+4+cleanup);uc.reg_write(x.UC_X86_REG_EIP,get(sp));return
   if state['active']and address!=STOP and not any(lo<=address<hi for lo,hi in state['allowed']):
    state['faults'].append(dict(kind='non-executable-or-unknown-target',address=hex(address)));uc.emu_stop()
  def write(uc,access,address,size,value,state):
   if state['active']and not(STACK<=address and address+size<=STACK+STACK_SIZE)and not any(lo<=address and address+size<=lo+n for lo,n in state['write_ranges']):
    state['faults'].append(dict(kind='unexpected-write',address=hex(address),size=size));uc.emu_stop()
  u.hook_add(UC_HOOK_CODE,code,state);u.hook_add(UC_HOOK_MEM_WRITE,write,state);machines[label]=(u,state)
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
  m=members[case['entry']];reference=None;reference_regs=None;reference_x87=None;routes=[];failures=[]
  preserved=list(saved);preserved[1]=case.get('initial_ebp',preserved[1])
  for label,(u,state)in machines.items():
   if label=='native':selected=[('native',case['entry'],case['arguments'])]
   else:
    provider=m['providers'][label];selected=[]
    if provider['named']:selected.append(('named',provider['named']['va'],case['named_arguments']))
    canonical=provider['canonical']
    if canonical and (Path(m['source']).stem+'.obj'in canonical['owner']or case.get('canonical_owner')and case['canonical_owner']in canonical['owner']):
     selected.append(('canonical',canonical['va'],case['arguments']))
   for route,pc,args in selected:
    state.update(active=False,faults=[],write_ranges=case['write_ranges'],events=[],physical_endpoint_arguments=[],endpoints={},terminated=None);errors=[]
    import_fixups=[]
    for spec in case.get('endpoints',[]):
     assert spec['result_kind']in ['void','al8','full32','x87_float32']and 0<=spec['arity']<=16 and 0<=spec.get('compiled_arity',spec['arity'])<=16
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
    u.mem_write(DATA_START,bytes(seed));u.mem_write(SCRATCH,b'\xa5'*0x50000);u.mem_write(SP-0x2000,b'\xcc'*0x4000);u.mem_write(0,b'\xcc'*4096)
    if 'initial_fs0'in case:
     assert (0,4)in case['write_ranges']or[0,4]in case['write_ranges']
     u.mem_write(0,struct.pack('<I',case['initial_fs0']))
    for at,value in case['stores']:u.mem_write(at,value)
    fixed=list(import_fixups)
    for at,value in import_fixups:u.mem_write(at,struct.pack('<I',value))
    for fixup in case.get('function_pointers',[]):
     value=fixup['native']if label=='native'else symbol(state['map_text'],fixup['compiled_symbol'])['va']
     assert not any(at<fixup['address']+4 and fixup['address']<at+n for at,n in case['watched']),('physical relocation watched',fixup)
     u.mem_write(fixup['address'],struct.pack('<I',value));fixed.append((fixup['address'],value))
    initial=[case.get('initial_eax',0xa5b6c7d8),case['receiver'],0x33333333,preserved[0],SP,*preserved[1:]]
    for reg,value in zip(registers,initial):u.reg_write(reg,value)
    for reg,value in [(x.UC_X86_REG_EFLAGS,case['eflags']),(x.UC_X86_REG_FPCW,case.get('initial_fpcw',0x37f)),(x.UC_X86_REG_FPSW,0),(x.UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    u.mem_write(SP,struct.pack('<I',STOP))
    for i,value in enumerate(args):u.mem_write(SP+4+i*4,struct.pack('<I',value&0xffffffff))
    attempts+=1;state['active']=True
    try:u.emu_start(pc,STOP,count=case.get('budget',200000))
    except Exception as exc:errors.append(dict(kind='execution-error',detail=repr(exc)))
    finally:state['active']=False
    values=[u.reg_read(r)for r in registers];flags=u.reg_read(x.UC_X86_REG_EFLAGS);ram=[bytes(u.mem_read(a,n))for a,n in case['watched']]
    fp=[u.reg_read(r)for r in [x.UC_X86_REG_FPCW,x.UC_X86_REG_FPSW,x.UC_X86_REG_FPTAG]]
    if u.reg_read(x.UC_X86_REG_EIP)!=STOP or state['faults']:errors.append(dict(kind='return/fault',faults=state['faults']))
    if case.get('noreturn_endpoint'):
     if state['terminated']!=case['noreturn_endpoint']:errors.append(dict(kind='missing-noreturn-boundary'))
    else:
     if state['terminated']:errors.append(dict(kind='unexpected-noreturn-boundary'))
     if values[4]!=SP+4+case[route+'_cleanup']:errors.append(dict(kind='stack-cleanup',actual=hex(values[4])))
     if [values[i]for i in [3,5,6,7]]!=preserved:errors.append(dict(kind='nonvolatile-registers'))
     if 'initial_fs0'in case and int.from_bytes(u.mem_read(0,4),'little')!=case['initial_fs0']:errors.append(dict(kind='exception-chain-not-restored'))
    if 'expected_flags'in case:
     mask,wanted=case['expected_flags']
     if flags&mask!=wanted:errors.append(dict(kind='authored-flags',actual=hex(flags),mask=hex(mask),expected=hex(wanted)))
    for at,value in fixed:
     if int.from_bytes(u.mem_read(at,4),'little')!=value:errors.append(dict(kind='function-pointer-slot-modified',address=hex(at)))
    for fixup in case.get('expected_function_pointers',[]):
     value=fixup['native']if label=='native'else symbol(state['map_text'],fixup['compiled_symbol'])['va']
     assert not any(at<fixup['address']+4 and fixup['address']<at+n for at,n in case['watched'])
     if int.from_bytes(u.mem_read(fixup['address'],4),'little')!=value:errors.append(dict(kind='expected-function-pointer',address=hex(fixup['address'])))
    if route=='native':
     reference=(None if case['result_mask']is None else values[0]&case['result_mask'],ram,state['events']);reference_regs=(values,flags);reference_x87=fp
     if 'expected_eax'in case and values[0]!=case['expected_eax']:errors.append(dict(kind='native-authored-result',actual=hex(values[0])))
     counts={token:sum(e['token']==token for e in state['events'])for token in case.get('expected_endpoint_counts',{})}
     if counts!=case.get('expected_endpoint_counts',{}):errors.append(dict(kind='native-authored-endpoint-counts',actual=counts,expected=case['expected_endpoint_counts']))
    else:
     if reference[:2]!=(None if case['result_mask']is None else values[0]&case['result_mask'],ram):
      differences=[]
      for (base,n),a,b in zip(case['watched'],reference[1],ram):
       if a!=b:
        i=next(i for i,(aa,bb)in enumerate(zip(a,b))if aa!=bb);differences.append(dict(address=hex(base+i),native=a[i],compiled=b[i]))
      errors.append(dict(kind='native-C-result-or-memory',native_eax=reference[0],compiled_eax=values[0],memory_differences=differences))
     if reference[2]!=state['events']:errors.append(dict(kind='native-C-endpoint-sequence'))
     if case.get('compare_x87')and reference_x87!=fp:errors.append(dict(kind='native-C-x87-state',native=reference_x87,compiled=fp))
     if route=='canonical'and case['canonical_registers_and_flags']and reference_regs!=(values,flags):errors.append(dict(kind='canonical-registers-or-flags'))
    row=dict(image=label,route=route,eax=hex(values[0]),eflags=hex(flags),x87=fp,termination=state['terminated'],physical_argument_words=[int.from_bytes(u.mem_read(SP+4+i*4,4),'little')for i in range(len(args))],events=list(state['events']),physical_endpoint_arguments=list(state['physical_endpoint_arguments']),physical_function_pointers=fixed,errors=errors)
    routes.append(row);failures+=errors
  assert any(r['image']=='retained'and r['route']=='named'for r in routes),m['name']
  rows.append(dict(id=case['id'],entry=hex(case['entry']),routes=routes,passed=not failures))
  if len(rows)%8==0:print('Fresh staged cases:',len(rows),'/',len(cases),flush=True)
 assert load(True)==registry,'Input/artifact registry changed during execution'
 for name,sha in sources.items():assert digest(ROOT/name)==sha,('source changed during replay',name)
 compared=sorted({hex(c['entry'])for c in cases});pending=[m['address']for m in registry['members']if hex(int(m['address'],16))not in compared]
 report=dict(schema='cohort3-staged-native-C-comparison-v1',source_entries=registry['source_entries'],entries_compared=len(compared),cases=len(rows),invocations=attempts,passed_cases=sum(r['passed']for r in rows),failed_cases=sum(not r['passed']for r in rows),compared_entries=compared,pending_entries=pending,cases_detail=rows,binding_sha256=digest(directory/'binding.json'),case_inputs_sha256=digest(directory/'case-inputs.json'),cohort_validation_complete=False,whole_game_equivalence=False,host_execution=False,scope='Fresh authored cases for the selected families; actual helpers execute unless endpoints are declared. Pending entries and unvisited domains remain explicit.')
 save(directory/'result.json',report);return report

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--run',action='store_true');p.add_argument('--group',action='append',default=[]);p.add_argument('--case-id',action='append',default=[]);p.add_argument('--output-dir',type=Path,default=ROOT/'build/actor-animal-following/cohort3-verification/runs');a=p.parse_args()
 registry=load(a.run);cases=definitions(registry)
 if a.group:cases=[c for c in cases if c['id'].split('/')[0]in a.group]
 if a.case_id:cases=[c for c in cases if c['id']in a.case_id]
 assert cases and len({c['id']for c in cases})==len(cases),'Empty or duplicate case selection'
 if not a.run:print(json.dumps(dict(source_entries=registry['source_entries'],planned_cases=len(cases),planned_entries=len({c['entry']for c in cases}),execution_performed=False)));return
 directory=a.output_dir/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f');directory.mkdir(parents=True,exist_ok=False)
 r=execute(registry,cases,directory);print(json.dumps(dict(source_entries=r['source_entries'],entries_compared=r['entries_compared'],cases=r['cases'],invocations=r['invocations'],failed_cases=r['failed_cases'],cohort_validation_complete=False,result=str(directory/'result.json'))));sys.exit(bool(r['failed_cases']))

if __name__=='__main__':main()
