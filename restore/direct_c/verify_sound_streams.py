"""Seven complete stream methods and real selected-voice compositions.

Only the seven Miles backend endpoints are controlled. No algorithm child or
host audio device is substituted/launched in the real voice compositions.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/sound-stream-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-stream-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
owned={};branches={};hits=set();edges=set();previous=[None];checked=invocations=compositions=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={int(p,16) for p in f['instruction_addresses'].split()};assert {i.address for i in ins}==owned[a]
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values())
names={0x55f449:'restore_sound_open_stream',0x55f639:'restore_sound_set_stream_volume',0x55f32f:'restore_sound_set_voice_volume',0x55f38d:'restore_sound_set_music_volume',0x55f561:'restore_sound_close_stream',0x55f5a1:'restore_sound_stream_is_playing',0x55f5f4:'restore_sound_get_stream_volume'}
void_methods={0x55f639,0x55f32f,0x55f38d,0x55f561};arity={a:3 if a==0x55f449 else 2 if a==0x55f639 else 1 for a in names}
voice_names={0x539a70:'restore_figure_voice_00539a70',0x53bfe0:'restore_actor_voice_variant_result',0x53d0b0:'restore_figure_selection_result_0053d0b0'}
OBJ=0xe92bf8;NAME=S+0x20000;PATH=0xf55a20
backends={int(at,16):(name,n) for at,name,n in catalogue['backends']}
for c,u,state in machines:
 state.update(collect=False,compiled=c)
 def store_event(uc,access,at,size,value,data):
  if not data['collect']:return
  base=data['self']
  if 0x560000<=at<0x126d000 or S+0xf000<=at<S+0x14000:data['writes'].append((at,size,value&((1<<(8*size))-1)))
  cfg=data['cfg']
  if at==data['slot'] and data['open_returned'] and not data['replacement_applied'] and cfg.get('stored_handle') is not None:
   data['pending_store']=(at,cfg['stored_handle']);data['replacement_applied']=True
  if at==base+(0x14c if data['fn']==0x55f38d else 0x154) and cfg.get('stored_volume') is not None and not data['volume_replaced']:
   data['pending_store']=(at,cfg['stored_volume']);data['volume_replaced']=True
 u.hook_add(UC_HOOK_MEM_WRITE,store_event,state)
 def after_store(uc,pc,size,data):
  if data['collect'] and data['pending_store'] is not None:
   at,value=data['pending_store'];data['pending_store']=None;put(uc,at,value)
 u.hook_add(UC_HOOK_CODE,after_store,state)
 def load_event(uc,access,at,size,value,data):
  if data['collect'] and at==data['slot'] and data['cfg'].get('second_handle') is not None:
   data['slot_reads']+=1
   if data['slot_reads']==2:put(uc,at,data['cfg']['second_handle'])
 u.hook_add(UC_HOOK_MEM_READ,load_event,state)
 def invalid(uc,access,at,size,value,data):
  data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def endpoint(uc,pc,size,data):
  if not data['collect']:return
  slot=data['backend_entries'][pc];name,n=backends[slot];sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(get(uc,sp+4+i*4) for i in range(n));cfg=data['cfg'];base=data['self'];target=data['slot']
  observed=args
  if slot==0x56f290:
   text=bytes(uc.mem_read(args[1],256)).split(b'\0')[0];observed=(args[0],args[1],text,args[2])
  data['events'].append((name,observed));data['counts'][slot]=data['counts'].get(slot,0)+1
  if slot==0x56f290:value=cfg.get('opened',0x22222222);data['open_returned']=True
  elif slot==0x56f298:value=cfg.get('status',4)
  elif slot==0x56f28c:value=cfg.get('reported_volume',0x12345678)
  else:value=cfg.get('backend_result',0xdeadbeef)
  if slot==0x56f2d0 and cfg.get('close_mutation'):
   put(uc,target,0xcccccccc);put(uc,base+0x200,1);put(uc,base+0x178,0x77777777)
  if slot==0x56f290 and cfg.get('open_mutation'):
   put(uc,base+0x14c,101);put(uc,base+0x154,202);put(uc,base+0x158,303);put(uc,target,0xabababab);put(uc,base+0x200,1)
  if slot==0x56f300 and cfg.get('loop_mutation'):
   put(uc,target,cfg.get('loop_handle',0x33333333));put(uc,base+0x14c,9001);put(uc,base+0x154,9002);put(uc,base+0x158,9003);put(uc,base+0x200,1)
  if slot==0x56f29c and cfg.get('set_mutation'):put(uc,target,cfg.get('set_handle',0x44444444))
  if slot==0x56f298 and cfg.get('status_mutation'):
   put(uc,target,cfg.get('status_handle',0x55555555));put(uc,base+0x14c,777);put(uc,base+0x154,888);put(uc,base+0x200,1)
  if slot==0x56f28c and cfg.get('get_volume_mutation'):
   for at,value in zip([0xe38ec8,0xe38ecc,0xe38ed0],cfg['get_volume_mutation']):put(uc,at,value)
   if cfg.get('getter_handle') is not None:put(uc,target,cfg['getter_handle'])
  if slot==0x56f29c and cfg.get('global_volume') is not None:put(uc,0xe38ecc,cfg['global_volume'])
  uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+(0 if data['compiled'] else n*4));uc.reg_write(UC_X86_REG_EIP,get(uc,sp))
 state['backend_entries']={(sym(name) if c else S+0x49000+i*16):slot for i,(slot,(name,n)) in enumerate(backends.items())}
 for pc in state['backend_entries']:u.hook_add(UC_HOOK_CODE,endpoint,state,begin=pc,end=pc)
 def method(uc,pc,size,data):
  if not data['collect']:return
  a=data['method_entries'][pc]
  if a==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);n=arity[a]
  args=tuple(get(uc,sp+4+i*4) for i in range(n+1)) if data['compiled'] else (uc.reg_read(UC_X86_REG_ECX),*(get(uc,sp+4+i*4) for i in range(n)))
  data['method_events'].append((a,args))
 method_symbols={a:('restore_sound_music_volume_0055f32f' if a==0x55f32f else 'FUN_%08x'%a) for a in names}
 state['method_entries']={(sym(name) if c else a):a for a,name in method_symbols.items()}
 for pc in state['method_entries']:u.hook_add(UC_HOOK_CODE,method,state,begin=pc,end=pc)

def prepare(u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,bytes(0x50000));u.mem_write(0x30000000,b'\xcc'*0x10000)
 for i,(slot,(name,n)) in enumerate(backends.items()):
  if not state['compiled']:put(u,slot,S+0x49000+i*16)
 base=cfg.get('self',OBJ);channel=cfg.get('channel',1)
 if fn==0x55f32f:channel=1
 if fn==0x55f38d:channel=0
 if fn in voice_names:base=OBJ;channel=1
 slot=(base+0x1d8+(channel&0xffffffff)*4)&0xffffffff;state.update(self=base,slot=slot)
 if base:
  u.mem_write(base-0x100,bytes(0x500));put(u,base+0x200,cfg.get('disabled',0));put(u,base+0x178,cfg.get('digital',0x11111111))
  put(u,base+0x14c,cfg.get('music_volume',41));put(u,base+0x154,cfg.get('voice_volume',51));put(u,base+0x158,cfg.get('ambient_volume',61));put(u,slot,cfg.get('handle',0x12121212))
 u.mem_write(NAME,b'stream.wav\0');u.mem_write(PATH,bytes(256));put(u,0xf55b88,cfg.get('audio_enabled',1));byte(u,0xe38e65,cfg.get('voice_enabled',1));byte(u,0xe38e63,0);put(u,0xe38ecc,cfg.get('volume',51))
 if fn==0x55f449:return [NAME,channel,cfg.get('loops',0)]
 if fn==0x55f639:return [channel,cfg.get('volume',51)]
 if fn in [0x55f32f,0x55f38d]:return [cfg.get('volume',51)]
 if fn in [0x55f561,0x55f5a1,0x55f5f4]:return [channel]
 if fn==0x539a70:return [NAME]
 if fn==0x53bfe0:return [cfg.get('bank',1),cfg.get('variant',0)]
 record=388;put(u,0xeadce4,0);put(u,0xeadd24,1);byte(u,0xbcd602+record,1);byte(u,0xbcd675+record,cfg.get('variant',0));put(u,0x6012ec+4,cfg.get('figure_code',230));return []

def invoke(u,pc,args,receiver,cleanup=0):
 for r,v in zip(registers,[0xa5b6c7d8,receiver,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(r,v)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,v in enumerate(args):put(u,SP+4+i*4,v)
 try:u.emu_start(pc,STOP,count=2000000)
 except UcError:return ('fault',u.reg_read(UC_X86_REG_EIP))
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(pc),hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+cleanup
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX)

def exercise(fn=0x55f449,**cfg):
 global checked,invocations,compositions
 expected=None
 for c,u,state in machines:
  routes=['native'] if not c else ['named','canonical',*(['legacy'] if fn==0x55f32f else [])]
  for route in routes:
   state.update(collect=False,writes=[],events=[],method_events=[],faults=[],counts={},cfg=cfg,fn=fn,slot_reads=0,open_returned=False,replacement_applied=False,volume_replaced=False,pending_store=None)
   args=prepare(u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   pc=fn if not c else sym((names.get(fn) or voice_names[fn]) if route=='named' else 'restore_sound_music_volume_0055f32f' if route=='legacy' else 'FUN_%08x'%fn)
   callargs=([state['self'],*args] if c and fn in names else [*args,0xa5b6c7d8] if c and route=='named' and fn==0x53bfe0 else args)
   result=invoke(u,pc,callargs,state['self'],arity[fn]*4 if not c and fn in names else 0);state['collect']=False
   if not c:
    for at in backends:put(u,at,struct.unpack_from('<I',seed,at-0x560000)[0])
   fault=tuple(state['faults'])
   if isinstance(result,tuple):assert fault,('unexpected execution fault',fn,cfg,route,result)
   elif cfg.get('expected_fault'):raise AssertionError(('missing fault',fn,cfg,route))
   if cfg.get('expected_fault'):assert fault[0][1:]==cfg['expected_fault'],(fn,cfg,route,fault)
   output=None if fn in void_methods or (c and route=='canonical' and fn in [0x53bfe0,0x53d0b0]) else result if not fault else 'fault'
   if not c and fn in [0x53bfe0,0x53d0b0]:native_output=result
   # Canonical voice entries are void. Compare named result separately below.
   private=bytes(u.mem_read(S+0xf000,0x5000))
   answer=(tuple(state['writes']),tuple(state['events']),tuple(state['method_events']),snapshot(u),private,fault,output)
   if expected is None:expected=answer
   else:
    compare_output=fn not in [0x53bfe0,0x53d0b0] or route=='named'
    if answer[:6]!=expected[:6] or (compare_output and output!=expected[6]):
     stores=next(((i,a,b) for i,(a,b) in enumerate(zip(expected[0],answer[0])) if a!=b),None);events=next(((i,a,b) for i,(a,b) in enumerate(zip(expected[1],answer[1])) if a!=b),None);memory=next(((hex(0x560000+i),a,b) for i,(a,b) in enumerate(zip(expected[3],answer[3])) if a!=b),None)
     raise AssertionError((hex(fn),cfg,route,'stores',stores,'events',events,'lengths',[(len(a),len(b)) for a,b in zip(expected[:3],answer[:3])],'methods',expected[2],answer[2],'memory',memory,'faults',expected[5],answer[5],'result',expected[6],output))
 checked+=1
 if fn in voice_names:compositions+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 for fn in voice_names:exercise(fn)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 channels=[-2,-1,0,1,2,3,10,0x40000000,-2147483648,-1073741824,-1073741814]
 for fn in [0x55f449,0x55f639,0x55f561,0x55f5a1,0x55f5f4]:
  for channel,disabled,handle in itertools.product(channels,[0,1,256],[0,0x12121212]):exercise(fn,channel=channel,disabled=disabled,handle=handle)
 for fn in [0x55f32f,0x55f38d]:
  for disabled,handle,status,volume in itertools.product([0,1,256],[0,0x12121212],[0,4,0x104],[-1,0,0x7fffffff]):exercise(fn,disabled=disabled,handle=handle,status=status,volume=volume)
 for fn in [0x55f449,0x55f639,0x55f561,0x55f5a1,0x55f5f4,0x55f32f,0x55f38d]:exercise(fn,self=S+0x10000)
 for opened in [0,1,0xffffffff]:exercise(opened=opened)
 for status in [0,1,4,256,0x104,0xffffffff]:exercise(0x55f5a1,status=status)
 for reported in [0,1,0xffffffff,0x80000000]:exercise(0x55f5f4,reported_volume=reported)
 for loops in [0,1,0xffffffff]:exercise(loops=loops)
 for cfg in [dict(close_mutation=True),dict(open_mutation=True),dict(loop_mutation=True),dict(set_mutation=True),dict(loop_mutation=True,set_mutation=True),dict(open_mutation=True,loop_mutation=True,set_mutation=True),dict(loop_mutation=True,loop_handle=0,set_mutation=True,set_handle=0),dict(opened=0,stored_handle=0x66666666),dict(opened=0x22222222,stored_handle=0),dict(channel=-1,loop_mutation=True),dict(channel=-1073741814,handle=0)]:exercise(**cfg)
 for fn in [0x55f449,0x55f639,0x55f5a1,0x55f5f4,0x55f32f,0x55f38d]:exercise(fn,second_handle=0x55555555)
 for fn in [0x55f32f,0x55f38d]:
  exercise(fn,status_mutation=True);exercise(fn,status_mutation=True,status_handle=0);exercise(fn,stored_volume=99);exercise(fn,status=0x104)
 exercise(0x55f561,close_mutation=True)
 exercise(self=0,channel=3)
 for fn in [0x55f449,0x55f639,0x55f561,0x55f5a1,0x55f5f4]:exercise(fn,self=0,expected_fault=(0x200,4))
 exercise(0x55f32f,self=0,expected_fault=(0x154,4));exercise(0x55f38d,self=0,expected_fault=(0x14c,4))
 for fn in voice_names:
  for cfg in [dict(handle=0),dict(opened=0),dict(disabled=1),dict(audio_enabled=0),dict(voice_enabled=0),dict(status=0x104),dict(status_mutation=True,loop_mutation=True,set_mutation=True),dict(global_volume=71,loop_mutation=True)]:exercise(fn,**cfg)
 exercise(0x53d0b0,variant=255);exercise(0x53d0b0,figure_code=309);exercise(0x53bfe0,bank=108,variant=19)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared streams:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real voice compositions',flush=True)
 print('Unexecuted instructions:',[hex(a) for a in sorted(missing)],'; edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_streams.c','include/sound_streams.h','include/sound_initialization.h','include/media_bindings.h','runtime/media_bindings.c','runtime/figure_menu_actions.c','include/figure_menu_actions.h','direct_c/verify_sound_streams.py','direct_c/verify_options_startup.py','metadata/sound-stream-native.json','metadata/sound-stream-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2704,2705,2707,2708,2709,2710,2711]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_backend_endpoints=[name for name,n in backends.values()],limitations=['All seven stream methods and complete reached voice algorithm chains execute; only seven Miles backend functions are controlled. Owned instruction coverage is the seven stream bodies, not all parent voice instructions.','Native thiscall receivers and RET4/8/12 normalize to explicit cdecl C parameters; void method effects/ABI are compared without incidental EAX. Open/status/get-volume and named voice result APIs compare full EAX.','Ordered static/object stores, backend inputs, method inputs, whole static memory, private mapped object region, saved registers and corresponding low-page faults checked. Deterministic callback/read/store probes verify load phases, not concurrent behavior. Physical sound, arbitrary unmapped channels, post-overflow filenames and live game execution remain unverified.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-stream-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
