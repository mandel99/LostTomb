"""Native/C 2D channel playback and cache lifecycle; only Miles is controlled.

Actual reset, reference refresh, channel/cache lookup and cold/cached loading
execute, including signed-byte indices and callback/read-sensitive aliases.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/sound-playback-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-playback-rea-evidence.json').read_text())['records']}
owned={};branches={};hits=set();edges=set();previous=[None]
checked=invocations=compositions=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
names={0x55e5d2:'restore_sound_reset_sample',0x55ecf3:'restore_sound_refresh_cache_references',0x55edd0:'restore_sound_load_sample_cache',0x55f02d:'restore_sound_find_available_channel',0x55f07f:'restore_sound_find_cached_sample',0x55f0e8:'restore_sound_play_sample'}
arity={0x55e5d2:1,0x55ecf3:0,0x55edd0:2,0x55f02d:0,0x55f07f:1,0x55f0e8:1}
voids={0x55e5d2,0x55ecf3}
OBJ,R,FILES,HANDLES,SAMPLES,CACHE,CACHE2,NAMES,DATA=S+0x1000,S+0x4000,S+0x5000,S+0x10000,S+0x20000,S+0x30000,S+0x36000,S+0x40000,S+0x48000
media={0x56f2c0:('_AIL_sample_status_4',1),0x56f2bc:('_AIL_set_sample_file_12',3),0x56f2a0:('_AIL_set_sample_loop_count_8',2),0x56f2a4:('_AIL_set_sample_pan_8',2),0x56f2f8:('_AIL_set_sample_volume_8',2),0x56f2fc:('_AIL_set_sample_reverb_16',4),0x56f2b0:('_AIL_start_sample_4',1),0x56f2b4:('_AIL_file_read_8',2),0x56f2b8:('_AIL_init_sample_4',1),0x56f308:('_AIL_mem_free_lock_4',1)}

def signed(v):return v-0x100000000 if v&0x80000000 else v
def text_arg(u,p):return None if not p else bytes(u.mem_read(p,256)).split(b'\0')[0]
for c,u,state in machines:
 state.update(compiled=c,collect=False)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def read(uc,access,at,size,value,data):
  if not data['collect'] or at not in data['read_mutations']:return
  n=data['read_counts'].get(at,0)+1;data['read_counts'][at]=n
  for nth,changes in data['read_mutations'][at]:
   if n==nth:
    data['events'].append(('read_mutation',at,n,changes))
    for destination,replacement in changes:put(uc,destination,replacement)
 u.hook_add(UC_HOOK_MEM_READ,read,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def method(uc,pc,size,data):
  if not data['collect']:return
  fn=data['methods'][pc]
  if fn==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP)
  args=tuple(get(uc,sp+4+i*4) for i in range(arity[fn]+1)) if data['compiled'] else (uc.reg_read(UC_X86_REG_ECX),*(get(uc,sp+4+i*4) for i in range(arity[fn])))
  data['methods_seen'].append((fn,args))
 state['methods']={(sym('FUN_%08x'%fn) if c else fn):fn for fn in names}
 for pc in state['methods']:u.hook_add(UC_HOOK_CODE,method,state,begin=pc,end=pc)

def setup(c,u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],methods_seen=[],faults=[],counts={},read_counts={},read_mutations=cfg.get('read_mutations',{}))
 state['hooks'].clear();state['calls'].clear()
 self=cfg.get('self',OBJ);state['self']=self
 for at,value in [(4,cfg.get('file_count',8)),(8,FILES),(0x198,cfg.get('channels',3)),(0x19c,HANDLES),(0x1a8,cfg.get('samples',SAMPLES)),(0x1ac,cfg.get('cache',CACHE)),(0x168,cfg.get('reverb',0)),(0x16c,0x3f000001),(0x170,0x40000002),(0x174,0x40400003),(0x200,cfg.get('disabled',0))]:
  if self:put(u,self+at,value)
 indices=range(-256,521)
 u.mem_write(HANDLES-256*4,struct.pack('<777I',*((0x22220000+i)&0xffffffff for i in indices)))
 rows=[]
 for i in indices:
  fields=[0x9a000000+n for n in range(10)];fields[0]=cfg.get('sample_owner',0);fields[1]=cfg.get('sample_file',0);fields[6]=i if cfg.get('identity_references') else cfg.get('reference',0)
  rows.extend(value&0xffffffff for value in fields)
 u.mem_write(SAMPLES-256*40,struct.pack('<7770I',*rows))
 cache_row=struct.pack('<III',cfg.get('other_file',0xffffffff)&0xffffffff,cfg.get('other_data',0)&0xffffffff,cfg.get('references',0)&0xffffffff)
 for cache in [CACHE,CACHE2]:u.mem_write(cache-256*12,cache_row*777)
 if cfg.get('cached',True):
  at=CACHE+cfg.get('cache_slot',1)*12;put(u,at,cfg.get('file',1));put(u,at+4,cfg.get('data',DATA));put(u,at+8,cfg.get('references',0))
 for slot,value in cfg.get('reference_rows',{}).items():put(u,CACHE+slot*12+8,value)
 for i in range(16):
  at=NAMES+i*128;u.mem_write(at,b'file%02d.wav\0'%i);put(u,FILES+i*8,at);put(u,FILES+i*8+4,0)
 if cfg.get('null_filename'):put(u,FILES+(cfg.get('file',1)&0xffffffff)*8,0)
 if cfg.get('empty_filename'):u.mem_write(NAMES+128,b'\0')
 if cfg.get('high_filename'):u.mem_write(NAMES+128,b'\xfffile.wav\0')
 req=cfg.get('request',R);state['request']=req
 if req:
  for i,value in enumerate([cfg.get('owner',42),cfg.get('file',1),cfg.get('volume',90),cfg.get('pan',64),cfg.get('loops',1)]):put(u,req+i*4,value)
 for at,value in cfg.get('initial_stores',[]):put(u,at,value)
 for slot,(name,n) in media.items():
  def endpoint(uc,args,slot=slot,name=name):
   number=state['counts'].get(slot,0)+1;state['counts'][slot]=number
   observed=(*args,text_arg(uc,args[0])) if slot==0x56f2b4 else args
   state['events'].append((name,observed))
   for key,nth,changes in cfg.get('mutations',[]):
    if key==slot and nth==number:
     for destination,replacement in changes:put(uc,destination,replacement)
   if slot==0x56f2c0:
    if cfg.get('status_results'):
     results=cfg['status_results'];return results[min(number-1,len(results)-1)]
    index=signed((args[0]-0x22220000)&0xffffffff)
    return cfg.get('busy_status',4) if index<cfg.get('free_channel',0) else cfg.get('free_status',2)
   if slot==0x56f2b4:return cfg.get('file_data',DATA+256)
   return cfg.get('media_result',0xa5b6c7d8)
  pc=sym(name) if c else S+0x4b000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(n,0 if c else n*4,endpoint)
 if fn==0x55e5d2:return [cfg.get('channel',0)]
 if fn==0x55edd0:return [req,cfg.get('channel',0)]
 if fn in [0x55f07f,0x55f0e8]:return [req]
 return []

def invoke(c,u,state,fn,args,route):
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 argv=[state['self'],*args] if c else args
 for reg,value in zip(registers,[0xa5b6c7d8,state['self'],0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 try:u.emu_start(pc,STOP,count=4000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(0 if c else arity[fn]*4)
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return None if fn in voids else u.reg_read(UC_X86_REG_EAX)

def exercise(fn=0x55f0e8,**cfg):
 global checked,invocations,compositions
 expected=None
 for c,u,state in machines:
  for route in ['native'] if not c else ['named','canonical']:
   args=setup(c,u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   result=invoke(c,u,state,fn,args,route);state['collect']=False
   if isinstance(result,tuple):assert state['faults'],(hex(fn),cfg,route,'execution fault without memory fault')
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   elif state['faults']:raise AssertionError((hex(fn),cfg,route,state['faults']))
   if not c:
    for slot in media:u.mem_write(slot,seed[slot-0x560000:slot-0x560000+4])
   current=(result,tuple(state['writes']),tuple(state['events']),tuple(state['methods_seen']),snapshot(u),bytes(u.mem_read(S,0x50000)),tuple(state['faults']))
   if expected is None:expected=current
   elif current!=expected:
    diff=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:4],current[1:4])) if x!=y]
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[4],current[4]),(S,expected[5],current[5])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'events',diff,'memory',memory,'faults',expected[-1],current[-1]))
   for slot,minimum in cfg.get('require_calls',{}).items():assert state['counts'].get(slot,0)>=minimum,(cfg,route,slot,state['counts'])
 checked+=1
 if expected[3]:compositions+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 exercise(cached=False,other_data=DATA+512,require_calls={0x56f308:1,0x56f2b4:1,0x56f2b0:1})
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in names:
  for disabled,count in itertools.product([0,1,256,0xffffffff],[-2,-1,0,1,3]):exercise(fn,disabled=disabled,channels=count)
 for channel in [-129,-128,-1,0,1,127,128,255,256,0x40000000,0x80000000]:exercise(0x55e5d2,channel=channel)
 for channels,free,status in itertools.product([1,3,8],[0,1,3,8],[0,2,4,256,0x104,0x80000004]):exercise(0x55f02d,channels=channels,free_channel=free,busy_status=status)
 for count,slot,data,file in itertools.product([0,1,3,8],[-1,0,1,2,7],[0,DATA],[-1,0,1,0x7fffffff]):exercise(0x55f07f,channels=count,cache_slot=slot,data=data,file=file)
 for cfg in [dict(null_filename=True),dict(empty_filename=True),dict(high_filename=True),dict(cached=False),dict(cached=False,file_data=0),dict(cached=False,other_data=DATA+512),dict(cached=False,other_data=DATA+512,file_data=0),dict(references=1),dict(reference_rows={0:1,1:1,2:1}),dict(channel=-1),dict(channel=128),dict(channel=0x40000000),dict(file=0x20000000)]:exercise(0x55edd0,**cfg)
 for count,free,reference in itertools.product([0,1,3,8],[0,1,8],[-1,0,1,7]):exercise(0x55ecf3,channels=count,free_channel=free,reference=reference)
 for cfg in [dict(sample_owner=42,sample_file=1,free_channel=3),dict(sample_owner=42,sample_file=0,free_channel=3),dict(sample_owner=0,sample_file=1,free_channel=3),dict(cached=False),dict(cached=False,file_data=0),dict(cached=False,other_data=DATA+512),dict(cached=False,other_data=DATA+512,file_data=0)]:exercise(**cfg)
 for slot in [0,1,2,127,128,254,255,256,511]:exercise(channels=max(3,slot+1),cache_slot=slot)
 for free in [0,1,2,127,128,254,255,256,511]:exercise(channels=max(3,free+1),free_channel=free)
 for file_count,file in itertools.product([0,1,2,8,0x7fffffff,0x80000000,0xffffffff],[-1,0,1,7,8,0x7fffffff]):exercise(file_count=file_count,file=file)
 for reverb,value in itertools.product([0,1,256,0xffffffff],[0,1,127,0x80000000,0xffffffff]):exercise(reverb=reverb,volume=value,pan=value,loops=value)
 for free in [128,254]:exercise(channels=free+1,free_channel=free,cached=False,identity_references=True)
 exercise(channels=256,status_results=[2]*257+[4]*255+[2],cached=False,identity_references=True,require_calls={0x56f2b4:1,0x56f2b0:1})
 for fn in names:exercise(fn,self=0,expected_fault=(0x1a8 if fn==0x55e5d2 else 0x198 if fn in [0x55f02d,0x55f07f] else 0x200,4))
 for fn in [0x55edd0,0x55f07f,0x55f0e8]:exercise(fn,request=0,expected_fault=(4,4))
 exercise(0x55e5d2,samples=OBJ+0x1a8,expected_fault=(3,4))
 exercise(0x55e5d2,read_mutations={OBJ+0x1a8:[(2,[(OBJ+0x1a8,SAMPLES+1280)])]})
 exercise(0x55ecf3,channels=1,free_channel=1,read_mutations={CACHE+8:[(1,[(SAMPLES+24,1),(OBJ+0x1ac,CACHE2)])]})
 for fn in [0x55ecf3,0x55edd0,0x55f02d,0x55f07f,0x55f0e8]:
  exercise(fn,channels=0x7fffffff,cached=False,references=1,free_channel=8,read_mutations={OBJ+0x198:[(4,[(OBJ+0x198,0)])]})
 for slot,changes in [(0x56f2c0,[(OBJ+0x198,1)]),(0x56f308,[(OBJ+0x1a8,SAMPLES+1280)]),(0x56f2b4,[(OBJ+0x1ac,CACHE2),(R+4,2)]),(0x56f2b8,[(OBJ+0x19c,HANDLES+4),(OBJ+0x1ac,CACHE2)]),(0x56f2bc,[(R+8,0x80000000),(R+12,0xffffffff),(R+16,256)]),(0x56f2a0,[(OBJ+0x19c,HANDLES+8),(R+12,0x12345678)]),(0x56f2f8,[(OBJ+0x168,256),(OBJ+0x174,0xdeadbeef)])]:
  exercise(cached=False,other_data=DATA+512,mutations=[(slot,1,changes)],require_calls={slot:1})
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared playback:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_playback.c','runtime/sound_sample_reset.c','include/sound_playback.h','include/sound_initialization.h','include/media_bindings.h','direct_c/verify_sound_playback.py','direct_c/verify_options_startup.py','metadata/sound-playback-native.json','metadata/sound-playback-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2691,2698,2699,2701,2702,2703]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_media_endpoints=[name for name,n in media.values()],limitations=['Six complete bodies execute, including real cache eviction/reset, reference refresh, lookup, cold loading and playback. Only Miles endpoints are controlled.','All static/private bytes, ordered stores, actual child/backend inputs and meaningful full EAX are compared; normal-return ABI and explicit matching memory faults are checked. Void incidental EAX and instruction/register state at faults are excluded.','Finite signed-byte boundary and alias/callback/read-mutation states are checked. Physical backend parsing/ownership/device behavior, concurrency and native OS exception dispatch remain unproved.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-playback-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
