"""Five native music/tokenizer bodies versus named and canonical C, offline.

CD fallback is explicitly disabled by the user and checked separately as policy;
its ten native instructions are excluded, not claimed equivalent or dead.
"""
from verify_options_startup import *
from unicorn import UC_HOOK_MEM_WRITE,UC_HOOK_MEM_READ,UC_HOOK_MEM_INVALID,UcError
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import json
catalogue=json.loads((ROOT/'metadata/music-configuration-native.json').read_text())
evidence={r['evidence_id']:r for r in json.loads((ROOT/'metadata/music-configuration-rea-evidence.json').read_text())['records']}
owned={};branches={};hits=set();edges=set();previous=[None]
checked=invocations=compositions=policy_checked=0
for f in catalogue['functions']:
 a=int(f['address'],16);raw=bytes.fromhex(f['bytes_hex']);ev=evidence[f['bytes_evidence_id']]
 assert ev['authority']=='shipped-artifact' and ev['subject']['digest']['sha256']==catalogue['native_sha256']
 assert ev['normalized_result']['complete'] and ev['normalized_result']['bytes_hex']==f['bytes_hex']
 assert raw==bytes(images[0].data[a-images[0].base:a-images[0].base+len(raw)])
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,a));owned[a]={i.address for i in ins};assert len(ins)==f['instruction_count']
 for i in ins:
  if i.mnemonic.startswith('j') and i.mnemonic!='jmp':branches[i.address]={int(i.op_str,16),i.address+i.size}
points=set.union(*owned.values());disabled={int(a,16) for a in catalogue['user_disabled_CD_instruction_addresses']}
disabled_edges={(int(a,16),int(b,16)) for a,b in catalogue['user_disabled_CD_conditional_edges']}
constant_edges={(0x53930e,0x539324),(0x539388,0x53939e),(0x53939c,0x53937e)}
# The two inline compares have immutable AMBIENT\0 / A\0 right operands.
# AMBIENT has no first-byte NUL in any compared pair; A has a second-byte NUL.
assert bytes(images[0].data[0x5f9080-images[0].base:0x5f908c-images[0].base])==b'A\0\0\0AMBIENT\0'
names={0x539250:'restore_music_load_configuration',0x5395e0:'restore_music_add_track',0x539730:'restore_music_select_population_range',0x539a50:'restore_music_set_range',0x561db1:'restore_tokenize_byte_set'}
arity={0x539250:0,0x5395e0:6,0x539730:1,0x539a50:2,0x561db1:2}
members={0x539250,0x5395e0,0x539730,0x539a50};returns={0x5395e0,0x561db1}
MUSIC,MQ,TEXT,DELIMS=0xe39180,0x1100000,S+0x1000,S+0x8000
def cstring(u,p):
 if not p:return None
 out=bytearray()
 while True:
  value=bytes(u.mem_read(p+len(out),1))[0]
  if not value:return bytes(out)
  out.append(value);assert len(out)<20000
def ptr(state,p):
 if 0x30000000<=p<0x30010000:
  return ('line',p-state['line']) if state.get('line') is not None else ('stack',)
 return p
def memory(u,state):
 data=bytearray(snapshot(u));cursor=get(u,0x12663c8);struct.pack_into('<I',data,0x12663c8-0x560000,0)
 return bytes(data),ptr(state,cursor),bytes(u.mem_read(S,0x50000))
for c,u,state in machines:
 state.update(compiled=c,collect=False,line=None)
 def write(uc,access,at,size,value,data):
  if data['collect'] and (0x560000<=at<0x126d000 or S<=at<S+0x50000):
   data['writes'].append((at,size,ptr(data,value) if at==0x12663c8 else value&((1<<(size*8))-1)))
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
   if not data['collect'] or data['policy']:return
   hits.add(pc)
   if previous[0] in branches:edges.add((previous[0],pc))
   previous[0]=pc
  for p in owned.values():u.hook_add(UC_HOOK_CODE,visit,state,begin=min(p),end=max(p))
 def method(uc,pc,size,data):
  if not data['collect']:return
  fn=data['methods'][pc]
  if fn==data['fn']:return
  sp=uc.reg_read(UC_X86_REG_ESP);start=4;args=[]
  if fn in members:
   args=[get(uc,sp+4) if data['compiled'] else uc.reg_read(UC_X86_REG_ECX)];start=8 if data['compiled'] else 4
  values=[get(uc,sp+start+i*4) for i in range(arity[fn])]
  if fn==0x561db1:values=[ptr(data,values[0]),cstring(uc,values[1])]
  if fn==0x5395e0:values[0]=(ptr(data,values[0]),cstring(uc,values[0]))
  data['methods_seen'].append((fn,tuple(args+values)))
 state['methods']={}
 for fn in names:
  pc=sym('FUN_%08x'%fn) if c else fn;state['methods'][pc]=fn;u.hook_add(UC_HOOK_CODE,method,state,begin=pc,end=pc)
