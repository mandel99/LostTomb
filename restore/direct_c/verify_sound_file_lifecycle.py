"""Real file-table/owned-buffer array lifetime, explicit frames and stop prefixes."""
import verify_sound_samples as samples
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_READ,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/sound-file-lifecycle-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-file-lifecycle-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=invocations=compositions=0
excluded=set()
for f in catalogue['functions']+catalogue['fragments']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));p={int(x,16) for x in f['instruction_addresses'].split()};owned[a]=p
 omitted={int(x,16) for x in f.get('exception_fragment_instruction_addresses',[])};excluded|=omitted
 assert {i.address for i in ins}-omitted==p,(hex(a),omitted)
 for i in ins:
  if i.address in p and i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
names={0x55dbc0:'restore_sound_file_table_initialize',0x55dbf8:'restore_sound_file_table_destroy',0x55dc18:'restore_sound_file_table_clear',0x55dc63:'restore_sound_file_table_resize',0x55e58a:'restore_sound_destroy',0x560210:'restore_owned_buffer_delete',0x561363:'restore_array_destroy',0x5613cb:'restore_array_destroy_cleanup_frame',0x5613e3:'restore_array_unwind',0x561441:'restore_array_construct',0x5614ab:'restore_array_construct_cleanup_frame',0x56e140:'restore_sound_file_allocation_cleanup_frame',0x56e155:'restore_owned_buffer_cleanup_frame',0x56e168:'restore_sound_file_table_cleanup_frame',0x563c7f:'restore_array_unwind_exception_filter'}
members={0x55dbc0,0x55dbf8,0x55dc18,0x55dc63,0x55e58a,0x560210}
frames={0x5613cb,0x5614ab,0x56e140,0x56e155,0x56e168}
arrays={0x561363,0x5613e3,0x561441};returns={0x55dbc0,0x55dc63,0x560210,0x563c7f}
Q,ARR,NEW,FRAME,EXC,RECORD=samples.Q,S+0x4040,S+0x24000,S+0xd000,S+0xe000,S+0xe100
COUNT=0x12663b0
# Native55EAC6 calls AIL_shutdown through IAT56F2D8, authenticated via REA.
for c,u,state in machines:
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   if pc in points:hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state)
 def callback(uc,pc,size,data):
  if not data['collect']:return
  fn=data['life_callbacks'][pc];cfg=data['life_cfg'];number=data['callback_counts'].get(fn,0)+1
  if cfg.get('prefix') and fn==cfg.get('prefix_callback',0x560280) and number>3:
   data['stopped']='prefix';uc.emu_stop();return
  data['callback_counts'][fn]=number
  obj=get(uc,uc.reg_read(UC_X86_REG_ESP)+4) if data['compiled'] else uc.reg_read(UC_X86_REG_ECX)
  data['callbacks'].append((fn,obj));data['last_object']=obj
 for fn in [0x560280,0x5602b0]:
  pc=sym('FUN_%08x'%fn) if c else fn
  state.setdefault('life_callbacks',{})[pc]=fn;u.hook_add(UC_HOOK_CODE,callback,state,begin=pc,end=pc)
 terminate=sym('FUN_005637ee') if c else 0x5637ee
 def stopped_terminate(uc,pc,size,data):
  if data['collect']:data['stopped']='terminate';uc.emu_stop()
 u.hook_add(UC_HOOK_CODE,stopped_terminate,state,begin=terminate,end=terminate)
 def slot_read(uc,access,at,size,value,data):
  if data['collect'] and at==data['life_self']+8 and data['life_cfg'].get('second_entries_null'):
   data['entry_reads']+=1
   if data['entry_reads']==2:put(uc,at,0)
 u.hook_add(UC_HOOK_MEM_READ,slot_read,state)
 def publish(uc,pc,size,data):
  if data['collect'] and data.get('pending_life_store') is not None:
   put(uc,data['life_self']+8,data['pending_life_store']);data['pending_life_store']=None
 u.hook_add(UC_HOOK_CODE,publish,state)
 from unicorn import UC_HOOK_MEM_WRITE
 def store(uc,access,at,size,value,data):
  if data['collect'] and data['fn']==0x55dc63 and at==data['life_self']+8 and data['life_cfg'].get('after_publish_null') and value!=0 and not data['publication_changed']:
   data['publication_changed']=True;data['pending_life_store']=0
 u.hook_add(UC_HOOK_MEM_WRITE,store,state)

