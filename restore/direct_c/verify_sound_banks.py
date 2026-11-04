"""Seven native bank bodies, real file registration and finite UI playback.

Reuse the established CRT/SEH fixture. No game algorithm is intercepted.
Miles endpoints are controlled only for the UI playback compositions.
"""
import verify_sound_samples as support
from verify_sound_samples import *

catalogue=json.loads((ROOT/'metadata/sound-bank-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-bank-rea-evidence.json').read_text())['records']}
owned={};branches={};hits=set();edges=set();previous=[None]
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a))
 owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
bank_names={0x539cf0:'restore_sound_load_building_sample',0x539d60:'restore_sound_initialize_building_samples',0x53af70:'restore_sound_load_ui_sample',0x53af90:'restore_sound_initialize_ui_samples',0x53b090:'restore_sound_play_ui_sample',0x53b100:'restore_sound_load_actor_sample',0x53b130:'restore_sound_initialize_actor_samples'}
support.names.update(bank_names)
support.arity.update({0x539cf0:3,0x539d60:0,0x53af70:2,0x53af90:0,0x53b090:1,0x53b100:3,0x53b130:0,0x55f0e8:1})
support.members.add(0x55f0e8)
media={0x56f2c0:('_AIL_sample_status_4',1),0x56f2bc:('_AIL_set_sample_file_12',3),0x56f2a0:('_AIL_set_sample_loop_count_8',2),0x56f2a4:('_AIL_set_sample_pan_8',2),0x56f2f8:('_AIL_set_sample_volume_8',2),0x56f2fc:('_AIL_set_sample_reverb_16',4),0x56f2b0:('_AIL_start_sample_4',1),0x56f2b4:('_AIL_file_read_8',2),0x56f2b8:('_AIL_init_sample_4',1),0x56f308:('_AIL_mem_free_lock_4',1)}
HANDLES,SAMPLES,CACHE=S+0x38000,S+0x39000,S+0x3a000
for c,u,state in machines:
 state['methods'].update({(sym('FUN_%08x'%fn) if c else fn):fn for fn in [*bank_names,0x55f0e8]})
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   if pc in points:hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def entry(uc,pc,size,data):
  if data['collect'] and pc==data['play_entry']:
   sp=uc.reg_read(UC_X86_REG_ESP);p=get(uc,sp+(8 if data['compiled'] else 4))
   data['events'].append(('play_request',tuple(get(uc,p+i*4) for i in range(5))))
 state['play_entry']=sym('FUN_0055f0e8') if c else 0x55f0e8
 u.hook_add(UC_HOOK_CODE,entry,state,begin=state['play_entry'],end=state['play_entry'])