def setup(c,u,state,fn,cfg):
 u.mem_write(0x560000,seed);u.mem_write(S,b'\xa5'*0x50000);u.mem_write(0x30000000,b'\xcc'*0x10000)
 state.update(collect=False,fn=fn,cfg=cfg,writes=[],events=[],methods_seen=[],faults=[],read_counts={},read_mutations=cfg.get('read_mutations',{}),line=None,position=0,data=cfg.get('data',b'').replace(b'\r\n',b'\n'),policy=cfg.get('policy',False))
 state['hooks'].clear();state['calls'].clear()
 self=cfg.get('self',MQ);state['self']=self
 u.mem_write(MUSIC,bytes(0x5d000));u.mem_write(MQ-0x1b10,bytes(0x62000))
 put(u,0x12663c8,cfg.get('cursor',0));u.mem_write(TEXT,cfg.get('text',b'song.mp3')+b'\0');u.mem_write(DELIMS,cfg.get('delimiters',b' ,\t\n')+b'\0')
 byte(u,0xe38ea8,cfg.get('category',0));put(u,0xeaac28,cfg.get('profile',0));byte(u,0xe38e62,cfg.get('drive',ord('D')))
 row=(self+(cfg.get('category',0)&0xffffffff)*0x1b10)&0xffffffff
 if fn in [0x5395e0,0x539730] and self and cfg.get('prepare_fields',True):
  put(u,row+0x1b08,cfg.get('count',0));put(u,row+0x1b0c,cfg.get('count',0))
 if fn==0x539730 and cfg.get('prepare_fields',True):
  put(u,(0xea1894+(cfg.get('profile',0)&0xffffffff)*0x49d8)&0xffffffff,cfg.get('population',150))
  for i,(minimum,maximum) in enumerate(cfg.get('tracks',[(0,199),(100,299),(300,999)])):
   put(u,row+i*112+104,minimum);put(u,row+i*112+108,maximum)
  cur=cfg.get('current',0)&0xffffffff
  if cfg.get('current_bounds') is not None:
   p=(row+cur*112)&0xffffffff;put(u,p+104,cfg['current_bounds'][0]);put(u,p+108,cfg['current_bounds'][1])
  for off,v in [(0x59a60,cur),(0x59a54,cfg.get('first',7)),(0x59a58,cfg.get('last',9)),(0x59a5c,cfg.get('previous',4)),(0x59a6c,cfg.get('waiting',0)),(0x59a64,42)]:put(u,self+off,v)
 args={0x539250:[],0x5395e0:[cfg.get('source',TEXT),cfg.get('category',0),cfg.get('delay',7),cfg.get('minimum',20),cfg.get('maximum',200),cfg.get('ambient',0)],0x539730:[cfg.get('force',0)],0x539a50:[cfg.get('first',7),cfg.get('last',9)],0x561db1:[cfg.get('source',TEXT),cfg.get('delim_pointer',DELIMS)]}[fn]
 if cfg.get('source_text') is not None:u.mem_write(args[0],cfg['source_text']+b'\0')
 for at,value in cfg.get('initial_stores',[]):put(u,at,value)
 def endpoint(uc,args,token):
  if token==0x561a0a:
   path,mode=cstring(uc,args[0]),cstring(uc,args[1]);result=(0x11110000 if cfg.get('configuration_open',True) else 0) if path==b'music.txt' else (0x11110004 if cfg.get('file_found',True) else 0)
   state['events'].append(('open',path,mode,result));return result
  if token==0x561994:state['events'].append(('close',args[0]));return 0
  if token==0x561e4d:
   destination,size,stream=args;assert size==200 and stream==0x11110000
   state['line']=destination;start=state['position'];data=state['data'];end=min(start+199,len(data));newline=data.find(b'\n',start,end)
   if newline>=0:end=newline+1
   value=0
   if start<len(data):uc.mem_write(destination,data[start:end]+b'\0');state['position']=end;value=destination
   state['events'].append(('read',size,stream,data[start:end] if value else None));return value
  if token==0x561a36:
   destination,format,a,b=args;fmt=cstring(uc,format)
   if fmt==b'audio\\music\\%s':text=b'audio\\music\\'+cstring(uc,a);observed=(fmt,cstring(uc,a))
   else:assert fmt==b'%c:\\audio\\music\\%s';text=bytes([a&255])+b':\\audio\\music\\'+cstring(uc,b);observed=(fmt,a&255,cstring(uc,b))
   state['events'].append(('format',observed));uc.mem_write(destination,text+b'\0');return len(text)
  if token==0x562dcb:
   source,end,radix=args;assert radix==10;text=cstring(uc,source);match=re.match(rb'[\t\n\v\f\r ]*([+-]?\d+)',text)
   value=max(-2147483648,min(2147483647,int(match[1]))) if match else 0
   if end:put(uc,end,source+(match.end() if match else 0))
   state['events'].append(('decimal',text,bool(end),radix,value));return value
  raise AssertionError(hex(token))
 for token,n in [(0x561a0a,2),(0x561994,1),(0x561e4d,3),(0x561a36,4),(0x562dcb,3)]:
  pc=sym('strtol' if token==0x562dcb else 'FUN_%08x'%token) if c else token
  state['hooks'][pc]=(n,0,lambda uc,args,token=token:endpoint(uc,args,token))
 previous[0]=None;return args
