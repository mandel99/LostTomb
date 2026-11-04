"""Native/C file registration, ownership and complete ambient-bank loading.

Real game and strcmp algorithms execute. Only fopen/fclose/sprintf/strdup/free
are controlled CRT boundaries. Native and compiler SEH frames run normally;
unmasked exception delivery and post-overflow stack returns are separate work.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json

catalogue=json.loads((ROOT/'metadata/sound-sample-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/sound-sample-rea-evidence.json').read_text())['records']}
assert catalogue['native_sha256']==hashlib.sha256(original.read_bytes()).hexdigest()
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
names={0x539c60:'restore_sound_load_file_id',0x53bc80:'restore_sound_load_ambient_sample',0x53bcf0:'restore_sound_initialize_ambient_samples',0x55dd3f:'restore_sound_register_file',0x560280:'restore_owned_buffer_initialize',0x5602b0:'restore_owned_buffer_release'}
members={0x55dd3f,0x560280,0x5602b0};returns={0x539c60,0x55dd3f,0x560280}
arity={0x539c60:1,0x53bc80:3,0x53bcf0:0,0x55dd3f:2,0x560280:0,0x5602b0:0}
Q,ENT,ENT2,NAME,HEAP=S+0x1000,S+0x3000,S+0x5000,S+0x8000,S+0x18000
OBJ=0xe92bf8;GDT,TEB=0x34000000,0x34001000
backends={0x561a0a:2,0x561994:1,0x561a36:4,0x56b177:1,0x561bdc:1}
def cstring(u,p):return bytes(u.mem_read(p,2048)).split(b'\0')[0]
def text_arg(u,p):return ('scratch' if 0x30000000<=p<0x30010000 else p,cstring(u,p))
def object_arg(p):return 'temporary' if 0x30000000<=p<0x30010000 else p
for c,u,state in machines:
 u.mem_map(GDT,0x2000)
 for selector,access in [(8,0x9b),(16,0x93)]:u.mem_write(GDT+selector,struct.pack('<HHBBBB',0xffff,0,0,access,0xcf,0))
 u.mem_write(GDT+24,struct.pack('<HHBBBB',0xfff,TEB&65535,(TEB>>16)&255,0x93,0x40,(TEB>>24)&255))
 u.reg_write(UC_X86_REG_GDTR,(0,GDT,0x3f,0));u.reg_write(UC_X86_REG_CS,8)
 for reg in [UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS]:u.reg_write(reg,16)
 u.reg_write(UC_X86_REG_FS,24)
 state['compiled']=c
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000):data['writes'].append((at,size,value&((1<<(size*8))-1)))
 u.hook_add(UC_HOOK_MEM_WRITE,write,state)
 def invalid(uc,access,at,size,value,data):data['faults'].append((access,at,size));return False
 u.hook_add(UC_HOOK_MEM_INVALID,invalid,state)
 if not c:
  def visit(uc,pc,size,data):
   if not data['collect']:return
   if pc in points:hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  u.hook_add(UC_HOOK_CODE,visit,state)
 def method(uc,pc,size,data):
  if not data['collect'] or pc not in data['methods']:return
  fn=data['methods'][pc]
  if fn==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);n=arity[fn]
  if fn in members:
   self=get(uc,sp+4) if data['compiled'] else uc.reg_read(UC_X86_REG_ECX);start=8 if data['compiled'] else 4
   args=[object_arg(self)]
  else:start=4;args=[]
  raw=[get(uc,sp+start+i*4) for i in range(n)]
  if fn in [0x539c60,0x55dd3f]:raw[0]=text_arg(uc,raw[0]) if raw[0] else (0,None)
  if fn==0x53bc80:raw[1]=text_arg(uc,raw[1]) if raw[1] else (0,None)
  data['methods_seen'].append((fn,tuple(args+raw)))
 state['methods']={(sym('FUN_%08x'%fn) if c else fn):fn for fn in names}
 u.hook_add(UC_HOOK_CODE,method,state)

def setup(c,u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\x6b'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000);put(u,TEB,0xfeedbabe)
 state.update(collect=False,writes=[],events=[],methods_seen=[],faults=[],counts={},fn=fn,cfg=cfg,bump=HEAP)
 state['hooks'].clear();state['calls'].clear()
 self=cfg.get('self',Q if fn in members else OBJ);state['self']=self
 filename=cfg.get('filename',b'new.wav');name=cfg.get('name_pointer',NAME+cfg.get('alignment',0));u.mem_write(NAME,bytes(4096))
 if name and name>=NAME and name<NAME+4096:u.mem_write(name,filename+b'\0')
 entries=cfg.get('entries',ENT);rows=cfg.get('names',[b'cached.wav',None,b'other.wav'])
 for table in {self,OBJ}:
  if table:
   u.mem_write(table,b'\xa5'*32)
   for i,value in enumerate([cfg.get('capacity',100),cfg.get('count',len(rows)),entries]):put(u,table+i*4,value)
 for at in [ENT,ENT2]:u.mem_write(at-64,bytes(2048))
 for i,text in enumerate(rows):
  p=S+0xa000+i*128
  if text is not None:u.mem_write(p+cfg.get('entry_alignment',0),text+b'\0');p+=cfg.get('entry_alignment',0)
  else:p=0
  put(u,ENT+i*8,p);put(u,ENT+i*8+4,0xaabb0000+i)
  put(u,ENT2+i*8,p);put(u,ENT2+i*8+4,0xaabb0000+i)
 if cfg.get('entry_alias_self') and self:
  # A cached negative index can alias the table's fields. Subsequent entry
  # stores must use the fresh pointer, not a cached C pointer.
  put(u,self+8,self+16);put(u,self+12,ENT2)
 if fn in [0x560280,0x5602b0] and self:
  put(u,self,cfg.get('pointer',HEAP));put(u,self+4,cfg.get('owned',1))
 byte(u,0xe38e62,cfg.get('drive',ord('D')))
 u.mem_write(0xf5454c,b'\xa5'*400)
 bank=cfg.get('bank',0);row=(0xf54550+(bank&0xffffffff)*48)&0xffffffff
 if not cfg.get('unmapped_bank'):
  put(u,row+40,cfg.get('default_id',0xabcdef01));put(u,row+44,cfg.get('variants',0))
 def endpoint(token):
  def action(uc,args):
   state['counts'][token]=state['counts'].get(token,0)+1;number=state['counts'][token]
   if token==0x561a0a:
    path=text_arg(uc,args[0]) if args[0] else (0,None);mode=text_arg(uc,args[1]);policy=cfg.get('open_policy','primary')
    if cfg.get('open_results') is not None:value=cfg['open_results'][min(number-1,len(cfg['open_results'])-1)]
    else:
     fallback=path[1] is not None and len(path[1])>=3 and path[1][1:3]==b':\\'
     okay=(policy=='primary' or policy=='fallback' and fallback or policy=='mixed' and (sum(path[1] or b'')%3==0 if not fallback else sum(path[1] or b'')%3!=0))
     value=0x11110000+number*16 if okay else 0
    state['events'].append((token,path,mode,value))
    if cfg.get('open_bank_count') is not None and number==cfg.get('open_mutation_call',1):put(uc,row+44,cfg['open_bank_count'])
    return value
   if token==0x561a36:
    destination,format,drive,source=args;text=cstring(uc,source);fmt=cstring(uc,format);assert fmt==b'%c:\\%s',fmt
    output=bytes([drive&255])+b':\\'+text;uc.mem_write(destination,output+b'\0')
    state['events'].append((token,fmt,drive&255,text,output));return len(output)
   if token==0x56b177:
    text=cstring(uc,args[0]) if args[0] else None
    failed=cfg.get('fail_duplicate') or number==cfg.get('fail_duplicate_at',-1)
    value=0 if failed else state['bump'];state['bump']+=1024
    if value:uc.mem_write(value,(text or b'')+b'\0')
    state['events'].append((token,text,value))
    if cfg.get('duplicate_count') is not None:put(uc,self+4,cfg['duplicate_count'])
    if cfg.get('duplicate_entries') is not None:put(uc,self+8,cfg['duplicate_entries'])
    return value
   state['events'].append((token,args))
   if token==0x561bdc and cfg.get('free_mutation'):
    put(uc,self,0x11223344);put(uc,self+4,0x55667788)
   if token==0x561994 and cfg.get('close_bank_count') is not None:put(uc,row+44,cfg['close_bank_count'])
   return cfg.get('close_result',0) if token==0x561994 else 0
  return action
 for token,n in backends.items():hook(c,state,token,n,endpoint(token))
 if fn==0x539c60:return [name]
 if fn==0x53bc80:return [bank,name,cfg.get('mode',1)]
 if fn==0x55dd3f:return [name,cfg.get('copy',0)]
 return []

def invoke(c,u,state,fn,args,route):
 self=state['self'];argv=([self]+args) if c and fn in members else args
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 for reg,value in zip(registers,[0xa5b6c7d8,self,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 try:u.emu_start(pc,STOP,count=3000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(8 if not c and fn==0x55dd3f else 0)
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 assert get(u,TEB)==0xfeedbabe,('exception chain not restored',hex(fn),route,hex(get(u,TEB)))
 return u.reg_read(UC_X86_REG_EAX) if fn in returns else None

def exercise(fn=0x55dd3f,**cfg):
 global checked,invocations,compositions
 expected=None
 for c,u,state in machines:
  for route in (['native'] if not c else ['named','canonical']):
   args=setup(c,u,state,fn,cfg);previous[0]=None;state['collect']=True;invocations+=1
   result=invoke(c,u,state,fn,args,route);state['collect']=False
   if isinstance(result,tuple):assert state['faults'],('unexpected execution fault',hex(fn),cfg,route,hex(u.reg_read(UC_X86_REG_EIP)))
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   elif state['faults']:raise AssertionError(('unexpected memory fault',hex(fn),cfg,route,state['faults']))
   current=(result,tuple(state['writes']),tuple(state['events']),tuple(state['methods_seen']),snapshot(u),bytes(u.mem_read(S,0x50000)),tuple(state['faults']))
   if expected is None:expected=current
   elif current!=expected:
    diffs=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:4],current[1:4])) if x!=y]
    memory=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[4],current[4]),(S,expected[5],current[5])]]
    raise AssertionError((hex(fn),cfg,route,'result',expected[0],current[0],'events',diffs,'memory',memory,'faults',expected[-1],current[-1]))
 checked+=1
 if expected[3]:compositions+=1
 if checked%25==0:print('progress',checked,'scenarios',flush=True)

if __name__=='__main__':
 for fn in names:exercise(fn)
 exercise(0x539c60,open_policy='fallback');exercise(0x53bc80,open_policy='fallback')
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for fn in [0x560280,0x5602b0]:
  for pointer,ownership in itertools.product([0,HEAP,0xffffffff],[0,1,256,0xffffffff]):exercise(fn,pointer=pointer,owned=ownership)
  exercise(fn,self=0,expected_fault=(4,4))
 exercise(0x5602b0,free_mutation=True)
 for capacity,count,entries,copy in itertools.product([-1,0,3,4],[ -2,-1,0,1,3,4],[0,ENT],[0,1,0xffffffff]):exercise(capacity=capacity,count=count,entries=entries,copy=copy)
 for name in [b'cached.wav',b'other.wav',b'Cached.wav',b'',b'new.wav']:
  for first,second in itertools.product(range(4),range(4)):exercise(filename=name,alignment=first,entry_alignment=second)
 for raw in [bytes([v])+b'file.wav' for v in [127,128,129,254,255]]:
  exercise(filename=raw,names=[raw],count=1);exercise(filename=raw,names=[raw.lower()],count=1,copy=1)
 for copy in [0,1,2,0xffffffff]:
  exercise(copy=copy,fail_duplicate=True);exercise(copy=copy,name_pointer=0,names=[],count=0)
  for count in [0,1,100,0xffffffff]:exercise(copy=copy,duplicate_count=count)
  exercise(copy=copy,duplicate_entries=ENT2)
 exercise(count=-1,entry_alias_self=True,copy=0)
 exercise(count=-2,entry_alias_self=True,copy=0)
 exercise(self=0,expected_fault=(4,4))
 exercise(name_pointer=0,names=[b'x.wav'],count=1,expected_fault=(0,1))
 for policy,capacity in itertools.product(['primary','fallback','missing'],[0,1,100]):
  for drive in [0,ord('D'),255]:exercise(0x539c60,open_policy=policy,capacity=capacity,drive=drive)
 for cfg in [dict(open_policy='fallback',fail_duplicate=True),dict(close_result=0xffffffff),dict(open_results=[0,0]),dict(open_results=[0,0x22222222]),dict(open_policy='fallback',duplicate_count=7),dict(open_policy='fallback',duplicate_entries=ENT2)]:exercise(0x539c60,**cfg)
 for bank,mode,variants in itertools.product([-1,0,5,6,0x10000000],[-1,0,1,2],[-1,0,9,10,0x7fffffff]):exercise(0x53bc80,bank=bank,mode=mode,variants=variants)
 for count in [-2,-1,0,9,10,11,12,0x7fffffff,0x80000000]:
  exercise(0x53bc80,open_bank_count=count);exercise(0x53bc80,close_bank_count=count)
 for cfg in [dict(open_policy='missing'),dict(open_policy='fallback'),dict(open_policy='fallback',fail_duplicate=True),dict(mode=2,name_pointer=0)]:exercise(0x53bc80,**cfg)
 for policy,capacity in itertools.product(['primary','fallback','missing','mixed'],[0,1,20,100]):exercise(0x53bcf0,open_policy=policy,capacity=capacity,names=[],count=0)
 exercise(0x53bcf0,open_policy='fallback',fail_duplicate_at=4,names=[],count=0)
 missing=points-hits;missing_edges={(a,b) for a,targets in branches.items() for b in targets if (a,b) not in edges}
 print('Compared samples:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges,(missing,missing_edges)
 sources=['CMakeLists.txt','runtime/sound_samples.c','runtime/sound_file_lookup.c','runtime/owned_buffer.c','include/sound_samples.h','include/sound_file_lookup.h','include/owned_buffer.h','include/byte_string_compare.h','include/sound_initialization.h','include/legacy_format.h','include/host_allocator.h','direct_c/verify_sound_samples.py','direct_c/verify_options_startup.py','metadata/sound-sample-native.json','metadata/sound-sample-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2483,2499,2500,2686,2725,2726,2848]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,controlled_algorithm_endpoints=[],controlled_crt_endpoints=[hex(a) for a in backends],limitations=['Six complete owned bodies; actual loaders, file registration, owned-buffer helpers and case-sensitive strcmp execute. Only fopen/fclose/sprintf/strdup/free are controlled.','Normal-return ABI and restored FS exception chain are checked for both native and compiler-generated SEH. Handler addresses/frame layouts differ; arbitrary host SEH dispatch/unwind and corrupted caller stack returns are not established.','All original static bytes, guarded private table/heap bytes, ordered stores, meaningful child/CRT inputs, consumed full EAX and matching explicit faults are compared. Temporary stack addresses are normalized by role. Physical filesystem/media and concurrency remain unverified.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/sound-sample-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
