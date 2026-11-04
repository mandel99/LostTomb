"""Native/C 55DEA2 sample-pool resize with real stop/reset members.

Only heap and Miles endpoints are controlled. The mapped test heap records
wrapped requests and exposes guarded pages, without allocating giant blocks.
"""
import verify_sound_playback as engine
import verify_sound_stopping as stopping
from verify_sound_playback import *
catalogue=json.loads((ROOT/'metadata/sound-allocation-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-allocation-rea-evidence.json').read_text())['records']}
owned={};branches={};hits=set();edges=set();previous=[None]
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
FN=0x55dea2;POOL=S+0x41000;AUX=S+0x8000
engine.names[FN]='restore_sound_resize_samples';engine.arity[FN]=1
allocation_media={0x56f2f4:('_AIL_allocate_sample_handle_4',1),0x56f2f0:('_AIL_release_sample_handle_4',1)}
for c,u,state in machines:
 pc=sym('FUN_0055dea2') if c else FN;state['methods'][pc]=FN;u.hook_add(UC_HOOK_CODE,engine.method,state,begin=pc,end=pc)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state,begin=min(points),end=max(points))

original_setup=engine.setup
def setup(c,u,state,fn,cfg):
 previous[0]=None
 clean={**cfg,'channels':cfg.get('old_count',2),'channels3d':cfg.get('channels3d',0)}
 original_setup(c,u,state,fn,clean);state['cfg']=cfg;base=state['self']
 if base:
  pointers=[HANDLES,AUX,cfg.get('samples',SAMPLES),cfg.get('cache',CACHE)]
  for i,off in enumerate([0x19c,0x1a0,0x1a8,0x1ac]):put(u,base+off,pointers[i] if cfg.get('old_mask',0)&(1<<i) else 0)
  for off,value in cfg.get('old_pointers',{}).items():put(u,base+off,value)
  put(u,base+0x178,cfg.get('digital',0x12345678))
 state['heap_requests']=[]
 def mutate(uc,token,number):
  for key,nth,changes in cfg.get('mutations',[]):
   if key==token and nth==number:
    for at,value in changes:put(uc,at,value)
 def allocate(uc,args):
  token=0x561a1d;number=state['counts'].get(token,0)+1;state['counts'][token]=number
  result=0 if number==cfg.get('fail_heap') else POOL+(number-1)*0x2000
  if cfg.get('heap_results') is not None:result=cfg['heap_results'][min(number-1,len(cfg['heap_results'])-1)]
  state['heap_requests'].append(args[0]);state['events'].append(('allocate',args[0],result))
  # The test endpoint supplies fixed mapped pages even for wrapped/huge sizes.
  # Actual owned writes, requested sizes and guard bytes are all compared.
  if result and POOL<=result<S+0x49000:uc.mem_write(result,b'\xcd'*0x1000)
  mutate(uc,token,number);return result
 def free(uc,args):
  token=0x561a2b;number=state['counts'].get(token,0)+1;state['counts'][token]=number
  state['events'].append(('free',args));mutate(uc,token,number);return 0
 hook(c,state,0x561a1d,1,allocate);hook(c,state,0x561a2b,1,free)
 for slot,(name,n) in allocation_media.items():
  def endpoint(uc,args,slot=slot,name=name):
   number=state['counts'].get(slot,0)+1;state['counts'][slot]=number
   result=(0 if number==cfg.get('fail_handle') else cfg.get('handle_base',0x70000000)+number) if slot==0x56f2f4 else cfg.get('release_result',0xffffffff)
   state['events'].append((name,args,result&0xffffffff));mutate(uc,slot,number);return result
  pc=sym(name) if c else S+0x49000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(n,0 if c else n*4,endpoint)
 return [cfg.get('requested',3)]
engine.setup=setup
original_invoke=engine.invoke
def invoke(c,u,state,fn,args,route):
 result=original_invoke(c,u,state,fn,args,route)
 if not c:
  state['collect']=False
  for slot in allocation_media:u.mem_write(slot,seed[slot-0x560000:slot-0x560000+4])
  state['collect']=True
 return result
engine.invoke=invoke

def exercise(**cfg):
 global checked,invocations,compositions
 engine.exercise(FN,**cfg)
 for c,u,state in machines:
  for token,value in cfg.get('exact_calls',{}).items():assert state['counts'].get(token,0)==value,(cfg,c,token,state['counts'])
  if 'expected_count' in cfg:assert get(u,state['self']+0x198)==(cfg['expected_count']&0xffffffff),(cfg,c,hex(get(u,state['self']+0x198)))
 checked=engine.checked;invocations=engine.invocations;compositions=engine.compositions