def setup(c,u,state,fn,cfg):
 samples.setup(c,u,state,0x55dd3f,dict(cfg,count=0,names=[]))
 table=cfg.get('self',Q);self=ARR if fn==0x560210 else table
 state.update(fn=fn,life_self=table,life_cfg=cfg,callbacks=[],callback_counts={},last_object=0,entry_reads=0,stopped=None,pending_life_store=None,publication_changed=False)
 state['events'].clear();state['writes'].clear();state['faults'].clear()
 for i in range(-16,24):
  put(u,ARR+i*8,0x70000000+((i+16)*64));put(u,ARR+i*8+4,cfg.get('ownership',1) if i%3 else 0)
 put(u,ARR-4,cfg.get('cookie',3))
 if table:
  u.mem_write(table,bytes(0x204));put(u,table,cfg.get('capacity',23));put(u,table+4,cfg.get('used',7));put(u,table+8,0 if cfg.get('null_entries') else ARR)
  put(u,table+0x200,cfg.get('disabled',1));put(u,table+0x164,5)
 put(u,COUNT,cfg.get('counter',0x10203040))
 construct=sym('restore_owned_buffer_construct_element') if c else 0x560280
 destroy=sym('restore_owned_buffer_destroy_element') if c else 0x5602b0
 count=cfg.get('count',3);stride=cfg.get('stride',8);base=cfg.get('base',ARR)
 end=(base+stride*(count&0xffffffff))&0xffffffff
 put(u,FRAME-32,cfg.get('completed',0));put(u,FRAME-28,cfg.get('completed',0) if fn==0x5613cb else count)
 put(u,FRAME+8,end);put(u,FRAME+12,stride);put(u,FRAME+16,count);put(u,FRAME+20,destroy);put(u,FRAME+24,destroy)
 if fn==0x56e140:put(u,FRAME-24,NEW)
 if fn==0x56e155:put(u,FRAME-20,0x71717171);put(u,FRAME-16,cfg.get('ownership',1))
 if fn==0x56e168:put(u,FRAME-16,table)
 put(u,EXC,RECORD);put(u,RECORD,cfg.get('exception_code',0xc0000005))
 def allocation(uc,args):
  result=0 if cfg.get('allocation_fails') else cfg.get('allocation',NEW)
  state['events'].append((0x561a1d,args,result))
  if cfg.get('allocation_mutation') and table:put(uc,table,101);put(uc,table+4,202);put(uc,table+8,ARR+8)
  return result
 def release(token):
  def action(uc,args):
   state['events'].append((token,args))
   if cfg.get('free_mutation'):
    put(uc,table,303);put(uc,table+4,404);put(uc,table+8,ARR+16)
   return 0
  return action
 hook(c,state,0x561a1d,1,allocation);hook(c,state,0x561a2b,1,release(0x561a2b));hook(c,state,0x561bdc,1,release(0x561bdc))
 # Disabled sound compositions skip every media endpoint. The enabled empty
 # composition reaches only AIL_shutdown; patch its IAT after reading the exact
 # slot from the legacy production shutdown source/metadata.
 if fn==0x55e58a:
  slot=0x56f2d8
  pc=sym('_AIL_shutdown_0') if c else S+0x49000
  if not c:put(u,slot,pc)
  def shutdown(uc,args):state['events'].append(('AIL_shutdown',args));return 0
  state['hooks'][pc]=(0,0,shutdown)
 if fn in frames:args=[FRAME] if c else []
 elif fn==0x55dc63:args=[count]
 elif fn==0x560210:args=[cfg.get('flags',3)]
 elif fn in arrays:
  args=[base if fn!=0x5613e3 else end,stride,count,construct if fn==0x561441 else destroy]
  if fn==0x561441:args.append(destroy)
 elif fn==0x563c7f:args=[cfg.get('exception_code',0xc0000005)] if c else [EXC]
 else:args=[]
 return self,args,destroy