def setup(c,u,state,fn,cfg):
 previous[0]=None
 # The common loader fixture also checks all static/private bytes and stores.
 clean={k:v for k,v in cfg.items() if k not in ['open_bank_count','close_bank_count']}
 args=original_setup(c,u,state,fn,clean);state['cfg']=cfg
 row=(0xf54670+(cfg.get('bank',0)&0xffffffff)*20)&0xffffffff
 if fn==0x539cf0 and not cfg.get('unmapped_bank'):put(u,(row+16)&0xffffffff,cfg.get('variants',0))
 for token,key in [(0x561a0a,'open_bank_count'),(0x561994,'close_bank_count')]:
  pc=address(c,token);n,cleanup,old=state['hooks'][pc]
  def mutate(uc,args,old=old,key=key):
   result=old(uc,args)
   if cfg.get(key) is not None:put(uc,(row+16)&0xffffffff,cfg[key])
   return result
  state['hooks'][pc]=(n,cleanup,mutate)
 def duplicate(uc,args):
  text=cstring(uc,args[0]);number=state['counts'].get(0x56b177,0)+1;state['counts'][0x56b177]=number
  failed=cfg.get('fail_duplicate') or number==cfg.get('fail_duplicate_at',-1)
  value=0 if failed else state['bump'];state['bump']+=128
  assert len(text)<128
  if value:uc.mem_write(value,text+b'\0')
  state['events'].append((0x56b177,text,value));return value
 hook(c,state,0x56b177,1,duplicate)
 put(u,0xf55b88,cfg.get('enabled',1));byte(u,0xe38e63,cfg.get('effects',1));put(u,0xe38ec4,cfg.get('volume',0x12345678))
 put(u,OBJ+0x200,cfg.get('disabled',0));put(u,OBJ+0x198,cfg.get('channels',0))
 for at,value in [(0x19c,HANDLES),(0x1a8,SAMPLES),(0x1ac,CACHE),(0x168,cfg.get('reverb',0)),(0x16c,0x3f000001),(0x170,0x3f000002),(0x174,0x3f000003)]:put(u,OBJ+at,value)
 channels=cfg.get('channels',0)
 for i in range(max(0,channels)):
  put(u,HANDLES+i*4,0x22220000+i);put(u,SAMPLES+i*40,cfg.get('owner',0));put(u,SAMPLES+i*40+4,cfg.get('playing_id',0));put(u,SAMPLES+i*40+24,0)
  put(u,CACHE+i*12,cfg.get('cache_id',7));put(u,CACHE+i*12+4,cfg.get('cache_data',S+0x3b000+i*128));put(u,CACHE+i*12+8,0xabcdef00+i)
 index=cfg.get('index',0);id_at=(0xf543b8+(index&0xffffffff)*4)&0xffffffff
 if fn==0x53b090 and not cfg.get('unmapped_index'):put(u,id_at,cfg.get('id',7))
 for slot,(name,n) in media.items():
  def backend(uc,args,slot=slot,name=name):
   number=state['counts'].get(slot,0)+1;state['counts'][slot]=number
   state['events'].append((name,args))
   if slot==0x56f2c0:
    statuses=cfg.get('statuses',[2]);return statuses[min(number-1,len(statuses)-1)]
   if slot==0x56f2b4:return cfg.get('file_data',0)
   return cfg.get('media_result',0xffffffff)
  pc=sym(name) if c else S+0x4b000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(n,0 if c else n*4,backend)
 if fn==0x539cf0:return [cfg.get('bank',0),NAME,cfg.get('mode',1)]
 if fn==0x53af70:return [index,NAME]
 if fn==0x53b090:return [index]
 if fn==0x53b100:return [cfg.get('bank',0),NAME,cfg.get('variant',0)]
 return []