def invoke(c,u,state,fn,args,route):
 self=state['self'];argv=[self,*args] if c and fn in members else args
 for reg,value in zip(registers,[0xa5b6c7d8,self,0x33333333,saved[0],SP,*saved[1:]]):u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_EFLAGS,2);put(u,SP,STOP)
 for i,value in enumerate(argv):put(u,SP+4+i*4,value)
 pc=fn if not c else sym(names[fn] if route=='named' else 'FUN_%08x'%fn)
 try:u.emu_start(pc,STOP,count=10000000)
 except UcError:return ('fault',)
 assert u.reg_read(UC_X86_REG_EIP)==STOP,(hex(fn),route,hex(u.reg_read(UC_X86_REG_EIP)))
 assert u.reg_read(UC_X86_REG_ESP)==SP+4+(len(args)*4 if not c and fn in members else 0)
 assert [u.reg_read(registers[i]) for i in [3,5,6,7]]==saved
 return u.reg_read(UC_X86_REG_EAX) if fn in returns else None
def exercise(fn=0x5395e0,**cfg):
 global checked,invocations,compositions,policy_checked
 expected=None;policy_results=[]
 for c,u,state in machines:
  for route in ['native'] if not c else ['named','canonical']:
   args=setup(c,u,state,fn,cfg);state['collect']=True;invocations+=1;result=invoke(c,u,state,fn,args,route)
   if cfg.get('continuations'):
    for text in cfg['continuations']:
     if text is not None:u.mem_write(TEXT,text+b'\0')
     result=invoke(c,u,state,fn,[TEXT if text is not None else 0,DELIMS],route)
   state['collect']=False
   if cfg.get('expected_fault'):assert state['faults'] and state['faults'][0][1:]==cfg['expected_fault'],(hex(fn),cfg,route,state['faults'])
   elif state['faults']:raise AssertionError((hex(fn),cfg,route,state['faults']))
   if cfg.get('policy'):
    path=cstring(u,MUSIC);policy_results.append((c,path))
    assert path==(b'audio\\music\\song.mp3' if c else (bytes([cfg['drive']])+b':\\audio\\music\\song.mp3').split(b'\0')[0]),(cfg,route,path)
    if not c:continue
   current=(result,tuple(state['writes']),tuple(state['events']),tuple(state['methods_seen']),memory(u,state),tuple(state['faults']),state['position'])
   if expected is None:expected=current
   elif current!=expected:
    diffs=[(i,next(((n,a,b) for n,(a,b) in enumerate(zip(x,y)) if a!=b),None),len(x),len(y)) for i,(x,y) in enumerate(zip(expected[1:4],current[1:4])) if x!=y]
    mem=[next(((hex(base+i),a,b) for i,(a,b) in enumerate(zip(x,y)) if a!=b),None) for base,x,y in [(0x560000,expected[4][0],current[4][0]),(S,expected[4][2],current[4][2])]]
    raise AssertionError((hex(fn),cfg,route,'return',expected[0],current[0],'events',diffs,'memory',mem,'cursor',expected[4][1],current[4][1],'faults',expected[-2],current[-2]))
 checked+=1
 if cfg.get('policy'):policy_checked+=1
 elif expected[3]:compositions+=1
 if checked%50==0:print('progress',checked,hex(fn),flush=True)