def exercise(fn=0x55dc63,**cfg):
 global checked,invocations,compositions
 expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['named'] if fn==0x563c7f else ['named','canonical']
  for route in routes:
   self,args,destroy=setup(c,u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
   argv=[self,*args] if c and fn in members else args
   frame=FRAME if not c and fn in frames else saved[1]
   for reg,value in zip(registers,[0xa5b6c7d8,self,0x33333333,saved[0],SP,frame,saved[2],saved[3]]):u.reg_write(reg,value)
   u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
   for i,value in enumerate(argv):put(u,SP+4+i*4,value)
   try:u.emu_start(pc,STOP,count=3000000);result=u.reg_read(UC_X86_REG_EAX) if fn in returns else None
   except UcError:result='fault'
   state['collect']=False
   if state['stopped']:result=state['stopped']
   elif cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   else:
    assert not state['faults'] and u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),cfg,route,result,state['faults'],hex(u.reg_read(UC_X86_REG_EIP)))
    cleanup=0 if c else 4 if fn in [0x55dc63,0x560210] else 20 if fn==0x561441 else 16 if fn in arrays else 0
    assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup,(hex(fn),route,hex(u.reg_read(UC_X86_REG_ESP)))
    assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==[saved[0],frame,saved[2],saved[3]]
    assert get(u,samples.TEB)==0xfeedbabe,('FS chain',hex(fn),route)
   if cfg.get('prefix'):assert state['stopped']=='prefix',(fn,cfg,route)
   if fn==0x563c7f and cfg.get('exception_code')==0xe06d7363:assert state['stopped']=='terminate'
   if c:
    assert get(u,FRAME+20)==destroy and get(u,FRAME+24)==destroy
    put(u,FRAME+20,0x5602b0);put(u,FRAME+24,0x5602b0)
   if not c and fn==0x55e58a:put(u,0x56f2d8,struct.unpack_from('<I',seed,0x56f2d8-0x560000)[0])
   current=(result,tuple(state['writes']),tuple(state['events']),tuple(state['callbacks']),snapshot(u),bytes(u.mem_read(S,0x50000)),tuple(state['faults']))
   if expected is None:expected=current
   elif current!=expected:
    differences=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:4],current[1:4])) if x!=y]
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[4],current[4]),(S,expected[5],current[5])]]
    raise AssertionError((hex(fn),cfg,route,'result',expected[0],current[0],'ordered',differences,'memory',memory,'faults',expected[-1],current[-1]))
 checked+=1
 if expected[3]:compositions+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in [0x55dbc0,0x55dbf8]:
  for counter in [0,1,12,0x7fffffff,0xfffffff8,0xffffffff]:exercise(fn,counter=counter,null_entries=True)
  exercise(fn,self=0,expected_fault=(8,4))
 for self in [COUNT,COUNT-4,COUNT-8]:exercise(0x55dbc0,self=self)
 for fn in [0x55dc18,0x55dbf8]:
  for cookie,empty,own in itertools.product([-2,-1,0,1,3,5],[False,True],[0,1,256]):exercise(fn,cookie=cookie,null_entries=empty,ownership=own)
  exercise(fn,second_entries_null=True);exercise(fn,free_mutation=True)
 for flags,cookie in itertools.product([0,1,2,3,4,0xffffffff],[-2,-1,0,1,4]):exercise(0x560210,flags=flags,cookie=cookie)
 for count,failed,empty in itertools.product([-2147483648,-1,0,1,2,5,10],[False,True],[False,True]):exercise(count=count,allocation_fails=failed,null_entries=empty)
 for count in [0x20000000,0x40000000,0x7fffffff]:exercise(count=count,allocation_fails=True,allocation_mutation=True)
 for allocation in [Q-4,Q,NEW]:exercise(allocation=allocation,count=2)
 for cfg in [dict(allocation_fails=True,allocation_mutation=True),dict(allocation_mutation=True),dict(after_publish_null=True),dict(free_mutation=True),dict(second_entries_null=True)]:exercise(**cfg)
 for fn in arrays:
  for count,stride in itertools.product([-2,-1,0,1,2,5],[0,4,8,12,0xfffffff8]):exercise(fn,count=count,stride=stride)
 for fn in [0x5613cb,0x5614ab]:
  for completed,count in itertools.product([0,1,256],[-2,-1,0,1,4]):exercise(fn,completed=completed,count=count)
 for own in [0,1,256]:exercise(0x56e155,ownership=own)
 for empty in [False,True]:exercise(0x56e168,null_entries=empty)
 for disabled in [0,1,256]:exercise(0x55e58a,disabled=disabled)
 for code in [0,1,0xc0000005,0xe06d7363]:exercise(0x563c7f,exception_code=code)
 exercise(0x561363,count=-2147483648,prefix=True,prefix_callback=0x5602b0)
 exercise(0x5613e3,count=-2147483648,prefix=True,prefix_callback=0x5602b0)
 exercise(count=0x20000000,prefix=True)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared lifecycle:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real array compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/array_lifecycle.c','runtime/owned_buffer_lifecycle.c','runtime/owned_buffer.c','runtime/sound_file_lifecycle.c','include/array_lifecycle.h','include/owned_buffer.h','include/sound_file_lookup.h','include/sound_initialization.h','direct_c/verify_sound_file_lifecycle.py','direct_c/verify_sound_samples.py','direct_c/verify_options_startup.py','metadata/sound-file-lifecycle-native.json','metadata/sound-file-lifecycle-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2682,2683,2684,2685,2689,2690,2693,2724,2725,2726,2776,2777,2778,2779,2780,4391,4392,4393]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],exception_only_fragments_not_executed=sorted(hex(a) for a in excluded),stopped_prefixes=['INT_MIN reverse destruction/unwind after3 callbacks','wrapped4-byte allocation/large constructor after3 callbacks','C++ exception filter at terminate entry'],limitations=['Fourteen canonical bodies and the7-instruction filter fragment; real ownership/array/clear/resize/shutdown algorithms execute. Only allocation/free and an empty-shutdown Miles endpoint are controlled.','Frame-based cleanup branches execute under explicit prepared original-frame inputs. Native/compiled OS exception dispatch and the five exception-only instructions inside5613E3 remain unverified. The C++ terminate target is a stopped prefix, not replaced by a normal return.','Normal ABI/FS restoration, full static/private bytes, ordered writes, heap calls and actual callbacks compared. Long prefix cases retain original unbounded production loops. Arbitrary corrupted caller-stack aliases, physical heap/device behavior and concurrency are outside this finite proof.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-file-lifecycle-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
