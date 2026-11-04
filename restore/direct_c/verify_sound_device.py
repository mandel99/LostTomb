"""Device/boot/global-constructor native/C oracle; actual game children run.

Controls only OS, Miles, heap and CRT services. Registration targets are
normalized by callback role; original raw CRT fragments retain byte evidence.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/sound-device-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-device-rea-evidence.json').read_text())['records']}
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
names={0x55e6c4:'restore_sound_initialize_digital',0x55e92b:'restore_sound_shutdown',0x5394f0:'restore_sound_boot',0x55e38b:'restore_sound_construct',0x539550:'restore_music_construct',0x4cdf70:'restore_sound_global_initialize',0x4cdf80:'restore_sound_global_construct',0x4cdf90:'restore_sound_global_register_cleanup',0x4cdfa0:'restore_sound_global_cleanup',0x4cdfb0:'restore_music_global_initialize',0x4cdfc0:'restore_music_global_construct',0x4cdfd0:'restore_music_global_register_cleanup',0x4cdfe0:'restore_music_global_cleanup'}
arity={a:0 for a in names};members={0x55e6c4,0x55e92b,0x55e38b,0x539550};returns=set(members)
children={0x55dea2:1,0x55e5d2:1,0x55dbc0:0,0x55dbf8:0,0x55dc18:0,0x55dc63:1,0x55dd3f:2,0x55e58a:0,0x539a40:1,0x539250:0,0x539730:1,0x539a50:2,0x560280:0,0x5602b0:0,0x51f780:0,0x539cf0:3,0x539d60:0,0x53bc80:3,0x53bcf0:0,0x53af70:2,0x53af90:0,0x53b100:3,0x53b130:0,0x539c60:1}
arity.update(children);members.update({0x55dea2,0x55e5d2,0x55dbc0,0x55dbf8,0x55dc18,0x55dc63,0x55dd3f,0x55e58a,0x539a40,0x539250,0x539730,0x539a50,0x560280,0x5602b0})
OBJ,MUSIC,Q,MQ,HANDLES,AUX,SAMPLES,CACHE,HANDLES3D,CACHE3D,CACHE2=0xe92bf8,0xe39180,S+0x1000,0x1100000,S+0x10000,S+0x8000,S+0x20000,S+0x30000,S+0x18000,S+0x32000,S+0x35000
HEAP,HEAP_SIZE,GDT,TEB=0x36000000,0x400000,0x34000000,0x34001000
media={0x56f2dc:('_AIL_startup_0',0),0x56f2e0:('_AIL_set_preference_8',2),0x56f2e4:('_AIL_waveOutOpen_16',4),0x56f2f4:('_AIL_allocate_sample_handle_4',1),0x56f2f0:('_AIL_release_sample_handle_4',1),0x56f2cc:('_AIL_end_sample_4',1),0x56f2c8:('_AIL_end_3D_sample_4',1),0x56f308:('_AIL_mem_free_lock_4',1),0x56f2d0:('_AIL_close_stream_4',1),0x56f2e8:('_AIL_release_3D_sample_handle_4',1),0x56f2d4:('_AIL_close_3D_provider_4',1),0x56f2d8:('_AIL_shutdown_0',0)}
crt={0x561a1d:1,0x561a2b:1,0x561bdc:1,0x563240:3,0x561a0a:2,0x561994:1,0x561e4d:3,0x56b177:1,0x561a36:4}

def cstring(u,p):return None if not p else bytes(u.mem_read(p,2048)).split(b'\0')[0]
def object_arg(p):return 'stack-object' if 0x30000000<=p<0x30010000 else p
def path_arg(u,p):return ('scratch' if 0x30000000<=p<0x30010000 else p,cstring(u,p))
def maybe_sym(name):
 try:return sym(name)
 except AssertionError:return None
for c,u,state in machines:
 state.update(compiled=c,collect=False);u.mem_map(HEAP,HEAP_SIZE);u.mem_map(GDT,0x2000)
 for selector,access in [(8,0x9b),(16,0x93)]:u.mem_write(GDT+selector,struct.pack('<HHBBBB',0xffff,0,0,access,0xcf,0))
 u.mem_write(GDT+24,struct.pack('<HHBBBB',0xfff,TEB&65535,(TEB>>16)&255,0x93,0x40,(TEB>>24)&255))
 u.reg_write(UC_X86_REG_GDTR,(0,GDT,0x3f,0));u.reg_write(UC_X86_REG_CS,8)
 for reg in [UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS]:u.reg_write(reg,16)
 u.reg_write(UC_X86_REG_FS,24)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000 or HEAP<=at<HEAP+HEAP_SIZE):data['writes'].append((at,size,value&((1<<(size*8))-1)))
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
  sp=uc.reg_read(UC_X86_REG_ESP);n=arity[fn]
  if fn in members:
   self=get(uc,sp+4) if data['compiled'] else uc.reg_read(UC_X86_REG_ECX);start=8 if data['compiled'] else 4;args=[object_arg(self)]
  else:start=4;args=[]
  values=[get(uc,sp+start+i*4) for i in range(n)]
  if fn in [0x539c60,0x55dd3f]:values[0]=path_arg(uc,values[0])
  if fn in [0x539cf0,0x53bc80,0x53af70,0x53b100]:values[1]=path_arg(uc,values[1])
  data['methods_seen'].append((fn,tuple(args+values)))
 state['methods']={}
 for fn in arity:
  pc=maybe_sym('FUN_%08x'%fn) if c else fn
  if pc is not None:state['methods'][pc]=fn;u.hook_add(UC_HOOK_CODE,method,state,begin=pc,end=pc)
 state['callbacks']={(sym('FUN_004cdfa0') if c and maybe_sym('FUN_004cdfa0') else 0x4cdfa0):'sound',(sym('FUN_004cdfe0') if c and maybe_sym('FUN_004cdfe0') else 0x4cdfe0):'music'}

def invoke(c,u,state,fn,args=(),route='canonical',receiver=None):
 self=state['self'] if receiver is None else receiver
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 argv=[self,*args] if c and fn in members else list(args)
 for reg,value in zip(registers,[0xa5b6c7d8,self,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 try:u.emu_start(pc,STOP,count=50000000)
 except UcError:return ('fault',)
 if state['cfg'].get('stop_at_version') and state.get('version_snapshot') is not None:return ('prefix',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(arity[fn]*4 if not c and fn in members else 0)
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 assert get(u,TEB)==0xfeedbabe
 return u.reg_read(UC_X86_REG_EAX) if fn in returns else None

def setup(c,u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(HEAP,b'\xcd'*HEAP_SIZE);u.mem_write(0x30000000,b'\xcc'*0x10000);put(u,TEB,0xfeedbabe)
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],methods_seen=[],faults=[],counts={},read_counts={},read_mutations=cfg.get('read_mutations',{}),cursor=HEAP+0x10000,exits=[],version_snapshot=None)
 state['hooks'].clear();state['calls'].clear()
 self=cfg.get('self',MUSIC if fn==0x539550 else Q if fn in members else OBJ);state['self']=self
 def event(value):
  if state['collect']:state['events'].append(value)
 def mutate(token,number):
  if not state['collect']:return
  for key,nth,changes in cfg.get('mutations',[]):
   if key==token and nth==number:
    for at,value in changes:put(u,at,value)
 def number(token):state['counts'][token]=state['counts'].get(token,0)+1;return state['counts'][token]
 def heap_allocate(size):
  result=state['cursor'];state['cursor']+=(size+15)&~15
  assert state['cursor']<HEAP+HEAP_SIZE,(size,state['cursor'])
  return result
 for token,n in crt.items():
  def endpoint(uc,args,token=token):
   call=number(token)
   if token==0x561a1d:
    result=0 if call==cfg.get('fail_heap') else heap_allocate(args[0])
    if cfg.get('heap_results') is not None:result=cfg['heap_results'][min(call-1,len(cfg['heap_results'])-1)]
    event(('allocate',args[0],result));mutate(token,call);return result
   if token in [0x561a2b,0x561bdc]:event((hex(token),args));mutate(token,call);return 0
   if token==0x563240:
    destination,value,size=args;uc.mem_write(destination,bytes([value&255])*size);event(('memset',object_arg(destination),value&255,size));return destination
   if token==0x561a0a:
    path,mode=cstring(uc,args[0]),cstring(uc,args[1]);fallback=path is not None and len(path)>2 and path[1:3]==b':\\'
    if path==b'music.txt':okay=cfg.get('music_policy','missing')=='empty'
    else:okay=cfg.get('file_policy','missing')=='primary' or cfg.get('file_policy')=='fallback' and fallback
    result=0x11110000+call*16 if okay else 0
    event(('open',path,mode,result));mutate(token,call);return result
   if token==0x561994:event(('close',args));return 0
   if token==0x561e4d:event(('readline',args[1],args[2]));return 0
   if token==0x56b177:
    text=cstring(uc,args[0]);result=0 if call==cfg.get('fail_duplicate') else heap_allocate(len(text)+1)
    if result:uc.mem_write(result,text+b'\0')
    event(('duplicate',text,result));return result
   if token==0x561a36:
    destination,format,drive,source=args;fmt=cstring(uc,format);assert fmt==b'%c:\\%s',fmt
    text=bytes([drive&255])+b':\\'+cstring(uc,source);uc.mem_write(destination,text+b'\0');event(('format',fmt,drive&255,cstring(uc,source)));return len(text)
   raise AssertionError(hex(token))
  pc=sym('FUN_%08x'%token) if c else token;state['hooks'][pc]=(n,0,endpoint)
 for slot,(name,n) in media.items():
  def endpoint(uc,args,slot=slot,name=name):
   call=number(slot);observed=args
   if slot==0x56f2e4:
    observed=(args[0],args[1],args[2],args[3],bytes(uc.mem_read(args[3],16)));put(uc,args[0],cfg.get('opened',0x70123456))
   event((name,observed));mutate(slot,call)
   if slot==0x56f2e4:return cfg.get('wave_result',0)
   if slot==0x56f2f4:return 0 if call==cfg.get('fail_handle') else 0x70000000+call
   return cfg.get('media_result',0xa5b6c7d8)
  pc=sym(name) if c else S+0x4a000+(slot&0xfff)
  if not c:put(u,slot,pc)
  state['hooks'][pc]=(n,0 if c else n*4,endpoint)
 def version(uc,args):
  if cfg.get('stop_at_version'):state['version_snapshot']=bytes(uc.mem_read(OBJ,0x204));uc.emu_stop()
  if c:put(uc,args[0],cfg.get('platform',2));put(uc,args[1],cfg.get('major',10))
  else:
   assert get(uc,args[0])==0x94;put(uc,args[0]+16,cfg.get('platform',2));put(uc,args[0]+4,cfg.get('major',10))
  event(('version',0x94,cfg.get('version_result',1),cfg.get('platform',2),cfg.get('major',10)));mutate(0x56f124,number(0x56f124));return cfg.get('version_result',1)
 def debug(uc,args):event(('debug',cstring(uc,args[0])));return 0
 def register_exit(uc,args):
  role=state['callbacks'].get(args[0]);assert role is not None,(c,hex(args[0]));result=cfg.get('exit_result',0)
  event(('atexit',role,result))
  if not result:state['exits'].append(role)
  return result
 if c:
  state['hooks'][sym('restore_sound_version')]=(2,0,version);state['hooks'][sym('restore_sound_debug')]=(1,0,debug);state['hooks'][sym('atexit')]=(1,0,register_exit)
 else:
  put(u,0x56f124,S+0x4b000);state['hooks'][S+0x4b000]=(1,4,version)
  put(u,0x56f0f8,S+0x4b020);state['hooks'][S+0x4b020]=(1,4,debug);state['hooks'][0x561953]=(1,0,register_exit)
 # Preparation uses actual constructors outside the observed entry.
 u.mem_write(OBJ,bytes(0x204));u.mem_write(MUSIC,bytes(0x59a74));put(u,0x12663b0,cfg.get('counter',0x10203040))
 if fn in [0x5394f0,0x4cdf90,0x4cdfa0,0x4cdfd0,0x4cdfe0]:
  invoke(c,u,state,0x55e38b,receiver=OBJ);invoke(c,u,state,0x539550,receiver=MUSIC)
 if fn in [0x55e38b,0x539550]:
  size=0x59a74 if fn==0x539550 else 0x204
  if self:u.mem_write(self,bytes([cfg.get('poison',0xa5)])*size)
 elif fn in members or fn==0x5394f0 or fn==0x4cdfa0:
  if self:u.mem_write(self,bytes(0x204))
  if self:
   for off,value in [(0x18c,cfg.get('rate',22050)),(0x190,cfg.get('bits',16)),(0x194,cfg.get('channels',2)),(0x198,cfg.get('old_count',0)),(0x19c,HANDLES),(0x1a0,AUX),(0x1a8,cfg.get('samples',SAMPLES)),(0x1ac,cfg.get('cache',CACHE)),(0x178,0x70123456),(0x164,cfg.get('provider',5)),(0x1e4,cfg.get('count3d',0)),(0x1e8,0x88112233),(0x1f0,HANDLES3D),(0x1f8,CACHE3D),(0x200,cfg.get('disabled',0))]:put(u,self+off,value)
   if not cfg.get('old_pool',False):
    for off in [0x19c,0x1a0,0x1a8,0x1ac]:put(u,self+off,0)
   for i,value in enumerate(cfg.get('streams',[0,0,0])):put(u,self+0x1d8+i*4,value)
 for i in range(16):
  put(u,HANDLES+i*4,0x71000000+i);put(u,HANDLES3D+i*4,0x72000000+i)
  for cache in [CACHE,CACHE3D,CACHE2]:
   put(u,cache+i*12,0x88880000+i);put(u,cache+i*12+4,cfg.get('cache_data',0x39000000+i));put(u,cache+i*12+8,17)
 put(u,0xeaac28,0);put(u,0xea1894,cfg.get('population',0));byte(u,0xe38ea8,cfg.get('category',0));byte(u,0xe38e62,cfg.get('drive',ord('D')))
 for at,value in cfg.get('initial_stores',[]):put(u,at,value)
 state.update(counts={},events=[],methods_seen=[],writes=[],faults=[],exits=[],cursor=HEAP+0x10000);previous[0]=None
 return []

def exercise(fn=0x55e6c4,**cfg):
 global checked,invocations,compositions
 expected=None
 for c,u,state in machines:
  for route in ['native'] if not c else ['named','canonical']:
   args=setup(c,u,state,fn,cfg);state['collect']=True;invocations+=1
   result=invoke(c,u,state,fn,args,route)
   if cfg.get('initialize_music') and not isinstance(result,tuple):invoke(c,u,state,0x4cdfb0,route='canonical')
   if cfg.get('exit_callbacks') and not isinstance(result,tuple):
    for role in reversed(state['exits'].copy()):invoke(c,u,state,0x4cdfa0 if role=='sound' else 0x4cdfe0,route='canonical')
   state['collect']=False
   if isinstance(result,tuple):assert state['faults'],(hex(fn),cfg,route,'execution fault')
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   elif state['faults']:raise AssertionError((hex(fn),cfg,route,state['faults']))
   if not c:
    for slot in [*media,0x56f124,0x56f0f8]:u.mem_write(slot,seed[slot-0x560000:slot-0x560000+4])
   current=(result,tuple(state['writes']),tuple(state['events']),tuple(state['methods_seen']),snapshot(u),bytes(u.mem_read(S,0x50000)),bytes(u.mem_read(HEAP,HEAP_SIZE)),tuple(state['faults']),tuple(state['exits']))
   if expected is None:expected=current
   elif current!=expected:
    diff=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:4],current[1:4])) if x!=y]
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[4],current[4]),(S,expected[5],current[5]),(HEAP,expected[6],current[6])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'events',diff,'memory',memory,'faults',expected[-2],current[-2]))
   for token,n in cfg.get('require_calls',{}).items():assert state['counts'].get(token,0)>=n,(cfg,route,token,state['counts'])
 checked+=1
 if expected[3]:compositions+=1
 print('progress',checked,hex(fn),flush=True) if checked%10==0 else None

if __name__=='__main__':
 for fn in [0x55e6c4,0x55e92b,0x55e38b,0x539550,*[a for a in names if 0x4cdf70<=a<=0x4cdfe0]]:exercise(fn)
 exercise(0x5394f0,version_result=0)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for platform,major,result in itertools.product([0,1,2,3,0x80000000],[0,4,5,10,0x80000000,0xffffffff],[0,1,256]):exercise(platform=platform,major=major,version_result=result)
 for wave in [1,256,0x80000000,0xffffffff]:exercise(wave_result=wave,require_calls={0x56f2e4:1})
 for rate,bits,channels in itertools.product([0,22050,0x7fffffff,0xffffffff],[0,7,8,16,-1,0x80000000],[0,1,2,6,0xffffffff]):exercise(rate=rate,bits=bits,channels=channels)
 for failure in range(1,5):exercise(fail_heap=failure)
 for failure in range(1,6):exercise(fail_handle=failure)
 for provider,count,streams,data in itertools.product([0,5,0x80000000],[-1,0,1,3],[[0,0,0],[11,22,33],[11,0,33]],[0,0xffffffff]):exercise(0x55e92b,old_pool=True,old_count=count,count3d=count,provider=provider,streams=streams,cache_data=data)
 for disabled in [1,256,0xffffffff]:exercise(0x55e92b,disabled=disabled,old_pool=True,old_count=2,count3d=2,provider=0,streams=[1,2,3])
 for fn in [0x55e38b,0x539550]:
  for poison in [0,0x5a,0xff]:exercise(fn,poison=poison,self=MQ if fn==0x539550 else Q)
 for fn in [0x4cdf70,0x4cdf90,0x4cdfb0,0x4cdfd0]:exercise(fn,exit_result=0xffffffff)
 exercise(0x4cdf70,exit_callbacks=True,initialize_music=True)
 for cfg in [dict(version_result=0),dict(platform=2,major=4),dict(wave_result=1),dict(fail_heap=1),dict(fail_heap=4),dict(fail_handle=2),dict(music_policy='empty')]:exercise(0x5394f0,**cfg)
 exercise(0x5394f0,version_result=0,old_pool=True,old_count=2,count3d=2,provider=0,streams=[1,2,3])
 exercise(0x5394f0,file_policy='primary',music_policy='empty')
 exercise(0x5394f0,file_policy='fallback',music_policy='missing')
 for slot,changes in [(0x56f2e0,[(Q+0x190,24),(Q+0x18c,44100)]),(0x56f2e4,[(Q+0x18c,0x80000000)]),(0x56f2f4,[(Q+0x1ac,CACHE2)])]:exercise(mutations=[(slot,1,changes)])
 for slot,changes in [(0x56f2f0,[(Q+0x198,1)]),(0x56f308,[(Q+0x1ac,CACHE2)]),(0x56f2d0,[(Q+0x1d8,0x99999999),(Q+0x1dc,42)]),(0x56f2e8,[(Q+0x1f8,CACHE2)]),(0x56f2d4,[(Q+0x1ac,0)]),(0x56f2d8,[(Q+0x1ac,0),(Q+0x19c,0)]),(0x561a2b,[(Q+0x1a8,0)])]:exercise(0x55e92b,old_pool=True,old_count=2,count3d=2,provider=0,streams=[1,2,3],mutations=[(slot,1,changes)])
 exercise(heap_results=[HEAP+0x10000,HEAP+0x11000,HEAP+0x12000,Q+0x1ac],expected_fault=(3,4))
 exercise(read_mutations={Q+0x190:[(2,[(Q+0x190,24)])]})
 exercise(0x55e92b,old_pool=True,old_count=2,streams=[1,2,3],read_mutations={Q+0x1d8:[(2,[(Q+0x1d8,0x12345678)])]})
 exercise(0x55e6c4,self=0,expected_fault=(0x17c,2));exercise(0x55e92b,self=0,expected_fault=(0x200,4));exercise(0x55e38b,self=0,expected_fault=(8,4))
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared device:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 startup=json.loads((ROOT/'metadata/audio-global-startup-audit.json').read_text())
 assert startup['passed'] and target_sha in [item['compiled_C_sha256'] for item in startup['variants'].values()]
 for source,sha in startup['source_hashes'].items():assert hashlib.sha256((ROOT/source).read_bytes()).hexdigest()==sha,source
 sources=['CMakeLists.txt','runtime/sound_device.c','runtime/sound_shutdown.c','runtime/sound_boot.c','runtime/sound_constructor.c','runtime/music_constructor.c','runtime/audio_global_lifecycle.c','runtime/sound_initialization.c','runtime/music_manager.c','runtime/standalone_link_check.c','include/sound_device.h','include/audio_global_lifecycle.h','include/sound_initialization.h','include/music_manager.h','direct_c/verify_sound_device.py','direct_c/verify_options_startup.py','metadata/sound-device-native.json','metadata/sound-device-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2474,2475,2473,2477,2479,2480,2689,2692,2693,1735,1736,1737,1738]]+list(startup['source_hashes'])+['metadata/audio-global-startup-audit.json']
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_crt_endpoints=[hex(a) for a in [*crt,0x561953]],controlled_media_endpoints=[name for name,n in media.values()],controlled_os_endpoints=['GetVersionExA/version adapter','OutputDebugStringA/debug adapter'],limitations=['Thirteen owned entries (including four explicit raw CRT/callback fragments) execute actual constructors, device/pool/reset/shutdown/file/array/music and bank children. Only OS, Miles and declared CRT services are controlled.','Whole boot runs all bank initializers, actual file registry and music parser with missing/empty music.txt; other parser contents, physical audio/OS/allocator behavior, concurrency and native exception dispatch are not certified.','All static/private/heap bytes, ordered CPU stores, meaningful child/backend inputs, full meaningful EAX, normal-return ABI and FS restoration are compared. Void incidental EAX, uninitialized OS scratch padding and fault register/PC state are excluded.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-device-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