if __name__=='__main__':
 exercise();exercise(category=-1);exercise(category=53);exercise(count=51);exercise(count=11,ambient=1)
 exercise(0x539730,count=3,force=1);exercise(0x539a50);exercise(0x561db1,text=b' ,song,next',continuations=[None,None]);exercise(0x539250,data=b'song.mp3 M 0 7 20 200\nAMBIENT\n')
 exercise(0x539730,count=3,previous=7)
 if '--quick' in sys.argv:print('PASS quick',checked,invocations);sys.exit(0)
 for category,count,ambient in itertools.product([-1,0,52,53,54,0x7fffffff,-2147483648],[-1,0,9,10,49,50,51,60],[0,1,256]):
  exercise(category=category,count=count,ambient=ambient)
 for length in [0,1,2,3,4,5,7,8,99,100,101,111,112,120,6924]:exercise(text=b'x'*length,delay=-1,minimum=-2147483648,maximum=0x7fffffff)
 for delta in [-4,-1,0,1,4]:exercise(source=MQ+delta,source_text=b'alias sample')
 exercise(category=54,self=0,source=0);exercise(count=50,source=0);exercise(source=0,expected_fault=(0,1));exercise(self=0,expected_fault=(0x1b08,4))
 for category,profile in itertools.product([-128,-1,0,52,53,127],[0,7,8,0x80000000,0xffffffff]):
  exercise(0x539730,category=category,profile=profile,count=3,force=1)
 for count,population,force,waiting in itertools.product([-1,0,1,3,51],[-1,0,99,100,199,200,300,1000],[0,1],[0,256]):exercise(0x539730,count=count,population=population,force=force,waiting=waiting)
 for current in [-1,3,50,60,0x80000000]:exercise(0x539730,current=current,current_bounds=(0,200),count=-1,force=0)
 for first,last in itertools.product([-2147483648,-1,0,2147483647],repeat=2):exercise(0x539a50,first=first,last=last)
 exercise(0x539a50,self=0xfffa65ac,expected_fault=(0,4));exercise(0x539730,self=0xfffa65a0,prepare_fields=False,expected_fault=(0,4))
 for byte_value in range(1,256):
  v=bytes([byte_value]);exercise(0x561db1,text=v+b'a'+v+b'b',delimiters=v,continuations=[None,None])
 for text,delimiters in itertools.product([b'',b' ',b' ,\t\n',b'abc',b'a,b,c',b'\x80\xff'],[b'',b' ,\t\n',b'\x80\xff']):exercise(0x561db1,text=text,delimiters=delimiters)
 exercise(0x561db1,source=0,expected_fault=(0,1));exercise(0x561db1,delim_pointer=0,expected_fault=(0,1))
 exercise(0x539250,configuration_open=False);exercise(0x539250,data=b'')
 for name in [b'AMBIENT',b'AMB',b'AMBIEN',b'AMBIENx',b'AMBIExT',b'AMBIENTX',b'A',b'B',b'\x80name',b'\xffname']:
  exercise(0x539250,data=name+b' M 0\n')
 for type_name in [b'A',b'M',b'a',b'AA',b'B',b'\x80']:
  for parameters in [b'',b' 0',b' 0 7',b' 0 7 20',b' 0 7 20 200']:exercise(0x539250,data=b'song.mp3 '+type_name+parameters+b'\n')
 for data in [b'; comment\n\n ,\t\n\0hidden\n',b'song.mp3 M 0 -2147483649 2147483648 -9\n',b'a.mp3 M -1\nAMBIENT\nb.mp3 M 53\n',b'a.mp3 M 0 8 20 200\nb.mp3 M\n',b'a.mp3 M 0\n'*50+b'AMBIENT\n',b'a.mp3 A 0\n'*11,b'x'*95+b' M 0\n']:
  exercise(0x539250,data=data)
 exercise(0x539250,data=b'bad\n',expected_fault=(0,1));exercise(0x539250,self=0,data=b'AMBIENT\n',expected_fault=(0x1b08,4))
 exercise(0x539250,self=MQ,data=b'song.mp3 M 0\nAMBIENT\n')
 exercise(read_mutations={MQ+0x1b08:[(2,[(MQ+0x1b08,41)])]})
 exercise(0x539730,count=3,force=1,read_mutations={MQ+0x59a60:[(2,[(MQ+0x59a60,1)])]})
 exercise(0x561db1,text=b'abc,def',read_mutations={TEXT:[(2,[(TEXT,0x2c)])]})
 for drive in [0,ord('D'),255]:exercise(0x539250,data=b'song.mp3 M 0 7 20 200\n',file_found=False,drive=drive,policy=True)
 missing=points-hits-disabled;all_edges={(a,b) for a,targets in branches.items() for b in targets};missing_edges=all_edges-edges-disabled_edges-constant_edges
 print('Compared music configuration:',checked,'scenarios;',invocations,'invocations;',len(points&hits),'/',len(points),'instructions;',len(edges),'conditional outcomes;',compositions,'real compositions;',policy_checked,'explicit CD-policy cases',flush=True)
 print('Unexecuted:',[hex(a) for a in sorted(missing)],'edges:',[(hex(a),hex(b)) for a,b in sorted(missing_edges)],flush=True)
 assert not missing and not missing_edges and not hits&disabled and not edges&constant_edges,(missing,missing_edges,hits&disabled,edges&constant_edges)
 sources=['CMakeLists.txt','runtime/music_configuration.c','runtime/music_tracks.c','runtime/music_population_range.c','runtime/byte_tokenizer.c','include/music_configuration.h','include/music_manager.h','include/byte_tokenizer.h','include/legacy_format.h','direct_c/verify_music_configuration.py','direct_c/verify_options_startup.py','metadata/music-configuration-native.json','metadata/music-configuration-rea-evidence.json']+[f'src/unit_{n:05}.c' for n in [2473,2476,2477,2480,2815]]
 proof=dict(native_sha256=catalogue['native_sha256'],compiled_C_path=str(target.resolve()),compiled_C_sha256=target_sha,passed=True,checked=checked,invocations=invocations,coverage={hex(a):dict(instructions=len(p),executed=len(p&hits),user_disabled_CD=len(p&disabled)) for a,p in owned.items()},conditional_edges=len(edges),unexecuted_instructions=[],unexecuted_edges=[],source_hashes={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},actual_composition_scenarios=compositions,explicit_CD_policy_scenarios=policy_checked,user_disabled_CD_instructions=sorted(hex(a) for a in disabled),user_disabled_CD_edges=[list(map(hex,p)) for p in sorted(disabled_edges)],analytically_unreachable_edges=[list(map(hex,p)) for p in sorted(constant_edges)],controlled_algorithm_endpoints=[],controlled_crt_endpoints=['fopen','fclose','fgets','sprintf','strtol'],limitations=['Five whole native bodies are recorded. Ten CD-only fallback instructions and their incoming outcome are explicitly disabled in C at the user request; three separate actual native/C policy cases assert the authorized different result and do not contribute equivalence coverage.','Three inline string-comparison outcomes are impossible for immutable AMBIENT/A constants. All remaining owned instructions/outcomes execute actual insertion, range-setter and byte-tokenizer children.','Static/private bytes, ordered CPU writes, child/CRT inputs, meaningful EAX, full normal-return ABI, matched explicit faults and tokenizer cursor are compared. Temporary parser buffer pointers are normalized by relative role. Physical CRT/file/decimal behavior, arbitrary caller-stack aliases, unbounded huge traversal and OS exception dispatch are unproved.'])
 suffix='diagnostic' if target.stem=='restore_link_check' else 'game';(ROOT/f'metadata/music-configuration-{suffix}.json').write_text(json.dumps(proof,indent=2)+'\n')