# Reuse comparison/ABI code without substituting an algorithm child.
original_setup=support.setup
support.setup=setup
def invoke(c,u,state,fn,args,route):
 pc=fn if not c else sym(bank_names[fn] if route=='named' else 'FUN_%08x'%fn)
 for reg,value in zip(registers,[0xa5b6c7d8,state['self'],0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(args):put(u,SP+4+i*4,value)
 try:u.emu_start(pc,STOP,count=20000000)
 except UcError:result=('fault',)
 else:
  assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
  assert u.reg_read(UC_X86_REG_ESP)==SP+4
  assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
  assert get(u,TEB)==0xfeedbabe
  result=None
 if not c:
  state['collect']=False
  for slot in media:u.mem_write(slot,seed[slot-0x560000:slot-0x560000+4])
  state['collect']=True
 return result
support.invoke=invoke

def exercise(fn,**cfg):
 global checked,invocations,compositions
 # support.exercise resolves its own setup/invoke and compares three routes.
 support.exercise(fn,**cfg)
 if cfg.get('cache_data')==0 and cfg.get('file_data')==0:
  assert all(state['counts'].get(0x56f2b4)==1 for c,u,state in machines),('cold-cache read was not reached',cfg)
 checked=support.checked;invocations=support.invocations;compositions=support.compositions

if __name__=='__main__':
 for fn in bank_names:exercise(fn,names=[],count=0,capacity=300,open_policy='missing')
 exercise(0x53b090,channels=2,count=10,cache_id=7)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for bank,mode,count in itertools.product([-1,0,1,236,237,0x40000000],[-1,0,1,2],[-2,-1,0,2,3,4,0x7fffffff,0x80000000]):
  exercise(0x539cf0,bank=bank,mode=mode,variants=count)
 for key in ['open_bank_count','close_bank_count']:
  for count in [-2,-1,0,2,3,4,5,0x7fffffff,0x80000000]:exercise(0x539cf0,**{key:count})
 for count in [-1,0,2]:
  for policy in ['primary','fallback','missing']:exercise(0x539cf0,variants=count,open_policy=policy)
 for index in [-101,-1,0,18,19,99,100,0x40000000]:
  for policy in ['primary','fallback','missing']:exercise(0x53af70,index=index,open_policy=policy)
 for bank,variant in itertools.product([-1,0,32,33,0x40000000],[-6,-1,0,4,5,0x40000000]):exercise(0x53b100,bank=bank,variant=variant)
 for fn in [0x539d60,0x53af90,0x53b130]:
  for policy,capacity in itertools.product(['primary','fallback','missing','mixed'],[0,1,20,300]):exercise(fn,open_policy=policy,capacity=capacity,names=[],count=0)
  exercise(fn,open_policy='fallback',fail_duplicate_at=4,names=[],count=0,capacity=300)
 for enabled,effects,index,id in itertools.product([0,1,256,0xffffffff],[0,1,128,255],[-1,0,18,19,0x7fffffff],[-1,0,7,0x7fffffff,0x80000000]):
  exercise(0x53b090,enabled=enabled,effects=effects,index=index,id=id)
 # Each request-field alias is a real accepted negative index. This exposes
 # the former late ID reload and compares the exact native store order.
 for field in range(5):
  index=(0xe92da8+field*4-0xf543b8)//4
  for volume in [0,64,0x12345678,0x80000000,0xffffffff]:exercise(0x53b090,index=index,id=7,volume=volume,channels=2,count=10,cache_id=7)
 for channels,statuses,reverb in itertools.product([1,2,4],[[2],[4],[4,2],[2,4,2,2,4,2]],[0,1,256]):
  exercise(0x53b090,channels=channels,statuses=statuses,count=10,reverb=reverb)
 for cfg in [dict(channels=2,owner=0xffffffff,playing_id=7,statuses=[4]),dict(channels=2,count=8,names=[b'file.wav']*8,cache_id=6,cache_data=0,file_data=0),dict(disabled=1),dict(disabled=256),dict(count=0),dict(id=100,count=10)]:exercise(0x53b090,**cfg)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared banks:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_banks.c','runtime/sound_bank_loads.inc','runtime/mission_unlock.c','include/sound_banks.h','runtime/sound_samples.c','runtime/sound_file_lookup.c','runtime/owned_buffer.c','include/sound_samples.h','include/sound_file_lookup.h','include/owned_buffer.h','include/byte_string_compare.h','include/sound_initialization.h','include/legacy_format.h','include/host_allocator.h','include/media_bindings.h','direct_c/verify_sound_banks.py','direct_c/verify_sound_samples.py','direct_c/verify_options_startup.py','metadata/sound-bank-native.json','metadata/sound-bank-rea-evidence.json','metadata/sound-sample-native.json','metadata/sound-sample-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2483,2484,2485,2487,2488,2489,2490,2491,2499,2500,2686,2698,2699,2701,2702,2703,2725,2726,2848]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_crt_endpoints=[hex(a) for a in backends],controlled_media_endpoints=[name for name,n in media.values()],limitations=['All seven bank bodies and ordered static loads execute. Actual file-ID loading, registration, case-sensitive strcmp and ownership helpers execute.','Finite UI paths include real duplicate scan, channel selection, cache-reference refresh, cache lookup and cached playback; cold cache is tested only at null file-read failure. Broader cache/channel states, backend file loading, hardware, concurrency and OS exception dispatch are not established.','All static bytes, private tables, ordered stores, meaningful child/CRT/Miles inputs, normal-return ABI and FS chain are compared. Incidental EAX from void APIs is excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-bank-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')

