"""Explicit-receiver stopping, nested disabled checks and file channel lookup.

Actual stop members execute. Only the three Miles endpoints are controlled;
the shared playback fixture supplies static/private memory and ABI comparison.
"""
import verify_sound_playback as support
from verify_sound_playback import *
catalogue=json.loads((ROOT/'metadata/sound-stop-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-stop-rea-evidence.json').read_text())['records']}
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
stop_names={0x55eb99:'restore_sound_stop_sample',0x55ebd4:'restore_sound_stop_3d_sample',0x55ec0f:'restore_sound_stop_looping_samples',0x55ec7c:'restore_sound_stop_all_samples',0x55efdc:'restore_sound_find_channel_file'}
support.names.update(stop_names);support.arity.update({0x55eb99:1,0x55ebd4:1,0x55ec0f:0,0x55ec7c:0,0x55efdc:1});support.voids.add(0x55ec7c)
stop_media={0x56f2cc:('_AIL_end_sample_4',1),0x56f2c8:('_AIL_end_3D_sample_4',1),0x56f2c4:('_AIL_sample_loop_count_4',1)}
HANDLES3D=S+0xc000
for c,u,state in machines:
 for fn in stop_names:
  pc=sym('FUN_%08x'%fn) if c else fn;state['methods'][pc]=fn;u.hook_add(UC_HOOK_CODE,support.method,state,begin=pc,end=pc)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))

original_setup=support.setup
def setup(c,u,state,fn,cfg):
 previous[0]=None;original_setup(c,u,state,fn,cfg)
 base=state['self']
 if base:put(u,base+0x1e4,cfg.get('channels3d',3));put(u,base+0x1f0,HANDLES3D)
 u.mem_write(HANDLES3D-256*4,struct.pack('<777I',*((0x33330000+i)&0xffffffff for i in range(-256,521))))
 if cfg.get('match') is not None:put(u,SAMPLES+cfg['match']*40+4,cfg.get('file',1))
 for slot,(name,n) in stop_media.items():
  def endpoint(uc,args,slot=slot,name=name):
   number=state['counts'].get(slot,0)+1;state['counts'][slot]=number;state['events'].append((name,args))
   for key,nth,changes in cfg.get('mutations',[]):
    if key==slot and nth==number:
     for destination,value in changes:put(uc,destination,value)
   if slot==0x56f2c4:
    results=cfg.get('loop_results',[cfg.get('loop_count',0)]);return results[min(number-1,len(results)-1)]
   return cfg.get('media_result',0xffffffff)
  pc=sym(name) if c else S+0x4a000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(n,0 if c else n*4,endpoint)
 if fn in [0x55eb99,0x55ebd4]:return [cfg.get('channel',0)]
 if fn==0x55efdc:return [cfg.get('file',1)]
 return []
support.setup=setup
original_invoke=support.invoke
def invoke(c,u,state,fn,args,route):
 result=original_invoke(c,u,state,fn,args,route)
 if not c:
  state['collect']=False
  for slot in stop_media:u.mem_write(slot,seed[slot-0x560000:slot-0x560000+4])
  state['collect']=True
 return result
support.invoke=invoke

def exercise(fn=0x55ec7c,**cfg):
 global checked,invocations,compositions
 support.exercise(fn,**cfg)
 for c,u,state in machines:
  for slot,n in cfg.get('exact_calls',{}).items():assert state['counts'].get(slot,0)==n,(hex(fn),cfg,c,slot,state['counts'])
 checked=support.checked;invocations=support.invocations;compositions=support.compositions

if __name__=='__main__':
 for fn in stop_names:exercise(fn)
 exercise(mutations=[(0x56f2cc,1,[(OBJ+0x200,256)])],exact_calls={0x56f2cc:1,0x56f2c8:0})
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in stop_names:
  for disabled,count,count3d in itertools.product([0,1,256,0xffffffff],[-1,0,1,3],[-1,0,2]):exercise(fn,disabled=disabled,channels=count,channels3d=count3d)
 for fn in [0x55eb99,0x55ebd4]:
  for channel in [-129,-128,-1,0,1,127,128,255,256,0x40000000,0x80000000]:exercise(fn,channel=channel)
 for count,match,file in itertools.product([0,1,3,8],[-1,0,1,2,7],[0,1,0x80000000,0xffffffff]):exercise(0x55efdc,channels=count,match=match,file=file)
 for value in [0,1,2,127,128,255,256,0x80000000,0xffffffff]:exercise(0x55ec0f,loop_count=value)
 exercise(0x55ec0f,loop_results=[0,1,0],exact_calls={0x56f2c4:3,0x56f2cc:2})
 for fn in [0x55ec0f,0x55ec7c]:
  for slot in [0x56f2cc,*([0x56f2c4] if fn==0x55ec0f else [0x56f2c8])]:
   for disabled in [1,256,0xffffffff]:exercise(fn,mutations=[(slot,1,[(OBJ+0x200,disabled)])])
 exercise(0x55ec0f,mutations=[(0x56f2c4,1,[(OBJ+0x200,1)])],exact_calls={0x56f2c4:3,0x56f2cc:0})
 exercise(mutations=[(0x56f2cc,1,[(OBJ+0x198,1),(OBJ+0x1e4,1)])],exact_calls={0x56f2cc:1,0x56f2c8:1})
 exercise(mutations=[(0x56f2cc,1,[(OBJ+0x19c,HANDLES+4),(OBJ+0x1f0,HANDLES3D+8)])])
 exercise(0x55ec0f,mutations=[(0x56f2c4,1,[(OBJ+0x19c,HANDLES+4)])])
 for fn in stop_names:exercise(fn,self=0,expected_fault=(0x198 if fn==0x55efdc else 0x200,4))
 for fn in [0x55ec0f,0x55ec7c,0x55efdc]:exercise(fn,channels=0x7fffffff,channels3d=0,read_mutations={OBJ+0x198:[(4,[(OBJ+0x198,0)])]})
 exercise(channels=0,channels3d=0x7fffffff,read_mutations={OBJ+0x1e4:[(4,[(OBJ+0x1e4,0)])]})
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared stopping:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_stop_channels.c','runtime/sound_stop_loops.c','include/sound_stopping.h','include/sound_initialization.h','include/media_bindings.h','direct_c/verify_sound_stopping.py','direct_c/verify_sound_playback.py','direct_c/verify_options_startup.py','metadata/sound-stop-native.json','metadata/sound-stop-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2694,2695,2696,2697,2700]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_media_endpoints=[name for name,n in stop_media.values()],limitations=['Five complete bodies execute, including actual nested stop members and receiver forwarding. Only three Miles endpoints are controlled.','Stop-all and infinite-loop stopping retain child disabled rechecks and fresh counts/handles after callbacks; looping means a full zero backend loop count.','All static/private bytes, ordered writes, child/backend inputs, meaningful full EAX and normal-return ABI are compared. Memory fault addresses/widths match; physical audio, backend internals and OS exception dispatch remain unproved.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-stop-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