if __name__=='__main__':
 exercise();exercise(old_mask=15);exercise(fail_heap=2,old_count=7,expected_count=7);exercise(fail_handle=1,expected_count=3)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for requested,mask,old_count in itertools.product([-1,0,1,3,8],range(16),[-1,0,2]):
  extra={}
  if requested>0 and old_count>0 and mask&1:
   if not mask&4:extra['expected_fault']=(0,4)
   elif not mask&8:extra['expected_fault']=(4,4)
  exercise(requested=requested,old_mask=mask,old_count=old_count,**extra)
 for mask,heap_failure in itertools.product([0,2,4,8,15],[1,2,3,4]):exercise(old_mask=mask,fail_heap=heap_failure,old_count=7,expected_count=7)
 for requested,failed in itertools.product([1,3,8],range(1,10)):exercise(requested=requested,fail_handle=failed,old_count=7,expected_count=requested)
 for count in [0x4000000,0x10000000,0x20000000,0x40000000,0x7fffffff,0x80000000,0xffffffff]:
  for failure in [1,2,3,4]:exercise(requested=count,fail_heap=failure,old_count=7,expected_count=7)
  if count<0x80000000:exercise(requested=count,fail_handle=1,expected_count=count,exact_calls={0x561a1d:4,0x56f2f4:1})
 for disabled,mask in itertools.product([0,1,256,0xffffffff],[0,15]):exercise(disabled=disabled,old_mask=mask,channels3d=2)
 for data in [0,DATA,0xffffffff]:exercise(old_mask=15,other_data=data)
 for cfg in [dict(mutations=[(0x56f2cc,1,[(OBJ+0x200,256)])]),dict(mutations=[(0x56f2f0,1,[(OBJ+0x198,1)])]),dict(mutations=[(0x56f308,1,[(OBJ+0x1ac,CACHE2)])]),dict(mutations=[(0x561a2b,1,[(OBJ+0x1a0,AUX+128),(OBJ+0x1a8,0)])])]:exercise(old_mask=15,**cfg)
 for ordinal,offset in enumerate([0x19c,0x1a0,0x1a8,0x1ac],1):
  exercise(mutations=[(0x561a1d,ordinal,[(OBJ+offset,0)])])
  exercise(mutations=[(0x561a1d,ordinal,[(OBJ+0x178,0x80000000+ordinal)])])
 for cfg in [dict(mutations=[(0x56f2f4,1,[(OBJ+0x19c,HANDLES)])]),dict(mutations=[(0x56f2f4,1,[(OBJ+0x198,1)])]),dict(mutations=[(0x56f2f4,1,[(OBJ+0x178,0xffffffff)])]),dict(handle_base=0x80000000),dict(handle_base=0xffffffff)]:exercise(**cfg)
 exercise(read_mutations={OBJ+0x19c:[(3,[(OBJ+0x19c,0)])]},exact_calls={0x561a1d:1},expected_count=2)
 exercise(old_mask=2,read_mutations={OBJ+0x1a0:[(2,[(OBJ+0x1a0,0x11223344)])]})
 exercise(old_mask=15,read_mutations={OBJ+0x1ac:[(2,[(OBJ+0x1ac,CACHE2)])]})
 exercise(old_mask=5,old_count=1,old_pointers={0x1ac:0},expected_fault=(4,4))
 exercise(self=0,requested=0);exercise(self=0,requested=1,expected_fault=(0x19c,4))
 exercise(heap_results=[OBJ+0x198,POOL+0x2000,POOL+0x4000,POOL+0x6000],fail_handle=2,expected_fault=(4,4))
 exercise(heap_results=[OBJ+0x1a8,POOL+0x2000,POOL+0x4000,POOL+0x6000])
 exercise(old_mask=15,old_count=0x7fffffff,read_mutations={OBJ+0x198:[(4,[(OBJ+0x198,0)])]})
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared allocation:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_allocation.c','include/sound_allocation.h','src/unit_02687.c','runtime/sound_sample_reset.c','runtime/sound_stop_channels.c','include/sound_stopping.h','include/sound_playback.h','include/sound_initialization.h','include/media_bindings.h','direct_c/verify_sound_allocation.py','direct_c/verify_sound_stopping.py','direct_c/verify_sound_playback.py','direct_c/verify_options_startup.py','metadata/sound-allocation-native.json','metadata/sound-allocation-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2691,2694,2695,2697]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_heap_endpoints=['00561a1d','00561a2b'],controlled_media_endpoints=['_AIL_allocate_sample_handle_4','_AIL_release_sample_handle_4','_AIL_end_sample_4','_AIL_end_3D_sample_4','_AIL_mem_free_lock_4'],limitations=['Complete 55DEA2 runs with actual stop-all, individual stop members and ordered channel reset. Only heap/Miles endpoints are controlled.','All static/private bytes, guarded heap pages, ordered writes, meaningful child/backend inputs, full EAX, normal-return ABI and matched explicit memory faults are compared.','Wrapped and huge requested sizes are observed at the test allocator; fixed mapped guard pages permit bounded actual paths through immediate allocation/handle failure. Physical giant allocations, full successful huge traversals, backend/device behavior, concurrency and native OS exception dispatch remain unproved.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-allocation-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
