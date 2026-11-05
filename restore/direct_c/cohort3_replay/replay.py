"""Shared staged cohort replay. Only --run instantiates Unicorn; no host launch.

The initial case families cover byte lookups and96-bit carry/rounding. Coverage
of the entire cohort remains false until every selected entry has fresh cases.
"""
from pathlib import Path
import argparse,datetime,hashlib,json,struct,sys
sys.dont_write_bytecode=True
from registry import ROOT,LEDGER,digest,load,save
from cases import definitions
DATA_START,DATA_END=0x560000,0x126d000
STACK,STACK_SIZE,SP,STOP,SCRATCH=0x2ff00000,0x110000,0x30008000,0x31000000,0x32000000

def executable_ranges(path,base):
 b=Path(path).read_bytes();nt=struct.unpack_from('<I',b,60)[0];n=struct.unpack_from('<H',b,nt+6)[0];table=nt+24+struct.unpack_from('<H',b,nt+20)[0];out=[]
 for i in range(n):
  p=table+i*40;size,rva=struct.unpack_from('<II',b,p+8)
  if struct.unpack_from('<I',b,p+36)[0]&0x20000000:out.append((base+rva,base+rva+size))
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
  u.mem_map(STACK,STACK_SIZE);u.mem_map(STOP,4096);u.mem_map(SCRATCH,0x50000)
  state=dict(active=False,allowed=executable_ranges(ROOT/item['path'],pe.base),faults=[])
  def code(uc,address,size,state):
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
  m=members[case['entry']];reference=None;reference_regs=None;routes=[];failures=[]
  for label,(u,state)in machines.items():
   if label=='native':selected=[('native',case['entry'],case['arguments'])]
   else:
    provider=m['providers'][label];selected=[]
    if provider['named']:selected.append(('named',provider['named']['va'],case['named_arguments']))
    canonical=provider['canonical']
    if canonical and Path(m['source']).stem+'.obj'in canonical['owner']:
     selected.append(('canonical',canonical['va'],case['arguments']))
   for route,pc,args in selected:
    state.update(active=False,faults=[],write_ranges=case['write_ranges']);errors=[]
    u.mem_write(DATA_START,bytes(seed));u.mem_write(SCRATCH,b'\xa5'*0x50000);u.mem_write(SP-0x2000,b'\xcc'*0x4000)
    for at,value in case['stores']:u.mem_write(at,value)
    initial=[0xa5b6c7d8,case['receiver'],0x33333333,saved[0],SP,*saved[1:]]
    for reg,value in zip(registers,initial):u.reg_write(reg,value)
    for reg,value in [(x.UC_X86_REG_EFLAGS,case['eflags']),(x.UC_X86_REG_FPCW,0x37f),(x.UC_X86_REG_FPSW,0),(x.UC_X86_REG_FPTAG,0xffff)]:u.reg_write(reg,value)
    u.mem_write(SP,struct.pack('<I',STOP))
    for i,value in enumerate(args):u.mem_write(SP+4+i*4,struct.pack('<I',value&0xffffffff))
    attempts+=1;state['active']=True
    try:u.emu_start(pc,STOP,count=200000)
    except Exception as exc:errors.append(dict(kind='execution-error',detail=repr(exc)))
    finally:state['active']=False
    values=[u.reg_read(r)for r in registers];flags=u.reg_read(x.UC_X86_REG_EFLAGS);ram=[bytes(u.mem_read(a,n))for a,n in case['watched']]
    if u.reg_read(x.UC_X86_REG_EIP)!=STOP or state['faults']:errors.append(dict(kind='return/fault',faults=state['faults']))
    if values[4]!=SP+4+case[route+'_cleanup']:errors.append(dict(kind='stack-cleanup',actual=hex(values[4])))
    if [values[i]for i in [3,5,6,7]]!=saved:errors.append(dict(kind='nonvolatile-registers'))
    if route=='native':
     reference=(None if case['result_mask']is None else values[0]&case['result_mask'],ram);reference_regs=(values,flags)
     if 'expected_eax'in case and values[0]!=case['expected_eax']:errors.append(dict(kind='native-authored-result',actual=hex(values[0])))
    else:
     if reference!=(None if case['result_mask']is None else values[0]&case['result_mask'],ram):errors.append(dict(kind='native-C-result-or-memory'))
     if route=='canonical'and case['canonical_registers_and_flags']and reference_regs!=(values,flags):errors.append(dict(kind='canonical-registers-or-flags'))
    row=dict(image=label,route=route,eax=hex(values[0]),eflags=hex(flags),errors=errors)
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
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--run',action='store_true');p.add_argument('--group',action='append',default=[]);p.add_argument('--output-dir',type=Path,default=ROOT/'build/actor-animal-following/cohort3-verification/runs');a=p.parse_args()
 registry=load(a.run);cases=definitions(registry)
 if a.group:cases=[c for c in cases if c['id'].split('/')[0]in a.group]
 assert cases and len({c['id']for c in cases})==len(cases),'Empty or duplicate case selection'
 if not a.run:print(json.dumps(dict(source_entries=registry['source_entries'],planned_cases=len(cases),planned_entries=len({c['entry']for c in cases}),execution_performed=False)));return
 directory=a.output_dir/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%d-%H%M%S-%f');directory.mkdir(parents=True,exist_ok=False)
 r=execute(registry,cases,directory);print(json.dumps(dict(source_entries=r['source_entries'],entries_compared=r['entries_compared'],cases=r['cases'],invocations=r['invocations'],failed_cases=r['failed_cases'],cohort_validation_complete=False,result=str(directory/'result.json'))));sys.exit(bool(r['failed_cases']))

if __name__=='__main__':main()
